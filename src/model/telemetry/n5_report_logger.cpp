// Model - 누리호 5차 위성 리포트 저장 (BEE1012 포트 15, UELYSYS 포트 10)
//
// GS_nuri5 miman_coms.cpp 의 리포트 해석 덤프(DumpReportPayloadParsed_ByMidCc 와 EPS / UELYSYS / ADCS / PAY_SLT
// 덤프)와 위성별 저장 함수(BEE1012ReportSaver, UELYSYSReportSaver)를 옮긴 것.
// BEE-1000 리포트 저장(telemetry_logger.cpp 의 ReportSaver)은 바꾸지 않는다.
// GS_nuri5 와 다른 점: 파일 이름에 밀리초 + 같은 이름이면 번호 (여러 리포트가 한 패킷(0x0825)에 묶여
// 오면 UELYSYS 는 초 단위 이름이라 서로 덮어썼다), 폴더는 OpenTelemetryFile 이 만든다.
#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"

#include <algorithm>

#include "n5_report_decoder.h"
#include "n5_report_logger.h"
#include "telemetry_files.h"

extern Console console;
extern int ReportCounter;

static void DumpHex(FILE *fp, const uint8_t *p, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        fprintf(fp, "%02X ", p[i]);
        if ((i + 1) % 16 == 0) fprintf(fp, "\n");
    }
    if (n % 16 != 0) fprintf(fp, "\n");
}

