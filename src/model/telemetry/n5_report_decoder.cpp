// Model - 누리호 5차 위성 리포트 해석 (BEE1012 포트 15, UELYSYS 포트 10, SD1SAT 포트 20)
//
// GS_nuri5 miman_coms.cpp 의 리포트 payload 해석 함수(DecodePayloadToView 와 EPS P80 / UELYSYS / BEE1012 /
// PAYYONSEI / SDR / SD1SAT / ADCS GET / ROMA / LGPM 해석), PAY_SLT 로그, 화면용 갱신 함수를 옮긴 것.
// BEE-1000(포트 25)은 이 파일을 쓰지 않는다 (report_parser.cpp 의 ParseReportWire540 / DetermineBee1000ReportKind 그대로).
// 이름만 바꿨다: DecodePayloadToView -> DecodeN5PayloadToView, UpdateReportViewFromReport -> UpdateN5ReportView,
//               UpdateReportViewFromSD1SATReport -> UpdateSD1SATReportView.
#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"
#include <netinet/in.h>

#include <algorithm>
#include <mutex>

#include "n5_report_decoder.h"

extern Console console;

// 화면/저장이 함께 읽는 5차 리포트 상태 (YonseiGS_coms.h 의 extern). 모두 g_report_view_mtx 아래에서 갱신한다
RPT_OpsData_t g_rpt_ops_data;
bool g_rpt_ops_data_valid = false;
EPS_Report_State_t g_eps_report_state;
EPS_RParam_Reassembly_State_t g_eps_rparam_reassembly;
int8_t g_roma_parget_requested_index = -1;

static uint16_t be16(const uint8_t *p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}
static uint32_t be32u(const uint8_t *p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}
static int32_t be32s(const uint8_t *p) {
    return (int32_t)be32u(p);
}

bool DecodeEPSQueryHeader(EPS_Report_Metadata_t &meta,
                                 const uint8_t *payload,
                                 uint16_t payload_len,
                                 uint8_t command_code,
                                 int32_t return_code)
{
    if (!payload || payload_len < EPS_QUERY_REPORT_HEADER_SIZE) return false;

    memset(&meta, 0, sizeof(meta));
    meta.valid            = true;
    meta.command_code     = command_code;
    meta.return_code      = return_code;
    meta.return_data_size = payload_len;
    meta.source           = payload[0];
    meta.data_id          = payload[1];
    meta.arg0             = payload[2];
    meta.arg1             = payload[3];
    memcpy(&meta.sequence,   payload + 4, sizeof(meta.sequence));
    memcpy(&meta.offset,     payload + 6, sizeof(meta.offset));
    memcpy(&meta.total_size, payload + 8, sizeof(meta.total_size));
    memcpy(&meta.chunk_size, payload + 10, sizeof(meta.chunk_size));

    const uint32_t available = payload_len - EPS_QUERY_REPORT_HEADER_SIZE;
    if (meta.chunk_size > available ||
        meta.chunk_size > EPS_QUERY_REPORT_DATA_MAX_LEN ||
        meta.offset > meta.total_size ||
        (uint32_t)meta.offset + meta.chunk_size > meta.total_size) {
        return false;
    }

    return true;
}

