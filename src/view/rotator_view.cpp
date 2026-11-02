// View - Rotator 창 (Pre-Pass 탭 오른쪽 열)
//
// ImGui 호출과 RotatorVM 호출만 있다. 상태와 동작은 viewmodel/rotator_vm.h 참고.
#include "imgui.h"
#include "rotator_vm.h"
#include "rotator_view.h"
#include "ui_kit.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

using namespace mim::ui;

void ImGui_RotatorWindow_Body(float fontscale)
{
    mim::vm::RotatorVM & vm = mim::vm::RotatorViewModel();

    // ───── 자동 추적
    SectionHeader("AUTO TRACKING");
    bool engaging = vm.Engaging();
    if (ToggleWideButton(engaging ? "Stop Rotator Engaging" : "Start Rotator Engaging", engaging))
    {
        if (engaging)
            vm.StopEngage();
        else
            vm.StartEngage();
    }

    // ───── 현재 지향
    SectionHeader("CURRENT POSITION");
    if (ImGui::BeginTable("##Rotspectable", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH))
    {
        ImGui::TableSetupColumn("Band");
        ImGui::TableSetupColumn("Azimuth");
        ImGui::TableSetupColumn("Elevation");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled("UHF");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d °", vm.CurrentAzimuth());
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%d °", vm.CurrentElevation());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled("S-band");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextDisabled("-");
        ImGui::TableSetColumnIndex(2);
        ImGui::TextDisabled("-");
        ImGui::EndTable();
    }

    // ───── 수동 제어
    SectionHeader("MANUAL CONTROL");
    RowLabel("Azimuth", 6.0f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_az", vm.TargetAzimuth());
    RowLabel("Elevation", 6.0f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_el", vm.TargetElevation());
    if (GridButton("Move to Target##Set", 0))
    {
        vm.MoveToTarget();
    }
    if (GridButton("Home (0, 0)##Init", 1))
    {
        vm.MoveToHome();
    }

    // ───── 전원 / IO
    SectionHeader("POWER");
    if (GridButton("Rotator On", 0))
    {
        vm.RotatorOn();
    }
    if (GridButton("Rotator Off##RotatorOFF", 1))
    {
        vm.RotatorOff();
    }
    if (WideButton("Flush IO Buffer##RotatorIOBufferFree"))
    {
        vm.FlushIo();
    }
}

void ImGui_RotatorWindow(float fontscale)
{
    ImGui::Begin("Rotator Window", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_RotatorWindow_Body(fontscale);   // 본체는 여기서 호출
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();

}
