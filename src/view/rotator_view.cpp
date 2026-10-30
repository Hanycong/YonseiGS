// View - Rotator 창 (Pre-Pass 탭 오른쪽 열)
//
// ImGui 호출과 RotatorVM 호출만 있다. 상태와 동작은 viewmodel/rotator_vm.h 참고.
#include "imgui.h"
#include "rotator_vm.h"
#include "rotator_view.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void ImGui_RotatorWindow_Body(float fontscale)
{
    mim::vm::RotatorVM & vm = mim::vm::RotatorViewModel();

    if(vm.Engaging() == false)
    {
        if (ImGui::Button("Start Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.StartEngage();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.StopEngage();
        }
        ImGui::PopStyleColor();
    }
    
    
    ImGui::Text("Rotator Manual Control");
    ImGui::Text("AZ : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_az", vm.TargetAzimuth());
    ImGui::PopItemWidth();
    ImGui::Text("EL : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_el", vm.TargetElevation());
    ImGui::PopItemWidth();
    if (ImGui::Button("Set", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        vm.MoveToTarget();
        //memset(&ControlBuf,0,sizeof(ControlBuf));
    }
    ImGui::SameLine();
    if (ImGui::Button("Init", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        vm.MoveToHome();
    }

    if (ImGui::BeginTable("##Rotspectable", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.8)))
    {
        ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
        ImGui::TableSetupColumn("Band",         ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Azimuth",      ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Elevation",    ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("UHF");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d °", vm.CurrentAzimuth());
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%d °", vm.CurrentElevation());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("S-band");
        ImGui::TableSetColumnIndex(1);
        // ImGui::Text("%d °", Now.az_now);
        ImGui::TableSetColumnIndex(2);
        // ImGui::Text("%d °", Now.el_now);
        ImGui::EndTable();
    }


    if(ImGui::Button("Rotator On", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        vm.RotatorOn();
    }
    ImGui::SameLine();
    if(ImGui::Button("Rotator OFF", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        vm.RotatorOff();
    }
    if(ImGui::Button("Rotator IO Buffer Free", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
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
