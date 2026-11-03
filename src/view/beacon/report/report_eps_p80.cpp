// View - Beacon 창 "Report" 탭: EPS(P80/BP8) 질의/HK 리포트와 EPS COMMAND STATUS
//
// GS_nuri5 miman_imgui.cpp Report 탭의 EPS_QUERY_RAW ~ EPS_BP8_HK, EPS_COMMAND_STATUS case 를 옮긴 것 (행 이름/형식 그대로).
// EPS P60(PDU/ACU/DOCK) 테이블 HK 는 BEE-1000 과 같은 report_eps.cpp 를 쓴다.
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawEpsP80Report(ReportCtx & c)
{
    ReportView_t & view = c.view;
    const EPS_Report_State_t & eps_state = c.eps_state;
    const EPS_RParam_Reassembly_State_t & eps_rparam = c.eps_rparam;
    bool & show_hexdump = c.show_hexdump;
    (void)eps_state; (void)eps_rparam; (void)show_hexdump;
    auto RowEPSHKAllSummary = [&]() { rpt5::RowEPSHKAllSummary(eps_state); };
    (void)RowEPSHKAllSummary;

    switch (view.kind)
    {
    case REPORT_KIND_EPS_QUERY_RAW:
    {
        RowText("Type", "EPS Query Result");
        RowEPSMeta(view.eps_meta);
        HeaderRow("[Query Data]");
        if (view.eps_meta.data_id == EPS_QUERY_REPORT_CSP_PING_MS &&
            view.u.uelysys.size >= sizeof(int32_t)) {
            int32_t elapsed_ms = 0;
            memcpy(&elapsed_ms, view.u.uelysys.bytes, sizeof(elapsed_ms));
            RowFmt("elapsed_ms", "%" PRIi32, elapsed_ms);
        }
        if (view.reflected_cc == EPS_RPARAM_GET_FULL_TABLE_CC) {
            HeaderRow("[CC42 Reassembly]");
            RowText("active", eps_rparam.active ? "true" : "false");
            RowText("valid", eps_rparam.valid ? "true" : "false");
            RowText("complete", eps_rparam.complete ? "true" : "false");
            RowText("error", eps_rparam.error ? "true" : "false");
            RowText("duplicate_seen", eps_rparam.duplicate_seen ? "true" : "false");
            RowText("missing_sequence_seen", eps_rparam.missing_sequence_seen ? "true" : "false");
            RowFmt("expected_sequence", "%" PRIu16, eps_rparam.expected_sequence);
            RowFmt("last_sequence", "%" PRIu16, eps_rparam.last_sequence);
            RowFmt("total_size", "%" PRIu16, eps_rparam.total_size);
            RowFmt("received_size", "%" PRIu16, eps_rparam.received_size);
            RowFmt("last_offset", "%" PRIu16, eps_rparam.last_offset);
            RowFmt("last_chunk_size", "%" PRIu16, eps_rparam.last_chunk_size);
            const float progress = eps_rparam.total_size
                ? (float)eps_rparam.received_size / eps_rparam.total_size : 0.0f;
            RowFmt("progress", "%.1f%%", progress * 100.0f);
            if (eps_rparam.complete)
                RowArr_Hex("reassembled_data", eps_rparam.buffer, eps_rparam.total_size, 16);
        }
        for (uint16_t i = 0; i < view.u.uelysys.size && i < sizeof(view.u.uelysys.bytes); ++i) {
            char label[32];
            snprintf(label, sizeof(label), "data[%" PRIu16 "]", i);
            RowFmt(label, "0x%02X (%" PRIu8 ")",
                   view.u.uelysys.bytes[i], view.u.uelysys.bytes[i]);
        }
        break;
    }

    case REPORT_KIND_EPS_BCN_REPORT:
    {
        const auto &bcn = view.u.eps_bcn;
        RowText("Type", "EPS Beacon Report");
        HeaderRow("[PMU]");
        RowFmt("bootcause", "%" PRIu32, bcn.PMU.bootcause);
        RowFmt("resetcause", "%" PRIu16, bcn.PMU.resetcause);
        RowFmt("bootcount", "%" PRIu16, bcn.PMU.bootcount);
        RowArr_u8("out_en", bcn.PMU.out_en, 6, 6);
        RowArr_i16("temp", bcn.PMU.temp, 2, 2);
        RowFmt("batt_mode", "%" PRIu8, bcn.PMU.batt_mode);
        RowFmt("batt_i", "%" PRIi16, bcn.PMU.batt_i);
        RowFmt("batt_v", "%" PRIu16, bcn.PMU.batt_v);
        RowFmt("sm_en", "%" PRIu8, bcn.PMU.sm_en);
        RowFmt("gnd_wdt_cnt", "%" PRIu16, bcn.PMU.gnd_wdt_cnt);
        RowFmt("bus_wdt_cnt", "%" PRIu16, bcn.PMU.bus_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, bcn.PMU.gnd_wdt_left);
        RowFmt("bus_wdt_left", "%" PRIu32, bcn.PMU.bus_wdt_left);
        HeaderRow("[PDU]");
        RowArr_i16("out_i", bcn.PDU.out_i, 12, 12);
        RowArr_u8("out_en", bcn.PDU.out_en, 12, 12);
        for (int acu = 0; acu < 2; ++acu) {
            char label[32];
            snprintf(label, sizeof(label), "[ACU %d]", acu + 1);
            HeaderRow(label);
            RowArr_i16("input_i", bcn.ACU[acu].input_i, 6, 6);
            RowArr_u16("input_v", bcn.ACU[acu].input_v, 6, 6);
            RowFmt("mppt_mode", "%" PRIu8, bcn.ACU[acu].mppt_mode);
        }
        HeaderRow("[BP8]");
        RowFmt("bootcount", "%" PRIu16, bcn.BP8.bootcount);
        RowFmt("bootcause", "%" PRIu16, bcn.BP8.bootcause);
        RowFmt("resetcause", "%" PRIu16, bcn.BP8.resetcause);
        RowFmt("soc", "%f", bcn.BP8.soc);
        RowFmt("bat_avr_temp", "%f", bcn.BP8.bat_avr_temp);
        RowFmt("vbat", "%" PRIu16, bcn.BP8.vbat);
        RowFmt("current", "%f", bcn.BP8.i);
        RowFmt("heater_i", "%" PRIu16, bcn.BP8.heater_i);
        break;
    }

    case REPORT_KIND_EPS_POWER_IF_STATUS:
    {
        auto &pl = view.u.eps_power_if_status;
        char name[9];
        CopyName8(name, pl.name);
        RowText("Type", "EPS P80 Power IF Status");
        RowEPSMeta(view.eps_meta);
        HeaderRow("[Report Data]");
        RowFmt("ch_idx", "%" PRIu8, pl.ch_idx);
        RowFmt("mode", "%" PRIu8, pl.mode);
        RowFmt("on_cnt", "%" PRIu16, pl.on_cnt);
        RowFmt("off_cnt", "%" PRIu16, pl.off_cnt);
        RowFmt("cur_lu_lim", "%" PRIu16, pl.cur_lu_lim);
        RowFmt("cur_lim", "%" PRIu16, pl.cur_lim);
        RowFmt("voltage", "%" PRIu16, pl.voltage);
        RowFmt("current", "%" PRIi16, pl.current);
        RowFmt("latchup", "%" PRIu16, pl.latchup);
        RowText("name", name);
        break;
    }

    case REPORT_KIND_EPS_POWER_IF_LIST:
    {
        auto &pl = view.u.eps_power_if_list;
        uint8_t count = (pl.count > 24) ? 24 : pl.count;
        RowText("Type", "EPS P80 Power IF List");
        RowEPSMeta(view.eps_meta);
        HeaderRow("[Report Data]");
        RowFmt("cmd", "%" PRIu8, pl.cmd);
        RowFmt("status", "%" PRIu8, pl.status);
        RowFmt("count", "%" PRIu8, pl.count);
        for (uint8_t i = 0; i < count; i++) {
            char label[32];
            char value[96];
            char name[9];
            CopyName8(name, pl.list[i].name);
            snprintf(label, sizeof(label), "list[%u]", (unsigned)i);
            snprintf(value, sizeof(value), "ch_idx=%" PRIu8 " mode=%" PRIu8 " name=%s",
                     pl.list[i].ch_idx, pl.list[i].mode, name);
            RowText(label, value);
        }
        break;
    }

        case REPORT_KIND_EPS_HK_ALL :
        {
            auto &pl = view.u.eps_hk_all;
            RowText("Type", "EPS HK ALL");
            RowEPSMeta(view.eps_meta);

            RowEPSHKAllSummary();
        HeaderRow("[HK ALL Status]");
        RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
        RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
        RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
        RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
        RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");

            RowFmt("valid_mask", "%" PRIu8, pl.valid_mask);
            RowFmt("node count", "%" PRIu8, pl.node_count);
            RowFmt("fail_count", "%" PRIu8, pl.fail_count);
            RowFmt("status 0", "%" PRIu8, pl.status[0]);
            RowFmt("status 1", "%" PRIu8, pl.status[1]);
            RowFmt("status 2", "%" PRIu8, pl.status[2]);
            RowFmt("status 3", "%" PRIu8, pl.status[3]);
            RowFmt("status 4", "%" PRIu8, pl.status[4]);

            HeaderRow("[PMU]");
        RowFmt("uptime", "%" PRIu32, pl.PMU.uptime);
        RowFmt("bootcause", "%" PRIu32, pl.PMU.bootcause);
        RowFmt("resetcause", "%" PRIu16, pl.PMU.resetcause);
        RowFmt("bootcount", "%" PRIu16, pl.PMU.bootcount);
        RowFmt("batt_v", "%" PRIu16, pl.PMU.batt_v);
        RowFmt("batt_i", "%" PRIi16, pl.PMU.batt_i);
        RowFmt("batt_mode", "%" PRIu8, pl.PMU.batt_mode);
        RowFmt("vbat_v", "%" PRIu16, pl.PMU.vbat_v);
        RowFmt("vcc_v", "%" PRIu16, pl.PMU.vcc_v);
        RowArr_i16("temp[0..1]", pl.PMU.temp, 2, 0);
        RowArr_u8("out_en[0..5]", pl.PMU.out_en, 6, 6);
        RowArr_i16("out_i[0..5]", pl.PMU.out_i, 6, 6);
        RowArr_u8("sm_en[0..7]", pl.PMU.sm_en, 8, 8);
        RowFmt("gnd_wdt_cnt", "%" PRIu16, pl.PMU.gnd_wdt_cnt);
        RowFmt("bus_wdt_cnt", "%" PRIu16, pl.PMU.bus_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.PMU.gnd_wdt_left);
        RowFmt("bus_wdt_left", "%" PRIu32, pl.PMU.bus_wdt_left);

            HeaderRow("[PDU]");
        RowFmt("uptime", "%" PRIu32, pl.PDU.uptime);
        RowFmt("bootcause", "%" PRIu32, pl.PDU.bootcause);
        RowFmt("bootcount", "%" PRIu32, pl.PDU.bootcount);
        RowFmt("resetcause", "%" PRIu16, pl.PDU.resetcause);
        RowFmt("vcc_v", "%" PRIu16, pl.PDU.vcc_v);
        RowFmt("vcc_i", "%" PRIu16, pl.PDU.vcc_i);
        RowFmt("vbat_v", "%" PRIu16, pl.PDU.vbat_v);
        RowFmt("temp", "%" PRIi16, pl.PDU.temp);
        RowFmt("batt_mode", "%" PRIu8, pl.PDU.batt_mode);
        RowArr_u8("out_en[0..23]", pl.PDU.out_en, 24, 12);
        RowArr_i16("out_i[0..23]", pl.PDU.out_i, 24, 12);
        RowFmt("gnd_wdt_cnt", "%" PRIu32, pl.PDU.gnd_wdt_cnt);
        RowFmt("bus_wdt_cnt", "%" PRIu32, pl.PDU.bus_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.PDU.gnd_wdt_left);
        RowFmt("bus_wdt_left", "%" PRIu32, pl.PDU.bus_wdt_left);

            HeaderRow("[ACU 0]");
        RowFmt("uptime", "%" PRIu32, pl.ACU[0].uptime);
        RowFmt("bootcause", "%" PRIu32, pl.ACU[0].bootcause);
        RowFmt("bootcount", "%" PRIu32, pl.ACU[0].bootcount);
        RowFmt("resetcause", "%" PRIu16, pl.ACU[0].resetcause);
        RowArr_i16("input_i[0..5]", pl.ACU[0].input_i, 6, 6);
        RowArr_u16("input_v[0..5]", pl.ACU[0].input_v, 6, 6);
        RowFmt("vcc_v", "%" PRIu16, pl.ACU[0].vcc_v);
        RowFmt("vbat_v", "%" PRIu16, pl.ACU[0].vbat_v);
        RowArr_i16("temp[0..2]", pl.ACU[0].temp, 3, 0);
        RowFmt("mppt_mode", "%" PRIu8, pl.ACU[0].mppt_mode);
        RowFmt("gnd_wdt_cnt", "%" PRIu32, pl.ACU[0].gnd_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.ACU[0].gnd_wdt_left);

            HeaderRow("[ACU 1]");
        RowFmt("uptime", "%" PRIu32, pl.ACU[1].uptime);
        RowFmt("bootcause", "%" PRIu32, pl.ACU[1].bootcause);
        RowFmt("bootcount", "%" PRIu32, pl.ACU[1].bootcount);
        RowFmt("resetcause", "%" PRIu16, pl.ACU[1].resetcause);
        RowArr_i16("input_i[0..5]", pl.ACU[1].input_i, 6, 6);
        RowArr_u16("input_v[0..5]", pl.ACU[1].input_v, 6, 6);
        RowFmt("vcc_v", "%" PRIu16, pl.ACU[1].vcc_v);
        RowFmt("vbat_v", "%" PRIu16, pl.ACU[1].vbat_v);
        RowArr_i16("temp[0..2]", pl.ACU[1].temp, 3, 0);
        RowFmt("mppt_mode", "%" PRIu8, pl.ACU[1].mppt_mode);
        RowFmt("gnd_wdt_cnt", "%" PRIu32, pl.ACU[1].gnd_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.ACU[1].gnd_wdt_left);

            HeaderRow("[BP8]");
        RowFmt("Uptime", "%" PRIu32, pl.BP8.Uptime);
        RowFmt("BootCount", "%" PRIu16, pl.BP8.BootCount);
        RowFmt("BootCause", "%" PRIu16, pl.BP8.BootCause);
        RowFmt("ResetCause", "%" PRIu16, pl.BP8.ResetCause);
        RowFmt("Vbat", "%" PRIu16, pl.BP8.Vbat);
        RowFmt("Soc", "%f", pl.BP8.Soc);
        RowFmt("Current", "%f", pl.BP8.Current);
        RowFmt("InCurrent", "%" PRIu16, pl.BP8.InCurrent);
        RowFmt("OutCurrent", "%" PRIu16, pl.BP8.OutCurrent);
        RowFmt("HeaterCurrent", "%" PRIu16, pl.BP8.HeaterCurrent);
        RowFmt("IntTemp", "%" PRIi16, pl.BP8.IntTemp);
        RowFmt("BatAvrTemp", "%f", pl.BP8.BatAvrTemp);
        RowArr_i16("BatTemp[0..3]", pl.BP8.BatTemp, 4, 4);
        RowFmt("OVoltCount", "%" PRIu16, pl.BP8.OVoltCount);
        RowFmt("BatFault", "%" PRIu8, pl.BP8.BatFault);

            break;
        }

    case REPORT_KIND_EPS_P80_PMU_HK:
    {
        auto &pl = view.u.eps_p80_pmu_hk;
        RowText("Type", "EPS P80 PMU HK");
        RowEPSMeta(view.eps_meta);
        RowEPSHKAllSummary();
        HeaderRow("[HK ALL Status]");
        RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
        RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
        RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
        RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
        RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");
        HeaderRow("[PMU]");
        RowFmt("uptime", "%" PRIu32, pl.uptime);
        RowFmt("bootcause", "%" PRIu32, pl.bootcause);
        RowFmt("resetcause", "%" PRIu16, pl.resetcause);
        RowFmt("bootcount", "%" PRIu16, pl.bootcount);
        RowFmt("batt_v", "%" PRIu16, pl.batt_v);
        RowFmt("batt_i", "%" PRIi16, pl.batt_i);
        RowFmt("batt_mode", "%" PRIu8, pl.batt_mode);
        RowFmt("vbat_v", "%" PRIu16, pl.vbat_v);
        RowFmt("vcc_v", "%" PRIu16, pl.vcc_v);
        RowArr_i16("temp[0..1]", pl.temp, 2, 0);
        RowArr_u8("out_en[0..5]", pl.out_en, 6, 6);
        RowArr_i16("out_i[0..5]", pl.out_i, 6, 6);
        RowArr_u8("sm_en[0..7]", pl.sm_en, 8, 8);
        RowFmt("gnd_wdt_cnt", "%" PRIu16, pl.gnd_wdt_cnt);
        RowFmt("bus_wdt_cnt", "%" PRIu16, pl.bus_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.gnd_wdt_left);
        RowFmt("bus_wdt_left", "%" PRIu32, pl.bus_wdt_left);
        break;
    }

    case REPORT_KIND_EPS_P80_PDU_HK:
    {
        auto &pl = view.u.eps_p80_pdu_hk;
        RowText("Type", "EPS P80 PDU HK");
        RowEPSMeta(view.eps_meta);
        RowEPSHKAllSummary();
        HeaderRow("[HK ALL Status]");
        RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
        RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
        RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
        RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
        RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");
        HeaderRow("[PDU]");
        RowFmt("uptime", "%" PRIu32, pl.uptime);
        RowFmt("bootcause", "%" PRIu32, pl.bootcause);
        RowFmt("bootcount", "%" PRIu32, pl.bootcount);
        RowFmt("resetcause", "%" PRIu16, pl.resetcause);
        RowFmt("vcc_v", "%" PRIu16, pl.vcc_v);
        RowFmt("vcc_i", "%" PRIu16, pl.vcc_i);
        RowFmt("vbat_v", "%" PRIu16, pl.vbat_v);
        RowFmt("temp", "%" PRIi16, pl.temp);
        RowFmt("batt_mode", "%" PRIu8, pl.batt_mode);
        RowArr_u8("out_en[0..23]", pl.out_en, 24, 12);
        RowArr_i16("out_i[0..23]", pl.out_i, 24, 12);
        RowFmt("gnd_wdt_cnt", "%" PRIu32, pl.gnd_wdt_cnt);
        RowFmt("bus_wdt_cnt", "%" PRIu32, pl.bus_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.gnd_wdt_left);
        RowFmt("bus_wdt_left", "%" PRIu32, pl.bus_wdt_left);
        break;
    }

    case REPORT_KIND_EPS_P80_ACU1_HK:
    case REPORT_KIND_EPS_P80_ACU2_HK:
    {
        auto &pl = view.u.eps_p80_acu_hk;
        RowText("Type", (view.kind == REPORT_KIND_EPS_P80_ACU1_HK) ? "EPS P80 ACU1 HK" : "EPS P80 ACU2 HK");
        RowEPSMeta(view.eps_meta);
        RowEPSHKAllSummary();
        HeaderRow("[HK ALL Status]");
        RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
        RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
        RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
        RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
        RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");
        HeaderRow("[ACU]");
        RowFmt("uptime", "%" PRIu32, pl.uptime);
        RowFmt("bootcause", "%" PRIu32, pl.bootcause);
        RowFmt("bootcount", "%" PRIu32, pl.bootcount);
        RowFmt("resetcause", "%" PRIu16, pl.resetcause);
        RowArr_i16("input_i[0..5]", pl.input_i, 6, 6);
        RowArr_u16("input_v[0..5]", pl.input_v, 6, 6);
        RowFmt("vcc_v", "%" PRIu16, pl.vcc_v);
        RowFmt("vbat_v", "%" PRIu16, pl.vbat_v);
        RowArr_i16("temp[0..2]", pl.temp, 3, 0);
        RowFmt("mppt_mode", "%" PRIu8, pl.mppt_mode);
        RowFmt("gnd_wdt_cnt", "%" PRIu32, pl.gnd_wdt_cnt);
        RowFmt("gnd_wdt_left", "%" PRIu32, pl.gnd_wdt_left);
        break;
    }

    case REPORT_KIND_EPS_BP8_HK:
    {
        auto &pl = view.u.eps_bp8_hk;
        RowText("Type", "EPS BP8 HK");
        RowEPSMeta(view.eps_meta);
        RowEPSHKAllSummary();
        HeaderRow("[HK ALL Status]");
        RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
        RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
        RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
        RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
        RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");
        HeaderRow("[BP8]");
        RowFmt("Uptime", "%" PRIu32, pl.Uptime);
        RowFmt("BootCount", "%" PRIu16, pl.BootCount);
        RowFmt("BootCause", "%" PRIu16, pl.BootCause);
        RowFmt("ResetCause", "%" PRIu16, pl.ResetCause);
        RowFmt("Vbat", "%" PRIu16, pl.Vbat);
        RowFmt("Soc", "%f", pl.Soc);
        RowFmt("Current", "%f", pl.Current);
        RowFmt("InCurrent", "%" PRIu16, pl.InCurrent);
        RowFmt("OutCurrent", "%" PRIu16, pl.OutCurrent);
        RowFmt("HeaterCurrent", "%" PRIu16, pl.HeaterCurrent);
        RowFmt("IntTemp", "%" PRIi16, pl.IntTemp);
        RowFmt("BatAvrTemp", "%f", pl.BatAvrTemp);
        RowArr_i16("BatTemp[0..3]", pl.BatTemp, 4, 4);
        RowFmt("OVoltCount", "%" PRIu16, pl.OVoltCount);
        RowFmt("BatFault", "%" PRIu8, pl.BatFault);
        break;
    }

    case REPORT_KIND_EPS_COMMAND_STATUS:
    {
        RowText("Type", "EPS command status / echoed command payload");
        RowFmt("Command Code", "%u", (unsigned)view.reflected_cc);
        RowFmt("Return Code", "%d", (int)view.ret_code);
        RowFmt("Payload Size", "%u", (unsigned)view.raw_size);
        auto CopyStatus = [&](void *dst, size_t size) {
            if (view.raw_size != size) return false;
            memcpy(dst, view.raw_bytes, size);
            return true;
        };
        switch (view.reflected_cc) {
            case EPS_NOOP_CC:
            case EPS_RESET_COUNTERS_CC: {
                uint32_t counters[4]{};
                if (CopyStatus(counters, sizeof(counters))) {
                    RowFmt("Command Counter", "%u", (unsigned)counters[0]);
                    RowFmt("Error Counter", "%u", (unsigned)counters[1]);
                    RowFmt("Get HK Error Counter", "%u", (unsigned)counters[2]);
                    RowFmt("Get Beacon Error Counter", "%u", (unsigned)counters[3]);
                }
                break;
            }
            case EPS_P80_POWER_IF_GET_CC: {
                EPS_P80_Power_If_Get_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Name", "%.*s", (int)sizeof(p.name), p.name);
                }
                break;
            }
            case EPS_P80_POWER_IF_SET_CC: {
                EPS_P80_Power_If_Set_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Mode", "%u", (unsigned)p.mode);
                    RowFmt("On Count", "%u", (unsigned)p.on_cnt);
                    RowFmt("Off Count", "%u", (unsigned)p.off_cnt);
                    RowFmt("Name", "%.*s", (int)sizeof(p.name), p.name);
                }
                break;
            }
            case EPS_RPARAM_GET_CC: {
                EPS_RParam_Get_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Table ID", "%u", (unsigned)p.table_id);
                    RowFmt("Address", "%u", (unsigned)p.addr);
                    RowFmt("Type", "%u", (unsigned)p.type);
                    RowFmt("Size", "%u", (unsigned)p.size);
                }
                break;
            }
            case EPS_RPARAM_SET_CC: {
                EPS_RParam_Set_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Table ID", "%u", (unsigned)p.table_id);
                    RowFmt("Address", "%u", (unsigned)p.addr);
                    RowFmt("Type", "%u", (unsigned)p.type);
                    RowFmt("Size", "%u", (unsigned)p.size);
                    RowArr_Hex("Data", p.data,
                               p.size > sizeof(p.data) ? sizeof(p.data) : p.size, 16);
                }
                break;
            }
            case EPS_RPARAM_SAVE_TO_STORE_CC:
            case EPS_RPARAM_LOAD_FROM_STORE_CC: {
                EPS_RParam_Store_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Table ID", "%u", (unsigned)p.table_id);
                    RowFmt("Store", "%.*s", (int)sizeof(p.store), p.store);
                    RowFmt("Slot", "%.*s", (int)sizeof(p.slot), p.slot);
                }
                break;
            }
            case EPS_CSP_PING_CC: {
                EPS_CSP_Ping_Cmd_Payload_t p{};
                if (CopyStatus(&p, sizeof(p))) {
                    RowFmt("CSP Node", "%u", (unsigned)p.csp_node);
                    RowFmt("Size", "%u", (unsigned)p.size);
                    RowFmt("Options", "0x%02X", (unsigned)p.opts);
                }
                break;
            }
            case EPS_GET_HK_ALL_CC:
            case EPS_P80_GND_WDT_CLEAR_ALL_CC: {
                if (view.raw_size == 5) {
                    int32_t status = 0;
                    memcpy(&status, view.raw_bytes + 1, sizeof(status));
                    RowFmt("CSP Node", "%u", (unsigned)view.raw_bytes[0]);
                    RowFmt("Node Status", "%d", (int)status);
                }
                break;
            }
            case EPS_RPARAM_GET_FULL_TABLE_CC:
            case EPS_RPARAM_TABLE_SAVE_CC:
            case EPS_RPARAM_TABLE_LOAD_CC:
                if (view.raw_size == 2) {
                    RowFmt("CSP Node", "%u", (unsigned)view.raw_bytes[0]);
                    RowFmt("Table ID", "%u", (unsigned)view.raw_bytes[1]);
                }
                break;
            default:
                if (view.raw_size == 1)
                    RowFmt("CSP Node", "%u", (unsigned)view.raw_bytes[0]);
                break;
        }
        if (!view.payload_valid && view.raw_size > 0)
            RowArr_Hex("Unexpected Payload", view.raw_bytes, view.raw_size, 16);
        break;
    }

    default:
        return false;
    }
    return true;
}

