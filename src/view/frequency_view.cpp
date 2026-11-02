// View - Frequency 창 (Pre-Pass 탭 가운데 열)
//
// ImGui 호출과 FrequencyVM 호출만 있다. 상태와 동작은 viewmodel/frequency_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "frequency_vm.h"
#include "frequency_view.h"
#include "ui_kit.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

using namespace mim::ui;

void ImGui_FrequencyWindow_Body(float fontscale)
{
    mim::vm::FrequencyVM & vm = mim::vm::FrequencyViewModel();

    // ───── 송수신기 주파수
    SectionHeader("TRANSCEIVER");
    RowLabel("Center (Hz)", 7.0f);
    float setW = ImGui::GetFontSize() * 4.0f;
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - setW - ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputDouble("##centerfrequency(Hz)", vm.CenterFrequencyHz(), NULL, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Set##SetDefaultFrequency", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight())))
    {
        vm.ApplyCenterFrequency();
    }

    ImGui::Spacing();
    if (ImGui::BeginTable("##TRXspectable", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH))
    {
        ImGui::TableSetupColumn("");
        ImGui::TableSetupColumn("RX");
        ImGui::TableSetupColumn("TX");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled("Frequency");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f MHz", vm.RxFrequencyMHz());
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.6f MHz", vm.TxFrequencyMHz());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled("Baud");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%" PRIu32 " bps", vm.RxBaud());
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%" PRIu32 " bps", vm.TxBaud());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextDisabled("Last RSSI");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%" PRId16 " dBm", vm.LastRssi());
        ImGui::EndTable();
    }
    ImGui::Spacing();
    if (GridButton("Set to RX", 0))
    {
        vm.SwitchToRx();
    }
    if (GridButton("Set to TX", 1))
    {
        vm.SwitchToTx();
    }

    // ───── 도플러 보정
    SectionHeader("DOPPLER");
    bool running = vm.DopplerRunning();
    if (ToggleWideButton(running ? "Stop Doppler Correction" : "Start Doppler Correction", running))
    {
        if (running)
            vm.StopDoppler();
        else
            vm.StartDoppler();
    }

    // ───── 링크 지연 설정
    SectionHeader("LINK TIMING");
    const float lw = 8.0f;
    RowLabel("Ping Size", lw);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##pingsize", ImGuiDataType_U16, vm.PingSize(), NULL, NULL, "%u");
    RowLabel("Default Delay", lw);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##defaulttimeout", ImGuiDataType_U32, vm.DefaultDelay(), NULL, NULL, "%u");
    RowLabel("Guard Delay", lw);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##guarddelay", ImGuiDataType_U32, vm.GuardDelay(), NULL, NULL, "%u");
    RowLabel("Queue Delay", lw);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##queuedelay", ImGuiDataType_U32, vm.QueueDelay(), NULL, NULL, "%u");
    if (WideButton("Apply##apply"))
    {
        vm.ApplyDelaySpec();
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
