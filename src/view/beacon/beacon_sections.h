// View - Beacon 창 "Beacon" 탭: BEE1012 / UELYSYS 비콘이 같이 가진 구간
//
// 두 위성의 비콘 구조체는 타입이 따로지만 EPS(PMU / PDU / ACU)와 ADCS 구간은 멤버 이름과 표시 형식이 같다.
// 예전에는 두 파일에 같은 행을 따로 적었다. 행 이름/형식은 GS_nuri5 화면 그대로다.
#pragma once

#include <inttypes.h>
#include <stdio.h>

#include "imgui.h"
#include "beacon_tabs.h"

// EPS - PMU / PDU / ACU (eps = 비콘의 eps 구간)
template <typename EpsT>
void DrawBeaconEpsP80Rows(const EpsT & eps)
{
    BeaconSectionHeader("EPS - PMU");
    StateWindowColumnManager("bootcause"); ImGui::Text("%" PRIu32, eps.pmu.bootcause);
    StateWindowColumnManager("resetcause"); ImGui::Text("%" PRIu16, eps.pmu.resetcause);
    StateWindowColumnManager("bootcount"); ImGui::Text("%" PRIu16, eps.pmu.bootcount);
    BeaconU8ArrayRow("out_en[6]", eps.pmu.out_en, 6);
    BeaconI16ArrayRow("temp[2]", eps.pmu.temp, 2);
    StateWindowColumnManager("batt_mode"); ImGui::Text("%" PRIu8, eps.pmu.batt_mode);
    StateWindowColumnManager("batt_i"); ImGui::Text("%" PRId16, eps.pmu.batt_i);
    StateWindowColumnManager("batt_v"); ImGui::Text("%" PRIu16, eps.pmu.batt_v);
    StateWindowColumnManager("sm_en"); ImGui::Text("%" PRIu8, eps.pmu.sm_en);
    StateWindowColumnManager("gnd_wdt_cnt"); ImGui::Text("%" PRIu16, eps.pmu.gnd_wdt_cnt);
    StateWindowColumnManager("bus_wdt_cnt"); ImGui::Text("%" PRIu16, eps.pmu.bus_wdt_cnt);
    StateWindowColumnManager("gnd_wdt_left"); ImGui::Text("%" PRIu32, eps.pmu.gnd_wdt_left);
    StateWindowColumnManager("bus_wdt_left"); ImGui::Text("%" PRIu32, eps.pmu.bus_wdt_left);

    BeaconSectionHeader("EPS - PDU");
    BeaconI16ArrayRow("out_i[13]", eps.pdu.out_i, 13);
    BeaconU8ArrayRow("out_en[13]", eps.pdu.out_en, 13);

    BeaconSectionHeader("EPS - ACU");
    for (int unit = 0; unit < 2; unit++) {
        char label[32];
        snprintf(label, sizeof(label), "ACU %d input_i[6]", unit);
        BeaconI16ArrayRow(label, eps.acu.acu[unit].input_i, 6);
        snprintf(label, sizeof(label), "ACU %d input_v[6]", unit);
        BeaconU16ArrayRow(label, eps.acu.acu[unit].input_v, 6);
        snprintf(label, sizeof(label), "ACU %d mppt_mode", unit);
        StateWindowColumnManager(label); ImGui::Text("%" PRIu8, eps.acu.acu[unit].mppt_mode);
    }
}

// ADCS (adcs = 비콘의 adcs 구간)
template <typename AdcsT>
void DrawBeaconAdcsRows(const AdcsT & adcs)
{
    BeaconSectionHeader("ADCS");
    StateWindowColumnManager("power_state"); ImGui::Text("%" PRIu8, adcs.power_state);
    StateWindowColumnManager("control_mode"); ImGui::Text("%" PRIu8, adcs.control_mode);
    StateWindowColumnManager("gyro0 rate x"); ImGui::Text("%.6f", adcs.gyro0_calibrated_rate_x);
    StateWindowColumnManager("gyro0 rate y"); ImGui::Text("%.6f", adcs.gyro0_calibrated_rate_y);
    StateWindowColumnManager("gyro0 rate z"); ImGui::Text("%.6f", adcs.gyro0_calibrated_rate_z);
    BeaconU8ArrayRow("css[6]", adcs.css, 6);
}