static bool DecodeEPSQueryPayloadToView(ReportView_t &v, const uint8_t *payload, uint16_t payload_len)
{
    EPS_Report_Metadata_t meta;
    if (!DecodeEPSQueryHeader(meta, payload, payload_len, v.reflected_cc, v.ret_code)) {
        v.kind = REPORT_KIND_SC_GENERIC;
        return false;
    }

    v.eps_meta = meta;
    const uint8_t *data = payload + EPS_QUERY_REPORT_HEADER_SIZE;
    const uint16_t data_len = meta.chunk_size;

    if (v.reflected_cc == EPS_P80_POWER_IF_GET_CC &&
        meta.data_id == EPS_QUERY_REPORT_P80_POWER_IF_STATUS) {
        if (data_len != sizeof(EPS_PowerIfStatus_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_POWER_IF_STATUS;
        memcpy(&v.u.eps_power_if_status, data, sizeof(EPS_PowerIfStatus_Report_t));
        return true;
    }

    if (v.reflected_cc == EPS_P80_POWER_IF_LIST_CC &&
        meta.data_id == EPS_QUERY_REPORT_P80_POWER_IF_LIST) {
        if (data_len != sizeof(EPS_PowerIfList_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_POWER_IF_LIST;
        memcpy(&v.u.eps_power_if_list, data, sizeof(EPS_PowerIfList_Report_t));
        return true;
    }

    if ((v.reflected_cc == EPS_GET_HK_ALL_CC) && 
        meta.data_id == EPS_QUERY_REPORT_HK_ALL &&
        meta.source == 0) {
        if (data_len != sizeof(EPS_HK_ALL_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_HK_ALL;
        memcpy(&v.u.eps_hk_all, data, sizeof(EPS_HK_ALL_Report_t));
        return true;
    }

    if ((v.reflected_cc == EPS_GET_HK_CC || v.reflected_cc == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_PMU_HK &&
        meta.source == 1) {
        if (data_len != sizeof(EPS_P80_PMU_HK_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_P80_PMU_HK;
        memcpy(&v.u.eps_p80_pmu_hk, data, sizeof(EPS_P80_PMU_HK_Report_t));
        return true;
    }

    if ((v.reflected_cc == EPS_GET_HK_CC || v.reflected_cc == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_PDU_HK &&
        meta.source == 4) {
        if (data_len != sizeof(EPS_P80_PDU_HK_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_P80_PDU_HK;
        memcpy(&v.u.eps_p80_pdu_hk, data, sizeof(EPS_P80_PDU_HK_Report_t));
        return true;
    }

    if ((v.reflected_cc == EPS_GET_HK_CC || v.reflected_cc == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_ACU_HK &&
        (meta.source == 2 || meta.source == 3)) {
        if (data_len != sizeof(EPS_P80_ACU_HK_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = (meta.source == 2) ? REPORT_KIND_EPS_P80_ACU1_HK : REPORT_KIND_EPS_P80_ACU2_HK;
        memcpy(&v.u.eps_p80_acu_hk, data, sizeof(EPS_P80_ACU_HK_Report_t));
        return true;
    }

    if ((v.reflected_cc == EPS_GET_HK_CC || v.reflected_cc == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_BP8_HK &&
        meta.source == 7) {
        if (data_len != sizeof(EPS_BP8_HK_Report_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
            return false;
        }
        v.kind = REPORT_KIND_EPS_BP8_HK;
        memcpy(&v.u.eps_bp8_hk, data, sizeof(EPS_BP8_HK_Report_t));
        return true;
    }

    v.kind = REPORT_KIND_EPS_QUERY_RAW;
    v.u.uelysys.size = (data_len > sizeof(v.u.uelysys.bytes))
                           ? (uint16_t)sizeof(v.u.uelysys.bytes)
                           : data_len;
    memcpy(v.u.uelysys.bytes, data, v.u.uelysys.size);
    return true;
}

static void UpdateEPSReportStateFromView(const ReportView_t &v)
{
    if (!v.eps_meta.valid) return;

    switch (v.kind) {
        case REPORT_KIND_EPS_POWER_IF_STATUS:
            g_eps_report_state.power_if_status_valid = true;
            g_eps_report_state.power_if_status_meta = v.eps_meta;
            g_eps_report_state.power_if_status = v.u.eps_power_if_status;
            break;

        case REPORT_KIND_EPS_POWER_IF_LIST:
            g_eps_report_state.power_if_list_valid = true;
            g_eps_report_state.power_if_list_meta = v.eps_meta;
            g_eps_report_state.power_if_list = v.u.eps_power_if_list;
            break;

        case REPORT_KIND_EPS_HK_ALL :
            g_eps_report_state.hk_all_meta = v.eps_meta;

            g_eps_report_state.pmu_hk_valid = true;
            g_eps_report_state.pmu_hk = v.u.eps_hk_all.PMU;

            g_eps_report_state.pdu_hk_valid = true;
            g_eps_report_state.pdu_hk = v.u.eps_hk_all.PDU;

            g_eps_report_state.acu1_hk_valid = true;
            g_eps_report_state.acu1_hk = v.u.eps_hk_all.ACU[0];

            g_eps_report_state.acu2_hk_valid = true;
            g_eps_report_state.acu2_hk = v.u.eps_hk_all.ACU[1];

            g_eps_report_state.bp8_hk_valid = true;
            g_eps_report_state.bp8_hk = v.u.eps_hk_all.BP8;
            
            break;

        case REPORT_KIND_EPS_P80_PMU_HK:
            g_eps_report_state.pmu_hk_valid = true;
            g_eps_report_state.pmu_hk_meta = v.eps_meta;
            g_eps_report_state.pmu_hk = v.u.eps_p80_pmu_hk;
            break;

        case REPORT_KIND_EPS_P80_PDU_HK:
            g_eps_report_state.pdu_hk_valid = true;
            g_eps_report_state.pdu_hk_meta = v.eps_meta;
            g_eps_report_state.pdu_hk = v.u.eps_p80_pdu_hk;
            break;

        case REPORT_KIND_EPS_P80_ACU1_HK:
            g_eps_report_state.acu1_hk_valid = true;
            g_eps_report_state.acu1_hk_meta = v.eps_meta;
            g_eps_report_state.acu1_hk = v.u.eps_p80_acu_hk;
            break;

        case REPORT_KIND_EPS_P80_ACU2_HK:
            g_eps_report_state.acu2_hk_valid = true;
            g_eps_report_state.acu2_hk_meta = v.eps_meta;
            g_eps_report_state.acu2_hk = v.u.eps_p80_acu_hk;
            break;

        case REPORT_KIND_EPS_BP8_HK:
            g_eps_report_state.bp8_hk_valid = true;
            g_eps_report_state.bp8_hk_meta = v.eps_meta;
            g_eps_report_state.bp8_hk = v.u.eps_bp8_hk;
            break;

        case REPORT_KIND_EPS_QUERY_RAW:
            if (v.reflected_cc == EPS_RPARAM_GET_FULL_TABLE_CC &&
                (v.eps_meta.data_id == EPS_QUERY_REPORT_RPARAM_TABLE_ROWS ||
                 v.eps_meta.data_id == EPS_QUERY_REPORT_RPARAM_TABLE_MEMORY)) {
                auto &s = g_eps_rparam_reassembly;
                const auto &m = v.eps_meta;
                const bool new_transaction =
                    !s.active || s.complete || s.error ||
                    s.source != m.source || s.data_id != m.data_id ||
                    s.arg0 != m.arg0 || s.arg1 != m.arg1 ||
                    s.total_size != m.total_size ||
                    (m.sequence == 0 && s.expected_sequence != 0);
                if (new_transaction) {
                    memset(&s, 0, sizeof(s));
                    s.active = true;
                    s.valid = true;
                    s.source = m.source;
                    s.data_id = m.data_id;
                    s.arg0 = m.arg0;
                    s.arg1 = m.arg1;
                    s.total_size = m.total_size;
                }
                if (m.total_size > EPS_RPARAM_REASSEMBLY_CAPACITY ||
                    (uint32_t)m.offset + m.chunk_size > s.total_size) {
                    s.valid = false;
                    s.error = true;
                    break;
                }
                if (m.sequence > s.expected_sequence) s.missing_sequence_seen = true;
                if (m.sequence < s.expected_sequence) s.duplicate_seen = true;
                if (m.sequence >= s.expected_sequence) s.expected_sequence = (uint16_t)(m.sequence + 1);
                s.last_sequence = m.sequence;
                s.last_offset = m.offset;
                s.last_chunk_size = m.chunk_size;
                for (uint16_t i = 0; i < m.chunk_size; ++i) {
                    const uint16_t pos = (uint16_t)(m.offset + i);
                    const uint8_t new_byte = v.u.uelysys.bytes[i];
                    if (s.coverage[pos]) {
                        s.duplicate_seen = true;
                        if (s.buffer[pos] != new_byte) {
                            s.error = true;
                            continue;
                        }
                    } else {
                        s.coverage[pos] = 1;
                        s.buffer[pos] = new_byte;
                        ++s.received_size;
                    }
                }
                s.complete = (!s.error && s.received_size == s.total_size);
                s.valid = !s.error;
            }
            break;

        default:
            break;
    }
}


static bool ValidateEPSCommandStatusSize(uint8_t cc, uint16_t payload_len)
{
    switch (cc) {
        case EPS_NOOP_CC:
        case EPS_RESET_COUNTERS_CC:
            return payload_len == 4U * sizeof(uint32_t);
        case EPS_P80_POWER_IF_GET_CC:
            return payload_len == sizeof(EPS_P80_Power_If_Get_Cmd_Payload_t);
        case EPS_P80_POWER_IF_SET_CC:
            return payload_len == sizeof(EPS_P80_Power_If_Set_Cmd_Payload_t);
        case EPS_P80_POWER_IF_LIST_CC:
            return payload_len == sizeof(EPS_P80_Power_If_List_Cmd_Payload_t);
        case EPS_GET_HK_CC:
            return payload_len == sizeof(EPS_Get_HK_Cmd_Payload_t);
        case EPS_GET_HK_ALL_CC:
        case EPS_P80_GND_WDT_CLEAR_ALL_CC:
            return payload_len == 5; /* packed uint8 csp_node + int32 status */
        case EPS_P80_GND_WDT_CLEAR_CC:
            return payload_len == sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_Payload_t);
        case EPS_RPARAM_GET_CC:
            return payload_len == sizeof(EPS_RParam_Get_Cmd_Payload_t);
        case EPS_RPARAM_SET_CC:
            return payload_len == sizeof(EPS_RParam_Set_Cmd_Payload_t);
        case EPS_RPARAM_GET_FULL_TABLE_CC:
            return payload_len == sizeof(EPS_RParam_Get_Full_Table_Cmd_Payload_t);
        case EPS_RPARAM_SAVE_ALL_CC:
            return payload_len == sizeof(EPS_RParam_Save_All_Cmd_Payload_t);
        case EPS_RPARAM_TABLE_SAVE_CC:
            return payload_len == sizeof(EPS_RParam_Table_Save_Cmd_Payload_t);
        case EPS_RPARAM_TABLE_LOAD_CC:
            return payload_len == sizeof(EPS_RParam_Table_Load_Cmd_Payload_t);
        case EPS_RPARAM_SAVE_TO_STORE_CC:
            return payload_len == sizeof(EPS_RParam_Save_To_Store_Cmd_Payload_t);
        case EPS_RPARAM_LOAD_FROM_STORE_CC:
            return payload_len == sizeof(EPS_RParam_Load_From_Store_Cmd_Payload_t);
        case EPS_CSP_PING_CC:
            return payload_len == sizeof(EPS_CSP_Ping_Cmd_Payload_t);
        case EPS_CSP_REBOOT_CC:
            return payload_len == sizeof(EPS_CSP_Reboot_Cmd_Payload_t);
        case EPS_CSP_PS_CC:
            return payload_len == sizeof(EPS_CSP_PS_Cmd_Payload_t);
        case EPS_CSP_MEMFREE_CC:
            return payload_len == sizeof(EPS_CSP_MemFree_Cmd_Payload_t);
        case EPS_CSP_BUF_FREE_CC:
            return payload_len == sizeof(EPS_CSP_BufFree_Cmd_Payload_t);
        case EPS_CSP_UPTIME_CC:
            return payload_len == sizeof(EPS_CSP_Uptime_Cmd_Payload_t);
        default:
            return false;
    }
}


size_t PaySltParamElementSize(uint8_t type)
{
    switch (type) {
        case PAY_SLT_PARAM_UINT8:
        case PAY_SLT_PARAM_INT8:
        case PAY_SLT_PARAM_STRING:
            return 1;
        case PAY_SLT_PARAM_UINT16:
        case PAY_SLT_PARAM_INT16:
            return 2;
        case PAY_SLT_PARAM_UINT32:
        case PAY_SLT_PARAM_FLOAT:
            return 4;
        default:
            return 0;
    }
}

static bool ValidatePaySltReportPayload(const ReportView_t &v, const uint8_t *payload, uint16_t payload_len)
{
    /* PAY_SLT_VerifyCmdLength() reports {actual_length, expected_length} for
     * any CC. Accept that common 8-byte error response first. */
    if (v.ret_type != 0 && payload_len == 2 * sizeof(uint32_t)) return true;

    switch (v.reflected_cc) {
        case BEE1012_PAY_SLT_NOOP_CC:
            return payload_len == 0 || payload_len == 32;
        case BEE1012_PAY_SLT_RESET_COUNTERS_CC:
        case BEE1012_PAY_SLT_PAR_SET_ARRAY_CC:
            return payload_len == 0;
        case BEE1012_PAY_SLT_REPORT_BCN_CC:
            return payload_len == 0 || payload_len == sizeof(uint8_t);
        case BEE1012_PAY_SLT_OUTPUT_ENABLED_CC:
            return payload_len == 0 || payload_len == sizeof(PAY_SLT_OutputEnabled_Report_t);
        case BEE1012_PAY_SLT_RS422_PING_CC:
            return payload_len == 0 || payload_len == 10;
        case BEE1012_PAY_SLT_PAR_GET_CC: {
            if (!payload || payload_len < sizeof(PAY_SLT_ParamHeader_Report_t)) return false;
            PAY_SLT_ParamHeader_Report_t h{};
            memcpy(&h, payload, sizeof(h));
            const size_t element_size = PaySltParamElementSize(h.type);
            if (element_size == 0) return payload_len == sizeof(h);
            size_t expected_size = sizeof(h) + (size_t)h.len * element_size;
            /* PAY_SLT_GetParamReportSize() clamps malformed requests to the
             * fixed PAY_SLT_Params_t size (8-byte prefix + 256-byte union). */
            if (expected_size > 264) expected_size = 264;
            return payload_len == expected_size;
        }
        case BEE1012_PAY_SLT_PAR_SET_CC:
            /* Invalid node/table echoes packed PAY_SLT_ParSet_Payload_t. */
            return payload_len == 0 || payload_len == 104;
        case BEE1012_PAY_SLT_SCAN_FILES_CC:
            return payload_len == 0 || payload_len == sizeof(uint8_t) ||
                   payload_len == sizeof(PAY_SLT_ScanFiles_Report_t);
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC:
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC:
            return payload_len == sizeof(PAY_SLT_DownloadRequest_Report_t) ||
                   payload_len == sizeof(PAY_SLT_DownloadResult_Report_t);
        case BEE1012_PAY_SLT_GET_FULL_TABLE_CC:
            /* Success is a raw table image. Error forms are the request or a
             * 32-bit gs_error_t/FSW-size_t value. */
            if (v.ret_type == 0) return payload_len <= 512;
            return payload_len == sizeof(PAY_SLT_GetFullTableRequest_Report_t) ||
                   payload_len == sizeof(int32_t);
        default:
            return false;
    }
}

bool DecodeN5PayloadToView(ReportView_t &v, const uint8_t *payload, uint16_t payload_len) {
    if (!payload) return false;
    v.raw_size = (payload_len > sizeof(v.raw_bytes)) ? (uint16_t)sizeof(v.raw_bytes) : payload_len;
    memcpy(v.raw_bytes, payload, v.raw_size);

    if (v.kind == REPORT_KIND_EPS_QUERY_REPORT) {
        if (DecodeEPSQueryPayloadToView(v, payload, payload_len)) return true;
        /* Query commands echo their request payload on failure. CSP PS also
         * sends a final one-byte status after its text reports. */
        v.kind = REPORT_KIND_EPS_COMMAND_STATUS;
        return ValidateEPSCommandStatusSize(v.reflected_cc, payload_len);
    }

    if (v.kind == REPORT_KIND_EPS_COMMAND_STATUS)
        return ValidateEPSCommandStatusSize(v.reflected_cc, payload_len);

    if (v.kind == REPORT_KIND_EPS_BCN_REPORT) {
        if (payload_len != sizeof(v.u.eps_bcn)) return false;
        memcpy(&v.u.eps_bcn, payload, sizeof(v.u.eps_bcn));
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_MEOW &&
        (v.reflected_msg_id == UELYSYS_MEOW_CMD_ID || v.reflected_msg_id == 0x8618)) {
        bool valid_size = false;
        switch (v.reflected_cc) {
            case UELYSYS_MEOW_NOOP_CC:
            case UELYSYS_MEOW_RESET_COUNTERS_CC:
                valid_size = (payload_len == 2);
                break;
            case UELYSYS_MEOW_SHELL_EXEC_SYNC_CC:
            case UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC:
                valid_size = (payload_len == sizeof(UELYSYS_MEOW_ShellResult_Report_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_ShellResultErrno_Report_t));
                break;
            case UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC:
            case UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_ValueErrno_Report_t));
                break;
            case UELYSYS_MEOW_SHELL_POLL_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_ShellPoll_Report_t));
                break;
            case UELYSYS_MEOW_FILE_READ_CC:
            case UELYSYS_MEOW_FILE_TAIL_CC:
                valid_size = (payload_len <= UELYSYS_MEOW_MISSION_MAX_READ_LEN);
                break;
            case UELYSYS_MEOW_FILE_STAT_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_FileStat_Report_t));
                break;
            case UELYSYS_MEOW_DISK_STAT_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_DiskStat_Report_t));
                break;
            case UELYSYS_MEOW_SYS_INFO_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_SysInfo_Report_t));
                break;
            case UELYSYS_MEOW_SYS_TIME_GET_CC:
                valid_size = (payload_len == sizeof(int32_t) ||
                              payload_len == sizeof(UELYSYS_MEOW_SysTime_Report_t));
                break;
            case UELYSYS_MEOW_CSP_IFSTATS_CC:
                valid_size = (payload_len == 0 ||
                              payload_len == sizeof(UELYSYS_MEOW_CspIfStats_Report_t));
                break;
            case UELYSYS_MEOW_SHELL_KILL_CC:
            case UELYSYS_MEOW_FILE_WRITE_CC:
            case UELYSYS_MEOW_FILE_REMOVE_CC:
            case UELYSYS_MEOW_FILE_COPY_CC:
            case UELYSYS_MEOW_FILE_MOVE_CC:
            case UELYSYS_MEOW_FILE_TRUNCATE_CC:
            case UELYSYS_MEOW_SYS_TIME_SET_CC:
            case UELYSYS_MEOW_CSP_HANDLER_LOAD_CC:
            case UELYSYS_MEOW_CSP_SEND_CC:
            case UELYSYS_MEOW_CSP_FTP_UPLOAD_CC:
            case UELYSYS_MEOW_CSP_FTP_DOWNLOAD_CC:
            case UELYSYS_MEOW_CSP_ROUTE_SET_CC:
                valid_size = (payload_len == 0 || payload_len == sizeof(int32_t));
                break;
            case UELYSYS_MEOW_FILE_CHECKSUM_CC:
            case UELYSYS_MEOW_SYS_SHUTDOWN_CC:
            case UELYSYS_MEOW_SYS_FORCE_KILL_CC:
                valid_size = (payload_len == sizeof(int32_t));
                break;
            case UELYSYS_MEOW_SYS_SYNC_CC:
            case UELYSYS_MEOW_CSP_SERVER_START_CC:
            case UELYSYS_MEOW_CSP_SERVER_STOP_CC:
            case UELYSYS_MEOW_CSP_SET_READ_TIMEOUT_CC:
            case UELYSYS_MEOW_CSP_HANDLER_CLEAR_CC:
            case UELYSYS_MEOW_CSP_REROUTE_SET_CC:
            case UELYSYS_MEOW_CSP_REROUTE_CLEAR_CC:
                valid_size = (payload_len == 0);
                break;
            default:
                /* Invalid command-code reports carry the rejected uint8 CC. */
                valid_size = (payload_len == sizeof(uint8_t));
                break;
        }
        /* Every dispatched command may instead report the native
         * {ActualLength, ExpectedLength} pair after a length failure. */
        if (v.ret_type != 0 && payload_len == (2 * sizeof(uint32_t))) valid_size = true;
        return valid_size;
    }

    if (v.kind == REPORT_KIND_UELYSYS_TTC) {
        switch (v.reflected_cc) {
            case UELYSYS_TTC_NOOP_CC:
            case UELYSYS_TTC_RESET_COUNTERS_CC:
            case UELYSYS_TTC_GET_TIMELINE_HK_CC:
            case UELYSYS_TTC_RESET_TIMELINE_HK_CC:
            case UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC:
            case UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC:
                return payload_len == sizeof(UELYSYS_TTC_Counters_Report_t);
            case UELYSYS_TTC_REPORT_CC:
                return payload_len == sizeof(UELYSYS_TTC_ReportStatus_Report_t);
            case UELYSYS_TTC_START_BEACON_BURST_CC:
                return payload_len == sizeof(UELYSYS_TTC_BeaconBurst_Report_t);
            case UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC:
            case UELYSYS_TTC_DELETE_ALL_ENTRIES_CC:
            case UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC:
                return payload_len == sizeof(uint16_t);
            case UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC:
                return payload_len == sizeof(UELYSYS_TTC_NextEntry_Report_t);
            case UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC:
                return payload_len == sizeof(UELYSYS_TTC_NextExecution_Report_t);
            case UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_InsertAbs_Report_t);
            case UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_InsertRel_Report_t);
            case UELYSYS_TTC_DELETE_CMD_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_INIT_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_EntryGroup_Report_t);
            case UELYSYS_TTC_DELETE_CMD_GROUP_CC:
                return payload_len == sizeof(UELYSYS_TTC_DeleteGroup_Report_t);
            case UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_ExecuteEntry_Report_t);
            case UELYSYS_TTC_EXECUTE_CMD_GROUP_CC:
                return payload_len == sizeof(UELYSYS_TTC_ExecuteGroup_Report_t);
            case UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_PlumbWrite_Report_t);
            case UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC:
                return payload_len == sizeof(UELYSYS_TTC_PlumbFinalize_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_GPIO) {
        bool valid_size = false;
        switch (v.reflected_cc) {
            case UELYSYS_GPIO_NOOP_CC:
            case UELYSYS_GPIO_RESET_COUNTERS_CC:
                valid_size = (payload_len == 2);
                break;
            case UELYSYS_GPIO_PROCESS_CC:
                valid_size = (payload_len == 0);
                break;
            case UELYSYS_GPIO_DIGPIOLAY_PARAM_CC:
                valid_size = (payload_len == sizeof(UELYSYS_GPIO_Digpiolay_Report_t));
                break;
            case UELYSYS_GPIO_STX_EN1_HIGH_CC:
            case UELYSYS_GPIO_STX_EN1_LOW_CC:
            case UELYSYS_GPIO_LGE_MCU_RESET_HIGH_CC:
            case UELYSYS_GPIO_LGE_MCU_RESET_LOW_CC:
            case UELYSYS_GPIO_STX_EN2_HIGH_CC:
            case UELYSYS_GPIO_STX_EN2_LOW_CC:
            case UELYSYS_GPIO_ADCS_EN_HIGH_CC:
            case UELYSYS_GPIO_ADCS_EN_LOW_CC:
            case UELYSYS_GPIO_ADCS_BOOT_HIGH_CC:
            case UELYSYS_GPIO_ADCS_BOOT_LOW_CC:
                valid_size = (payload_len == 0 || payload_len == 1);
                break;
            default:
                valid_size = (payload_len == sizeof(uint8_t));
                break;
        }
        if (v.ret_type != 0 && payload_len == (2 * sizeof(uint32_t))) valid_size = true;
        return valid_size;
    }

    if (v.kind == REPORT_KIND_UELYSYS_STX) {
        switch (v.reflected_cc) {
            case UELYSYS_STX_NOOP_CC:
            case UELYSYS_STX_RESET_COUNTERS_CC:
                return payload_len == sizeof(STX_NOOP_Report_t);
            case UELYSYS_STX_INITIALIZE_CC:
                return payload_len == sizeof(STX_Initialize_Report_t);
            case UELYSYS_STX_SET_MODULE_ID_CC:
                return payload_len == sizeof(STX_SetModuleID_Report_t);
            case UELYSYS_STX_PARAM_INIT_CC:
                return payload_len == sizeof(STX_ParamInit_Report_t);
            case UELYSYS_STX_GET_FILEHANDLE_CC:
                return payload_len == sizeof(STX_GetFileHandle_Report_t);

            case UELYSYS_STX_SET_SYMBOL_RATE_CC:
                return payload_len == sizeof(STX_SetSymbolRate_Report_t);
            case UELYSYS_STX_SET_TX_POWER_CC:
                return payload_len == sizeof(STX_SetTxPower_Report_t);
            case UELYSYS_STX_SET_CENTER_FREQ_CC:
                return payload_len == sizeof(STX_SetCenterFreq_Report_t);
            case UELYSYS_STX_SET_MODCOD_CC:
                return payload_len == sizeof(STX_SetModCode_Report_t);
            case UELYSYS_STX_SET_ROLL_OFF_CC:
                return payload_len == sizeof(STX_SetRollOff_Report_t);
            case UELYSYS_STX_SET_PILOT_SIGNAL_CC:
                return payload_len == sizeof(STX_SetPilotSig_Report_t);
            case UELYSYS_STX_SET_FEC_FRAME_SIZE_CC:
                return payload_len == sizeof(STX_SetFECFrame_Report_t);
            case UELYSYS_STX_SET_PRETX_DELAY_CC:
                return payload_len == sizeof(STX_SetPreTxDelay_Report_t);
            case UELYSYS_STX_SET_ALL_PARAMETERS_CC:
                return payload_len == sizeof(STX_SetAllParam_Report_t);
            case UELYSYS_STX_SET_RS485_BAUD_CC:
                return payload_len == sizeof(STX_SetRS485_Report_t);
            case UELYSYS_STX_SET_MOD_DATA_IFACE_CC:
                return payload_len == sizeof(STX_SetModInterface_Report_t);

            case UELYSYS_STX_GET_SYMBOL_RATE_CC:
                return payload_len == sizeof(STX_GetSymbolRate_Report_t);
            case UELYSYS_STX_GET_TX_POWER_CC:
                return payload_len == sizeof(STX_GetTxPower_Report_t);
            case UELYSYS_STX_GET_CENTER_FREQ_CC:
                return payload_len == sizeof(STX_GetCenterFreq_Report_t);
            case UELYSYS_STX_GET_MODCOD_CC:
                return payload_len == sizeof(STX_GetModCode_Report_t);
            case UELYSYS_STX_GET_ROLL_OFF_CC:
                return payload_len == sizeof(STX_GetRollOff_Report_t);
            case UELYSYS_STX_GET_PILOT_SIGNAL_CC:
                return payload_len == sizeof(STX_GetPilot_Report_t);
            case UELYSYS_STX_GET_FEC_FRAME_SIZE_CC:
                return payload_len == sizeof(STX_GetFECFrame_Report_t);
            case UELYSYS_STX_GET_PRETX_DELAY_CC:
                return payload_len == sizeof(STX_GetPreTxDelay_Report_t);
            case UELYSYS_STX_GET_ALL_PRAMETERS_CC:
                return payload_len == sizeof(STX_GetAllParams_Report_t);
            case UELYSYS_STX_GET_REPORT_CC:
                return payload_len == sizeof(STX_GetReport_Report_t);
            case UELYSYS_STX_GET_MOD_DATA_IFACE_CC:
                return payload_len == sizeof(STX_GetDataIface_Report_t);

            case UELYSYS_STX_FILESYS_CC_DIR:
                return (payload_len >= sizeof(esup_ret_t) && payload_len <= sizeof(STX_DIR_Report_t));
            case UELYSYS_STX_FILESYS_CC_DIRNEXT:
                return (payload_len >= sizeof(esup_ret_t) && payload_len <= sizeof(STX_DIRNext_Report_t));
            case UELYSYS_STX_FILESYS_CC_DELFILE:
                return payload_len == sizeof(STX_DelFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_DELALLFILE:
                return payload_len == sizeof(STX_DelAllFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_CREATEFILE:
                return payload_len == sizeof(STX_CreateFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_WRITEFILE:
                return payload_len == sizeof(STX_WriteFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_OPENFILE:
                return payload_len == sizeof(STX_OpenFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_READFILE:
                return (payload_len >= 14 && payload_len <= sizeof(STX_ReadFile_Report_t));
            case UELYSYS_STX_FILESYS_CC_SENDFILE:
                return payload_len == sizeof(STX_SendFile_Report_t);
            case UELYSYS_STX_FILESYS_CC_SENDFILERTI:
                return payload_len == sizeof(STX_SendFileWithError_Report_t);
                
            case UELYSYS_STX_SYSCONF_CC_TRANSMITMODE:
                return payload_len == sizeof(STX_TxMode_Report_t);
            case UELYSYS_STX_SYSCONF_CC_IDLEMODE:
                return payload_len == sizeof(STX_IdleMode_Report_t);
            case UELYSYS_STX_SYSCONF_CC_UPDATEFW:
                return payload_len == sizeof(STX_UpdateFW_Report_t);
            case UELYSYS_STX_SYSCONF_CC_SAFESHUTDOWN:
                return payload_len == sizeof(STX_ShutDown_Report_t);
            
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_UANT) {
        const bool success = v.ret_type == 0;
        switch (v.reflected_cc) {
            case UELYSYS_UANT_NOOP_CC:
            case UELYSYS_UANT_RESET_COUNTERS_CC:
            case UELYSYS_UANT_RESET_STATUS_COUNT_CC:
            case UELYSYS_UANT_BURN_CHANNEL_CC:
            case UELYSYS_UANT_STOP_BURN_CC:
            case UELYSYS_UANT_SET_SETTINGS_CC:
                return payload_len == 0;
            case UELYSYS_UANT_GET_STATUS_CC:
                return payload_len == (success ? sizeof(UELYSYS_UANT_ReleaseStatus_Report_t) : 0);
            case UELYSYS_UANT_GET_BACKUP_STATUS_CC:
                return payload_len == (success ? sizeof(UELYSYS_UANT_BackupStatus_Report_t) : 0);
            case UELYSYS_UANT_GET_BOARD_STATUS_CC:
                return payload_len == (success ? sizeof(UELYSYS_UANT_BoardStatus_Report_t) : 0);
            case UELYSYS_UANT_GET_TEMPERATURE_CC:
                return payload_len == (success ? sizeof(UELYSYS_UANT_Temperature_Report_t) : 0);
            case UELYSYS_UANT_GET_SETTINGS_CC:
                return payload_len == (success ? sizeof(UELYSYS_UANT_Settings_Report_t) : 0);
            case UELYSYS_UANT_AUTODEPLOY_CC:
                return payload_len == sizeof(UELYSYS_UANT_Autodeploy_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_UTRX) {
        const bool success = v.ret_type == 0;
        switch (v.reflected_cc) {
            case UELYSYS_UTRX_NOOP_CC:
            case UELYSYS_UTRX_RESET_COUNTERS_CC:
            case UELYSYS_UTRX_RESET_APP_COUNTERS_CC:
            case UELYSYS_UTRX_RESET_DEVICE_COUNTERS_CC:
                if (payload_len != sizeof(v.u.utrx_noop)) return false;
                memcpy(&v.u.utrx_noop, payload, sizeof(v.u.utrx_noop));
                break;
            case UELYSYS_UTRX_RXCONF_GET_BAUD_CC:
            case UELYSYS_UTRX_TXCONF_GET_BAUD_CC:
            case UELYSYS_UTRX_TLM_GET_BOOT_CAUSE_CC:
            case UELYSYS_UTRX_TLM_GET_LAST_CONTACT_CC:
            case UELYSYS_UTRX_TLM_GET_TOT_TX_BYTES_CC:
            case UELYSYS_UTRX_TLM_GET_TOT_RX_BYTES_CC:
            case UELYSYS_UTRX_RXCONF_GET_FREQ_CC:
            case UELYSYS_UTRX_TXCONF_GET_FREQ_CC:
                if (payload_len != (success ? sizeof(uint32_t) : 0)) return false;
                if (payload_len == sizeof(v.u.utrx_rx_baud))
                    memcpy(&v.u.utrx_rx_baud, payload, sizeof(v.u.utrx_rx_baud));
                break;
            case UELYSYS_UTRX_RXCONF_GET_GUARD_CC:
            case UELYSYS_UTRX_TLM_GET_BOOT_COUNT_CC:
            case UELYSYS_UTRX_TLM_GET_TEMP_BRD_CC:
            case UELYSYS_UTRX_TLM_GET_LAST_RSSI_CC:
            case UELYSYS_UTRX_TLM_GET_LAST_RFERR_CC:
                if (payload_len != (success ? sizeof(uint16_t) : 0)) return false;
                break;
            case UELYSYS_UTRX_TLM_GET_ACTIVE_CONF_CC:
                if (payload_len != (success ? sizeof(uint8_t) : 0)) return false;
                break;
            default:
                if (payload_len != 0) return false;
                break;
        }
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC ||
        v.kind == REPORT_KIND_UELYSYS_PAYUEL_CAM) {
        const bool is_obc = v.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC;
        bool valid = false;
        if (is_obc) {
            switch (v.reflected_cc) {
                case UELYSYS_PAYUEL_OBC_NOOP_CC:
                case UELYSYS_PAYUEL_OBC_RESET_COUNTERS_CC:
                case UELYSYS_PAYUEL_OBC_SEND_OBC_BCN_CC:
                    valid = payload_len == 0;
                    break;
                case UELYSYS_PAYUEL_OBC_MOTOR_MODE_CC:
                    valid = payload_len == 0 || payload_len == 4 || payload_len == 12 || payload_len == sizeof(PAYUEL_OBC_MotorMode_Report_Payload_t);
                    break;
                case UELYSYS_PAYUEL_OBC_CAM_SHOT_CC:
                    valid = payload_len == sizeof(v.u.payload_status);
                    break;
                case UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC:
                case UELYSYS_PAYUEL_OBC_SENSOR_META_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) ||
                            payload_len == sizeof(v.u.image_meta);
                    break;
                case UELYSYS_PAYUEL_OBC_CHUNK_DOWNLOAD_CC:
                case UELYSYS_PAYUEL_OBC_SENSOR_CHUNK_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) ||
                            (payload_len >= 5 && payload_len <= 251);
                    break;
                case UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC:
                case UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC:
                    valid = payload_len == sizeof(v.u.download_request) ||
                            payload_len == sizeof(v.u.image_meta);
                    break;
                case UELYSYS_PAYUEL_OBC_REPORT_BCN_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) || payload_len == 22;
                    break;
                case UELYSYS_PAYUEL_OBC_DOWNLOAD_RANGE_CC:
                    valid = payload_len == 6;
                    break;
                default:
                    break;
            }
        } else {
            switch (v.reflected_cc) {
                case UELYSYS_PAYUEL_CAM_NOOP_CC:
                case UELYSYS_PAYUEL_CAM_RESET_COUNTERS_CC:
                case UELYSYS_PAYUEL_CAM_SEND_BCN_CC:
                    valid = payload_len == 0;
                    break;
                case UELYSYS_PAYUEL_CAM_SHOT_CC:
                case UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC:
                case UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC:
                    valid = payload_len == sizeof(v.u.payload_status);
                    break;
                case UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) ||
                            payload_len == sizeof(v.u.image_meta);
                    break;
                case UELYSYS_PAYUEL_CAM_CHUNK_DOWNLOAD_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) ||
                            (payload_len >= 5 && payload_len <= 251);
                    break;
                case UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC:
                    valid = payload_len == sizeof(v.u.download_request) ||
                            payload_len == sizeof(v.u.image_meta);
                    break;
                case UELYSYS_PAYUEL_CAM_DOWNLOAD_RANGE_CC:
                    valid = payload_len == 6;
                    break;
                case UELYSYS_PAYUEL_CAM_REPORT_BCN_CC:
                    valid = payload_len == 0 || payload_len == sizeof(v.u.payload_error) || payload_len == 12;
                    break;
                default:
                    break;
            }
        }
        if (!valid && v.ret_type != 0 && (payload_len == 1 || payload_len == 8)) valid = true;
        if (!valid) return false;

        if (payload_len == sizeof(v.u.image_meta) &&
            ((is_obc && (v.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC ||
                         v.reflected_cc == UELYSYS_PAYUEL_OBC_SENSOR_META_CC ||
                         v.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC ||
                         v.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC)) ||
             (!is_obc && (v.reflected_cc == UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC ||
                          v.reflected_cc == UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC))))
            memcpy(&v.u.image_meta, payload, sizeof(v.u.image_meta));
        else if (payload_len == sizeof(v.u.download_request) &&
                 ((is_obc && (v.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC ||
                              v.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC)) ||
                  (!is_obc && v.reflected_cc == UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC)))
            memcpy(&v.u.download_request, payload, sizeof(v.u.download_request));
        else if (payload_len == sizeof(v.u.payload_status) &&
                 ((is_obc && v.reflected_cc == UELYSYS_PAYUEL_OBC_CAM_SHOT_CC) ||
                  (!is_obc && (v.reflected_cc == UELYSYS_PAYUEL_CAM_SHOT_CC ||
                               v.reflected_cc == UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC ||
                               v.reflected_cc == UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC))))
            memcpy(&v.u.payload_status, payload, sizeof(v.u.payload_status));
        else if (payload_len == sizeof(v.u.payload_error))
            memcpy(&v.u.payload_error, payload, sizeof(v.u.payload_error));
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_LGBAT) {
        if (v.reflected_cc == UELYSYS_LGBAT_REQUEST_DATA_CC) {
            if (payload_len != 0 && payload_len != sizeof(v.u.lgbat_block)) return false;
            if (payload_len) memcpy(&v.u.lgbat_block, payload, sizeof(v.u.lgbat_block));
        } else if (v.reflected_cc == UELYSYS_LGBAT_REQUEST_ALL_DATA_CC) {
            if (payload_len != 0 && payload_len != sizeof(v.u.lgbat_all)) return false;
            if (payload_len) memcpy(&v.u.lgbat_all, payload, sizeof(v.u.lgbat_all));
        } else if (v.reflected_cc == UELYSYS_LGBAT_CRITICAL_ALERT_CC) {
            if (payload_len != sizeof(v.u.lgbat_critical)) return false;
            memcpy(&v.u.lgbat_critical, payload, sizeof(v.u.lgbat_critical));
        } else if (v.reflected_cc == UELYSYS_LGBAT_NOOP_CC ||
                   v.reflected_cc == UELYSYS_LGBAT_RESET_COUNTERS_CC ||
                   v.reflected_cc == UELYSYS_LGBAT_SEND_BCN_CC) {
            if (payload_len != 0) return false;
        } else if (v.ret_type != 0 && (payload_len == 1 || payload_len == 8)) {
            return true;
        } else if (payload_len != 0) {
            return false;
        }
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_AOS) {
        if (v.reflected_cc == UELYSYS_PAYUEL_AOS_NOOP_CC ||
            v.reflected_cc == UELYSYS_PAYUEL_AOS_RESET_COUNTERS_CC) {
            if (payload_len != sizeof(v.u.aos_noop)) return false;
            memcpy(&v.u.aos_noop, payload, sizeof(v.u.aos_noop));
        } else if (v.reflected_cc == UELYSYS_PAYUEL_AOS_READ_REGISTER_CC) {
            if (payload_len != sizeof(v.u.aos_read_register)) return false;
            memcpy(&v.u.aos_read_register, payload, sizeof(v.u.aos_read_register));
        } else if (v.reflected_cc == UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC) {
            if (payload_len != sizeof(v.u.aos_read_all)) return false;
            memcpy(&v.u.aos_read_all, payload, sizeof(v.u.aos_read_all));
        } else if (v.ret_type != 0 && (payload_len == 1 || payload_len == 8)) {
            return true;
        } else if (payload_len != 0) {
            return false;
        }
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_PAYUEL_YONSEI) {
        if (v.reflected_cc == UELYSYS_PAYUEL_YONSEI_NOOP_CC ||
            v.reflected_cc == UELYSYS_PAYUEL_YONSEI_RESET_COUNTERS_CC) {
            if (payload_len != sizeof(v.u.yonsei_status)) {
                return v.ret_type != 0 && (payload_len == 1 || payload_len == 8);
            }
            memcpy(&v.u.yonsei_status, payload, sizeof(v.u.yonsei_status));
            return true;
        }
        if (v.reflected_cc == UELYSYS_PAYUEL_YONSEI_FORWARD_SB_CC) {
            if (payload_len != sizeof(v.u.yonsei_forward)) return false;
            memcpy(&v.u.yonsei_forward, payload, sizeof(v.u.yonsei_forward));
            return true;
        }
        return v.ret_type != 0 && (payload_len == 1 || payload_len == 8);
    }

    if (v.kind == REPORT_KIND_UELYSYS_CI_LAB) {
        switch (v.reflected_cc) {
            case UELYSYS_CI_LAB_NOOP_CC:
            case UELYSYS_CI_LAB_RESET_COUNTERS_CC:
                return payload_len == 4;
            case UELYSYS_CI_LAB_CREATE_CHILD_TASK_CC:
                return payload_len == 0;
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_EO) {
        switch (v.reflected_cc) {
            case UELYSYS_EO_NOOP_CC:
                return payload_len == 1;
            case UELYSYS_EO_RESET_COUNTER_CC:
                return payload_len == 4;
            case UELYSYS_EO_RESET_PHASE_CC:
            case UELYSYS_EO_FINISH_PHASE_CC:
                return payload_len == 0 || payload_len == 1;
            case UELYSYS_EO_SEND_STATE_CC:
                if (payload_len != sizeof(v.u.eo_state)) return false;
                memcpy(&v.u.eo_state, payload, sizeof(v.u.eo_state));
                return true;
            case UELYSYS_EO_START_CHILD_TASK_CC:
                return payload_len == 0;
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_GPS) {
        /* GPS reports sizeof(size_t) for malformed packet lengths.  The FSW
         * wire ABI is 32-bit; accepting 8 also keeps captures from host-test
         * builds bounded without interpreting the value. */
        if (v.ret_type != 0 && (payload_len == 4 || payload_len == 8)) return true;
        switch (v.reflected_cc) {
            case 0:  return payload_len == 0;                         /* NOOP */
            case 12: case 13: case 14: case 15: case 16:
            case 17: case 18: case 19: case 20: case 21:
                return payload_len == 0;
            case 22: return payload_len == 0 || payload_len == 2;
            case 30: return payload_len == (v.ret_type == 0 ? sizeof(UELYSYS_GPS_OemLogHandlerHk_Report_t) : 0);
            case 31: return payload_len == (v.ret_type == 0 ? 16 : 0);
            case 32: return payload_len == 0;
            case 33: return payload_len == (v.ret_type == 0 ? 1 : 0);
            case 34: case 35: case 36: case 37: case 38: case 39:
                return payload_len == 0;
            case 50: return payload_len == 16;
            case 51: return payload_len == 0;
            case 52: return payload_len == 0 || payload_len == 64;
            case 53: case 54: return payload_len == 0;
            case 55: return payload_len == 2;
            case 56: return payload_len == (v.ret_type == 0 ? 16 : 0);
            case 57: return payload_len == 0;
            case 58: return payload_len <= 256;
            case 59: case 60: return payload_len == 0;
            default:
                return v.ret_type != 0 && payload_len == 1;
        }
    }

    if (v.kind == REPORT_KIND_BEE1012_LTRX) {
        switch (v.reflected_cc) {
            case LTRX_RESET_COUNTERS_CC:
            case LTRX_RESET_APP_CMD_COUNTERS_CC:
            case LTRX_RESET_DEVICE_CMD_COUNTERS_CC:
            case LTRX_SESSION_START_DOWNLINK_CC:
            case LTRX_SESSION_ABORT_CC:
            case LTRX_SESSION_RESET_STATE_CC:
            case LTRX_DOWNSTREAM_ENABLE_CC:
            case LTRX_DOWNSTREAM_DISABLE_CC:
                return payload_len == 0;
            case LTRX_SET_BUS_BEACON_PERIOD_CC:
                return payload_len == 0 || payload_len == sizeof(uint16_t);
            case LTRX_NOOP_CC:
                /* LTRX_HandleReport() hardcodes CommandCode=0 for both the NOOP
                 * echo string (14B) and the periodic HK snapshot sent through the
                 * same function when triggered by SEND_HK - disambiguate by size. */
                return payload_len == 0 || payload_len == 14 || payload_len == sizeof(LTRX_HkSnapshot_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_BEE1012_KISSCAM) {
        switch (v.reflected_cc) {
            case PAYBEE_KISSCAM_NOOP_CC:
            case PAYBEE_KISSCAM_RESET_COUNTERS_CC:
                return payload_len == 0 || payload_len == 2;
            default:
                /* PING/SET_MODE/MEMORY_STATUS/SET_EXPOSURE/CAPTURE/READ_REGISTER/
                 * WRITE_REGISTER/DOWNLOAD/DOWNLOAD_ALL/IMAGE_COMPRESS echo raw, undocumented
                 * camera-protocol bytes of varying length - always accept and let
                 * the renderer show the hex dump. */
                return true;
        }
    }

    if (v.kind == REPORT_KIND_BEE1012_PAY_SLT) {
        return ValidatePaySltReportPayload(v, payload, payload_len);
    }

    if (v.kind == REPORT_KIND_BEE1012_THRUST) {
        switch (v.reflected_cc) {
            case BEE1012_THRUST_NOOP_CC:
            case BEE1012_THRUST_RESET_COUNTERS_CC:
                return payload_len == 0;
            case BEE1012_THRUST_PING_CC:
            case BEE1012_THRUST_RESET_MODULE_CC:
            case BEE1012_THRUST_SET_MODE_CC:
            case BEE1012_THRUST_ARM_CC:
            case BEE1012_THRUST_DISARM_CC:
            case BEE1012_THRUST_MAIN_FIRE_CC:
            case BEE1012_THRUST_MAIN_ABORT_CC:
            case BEE1012_THRUST_CG_PULSE_CC:
            case BEE1012_THRUST_CG_ABORT_CC:
            case BEE1012_THRUST_CLEAR_FAULT_CC:
                return payload_len == 0 || payload_len == sizeof(THRUST_DefaultResponse_Report_t);
            case BEE1012_THRUST_REQ_STATUS_CC:
                return payload_len == 0 || payload_len == sizeof(THRUST_StatusData_Report_t);
            case BEE1012_THRUST_REQ_FAULT_LOG_CC:
                return payload_len == 0 || payload_len == 40;
            case BEE1012_THRUST_REQ_HK_CC:
                return payload_len == 0 || payload_len == sizeof(THRUST_HkData_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_PAYYONSEI_TEMP) {
        switch (v.reflected_cc) {
            case UELYSYS_PAYYONSEI_TEMP_NOOP_CC:
            case UELYSYS_PAYYONSEI_TEMP_RESET_COUNTERS_CC:
                return payload_len == 2;
            case UELYSYS_PAYYONSEI_TEMP_READ_TEMP_CC:
                return payload_len == sizeof(TEMP_DualTemp_Report_t);
            case UELYSYS_PAYYONSEI_TEMP_READ_CONFIG_CC:
                return payload_len == sizeof(TEMP_DualConfig_Report_t);
            case UELYSYS_PAYYONSEI_TEMP_READ_LIMITS_CC:
                return payload_len == sizeof(TEMP_DualLimits_Report_t);
            case UELYSYS_PAYYONSEI_TEMP_READ_ID_CC:
                return payload_len == sizeof(TEMP_DualId_Report_t);
            case UELYSYS_PAYYONSEI_TEMP_SET_CONFIG_CC:
            case UELYSYS_PAYYONSEI_TEMP_SET_LIMITS_CC:
            case UELYSYS_PAYYONSEI_TEMP_SET_RESOLUTION_CC:
                return payload_len == 0;
            case UELYSYS_PAYYONSEI_TEMP_START_LOG_CC:
                return payload_len == sizeof(TEMP_LogStart_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_PAYYONSEI_IMU) {
        switch (v.reflected_cc) {
            case UELYSYS_PAYYONSEI_IMU_NOOP_CC:
            case UELYSYS_PAYYONSEI_IMU_RESET_COUNTERS_CC:
            case UELYSYS_PAYYONSEI_IMU_RESET_MODULE_CC:
            case UELYSYS_PAYYONSEI_IMU_SEND_DATA_CC:
            case UELYSYS_PAYYONSEI_IMU_START_LOGGING_CC:
            case UELYSYS_PAYYONSEI_IMU_MEASURE_CC:
            case UELYSYS_PAYYONSEI_IMU_STOP_LOGGING_CC:
                return payload_len == sizeof(IMU_CommandResult_Report_t);
            case UELYSYS_PAYYONSEI_IMU_RUN_SELFTEST_CC:
                return payload_len == sizeof(IMU_SelftestResult_Report_t);
            case UELYSYS_PAYYONSEI_IMU_SAVE_LOG_FILE_CC:
                /* Success echoes the saved file path (variable length); failure
                 * paths call IMU_ReportCommandResult() with the 16B status struct. */
                return payload_len == sizeof(IMU_CommandResult_Report_t) || payload_len <= 64;
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_PAYYONSEI_IWSB) {
        switch (v.reflected_cc) {
            case UELYSYS_PAYYONSEI_IWSB_NOOP_CC:
                return payload_len == 4;
            case UELYSYS_PAYYONSEI_IWSB_RESET_COUNTERS_CC:
                return payload_len == 0;
            case UELYSYS_PAYYONSEI_IWSB_MISSION_START_CC:
                return payload_len == 1;
            case UELYSYS_PAYYONSEI_IWSB_ERASE_SLOT_CC:
            case UELYSYS_PAYYONSEI_IWSB_VERIFY_SLOT_CC:
            case UELYSYS_PAYYONSEI_IWSB_UPDATE_FW_CC:
                return payload_len == 1;
            case UELYSYS_PAYYONSEI_IWSB_WRITE_FW_CC:
                return payload_len == sizeof(IWSB_WriteFirmware_Report_t);
            case UELYSYS_PAYYONSEI_IWSB_GET_CURRENT_FW_CC:
                return payload_len == 0 || payload_len == 1;
            case UELYSYS_PAYYONSEI_IWSB_GET_THERMAL_CC:
                return payload_len == 0 || payload_len == sizeof(IWSB_Thermal_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_PAYYONSEI_I2C) {
        switch (v.reflected_cc) {
            case UELYSYS_PAYYONSEI_I2C_NOOP_CC:
            case UELYSYS_PAYYONSEI_I2C_RESET_COUNTERS_CC:
                return payload_len == 2;
            case UELYSYS_PAYYONSEI_I2C_SET_THRESHOLD_CC:
                return payload_len == sizeof(I2C_ThresholdReport_Report_t);
            case UELYSYS_PAYYONSEI_I2C_CLEAR_LATCH_CC:
                return payload_len == sizeof(I2C_ClearLatchReport_Report_t);
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_SDR_CONTROLLER) {
        switch (v.reflected_cc) {
            case UELYSYS_SDR_CONTROLLER_RESET_COUNTERS_CC:
                return payload_len == 2;
            case UELYSYS_SDR_CONTROLLER_SET_PHASE_SHIFT_CC:
                return payload_len == 0 || payload_len == sizeof(SDRC_SetPhaseShift_Report_t);
            case UELYSYS_SDR_CONTROLLER_PSDR_GET_SLOT_INFO_CC:
                return payload_len == 0 || payload_len == 1 || payload_len == 4 ||
                       payload_len == sizeof(SDRC_SlotInfo_Report_t);
            case UELYSYS_SDR_CONTROLLER_PSDR_GET_CUR_FW_CC:
                return payload_len == 0 || payload_len == 1 || payload_len == 4 ||
                       payload_len == sizeof(SDRC_FwVersion_Report_t);
            case UELYSYS_SDR_CONTROLLER_PSDR_GET_THERMAL_CC:
                return payload_len == 0 || payload_len == 1 || payload_len == 4 ||
                       payload_len == sizeof(SDRC_Thermal_Report_t);
            default:
                /* NOOP echoes the version string; AMP/PSDR power, file-mgmt and
                 * PSDR erase/write/verify/update commands echo a plain int32
                 * result, a single ack byte, or an opaque I2C/SPI device reply -
                 * always accept and let the renderer fall back to raw bytes. */
                return true;
        }
    }

    if (v.kind == REPORT_KIND_SD1SAT_CR) {
        switch (v.reflected_cc) {
            case SD1SAT_CR_NOOP_CC:
            case SD1SAT_CR_RESET_COUNTERS_CC:
                return payload_len == 0;
            case SD1SAT_CR_RETRANSMIT_LATEST_CC:
            case SD1SAT_CR_RETRANSMIT_APP_CC:
            case SD1SAT_CR_RETRANSMIT_APPCMD_CC:
                return true;
            case SD1SAT_CR_DUMP_QUEUE_CC:
                return payload_len == 0 || payload_len == sizeof(SD1SAT_CR_DumpQueue_tlm_Payload_t);
            case SD1SAT_CR_SET_DOWNLINK_MODE_CC:
                return payload_len == sizeof(SD1SAT_CR_SetDownlinkMode_tlm_Payload_t);
            case SD1SAT_CR_GET_FULL_STATES_CC:
                return payload_len == sizeof(SD1SAT_CR_GetFullReportState_tlm_Payload_t);
            default:
                return false;
        }
    }
    
    if (v.kind == REPORT_KIND_SD1SAT_EPS) {
        switch(v.reflected_cc) {
            case SD1_EPS_PING_CC :
                return payload_len == sizeof(SD1_EPS_PING_tlm_Payload_t);
            case SD1_EPS_IDENT_CC :
                return payload_len == sizeof(SD1_EPS_IDENT_tlm_Payload_t);
            case SD1_EPS_MEMFREE_CC :
                return payload_len == sizeof(SD1_EPS_MEMFREE_tlm_Payload_t);
            case SD1_EPS_BUFFREE_CC :
                return payload_len == sizeof(SD1_EPS_BUFFREE_tlm_Payload_t);
            case SD1_EPS_HK_SET_MODE_CC :
                return payload_len == sizeof(SD1_EPS_HkSetMode_tlm_Payload_t);
            case SD1_EPS_HK_GET_MODE_CC :
                return payload_len == sizeof(SD1_EPS_HkGetMode_tlm_Payload_t);
            case SD1_EPS_CHANNEL_GET_CC :
                return payload_len == sizeof(SD1_EPS_CHANNEL_GET_tlm_Payload_t);
            case SD1_EPS_CHANNELS_GET_CC :
                return payload_len == sizeof(SD1_EPS_CHANNELS_GET_tlm_Payload_t);
            case SD1_EPS_TABLE_ACQUIRE_CC :
                return payload_len == sizeof(SD1_EPS_TABLE_ACQUIRE_tlm_Payload_t);
            case SD1_EPS_WDT_GET_CC :
                return payload_len == sizeof(SD1_EPS_WDT_GET_tlm_Payload_t);
            case SD1_EPS_TABLE_TO_FILE_CC :
                return true;
            default :
                if (v.ret_code == 0) return true;
                else return false;
        }
    }

    if (v.kind == REPORT_KIND_SD1SAT_SNBP) {
        switch(v.reflected_cc) {
            case SD1SAT_SNBP_NOOP_CC :
            case SD1SAT_SNBP_RESET_COUNTERS_CC :
            case SD1SAT_SNBP_RAMM_STORE_CC :
            case SD1SAT_SNBP_REMM_STORE_CC :
            case SD1SAT_SNBP_RAMM_SET_PERIOD_CC :
            case SD1SAT_SNBP_REMM_SET_PERIOD_CC :
                return payload_len == 0;
            case SD1SAT_SNBP_RAMM_GET_DPSD_CC :
            case SD1SAT_SNBP_RAMM_GET_ADSD_CC :
            case SD1SAT_SNBP_RAMM_GET_HKSD_CC :
            case SD1SAT_SNBP_RAMM_GET_BESD_CC :
            case SD1SAT_SNBP_REMM_GET_POSD_CC :
            case SD1SAT_SNBP_REMM_GET_BESD_CC :
            case SD1SAT_SNBP_REMM_GET_ADSD_CC :
                return payload_len <= sizeof(SD1SAT_SNBP_Reading_t);
            default :
                return false;
        }
    }

    if (v.kind == REPORT_KIND_SD1SAT_UTRX) {
        switch(v.reflected_cc) {
            case SD1SAT_UTRX_NOOP_CC :
            case SD1SAT_UTRX_RESET_COUNTERS_CC :
            case SD1SAT_UTRX_AX100_GNDWDT_CLEAR_CC :
            case SD1SAT_UTRX_AX100_SET_TX_BAUD_CC :
            case SD1SAT_UTRX_AX100_SET_RX_BAUD_CC :
            case SD1SAT_UTRX_AX100_REBOOT_CC :
            case SD1SAT_UTRX_AX100_TABLE_SAVE_CC :
            case SD1SAT_UTRX_AX100_TABLE_LOAD_CC :
            case SD1SAT_UTRX_AX100_SET_CC :
            case SD1SAT_UTRX_AX100_SET_RAW_CC :
                return payload_len == 0;
            case SD1SAT_UTRX_AX100_PING_CC :
                return payload_len == sizeof(SD1SAT_UTRX_PingPayload_t);
            case SD1SAT_UTRX_AX100_GNDWDT_GET_CC :
                return payload_len == sizeof(SD1SAT_UTRX_GndWdtGetPayload_t);
            case SD1SAT_UTRX_AX100_GET_DIAG_CC : 
                return payload_len == sizeof(SD1SAT_UTRX_Diagnosis_t);
            case SD1SAT_UTRX_AX100_TABLE_GET_CC :
                return payload_len <= sizeof(SD1SAT_UTRX_TableGetPayload_t);
            case SD1SAT_UTRX_AX100_IDENT_CC :
                return payload_len == sizeof(SD1SAT_UTRX_ident_t);
            case SD1SAT_UTRX_AX100_MEMFREE_CC :
                return payload_len == sizeof(SD1SAT_UTRX_MemFreePayload_t);
            case SD1SAT_UTRX_AX100_BUFFREE_CC :
                return payload_len == sizeof(SD1SAT_UTRX_BufFreePayload_t);
            case SD1SAT_UTRX_AX100_GET_CC :
                return payload_len <= sizeof(SD1SAT_UTRX_GetPayload_t);
            case SD1SAT_UTRX_AX100_GET_RAW_CC :
                return payload_len <= sizeof(SD1SAT_UTRX_GetRawPayload_t);
            case SD1SAT_UTRX_AX100_HK_SET_MODE_CC :
                return payload_len == sizeof(SD1SAT_UTRX_HkSetModePayload_t);
            case SD1SAT_UTRX_AX100_HK_GET_MODE_CC :
                return payload_len == sizeof(SD1SAT_UTRX_HkGetModePayload_t);
            default :
                return false;
        }
    }

    if (
        v.kind == REPORT_KIND_SD1SAT_GPS || v.kind == REPORT_KIND_SD1SAT_MEOW ||
        v.kind == REPORT_KIND_SD1SAT_OPS || v.kind == REPORT_KIND_SD1SAT_PAYSD1_LOTHAN ||
        v.kind == REPORT_KIND_SD1SAT_PAYSD1_ROMA || v.kind == REPORT_KIND_SD1SAT_SNBP ||
        v.kind == REPORT_KIND_SD1SAT_TTC || v.kind == REPORT_KIND_SD1SAT_UTRX) {
        /* AssembleSD1SATReportPortChunk() already framed this packet by its own
         * CCSDS-like length field (see ParseSD1SATReportWire), so payload_len is
         * trustworthy by construction - no size table to maintain per CC here.
         * NOOP/RESET_COUNTERS send DataLength=0 in every SD1SAT app (RetVal alone
         * carries success/failure); everything else is rendered as labeled raw
         * bytes in the GUI until a command warrants a dedicated struct. */
        (void)payload;
        return true;
    }

    if (v.kind == REPORT_KIND_UELYSYS_SCH_LAB) {
        switch (v.reflected_cc) {
            case UELYSYS_SCH_LAB_NOOP_CC:
            case UELYSYS_SCH_LAB_RESET_COUNTERS_CC:
                return payload_len == 0;
            case UELYSYS_SCH_LAB_SET_ENTRY_STATE_CC:
            case UELYSYS_SCH_LAB_SET_ENTRY_CONFIG_CC:
                if (payload_len == 0 && v.ret_type != 0) return true;
                if (payload_len != sizeof(v.u.sch_entry)) return false;
                memcpy(&v.u.sch_entry, payload, sizeof(v.u.sch_entry));
                return true;
            default:
                return false;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_SP) {
        if (v.ret_type != 0 && payload_len == 8) return true;
        switch (v.reflected_cc) {
            case UELYSYS_SP_NOOP_CC:
            case UELYSYS_SP_RESET_COUNTERS_CC:
                return payload_len == 2;
            case UELYSYS_SP_GET_AR6_STATUS_CC:
            case UELYSYS_SP_RESET_AR6_COUNTERS_CC:
                /* SP_ResetAr6CountersCmd() re-queries status after the reset and
                 * reports back the same SP_AR6_Readback_Payload_t as GET_AR6_STATUS. */
                if (payload_len == 0 && v.ret_type != 0) return true;
                if (payload_len != sizeof(v.u.sp_readback)) return false;
                memcpy(&v.u.sp_readback, payload, sizeof(v.u.sp_readback));
                return true;
            case UELYSYS_SP_DEPLOY_CC:
            case UELYSYS_SP_STOP_BURN_CC:
            case UELYSYS_SP_AUTO_DEPLOY_CC:
            case UELYSYS_SP_SET_AR6_ADDR_CC:
            case UELYSYS_SP_SCAN_AR6_CC:
            case UELYSYS_SP_ALL_STOP_BURN_CC:
                return payload_len == 0;
            default:
                return v.ret_type != 0 && payload_len == 1;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_TO_LAB) {
        switch (v.reflected_cc) {
            case UELYSYS_TO_LAB_NOOP_CC:
            case UELYSYS_TO_LAB_RESET_STATUS_CC:
                return payload_len == 2;
            case UELYSYS_TO_LAB_ADD_PKT_CC:
            case UELYSYS_TO_LAB_REMOVE_PKT_CC:
                return payload_len == 4;
            case UELYSYS_TO_LAB_SEND_DATA_TYPES_CC:
                return payload_len == 0;
            case UELYSYS_TO_LAB_REMOVE_ALL_PKT_CC:
                return payload_len == 0 || payload_len == 2;
            case UELYSYS_TO_LAB_OUTPUT_ENABLE_CC:
            case UELYSYS_TO_LAB_OUTPUT_DISABLE_CC:
                return payload_len == 1;
            default:
                return v.ret_type != 0 && payload_len == 1;
        }
    }

    if (v.kind == REPORT_KIND_UELYSYS_TTC ||
        v.kind == REPORT_KIND_UELYSYS_MEOW ||
        v.kind == REPORT_KIND_UELYSYS_STX ||
        v.kind == REPORT_KIND_UELYSYS_UANT ||
        v.kind == REPORT_KIND_UELYSYS_GPIO ||
        v.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC ||
        v.kind == REPORT_KIND_UELYSYS_PAYUEL_CAM) {
        const uint16_t copy_len =
            (payload_len > sizeof(v.u.uelysys.bytes)) ? (uint16_t)sizeof(v.u.uelysys.bytes) : payload_len;
        v.u.uelysys.size = copy_len;
        memset(v.u.uelysys.bytes, 0, sizeof(v.u.uelysys.bytes));
        memcpy(v.u.uelysys.bytes, payload, copy_len);
        return true;
    }

    if (v.kind == REPORT_KIND_GPIO_DEP1_EN_ON ||
        v.kind == REPORT_KIND_GPIO_DEP1_EN_OFF ||
        v.kind == REPORT_KIND_GPIO_DEP2_EN_ON ||
        v.kind == REPORT_KIND_GPIO_DEP2_EN_OFF ||
        v.kind == REPORT_KIND_GPIO_SP_IN_READ_5S) {
        return payload_len == 0;
    }

    if (v.kind == REPORT_KIND_UANT_GET_STATUS_TLM) {
        if (payload_len < sizeof(gs_gssb_ant6_release_status_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.uant_getstatus, payload, sizeof(gs_gssb_ant6_release_status_t));
            return true;
        }
    }


    if (v.kind == REPORT_KIND_ADCS_LOG_MASK) {
        if (payload_len != sizeof(ADCS_TlmLogInclMaskTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_logmask, payload, sizeof(ADCS_TlmLogInclMaskTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_UNSOLICIT_TLM_SETUP_TLM) {
        if (payload_len < sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_unsolicited_tlm_tlm, payload, sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t));
            return true;
        }
    }


    if (v.kind == REPORT_KIND_EPS_P60_DOCK_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_DOCK_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60dockgettablehk, payload, sizeof(EPS_P60_DOCK_GET_TABLE_HK));
            return true;
        }
    }


        if (v.kind == REPORT_KIND_EPS_P60_PDU_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_PDU_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60pdugettablehk, payload, sizeof(EPS_P60_PDU_GET_TABLE_HK));
            return true;
        }
    }

        if (v.kind == REPORT_KIND_EPS_P60_ACU_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_ACU_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60acugettablehk, payload, sizeof(EPS_P60_ACU_GET_TABLE_HK));
            return true;
        }
    }

    /****************************************************************************************************************************** */
    if (v.kind == REPORT_KIND_ADCS_NOOP_REPORT)
    {
        if (payload_len == sizeof(ADCS_HkTlm_Payload_t))
        {
            v.kind = REPORT_KIND_ADCS_HK_REPORT;
            memcpy(&v.u.adcs_hk, payload, sizeof(v.u.adcs_hk));
            return true;
        }
        /* BEE1012 returns "Yosi In Space\0" (14 bytes); UELYSYS returns
         * CmdCounter/ErrCounter (2 bytes). */
        return payload_len == 14 || payload_len == 2;
    }

    if (v.kind == REPORT_KIND_ADCS_RESET_COUNTERS_REPORT)
        return payload_len == 2;

    if (v.kind == REPORT_KIND_ADCS_INTERFACE_TRANSPORT_REPORT)
    {
        if (payload_len != sizeof(v.u.adcs_interface_transport)) return false;
        memcpy(&v.u.adcs_interface_transport, payload, sizeof(v.u.adcs_interface_transport));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GYRO_LOG_REPORT)
    {
        if (payload_len != sizeof(v.u.adcs_gyro_log)) return false;
        memcpy(&v.u.adcs_gyro_log, payload, sizeof(v.u.adcs_gyro_log));
        return true;
    }

    /* Set commands and sequence/commissioning commands normally return an
     * empty status payload. Some sequences emit several stage-specific
     * payloads under the same CC, so the existing raw view is retained. */
    if (v.kind == REPORT_KIND_ADCS_COMMAND_REPORT) return true;

    if (v.kind == REPORT_KIND_ADCS_GET_ERROR_LOG_SETTING)
    {
        if (payload_len != sizeof(ADCS_ErrorLogSettingTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_errorlogsetting, payload, sizeof(ADCS_ErrorLogSettingTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CURRENT_UNIX_TIME)
    {
        if (payload_len != sizeof(ADCS_CurrentUnixTimeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_currentunixtime, payload, sizeof(ADCS_CurrentUnixTimeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_PERSIST_CONFIG_DIAGNOSTIC)
    {
        if (payload_len != sizeof(ADCS_PersistConfigDiagnosticTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_persistconfigdiagnostic, payload, sizeof(ADCS_PersistConfigDiagnosticTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_COMMUNICATION_STATUS)
    {
        if (payload_len != sizeof(ADCS_CommunicationStatusTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_communicationstatus, payload, sizeof(ADCS_CommunicationStatusTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CONTROL_ESTIMATION_MODE)
    {
        if (payload_len != sizeof(ADCS_ControlEstimationModeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_controlestimationmode, payload, sizeof(ADCS_ControlEstimationModeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_REFERENCE_IRC_VECTOR)
    {
        if (payload_len != sizeof(ADCS_ReferenceIRCVectorTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_referenceircvector, payload, sizeof(ADCS_ReferenceIRCVectorTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_REFERENCE_LLH_TARGET)
    {
        if (payload_len != sizeof(ADCS_ReferenceLLHTargetTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_referencellhtarget, payload, sizeof(ADCS_ReferenceLLHTargetTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_ORBIT_MODE)
    {
        if (payload_len != sizeof(ADCS_OrbitModeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_orbitmode, payload, sizeof(ADCS_OrbitModeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_HEALTH_TLM_MMT)
    {
        if (payload_len != sizeof(ADCS_HealthTlmMMTTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_healthtlmmmt, payload, sizeof(ADCS_HealthTlmMMTTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_RAW_CUBESENSE_SUN)
    {
        if (payload_len == sizeof(ADCS_RawCalibratedFSSReport_Payload_t))
        {
            v.kind = REPORT_KIND_ADCS_GET_RAW_CALIBRATED_FSS;
            memcpy(&v.u.adcs_rawcalibratedfss, payload, sizeof(v.u.adcs_rawcalibratedfss));
            return true;
        }
        if (payload_len != sizeof(ADCS_RawCubeSenseSunTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_rawcubesensesun, payload, sizeof(ADCS_RawCubeSenseSunTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_REFERENCE_RPY_VALUES)
    {
        if (payload_len != sizeof(ADCS_ReferenceRPYvaluesTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_referencerpyvalues, payload, sizeof(ADCS_ReferenceRPYvaluesTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_OPENLOOPCMD_MTQ)
    {
        if (payload_len != sizeof(ADCS_OpenLoopCmdMTQTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_openloopcmdmtq, payload, sizeof(ADCS_OpenLoopCmdMTQTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_POWER_STATE)
    {
        if (payload_len != sizeof(ADCS_PowerStateTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_powerstate, payload, sizeof(ADCS_PowerStateTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_RUN_MODE)
    {
        if (payload_len != sizeof(ADCS_RunModeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_runmode, payload, sizeof(ADCS_RunModeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CONTROL_MODE)
    {
        if (payload_len != sizeof(ADCS_ControlModeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_controlmode, payload, sizeof(ADCS_ControlModeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MAG0_MMT_CALIB_CONFIG)
    {
        if (payload_len != sizeof(ADCS_Mag0MMTCalibConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_mag0mmtcalibconfig, payload, sizeof(ADCS_Mag0MMTCalibConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MAG1_MMT_CALIB_CONFIG)
    {
        if (payload_len != sizeof(ADCS_Mag1MMTCalibConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_mag1mmtcalibconfig, payload, sizeof(ADCS_Mag1MMTCalibConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_ESTIMATION_MODE)
    {
        if (payload_len != sizeof(ADCS_EstimationModeTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_estimationmode, payload, sizeof(ADCS_EstimationModeTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_OPERATIONAL_STATE)
    {
        if (payload_len != sizeof(ADCS_OperationalStateTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_operationalstate, payload, sizeof(ADCS_OperationalStateTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_RAW_CSS_SENSOR)
    {
        if (payload_len == sizeof(ADCS_RawCalibratedCSSReport_Payload_t))
        {
            v.kind = REPORT_KIND_ADCS_GET_RAW_CALIBRATED_CSS;
            memcpy(&v.u.adcs_rawcalibratedcss, payload, sizeof(v.u.adcs_rawcalibratedcss));
            return true;
        }
        if (payload_len != sizeof(ADCS_RawCSSSensorTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_rawcsssensor, payload, sizeof(ADCS_RawCSSSensorTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_RAW_GYR_SENSOR)
    {
        if (payload_len == sizeof(ADCS_RawCalibratedGYRReport_Payload_t))
        {
            v.kind = REPORT_KIND_ADCS_GET_RAW_CALIBRATED_GYR;
            memcpy(&v.u.adcs_rawcalibratedgyr, payload, sizeof(v.u.adcs_rawcalibratedgyr));
            return true;
        }
        if (payload_len != sizeof(ADCS_RawGYRSensorTlm_Paylaod_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_rawgyrsensor, payload, sizeof(ADCS_RawGYRSensorTlm_Paylaod_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CALIBRATED_GYR_SENSOR)
    {
        if (payload_len != sizeof(ADCS_CalibratedGYRSensorTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_calibratedgyrsensor, payload, sizeof(ADCS_CalibratedGYRSensorTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG)
    {
        if (payload_len != sizeof(ADCS_MagSensingElmConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_magsensingelmconfig, payload, sizeof(ADCS_MagSensingElmConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_TLM_LOG_INCLMASK)
    {
        if (payload_len != sizeof(ADCS_TlmLogInclMaskTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_tlmloginclmask, payload, sizeof(ADCS_TlmLogInclMaskTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_UNSOLICIT_TLM_MSG_SETUP)
    {
        if (payload_len != sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_unsolicittlmmsgsetup, payload, sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_UNSOLICIT_EVENT_MSG_SETUP)
    {
        if (payload_len != sizeof(ADCS_UnsolicitEventMsgSetupTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_unsoliciteventmsgsetup, payload, sizeof(ADCS_UnsolicitEventMsgSetupTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_EVENT_LOG_STATUS_RESPONSE)
    {
        if (payload_len != sizeof(ADCS_EventLogStatusResponseTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_eventlogstatusresponse, payload, sizeof(ADCS_EventLogStatusResponseTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_PORTMAP)
    {
        if (payload_len != sizeof(ADCS_PortMapTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.adcs_portmap, payload, sizeof(ADCS_PortMapTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_WHL_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_whlconfig)) return false;
        memcpy(&v.u.adcs_whlconfig, payload, sizeof(v.u.adcs_whlconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_SATELLITE_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_satelliteconfig)) return false;
        memcpy(&v.u.adcs_satelliteconfig, payload, sizeof(v.u.adcs_satelliteconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_controllerconfig)) return false;
        memcpy(&v.u.adcs_controllerconfig, payload, sizeof(v.u.adcs_controllerconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_defaultmodeconfig)) return false;
        memcpy(&v.u.adcs_defaultmodeconfig, payload, sizeof(v.u.adcs_defaultmodeconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MOUNTING_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_mountingconfig)) return false;
        memcpy(&v.u.adcs_mountingconfig, payload, sizeof(v.u.adcs_mountingconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_estimatorconfig)) return false;
        memcpy(&v.u.adcs_estimatorconfig, payload, sizeof(v.u.adcs_estimatorconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_satorbitparamconfig)) return false;
        memcpy(&v.u.adcs_satorbitparamconfig, payload, sizeof(v.u.adcs_satorbitparamconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_nodeselectionconfig)) return false;
        memcpy(&v.u.adcs_nodeselectionconfig, payload, sizeof(v.u.adcs_nodeselectionconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MTQ_CONFIG)
    {
        if (payload_len != sizeof(v.u.adcs_mtqconfig)) return false;
        memcpy(&v.u.adcs_mtqconfig, payload, sizeof(v.u.adcs_mtqconfig));
        return true;
    }

    if (v.kind == REPORT_KIND_ADCS_GET_RAW_RWL_SENSOR)
    {
        if (payload_len == sizeof(ADCS_RawCalibratedRWLReport_Payload_t))
        {
            v.kind = REPORT_KIND_ADCS_GET_RAW_CALIBRATED_RWL;
            memcpy(&v.u.adcs_rawcalibratedrwl, payload, sizeof(v.u.adcs_rawcalibratedrwl));
            return true;
        }
        if (payload_len != sizeof(v.u.adcs_rawrwlsensor)) return false;
        memcpy(&v.u.adcs_rawrwlsensor, payload, sizeof(v.u.adcs_rawrwlsensor));
        return true;
    }
/**************************************************************************************************************************************************** */
    
/******************************************************5차 추가**************************************************************/

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_NOOP)
    {
        if (payload_len != sizeof(payuel_roma_Noop_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_noop, payload, sizeof(payuel_roma_Noop_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_RESETCOUNTERS)
    {
        if (payload_len != sizeof(payuel_roma_ResetCounters_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_resetcounters, payload, sizeof(payuel_roma_ResetCounters_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_COMMTEST)
    {
        if (payload_len == sizeof(payuel_roma_CommTest_tlm_payload_t)) {
            memcpy(&v.u.roma_commtest, payload, sizeof(payuel_roma_CommTest_tlm_payload_t));
            return true;
        }
        if (payload_len <= sizeof(payuel_roma_CommTest_tlm_payload_t)) {
            v.kind = REPORT_KIND_PAYUEL_ROMA_GENERIC;
            return true;
        }
        return false;
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_GETSPECIFICLINE)
    {
        /* FSW transmits packed 12-byte line metadata followed by a variable string. */
        v.kind = REPORT_KIND_PAYUEL_ROMA_GENERIC;
        return payload_len == 0 || (payload_len >= 12 && payload_len <= 256);
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_GETMULTIPLELINES)
    {
        /* Concatenated variable-length log records have no outer record count. */
        v.kind = REPORT_KIND_PAYUEL_ROMA_GENERIC;
        return payload_len <= 512;
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_GETLATESTLINE)
    {
        v.kind = REPORT_KIND_PAYUEL_ROMA_GENERIC;
        return payload_len == 0 || (payload_len >= 12 && payload_len <= 256);
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_GETLATESTNLINES)
    {
        v.kind = REPORT_KIND_PAYUEL_ROMA_GENERIC;
        return payload_len <= 512;
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SETROUTEDEFAULT)
    {
        if (payload_len != sizeof(payuel_roma_SetRouteDefault_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_setroutedefault, payload, sizeof(payuel_roma_SetRouteDefault_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_RESETROUTE)
    {
        if (payload_len != sizeof(payuel_roma_ResetRoute_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_resetroute, payload, sizeof(payuel_roma_ResetRoute_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_LOADROUTE)
    {
        if (payload_len != sizeof(payuel_roma_LoadRoute_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_loadroute, payload, sizeof(payuel_roma_LoadRoute_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SAVEROUTE)
    {
        if (payload_len != sizeof(payuel_roma_SaveRoute_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_saveroute, payload, sizeof(payuel_roma_SaveRoute_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SENDROUTE)
    {
        if (payload_len != sizeof(payuel_roma_SendRoute_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_sendroute, payload, sizeof(payuel_roma_SendRoute_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARGET)
    {
        if (payload_len != sizeof(payuel_roma_ParGet_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parget, payload, sizeof(payuel_roma_ParGet_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARSET)
    {
        if (payload_len != sizeof(payuel_roma_ParSet_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parset, payload, sizeof(payuel_roma_ParSet_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARDEFAULTS)
    {
        if (payload_len != sizeof(payuel_roma_ParDefaults_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_pardefaults, payload, sizeof(payuel_roma_ParDefaults_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARSAVE)
    {
        if (payload_len != sizeof(payuel_roma_ParSave_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parsave, payload, sizeof(payuel_roma_ParSave_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARRESTORE)
    {
        if (payload_len != sizeof(payuel_roma_ParRestore_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parrestore, payload, sizeof(payuel_roma_ParRestore_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARLOAD)
    {
        if (payload_len != sizeof(payuel_roma_ParLoad_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parload, payload, sizeof(payuel_roma_ParLoad_tlm_payload_t));
            return true;
        }
    }
    
    if (v.kind == REPORT_KIND_PAYUEL_ROMA_PARSETOOB)
    {
        if (payload_len != sizeof(payuel_roma_ParSetOOB_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_parsetOOB, payload, sizeof(payuel_roma_ParSetOOB_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SENDCOMMAND)
    {
        if (payload_len != sizeof(payuel_roma_SendCommand_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.roma_sendcommand, payload, sizeof(payuel_roma_SendCommand_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SETROUTE)
    {
        if (payload_len != sizeof(payuel_roma_SetRoute_tlm_payload_t))
            v.kind = REPORT_KIND_SC_GENERIC;
        else {
            memcpy(&v.u.roma_setroute, payload, sizeof(payuel_roma_SetRoute_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_SENDMSG ||
        v.kind == REPORT_KIND_PAYUEL_ROMA_PAYINIT)
        return payload_len == 0;

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_ISL_TEST_COUNT) {
        if (payload_len != sizeof(v.u.roma_isl_count)) return false;
        memcpy(&v.u.roma_isl_count, payload, sizeof(v.u.roma_isl_count));
        return true;
    }

    if (v.kind == REPORT_KIND_PAYUEL_ROMA_GENERIC) {
        switch (v.reflected_cc) {
            case PAYUEL_ROMA_CLOCK_SYNC_CC:
                return payload_len == 0 || payload_len == 9;
            case PAYUEL_ROMA_LOG_TEST_CC:
                return payload_len == 0 || payload_len == sizeof(uint16_t);
            case PAYUEL_ROMA_TRANS_TEST_CC:
                return payload_len == 0 || payload_len == sizeof(uint8_t);
            case PAYUEL_ROMA_CLEAR_ALL_LINES_CC:
            case PAYUEL_ROMA_SEND_MSG_CC:
            case PAYUEL_ROMA_SYNC_RX_CC:
            case PAYUEL_ROMA_SYNC_TX_CC:
            case PAYUEL_ROMA_PAY_INIT_CC:
            case PAYUEL_ROMA_LISTEN_START_CC:
            case PAYUEL_ROMA_LISTEN_STOP_CC:
            case PAYUEL_ROMA_SHOUT_START_CC:
            case PAYUEL_ROMA_SHOUT_START_RELATIVE_CC:
            case PAYUEL_ROMA_SHOUT_STOP_CC:
                return payload_len == 0;
            case PAYUEL_ROMA_GET_SINGLE_ENTRY_CC:
                return payload_len == 0 || payload_len == 2 ||
                       (payload_len >= 15 && payload_len <= 256);
            case PAYUEL_ROMA_GET_MULTIPLE_ENTRIES_CC:
                return payload_len <= 512;
            case PAYUEL_ROMA_ADD_ENTRY_CC:
            case PAYUEL_ROMA_REMOVE_ENTRY_CC:
            case PAYUEL_ROMA_GET_USED_SLOTS_CC:
                return payload_len == sizeof(uint8_t);
            default:
                return v.ret_type != 0 && (payload_len == 1 || payload_len == 8);
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_NOOP)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_Noop_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_noop, payload, sizeof(PAYUEL_LGPM_Noop_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RESETCOUNTERS)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_ResetCounters_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_resetcounters, payload, sizeof(PAYUEL_LGPM_ResetCounters_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_MCU_ALIVE)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Tlm_Payload))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_mcualivecheck, payload, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Tlm_Payload));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_3V3_PWR_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_3V3PwrOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_3v3pwron, payload, sizeof(PAYUEL_LGPM_3V3PwrOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_3V3_PWR_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_3V3PwrOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_3v3pwroff, payload, sizeof(PAYUEL_LGPM_3V3PwrOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_MAIN_BOOST_SW_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_MainBoostSwOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_mainboostswon, payload, sizeof(PAYUEL_LGPM_MainBoostSwOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_MAIN_BOOST_SW_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_MainBoostSwOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_mainboostswoff, payload, sizeof(PAYUEL_LGPM_MainBoostSwOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_SUB_BOOST_SW_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_SubBoostSwOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_subboostswon, payload, sizeof(PAYUEL_LGPM_SubBoostSwOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_SUB_BOOST_SW_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_SubBoostSwOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_subboostswoff, payload, sizeof(PAYUEL_LGPM_SubBoostSwOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V28_MAIN_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V28MainOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v28mainon, payload, sizeof(PAYUEL_LGPM_V28MainOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V28_MAIN_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V28MainOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v28mainoff, payload, sizeof(PAYUEL_LGPM_V28MainOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V28_SUB_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V28SubOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v28subon, payload, sizeof(PAYUEL_LGPM_V28SubOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V28_SUB_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V28SubOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v28suboff, payload, sizeof(PAYUEL_LGPM_V28SubOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V12_MAIN_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V12MainOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v12mainon, payload, sizeof(PAYUEL_LGPM_V12MainOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_V12_MAIN_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_V12MainOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_v12mainoff, payload, sizeof(PAYUEL_LGPM_V12MainOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_PWR_SENSE_INFO)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_PwrSenseInfo_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_pwrsenseinfo, payload, sizeof(PAYUEL_LGPM_PwrSenseInfo_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_PWR_SEQ_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_PwrSeqOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_pwrseqon, payload, sizeof(PAYUEL_LGPM_PwrSeqOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_PWR_SEQ_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_PwrSeqOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_pwrseqoff, payload, sizeof(PAYUEL_LGPM_PwrSeqOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX1)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaControlIdx1_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwacontrol_idx1, payload, sizeof(PAYUEL_LGPM_RwaControlIdx1_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX2)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaControlIdx2_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwacontrol_idx2, payload, sizeof(PAYUEL_LGPM_RwaControlIdx2_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX3)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaControlIdx3_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwacontrol_idx3, payload, sizeof(PAYUEL_LGPM_RwaControlIdx3_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_PWR_ON)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaPwrOn_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwapwron, payload, sizeof(PAYUEL_LGPM_RwaPwrOn_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_PWR_OFF)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaPwrOff_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwapwroff, payload, sizeof(PAYUEL_LGPM_RwaPwrOff_tlm_payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_PAYUEL_LGPM_RWA_SENSE_INFO)
    {
        if (payload_len != sizeof(PAYUEL_LGPM_RwaSenseInfo_tlm_payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else{
            memcpy(&v.u.lgpm_rwasenseinfo, payload, sizeof(PAYUEL_LGPM_RwaSenseInfo_tlm_payload_t));
            return true;
        }
    }
    /********************************************************************************************************************************* */

    v.kind = REPORT_KIND_SC_GENERIC;

    uint16_t n = payload_len;
    if (n > sizeof(v.u.generic.bytes)) n = sizeof(v.u.generic.bytes);
    memcpy(v.u.generic.bytes, payload, n);
    if (n < sizeof(v.u.generic.bytes)) memset(v.u.generic.bytes + n, 0, sizeof(v.u.generic.bytes) - n);

    return true;
}



const char *PaySltCommandName(uint8_t cc)
{
    switch (cc) {
        case BEE1012_PAY_SLT_NOOP_CC: return "NOOP";
        case BEE1012_PAY_SLT_RESET_COUNTERS_CC: return "RESET_COUNTERS";
        case BEE1012_PAY_SLT_REPORT_BCN_CC: return "REPORT_BCN";
        case BEE1012_PAY_SLT_OUTPUT_ENABLED_CC: return "OUTPUT_ENABLED";
        case BEE1012_PAY_SLT_RS422_PING_CC: return "RS422_PING";
        case BEE1012_PAY_SLT_PAR_GET_CC: return "PAR_GET";
        case BEE1012_PAY_SLT_PAR_SET_CC: return "PAR_SET";
        case BEE1012_PAY_SLT_SCAN_FILES_CC: return "SCAN_FILES";
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC: return "DOWNLOAD_FILE_I2C";
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC: return "DOWNLOAD_FILE_RS422";
        case BEE1012_PAY_SLT_GET_FULL_TABLE_CC: return "GET_FULL_TABLE";
        case BEE1012_PAY_SLT_PAR_SET_ARRAY_CC: return "PAR_SET_ARRAY";
        default: return "UNKNOWN";
    }
}

static void LogPaySltReport(const ReportView_t &v)
{
    const uint8_t *p = v.raw_bytes;
    const uint16_t n = v.raw_size;
    console.AddLog("[PAY_SLT] Received %s (CC=%u, ReturnType=%u, ReturnCode=%" PRId32
                   ", ReturnDataSize=%u)",
                   PaySltCommandName(v.reflected_cc), (unsigned)v.reflected_cc,
                   (unsigned)v.ret_type, v.ret_code, (unsigned)n);

    if (!v.payload_valid) {
        console.AddLog("[ERROR][PAY_SLT] Unexpected ReturnDataSize=%u for %s (CC=%u); payload parsing blocked.",
                       (unsigned)n, PaySltCommandName(v.reflected_cc), (unsigned)v.reflected_cc);
        return;
    }

    switch (v.reflected_cc) {
        case BEE1012_PAY_SLT_NOOP_CC:
            if (n == 32) console.AddLog("[PAY_SLT] Message: %.*s", 31, (const char *)p);
            break;
        case BEE1012_PAY_SLT_REPORT_BCN_CC:
            if (n == 1) console.AddLog("[PAY_SLT] BcnEnabled: %u", (unsigned)p[0]);
            break;
        case BEE1012_PAY_SLT_OUTPUT_ENABLED_CC:
            if (n == 4) console.AddLog("[PAY_SLT] HK=%u BCN=%u IFB=%u EXP=%u",
                                       (unsigned)p[0], (unsigned)p[1], (unsigned)p[2], (unsigned)p[3]);
            break;
        case BEE1012_PAY_SLT_RS422_PING_CC:
            if (n == 10) {
                console.AddLog("[PAY_SLT] RS422 RX: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                               p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
            }
            break;
        case BEE1012_PAY_SLT_PAR_GET_CC:
            if (n >= sizeof(PAY_SLT_ParamHeader_Report_t)) {
                PAY_SLT_ParamHeader_Report_t h{};
                memcpy(&h, p, sizeof(h));
                console.AddLog("[PAY_SLT] node=%u table=%u addr=0x%04X type=%u count=%u",
                               (unsigned)h.node, (unsigned)h.table, (unsigned)h.addr,
                               (unsigned)h.type, (unsigned)h.len);
                const uint8_t *values = p + sizeof(h);
                const size_t element_size = PaySltParamElementSize(h.type);
                const uint8_t value_count = element_size == 0 ? 0 :
                    (uint8_t)std::min<size_t>(h.len, (n - sizeof(h)) / element_size);
                for (uint8_t i = 0; i < value_count; ++i) {
                    if (h.type == PAY_SLT_PARAM_UINT8) {
                        console.AddLog("[PAY_SLT] Value[%u]=%u", (unsigned)i, (unsigned)values[i]);
                    } else if (h.type == PAY_SLT_PARAM_INT8) {
                        console.AddLog("[PAY_SLT] Value[%u]=%d", (unsigned)i, (int)(int8_t)values[i]);
                    } else if (h.type == PAY_SLT_PARAM_UINT16 || h.type == PAY_SLT_PARAM_INT16) {
                        uint16_t value = 0; memcpy(&value, values + 2U * i, sizeof(value));
                        if (h.type == PAY_SLT_PARAM_UINT16)
                            console.AddLog("[PAY_SLT] Value[%u]=%u", (unsigned)i, (unsigned)value);
                        else
                            console.AddLog("[PAY_SLT] Value[%u]=%d", (unsigned)i, (int)(int16_t)value);
                    } else if (h.type == PAY_SLT_PARAM_UINT32 || h.type == PAY_SLT_PARAM_FLOAT) {
                        uint32_t bits = 0; memcpy(&bits, values + 4U * i, sizeof(bits));
                        if (h.type == PAY_SLT_PARAM_UINT32) {
                            console.AddLog("[PAY_SLT] Value[%u]=%" PRIu32, (unsigned)i, bits);
                        } else {
                            float value = 0.0f; memcpy(&value, &bits, sizeof(value));
                            console.AddLog("[PAY_SLT] Value[%u]=%g", (unsigned)i, (double)value);
                        }
                    }
                }
                if (h.type == PAY_SLT_PARAM_STRING)
                    console.AddLog("[PAY_SLT] String=%.*s", (int)value_count, (const char *)values);
            }
            break;
        case BEE1012_PAY_SLT_SCAN_FILES_CC:
            if (n == sizeof(PAY_SLT_ScanFiles_Report_t)) {
                PAY_SLT_ScanFiles_Report_t x{}; memcpy(&x, p, sizeof(x));
                console.AddLog("[PAY_SLT] snap_id=%" PRIu32 " file_count=%" PRIu32, x.snap_id, x.file_count);
            } else if (n == 1) {
                console.AddLog("[PAY_SLT] Invalid node=%u", (unsigned)p[0]);
            }
            break;
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC:
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC:
            if (n == sizeof(PAY_SLT_DownloadRequest_Report_t)) {
                PAY_SLT_DownloadRequest_Report_t x{}; memcpy(&x, p, sizeof(x));
                console.AddLog("[PAY_SLT] file_index=%" PRIu32 " start_chunk=%" PRIu32,
                               x.file_index, x.start_chunk);
            } else if (n == sizeof(PAY_SLT_DownloadResult_Report_t)) {
                PAY_SLT_DownloadResult_Report_t x{}; memcpy(&x, p, sizeof(x));
                x.file_path[sizeof(x.file_path) - 1] = '\0';
                console.AddLog("[PAY_SLT] file_path=%s expected_file_size=%" PRIu32,
                               x.file_path, x.expected_file_size);
            }
            break;
        case BEE1012_PAY_SLT_GET_FULL_TABLE_CC:
            console.AddLog("[PAY_SLT] Full-table return bytes: %u", (unsigned)n);
            break;
        default:
            break;
    }
}

void UpdateN5ReportView(const Report &rpt, int report_dport) {
    ReportView_t tmp;
    memset(&tmp, 0, sizeof(tmp));

    tmp.valid = false;

    tmp.CCMessage_ID = rpt.CCSDS_MsgId;
    tmp.CCCount      = rpt.CCSDS_Seq;
    tmp.CCLength     = rpt.CCSDS_Len;
    memcpy(tmp.CCTime_code, rpt.CCSDS_TimeCode, 6);

    tmp.reflected_msg_id       = rpt.ReflectedMID;
    tmp.reflected_cc           = rpt.ReflectedCC;
    tmp.ret_type     = rpt.RetType;
    tmp.ret_code     = rpt.RetCode;

    tmp.ret_val_size = rpt.RetValSize;
    if (tmp.ret_val_size > 512) tmp.ret_val_size = 512;

    tmp.is_critical      = rpt.IsCritical != 0;
    tmp.crit_time_sec    = rpt.CritTimeSec;
    tmp.crit_time_subsec = rpt.CritTimeSubsec;

    /* PAY_SLT and UELYSYS ROMA both reflect command MID 0x1830.  Their report
     * sockets are mission-specific (BEE1012=15, UELYSYS=10), so retain the
     * receiving dport as the primary discriminator instead of guessing from
     * payload bytes or replacing the existing ROMA mapping. */
    tmp.kind = (report_dport == 15 && tmp.reflected_msg_id == BEE1012_PAY_SLT_CMD_ID)
                   ? REPORT_KIND_BEE1012_PAY_SLT
                   : DetermineReportKind(tmp.reflected_msg_id, tmp.reflected_cc);
    tmp.roma_parget_index = (tmp.kind == REPORT_KIND_PAYUEL_ROMA_PARGET)
                                ? g_roma_parget_requested_index : -1;

    tmp.payload_valid = DecodeN5PayloadToView(tmp, rpt.RetVal, tmp.ret_val_size);

    tmp.valid = true;

	    {
	        std::lock_guard<std::mutex> lk(g_report_view_mtx);
	        g_report_view = tmp;
	        UpdateEPSReportStateFromView(tmp);
	        if (tmp.reflected_msg_id == EPS_CMD_ID &&
	            tmp.reflected_cc == EPS_RPARAM_GET_FULL_TABLE_CC &&
	            !tmp.payload_valid) {
	            g_eps_rparam_reassembly.active = true;
	            g_eps_rparam_reassembly.valid = false;
	            g_eps_rparam_reassembly.complete = false;
	            g_eps_rparam_reassembly.error = true;
	        }
	    }

    if (tmp.kind == REPORT_KIND_BEE1012_PAY_SLT) LogPaySltReport(tmp);
}


void UpdateSD1SATReportView(const SD1SATReport &rpt) {
    ReportView_t tmp;
    memset(&tmp, 0, sizeof(tmp));

    tmp.valid = false;

    // tmp.CCMessage_ID = rpt.CCSDS_MsgId;
    // tmp.CCCount      = rpt.CCSDS_Seq;
    // tmp.CCLength     = rpt.CCSDS_Len;
    // memcpy(tmp.CCTime_code, rpt.CCSDS_TimeCode, 6);

    tmp.CCMessage_ID = 0;
    tmp.CCCount = 0;
    tmp.CCLength = 0;
    memset(tmp.CCTime_code, 0, sizeof(tmp.CCTime_code));

    /* SD1SAT has no separate critical wire shape, but CR's own retransmit flag
     * is analogous ground-facing metadata - surface it the same way. */
    tmp.is_critical      = rpt.IsRetransmit != 0;
    tmp.crit_time_sec    = rpt.CrSeq;
    tmp.crit_time_subsec = 0;

    tmp.reflected_msg_id = rpt.CmdMsgId;
    tmp.reflected_cc     = rpt.CmdCode;
    tmp.ret_type          = rpt.RetType;
    tmp.ret_code          = rpt.RetVal;

    tmp.ret_val_size = rpt.DataLength;
    //if (tmp.ret_val_size > 512) tmp.ret_val_size = 512;

    tmp.kind = DetermineSD1SATReportKind(tmp.reflected_msg_id, tmp.reflected_cc);
    tmp.payload_valid = DecodeN5PayloadToView(tmp, rpt.Data, tmp.ret_val_size);

    tmp.valid = true;

    {
        std::lock_guard<std::mutex> lk(g_report_view_mtx);
        g_report_view = tmp;
    }
}