// EPS HK ALL: 장치별 마지막으로 받은 HK 의 주요 값 (report_rows.h)
void rpt5::RowEPSHKAllSummary(const EPS_Report_State_t & eps_state)
{
    char value[256];
    HeaderRow("[EPS HK ALL Last Values]");
    RowText("PMU", eps_state.pmu_hk_valid ? "Received" : "No data");
    if (eps_state.pmu_hk_valid) {
        snprintf(value, sizeof(value), "bootcount=%" PRIu16 " batt_v=%" PRIu16 " batt_i=%" PRIi16 " batt_mode=%" PRIu8,
                    eps_state.pmu_hk.bootcount, eps_state.pmu_hk.batt_v,
                    eps_state.pmu_hk.batt_i, eps_state.pmu_hk.batt_mode);
        RowText("PMU key", value);
    }

    RowText("PDU", eps_state.pdu_hk_valid ? "Received" : "No data");
    if (eps_state.pdu_hk_valid) {
        snprintf(value, sizeof(value), "bootcount=%" PRIu32 " vcc_v=%" PRIu16 " vbat_v=%" PRIu16 " batt_mode=%" PRIu8,
                    eps_state.pdu_hk.bootcount, eps_state.pdu_hk.vcc_v,
                    eps_state.pdu_hk.vbat_v, eps_state.pdu_hk.batt_mode);
        RowText("PDU key", value);
    }

    RowText("ACU1", eps_state.acu1_hk_valid ? "Received" : "No data");
    if (eps_state.acu1_hk_valid) {
        snprintf(value, sizeof(value), "bootcount=%" PRIu32 " vcc_v=%" PRIu16 " vbat_v=%" PRIu16 " mppt_mode=%" PRIu8,
                    eps_state.acu1_hk.bootcount, eps_state.acu1_hk.vcc_v,
                    eps_state.acu1_hk.vbat_v, eps_state.acu1_hk.mppt_mode);
        RowText("ACU1 key", value);
    }

    RowText("ACU2", eps_state.acu2_hk_valid ? "Received" : "No data");
    if (eps_state.acu2_hk_valid) {
        snprintf(value, sizeof(value), "bootcount=%" PRIu32 " vcc_v=%" PRIu16 " vbat_v=%" PRIu16 " mppt_mode=%" PRIu8,
                    eps_state.acu2_hk.bootcount, eps_state.acu2_hk.vcc_v,
                    eps_state.acu2_hk.vbat_v, eps_state.acu2_hk.mppt_mode);
        RowText("ACU2 key", value);
    }

    RowText("BP8", eps_state.bp8_hk_valid ? "Received" : "No data");
    if (eps_state.bp8_hk_valid) {
        snprintf(value, sizeof(value), "BootCount=%" PRIu16 " Vbat=%" PRIu16 " Soc=%f Current=%f",
                    eps_state.bp8_hk.BootCount, eps_state.bp8_hk.Vbat,
                    eps_state.bp8_hk.Soc, eps_state.bp8_hk.Current);
        RowText("BP8 key", value);
    }
}
