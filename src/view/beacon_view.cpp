// View - Beacon 창 (Pass Operation 탭 오른쪽 열: 수신 텔레메트리 탭 6개)
//
// 탭 바만 그리고 탭마다 view/beacon/<탭>_tab.cpp 의 함수를 부른다.
#include "imgui.h"
#include "beacon_tabs.h"
#include "beacon_view.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

void ImGui_BeaconWindow_Body(float fontscale)
{
    ImGui::BeginTabBar("##InfoDesk");
    DrawBeaconTab(fontscale);
    DrawMissionBeaconTab(fontscale);
    DrawReportTab(fontscale);
    DrawEventTab(fontscale);
    DrawResponseTab(fontscale);

    // if(ImGui::BeginTabItem("Telemetry"))
    // {
    //     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
    //     ImGui::Text("Total RX Bytes(Beacon) : %"PRIu32, get_rx_bytes());
    //     ImGui::SetWindowFontScale(0.6 * fontscale);
    //     ImGui::PopStyleColor();
    //     ImGui::SetWindowFontScale(1.0 * fontscale);
    //     ImGui::EndTabItem();
    // }

    DrawAx100RparamTab(fontscale);
    ImGui::EndTabBar();
}

void ImGui_BeaconWindow(float fontscale)
{
    ImGui::Begin("Beacon GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_BeaconWindow_Body(fontscale);    
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}
