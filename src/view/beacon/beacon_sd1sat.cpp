// View - Beacon 창 "Beacon" 탭: SD1-SAT 위성 비콘 표 (위성 모드가 SD1SAT 일 때)
//
// GS_nuri5 miman_imgui.cpp 의 ImGui_SD1SATBeaconWindow 를 옮긴 것 (행 이름/형식 그대로).
// 표(##BeaconTables) 안에서 불린다. b 가 NULL 이면 "No data" 한 줄.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "imgui.h"
#include "beacon_tabs.h"

void DrawSD1SATBeacon(const SD1SAT_Beacon_t * b)
{
    if (!b) {
        StateWindowColumnManager("SD1SAT Beacon");
        ImGui::Text("No data");
        return;
    }

    char call_sign_buf[5] = {0};
    memcpy(call_sign_buf, b->header.call_sign, 4);

    BeaconSectionHeader("SD1SAT Header");
    StateWindowColumnManager("Call Sign"); ImGui::Text("%s", call_sign_buf);

    BeaconSectionHeader("OPS");
    StateWindowColumnManager("RestoredTimestamp"); ImGui::Text("%" PRIu32, b->OPS.RestoredTimestamp);
    StateWindowColumnManager("SecondsSinceFirstBoot"); ImGui::Text("%" PRIu32, b->OPS.SecondsSinceFirstBoot);
    StateWindowColumnManager("BootCount"); ImGui::Text("%" PRIu16, b->OPS.BootCount);
    StateWindowColumnManager("WarningFlags"); ImGui::Text("0x%04X", b->OPS.WarningFlags);

    BeaconSectionHeader("UTRX");
    StateWindowColumnManager("BootCause"); ImGui::Text("0x%08X", b->UTRX.BootCause);
    StateWindowColumnManager("BootCount"); ImGui::Text("%" PRIu16, b->UTRX.BootCount);
    StateWindowColumnManager("BoardTemp"); ImGui::Text("%" PRId16, b->UTRX.BoardTemp);
    StateWindowColumnManager("ActiveConf"); ImGui::Text("%" PRIu8, b->UTRX.ActiveConf);

    BeaconSectionHeader("EPS - PCDU P60 Dock");
    StateWindowColumnManager("DockOutEnMask"); ImGui::Text("0x%04X", b->EPS.DockOutEnMask);
    StateWindowColumnManager("PduOutEnMask"); ImGui::Text("0x%04X", b->EPS.PduOutEnMask);
    StateWindowColumnManager("PduConvEnMask"); ImGui::Text("0x%04X", b->EPS.PduConvEnMask);
    BeaconI16ArrayRow("AcuCin[6]", b->EPS.AcuCin, 6);
    BeaconU16ArrayRow("AcuVin[6]", b->EPS.AcuVin, 6);
    StateWindowColumnManager("Vbatt"); ImGui::Text("%" PRIu16, b->EPS.Vbatt);
    StateWindowColumnManager("Vcc"); ImGui::Text("%" PRId16, b->EPS.Vcc);
    StateWindowColumnManager("BattV"); ImGui::Text("%" PRIu16, b->EPS.BattV);
    StateWindowColumnManager("BattC"); ImGui::Text("%" PRId16, b->EPS.BattC);
    StateWindowColumnManager("BattChrg"); ImGui::Text("%" PRId16, b->EPS.BattChrg);
    StateWindowColumnManager("BattDischrg"); ImGui::Text("%" PRId16, b->EPS.BattDischrg);
    StateWindowColumnManager("BattTemp"); ImGui::Text("%" PRId16, b->EPS.BattTemp);
    StateWindowColumnManager("DockTemp"); ImGui::Text("%" PRId16, b->EPS.DockTemp);
    StateWindowColumnManager("DockBootCount"); ImGui::Text("%" PRIu16, b->EPS.DockBootCount);
    StateWindowColumnManager("GndWdtCnt"); ImGui::Text("%" PRIu16, b->EPS.GndWdtCnt);
    StateWindowColumnManager("CanWdtCnt"); ImGui::Text("%" PRIu16, b->EPS.CanWdtCnt);
    StateWindowColumnManager("GndWdtLeft"); ImGui::Text("%" PRIu32, b->EPS.GndWdtLeft);
    StateWindowColumnManager("CanWdtLeft"); ImGui::Text("%" PRIu32, b->EPS.CanWdtLeft);
    StateWindowColumnManager("DockResetCause"); ImGui::Text("0x%08X", b->EPS.DockResetCause);

    BeaconSectionHeader("UANT");
    StateWindowColumnManager("DoorStates"); ImGui::Text("0x%02X", b->UANT.DoorStates);
    StateWindowColumnManager("FeedbackSwitches"); ImGui::Text("0x%02X", b->UANT.FeedbackSwitches);
}

