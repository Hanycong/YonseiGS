// View - Frequency 창 (Pre-Pass 탭 가운데 열)
//
// ImGui 호출과 FrequencyVM 호출만 있다. 상태와 동작은 viewmodel/frequency_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "frequency_vm.h"
#include "frequency_view.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void ImGui_FrequencyWindow_Body(float fontscale)
{
    {   
    
    mim::vm::FrequencyVM & vm = mim::vm::FrequencyViewModel();

    ImGui::SetWindowFontScale(fontscale);
    ImGui::Text("Center Frequency");
    ImGui::InputDouble("##centerfrequency(Hz)", vm.CenterFrequencyHz(), NULL, NULL);
    ImGui::SameLine();
    if(ImGui::Button("Set##SetDefaultFrequency", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        vm.ApplyCenterFrequency();
    }

    if (ImGui::BeginTable("##TRXspectable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.8)))
    {
        ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
        ImGui::TableSetupColumn("RX",       ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("TX",       ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%f MHz", vm.RxFrequencyMHz());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%f MHz", vm.TxFrequencyMHz());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%"PRIu32" bps", vm.RxBaud());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%"PRIu32" bps", vm.TxBaud());
        ImGui::EndTable();
    }
    ImGui::Text("Last RSSI(dBm) : ");
    ImGui::SameLine();
    ImGui::Text("%"PRId16"", vm.LastRssi());
    if (ImGui::Button("Set to RX", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        vm.SwitchToRx();
    }
    ImGui::SameLine();
    if (ImGui::Button("Set to TX", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        vm.SwitchToTx();
    }

    if(vm.DopplerRunning() == false)
    {

        if (ImGui::Button("Start Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.StartDoppler();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            vm.StopDoppler();
        }
        ImGui::PopStyleColor();
    }
    ImGui::Text("Ping Size  ");
    ImGui::SameLine();
    ImGui::InputScalar("##pingsize", ImGuiDataType_U16, vm.PingSize(), NULL, NULL, "%u");
    ImGui::BeginChild("##DelaySettings", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, mim_winflags);
    ImGui::Text("Default Delay");
    ImGui::SameLine();
    ImGui::InputScalar("##defaulttimeout", ImGuiDataType_U32, vm.DefaultDelay(), NULL, NULL, "%u");
    ImGui::Text("Guard Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##guarddelay", ImGuiDataType_U32, vm.GuardDelay(), NULL, NULL, "%u");
    ImGui::Text("Queue Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##queuedelay", ImGuiDataType_U32, vm.QueueDelay(), NULL, NULL, "%u");
    if(ImGui::Button("Apply##apply", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        vm.ApplyDelaySpec();
    }
    ImGui::EndChild();

    ImGui::SetWindowFontScale(1.0);
    }
}


void ImGui_FrequencyWindow(float fontscale)
{   
    
    ImGui::Begin("Frequency Window", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_FrequencyWindow_Body(fontscale);   // 본체는 여기서 호출
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}
