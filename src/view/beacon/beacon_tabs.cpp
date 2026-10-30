// View - Beacon 창 탭 공통 표 도우미
#include "imgui.h"
#include "beacon_tabs.h"

void StateWindowColumnManager(const char * info)
{
    ImGui::TableNextColumn();
    ImGui::Text(info);
    ImGui::TableNextColumn();
}

// 비콘 가독성 업!!
// static void BeaconSectionHeader(const char* title)
// {
//     ImGui::TableNextRow();
//     ImGui::TableSetColumnIndex(0);

//     ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1,
//                        ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.35f, 0.4f)));


//     ImGui::PushFont(ImGui::GetFont());        
//     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
//     ImGui::TextUnformatted(title);
//     ImGui::PopStyleColor();
//     ImGui::PopFont();

//     ImGui::TableSetColumnIndex(1);
//     ImGui::TextUnformatted("");
// }
void BeaconSectionHeader(const char* title)
{
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Row background
    ImGui::TableSetBgColor(
        ImGuiTableBgTarget_RowBg0,
        ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.35f, 0.6f))
    );

    ImGui::TableSetColumnIndex(0);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.6f, 1.0f));
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();

    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted("");
}
