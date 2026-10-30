// View - Beacon 창 "Response" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"

// 표 머리글 고정 행/열 수 (miman_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawResponseTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
    GETFILEINFO *getfileinfo = vm.LastFileInfo();
    if(ImGui::BeginTabItem("Response"))
    {

        if (ImGui::BeginTable("##ResponseTables", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))


    ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
    ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Data",      ImGuiTableColumnFlags_NoHide, 0.0f);
   
    ImGui::TableHeadersRow();


    if (!getfileinfo)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Response");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("No data");
    }
    else
    {
        char buf[128];


        // =========================
        //  TLM Header
        // =========================
        BeaconSectionHeader("Telemetry Header");
        StateWindowColumnManager("Telemetry MsgID");
        ImGui::Text("0x%02X 0x%02X", getfileinfo->CCSDS_MID[0], getfileinfo->CCSDS_MID[1]);

        StateWindowColumnManager("Telemetry Sequence");
        ImGui::Text("0x%02X 0x%02X", getfileinfo->CCSDS_Seq[0], getfileinfo->CCSDS_Seq[1]);

        StateWindowColumnManager("Telemetry Length");
        ImGui::Text("0x%02X 0x%02X", getfileinfo->CCSDS_Len[0], getfileinfo->CCSDS_Len[1]);

        StateWindowColumnManager("Telemetry Time Code");
        ImGui::Text("0x%02X %02X %02X %02X %02X %02X",
                    getfileinfo->CCSDS_TimeCode[0], getfileinfo->CCSDS_TimeCode[1],
                    getfileinfo->CCSDS_TimeCode[2], getfileinfo->CCSDS_TimeCode[3],
                    getfileinfo->CCSDS_TimeCode[4], getfileinfo->CCSDS_TimeCode[5]);

        StateWindowColumnManager("Telemetry Padding");
        ImGui::Text("0x%02X %02X %02X %02X",
                    getfileinfo->padding[0], getfileinfo->padding[1],
                    getfileinfo->padding[2], getfileinfo->padding[3]);


        // =========================
        //  GETFILEINFO payload
        // =========================


        BeaconSectionHeader("Payload");
        StateWindowColumnManager("File Status");
        ImGui::Text("%" PRIu8, getfileinfo->FileStatus);

        StateWindowColumnManager("CRC Computed");
        ImGui::Text("%" PRIu8, getfileinfo->CRC_Computed);

        StateWindowColumnManager("Spare");
        ImGui::Text("0x%02X 0x%02X", getfileinfo->Spare[0], getfileinfo->Spare[1]);

        StateWindowColumnManager("CRC");
        ImGui::Text("%" PRIu32, getfileinfo->CRC);

        StateWindowColumnManager("File Size");
        ImGui::Text("%" PRIu32, getfileinfo->FileSize);

        StateWindowColumnManager("Last Modified Time");
        ImGui::Text("%" PRIu32, getfileinfo->LastModifiedTime);

        StateWindowColumnManager("Mode");
        ImGui::Text("%" PRIu32, getfileinfo->Mode);

        StateWindowColumnManager("Filename");
        ImGui::Text("%s", getfileinfo->Filename);

    }


        ImGui::EndTable();
        ImGui::EndTabItem();
        ImGui::SetWindowFontScale(1.0 * fontscale);
    }
}