static void DumpArr_i16(FILE *fp, const char *label, const int16_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIi16 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u16(FILE *fp, const char *label, const uint16_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu16 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u8(FILE *fp, const char *label, const uint8_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu8 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u32(FILE *fp, const char *label, const uint32_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu32 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpFixedName8(FILE *fp, const char *label, const char name[8])
{
    char buf[9] = {0};
    memcpy(buf, name, 8);
    fprintf(fp, "%s: %s\n", label, buf);
}

static void DumpIndexedBytes(FILE *fp, const char *name, const uint8_t *data, uint16_t size);

static void DumpEPSQueryMeta(FILE *fp, const EPS_Report_Metadata_t &meta)
{
    fprintf(fp, "source     : %" PRIu8 "\n", meta.source);
    fprintf(fp, "data_id    : %" PRIu8 "\n", meta.data_id);
    fprintf(fp, "arg0       : %" PRIu8 "\n", meta.arg0);
    fprintf(fp, "arg1       : %" PRIu8 "\n", meta.arg1);
    fprintf(fp, "sequence   : %" PRIu16 "\n", meta.sequence);
    fprintf(fp, "offset     : %" PRIu16 "\n", meta.offset);
    fprintf(fp, "total_size : %" PRIu16 "\n", meta.total_size);
    fprintf(fp, "chunk_size : %" PRIu16 "\n", meta.chunk_size);
}

static bool DumpEPSReportParsed(FILE *fp, const Report *rpt)
{
    EPS_Report_Metadata_t meta;
    if (!DecodeEPSQueryHeader(meta, rpt->RetVal, rpt->RetValSize, rpt->ReflectedCC, rpt->RetCode)) {
        fprintf(fp, "\n[EPS] Query payload header decode failed\n");
        return false;
    }

    const uint8_t *data = rpt->RetVal + EPS_QUERY_REPORT_HEADER_SIZE;

    fprintf(fp, "\n[EPS Query Report]\n");
    DumpEPSQueryMeta(fp, meta);

    if (rpt->ReflectedCC == EPS_P80_POWER_IF_GET_CC &&
        meta.data_id == EPS_QUERY_REPORT_P80_POWER_IF_STATUS &&
        meta.chunk_size == sizeof(EPS_PowerIfStatus_Report_t)) {
        EPS_PowerIfStatus_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        fprintf(fp, "\n[EPS Power IF Status]\n");
        fprintf(fp, "ch_idx     : %" PRIu8 "\n", pl.ch_idx);
        fprintf(fp, "mode       : %" PRIu8 "\n", pl.mode);
        fprintf(fp, "on_cnt     : %" PRIu16 "\n", pl.on_cnt);
        fprintf(fp, "off_cnt    : %" PRIu16 "\n", pl.off_cnt);
        fprintf(fp, "cur_lu_lim : %" PRIu16 "\n", pl.cur_lu_lim);
        fprintf(fp, "cur_lim    : %" PRIu16 "\n", pl.cur_lim);
        fprintf(fp, "voltage    : %" PRIu16 "\n", pl.voltage);
        fprintf(fp, "current    : %" PRIi16 "\n", pl.current);
        fprintf(fp, "latchup    : %" PRIu16 "\n", pl.latchup);
        DumpFixedName8(fp, "name       ", pl.name);
        return true;
    }

    if (rpt->ReflectedCC == EPS_P80_POWER_IF_LIST_CC &&
        meta.data_id == EPS_QUERY_REPORT_P80_POWER_IF_LIST &&
        meta.chunk_size == sizeof(EPS_PowerIfList_Report_t)) {
        EPS_PowerIfList_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        uint8_t count = (pl.count > 24) ? 24 : pl.count;
        fprintf(fp, "\n[EPS Power IF List]\n");
        fprintf(fp, "cmd    : %" PRIu8 "\n", pl.cmd);
        fprintf(fp, "status : %" PRIu8 "\n", pl.status);
        fprintf(fp, "count  : %" PRIu8 "\n", pl.count);
        for (uint8_t i = 0; i < count; i++) {
            char name[9] = {0};
            memcpy(name, pl.list[i].name, 8);
            fprintf(fp, "list[%u]: ch_idx=%" PRIu8 " mode=%" PRIu8 " name=%s\n",
                    (unsigned)i, pl.list[i].ch_idx, pl.list[i].mode, name);
        }
        return true;
    }

    if ((rpt->ReflectedCC == EPS_GET_HK_CC || rpt->ReflectedCC == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_PMU_HK &&
        meta.source == 1 &&
        meta.chunk_size == sizeof(EPS_P80_PMU_HK_Report_t)) {
        EPS_P80_PMU_HK_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        fprintf(fp, "\n[EPS P80 PMU HK]\n");
        fprintf(fp, "uptime       : %" PRIu32 "\n", pl.uptime);
        fprintf(fp, "bootcause    : %" PRIu32 "\n", pl.bootcause);
        fprintf(fp, "resetcause   : %" PRIu16 "\n", pl.resetcause);
        fprintf(fp, "bootcount    : %" PRIu16 "\n", pl.bootcount);
        fprintf(fp, "batt_v       : %" PRIu16 "\n", pl.batt_v);
        fprintf(fp, "batt_i       : %" PRIi16 "\n", pl.batt_i);
        fprintf(fp, "batt_mode    : %" PRIu8 "\n", pl.batt_mode);
        fprintf(fp, "vbat_v       : %" PRIu16 "\n", pl.vbat_v);
        fprintf(fp, "vcc_v        : %" PRIu16 "\n", pl.vcc_v);
        DumpArr_i16(fp, "temp[0..1]", pl.temp, 2);
        DumpArr_u8(fp, "out_en[0..5]", pl.out_en, 6);
        DumpArr_i16(fp, "out_i[0..5]", pl.out_i, 6);
        DumpArr_u8(fp, "sm_en[0..7]", pl.sm_en, 8);
        fprintf(fp, "gnd_wdt_cnt  : %" PRIu16 "\n", pl.gnd_wdt_cnt);
        fprintf(fp, "bus_wdt_cnt  : %" PRIu16 "\n", pl.bus_wdt_cnt);
        fprintf(fp, "gnd_wdt_left : %" PRIu32 "\n", pl.gnd_wdt_left);
        fprintf(fp, "bus_wdt_left : %" PRIu32 "\n", pl.bus_wdt_left);
        return true;
    }

    if ((rpt->ReflectedCC == EPS_GET_HK_CC || rpt->ReflectedCC == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_PDU_HK &&
        meta.source == 4 &&
        meta.chunk_size == sizeof(EPS_P80_PDU_HK_Report_t)) {
        EPS_P80_PDU_HK_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        fprintf(fp, "\n[EPS P80 PDU HK]\n");
        fprintf(fp, "uptime       : %" PRIu32 "\n", pl.uptime);
        fprintf(fp, "bootcause    : %" PRIu32 "\n", pl.bootcause);
        fprintf(fp, "bootcount    : %" PRIu32 "\n", pl.bootcount);
        fprintf(fp, "resetcause   : %" PRIu16 "\n", pl.resetcause);
        fprintf(fp, "vcc_v        : %" PRIu16 "\n", pl.vcc_v);
        fprintf(fp, "vcc_i        : %" PRIu16 "\n", pl.vcc_i);
        fprintf(fp, "vbat_v       : %" PRIu16 "\n", pl.vbat_v);
        fprintf(fp, "temp         : %" PRIi16 "\n", pl.temp);
        fprintf(fp, "batt_mode    : %" PRIu8 "\n", pl.batt_mode);
        DumpArr_u8(fp, "out_en[0..23]", pl.out_en, 24);
        DumpArr_i16(fp, "out_i[0..23]", pl.out_i, 24);
        fprintf(fp, "gnd_wdt_cnt  : %" PRIu32 "\n", pl.gnd_wdt_cnt);
        fprintf(fp, "bus_wdt_cnt  : %" PRIu32 "\n", pl.bus_wdt_cnt);
        fprintf(fp, "gnd_wdt_left : %" PRIu32 "\n", pl.gnd_wdt_left);
        fprintf(fp, "bus_wdt_left : %" PRIu32 "\n", pl.bus_wdt_left);
        return true;
    }

    if ((rpt->ReflectedCC == EPS_GET_HK_CC || rpt->ReflectedCC == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_P80_ACU_HK &&
        (meta.source == 2 || meta.source == 3) &&
        meta.chunk_size == sizeof(EPS_P80_ACU_HK_Report_t)) {
        EPS_P80_ACU_HK_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        fprintf(fp, "\n[EPS P80 ACU%u HK]\n", (meta.source == 2) ? 1U : 2U);
        fprintf(fp, "uptime       : %" PRIu32 "\n", pl.uptime);
        fprintf(fp, "bootcause    : %" PRIu32 "\n", pl.bootcause);
        fprintf(fp, "bootcount    : %" PRIu32 "\n", pl.bootcount);
        fprintf(fp, "resetcause   : %" PRIu16 "\n", pl.resetcause);
        DumpArr_i16(fp, "input_i[0..5]", pl.input_i, 6);
        DumpArr_u16(fp, "input_v[0..5]", pl.input_v, 6);
        fprintf(fp, "vcc_v        : %" PRIu16 "\n", pl.vcc_v);
        fprintf(fp, "vbat_v       : %" PRIu16 "\n", pl.vbat_v);
        DumpArr_i16(fp, "temp[0..2]", pl.temp, 3);
        fprintf(fp, "mppt_mode    : %" PRIu8 "\n", pl.mppt_mode);
        fprintf(fp, "gnd_wdt_cnt  : %" PRIu32 "\n", pl.gnd_wdt_cnt);
        fprintf(fp, "gnd_wdt_left : %" PRIu32 "\n", pl.gnd_wdt_left);
        return true;
    }

    if ((rpt->ReflectedCC == EPS_GET_HK_CC || rpt->ReflectedCC == EPS_GET_HK_ALL_CC) &&
        meta.data_id == EPS_QUERY_REPORT_BP8_HK &&
        meta.source == 7 &&
        meta.chunk_size == sizeof(EPS_BP8_HK_Report_t)) {
        EPS_BP8_HK_Report_t pl;
        memcpy(&pl, data, sizeof(pl));
        fprintf(fp, "\n[EPS BP8 HK]\n");
        fprintf(fp, "Uptime        : %" PRIu32 "\n", pl.Uptime);
        fprintf(fp, "BootCount     : %" PRIu16 "\n", pl.BootCount);
        fprintf(fp, "BootCause     : %" PRIu16 "\n", pl.BootCause);
        fprintf(fp, "ResetCause    : %" PRIu16 "\n", pl.ResetCause);
        fprintf(fp, "Vbat          : %" PRIu16 "\n", pl.Vbat);
        fprintf(fp, "Soc           : %f\n", pl.Soc);
        fprintf(fp, "Current       : %f\n", pl.Current);
        fprintf(fp, "InCurrent     : %" PRIu16 "\n", pl.InCurrent);
        fprintf(fp, "OutCurrent    : %" PRIu16 "\n", pl.OutCurrent);
        fprintf(fp, "HeaterCurrent : %" PRIu16 "\n", pl.HeaterCurrent);
        fprintf(fp, "IntTemp       : %" PRIi16 "\n", pl.IntTemp);
        fprintf(fp, "BatAvrTemp    : %f\n", pl.BatAvrTemp);
        DumpArr_i16(fp, "BatTemp[0..3]", pl.BatTemp, 4);
        fprintf(fp, "OVoltCount    : %" PRIu16 "\n", pl.OVoltCount);
        fprintf(fp, "BatFault      : %" PRIu8 "\n", pl.BatFault);
        return true;
    }

    fprintf(fp, "\n[EPS Query Data]\n");
    if (meta.data_id == EPS_QUERY_REPORT_CSP_PING_MS && meta.chunk_size >= sizeof(int32_t)) {
        int32_t elapsed_ms = 0;
        memcpy(&elapsed_ms, data, sizeof(elapsed_ms));
        fprintf(fp, "elapsed_ms : %" PRIi32 "\n", elapsed_ms);
    } else if ((meta.data_id == EPS_QUERY_REPORT_CSP_MEMFREE ||
                meta.data_id == EPS_QUERY_REPORT_CSP_BUF_FREE ||
                meta.data_id == EPS_QUERY_REPORT_CSP_UPTIME) &&
               meta.chunk_size == sizeof(uint32_t)) {
        uint32_t value = 0;
        memcpy(&value, data, sizeof(value));
        const char *name = (meta.data_id == EPS_QUERY_REPORT_CSP_MEMFREE) ? "memfree_bytes" :
                           (meta.data_id == EPS_QUERY_REPORT_CSP_BUF_FREE) ? "buffer_count" :
                                                                           "uptime_seconds";
        fprintf(fp, "%s : %" PRIu32 "\n", name, value);
    } else if (meta.data_id == EPS_QUERY_REPORT_CSP_PS_TEXT) {
        fprintf(fp, "process_text: %.*s\n", (int)meta.chunk_size, (const char *)data);
    }
    fprintf(fp, "query_data (%" PRIu16 " bytes):\n", meta.chunk_size);
    for (uint16_t i = 0; i < meta.chunk_size; ++i)
        fprintf(fp, "  [%03" PRIu16 "] = 0x%02X (%" PRIu8 ")\n", i, data[i], data[i]);
    if (rpt->ReflectedCC == EPS_RPARAM_GET_FULL_TABLE_CC) {
        EPS_RParam_Reassembly_State_t s{};
        {
            std::lock_guard<std::mutex> lk(g_report_view_mtx);
            s = g_eps_rparam_reassembly;
        }
        fprintf(fp, "\n[EPS CC42 Reassembly]\nactive=%u\nvalid=%u\ncomplete=%u\nerror=%u\n"
                    "duplicate_seen=%u\nmissing_sequence_seen=%u\nexpected_sequence=%" PRIu16
                    "\nlast_sequence=%" PRIu16 "\ntotal_size=%" PRIu16 "\nreceived_size=%" PRIu16
                    "\nlast_offset=%" PRIu16 "\nlast_chunk_size=%" PRIu16 "\n",
                s.active, s.valid, s.complete, s.error, s.duplicate_seen, s.missing_sequence_seen,
                s.expected_sequence, s.last_sequence, s.total_size, s.received_size,
                s.last_offset, s.last_chunk_size);
        if (s.complete) DumpIndexedBytes(fp, "reassembled_data", s.buffer, s.total_size);
    }
    return true;
}

typedef struct __attribute__((packed))
{
    uint8_t csp_node;
    int32_t status;
} EPS_Node_Status_Report_GS_t;

typedef struct
{
    uint32_t CmdCounter;
    uint32_t ErrCounter;
    uint32_t GetHkErrCounter;
    uint32_t GetBcnErrCounter;
} EPS_AppData_Counters_Report_GS_t;

static bool DumpEPSCommandStatus(FILE *fp, const Report *rpt)
{
    const uint16_t n = (rpt->RetValSize > sizeof(rpt->RetVal))
                           ? (uint16_t)sizeof(rpt->RetVal)
                           : rpt->RetValSize;
    const uint8_t *p = rpt->RetVal;

    fprintf(fp, "\n[EPS Command Status]\n");
    switch (rpt->ReflectedCC) {
        case EPS_NOOP_CC:
        case EPS_RESET_COUNTERS_CC: {
            if (n != sizeof(EPS_AppData_Counters_Report_GS_t)) return false;
            EPS_AppData_Counters_Report_GS_t v{};
            memcpy(&v, p, sizeof(v));
            fprintf(fp, "CmdCounter=%" PRIu32 "\nErrCounter=%" PRIu32
                        "\nGetHkErrCounter=%" PRIu32 "\nGetBcnErrCounter=%" PRIu32 "\n",
                    v.CmdCounter, v.ErrCounter, v.GetHkErrCounter, v.GetBcnErrCounter);
            return true;
        }
        case EPS_P80_POWER_IF_GET_CC: {
            if (n != sizeof(EPS_P80_Power_If_Get_Cmd_Payload_t)) return false;
            EPS_P80_Power_If_Get_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\nname=%.*s\n", v.csp_node, (int)sizeof(v.name), v.name);
            return true;
        }
        case EPS_P80_POWER_IF_SET_CC: {
            if (n != sizeof(EPS_P80_Power_If_Set_Cmd_Payload_t)) return false;
            EPS_P80_Power_If_Set_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\nmode=%u\non_cnt=%u\noff_cnt=%u\nname=%.*s\n",
                    v.csp_node, v.mode, v.on_cnt, v.off_cnt, (int)sizeof(v.name), v.name);
            return true;
        }
        case EPS_P80_POWER_IF_LIST_CC:
        case EPS_GET_HK_CC:
        case EPS_P80_GND_WDT_CLEAR_CC:
        case EPS_RPARAM_SAVE_ALL_CC:
        case EPS_CSP_REBOOT_CC:
        case EPS_CSP_PS_CC:
        case EPS_CSP_MEMFREE_CC:
        case EPS_CSP_BUF_FREE_CC:
        case EPS_CSP_UPTIME_CC:
            if (n != 1) return false;
            fprintf(fp, "csp_node=%u\n", p[0]);
            return true;
        case EPS_GET_HK_ALL_CC:
        case EPS_P80_GND_WDT_CLEAR_ALL_CC: {
            if (n != sizeof(EPS_Node_Status_Report_GS_t)) return false;
            EPS_Node_Status_Report_GS_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\nstatus=%" PRId32 "\n", v.csp_node, v.status);
            return true;
        }
        case EPS_RPARAM_GET_CC: {
            if (n != sizeof(EPS_RParam_Get_Cmd_Payload_t)) return false;
            EPS_RParam_Get_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\ntable_id=%u\naddr=%u\ntype=%u\nsize=%u\n",
                    v.csp_node, v.table_id, v.addr, v.type, v.size);
            return true;
        }
        case EPS_RPARAM_SET_CC: {
            if (n != sizeof(EPS_RParam_Set_Cmd_Payload_t)) return false;
            EPS_RParam_Set_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\ntable_id=%u\naddr=%u\ntype=%u\nsize=%u\n",
                    v.csp_node, v.table_id, v.addr, v.type, v.size);
            DumpIndexedBytes(fp, "data", v.data,
                             (v.size > sizeof(v.data)) ? (uint16_t)sizeof(v.data) : v.size);
            return true;
        }
        case EPS_RPARAM_GET_FULL_TABLE_CC:
        case EPS_RPARAM_TABLE_SAVE_CC:
        case EPS_RPARAM_TABLE_LOAD_CC:
            if (n != 2) return false;
            fprintf(fp, "csp_node=%u\ntable_id=%u\n", p[0], p[1]);
            return true;
        case EPS_RPARAM_SAVE_TO_STORE_CC:
        case EPS_RPARAM_LOAD_FROM_STORE_CC: {
            if (n != sizeof(EPS_RParam_Store_Cmd_Payload_t)) return false;
            EPS_RParam_Store_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\ntable_id=%u\nstore=%.*s\nslot=%.*s\n",
                    v.csp_node, v.table_id, (int)sizeof(v.store), v.store,
                    (int)sizeof(v.slot), v.slot);
            return true;
        }
        case EPS_CSP_PING_CC: {
            if (n != sizeof(EPS_CSP_Ping_Cmd_Payload_t)) return false;
            EPS_CSP_Ping_Cmd_Payload_t v{}; memcpy(&v, p, sizeof(v));
            fprintf(fp, "csp_node=%u\nsize=%u\nopts=0x%02X\n", v.csp_node, v.size, v.opts);
            return true;
        }
        default:
            return false;
    }
}
template <typename T>
static bool CopyReportPayload(const Report *rpt, T &out)
{
    if (rpt->RetValSize < sizeof(T)) return false;
    memcpy(&out, rpt->RetVal, sizeof(T));
    return true;
}
template <typename T>
static bool CopyExactReportPayload(const Report *rpt, T &out)
{
    if (rpt->RetValSize != sizeof(T)) return false;
    memcpy(&out, rpt->RetVal, sizeof(T));
    return true;
}

static void DumpIndexedBytes(FILE *fp, const char *name, const uint8_t *data, uint16_t size)
{
    fprintf(fp, "%s (%" PRIu16 " bytes):\n", name, size);
    for (uint16_t i = 0; i < size; ++i)
        fprintf(fp, "  [%03" PRIu16 "] = 0x%02X (%" PRIu8 ")\n", i, data[i], data[i]);
}

static uint16_t ReadU16BE(const uint8_t b[2])
{
    return (uint16_t)(((uint16_t)b[0] << 8) | b[1]);
}

static uint32_t ReadU32BE(const uint8_t b[4])
{
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | b[3];
}

static void DumpLgbatBlock(FILE *fp, const UELYSYS_LGBAT_Block_Report_t &b)
{
    fprintf(fp, "ID=0x%02X\n", b.raw[0]);
    switch (b.raw[0]) {
        case 0x01: fprintf(fp, "Pack_Voltage=%" PRIu16 "\nPack_Current=%" PRId16 "\nAverage_Time_To_Empty=%" PRIu16 "\nAverage_Time_To_Full=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data01.Pack_Voltage, b.Data01.Pack_Current, b.Data01.Average_Time_To_Empty, b.Data01.Average_Time_To_Full, b.Data01.CheckSum); break;
        case 0x02: fprintf(fp, "Power_Supply_Status=%" PRIu8 "\nSOH=%" PRIu8 "\nSOC=%" PRIu16 "\nRC_Remaining_Capacity=%" PRIu16 "\nAE_Available_Energy=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data02.Power_Supply_Status, b.Data02.SOH, b.Data02.SOC, b.Data02.RC_Remaining_Capacity, b.Data02.AE_Available_Energy, b.Data02.CheckSum); break;
        case 0x03: fprintf(fp, "System_Max_Voltage=%" PRIu16 "\nPower_Supply_Health=%" PRIu8 "\nFETTestRequiredVoltage=%" PRIu16 "\n", b.Data03.System_Max_Voltage, b.Data03.Power_Supply_Health, b.Data03.FETTestRequiredVoltage); DumpArr_u8(fp, "Battery_Information_Reserved", b.Data03.Battery_Information_Reserved, 3); fprintf(fp, "CheckSum=0x%02X\n", b.Data03.CheckSum); break;
        case 0x04: fprintf(fp, "Percentage=%" PRIu16 "\nDesign_Capacity=%" PRIu16 "\nCapacity=%" PRIu16 "\nCharge=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data04.Percentage, b.Data04.Design_Capacity, b.Data04.Capacity, b.Data04.Charge, b.Data04.CheckSum); break;
        case 0x05: fprintf(fp, "Cell_Voltage_Max=%" PRIu16 "\nCell_Voltage_Min=%" PRIu16 "\nCell_Temperature_Max=%" PRId16 "\nCell_Temperature_Min=%" PRId16 "\nCheckSum=0x%02X\n", b.Data05.Cell_Voltage_Max, b.Data05.Cell_Voltage_Min, b.Data05.Cell_Temperature_Max, b.Data05.Cell_Temperature_Min, b.Data05.CheckSum); break;
        case 0x06: fprintf(fp, "Cell_Voltage_01=%" PRIu16 "\nCell_Voltage_02=%" PRIu16 "\nReserved_01=%" PRIu16 "\nReserved_02=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data06.Cell_Voltage_01, b.Data06.Cell_Voltage_02, b.Data06.Reserved_01, b.Data06.Reserved_02, b.Data06.CheckSum); break;
        case 0x07: fprintf(fp, "Cell_Temperature_02=%" PRId16 "\nCell_Temperature_01=%" PRId16 "\nBalancing_R_Temperature=%" PRId16 "\nPreCharge_R_Temperature=%" PRId16 "\nCheckSum=0x%02X\n", b.Data07.Cell_Temperature_02, b.Data07.Cell_Temperature_01, b.Data07.Balancing_R_Temperature, b.Data07.PreCharge_R_Temperature, b.Data07.CheckSum); break;
        case 0x08: fprintf(fp, "FET_Down_Temperature=%" PRId16 "\nFET_Up_Temperature=%" PRId16 "\nCtrlCBStatus=%" PRIu16 "\nTemperature_002_Reserved=%" PRIu8 "\nSoftVersion=%" PRIu8 "\nCheckSum=0x%02X\n", b.Data08.FET_Down_Temperature, b.Data08.FET_Up_Temperature, b.Data08.CtrlCBStatus, b.Data08.Temperature_002_Reserved, b.Data08.SoftVersion, b.Data08.CheckSum); break;
        case 0x09: fprintf(fp, "BMS_Wakeup=%" PRIu8 "\nWakeupHoldStatus=%" PRIu8 "\nVoltCurrDiag=%" PRIu8 "\nTempFailLevel=%" PRIu8 "\nFETStatus=%" PRIu8 "\n", b.Data09.BMS_Wakeup, b.Data09.WakeupHoldStatus, b.Data09.VoltCurrDiag, b.Data09.TempFailLevel, b.Data09.FETStatus); DumpArr_u8(fp, "Reserved", b.Data09.Reserved, 3); fprintf(fp, "CheckSum=0x%02X\n", b.Data09.CheckSum); break;
        case 0x0A: fprintf(fp, "FailStatus2=0x%02X\nFailStatus3=0x%02X\n", b.Data0A.FailStatus2, b.Data0A.FailStatus3); DumpArr_u8(fp, "Reserved", b.Data0A.Reserved, 6); fprintf(fp, "CheckSum=0x%02X\n", b.Data0A.CheckSum); break;
        case 0x0B: fprintf(fp, "BOOST_Out_Voltage=%" PRIu16 "\nMCU_B_Plus_Volt=%" PRIu16 "\nMCU_P_Plus_Volt=%" PRIu16 "\nMCU_BMIC_REG_Out_Volt=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data0B.BOOST_Out_Voltage, b.Data0B.MCU_B_Plus_Volt, b.Data0B.MCU_P_Plus_Volt, b.Data0B.MCU_BMIC_REG_Out_Volt, b.Data0B.CheckSum); break;
        case 0x0C: fprintf(fp, "BMS_Current=%" PRId16 "\nPCB_Temperature=%" PRId16 "\nBOOST_Temperature=%" PRId16 "\nMCU_SBC_AMUX_Voltage=%" PRIu16 "\nCheckSum=0x%02X\n", b.Data0C.BMS_Current, b.Data0C.PCB_Temperature, b.Data0C.BOOST_Temperature, b.Data0C.MCU_SBC_AMUX_Voltage, b.Data0C.CheckSum); break;
    }
}

static void DumpUelysysReportParsed(FILE *fp, const Report *rpt)
{
    const uint16_t n = (rpt->RetValSize > sizeof(rpt->RetVal))
                           ? (uint16_t)sizeof(rpt->RetVal)
                           : rpt->RetValSize;
    const uint8_t *p = rpt->RetVal;
    ReportView_t decoded{};
    decoded.reflected_msg_id = rpt->ReflectedMID;
    decoded.reflected_cc = rpt->ReflectedCC;
    decoded.ret_type = rpt->RetType;
    decoded.ret_code = rpt->RetCode;
    decoded.kind = DetermineReportKind(rpt->ReflectedMID, rpt->ReflectedCC);
    const bool payload_valid = DecodeN5PayloadToView(decoded, p, n);
    fprintf(fp, "payload_valid=%u\n", payload_valid ? 1U : 0U);

    if (rpt->ReflectedMID == UELYSYS_TTC_CMD_ID || rpt->ReflectedMID == 0x8018)
    {
        fprintf(fp, "\n[UELYSYS TTC]\n");
        switch (rpt->ReflectedCC)
        {
            case UELYSYS_TTC_NOOP_CC:
            case UELYSYS_TTC_RESET_COUNTERS_CC:
            case UELYSYS_TTC_GET_TIMELINE_HK_CC:
            case UELYSYS_TTC_RESET_TIMELINE_HK_CC:
            case UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC:
            case UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC: {
                UELYSYS_TTC_Counters_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "CommandCounter=%" PRIu16 "\nErrorCounter=%" PRIu16 "\n",
                            x.counters[0], x.counters[1]);
                break;
            }
            case UELYSYS_TTC_START_BEACON_BURST_CC: {
                UELYSYS_TTC_BeaconBurst_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "GroupId=%" PRIu16 "\nFirstEntryId=%" PRIu16
                                "\nScheduledEntries=%" PRIu16 "\nFailCount=%" PRIu16 "\n",
                            x.group_id, x.first_entry_id, x.scheduled_entries, x.fail_count);
                break;
            }
            case UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC:
            case UELYSYS_TTC_DELETE_ALL_ENTRIES_CC:
            case UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC: {
                uint16_t x = 0;
                if (n == sizeof(x)) memcpy(&x, p, sizeof(x));
                fprintf(fp, "Count=%" PRIu16 "\n", x);
                break;
            }
            case UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC: {
                UELYSYS_TTC_NextEntry_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "EntryId=%" PRIu16 "\nGroupId=%" PRIu16 "\nHasEntry=%" PRIu8 "\n",
                            x.entry_id, x.group_id, x.has_entry);
                break;
            }
            case UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC: {
                UELYSYS_TTC_NextExecution_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "NextExecutionTime=%" PRIu32 "\nHasEntry=%" PRIu8 "\n",
                            x.next_execution_time, x.has_entry);
                break;
            }
            case UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC: {
                UELYSYS_TTC_InsertAbs_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "ExecutionTimeAbsolute=%" PRIu32 "\nStalenessThreshold=%" PRIu16
                                "\nEntryId=%" PRIu16 "\nGroupId=%" PRIu16
                                "\nExecutionType=%" PRIu8 "\nCommandSize=%" PRIu16 "\n",
                            x.execution_time_absolute, x.staleness_threshold, x.entry_id,
                            x.group_id, x.execution_type, x.command_size);
                break;
            }
            case UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC: {
                UELYSYS_TTC_InsertRel_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "ExecutionTimeRelative=%" PRIu16 "\nStalenessThreshold=%" PRIu16
                                "\nEntryId=%" PRIu16 "\nGroupId=%" PRIu16
                                "\nExecutionType=%" PRIu8 "\nCommandSize=%" PRIu16 "\n",
                            x.execution_time_relative, x.staleness_threshold, x.entry_id,
                            x.group_id, x.execution_type, x.command_size);
                break;
            }
            case UELYSYS_TTC_DELETE_CMD_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_INIT_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC: {
                UELYSYS_TTC_EntryGroup_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "EntryId=%" PRIu16 "\nGroupId=%" PRIu16 "\n", x.entry_id, x.group_id);
                break;
            }
            case UELYSYS_TTC_DELETE_CMD_GROUP_CC: {
                UELYSYS_TTC_DeleteGroup_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "GroupId=%" PRIu16 "\nDeleteCount=%" PRIu16 "\n", x.group_id, x.delete_count);
                break;
            }
            case UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC: {
                UELYSYS_TTC_ExecuteEntry_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "GroupId=%" PRIu16 "\nEntryId=%" PRIu16 "\nPersistent=%" PRIu8 "\n",
                            x.group_id, x.entry_id, x.persistent);
                break;
            }
            case UELYSYS_TTC_EXECUTE_CMD_GROUP_CC: {
                UELYSYS_TTC_ExecuteGroup_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "GroupId=%" PRIu16 "\nPersistent=%" PRIu8 "\n", x.group_id, x.persistent);
                break;
            }
            case UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC: {
                UELYSYS_TTC_PlumbWrite_Report_t x{};
                if (CopyExactReportPayload(rpt, x)) {
                    fprintf(fp, "EntryId=%" PRIu16 "\nGroupId=%" PRIu16 "\nOffset=%" PRIu16
                                "\nChunkSize=%" PRIu16 "\n",
                            x.entry_id, x.group_id, x.offset, x.chunk_size);
                    DumpIndexedBytes(fp, "Data", x.data,
                                     (x.chunk_size > sizeof(x.data)) ? sizeof(x.data) : x.chunk_size);
                }
                break;
            }
            case UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC: {
                UELYSYS_TTC_PlumbFinalize_Report_t x{};
                if (CopyExactReportPayload(rpt, x))
                    fprintf(fp, "EntryId=%" PRIu16 "\nGroupId=%" PRIu16 "\nTimeTag=%" PRIu32
                                "\nStalenessThreshold=%" PRIu16 "\nCommandSize=%" PRIu16
                                "\nTimeTagType=%" PRIu8 "\nExecutionType=%" PRIu8 "\n",
                            x.entry_id, x.group_id, x.time_tag, x.staleness_threshold,
                            x.command_size, x.time_tag_type, x.execution_type);
                break;
            }
            default:
                fprintf(fp, "No typed return payload for CC=%" PRIu8 "\n", rpt->ReflectedCC);
                break;
        }
        return;
    }

    if (rpt->ReflectedMID == BEE1012_MEOW_CMD_ID || rpt->ReflectedMID == 0x8218 ||
        rpt->ReflectedMID == UELYSYS_MEOW_CMD_ID || rpt->ReflectedMID == 0x8618)
    {
        const bool bee1012_meow = (rpt->ReflectedMID == BEE1012_MEOW_CMD_ID || rpt->ReflectedMID == 0x8218);
        fprintf(fp, "\n[%s MEOW]\n", bee1012_meow ? "BEE1012" : "UELYSYS");
        if (!bee1012_meow) {
            const bool success = (rpt->RetType == 0);
            switch (rpt->ReflectedCC) {
                case UELYSYS_MEOW_NOOP_CC:
                case UELYSYS_MEOW_RESET_COUNTERS_CC:
                    if (n == 2)
                        fprintf(fp, "cmd_counter=%" PRIu8 "\nerr_counter=%" PRIu8 "\n", p[0], p[1]);
                    else
                        DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_SHELL_EXEC_SYNC_CC:
                case UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC:
                    if (n == sizeof(UELYSYS_MEOW_ShellResultErrno_Report_t)) {
                        UELYSYS_MEOW_ShellResultErrno_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "exit_code=%" PRId32 "\npid=%" PRId32 "\nos_errno=%" PRId32 "\n",
                                x.exit_code, x.pid, x.os_errno);
                    } else if (n == sizeof(UELYSYS_MEOW_ShellResult_Report_t)) {
                        UELYSYS_MEOW_ShellResult_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "exit_code=%" PRId32 "\npid=%" PRId32 "\n", x.exit_code, x.pid);
                    } else {
                        DumpIndexedBytes(fp, "return_data", p, n);
                    }
                    break;
                case UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC:
                case UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC:
                    if (n == sizeof(UELYSYS_MEOW_ValueErrno_Report_t)) {
                        UELYSYS_MEOW_ValueErrno_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "pid=%" PRId32 "\nos_errno=%" PRId32 "\n", x.value, x.os_errno);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "%s=%" PRId32 "\n", success ? "pid" : "error_detail", x);
                    } else {
                        DumpIndexedBytes(fp, "return_data", p, n);
                    }
                    break;
                case UELYSYS_MEOW_SHELL_POLL_CC:
                    if (n == sizeof(UELYSYS_MEOW_ShellPoll_Report_t)) {
                        UELYSYS_MEOW_ShellPoll_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "pid=%" PRId32 "\nexit_code=%" PRId32 "\n", x.pid, x.exit_code);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else {
                        DumpIndexedBytes(fp, "return_data", p, n);
                    }
                    break;
                case UELYSYS_MEOW_FILE_READ_CC:
                case UELYSYS_MEOW_FILE_TAIL_CC:
                    if (success)
                        DumpIndexedBytes(fp, "file_data", p, n);
                    else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else
                        DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_FILE_STAT_CC:
                    if (n == sizeof(UELYSYS_MEOW_FileStat_Report_t)) {
                        UELYSYS_MEOW_FileStat_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "size=%" PRIu64 "\nmtime=%" PRId64 "\nctime=%" PRId64
                                    "\nmode=0x%08" PRIX32 "\nis_dir=%" PRId32 "\npreview_len=%" PRIu64 "\n",
                                x.size, x.mtime, x.ctime, x.mode, x.is_dir, x.preview_len);
                        DumpIndexedBytes(fp, "preview", x.preview,
                                         (x.preview_len > sizeof(x.preview)) ? sizeof(x.preview) : (uint16_t)x.preview_len);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_DISK_STAT_CC:
                    if (n == sizeof(UELYSYS_MEOW_DiskStat_Report_t)) {
                        UELYSYS_MEOW_DiskStat_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "total_bytes=%" PRIu64 "\navail_bytes=%" PRIu64 "\n", x.total_bytes, x.avail_bytes);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_SYS_INFO_CC:
                    if (n == sizeof(UELYSYS_MEOW_SysInfo_Report_t)) {
                        UELYSYS_MEOW_SysInfo_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "uptime_sec=%" PRIu32 "\nload_1min=%" PRIu32 "\nload_5min=%" PRIu32
                                    "\nload_15min=%" PRIu32 "\ntotal_ram=%" PRIu64 "\nfree_ram=%" PRIu64
                                    "\nnum_procs=%" PRIu16 "\n",
                                x.uptime, x.load1, x.load5, x.load15, x.total_ram, x.free_ram, x.num_procs);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_SYS_TIME_GET_CC:
                    if (n == sizeof(UELYSYS_MEOW_SysTime_Report_t)) {
                        UELYSYS_MEOW_SysTime_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "sec=%" PRId64 "\nnsec=%" PRIu32 "\n", x.sec, x.nsec);
                    } else if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "os_errno=%" PRId32 "\n", x);
                    } else DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_CSP_IFSTATS_CC:
                    if (n == sizeof(UELYSYS_MEOW_CspIfStats_Report_t)) {
                        UELYSYS_MEOW_CspIfStats_Report_t x{}; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "tx=%" PRIu32 "\nrx=%" PRIu32 "\ntx_error=%" PRIu32
                                    "\nrx_error=%" PRIu32 "\ndrop=%" PRIu32 "\n",
                                x.tx, x.rx, x.tx_error, x.rx_error, x.drop);
                    } else if (n != 0) DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                case UELYSYS_MEOW_FILE_CHECKSUM_CC:
                    if (n == sizeof(uint32_t)) {
                        uint32_t x = 0; memcpy(&x, p, sizeof(x));
                        if (success) fprintf(fp, "crc32=0x%08" PRIX32 "\n", x);
                        else fprintf(fp, "os_errno=%" PRId32 "\n", (int32_t)x);
                    } else DumpIndexedBytes(fp, "return_data", p, n);
                    break;
                default:
                    if (n == sizeof(int32_t)) {
                        int32_t x = 0; memcpy(&x, p, sizeof(x));
                        fprintf(fp, "error_detail=%" PRId32 "\n", x);
                    } else if (n != 0) {
                        DumpIndexedBytes(fp, "return_data", p, n);
                    } else {
                        fprintf(fp, "ack_payload=empty\n");
                    }
                    break;
            }
            return;
        }
        if (rpt->ReflectedCC == UELYSYS_MEOW_FILE_STAT_CC) {
            UELYSYS_MEOW_FileStat_Report_t x{};
            if (CopyReportPayload(rpt, x)) {
                fprintf(fp, "size=%" PRIu64 "\nmtime=%" PRId64 "\nctime=%" PRId64
                            "\nmode=0x%08" PRIX32 "\nis_dir=%" PRId32 "\npreview_len=%" PRIu64 "\n",
                        x.size, x.mtime, x.ctime, x.mode, x.is_dir, x.preview_len);
                DumpIndexedBytes(fp, "preview", x.preview,
                                 (x.preview_len > sizeof(x.preview)) ? sizeof(x.preview) : (uint16_t)x.preview_len);
            }
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_DISK_STAT_CC) {
            UELYSYS_MEOW_DiskStat_Report_t x{};
            if (CopyReportPayload(rpt, x))
                fprintf(fp, "total_bytes=%" PRIu64 "\navail_bytes=%" PRIu64 "\n", x.total_bytes, x.avail_bytes);
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_SYS_INFO_CC) {
            UELYSYS_MEOW_SysInfo_Report_t x{};
            if (CopyReportPayload(rpt, x))
                fprintf(fp, "uptime=%" PRIu32 "\nload1=%" PRIu32 "\nload5=%" PRIu32
                            "\nload15=%" PRIu32 "\ntotal_ram=%" PRIu64 "\nfree_ram=%" PRIu64
                            "\nnum_procs=%" PRIu16 "\n",
                        x.uptime, x.load1, x.load5, x.load15, x.total_ram, x.free_ram, x.num_procs);
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_SYS_TIME_GET_CC) {
            UELYSYS_MEOW_SysTime_Report_t x{};
            if (CopyReportPayload(rpt, x))
                fprintf(fp, "sec=%" PRId64 "\nnsec=%" PRIu32 "\n", x.sec, x.nsec);
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_CSP_IFSTATS_CC) {
            UELYSYS_MEOW_CspIfStats_Report_t x{};
            if (CopyReportPayload(rpt, x))
                fprintf(fp, "rx=%" PRIu32 "\nrx_error=%" PRIu32 "\ntx=%" PRIu32
                            "\ntx_error=%" PRIu32 "\ndrop=%" PRIu32 "\n",
                        x.rx, x.rx_error, x.tx, x.tx_error, x.drop);
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_FILE_CHECKSUM_CC && n >= 4) {
            uint32_t crc = 0; memcpy(&crc, p, sizeof(crc));
            fprintf(fp, "crc32=0x%08" PRIX32 "\n", crc);
        } else if (rpt->ReflectedCC == UELYSYS_MEOW_FILE_READ_CC ||
                   rpt->ReflectedCC == UELYSYS_MEOW_FILE_TAIL_CC) {
            DumpIndexedBytes(fp, "file_data", p, n);
        } else if (n == sizeof(UELYSYS_MEOW_ShellResult_Report_t)) {
            UELYSYS_MEOW_ShellResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            fprintf(fp, "value0=%" PRId32 "\nvalue1=%" PRId32 "\n", x.exit_code, x.pid);
        } else if (n == sizeof(int32_t)) {
            int32_t value = 0; memcpy(&value, p, sizeof(value));
            fprintf(fp, "value=%" PRId32 "\n", value);
        } else {
            DumpIndexedBytes(fp, "return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_GPIO_CMD_ID || rpt->ReflectedMID == 0x8A18)
    {
        fprintf(fp, "\n[UELYSYS GPIO]\n");
        switch (rpt->ReflectedCC) {
            case UELYSYS_GPIO_NOOP_CC:
            case UELYSYS_GPIO_RESET_COUNTERS_CC:
                if (n == 2)
                    fprintf(fp, "cmd_counter=%" PRIu8 "\nerr_counter=%" PRIu8 "\n", p[0], p[1]);
                else
                    DumpIndexedBytes(fp, "return_data", p, n);
                break;
            case UELYSYS_GPIO_PROCESS_CC:
                if (n == 0) fprintf(fp, "ack_payload=empty\n");
                else DumpIndexedBytes(fp, "return_data", p, n);
                break;
            case UELYSYS_GPIO_DIGPIOLAY_PARAM_CC:
                if (n == sizeof(UELYSYS_GPIO_Digpiolay_Report_t)) {
                    UELYSYS_GPIO_Digpiolay_Report_t x{}; memcpy(&x, p, sizeof(x));
                    fprintf(fp, "ValU32=%" PRIu32 "\nValI16=%" PRId16 "\nValStr=%.*s\n",
                            x.val_u32, x.val_i16, (int)sizeof(x.val_str), x.val_str);
                } else DumpIndexedBytes(fp, "return_data", p, n);
                break;
            default:
                if (n == 1) fprintf(fp, "pin_state=%" PRIu8 "\n", p[0]);
                else if (n == 0) fprintf(fp, "hardware_error_payload=empty\n");
                else DumpIndexedBytes(fp, "return_data", p, n);
                break;
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_STX_CMD_ID || rpt->ReflectedMID == 0x5418)
    {
        // fprintf(fp, "\n[UELYSYS STX]\n");
        // if ((rpt->ReflectedCC == UELYSYS_STX_NOOP_CC ||
        //      rpt->ReflectedCC == UELYSYS_STX_RESET_COUNTERS_CC) && n == 4) {
        //     uint16_t counters[2]{}; memcpy(counters, p, sizeof(counters));
        //     fprintf(fp, "cmd_counter=%" PRIu16 "\nerr_counter=%" PRIu16 "\n", counters[0], counters[1]);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_INITIALIZE_CC &&
        //            n == sizeof(UELYSYS_STX_Initialize_Report_t)) {
        //     UELYSYS_STX_Initialize_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "module_1_result=%" PRId32 "\nmodule_2_result=%" PRId32 "\n", x.result[0], x.result[1]);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_SET_MODULE_ID_CC && n == sizeof(uint16_t)) {
        //     uint16_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "module_id=%" PRIu16 "\n", x);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_FILEHANDLE_CC && n == sizeof(uint32_t)) {
        //     uint32_t x = 0; memcpy(&x, p, sizeof(x)); fprintf(fp, "file_handle=%" PRIu32 "\n", x);
        // } else if ((rpt->ReflectedCC == UELYSYS_STX_GET_SYMBOL_RATE_CC ||
        //             rpt->ReflectedCC == UELYSYS_STX_GET_TX_POWER_CC ||
        //             rpt->ReflectedCC == UELYSYS_STX_GET_MODCOD_CC ||
        //             rpt->ReflectedCC == UELYSYS_STX_GET_ROLL_OFF_CC ||
        //             rpt->ReflectedCC == UELYSYS_STX_GET_PILOT_SIGNAL_CC ||
        //             rpt->ReflectedCC == UELYSYS_STX_GET_FEC_FRAME_SIZE_CC) &&
        //            n == sizeof(UELYSYS_STX_GetU8_Report_t)) {
        //     UELYSYS_STX_GetU8_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nvalue=%" PRIu8 "\n", x.ret, x.value);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_CENTER_FREQ_CC &&
        //            n == sizeof(UELYSYS_STX_GetFloat_Report_t)) {
        //     UELYSYS_STX_GetFloat_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\ncenter_frequency=%f\n", x.ret, x.value);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_PRETX_DELAY_CC &&
        //            n == sizeof(UELYSYS_STX_GetU16_Report_t)) {
        //     UELYSYS_STX_GetU16_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\npretransmission_delay=%" PRIu16 "\n", x.ret, x.value);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_ALL_PRAMETERS_CC &&
        //            n == sizeof(UELYSYS_STX_AllParameters_Report_t)) {
        //     UELYSYS_STX_AllParameters_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\n", x.ret);
        //         fprintf(fp, "symbol_rate=%" PRIu8 "\ntransmit_power=%" PRIu8 "\nmodcod=%" PRIu8
        //                     "\nroll_off=%" PRIu8 "\npilot_signal=%" PRIu8 "\nfec_frame_size=%" PRIu8
        //                     "\npretransmission_delay=%" PRIu16 "\ncenter_frequency=%f\n",
        //                 x.symbol_rate, x.transmit_power, x.modcod, x.rolloff, x.pilot_signal,
        //                 x.fec_frame_size, x.pretx_delay, x.center_frequency);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_REPORT_CC &&
        //            n == sizeof(UELYSYS_STX_GetReport_Report_t)) {
        //     UELYSYS_STX_GetReport_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nstate=%" PRIu8 "\nflags=0x%02X\ncpu_temperature=%f\nfirmware_version=%" PRIu32 "\n",
        //             x.ret, x.state, x.flags, x.cpu_temperature, x.firmware_version);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_GET_MOD_DATA_IFACE_CC &&
        //            n == sizeof(UELYSYS_STX_ModDataIface_Report_t)) {
        //     UELYSYS_STX_ModDataIface_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\ninterface_type=%" PRIu8 "\nlvds_io_type=%" PRIu8 "\n",
        //             x.ret, x.interface_type, x.lvds_io_type);
        // } else if ((rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_DIR ||
        //             rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_DIRNEXT) && n >= 4) {
        //     int32_t ret = 0; memcpy(&ret, p, sizeof(ret));
        //     fprintf(fp, "device_result=%" PRId32 "\n", ret);
        //     const uint16_t complete = (uint16_t)((n - 4) / sizeof(UELYSYS_STX_DirEntry_Report_t));
        //     fprintf(fp, "complete_entry_count=%" PRIu16 "\n", complete);
        //     for (uint16_t i = 0; i < complete; ++i) {
        //         UELYSYS_STX_DirEntry_Report_t x{};
        //         memcpy(&x, p + 4 + i * sizeof(x), sizeof(x));
        //         fprintf(fp, "file[%" PRIu16 "].name=%.*s\nfile[%" PRIu16 "].size=%" PRIu32 "\n",
        //                 i, (int)sizeof(x.name), x.name, i, x.length);
        //     }
        //     const uint16_t used = (uint16_t)(4 + complete * sizeof(UELYSYS_STX_DirEntry_Report_t));
        //     if (used < n) DumpIndexedBytes(fp, "truncated_entry_bytes", p + used, (uint16_t)(n - used));
        // } else if (rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_CREATEFILE &&
        //            n == sizeof(UELYSYS_STX_CreateFile_Report_t)) {
        //     UELYSYS_STX_CreateFile_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nfile_handle=%" PRIu32 "\n", x.ret, x.file_handle);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_WRITEFILE &&
        //            n == sizeof(UELYSYS_STX_WriteFile64_Report_t)) {
        //     UELYSYS_STX_WriteFile64_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nfile_handle=%" PRIu32 "\nremaining=%" PRIu64
        //                 "\npacket_number=%" PRIu32 "\nerr_count=%" PRIu8 "\n",
        //             x.ret, x.file_handle, x.remaining, x.packet_number, x.err_count);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_WRITEFILE &&
        //            n == sizeof(UELYSYS_STX_WriteFile32_Report_t)) {
        //     UELYSYS_STX_WriteFile32_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nfile_handle=%" PRIu32 "\nremaining=%" PRIu32
        //                 "\npacket_number=%" PRIu32 "\nerr_count=%" PRIu8 "\n",
        //             x.ret, x.file_handle, x.remaining, x.packet_number, x.err_count);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_OPENFILE &&
        //            n == sizeof(UELYSYS_STX_OpenFile_Report_t)) {
        //     UELYSYS_STX_OpenFile_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\nfile_handle=%" PRId32 "\nfile_length=%" PRIu32 "\n",
        //             x.ret, x.file_handle, x.file_length);
        // } else if (rpt->ReflectedCC == UELYSYS_STX_FILESYS_CC_READFILE && n >= 10) {
        //     UELYSYS_STX_ReadFileHeader_Report_t x{}; memcpy(&x, p, sizeof(x));
        //     fprintf(fp, "device_result=%" PRId32 "\npacket_len=%" PRIu16 "\npacket_number=%" PRIu32
        //                 "\ncopied_data_size=%" PRIu16 "\n",
        //             x.ret, x.packet_len, x.packet_number, (uint16_t)(n - sizeof(x)));
        //     DumpIndexedBytes(fp, "file_data", p + sizeof(x), (uint16_t)(n - sizeof(x)));
        // } else if (n == sizeof(int32_t)) {
        //     int32_t ret = 0; memcpy(&ret, p, sizeof(ret)); fprintf(fp, "device_result=%" PRId32 "\n", ret);
        // } else {
        //     DumpIndexedBytes(fp, "return_data", p, n);
        // }
        // return;
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_UANT_CMD_ID || rpt->ReflectedMID == 0x4018)
    {
        fprintf(fp, "\n[UELYSYS UANT]\n");
        if (rpt->ReflectedCC == UELYSYS_UANT_GET_STATUS_CC && n == sizeof(UELYSYS_UANT_ReleaseStatus_Report_t)) {
            static const char *names[8] = {"channel_0_state", "channel_0_status", "channel_0_burn_time_left",
                                           "channel_0_burn_tries", "channel_1_state", "channel_1_status",
                                           "channel_1_burn_time_left", "channel_1_burn_tries"};
            for (int i = 0; i < 8; ++i) fprintf(fp, "%s=%" PRIu8 "\n", names[i], p[i]);
        } else if (rpt->ReflectedCC == UELYSYS_UANT_GET_BACKUP_STATUS_CC &&
                   n == sizeof(UELYSYS_UANT_BackupStatus_Report_t)) {
            UELYSYS_UANT_BackupStatus_Report_t x{}; memcpy(&x, p, sizeof(x));
            fprintf(fp, "state=%" PRIu8 "\nseconds_to_deploy=%" PRIu32 "\n", x.state, x.seconds_to_deploy);
        } else if (rpt->ReflectedCC == UELYSYS_UANT_GET_BOARD_STATUS_CC &&
                   n == sizeof(UELYSYS_UANT_BoardStatus_Report_t)) {
            UELYSYS_UANT_BoardStatus_Report_t x{};
            memcpy(&x, p, sizeof(x));
            fprintf(fp, "seconds_since_boot=%" PRIu32 "\nreboot_count=%" PRIu8 "\n",
                    x.seconds_since_boot, x.reboot_count);
        } else if (rpt->ReflectedCC == UELYSYS_UANT_GET_TEMPERATURE_CC &&
                   n == sizeof(UELYSYS_UANT_Temperature_Report_t)) {
            UELYSYS_UANT_Temperature_Report_t x{}; memcpy(&x, p, sizeof(x));
            fprintf(fp, "temperature=%" PRId16 "\n", x.temperature);
        } else if (rpt->ReflectedCC == UELYSYS_UANT_GET_SETTINGS_CC &&
                   n == sizeof(UELYSYS_UANT_Settings_Report_t)) {
            UELYSYS_UANT_Settings_Report_t x{}; memcpy(&x, p, sizeof(x));
            fprintf(fp, "minutes=%" PRIu16 "\nbackup_active=%" PRIu8 "\nmax_burn_duration=%" PRIu8 "\n",
                    x.minutes, x.backup_active, x.max_burn_duration);
        } else if (rpt->ReflectedCC == UELYSYS_UANT_AUTODEPLOY_CC && n == 1) {
            fprintf(fp, "autodeploy_result=%" PRId8 "\n", (int8_t)p[0]);
        } else if (n == 0) {
            fprintf(fp, "status_only=true\n");
        } else {
            DumpIndexedBytes(fp, "return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_UTRX_CMD_ID || rpt->ReflectedMID == 0x5018) {
        fprintf(fp, "\n[UELYSYS UTRX]\n");
        if ((rpt->ReflectedCC == UELYSYS_UTRX_NOOP_CC ||
             rpt->ReflectedCC == UELYSYS_UTRX_RESET_COUNTERS_CC ||
             rpt->ReflectedCC == UELYSYS_UTRX_RESET_APP_COUNTERS_CC ||
             rpt->ReflectedCC == UELYSYS_UTRX_RESET_DEVICE_COUNTERS_CC) &&
            n == sizeof(UELYSYS_UTRX_Noop_Report_t)) {
            fprintf(fp, "cmd_counter=%" PRIu8 "\napp_err_counter=%" PRIu8 "\ndevice_err_counter=%" PRIu8 "\n",
                    decoded.u.utrx_noop.cmd_counter, decoded.u.utrx_noop.app_err_counter,
                    decoded.u.utrx_noop.device_err_counter);
        } else if ((rpt->ReflectedCC == UELYSYS_UTRX_RXCONF_GET_BAUD_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TXCONF_GET_BAUD_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_BOOT_CAUSE_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_LAST_CONTACT_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_TOT_TX_BYTES_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_TOT_RX_BYTES_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_RXCONF_GET_FREQ_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TXCONF_GET_FREQ_CC) && n == sizeof(uint32_t)) {
            uint32_t value = 0; memcpy(&value, p, sizeof(value));
            fprintf(fp, "value_u32=%" PRIu32 "\n", value);
        } else if ((rpt->ReflectedCC == UELYSYS_UTRX_RXCONF_GET_GUARD_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_BOOT_COUNT_CC) && n == sizeof(uint16_t)) {
            uint16_t value = 0; memcpy(&value, p, sizeof(value));
            fprintf(fp, "value_u16=%" PRIu16 "\n", value);
        } else if ((rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_TEMP_BRD_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_LAST_RSSI_CC ||
                    rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_LAST_RFERR_CC) && n == sizeof(int16_t)) {
            int16_t value = 0; memcpy(&value, p, sizeof(value));
            fprintf(fp, "value_i16=%" PRId16 "\n", value);
        } else if (rpt->ReflectedCC == UELYSYS_UTRX_TLM_GET_ACTIVE_CONF_CC && n == sizeof(uint8_t)) {
            fprintf(fp, "active_configuration=%" PRIu8 "\n", p[0]);
        } else if (n == 0) {
            fprintf(fp, "ack_payload=empty\n");
        } else {
            fprintf(fp, "payload_size_mismatch=1\n");
            DumpIndexedBytes(fp, "unexpected_return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_PAYUEL_OBC_CMD_ID || rpt->ReflectedMID == 0xC018 ||
        rpt->ReflectedMID == UELYSYS_PAYUEL_CAM_CMD_ID || rpt->ReflectedMID == 0xC318) {
        const bool obc = rpt->ReflectedMID == UELYSYS_PAYUEL_OBC_CMD_ID || rpt->ReflectedMID == 0xC018;
        fprintf(fp, "\n[UELYSYS PAYUEL %s]\n", obc ? "OBC" : "CAM");
        const bool meta = (obc && (rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC ||
                                  rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_SENSOR_META_CC)) ||
                          (!obc && rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC);
        const bool download = (obc && (rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC ||
                                      rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC)) ||
                              (!obc && rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC);
        const bool chunk = (obc && (rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_CHUNK_DOWNLOAD_CC ||
                                   rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_SENSOR_CHUNK_CC)) ||
                           (!obc && rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_CHUNK_DOWNLOAD_CC);
        const bool status = (obc && rpt->ReflectedCC == UELYSYS_PAYUEL_OBC_CAM_SHOT_CC) ||
                            (!obc && (rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_SHOT_CC ||
                                     rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC ||
                                     rpt->ReflectedCC == UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC));
        if ((meta || download) && n == sizeof(UELYSYS_ImageMeta_Report_t)) {
            const UELYSYS_ImageMeta_Report_t &x = decoded.u.image_meta;
            fprintf(fp, "%s=0x%02X\nimage_index=%" PRIu8 "\nchunk_count=%" PRIu16
                        "\nlast_chunk_size=%" PRIu8 "\nfile_crc32=0x%08" PRIX32 "\n",
                    obc ? "camera_id" : "image_slot", x.object_id, x.image_index,
                    ReadU16BE(x.chunk_count_be), x.last_chunk_size, ReadU32BE(x.file_crc32_be));
        } else if (download && n == sizeof(UELYSYS_DownloadRequest_Report_t)) {
            const UELYSYS_DownloadRequest_Report_t &x = decoded.u.download_request;
            fprintf(fp, "%s=0x%02X\nimage_index=%" PRIu8 "\ndownload_state=queue_or_start_failure\n",
                    obc ? "camera_id" : "image_slot", x.object_id, x.image_index);
        } else if (status && n == sizeof(UELYSYS_PayloadStatus_Report_t)) {
            fprintf(fp, "hardware_status=0x%02X\n", decoded.u.payload_status.status);
        } else if ((meta || status || chunk) && n == sizeof(UELYSYS_PayloadError_Report_t)) {
            const UELYSYS_PayloadError_Report_t &x = decoded.u.payload_error;
            fprintf(fp, "error_command_id=0x%02X\nerror_status=0x%02X\nerror_crc16=0x%04X\n",
                    x.command_id, x.status, ReadU16BE(x.crc16_be));
            DumpIndexedBytes(fp, "payload_error",
                             reinterpret_cast<const uint8_t *>(&x), sizeof(x));
        } else if (n == 0) {
            fprintf(fp, "return_payload=empty\n");
        } else {
            fprintf(fp, "payload_size_mismatch=1\n");
            DumpIndexedBytes(fp, "unexpected_return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_PAYUEL_AOS_CMD_ID || rpt->ReflectedMID == 0x4718) {
        fprintf(fp, "\n[UELYSYS PAYUEL AOS]\n");
        if ((rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_NOOP_CC ||
             rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_RESET_COUNTERS_CC) &&
            n == sizeof(UELYSYS_AOS_Noop_Report_t)) {
            const UELYSYS_AOS_Noop_Report_t &x = decoded.u.aos_noop;
            fprintf(fp, "cmd_counter=%" PRIu8 "\nerr_counter=%" PRIu8 "\n", x.cmd_counter, x.err_counter);
        } else if (rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_READ_REGISTER_CC &&
            n == sizeof(UELYSYS_AOS_ReadRegister_Report_t)) {
            const UELYSYS_AOS_ReadRegister_Report_t &x = decoded.u.aos_read_register;
            fprintf(fp, "msb=0x%02X\nlsb=0x%02X\nraw_be=%" PRIi16 "\n",
                    x.msb, x.lsb, (int16_t)(((uint16_t)x.msb << 8) | x.lsb));
        } else if (rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC &&
                   n == sizeof(UELYSYS_AOS_ReadAll_Report_t)) {
            const UELYSYS_AOS_ReadAll_Report_t &x = decoded.u.aos_read_all;
            for (int i = 0; i < 4; ++i)
                fprintf(fp, "resistance[%d]=%f\n", i, x.resistance[i]);
        } else if ((rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_RESET_CC ||
                    rpt->ReflectedCC == UELYSYS_PAYUEL_AOS_WRITE_REGISTER_CC) && n == 0) {
            fprintf(fp, "ack_payload=empty\n");
        } else if (!payload_valid) {
            fprintf(fp, "payload_size_mismatch=1\n");
            DumpIndexedBytes(fp, "unexpected_return_data", p, n);
        } else {
            DumpIndexedBytes(fp, "return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_LGBAT_CMD_ID || rpt->ReflectedMID == 0xC618) {
        fprintf(fp, "\n[UELYSYS PAYUEL LGBAT]\n");
        if (rpt->ReflectedCC == UELYSYS_LGBAT_REQUEST_DATA_CC && n == 10) {
            DumpLgbatBlock(fp, decoded.u.lgbat_block);
        } else if (rpt->ReflectedCC == UELYSYS_LGBAT_REQUEST_ALL_DATA_CC && n == 120) {
            const uint8_t *all_blocks =
                reinterpret_cast<const uint8_t *>(&decoded.u.lgbat_all);
            for (int i = 0; i < 12; ++i) {
                fprintf(fp, "\n[BMS Block 0x%02X]\n", i + 1);
                UELYSYS_LGBAT_Block_Report_t block{};
                memcpy(block.raw, all_blocks + i * sizeof(block.raw), sizeof(block.raw));
                DumpLgbatBlock(fp, block);
            }
        } else if (rpt->ReflectedCC == UELYSYS_LGBAT_CRITICAL_ALERT_CC && n == 6) {
            const UELYSYS_LGBAT_CriticalAlert_Report_t &x = decoded.u.lgbat_critical;
            fprintf(fp, "failure_level=0x%02X\nvoltage_diag=0x%02X\ncurrent_diag=0x%02X\n"
                        "temperature_diag=0x%02X\nfail_status_2=0x%02X\nfail_status_3=0x%02X\n",
                    x.failure_level, x.voltage_diag, x.current_diag, x.temperature_diag,
                    x.fail_status_2, x.fail_status_3);
        } else if (n == 0) {
            fprintf(fp, "ack_payload=empty\n");
        } else if (!payload_valid) {
            fprintf(fp, "payload_size_mismatch=1\n");
            DumpIndexedBytes(fp, "unexpected_return_data", p, n);
        } else {
            DumpIndexedBytes(fp, "return_data", p, n);
        }
        return;
    }

    if (rpt->ReflectedMID == UELYSYS_PAYUEL_YONSEI_CMD_ID || rpt->ReflectedMID == 0xCC18) {
        fprintf(fp, "\n[UELYSYS PAYUEL YONSEI]\n");
        if ((rpt->ReflectedCC == UELYSYS_PAYUEL_YONSEI_NOOP_CC ||
             rpt->ReflectedCC == UELYSYS_PAYUEL_YONSEI_RESET_COUNTERS_CC) &&
            n == sizeof(decoded.u.yonsei_status)) {
            fprintf(fp, "command_counter=%" PRIu8 "\ncommand_error_counter=%" PRIu8 "\n",
                    decoded.u.yonsei_status.command_counter,
                    decoded.u.yonsei_status.command_error_counter);
        } else if (rpt->ReflectedCC == UELYSYS_PAYUEL_YONSEI_FORWARD_SB_CC &&
                   n == sizeof(decoded.u.yonsei_forward)) {
            const auto &x = decoded.u.yonsei_forward;
            fprintf(fp, "target_mid=0x%04X\narg_length=%" PRIu16 "\ntarget_cc=%" PRIu8
                        "\ntarget_node=%" PRIu8 "\ntarget_port=%" PRIu8 "\n",
                    x.target_mid, x.arg_length, x.target_fcn_code, x.target_node, x.target_port);
        } else {
            DumpIndexedBytes(fp, payload_valid ? "return_data" : "unexpected_return_data", p, n);
        }
        return;
    }

    if (decoded.kind == REPORT_KIND_UELYSYS_EO &&
        rpt->ReflectedCC == UELYSYS_EO_SEND_STATE_CC &&
        n == sizeof(decoded.u.eo_state)) {
        const auto &x = decoded.u.eo_state;
        fprintf(fp, "\n[UELYSYS EO]\ncurrent_phase=%" PRIu8
                    "\nburn_channel_on=%" PRIu8 "\nuant_release_verified=%" PRIu8
                    "\nburn_channel_off=%" PRIu8 "\ndetumble_cmd_sent=%" PRIu8
                    "\nto_lab_output_enabled=%" PRIu8 "\ndeployment_holdoff_complete=%" PRIu8
                    "\nholdoff_elapsed_sec=%" PRIu32 "\nstate_version=%" PRIu16
                    "\ncrc=0x%08" PRIX32 "\n",
                x.current_phase, x.burn_channel_on, x.uant_release_verified,
                x.burn_channel_off, x.detumble_cmd_sent, x.to_lab_output_enabled,
                x.deployment_holdoff_complete, x.holdoff_elapsed_accumulated_sec,
                x.state_version, x.crc);
        return;
    }

    if (decoded.kind == REPORT_KIND_UELYSYS_SCH_LAB &&
        (rpt->ReflectedCC == UELYSYS_SCH_LAB_SET_ENTRY_STATE_CC ||
         rpt->ReflectedCC == UELYSYS_SCH_LAB_SET_ENTRY_CONFIG_CC) &&
        n == sizeof(decoded.u.sch_entry)) {
        const auto &x = decoded.u.sch_entry;
        fprintf(fp, "\n[UELYSYS SCH_LAB]\nentry_index=%" PRIu16 "\nenabled=%" PRIu16
                    "\nmessage_id=0x%08" PRIX32 "\npacket_rate=%" PRIu32
                    "\nphase_ticks=%" PRIu32 "\nnot_before_tick=%" PRIu32 "\n",
                x.entry_index, x.enabled, x.message_id, x.packet_rate,
                x.phase_ticks, x.not_before_tick);
        return;
    }

    if (decoded.kind == REPORT_KIND_UELYSYS_SP &&
        (rpt->ReflectedCC == UELYSYS_SP_GET_AR6_STATUS_CC ||
         rpt->ReflectedCC == UELYSYS_SP_RESET_AR6_COUNTERS_CC) &&
        n == sizeof(decoded.u.sp_readback)) {
        const auto &x = decoded.u.sp_readback;
        fprintf(fp, "\n[UELYSYS SP]\nar6_addr=%" PRIu8 "\nreachable=%" PRIu8
                    "\nburn_state=%" PRIu8 "\nrelease_status=%" PRIu8
                    "\nburn_time_left=%" PRIu8 "\nburn_tries=%" PRIu8
                    "\ntemperature=%" PRId16 "\nseconds_since_boot=%" PRIu32
                    "\nreboot_count=%" PRIu8 "\n",
                x.ar6_addr, x.reachable, x.burn_state, x.release_status,
                x.burn_time_left, x.burn_tries, x.temperature,
                x.seconds_since_boot, x.reboot_count);
        return;
    }

    fprintf(fp, "\n[UELYSYS APP REPORT]\n");
    DumpIndexedBytes(fp, payload_valid ? "return_data" : "unexpected_return_data", p, n);
}

static void DumpADCSReportParsed(FILE *fp, const Report *rpt)
{
    ReportView_t v{};
    v.reflected_msg_id = rpt->ReflectedMID;
    v.reflected_cc = rpt->ReflectedCC;
    v.ret_code = rpt->RetCode;
    v.kind = DetermineReportKind(v.reflected_msg_id, v.reflected_cc);
    const uint16_t n = (rpt->RetValSize > sizeof(rpt->RetVal))
                           ? (uint16_t)sizeof(rpt->RetVal)
                           : rpt->RetValSize;
    const bool ok = DecodeN5PayloadToView(v, rpt->RetVal, n);

    fprintf(fp, "\n[ADCS Report]\npayload_valid=%u\n", ok);
    switch (v.kind) {
        case REPORT_KIND_ADCS_HK_REPORT:
            fprintf(fp, "Type=HK\nMAG0MCUCurrent=%u\nFSS0MCUTemperature=%d\n"
                        "FSS0MCUCurrent=%u\nFSS0MCUVoltage=%u\nFSS0CAMSRAMOvercurrent=%u\n"
                        "MTQCurrentAverage=%f,%f,%f,%f,%f,%f\nMTQPolarity=0x%02X\n"
                        "bytesReceived=%" PRIu32 "\nMTQMaxDipole=%f,%f,%f\n"
                        "MTQOnTimeMax=%u\nMTQFilterFactor=%f\n"
                        "CSSRaw=%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n"
                        "RawCSSValid=%u\nCSSCalVector=%d,%d,%d\nCalCSSValid=%u\n",
                    v.u.adcs_hk.MAG0MCUCurrent, v.u.adcs_hk.FSS0MCUTemperature,
                    v.u.adcs_hk.FSS0MCUCurrent, v.u.adcs_hk.FSS0MCUVoltage,
                    v.u.adcs_hk.FSS0CAMSRAMOvercurrent,
                    v.u.adcs_hk.MTQ1PositiveCurrentAverage, v.u.adcs_hk.MTQ1NegativeCurrentAverage,
                    v.u.adcs_hk.MTQ2PositiveCurrentAverage, v.u.adcs_hk.MTQ2NegativeCurrentAverage,
                    v.u.adcs_hk.MTQ3PositiveCurrentAverage, v.u.adcs_hk.MTQ3NegativeCurrentAverage,
                    v.u.adcs_hk.MTQPolarity, v.u.adcs_hk.bytesReceived,
                    v.u.adcs_hk.mtq0Mmax, v.u.adcs_hk.mtq1Mmax, v.u.adcs_hk.mtq2Mmax,
                    v.u.adcs_hk.onTimeMax, v.u.adcs_hk.mtqFfac,
                    v.u.adcs_hk.css0Raw, v.u.adcs_hk.css1Raw, v.u.adcs_hk.css2Raw,
                    v.u.adcs_hk.css3Raw, v.u.adcs_hk.css4Raw, v.u.adcs_hk.css5Raw,
                    v.u.adcs_hk.css6Raw, v.u.adcs_hk.css7Raw, v.u.adcs_hk.css8Raw,
                    v.u.adcs_hk.css9Raw, v.u.adcs_hk.rawCssIsValid,
                    v.u.adcs_hk.cssCalVecX, v.u.adcs_hk.cssCalVecY,
                    v.u.adcs_hk.cssCalVecZ, v.u.adcs_hk.calCssIsValid);
            break;
        case REPORT_KIND_ADCS_NOOP_REPORT:
            if (n == 14) fprintf(fp, "Type=NOOP\nVersion=%.*s\n", (int)n, (const char *)rpt->RetVal);
            else if (n == 2) fprintf(fp, "Type=NOOP\nCmdCounter=%u\nErrCounter=%u\n",
                                    rpt->RetVal[0], rpt->RetVal[1]);
            break;
        case REPORT_KIND_ADCS_RESET_COUNTERS_REPORT:
            if (n == 2) fprintf(fp, "Type=RESET_COUNTERS\nCmdCounter=%u\nErrCounter=%u\n",
                                rpt->RetVal[0], rpt->RetVal[1]);
            break;
        case REPORT_KIND_ADCS_INTERFACE_TRANSPORT_REPORT:
            fprintf(fp, "Type=SET_INTERFACE_TRANSPORT\nTransportType=%u\n",
                    v.u.adcs_interface_transport.TransportType);
            break;
        case REPORT_KIND_ADCS_GYRO_LOG_REPORT:
            fprintf(fp, "Type=GYRO_LOG\nDurationSec=%u\nSamplesWritten=%u\nReadErrors=%u\nGyroSource=%u\n",
                    v.u.adcs_gyro_log.DurationSec, v.u.adcs_gyro_log.SamplesWritten,
                    v.u.adcs_gyro_log.ReadErrors, v.u.adcs_gyro_log.GyroSource);
            break;
        case REPORT_KIND_ADCS_GET_ERROR_LOG_SETTING:
            fprintf(fp, "ActiveState=%u\nBufferFullAction=%u\n",
                    v.u.adcs_errorlogsetting.ActiveState, v.u.adcs_errorlogsetting.BufferFullAction);
            break;
        case REPORT_KIND_ADCS_GET_CURRENT_UNIX_TIME:
            fprintf(fp, "CurrentUnixseconds=%" PRIu32 "\nCurrentUnixNanoseconds=%" PRIu32 "\n",
                    v.u.adcs_currentunixtime.CurrentUnixseconds,
                    v.u.adcs_currentunixtime.CurrentUnixNanoseconds);
            break;
        case REPORT_KIND_ADCS_GET_PERSIST_CONFIG_DIAGNOSTIC:
            fprintf(fp, "State=%u\nLastResult=%u\nTimestamp=%" PRIu32 "\n",
                    v.u.adcs_persistconfigdiagnostic.State,
                    v.u.adcs_persistconfigdiagnostic.LastResult,
                    v.u.adcs_persistconfigdiagnostic.Timestamp);
            break;
        case REPORT_KIND_ADCS_GET_COMMUNICATION_STATUS: {
            const auto &p = v.u.adcs_communicationstatus;
            fprintf(fp, "UART=%u,%u,%u,%u\nUART2=%u,%u,%u,%u\nCAN=%u,%u,%u,%u\nI2C=%u,%u,%u,%u\n",
                    p.UART_TcCnt, p.UART_TlmCnt, p.UART_ErrSW, p.UART_ErrHW,
                    p.UART2_TcCnt, p.UART2_TlmCnt, p.UART2_ErrSW, p.UART2_ErrHW,
                    p.CAN_TcCnt, p.CAN_TlmCnt, p.CAN_ErrSW, p.CAN_ErrHW,
                    p.I2C_TcCnt, p.I2C_TlmCnt, p.I2C_ErrSW, p.I2C_ErrHW);
            break;
        }
        case REPORT_KIND_ADCS_GET_CONTROL_ESTIMATION_MODE:
            fprintf(fp, "ControlMode=%u\nMainEstimatorMode=%u\nBackupEstimatorMode=%u\nControlTimeout=%u\n",
                    v.u.adcs_controlestimationmode.ControlMode,
                    v.u.adcs_controlestimationmode.MainEstimatorMode,
                    v.u.adcs_controlestimationmode.BackupEstimatorMode,
                    v.u.adcs_controlestimationmode.ControlTimeout);
            break;
        case REPORT_KIND_ADCS_GET_REFERENCE_IRC_VECTOR:
            fprintf(fp, "ECIPointingVector=%f,%f,%f\n",
                    v.u.adcs_referenceircvector.ECIPointingVectorX,
                    v.u.adcs_referenceircvector.ECIPointingVectorY,
                    v.u.adcs_referenceircvector.ECIPointingVectorZ);
            break;
        case REPORT_KIND_ADCS_GET_REFERENCE_LLH_TARGET:
            fprintf(fp, "LLH=%f,%f,%f\n", v.u.adcs_referencellhtarget.Latitude,
                    v.u.adcs_referencellhtarget.Longitude, v.u.adcs_referencellhtarget.Altitude);
            break;
        case REPORT_KIND_ADCS_GET_ORBIT_MODE:
            fprintf(fp, "OrbitMode=%u\n", v.u.adcs_orbitmode.OrbitMode);
            break;
        case REPORT_KIND_ADCS_GET_REFERENCE_RPY_VALUES:
            fprintf(fp, "Roll=%f\nPitch=%f\nYaw=%f\n", v.u.adcs_referencerpyvalues.Roll,
                    v.u.adcs_referencerpyvalues.Pitch, v.u.adcs_referencerpyvalues.Yaw);
            break;
        case REPORT_KIND_ADCS_GET_OPENLOOPCMD_MTQ:
            fprintf(fp, "MTQ=%d,%d,%d\n", v.u.adcs_openloopcmdmtq.MTQ0_OpenLoopCmd,
                    v.u.adcs_openloopcmdmtq.MTQ1_OpenLoopCmd,
                    v.u.adcs_openloopcmdmtq.MTQ2_OpenLoopCmd);
            break;
        case REPORT_KIND_ADCS_GET_RUN_MODE:
            fprintf(fp, "RunMode=%u\n", v.u.adcs_runmode.RunMode);
            break;
        case REPORT_KIND_ADCS_GET_CONTROL_MODE:
            fprintf(fp, "ControlMode=%u\nControlTimeout=%u\n", v.u.adcs_controlmode.ControlMode,
                    v.u.adcs_controlmode.ControlTimeout);
            break;
        case REPORT_KIND_ADCS_GET_ESTIMATION_MODE:
            fprintf(fp, "MainEstimatorMode=%u\nBackupEstimatorMode=%u\n",
                    v.u.adcs_estimationmode.MainEstimatorMode,
                    v.u.adcs_estimationmode.BackupEstimatorMode);
            break;
        case REPORT_KIND_ADCS_GET_OPERATIONAL_STATE:
            fprintf(fp, "OperationalMode=%u\n", v.u.adcs_operationalstate.OperationalMode);
            break;
        case REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG:
            fprintf(fp, "Mag0SensingElement=%u\nMag1SensingElement=%u\n",
                    v.u.adcs_magsensingelmconfig.Mag0SensingElement,
                    v.u.adcs_magsensingelmconfig.Mag1SensingElement);
            break;
        case REPORT_KIND_ADCS_GET_WHL_CONFIG: {
            const auto &p = v.u.adcs_whlconfig;
            fprintf(fp, "RWL0=%f,%f,%f\nRWL1=%f,%f,%f\nRWL2=%f,%f,%f\nRWL3=%f,%f,%f\n"
                        "WheelRampTorque=%f\nWheelScheme=%u\nFailedWheelID=%u\nPyramidNominalMomentum=%f\nPyramidTiltAngle=%f\n",
                    p.Rwl0Inertia, p.Rwl0MaxMomentum, p.Rwl0MaxToque,
                    p.Rwl1Inertia, p.Rwl1MaxMomentum, p.Rwl1MaxToque,
                    p.Rwl2Inertia, p.Rwl2MaxMomentum, p.Rwl2MaxToque,
                    p.Rwl3Inertia, p.Rwl3MaxMomentum, p.Rwl3MaxToque,
                    p.WheelRampTorque, p.WheelScheme, p.FailedWheelID,
                    p.PyramidNominalMomentum, p.PyramidTiltAngle);
            break;
        }
        case REPORT_KIND_ADCS_GET_SATELLITE_CONFIG: {
            const auto &p = v.u.adcs_satelliteconfig;
            fprintf(fp, "Inertia=%f,%f,%f,%f,%f,%f\nSunBody=%d,%d,%d\nTargetBody=%d,%d,%d\nSatBody=%d,%d,%d\n",
                    p.Ixx, p.Iyy, p.Izz, p.Ixy, p.Ixz, p.Iyz,
                    p.SunPointingBodyVectorX, p.SunPointingBodyVectorY, p.SunPointingBodyVectorZ,
                    p.TargetTrackingBodyVectorX, p.TargetTrackingBodyVectorY, p.TargetTrackingBodyVectorZ,
                    p.SatTrackingBodyVectorX, p.SatTrackingBodyVectorY, p.SatTrackingBodyVectorZ);
            break;
        }
        case REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG: {
            const auto &p = v.u.adcs_controllerconfig;
            fprintf(fp, "DefaultControlMode=%u\nGains=%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n"
                        "ReferenceSpinRate=%f\nReferenceWheelMomentum=%f\nYWheelBiasMomentum=%f\nRefSpinRateRW=%f\nSunKeepOutAngle=%f\nRollLimitAngle=%f\nFlags=%u,%u,%u\n",
                    p.DefaultControlMode, p.DetumblingDampingGain, p.SunSpinGain_Sunlit,
                    p.SunSpinGain_Eclipse, p.DetumblingSpinGain, p.FastBDotGain,
                    p.YMomNutationDampingGain, p.YMomNutationDampingQuatGain,
                    p.XGGQuatGain, p.YGGQuatGain, p.ZGGQuatGain, p.WheelDesatControlGain,
                    p.YMomProportionalGain, p.YMomDerivativeGain, p.RWheelProportionalGain,
                    p.RWheelDerivativeGain, p.TrackingProportionalGain,
                    p.TrackingDerivativeGain, p.TrackingIntegralGain,
                    p.ReferenceSpinRate_degps, p.ReferenceWheelMomentum_Nms,
                    p.YWheelBiasMomentum_Nms, p.RefSpinRate_RW_degps, p.SunKeepOutAngle_deg,
                    p.RollLimitAngle_deg, p.flags.YawCompensationForEarthRotation,
                    p.flags.EnableSunTrackingInEclipse, p.flags.EnableSunAvoidance);
            break;
        }
        case REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG:
            fprintf(fp, "DefaultRunMode=%u\nDefaultOperationalState=%u\nDefaultSafeControlMode=%u\nDefaultAutoControlMode=%u\n",
                    v.u.adcs_defaultmodeconfig.DefaultRunMode,
                    v.u.adcs_defaultmodeconfig.DefaultOperationalState,
                    v.u.adcs_defaultmodeconfig.DefaultControlModeInOpStateSafe,
                    v.u.adcs_defaultmodeconfig.DefaultControlModeInOpStateAuto);
            break;
        case REPORT_KIND_ADCS_GET_MOUNTING_CONFIG: {
            const auto &p = v.u.adcs_mountingconfig;
            fprintf(fp, "Stack=%u,%u,%u\nMTQ=%u,%u,%u\nWheel=%u,%u,%u,%u\n"
                        "PyramidRWL=%d,%d,%d\nCSS=%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n"
                        "FSS0=%d,%d,%d\nFSS1=%d,%d,%d\nFSS2=%d,%d,%d\nFSS3=%d,%d,%d\n"
                        "HSS0=%d,%d,%d\nHSS1=%d,%d,%d\nMAG0=%d,%d,%d\nMAG1=%d,%d,%d\n"
                        "STR0=%d,%d,%d\nSTR1=%d,%d,%d\nExtSensor0=%d,%d,%d\n"
                        "ExtSensor1=%d,%d,%d\nExtGyro0=%u,%u,%u\nExtGyro1=%u,%u,%u\n",
                    p.StackX_mounting, p.StackY_mounting, p.StackZ_mounting,
                    p.MTQ0_mounting, p.MTQ1_mounting, p.MTQ2_mounting,
                    p.Wheel0_mounting, p.Wheel1_mounting, p.Wheel2_mounting, p.Wheel3_mounting,
                    p.PyramidRWL_alpha, p.PyramidRWL_beta, p.PyramidRWL_gamma,
                    p.CSS0_mounting, p.CSS1_mounting, p.CSS2_mounting, p.CSS3_mounting,
                    p.CSS4_mounting, p.CSS5_mounting, p.CSS6_mounting, p.CSS7_mounting,
                    p.CSS8_mounting, p.CSS9_mounting,
                    p.FSS0_alpha, p.FSS0_beta, p.FSS0_gamma,
                    p.FSS1_alpha, p.FSS1_beta, p.FSS1_gamma,
                    p.FSS2_alpha, p.FSS2_beta, p.FSS2_gamma,
                    p.FSS3_alpha, p.FSS3_beta, p.FSS3_gamma,
                    p.HSS0_alpha, p.HSS0_beta, p.HSS0_gamma,
                    p.HSS1_alpha, p.HSS1_beta, p.HSS1_gamma,
                    p.MAG0_alpha, p.MAG0_beta, p.MAG0_gamma,
                    p.MAG1_alpha, p.MAG1_beta, p.MAG1_gamma,
                    p.STR0_alpha, p.STR0_beta, p.STR0_gamma,
                    p.STR1_alpha, p.STR1_beta, p.STR1_gamma,
                    p.ExtSensor0_alpha, p.ExtSensor0_beta, p.ExtSensor0_gamma,
                    p.ExtSensor1_alpha, p.ExtSensor1_beta, p.ExtSensor1_gamma,
                    p.ExtGyro0_axis1_mounting, p.ExtGyro0_axis2_mounting,
                    p.ExtGyro0_axis3_mounting, p.ExtGyro1_axis1_mounting,
                    p.ExtGyro1_axis2_mounting, p.ExtGyro1_axis3_mounting);
            break;
        }
        case REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG: {
            const auto &p = v.u.adcs_estimatorconfig;
            fprintf(fp, "DefaultEstimators=%u,%u\nNoise=%f,%f,%f,%f,%f,%f,%f\nNutation=%f,%f\n"
                        "UseSensors=%u,%u,%u,%u\nTriadVectors=%u,%u\n",
                    p.DefaultMainEstimatorMode, p.DefaultBackupEstimatorMode,
                    p.MAGMeasurementNoise, p.CSSMeasurementNoise, p.FSSMeasurementNoise,
                    p.HSSMeasurementNoise, p.STRMeasurementNoise, p.MMTRKFSystemNoise,
                    p.EKFSystemNoise, p.NutationEpsilonCorrection, p.NutationPsiCorrection,
                    p.UseFSSinEKF, p.UseCSSinEKF, p.UseHSSinEKF, p.UseSTRinEKF,
                    p.TriadVector1, p.TriadVector2);
            break;
        }
        case REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG: {
            const auto &p = v.u.adcs_satorbitparamconfig;
            fprintf(fp, "Epoch=%lf\nInclination=%lf\nRAAN=%lf\nEccentricity=%lf\nAOP=%lf\n"
                        "MeanAnomaly=%lf\nMeanMotion=%lf\nB_StarDrag=%lf\n",
                    p.Epoch, p.Inclination, p.RAAN, p.Eccentricity, p.AOP,
                    p.MeanAnomaly, p.MeanMotion, p.B_StarDrag);
            break;
        }
        case REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG: {
            const auto &p = v.u.adcs_nodeselectionconfig;
            fprintf(fp, "RWL=0x%02X\nMAG=0x%02X\nFSS=0x%02X\nHSS=0x%02X\nGYR=0x%02X\nSTR=0x%02X\nGNSS=0x%02X\nExtSensor=0x%02X\n",
                    p.RWLSelectionFlags, p.MAGSelectionFlags, p.FSSSelectionFlags,
                    p.HSSSelectionFlags, p.GYRSelectionFlags, p.STRSelectionFlags,
                    p.GNSSSelectionFlags, p.ExtSensorSelectionFlags);
            break;
        }
        case REPORT_KIND_ADCS_GET_MTQ_CONFIG:
            fprintf(fp, "MaxDipole=%f,%f,%f\nMaxOnTime=%u\nMinOnTime=%u\nFilterFactor=%f\n",
                    v.u.adcs_mtqconfig.MTQ0MaxDipoleMoment,
                    v.u.adcs_mtqconfig.MTQ1MaxDipoleMoment,
                    v.u.adcs_mtqconfig.MTQ2MaxDipoleMoment,
                    v.u.adcs_mtqconfig.MaxMTQOnTime, v.u.adcs_mtqconfig.MinMTQOnTime,
                    v.u.adcs_mtqconfig.MagneticControlFilterFactor);
            break;
        case REPORT_KIND_ADCS_GET_RAW_RWL_SENSOR: {
            const auto &p = v.u.adcs_rawrwlsensor;
            fprintf(fp, "Time=%" PRIu32 ",%" PRIu32 "\nRWL=%f,%f,%f,%f\nValid=%u,%u,%u,%u\n",
                    p.TimeSeconds, p.TimeNanoSeconds, p.RWL0MeasuredSpeed,
                    p.RWL1MeasuredSpeed, p.RWL2MeasuredSpeed, p.RWL3MeasuredSpeed,
                    p.RWL0ValidFlag, p.RWL1ValidFlag, p.RWL2ValidFlag, p.RWL3ValidFlag);
            break;
        }
        case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_FSS: {
            const auto &p = v.u.adcs_rawcalibratedfss;
            fprintf(fp, "Type=BEE1012_RAW_CALIBRATED_FSS\nRawTime=%" PRIu32 ",%" PRIu32
                        "\nCalTime=%" PRIu32 ",%" PRIu32 "\nCalFSS0=%d,%d,%d\nCalFSS1=%d,%d,%d\n"
                        "CalFSS2=%d,%d,%d\nCalFSS3=%d,%d,%d\nValid=%u,%u,%u,%u\nBest=%u,%u,%u,%u\n",
                    p.Raw.TimeSecond, p.Raw.TimeNanoSecond,
                    p.Calibrated.TimeSeconds, p.Calibrated.TimeNanoSeconds,
                    p.Calibrated.FSS0CalVecX, p.Calibrated.FSS0CalVecY, p.Calibrated.FSS0CalVecZ,
                    p.Calibrated.FSS1CalVecX, p.Calibrated.FSS1CalVecY, p.Calibrated.FSS1CalVecZ,
                    p.Calibrated.FSS2CalVecX, p.Calibrated.FSS2CalVecY, p.Calibrated.FSS2CalVecZ,
                    p.Calibrated.FSS3CalVecX, p.Calibrated.FSS3CalVecY, p.Calibrated.FSS3CalVecZ,
                    p.Calibrated.FSS0ValidFlag, p.Calibrated.FSS1ValidFlag,
                    p.Calibrated.FSS2ValidFlag, p.Calibrated.FSS3ValidFlag,
                    p.Calibrated.FSS0BestFlag, p.Calibrated.FSS1BestFlag,
                    p.Calibrated.FSS2BestFlag, p.Calibrated.FSS3BestFlag);
            break;
        }
        case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_CSS:
            fprintf(fp, "Type=BEE1012_RAW_CALIBRATED_CSS\nCalVector=%d,%d,%d\nCalValid=%u\n",
                    v.u.adcs_rawcalibratedcss.Calibrated.CSSCalUnitVecX,
                    v.u.adcs_rawcalibratedcss.Calibrated.CSSCalUnitVecY,
                    v.u.adcs_rawcalibratedcss.Calibrated.CSSCalUnitVecZ,
                    v.u.adcs_rawcalibratedcss.Calibrated.CSSValidFlag);
            break;
        case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_GYR:
            fprintf(fp, "Type=BEE1012_RAW_CALIBRATED_GYR\nRawTime=%" PRIu32 ",%" PRIu32
                        "\nRawGYR0=%f,%f,%f\nRawGYR1=%f,%f,%f\nCalGYR0=%f,%f,%f\nCalGYR1=%f,%f,%f\n",
                    v.u.adcs_rawcalibratedgyr.Raw.TimeSeconds,
                    v.u.adcs_rawcalibratedgyr.Raw.TimeNanoSeconds,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR0RawRateX,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR0RawRateY,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR0RawRateZ,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR1RawRateX,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR1RawRateY,
                    v.u.adcs_rawcalibratedgyr.Raw.GYR1RawRateZ,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR0CalibratedRateX,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR0CalibratedRateY,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR0CalibratedRateZ,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR1CalibratedRateX,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR1CalibratedRateY,
                    v.u.adcs_rawcalibratedgyr.Calibrated.GYR1CalibratedRateZ);
            break;
        case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_RWL:
            fprintf(fp, "Type=BEE1012_RAW_CALIBRATED_RWL\nRawRWL=%f,%f,%f,%f\nTorque=%f,%f,%f\nMomentum=%f,%f,%f\n",
                    v.u.adcs_rawcalibratedrwl.Raw.RWL0MeasuredSpeed,
                    v.u.adcs_rawcalibratedrwl.Raw.RWL1MeasuredSpeed,
                    v.u.adcs_rawcalibratedrwl.Raw.RWL2MeasuredSpeed,
                    v.u.adcs_rawcalibratedrwl.Raw.RWL3MeasuredSpeed,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCTrqX,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCTrqY,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCTrqZ,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCMomX,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCMomY,
                    v.u.adcs_rawcalibratedrwl.Calibrated.WhlSBCMomZ);
            break;
        case REPORT_KIND_ADCS_COMMAND_REPORT:
            fprintf(fp, "Type=COMMAND_STATUS_OR_SEQUENCE_STAGE\n");
            break;
        default:
            fprintf(fp, "Type=ADCS_TYPED_REPORT_CC_%u\n", rpt->ReflectedCC);
            break;
    }
    if (n > 0) DumpIndexedBytes(fp, "typed_payload_bytes", rpt->RetVal, n);
}

static void DumpPaySltReportParsed(FILE *fp, const Report *rpt)
{
    const uint16_t n = (rpt->RetValSize > sizeof(rpt->RetVal))
                           ? (uint16_t)sizeof(rpt->RetVal) : rpt->RetValSize;
    const uint8_t *p = rpt->RetVal;
    ReportView_t v{};
    v.reflected_msg_id = rpt->ReflectedMID;
    v.reflected_cc = rpt->ReflectedCC;
    v.ret_type = rpt->RetType;
    v.ret_code = rpt->RetCode;
    v.kind = REPORT_KIND_BEE1012_PAY_SLT;
    const bool valid = DecodeN5PayloadToView(v, p, n);

    fprintf(fp, "\n[PAY_SLT %s]\n", PaySltCommandName(rpt->ReflectedCC));
    fprintf(fp, "payload_valid=%u\n", valid ? 1U : 0U);
    if (!valid) {
        fprintf(fp, "error=Unexpected ReturnDataSize for CC %u\n", (unsigned)rpt->ReflectedCC);
        return;
    }

    switch (rpt->ReflectedCC) {
        case BEE1012_PAY_SLT_NOOP_CC:
            if (n == 32) fprintf(fp, "Message=%.*s\n", 31, (const char *)p);
            break;
        case BEE1012_PAY_SLT_REPORT_BCN_CC:
            if (n == 1) fprintf(fp, "BcnEnabled=%u\n", (unsigned)p[0]);
            break;
        case BEE1012_PAY_SLT_OUTPUT_ENABLED_CC:
            if (n == 4) {
                fprintf(fp, "HkEnabled=%u\nBcnEnabled=%u\nIfbEnabled=%u\nExpEnabled=%u\n",
                        (unsigned)p[0], (unsigned)p[1], (unsigned)p[2], (unsigned)p[3]);
            }
            break;
        case BEE1012_PAY_SLT_RS422_PING_CC:
            if (n == 10) DumpIndexedBytes(fp, "RS422Rx", p, n);
            break;
        case BEE1012_PAY_SLT_PAR_GET_CC:
            if (n >= sizeof(PAY_SLT_ParamHeader_Report_t)) {
                PAY_SLT_ParamHeader_Report_t h{}; memcpy(&h, p, sizeof(h));
                fprintf(fp, "Node=%u\nTable=%u\nAddr=0x%04X\nType=%u\nCount=%u\n",
                        (unsigned)h.node, (unsigned)h.table, (unsigned)h.addr,
                        (unsigned)h.type, (unsigned)h.len);
                const uint8_t *values = p + sizeof(h);
                const size_t element_size = PaySltParamElementSize(h.type);
                const uint8_t value_count = element_size == 0 ? 0 :
                    (uint8_t)std::min<size_t>(h.len, (n - sizeof(h)) / element_size);
                for (uint8_t i = 0; i < value_count; ++i) {
                    if (h.type == PAY_SLT_PARAM_UINT8)
                        fprintf(fp, "Value[%u]=%u\n", (unsigned)i, (unsigned)values[i]);
                    else if (h.type == PAY_SLT_PARAM_INT8)
                        fprintf(fp, "Value[%u]=%d\n", (unsigned)i, (int)(int8_t)values[i]);
                    else if (h.type == PAY_SLT_PARAM_UINT16 || h.type == PAY_SLT_PARAM_INT16) {
                        uint16_t x = 0; memcpy(&x, values + 2U * i, sizeof(x));
                        if (h.type == PAY_SLT_PARAM_UINT16)
                            fprintf(fp, "Value[%u]=%u\n", (unsigned)i, (unsigned)x);
                        else
                            fprintf(fp, "Value[%u]=%d\n", (unsigned)i, (int)(int16_t)x);
                    } else if (h.type == PAY_SLT_PARAM_UINT32 || h.type == PAY_SLT_PARAM_FLOAT) {
                        uint32_t bits = 0; memcpy(&bits, values + 4U * i, sizeof(bits));
                        if (h.type == PAY_SLT_PARAM_UINT32)
                            fprintf(fp, "Value[%u]=%" PRIu32 "\n", (unsigned)i, bits);
                        else {
                            float x = 0.0f; memcpy(&x, &bits, sizeof(x));
                            fprintf(fp, "Value[%u]=%g\n", (unsigned)i, (double)x);
                        }
                    }
                }
                if (h.type == PAY_SLT_PARAM_STRING)
                    fprintf(fp, "String=%.*s\n", (int)value_count, (const char *)values);
            }
            break;
        case BEE1012_PAY_SLT_PAR_SET_CC:
            if (n == 104) {
                uint16_t addr = 0; memcpy(&addr, p + 2, sizeof(addr));
                fprintf(fp, "RejectedNode=%u\nRejectedTable=%u\nAddr=0x%04X\nType=%u\n",
                        (unsigned)p[0], (unsigned)p[1], (unsigned)addr, (unsigned)p[4]);
                if (p[4] == PAY_SLT_PARAM_STRING)
                    fprintf(fp, "String=%.*s\n", 96, (const char *)(p + 8));
                else {
                    uint32_t value = 0; memcpy(&value, p + 8, sizeof(value));
                    fprintf(fp, "Value=%" PRIu32 "\n", value);
                }
            }
            break;
        case BEE1012_PAY_SLT_SCAN_FILES_CC:
            if (n == sizeof(PAY_SLT_ScanFiles_Report_t)) {
                PAY_SLT_ScanFiles_Report_t x{}; memcpy(&x, p, sizeof(x));
                fprintf(fp, "SnapId=%" PRIu32 "\nFileCount=%" PRIu32 "\n", x.snap_id, x.file_count);
            } else if (n == 1) {
                fprintf(fp, "RejectedNode=%u\n", (unsigned)p[0]);
            }
            break;
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC:
        case BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC:
            if (n == sizeof(PAY_SLT_DownloadRequest_Report_t)) {
                PAY_SLT_DownloadRequest_Report_t x{}; memcpy(&x, p, sizeof(x));
                fprintf(fp, "FileIndex=%" PRIu32 "\nStartChunk=%" PRIu32 "\n", x.file_index, x.start_chunk);
            } else if (n == sizeof(PAY_SLT_DownloadResult_Report_t)) {
                PAY_SLT_DownloadResult_Report_t x{}; memcpy(&x, p, sizeof(x));
                x.file_path[sizeof(x.file_path) - 1] = '\0';
                fprintf(fp, "FilePath=%s\nExpectedFileSize=%" PRIu32 "\n", x.file_path, x.expected_file_size);
            }
            break;
        case BEE1012_PAY_SLT_GET_FULL_TABLE_CC:
            if (rpt->RetType == 0) {
                DumpIndexedBytes(fp, "TableData", p, n);
            } else if (n == sizeof(PAY_SLT_GetFullTableRequest_Report_t)) {
                fprintf(fp, "RejectedNode=%u\nRejectedTable=%u\n", (unsigned)p[0], (unsigned)p[1]);
            } else if (n == sizeof(int32_t)) {
                int32_t error = 0; memcpy(&error, p, sizeof(error));
                fprintf(fp, "ErrorOrRequiredSize=%" PRId32 "\n", error);
            }
            break;
        default:
            break;
    }
}


static void DumpReportPayloadParsed_ByMidCc(FILE *fp, const Report *rpt, int report_dport)
{
    uint16_t payload_len = rpt->RetValSize;
    if (payload_len > sizeof(rpt->RetVal)) payload_len = (uint16_t)sizeof(rpt->RetVal);

    fprintf(fp, "\n================= PARSED BY MID/CC =================\n");
    fprintf(fp, "ReflectedMID : 0x%04X\n", (unsigned)rpt->ReflectedMID);
    fprintf(fp, "ReflectedCC  : 0x%02X\n", (unsigned)rpt->ReflectedCC);
    fprintf(fp, "PayloadLen   : %" PRIu16 "\n", payload_len);


    const uint8_t *p = rpt->RetVal;

    if (report_dport == 15 && rpt->ReflectedMID == BEE1012_PAY_SLT_CMD_ID)
    {
        DumpPaySltReportParsed(fp, rpt);
    }
    else if (DetermineReportKind(rpt->ReflectedMID, rpt->ReflectedCC) >= REPORT_KIND_UELYSYS_TTC &&
        DetermineReportKind(rpt->ReflectedMID, rpt->ReflectedCC) <= REPORT_KIND_UELYSYS_TO_LAB)
    {
        DumpUelysysReportParsed(fp, rpt);
    }
    else if (rpt->ReflectedMID == PAYUEL_ROMA_CMD_MID || rpt->ReflectedMID == 0x3018)
    {
        ReportView_t v{};
        v.reflected_msg_id = rpt->ReflectedMID;
        v.reflected_cc = rpt->ReflectedCC;
        v.ret_code = rpt->RetCode;
        v.kind = DetermineReportKind(v.reflected_msg_id, v.reflected_cc);
        v.roma_parget_index = (v.kind == REPORT_KIND_PAYUEL_ROMA_PARGET)
                                  ? g_roma_parget_requested_index : -1;
        const bool ok = DecodeN5PayloadToView(v, p, payload_len);
        fprintf(fp, "\n[PAYUEL ROMA]\npayload_valid=%u\n", ok);
        switch (v.kind) {
            case REPORT_KIND_PAYUEL_ROMA_NOOP:
                fprintf(fp, "CmdCounter=%" PRIu8 "\nErrCounter=%" PRIu8 "\n", v.u.roma_noop.CmdCounter, v.u.roma_noop.ErrCounter); break;
            case REPORT_KIND_PAYUEL_ROMA_RESETCOUNTERS:
                fprintf(fp, "CmdCounter=%" PRIu8 "\nErrCounter=%" PRIu8 "\n", v.u.roma_resetcounters.CmdCounter, v.u.roma_resetcounters.ErrCounter); break;
            case REPORT_KIND_PAYUEL_ROMA_COMMTEST:
                DumpIndexedBytes(fp, "RxBuf", v.u.roma_commtest.RxBuf, sizeof(v.u.roma_commtest.RxBuf)); break;
            case REPORT_KIND_PAYUEL_ROMA_GETSPECIFICLINE:
                fprintf(fp, "line_no=%" PRIu16 "\ninfo=%" PRIu8 "\nlen=%" PRIu8 "\ntime_s=%" PRIu32 "\ntime_ms=%" PRIu16 "\nincr=%" PRIu16 "\nline=%.*s\n",
                        v.u.roma_getspecificline.line_no, v.u.roma_getspecificline.info, v.u.roma_getspecificline.len,
                        v.u.roma_getspecificline.time_s, v.u.roma_getspecificline.time_ms, v.u.roma_getspecificline.incr,
                        (int)sizeof(v.u.roma_getspecificline.line), v.u.roma_getspecificline.line); break;
            case REPORT_KIND_PAYUEL_ROMA_GETMULTIPLELINES:
                fprintf(fp, "offset=%" PRIu16 "\n", v.u.roma_getmultiplelines.offset);
                DumpIndexedBytes(fp, "buffer", v.u.roma_getmultiplelines.buffer, sizeof(v.u.roma_getmultiplelines.buffer)); break;
            case REPORT_KIND_PAYUEL_ROMA_GETLATESTLINE:
                fprintf(fp, "line_no=%" PRIu16 "\ninfo=%" PRIu8 "\nlen=%" PRIu8 "\ntime_s=%" PRIu32 "\ntime_ms=%" PRIu16 "\nincr=%" PRIu16 "\nline=%.*s\n",
                        v.u.roma_getlatestline.line_no, v.u.roma_getlatestline.info, v.u.roma_getlatestline.len,
                        v.u.roma_getlatestline.time_s, v.u.roma_getlatestline.time_ms, v.u.roma_getlatestline.incr,
                        (int)sizeof(v.u.roma_getlatestline.line), v.u.roma_getlatestline.line); break;
            case REPORT_KIND_PAYUEL_ROMA_GETLATESTNLINES:
                fprintf(fp, "offset=%" PRIu16 "\n", v.u.roma_getlatestNlines.offset);
                DumpIndexedBytes(fp, "buffer", v.u.roma_getlatestNlines.buffer, sizeof(v.u.roma_getlatestNlines.buffer)); break;
            case REPORT_KIND_PAYUEL_ROMA_SAVEROUTE:
                fprintf(fp, "route=%.*s\n", (int)sizeof(v.u.roma_saveroute.route), v.u.roma_saveroute.route); break;
            case REPORT_KIND_PAYUEL_ROMA_PARGET:
                fprintf(fp, "value=%" PRId32 "\nrequested_index=%" PRId8 "\n", v.u.roma_parget.value, v.roma_parget_index); break;
            case REPORT_KIND_PAYUEL_ROMA_SETROUTEDEFAULT: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_setroutedefault.result); break;
            case REPORT_KIND_PAYUEL_ROMA_RESETROUTE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_resetroute.result); break;
            case REPORT_KIND_PAYUEL_ROMA_LOADROUTE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_loadroute.result); break;
            case REPORT_KIND_PAYUEL_ROMA_SENDROUTE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_sendroute.result); break;
            case REPORT_KIND_PAYUEL_ROMA_SETROUTE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_setroute.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARSET: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_parset.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARDEFAULTS: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_pardefaults.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARSAVE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_parsave.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARRESTORE: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_parrestore.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARLOAD: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_parload.result); break;
            case REPORT_KIND_PAYUEL_ROMA_PARSETOOB: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_parsetOOB.result); break;
            case REPORT_KIND_PAYUEL_ROMA_SENDCOMMAND: fprintf(fp, "result=%" PRIu8 "\n", v.u.roma_sendcommand.result); break;
            case REPORT_KIND_PAYUEL_ROMA_SENDMSG:
            case REPORT_KIND_PAYUEL_ROMA_PAYINIT:
                fprintf(fp, "ack_payload=empty\n"); break;
            case REPORT_KIND_PAYUEL_ROMA_ISL_TEST_COUNT:
                fprintf(fp, "isl_test_count=%" PRIu32 "\n", v.u.roma_isl_count.count); break;
            case REPORT_KIND_PAYUEL_ROMA_GENERIC:
                DumpIndexedBytes(fp, "return_data", v.raw_bytes, v.raw_size); break;
            default: break;
        }
    }
    else if ((rpt->ReflectedMID == ADCS_CMD_ID) || (rpt->ReflectedMID == 0x6518))
    {
        DumpADCSReportParsed(fp, rpt);
    }
    else if ((rpt->ReflectedMID == EPS_CMD_ID) ||(rpt->ReflectedMID == 0x7518))
	    {
	        switch (rpt->ReflectedCC)
	        {
	            case EPS_P80_POWER_IF_GET_CC:
	            case EPS_P80_POWER_IF_LIST_CC:
	            case EPS_GET_HK_CC:
	            case EPS_GET_HK_ALL_CC:
	            case EPS_RPARAM_GET_CC:
	            case EPS_RPARAM_GET_FULL_TABLE_CC:
	            case EPS_CSP_PING_CC:
	            case EPS_CSP_PS_CC:
	            case EPS_CSP_MEMFREE_CC:
	            case EPS_CSP_BUF_FREE_CC:
	            case EPS_CSP_UPTIME_CC:
	                if (!DumpEPSReportParsed(fp, rpt)) DumpEPSCommandStatus(fp, rpt);
	                break;

	            case EPS_REPORT_BCN_CC: {
	                EPS_Bcn_Report_t bcn{};
	                if (rpt->RetValSize < sizeof(bcn)) break;
	                memcpy(&bcn, rpt->RetVal, sizeof(bcn));
	                fprintf(fp, "\n[EPS Beacon Report]\n");
	                fprintf(fp, "PMU.bootcause=%" PRIu32 "\nPMU.resetcause=%" PRIu16 "\nPMU.bootcount=%" PRIu16 "\n",
	                        bcn.PMU.bootcause, bcn.PMU.resetcause, bcn.PMU.bootcount);
	                DumpArr_u8(fp, "PMU.out_en", bcn.PMU.out_en, 6);
	                DumpArr_i16(fp, "PMU.temp", bcn.PMU.temp, 2);
	                fprintf(fp, "PMU.batt_mode=%" PRIu8 "\nPMU.batt_i=%" PRIi16 "\nPMU.batt_v=%" PRIu16
	                            "\nPMU.sm_en=%" PRIu8 "\nPMU.gnd_wdt_cnt=%" PRIu16 "\nPMU.bus_wdt_cnt=%" PRIu16
	                            "\nPMU.gnd_wdt_left=%" PRIu32 "\nPMU.bus_wdt_left=%" PRIu32 "\n",
	                        bcn.PMU.batt_mode, bcn.PMU.batt_i, bcn.PMU.batt_v, bcn.PMU.sm_en,
	                        bcn.PMU.gnd_wdt_cnt, bcn.PMU.bus_wdt_cnt,
	                        bcn.PMU.gnd_wdt_left, bcn.PMU.bus_wdt_left);
	                DumpArr_i16(fp, "PDU.out_i", bcn.PDU.out_i, 13);
	                DumpArr_u8(fp, "PDU.out_en", bcn.PDU.out_en, 13);
	                for (int acu = 0; acu < 2; ++acu) {
	                    char label[32];
	                    snprintf(label, sizeof(label), "ACU[%d].input_i", acu);
	                    DumpArr_i16(fp, label, bcn.ACU[acu].input_i, 6);
	                    snprintf(label, sizeof(label), "ACU[%d].input_v", acu);
	                    DumpArr_u16(fp, label, bcn.ACU[acu].input_v, 6);
	                    fprintf(fp, "ACU[%d].mppt_mode=%" PRIu8 "\n", acu, bcn.ACU[acu].mppt_mode);
	                }
	                fprintf(fp, "BP8.bootcount=%" PRIu16 "\nBP8.bootcause=%" PRIu16 "\nBP8.resetcause=%" PRIu16
	                            "\nBP8.soc=%f\nBP8.bat_avr_temp=%f\nBP8.vbat=%" PRIu16
	                            "\nBP8.current=%f\nBP8.heater_i=%" PRIu16 "\n",
	                        bcn.BP8.bootcount, bcn.BP8.bootcause, bcn.BP8.resetcause,
	                        bcn.BP8.soc, bcn.BP8.bat_avr_temp, bcn.BP8.vbat,
	                        bcn.BP8.i, bcn.BP8.heater_i);
	                break;
	            }

	            default:
	                if (!DumpEPSCommandStatus(fp, rpt))
	                    fprintf(fp, "\n[EPS] Unrecognized status payload for CC 0x%02X\n",
	                            (unsigned)rpt->ReflectedCC);
                break;
        }
    }
    else if ((rpt->ReflectedMID == GPIO_CMD_ID) || (rpt->ReflectedMID == 0x9218))
    {
        const char *cmd_name = "GPIO Unknown";
        switch (rpt->ReflectedCC)
        {
            case GPIO_DEP1_EN_ON_CC:
                cmd_name = "GPIO DEP1 EN ON";
                break;
            case GPIO_DEP1_EN_OFF_CC:
                cmd_name = "GPIO DEP1 EN OFF";
                break;
            case GPIO_DEP2_EN_ON_CC:
                cmd_name = "GPIO DEP2 EN ON";
                break;
            case GPIO_DEP2_EN_OFF_CC:
                cmd_name = "GPIO DEP2 EN OFF";
                break;
            case GPIO_SP_IN_READ_5S_CC:
                cmd_name = "GPIO SP IN READ 5S";
                break;
            default:
                break;
        }

        fprintf(fp, "\n[GPIO Report]\n");
        fprintf(fp, "Command      : %s\n", cmd_name);
        fprintf(fp, "ReturnData   : none\n");
    }
    else if ((rpt->ReflectedMID == UELYSYS_STX_CMD_ID) || (rpt->ReflectedMID == 0x5418))
    {
        fprintf(fp, "\n[UELYSYS STX Report]\n");
        switch (rpt->ReflectedCC)
        {
            case UELYSYS_STX_SET_MODULE_ID_CC:
                fprintf(fp, "Command      : STX SET MODULE ID\n");
                if (payload_len >= sizeof(uint16_t))
                {
                    uint16_t module_id = 0;
                    memcpy(&module_id, p, sizeof(module_id));
                    fprintf(fp, "ModuleId     : 0x%04X (%u)\n", module_id, module_id);
                }
                else
                {
                    fprintf(fp, "Payload      : length mismatch, expected >= 2\n");
                }
                break;

            case UELYSYS_STX_FILESYS_CC_CREATEFILE:
                fprintf(fp, "Command      : STX FILESYS CREATEFILE\n");
                if (payload_len >= 5)
                {
                    uint8_t command_status = p[0];
                    uint32_t file_handle = 0;
                    memcpy(&file_handle, p + 1, sizeof(file_handle));
                    fprintf(fp, "commad_status: %u\n", command_status);
                    fprintf(fp, "file_handle  : %" PRIu32 "\n", file_handle);
                }
                else
                {
                    fprintf(fp, "Payload      : length mismatch, expected >= 5\n");
                }
                break;

            case UELYSYS_STX_FILESYS_CC_WRITEFILE:
                fprintf(fp, "Command      : STX FILESYS WRITEFILE\n");
                if (payload_len >= 9)
                {
                    int32_t ret = 0;
                    uint32_t packet_number = 0;
                    memcpy(&ret, p, sizeof(ret));
                    memcpy(&packet_number, p + 4, sizeof(packet_number));
                    fprintf(fp, "ret          : %" PRId32 "\n", ret);
                    fprintf(fp, "packet_number: %" PRIu32 "\n", packet_number);
                    fprintf(fp, "cstatus      : %u\n", p[8]);
                }
                else
                {
                    fprintf(fp, "Payload      : length mismatch, expected >= 9\n");
                }
                break;

            case UELYSYS_STX_FILESYS_CC_SENDFILE:
                fprintf(fp, "Command      : STX FILESYS SENDFILE\n");
                if (payload_len >= 1)
                    fprintf(fp, "rxdata       : %u\n", p[0]);
                else
                    fprintf(fp, "Payload      : empty (error/no result data)\n");
                break;

            default:
                fprintf(fp, "Command      : UELYSYS STX CC 0x%02X\n", (unsigned)rpt->ReflectedCC);
                fprintf(fp, "Parser       : not specialized\n");
                break;
        }
    }
    else
    {
        fprintf(fp, "\n[UNKNOWN MID] No parser for ReflectedMID=0x%04X\n", (unsigned)rpt->ReflectedMID);
    }

    fprintf(fp, "\n[PAYLOAD HEX]\n");
    DumpHex(fp, rpt->RetVal, payload_len);
}




int BEE1012ReportSaver(Report* rpt)
{
    if (!rpt) return -1;

    ReportCounter++;

    const bool is_pay_slt = (rpt->ReflectedMID == BEE1012_PAY_SLT_CMD_ID);
    const char* report_dir = is_pay_slt ? "../data/BEE1012/PAY_SLT/report_parsed"
                                        : "../data/BEE1012/report/report_parsed";
    FILE* fp = OpenTelemetryFile(report_dir, is_pay_slt ? "PAY_SLT_Report--" : "Report--", "--.txt", NULL, "w");
    if (!fp) {
        console.AddLog("[ERROR] Failed to save BEE1012 report in %s", report_dir);
        return -2;
    }

    fprintf(fp, "================= REPORT SAVE =================\n");

    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MsgId      : 0x%04X\n", (unsigned int)rpt->CCSDS_MsgId);
    fprintf(fp, "Seq        : 0x%04X\n", (unsigned int)rpt->CCSDS_Seq);
    fprintf(fp, "Len        : 0x%04X\n", (unsigned int)rpt->CCSDS_Len);
    fprintf(fp, "TimeCode   : %02X %02X %02X %02X %02X %02X\n",
            rpt->CCSDS_TimeCode[0], rpt->CCSDS_TimeCode[1],
            rpt->CCSDS_TimeCode[2], rpt->CCSDS_TimeCode[3],
            rpt->CCSDS_TimeCode[4], rpt->CCSDS_TimeCode[5]);
    fprintf(fp, "Padding    : 0x%08X\n", (unsigned int)rpt->CCSDS_Padding);

    fprintf(fp, "\n[REPORT BODY]\n");
    fprintf(fp, "Reflected MID  : 0x%04X\n", (unsigned int)rpt->ReflectedMID);
    fprintf(fp, "Reflected CC   : 0x%02X\n", (unsigned int)rpt->ReflectedCC);
    fprintf(fp, "RetType        : 0x%02X\n", (unsigned int)rpt->RetType);
    fprintf(fp, "RetCode        : %" PRId32 "\n", (int32_t)rpt->RetCode);
    fprintf(fp, "RetValSize     : %" PRIu16 "\n", (uint16_t)rpt->RetValSize);

    DumpReportPayloadParsed_ByMidCc(fp, rpt, 15);

    fprintf(fp, "\n[RETURN VALUE]\n");

    uint16_t dump_size = rpt->RetValSize;
    if (dump_size > sizeof(rpt->RetVal))
        dump_size = sizeof(rpt->RetVal);

    for (uint16_t i = 0; i < dump_size; i++) {
        fprintf(fp, "%02X ", rpt->RetVal[i]);

        if ((i + 1) % 16 == 0)
            fprintf(fp, "\n");
    }

    if (dump_size % 16 != 0)
        fprintf(fp, "\n");

    fprintf(fp, "\n[BINARY DATA]\n");

    for (size_t i = 0; i < sizeof(*rpt); i++)
        fprintf(fp, "%02X ", ((unsigned char*)rpt)[i]);

    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}


int UELYSYSReportSaver(Report* rpt)
{
    if (!rpt) return -1;

    ReportCounter++;

    const char* report_dir = "../data/UELYSYS/report/report_parsed";
    FILE* fp = OpenTelemetryFile(report_dir, "Report--", "--.txt", NULL, "w");
    if (!fp) {
        console.AddLog("[ERROR] Failed to save UELYSYS report in %s", report_dir);
        return -2;
    }

    fprintf(fp, "================= REPORT SAVE =================\n");

    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MsgId      : 0x%04X\n", (unsigned int)rpt->CCSDS_MsgId);
    fprintf(fp, "Seq        : 0x%04X\n", (unsigned int)rpt->CCSDS_Seq);
    fprintf(fp, "Len        : 0x%04X\n", (unsigned int)rpt->CCSDS_Len);
    fprintf(fp, "TimeCode   : %02X %02X %02X %02X %02X %02X\n",
            rpt->CCSDS_TimeCode[0], rpt->CCSDS_TimeCode[1],
            rpt->CCSDS_TimeCode[2], rpt->CCSDS_TimeCode[3],
            rpt->CCSDS_TimeCode[4], rpt->CCSDS_TimeCode[5]);
    fprintf(fp, "Padding    : 0x%08X\n", (unsigned int)rpt->CCSDS_Padding);

    fprintf(fp, "\n[REPORT BODY]\n");
    fprintf(fp, "Reflected MID  : 0x%04X\n", (unsigned int)rpt->ReflectedMID);
    fprintf(fp, "Reflected CC   : 0x%02X\n", (unsigned int)rpt->ReflectedCC);
    fprintf(fp, "RetType        : 0x%02X\n", (unsigned int)rpt->RetType);
    fprintf(fp, "RetCode        : %" PRId32 "\n", (int32_t)rpt->RetCode);
    fprintf(fp, "RetValSize     : %" PRIu16 "\n", (uint16_t)rpt->RetValSize);

    DumpReportPayloadParsed_ByMidCc(fp, rpt, 10);

    fprintf(fp, "\n[RETURN VALUE]\n");

    uint16_t dump_size = rpt->RetValSize;
    if (dump_size > sizeof(rpt->RetVal))
        dump_size = sizeof(rpt->RetVal);

    for (uint16_t i = 0; i < dump_size; i++) {
        fprintf(fp, "%02X ", rpt->RetVal[i]);

        if ((i + 1) % 16 == 0)
            fprintf(fp, "\n");
    }

    if (dump_size % 16 != 0)
        fprintf(fp, "\n");

    fprintf(fp, "\n[BINARY DATA]\n");

    for (size_t i = 0; i < sizeof(*rpt); i++)
        fprintf(fp, "%02X ", ((unsigned char*)rpt)[i]);

    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}
