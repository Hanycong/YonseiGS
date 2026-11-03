// Model - 누리호 5차 위성 리포트 조립 (BEE1012 포트 15, UELYSYS 포트 10, SD1SAT 포트 20)
//
// GS_nuri5 miman_coms.cpp 의 AssembleReportPortChunk (CCSDS 길이 필드로 패킷 경계를 찾는 조립기),
// ProcessAssembledReportWire (0x0825 여러 리포트 / 0x0826 critical / 0x0827 OPS), SD1SAT 조각 재조립
// (AssembleSD1SATReportPortChunk, ParseSD1SATReportWire)을 옮긴 것.
// BEE-1000(포트 25)은 이 조립기를 쓰지 않는다 (YonseiGS_coms.cpp 의 540 바이트 조립 그대로).
// GS_nuri5 와 다른 점:
//  - ParseSD1SATReportWire: Data 는 오프셋 16 부터인데 남은 길이를 len-4 로 계산해 12 바이트 더 읽던 것을
//    len-16 으로 고침. 16 바이트(봉투 4 + CmdReport 헤더 12)보다 짧으면 해석하지 않는다.
//  - g_rpt_ops_data 는 g_report_view_mtx 아래에서 갱신한다.
//  - 포트 15/10 은 같은 조립 버퍼를 쓰므로, 조립 중 포트가 바뀌면 조립기 안에서 버린다
//    (GS_nuri5 는 수신 루프에서 25/15/10/20 모두 같은 버퍼로 검사했다).
#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"

#include <mutex>
#include <string>

#include "n5_report_assembler.h"
#include "n5_report_decoder.h"
#include "telemetry_files.h"

extern Console console;


/* Same MID on BEE-1012 and UELYSYS (rpt app is unmodified across missions,
 * apps/rpt/config/default_rpt_topicids.h topic 0x25/0x26/0x27 -> CFE_CPU1_TLM_MID_BASE
 * 0x0800 | topic). All three ride the app's own RPT downlink port (25/15/10)
 * alongside the per-command reflected Report, single-entry Report/Critical wire
 * sizes and Ops size were confirmed against apps/rpt struct layouts. */
static constexpr uint16_t RPT_REPORT_TLM_MID   = 0x0825;
static constexpr uint16_t RPT_CRITICAL_TLM_MID = 0x0826;
static constexpr uint16_t RPT_OPS_TLM_MID      = 0x0827;
static constexpr size_t   RPT_REPORT_ENTRY_SIZE   = 524; /* sizeof(RPT_Report_t), 2B trailing struct pad */
static constexpr size_t   RPT_CRITICAL_ENTRY_SIZE = 532; /* RPT_Report_t + CFE_TIME_SysTime_t(8B) */
static constexpr size_t   RPT_OPS_WIRE_SIZE       = 44;  /* 16B header + sizeof(RPT_OperationData_t)=28 */
static constexpr size_t   REPORT_WIRE_MAX_SIZE    = 21296; /* 16B header + RPT_CRITICAL_QUEUE_LEN(40)*532 */

static uint8_t g_report_wire[REPORT_WIRE_MAX_SIZE];
static size_t  g_report_off = 0;
static size_t  g_report_target = 0; /* 0 = not yet known; learned from the CCSDS length field */
static bool    g_report_collecting = false;
static int     g_report_dport = -1;

static time_t  g_report_last_chunk_time = 0;
static bool    g_report_have_last_time = false;

static uint16_t be16(const uint8_t *p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}
static uint32_t be32u(const uint8_t *p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}


/* Decode one RPT_Report_t (or RPT_Critical_t, which is RPT_Report_t + an extra
 * CFE_TIME_SysTime_t appended right after it) entry sitting at `entry_off` bytes
 * into the body that follows the shared 16-byte CCSDS-like header. Used both for
 * a lone reflected-command Report (entry_off=0) and for each slot inside an
 * RPT_MultipleReportTlm_t / RPT_MultipleCriticalTlm_t replay batch. */
