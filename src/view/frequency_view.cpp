// View - Frequency 창 (Pre-Pass 탭 가운데 열)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것 (MVVM 1단계: View 추출).
// 다음 단계에서 setup-> / State. / 장비 호출을 FrequencyVM 으로 옮긴다.
#include <inttypes.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_radial.h"
#include "svc_tracking.h"
#include "frequency_view.h"

extern Setup * setup;
extern StateCheckUnit State;
extern Console console;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void ImGui_FrequencyWindow_Body(float fontscale)
{
    {   
    
    ImGui::SetWindowFontScale(fontscale);
    ImGui::Text("Center Frequency");
    ImGui::InputDouble("##centerfrequency(Hz)", &setup->default_freq, NULL, NULL);
    ImGui::SameLine();
    if(ImGui::Button("Set##SetDefaultFrequency", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        SetRxFreq(setup->default_freq);
        SetTxFreq(setup->default_freq);
        SetRxBaud(4800);
        SetTxBaud(4800);
        UpdateFreq();
        UpdateBaud();
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
        ImGui::Text("%f MHz", ((float)param1.freq) / 1000000);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%f MHz", ((float)param5.freq) / 1000000);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%"PRIu32" bps", param1.baud);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%"PRIu32" bps", param5.baud);
        ImGui::EndTable();
    }
    ImGui::Text("Last RSSI(dBm) : ");
    ImGui::SameLine();
    ImGui::Text("%"PRId16"", Return_RSSI());
    if (ImGui::Button("Set to RX", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        switch_to_rx(setup->ax100_node);
    }
    ImGui::SameLine();
    if (ImGui::Button("Set to TX", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        switch_to_tx(setup->ax100_node);
    }

    if(State.Doppler == false)
    {

        if (ImGui::Button("Start Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            mim::svc::StartDoppler();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            mim::svc::StopDoppler();
        }
        ImGui::PopStyleColor();
    }
    ImGui::Text("Ping Size  ");
    ImGui::SameLine();
    ImGui::InputScalar("##pingsize", ImGuiDataType_U16, &setup->pingsize, NULL, NULL, "%u");
    ImGui::BeginChild("##DelaySettings", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, mim_winflags);
    ImGui::Text("Default Delay");
    ImGui::SameLine();
    ImGui::InputScalar("##defaulttimeout", ImGuiDataType_U32, &setup->default_timeout, NULL, NULL, "%u");
    ImGui::Text("Guard Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##guarddelay", ImGuiDataType_U32, &setup->guard_delay, NULL, NULL, "%u");
    ImGui::Text("Queue Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##queuedelay", ImGuiDataType_U32, &setup->queue_delay, NULL, NULL, "%u");
    if(ImGui::Button("Apply##apply", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        set_serial_spec_micsec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);
        console.AddLog("Set Delay Spec : Default : %u, Guard : %u, Queue : %u", setup->default_timeout, setup->guard_delay, setup->queue_delay);
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
