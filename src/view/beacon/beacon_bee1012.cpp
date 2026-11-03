// View - Beacon 창 "Beacon" 탭: BEE1012 위성 비콘 표 (위성 모드가 BEE1012 일 때)
//
// GS_nuri5 miman_imgui.cpp 의 ImGui_BEE1012BeaconWindow 를 옮긴 것 (행 이름/형식 그대로).
// 표(##BeaconTables) 안에서 불린다. b 가 NULL 이면 "No data" 한 줄.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "imgui.h"
#include "beacon_tabs.h"

void DrawBEE1012Beacon(const BEE1012_Beacon_t * b)
{
    if (!b) {
        StateWindowColumnManager("BEE1012 Beacon");
        ImGui::Text("No data");
        return;
    }

    char call_sign_buf[8] = {0};
    memcpy(call_sign_buf, b->header.call_sign, 7);

    BeaconSectionHeader("BEE1012 Header");
    StateWindowColumnManager("Call Sign"); ImGui::Text("%s", call_sign_buf);
    BeaconRawBytesRow("Message ID", b->header.msg_id, 2);
    BeaconRawBytesRow("Sequence", b->header.sequence, 2);
    BeaconRawBytesRow("Length", b->header.length, 2);
    BeaconRawBytesRow("Time Code", b->header.time_code, 6);

    BeaconSectionHeader("RPT");
    StateWindowColumnManager("boot_count"); ImGui::Text("%" PRIu16, b->rpt.boot_count);
    StateWindowColumnManager("sequence"); ImGui::Text("%" PRIu32, b->rpt.sequence);
    StateWindowColumnManager("reset_cause"); ImGui::Text("%" PRIu8, b->rpt.reset_cause);

    BeaconSectionHeader("COMS");
    StateWindowColumnManager("AX100 active_conf"); ImGui::Text("%" PRIu8, b->coms.ax100.active_conf);
    StateWindowColumnManager("AX100 boot_count"); ImGui::Text("%" PRIu16, b->coms.ax100.boot_count);
    StateWindowColumnManager("AX100 boot_cause"); ImGui::Text("%" PRIu32, b->coms.ax100.boot_cause);
    StateWindowColumnManager("AX100 temp_board"); ImGui::Text("%" PRId16, b->coms.ax100.temp_board);
    StateWindowColumnManager("LTRX temp"); ImGui::Text("%" PRId16, b->coms.ltrx.temp);
    StateWindowColumnManager("LTRX connection_quality"); ImGui::Text("%" PRId8, b->coms.ltrx.connection_quality);
    StateWindowColumnManager("LTRX battery_capacity"); ImGui::Text("%" PRIu16, b->coms.ltrx.battery_capacity);

    BeaconSectionHeader("EPS - PMU");
    StateWindowColumnManager("bootcause"); ImGui::Text("%" PRIu32, b->eps.pmu.bootcause);
    StateWindowColumnManager("resetcause"); ImGui::Text("%" PRIu16, b->eps.pmu.resetcause);
    StateWindowColumnManager("bootcount"); ImGui::Text("%" PRIu16, b->eps.pmu.bootcount);
    BeaconU8ArrayRow("out_en[6]", b->eps.pmu.out_en, 6);
    BeaconI16ArrayRow("temp[2]", b->eps.pmu.temp, 2);
    StateWindowColumnManager("batt_mode"); ImGui::Text("%" PRIu8, b->eps.pmu.batt_mode);
    StateWindowColumnManager("batt_i"); ImGui::Text("%" PRId16, b->eps.pmu.batt_i);
    StateWindowColumnManager("batt_v"); ImGui::Text("%" PRIu16, b->eps.pmu.batt_v);
    StateWindowColumnManager("sm_en"); ImGui::Text("%" PRIu8, b->eps.pmu.sm_en);
    StateWindowColumnManager("gnd_wdt_cnt"); ImGui::Text("%" PRIu16, b->eps.pmu.gnd_wdt_cnt);
    StateWindowColumnManager("bus_wdt_cnt"); ImGui::Text("%" PRIu16, b->eps.pmu.bus_wdt_cnt);
    StateWindowColumnManager("gnd_wdt_left"); ImGui::Text("%" PRIu32, b->eps.pmu.gnd_wdt_left);
    StateWindowColumnManager("bus_wdt_left"); ImGui::Text("%" PRIu32, b->eps.pmu.bus_wdt_left);

    BeaconSectionHeader("EPS - PDU");
    BeaconI16ArrayRow("out_i[13]", b->eps.pdu.out_i, 13);
    BeaconU8ArrayRow("out_en[13]", b->eps.pdu.out_en, 13);

    BeaconSectionHeader("EPS - ACU");
    for (int unit = 0; unit < 2; unit++) {
        char label[32];
        snprintf(label, sizeof(label), "ACU %d input_i[6]", unit);
        BeaconI16ArrayRow(label, b->eps.acu.acu[unit].input_i, 6);
        snprintf(label, sizeof(label), "ACU %d input_v[6]", unit);
        BeaconU16ArrayRow(label, b->eps.acu.acu[unit].input_v, 6);
        snprintf(label, sizeof(label), "ACU %d mppt_mode", unit);
        StateWindowColumnManager(label); ImGui::Text("%" PRIu8, b->eps.acu.acu[unit].mppt_mode);
    }

    BeaconSectionHeader("EPS - BP8 / GPIO");
    StateWindowColumnManager("BP8 bootcount"); ImGui::Text("%" PRIu16, b->eps.bp8.bootcount);
    StateWindowColumnManager("BP8 bootcause"); ImGui::Text("%" PRIu16, b->eps.bp8.bootcause);
    StateWindowColumnManager("BP8 resetcause"); ImGui::Text("%" PRIu16, b->eps.bp8.resetcause);
    StateWindowColumnManager("BP8 soc"); ImGui::Text("%.3f", b->eps.bp8.soc);
    StateWindowColumnManager("BP8 bat_avr_temp"); ImGui::Text("%.3f", b->eps.bp8.bat_avr_temp);
    StateWindowColumnManager("BP8 vbat"); ImGui::Text("%" PRIu16, b->eps.bp8.vbat);
    StateWindowColumnManager("BP8 i"); ImGui::Text("%.3f", b->eps.bp8.i);
    StateWindowColumnManager("BP8 heater_i"); ImGui::Text("%" PRIu16, b->eps.bp8.heater_i);
    StateWindowColumnManager("GPIO gpio_status"); ImGui::Text("0x%04X", b->eps.gpio.gpio_status);
    StateWindowColumnManager("GPIO sp_deploy_status"); ImGui::Text("%" PRIu8, b->eps.gpio.sp_deploy_status);

    BeaconSectionHeader("ADCS");
    StateWindowColumnManager("power_state"); ImGui::Text("%" PRIu8, b->adcs.power_state);
    StateWindowColumnManager("control_mode"); ImGui::Text("%" PRIu8, b->adcs.control_mode);
    StateWindowColumnManager("gyro0 rate x"); ImGui::Text("%.6f", b->adcs.gyro0_calibrated_rate_x);
    StateWindowColumnManager("gyro0 rate y"); ImGui::Text("%.6f", b->adcs.gyro0_calibrated_rate_y);
    StateWindowColumnManager("gyro0 rate z"); ImGui::Text("%.6f", b->adcs.gyro0_calibrated_rate_z);
    BeaconU8ArrayRow("css[6]", b->adcs.css, 6);
}