static bool ParseReportEntryAt(const uint8_t *buf, size_t entry_off, bool is_critical, Report &out) {
    if (!buf) return false;

    memset(&out, 0, sizeof(out));

    out.CCSDS_MsgId = be16(buf + 0);
    out.CCSDS_Seq   = be16(buf + 2);
    out.CCSDS_Len   = be16(buf + 4);
    memcpy(out.CCSDS_TimeCode, buf + 6, 6);
    out.CCSDS_Padding = be32u(buf + 12);

    const uint8_t *body = buf + 16 + entry_off;
    memcpy(&out.ReflectedMID, body + 0,  sizeof(out.ReflectedMID));
    memcpy(&out.ReflectedCC,  body + 2,  sizeof(out.ReflectedCC));
    memcpy(&out.RetType,      body + 3,  sizeof(out.RetType));
    memcpy(&out.RetCode,      body + 4,  sizeof(out.RetCode));
    memcpy(&out.RetValSize,   body + 8,  sizeof(out.RetValSize));

    size_t copy_len = 512;
    if (out.RetValSize < copy_len) copy_len = out.RetValSize;
    memcpy(out.RetVal, body + 10, copy_len);
    if (out.RetValSize > 512) out.RetValSize = 512;

    out.IsCritical = is_critical ? 1 : 0;
    if (is_critical) {
        memcpy(&out.CritTimeSec,    body + RPT_REPORT_ENTRY_SIZE,     sizeof(out.CritTimeSec));
        memcpy(&out.CritTimeSubsec, body + RPT_REPORT_ENTRY_SIZE + 4, sizeof(out.CritTimeSubsec));
    }

    return true;
}

/* Decode RPT_OperationData_t (RPT_OPS_TLM_MID, RPT_GetOpsDataCmd()'s reply). */
static bool ParseRptOpsWire(const uint8_t *buf, size_t len, RPT_OpsData_t &out) {
    if (!buf || len < RPT_OPS_WIRE_SIZE) return false;

    memset(&out, 0, sizeof(out));
    const uint8_t *body = buf + 16;
    memcpy(&out.epoch_sec,    body + 0,  sizeof(out.epoch_sec));
    memcpy(&out.epoch_subsec, body + 4,  sizeof(out.epoch_subsec));
    out.reset_cause = body[8];
    /* body[9] is compiler padding ahead of the uint16 BootCount */
    memcpy(&out.boot_count,   body + 10, sizeof(out.boot_count));
    memcpy(&out.time_sec,     body + 12, sizeof(out.time_sec));
    memcpy(&out.time_subsec,  body + 16, sizeof(out.time_subsec));
    memcpy(&out.sequence,     body + 20, sizeof(out.sequence));
    memcpy(&out.crc,          body + 24, sizeof(out.crc));
    return true;
}

/* Route one fully-assembled wire packet from the shared RPT downlink port
 * (25=BEE-1000, 15=BEE-1012, 10=UELYSYS) to the right decoder, by its outer
 * CCSDS-like MsgId. RPT_REPORT_TLM_MID/RPT_CRITICAL_TLM_MID carry 1..N fixed-size
 * entries back to back (N=1 is the everyday single reflected-command Report -
 * the previously-existing, unchanged behavior); RPT_OPS_TLM_MID carries the
 * app's own operations-data snapshot instead of a reflected command result. */
