// View - Rotator 창 (Pre-Pass 탭 오른쪽 열)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것 (MVVM 1단계: View 추출).
// 다음 단계에서 State. / Now / 장비 호출을 RotatorVM 으로 옮긴다.
#include "miman_config.h"
#include "miman_orbital.h"
#include "svc_tracking.h"
#include "rotator_view.h"

extern StateCheckUnit State;
extern struct Antenna Now;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void ImGui_RotatorWindow_Body(float fontscale)
{
    if(State.Engage == false)
    {
        if (ImGui::Button("Start Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            mim::svc::StartEngage();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            mim::svc::StopEngage();
        }
        ImGui::PopStyleColor();
    }
    
    
    ImGui::Text("Rotator Manual Control");
    ImGui::Text("AZ : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_az", &Now.az_tag);
    ImGui::PopItemWidth();
    ImGui::Text("EL : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_el", &Now.el_tag);
    ImGui::PopItemWidth();
    if (ImGui::Button("Set", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        RotatorSetTo(Now.az_tag, Now.el_tag);
        //memset(&ControlBuf,0,sizeof(ControlBuf));
    }
    ImGui::SameLine();
    if (ImGui::Button("Init", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        RotatorSetTo(0, 0);
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
        ImGui::Text("%d °", Now.az_now);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%d °", Now.el_now);
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
        mim::svc::RotatorOn();
    }
    ImGui::SameLine();
    if(ImGui::Button("Rotator OFF", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        mim::svc::RotatorOff();
    }
    if(ImGui::Button("Rotator IO Buffer Free", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        RotatorEnter(NULL);
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
