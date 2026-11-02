// View - Beacon 창 "Event" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"

// 표 머리글 고정 행/열 수 (YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawEventTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
    Event *event = vm.LastEvent();
    if(ImGui::BeginTabItem("Event"))
    {

        if (ImGui::BeginTable("##EventTables", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))


    ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
    ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Data",      ImGuiTableColumnFlags_NoHide, 0.0f);
   
    ImGui::TableHeadersRow();


    if (!event)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Event");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("No data");
    }
    else
    {
        char buf[128];

        // =========================
        //  TLM Header
        // =========================
        BeaconSectionHeader("Event Header");

        StateWindowColumnManager("Telemetry MsgID");
        ImGui::Text("0x%04" PRIX16, (uint16_t)event->CCSDS_MsgId);

        StateWindowColumnManager("Telemetry Sequence");
        ImGui::Text("0x%04" PRIX16, (uint16_t)event->CCSDS_Seq);

        StateWindowColumnManager("Telemetry Length");
        ImGui::Text("0x%04" PRIX16, (uint16_t)event->CCSDS_Len);

        StateWindowColumnManager("Telemetry Time Code");
        ImGui::Text("0x%02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8,
                    (uint8_t)event->CCSDS_TimeCode[0], (uint8_t)event->CCSDS_TimeCode[1],
                    (uint8_t)event->CCSDS_TimeCode[2], (uint8_t)event->CCSDS_TimeCode[3],
                    (uint8_t)event->CCSDS_TimeCode[4], (uint8_t)event->CCSDS_TimeCode[5]);

        StateWindowColumnManager("Telemetry Padding");
        ImGui::Text("0x%08" PRIX32, (uint32_t)event->CCSDS_Padding);

        // =========================
        //  EVENT payload
        // =========================
        BeaconSectionHeader("Payload");

        StateWindowColumnManager("AppName");
        ImGui::Text("%.*s", (int)sizeof(event->AppName), event->AppName);

        StateWindowColumnManager("EventID");
        ImGui::Text("%" PRIu16 " (0x%04" PRIX16 ")", (uint16_t)event->EventID, (uint16_t)event->EventID);

        StateWindowColumnManager("EventType");
        ImGui::Text("%" PRIu16 " (0x%04" PRIX16 ")", (uint16_t)event->EventType, (uint16_t)event->EventType);

        StateWindowColumnManager("SpacecraftID");
        ImGui::Text("%" PRIu32 " (0x%08" PRIX32 ")", (uint32_t)event->SpacecraftID, (uint32_t)event->SpacecraftID);

        StateWindowColumnManager("ProcessorID");
        ImGui::Text("%" PRIu32 " (0x%08" PRIX32 ")", (uint32_t)event->ProcessorID, (uint32_t)event->ProcessorID);

        StateWindowColumnManager("Message");
        ImGui::Text("%.*s", (int)sizeof(event->Message), event->Message);

        StateWindowColumnManager("Spare1");
        ImGui::Text("0x%02" PRIX8, (uint8_t)event->Spare1);

        StateWindowColumnManager("Spare2");
        ImGui::Text("0x%02" PRIX8, (uint8_t)event->Spare2);
    }


        ImGui::EndTable();
        ImGui::EndTabItem();
        ImGui::SetWindowFontScale(1.0 * fontscale);
    }
}