static void ProcessAssembledReportWire(const char *sat_tag, int report_dport,
                                        const uint8_t *buf, size_t total_size,
                                        int (*report_saver)(Report *)) {
    if (!buf || total_size < 16) return;

    const uint16_t outer_msg_id = be16(buf + 0);

    if (outer_msg_id == RPT_OPS_TLM_MID) {
        RPT_OpsData_t ops;
        if (ParseRptOpsWire(buf, total_size, ops)) {
            {
                std::lock_guard<std::mutex> lk(g_report_view_mtx);   // 화면 스레드가 읽는다
                g_rpt_ops_data = ops;
                g_rpt_ops_data_valid = true;
            }
            printf("%s: RPT OPS DATA boot_count=%u reset_cause=0x%02X time=%u.%06u seq=%u\n",
                   sat_tag, (unsigned)ops.boot_count, (unsigned)ops.reset_cause,
                   (unsigned)ops.time_sec, (unsigned)ops.time_subsec, (unsigned)ops.sequence);
        } else {
            printf("%s: RPT OPS DATA wire too short (%zu bytes)\n", sat_tag, total_size);
        }
        return;
    }

    const bool is_critical = (outer_msg_id == RPT_CRITICAL_TLM_MID);
    const size_t entry_size = is_critical ? RPT_CRITICAL_ENTRY_SIZE : RPT_REPORT_ENTRY_SIZE;

    if (total_size < 16 + entry_size) {
        printf("%s: report wire too short for one %s entry (%zu bytes)\n",
               sat_tag, is_critical ? "critical" : "report", total_size);
        return;
    }

    if (((total_size - 16) % entry_size) != 0) {
        console.AddLog("[ERROR]##%s report length %zu is not 16 + N * %zu; parsing blocked.",
                       sat_tag, total_size, entry_size);
        return;
    }

    const size_t n_entries = (total_size - 16) / entry_size;
    for (size_t i = 0; i < n_entries; i++) {
        Report rpt;
        if (!ParseReportEntryAt(buf, i * entry_size, is_critical, rpt)) continue;

        printf("%s: REPORT %zu/%zu RefMID=0x%04x CC=0x%02x RetSize=%u%s\n",
               sat_tag, i + 1, n_entries, rpt.ReflectedMID, rpt.ReflectedCC,
               (unsigned)rpt.RetValSize, is_critical ? " [CRITICAL]" : "");

        UpdateN5ReportView(rpt, report_dport);
        if (report_saver) report_saver(&rpt);
    }
}

/* Shared byte-stream reassembler for the RPT downlink port (25/15/10, one per
 * satellite generation). A logical telemetry packet may arrive split across
 * several CSP reads, so bytes accumulate here until the packet's own declared
 * length is satisfied. The target length is not a fixed constant: it is read
 * back out of the packet's own CCSDS-like length field (offset 4-5, standard
 * CCSDS convention total_size = length_field + 7) once the first 6 bytes are
 * in hand, which lets the exact same assembler frame a single reflected-command
 * Report (540B), a single critical Report (548B, +8B CFE_TIME_SysTime_t), an
 * RPT_MultipleReportTlm_t/RPT_MultipleCriticalTlm_t replay batch (variable,
 * up to RPT_REPORT_QUEUE_LEN/RPT_CRITICAL_QUEUE_LEN entries), or the RPT app's
 * own Ops-data snapshot (44B) - all of which share this one port. */
/* FSW_SD1SAT report wire: 16B CCSDS-like header + CR_ReportTlm_Payload_t
 * (CrSeq/IsRetransmit/Spare + the embedded <APP>_CmdReport_Payload_t). Unlike
 * the RPT-app pipeline this never batches multiple entries in one packet -
 * every wire packet is exactly one report. */
static bool ParseSD1SATReportWire(const uint8_t *buf, size_t len, SD1SATReport &out) {
    static constexpr size_t kFixedSize = 16;   /* 4B 봉투(CrSeq/IsRetransmit/Spare) + 12B CmdReport 헤더 */
    if (!buf || len < kFixedSize) {
        printf("unsufficient data length : received data size = %d / min size = %d \n", (int)len, (int) kFixedSize);
        return false;
    }

    memset(&out, 0, sizeof(out));

    memcpy(&out.CrSeq, buf + 0, sizeof(out.CrSeq));
    out.IsRetransmit = buf[2];
    out.Spare        = buf[3];

    const uint8_t *cmdReport = buf + 4;    /* Data 는 cmdReport + 12 = buf + kFixedSize 부터 */
    memcpy(&out.CmdMsgId,   cmdReport + 0,  sizeof(out.CmdMsgId));
    memcpy(&out.CmdSeq,     cmdReport + 2,  sizeof(out.CmdSeq));
    memcpy(&out.RetVal,     cmdReport + 4,  sizeof(out.RetVal));
    out.CmdCode = cmdReport[8];
    out.RetType = cmdReport[9];
    memcpy(&out.DataLength, cmdReport + 10, sizeof(out.DataLength));

    size_t available = (len > kFixedSize) ? (len - kFixedSize) : 0;
    size_t copy_len  = out.DataLength;
    if (copy_len > sizeof(out.Data)) copy_len = sizeof(out.Data);
    if (copy_len > available) copy_len = available;
    memcpy(out.Data, cmdReport + 12, copy_len);
    if (out.DataLength > sizeof(out.Data)) out.DataLength = (uint16_t)sizeof(out.Data);

    return true;
}

