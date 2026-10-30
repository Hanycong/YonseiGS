// View - Beacon 창 "Mission Beacon" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"

// 표 머리글 고정 행/열 수 (miman_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawMissionBeaconTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
    MissionBeacon *missionbeacon = vm.LastMissionBeacon();
    if(ImGui::BeginTabItem("Mission Beacon"))
    {

        if (ImGui::BeginTable("##MissionBeaconTables", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))


    ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
    ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Data",      ImGuiTableColumnFlags_NoHide, 0.0f);
   
    ImGui::TableHeadersRow();


    if (!missionbeacon)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Mission Beacon");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("No data");
    }
    else
    {
        char buf[128];

        // =========================
        //  CCSDS HEADER
        // =========================
        BeaconSectionHeader("Telemetry Header");
        StateWindowColumnManager("CCSDS MsgID");
        ImGui::Text("0x%02X 0x%02X", missionbeacon->CCSDS_MID[0], missionbeacon->CCSDS_MID[1]);

        StateWindowColumnManager("CCSDS Sequence");
        ImGui::Text("0x%02X 0x%02X", missionbeacon->CCSDS_Seq[0], missionbeacon->CCSDS_Seq[1]);

        StateWindowColumnManager("CCSDS Length");
        ImGui::Text("0x%02X 0x%02X", missionbeacon->CCSDS_Len[0], missionbeacon->CCSDS_Len[1]);

        StateWindowColumnManager("CCSDS Time Code");
        ImGui::Text("0x%02X %02X %02X %02X %02X %02X",
                    missionbeacon->CCSDS_TimeCode[0], missionbeacon->CCSDS_TimeCode[1],
                    missionbeacon->CCSDS_TimeCode[2], missionbeacon->CCSDS_TimeCode[3],
                    missionbeacon->CCSDS_TimeCode[4], missionbeacon->CCSDS_TimeCode[5]);


        // =========================
        //  SRL HouseKeeping
        // =========================
        BeaconSectionHeader("SRL HouseKeeping");

        StateWindowColumnManager("SRL Command Counter");
        ImGui::Text("%" PRIu8, missionbeacon->srlpayload.CommandCounter);

        StateWindowColumnManager("SRL Command Error Counter");
        ImGui::Text("%" PRIu8, missionbeacon->srlpayload.CommandErrorCounter);

        for (int i = 0; i < 4; i++) {
            char label[64];

            snprintf(label, sizeof(label), "IOHandleStatus[%d]", i);
            StateWindowColumnManager(label);
            ImGui::Text("%" PRIu8, missionbeacon->srlpayload.IOHandleStatus[i]);

            snprintf(label, sizeof(label), "IOHandleTxCount[%d]", i);
            StateWindowColumnManager(label);
            ImGui::Text("%" PRIu16, missionbeacon->srlpayload.IOHandleTxCount[i]);
        }


        // =========================
        //  RPT Payload (FSW Report summary)
        // =========================
        BeaconSectionHeader("RPT Payload Summary");

        StateWindowColumnManager("CmdCounter");
        ImGui::Text("%" PRIu8, missionbeacon->rptpayload.CmdCounter);

        StateWindowColumnManager("CmdErrCounter");
        ImGui::Text("%" PRIu8, missionbeacon->rptpayload.CmdErrCounter);

        BeaconSectionHeader("RPT Queue Info");
        StateWindowColumnManager("ReportQueueCnt");
        ImGui::Text("%" PRIu8, missionbeacon->rptpayload.ReportQueueCnt);

        StateWindowColumnManager("CriticalQueueCnt");
        ImGui::Text("%" PRIu8, missionbeacon->rptpayload.CriticalQueueCnt);


        // =========================
        //  Operation Data
        // =========================
        BeaconSectionHeader("RPT Operation Data");

        StateWindowColumnManager("BootCount");
        ImGui::Text("%" PRIu16, missionbeacon->rptpayload.BootCount);

        StateWindowColumnManager("TimeSec");
        ImGui::Text("%" PRIu32, missionbeacon->rptpayload.TimeSec);

        StateWindowColumnManager("TimeSubsec");
        ImGui::Text("%" PRIu32, missionbeacon->rptpayload.TimeSubsec);

        StateWindowColumnManager("Sequence");
        ImGui::Text("%" PRIu32, missionbeacon->rptpayload.Sequence);

        StateWindowColumnManager("ResetCause");
        ImGui::Text("0x%02X", (unsigned int)missionbeacon->rptpayload.ResetCause);


        // =========================
        // Mission Beacon Payload
        // =========================
        BeaconSectionHeader("Mission Beacon Payload");

        StateWindowColumnManager("Command Counter");
        ImGui::Text("%" PRIu8, missionbeacon->paybcnpayload.CommandCounter);

        StateWindowColumnManager("Command Error Counter");
        ImGui::Text("%" PRIu8, missionbeacon->paybcnpayload.CommandErrorCounter);

        StateWindowColumnManager("Payload System Status");
        ImGui::Text("%" PRIi8, missionbeacon->paybcnpayload.sys_status);

        StateWindowColumnManager("NTC 0 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_0);

        StateWindowColumnManager("NTC 1 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_1);

        StateWindowColumnManager("NTC 2 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_2);

        StateWindowColumnManager("NTC 3 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_3);

        StateWindowColumnManager("NTC 4 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_4);

        StateWindowColumnManager("NTC 5 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_5);

        StateWindowColumnManager("NTC 6 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_6);

        StateWindowColumnManager("NTC 7 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_7);

        StateWindowColumnManager("NTC 8 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_8);

        StateWindowColumnManager("NTC 9 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_9);

        StateWindowColumnManager("NTC 10 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_10);

        StateWindowColumnManager("NTC 11 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->paybcnpayload.temp_ntc_11);


        // =========================
        //  Mission Housekeeping Payload
        // =========================
        BeaconSectionHeader("Mission Housekeeping Payload");

        StateWindowColumnManager("HK CommandCounter");
        ImGui::Text("%" PRIu8, missionbeacon->payhkpayload.CommandCounter);

        StateWindowColumnManager("HK CommandErrorCounter");
        ImGui::Text("%" PRIu8, missionbeacon->payhkpayload.CommandErrorCounter);

        StateWindowColumnManager("HK System Status");
        ImGui::Text("%" PRIi8, missionbeacon->payhkpayload.sys_status);

        StateWindowColumnManager("HK NTC 0 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_0);

        StateWindowColumnManager("HK NTC 1 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_1);

        StateWindowColumnManager("HK NTC 2 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_2);

        StateWindowColumnManager("HK NTC 3 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_3);

        StateWindowColumnManager("HK NTC 4 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_4);

        StateWindowColumnManager("HK NTC 5 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_5);

        StateWindowColumnManager("HK NTC 6 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_6);

        StateWindowColumnManager("HK NTC 7 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_7);

        StateWindowColumnManager("HK NTC 8 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_8);

        StateWindowColumnManager("HK NTC 9 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_9);

        StateWindowColumnManager("HK NTC 10 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_10);

        StateWindowColumnManager("HK NTC 11 Temperature");
        ImGui::Text("%" PRIi16, missionbeacon->payhkpayload.temp_ntc_11);

        StateWindowColumnManager("HK sen1_data_0");
        ImGui::Text("%" PRIu32, missionbeacon->payhkpayload.sen1_data_0);

        StateWindowColumnManager("HK sen1_data_1");
        ImGui::Text("%" PRIu32, missionbeacon->payhkpayload.sen1_data_1);



    }


        ImGui::EndTable();
        ImGui::EndTabItem();
        ImGui::SetWindowFontScale(1.0 * fontscale);
    }
}