/* ===================== SD1SAT report fragment reassembly (port 20) =====================
 * SD1SAT's downlink cannot carry more than 198 payload bytes in one CSP packet, so a
 * logical CCSDS report packet is chopped into pieces and every piece is prefixed with
 * the 4B fragment header declared at the top of struct SD1SATReport:
 *
 *     uint16 StreamID  (BE)  - identity shared by every fragment of one logical packet
 *     uint8  Index           - 0-based position of this fragment inside that packet
 *     uint8  Count           - number of fragments the packet was split into
 *
 * Fragments are expected strictly in order (Index 0, 1, ... Count-1). A gap, a repeat,
 * or a StreamID/Count that changes mid-packet is reported as an error and the partial
 * packet is discarded, so a damaged stream can never be silently glued together into a
 * plausible-looking report. When Index reaches Count-1 the CCSDS packet is complete:
 * it is saved, parsed and pushed to the report view, and the assembler goes back to
 * expecting Index 0 so the next report can start on the very next fragment.
 */
static constexpr size_t SD1SAT_FRAG_HDR_SIZE      = 4;
static constexpr size_t SD1SAT_FRAG_WIRE_MAX_SIZE = 2048; /* 16B hdr + 4B envelope + 12B CmdReport + 512B Data, with room to spare */
static constexpr double SD1SAT_FRAG_TIMEOUT_SEC   = 5.0;

static uint8_t  g_sd1sat_frag_wire[SD1SAT_FRAG_WIRE_MAX_SIZE];
static size_t   g_sd1sat_frag_off = 0;
static bool     g_sd1sat_frag_collecting = false;
static uint16_t g_sd1sat_frag_stream_id = 0;
static uint8_t  g_sd1sat_frag_count = 0;      /* Count carried by the packet being assembled */
static uint8_t  g_sd1sat_frag_next_index = 0; /* Index the next fragment has to carry */
static int      g_sd1sat_frag_dport = -1;
static time_t   g_sd1sat_frag_last_time = 0;
static bool     g_sd1sat_frag_have_last_time = false;

static void ResetSD1SATFragAssembler() {
    g_sd1sat_frag_collecting = false;
    g_sd1sat_frag_off = 0;
    g_sd1sat_frag_stream_id = 0;
    g_sd1sat_frag_count = 0;
    g_sd1sat_frag_next_index = 0;
    g_sd1sat_frag_dport = -1;
    g_sd1sat_frag_last_time = 0;
    g_sd1sat_frag_have_last_time = false;
    memset(g_sd1sat_frag_wire, 0, sizeof(g_sd1sat_frag_wire));
}

/* Timestamped .bin dump, same naming convention as AssembleReportPortChunk()'s raw
 * saver; `tag` distinguishes the fragments from the reassembled packet. */
static void SaveSD1SATReportBlob(const char *dir, const char *prefix, const char *tag,
                                 const uint8_t *buf, size_t len) {
    const std::string suffix = std::string(tag) + ".bin";
    std::string filename;
    FILE *fp = OpenTelemetryFile(dir, prefix, suffix.c_str(), NULL, "wb", &filename);
    if (fp) {
        fwrite(buf, 1, len, fp);
        fclose(fp);
        console.AddLog("Saved %zu-byte SD1SAT report data: %s", len, filename.c_str());
    } else {
        console.AddLog("[ERROR] Failed to save SD1SAT report data: %s", filename.c_str());
    }
}

void AssembleSD1SATReportChunk(int dport, const csp_packet_t *packet) {
    const time_t now = time(NULL);
    const size_t chunk_len = packet->length;

    printf("sd1sat_rpt: chunk_len=%zu\n", chunk_len);

    printf("===== SD1SAT REPORT RAW FRAGMENT DUMP (%zu bytes) =====\n", chunk_len);
    for (size_t i = 0; i < chunk_len; i++) {
        printf("%02X ", packet->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (chunk_len % 16 != 0) printf("\n");
    printf("=======================================================\n");

    if (chunk_len < SD1SAT_FRAG_HDR_SIZE) {
        printf("sd1sat_rpt: fragment shorter than the %zuB fragment header, dropping\n",
               SD1SAT_FRAG_HDR_SIZE);
        console.AddLog("[ERROR]##SD1SAT report fragment too short: %zu bytes (need at least %zu).",
                       chunk_len, SD1SAT_FRAG_HDR_SIZE);
        ResetSD1SATFragAssembler();
        return;
    }

    const uint16_t stream_id    = be16(packet->data + 0);
    const uint8_t  frag_index   = packet->data[2];
    const uint8_t  frag_count   = packet->data[3];
    const uint8_t *frag_payload = packet->data + SD1SAT_FRAG_HDR_SIZE;
    const size_t   frag_pay_len = chunk_len - SD1SAT_FRAG_HDR_SIZE;

    printf("sd1sat_rpt: fragment stream=0x%04X index=%u count=%u payload=%zu\n",
           stream_id, (unsigned)frag_index, (unsigned)frag_count, frag_pay_len);

    {
        char frag_tag[64];
        snprintf(frag_tag, sizeof(frag_tag), "--s%04X-i%02u-of%02u--",
                 stream_id, (unsigned)frag_index, (unsigned)frag_count);
        SaveSD1SATReportBlob("../data/SD1SAT/report/fragment", "rptfrag--", frag_tag,
                             packet->data, chunk_len);
    }

    if (frag_count == 0 || frag_index >= frag_count) {
        printf("sd1sat_rpt: invalid fragment header (index=%u, count=%u), dropping\n",
               (unsigned)frag_index, (unsigned)frag_count);
        console.AddLog("[ERROR]##SD1SAT fragment header invalid: stream=0x%04X index=%u count=%u.",
                       stream_id, (unsigned)frag_index, (unsigned)frag_count);
        ResetSD1SATFragAssembler();
        return;
    }

    /* A partial packet whose remaining fragments never showed up would otherwise sit
     * here forever and corrupt whichever report arrives next. */
    if (g_sd1sat_frag_collecting && g_sd1sat_frag_have_last_time) {
        double dt = difftime(now, g_sd1sat_frag_last_time);
        if (dt > SD1SAT_FRAG_TIMEOUT_SEC) {
            printf("sd1sat_rpt: timeout dt=%.1f sec (>%.0f). drop partial packet (%u/%u fragments).\n",
                   dt, SD1SAT_FRAG_TIMEOUT_SEC,
                   (unsigned)g_sd1sat_frag_next_index, (unsigned)g_sd1sat_frag_count);
            console.AddLog("[ERROR]##SD1SAT report timed out after %u/%u fragments; assembler reset.",
                           (unsigned)g_sd1sat_frag_next_index, (unsigned)g_sd1sat_frag_count);
            ResetSD1SATFragAssembler();
        }
    }

    if (g_sd1sat_frag_collecting) {
        const bool stream_changed = (stream_id  != g_sd1sat_frag_stream_id);
        const bool count_changed  = (frag_count != g_sd1sat_frag_count);
        const bool out_of_order   = (frag_index != g_sd1sat_frag_next_index);

        if (stream_changed || count_changed || out_of_order) {
            printf("sd1sat_rpt: SEQUENCE ERROR - expected stream=0x%04X index=%u count=%u, "
                   "got stream=0x%04X index=%u count=%u. dropping %zu assembled bytes.\n",
                   g_sd1sat_frag_stream_id, (unsigned)g_sd1sat_frag_next_index,
                   (unsigned)g_sd1sat_frag_count,
                   stream_id, (unsigned)frag_index, (unsigned)frag_count, g_sd1sat_frag_off);
            console.AddLog("[ERROR]##SD1SAT fragment out of sequence: expected stream=0x%04X index=%u count=%u, "
                           "got stream=0x%04X index=%u count=%u. Partial report discarded.",
                           g_sd1sat_frag_stream_id, (unsigned)g_sd1sat_frag_next_index,
                           (unsigned)g_sd1sat_frag_count,
                           stream_id, (unsigned)frag_index, (unsigned)frag_count);
            ResetSD1SATFragAssembler();

            /* Index 0 is always a legal packet start, so a fragment that merely lost its
             * predecessors still opens the next report instead of being thrown away too. */
            if (frag_index != 0) return;
        }
    }

    if (!g_sd1sat_frag_collecting) {
        if (frag_index != 0) {
            printf("sd1sat_rpt: no report in progress and index=%u != 0, dropping\n",
                   (unsigned)frag_index);
            console.AddLog("[ERROR]##SD1SAT fragment index %u arrived with no report in progress "
                           "(expected index 0); dropped.", (unsigned)frag_index);
            return;
        }
        g_sd1sat_frag_collecting  = true;
        g_sd1sat_frag_stream_id   = stream_id;
        g_sd1sat_frag_count       = frag_count;
        g_sd1sat_frag_next_index  = 0;
        g_sd1sat_frag_off         = 0;
        g_sd1sat_frag_dport       = dport;
        memset(g_sd1sat_frag_wire, 0, sizeof(g_sd1sat_frag_wire));
    }

    g_sd1sat_frag_last_time = now;
    g_sd1sat_frag_have_last_time = true;

    if (frag_pay_len > sizeof(g_sd1sat_frag_wire) - g_sd1sat_frag_off) {
        printf("sd1sat_rpt: overflow (off=%zu, payload=%zu, cap=%zu), dropping packet\n",
               g_sd1sat_frag_off, frag_pay_len, sizeof(g_sd1sat_frag_wire));
        console.AddLog("[ERROR]##SD1SAT report exceeds the %zu-byte reassembly buffer at fragment %u/%u; discarded.",
                       sizeof(g_sd1sat_frag_wire), (unsigned)(frag_index + 1), (unsigned)frag_count);
        ResetSD1SATFragAssembler();
        return;
    }

    memcpy(g_sd1sat_frag_wire + g_sd1sat_frag_off, frag_payload, frag_pay_len);
    g_sd1sat_frag_off += frag_pay_len;
    g_sd1sat_frag_next_index = (uint8_t)(frag_index + 1);

    printf("sd1sat_rpt: assembled fragment %u/%u, %zu bytes \n",
           (unsigned)g_sd1sat_frag_next_index, (unsigned)g_sd1sat_frag_count, g_sd1sat_frag_off);

    if (g_sd1sat_frag_next_index < g_sd1sat_frag_count) return; /* more fragments still to come */

    /* ---- Count fragments in: one whole CCSDS packet is back ---- */
    size_t total_size = g_sd1sat_frag_off;

    SaveSD1SATReportBlob("../data/SD1SAT/report", "rpt_raw--", "--",
                         g_sd1sat_frag_wire, total_size);

    SD1SATReport rpt;
    if (ParseSD1SATReportWire(g_sd1sat_frag_wire, total_size, rpt)) {
        rpt.StreamID = g_sd1sat_frag_stream_id;
        rpt.Count    = g_sd1sat_frag_count;
        rpt.Index    = (uint8_t)(g_sd1sat_frag_count - 1);
        printf("sd1sat_rpt: REPORT COMPLETE (stream=0x%04X, %u fragments, %zu bytes) "
               "CmdMsgId=0x%04x CmdCode=%u RetVal=%d DataLength=%u%s\n",
               g_sd1sat_frag_stream_id, (unsigned)g_sd1sat_frag_count, total_size,
               rpt.CmdMsgId, rpt.CmdCode, rpt.RetVal, rpt.DataLength,
               rpt.IsRetransmit ? " [RETRANSMIT]" : "");
        UpdateSD1SATReportView(rpt);
    } else {
        printf("sd1sat_rpt: ParseSD1SATReportWire failed on %zu reassembled bytes\n", total_size);
        //console.AddLog("[ERROR]##SD1SAT report reassembled (%zu bytes) but failed to parse.", total_size);
    }

    /* Back to expecting Index 0: the next report can start on the very next fragment. */
    ResetSD1SATFragAssembler();
}

void AssembleN5ReportChunk(const char *sat_tag, int dport, const csp_packet_t *packet,
                           const char *raw_save_dir, const char *raw_filename_prefix,
                           int (*report_saver)(Report *)) {
    const time_t now = time(NULL);
    const size_t chunk_len = packet->length;

    if (g_report_collecting && g_report_dport != dport) {
        console.AddLog("[WARN]##Report port changed during assembly (%d -> %d). Dropping partial report.",
                       g_report_dport, dport);
        g_report_collecting = false;
        g_report_off = 0;
        g_report_target = 0;
        g_report_dport = -1;
        g_report_have_last_time = false;
        g_report_last_chunk_time = 0;
        memset(g_report_wire, 0, sizeof(g_report_wire));
    }

    printf("%s: chunk_len=%zu\n", sat_tag, chunk_len);

    printf("===== REPORT RAW CHUNK DUMP (%zu bytes) =====\n", chunk_len);
    for (size_t i = 0; i < chunk_len; i++) {
        printf("%02X ", packet->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (chunk_len % 16 != 0) printf("\n");
    printf("===========================================\n");

    if (g_report_collecting && g_report_have_last_time) {
        double dt = difftime(now, g_report_last_chunk_time);
        if (dt > 5.0) {
            printf("%s: timeout dt=%.1f sec (>5). drop current report assembler.\n", sat_tag, dt);
            g_report_collecting = false;
            g_report_off = 0;
            g_report_target = 0;
            g_report_dport = -1;
            memset(g_report_wire, 0, sizeof(g_report_wire));
            g_report_have_last_time = false;
            g_report_last_chunk_time = 0;
        }
    }

    if (!g_report_collecting) {
        g_report_collecting = true;
        g_report_dport = dport;
        g_report_off = 0;
        g_report_target = 0;
        memset(g_report_wire, 0, sizeof(g_report_wire));
    }

    g_report_last_chunk_time = now;
    g_report_have_last_time = true;

    size_t remaining_capacity = sizeof(g_report_wire) - g_report_off;
    if (chunk_len > remaining_capacity) {
        printf("%s: overflow (off=%zu, chunk=%zu, cap=%zu). drop current and restart with this chunk.\n",
               sat_tag, g_report_off, chunk_len, remaining_capacity);
        g_report_off = 0;
        g_report_target = 0;
        memset(g_report_wire, 0, sizeof(g_report_wire));
        remaining_capacity = sizeof(g_report_wire);
        if (chunk_len > remaining_capacity) {
            printf("%s: chunk too big to fit even after restart (%zu)\n", sat_tag, chunk_len);
            g_report_collecting = false;
            g_report_dport = -1;
            g_report_have_last_time = false;
            return;
        }
    }

    memcpy(g_report_wire + g_report_off, packet->data, chunk_len);
    g_report_off += chunk_len;

    if (g_report_target == 0 && g_report_off >= 6) {
        const uint16_t len_field = be16(g_report_wire + 4);
        const size_t total = (size_t)len_field + 7;
        if (total < 16 || total > sizeof(g_report_wire)) {
            printf("%s: implausible wire length %zu (len_field=%u), dropping assembler\n",
                   sat_tag, total, (unsigned)len_field);
            g_report_collecting = false;
            g_report_off = 0;
            g_report_target = 0;
            g_report_dport = -1;
            memset(g_report_wire, 0, sizeof(g_report_wire));
            g_report_have_last_time = false;
            return;
        }
        g_report_target = total;
    }

    printf("%s: assembled %zu/%zu\n", sat_tag, g_report_off, g_report_target);

    if (g_report_target != 0 && g_report_off >= g_report_target) {
        const size_t total_size = g_report_target;

        std::string rptpktfilename;
        FILE *rpt_fp = OpenTelemetryFile(raw_save_dir, raw_filename_prefix, "--.bin", NULL, "wb", &rptpktfilename);
        if (rpt_fp) {
            fwrite(g_report_wire, 1, total_size, rpt_fp);
            fclose(rpt_fp);
            console.AddLog("Saved %zu-byte report: %s", total_size, rptpktfilename.c_str());
        } else {
            console.AddLog("[ERROR] Failed to save report: %s", rptpktfilename.c_str());
        }

        ProcessAssembledReportWire(sat_tag, dport, g_report_wire, total_size, report_saver);

        g_report_collecting = false;
        g_report_off = 0;
        g_report_target = 0;
        g_report_dport = -1;
        memset(g_report_wire, 0, sizeof(g_report_wire));
        g_report_have_last_time = false;
        g_report_last_chunk_time = 0;
    }
}
