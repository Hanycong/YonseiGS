// View - AutoPilot 창 (자동 운용 설정·시작/정지, 카운터, 간이 명령 생성기)
//
// main.cpp 의 호출이 주석 처리돼 있어 지금은 화면에 나오지 않는다.
// 상태와 동작은 viewmodel/autopilot_vm (AutoPilotVM).
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "autopilot_vm.h"

extern ImGuiWindowFlags mim_winflags;

void ImGui_AutoPilotWindow(float fontscale)
{
    mim::vm::AutoPilotVM & vm = mim::vm::AutoPilotViewModel();
    static bool p_open = true;

    ImGui::Begin("AutoPilot", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);

    // ===== Autopilot controls =====
    ImGui::Text("AutoPilot Controls");
    ImGui::Checkbox("Beacon Check", vm.DoBeaconCheck());
    // ImGui::SameLine();
    ImGui::Checkbox("Ping", vm.DoPing());
    ImGui::SameLine();
    ImGui::Checkbox("CMD", vm.DoCmd());

    // ImGui::Checkbox("FTP DL", &State.DoFTPDL);
    // ImGui::SameLine();
    // ImGui::Checkbox("FTP UL", &State.DoFTPUL);
    // ImGui::SameLine();
    ImGui::Checkbox("AMP", vm.AmpMode());

    // ImGui::Checkbox("Baud Calibration", &State.DoBaudCalibration);
    // ImGui::SameLine();
    // ImGui::Checkbox("Freq Calibration", &State.DoFreqCalibration);

    ImGui::Text("iteration");
    ImGui::SameLine();
    ImGui::InputInt("##AutoPilotIter", vm.Iteration());

    ImGui::Text("Time Span");
    ImGui::SameLine();
    ImGui::InputInt("##AutoPilotSpan", vm.SpanTime());

    if (!vm.Running())
    {
        if (ImGui::Button("Start AutoPilot", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.8f)))
        {
            vm.Start();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop AutoPilot", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.8f)))
        {
            vm.Stop();
        }
        ImGui::PopStyleColor();
    }

    // ===== Live status =====
    ImGui::Separator();
    ImGui::Text("Live Counters");
    ImGui::Text("BeaconCounter : %d", vm.BeaconCount());
    ImGui::Text("PingCounter   : %d", vm.PingCount());
    ImGui::Text("CMDCount      : %d", vm.CmdCount());

    // ===== CMD Generator (simple) =====
    ImGui::Separator();
    ImGui::Text("CMD Generator");

    ImGui::InputScalar("MsgID",   ImGuiDataType_U32, vm.TestMsgId(),  NULL, NULL, "0x%08X");
    ImGui::InputScalar("FncCode", ImGuiDataType_U16, vm.TestFncCode(), NULL, NULL, "%u");

    CMDDataGenerator(*vm.TestMsgId(), *vm.TestFncCode(), vm.TestHeader(), vm.TestHeaderSize());

    ImGui::Separator();
    ImGui::Text("SatCMD Queue");
    for (int i = 0; i < 256; i++)
    {
        if (vm.QueuedCommandReady(i))
            ImGui::Text("SatCMD[%d] : READY", i);
    }

    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

int CMDDataGenerator(uint32_t msgid, uint16_t fnccode, void *Requested, size_t RequestedSize) 
{
    mim::vm::AutoPilotVM & vm = mim::vm::AutoPilotViewModel();
    ImGui::Checkbox("Checksum", vm.ChecksumOn());
    ImGui::SameLine();
	ImGui::Checkbox("Scheduler", vm.SchedulerOn());
	if(*vm.SchedulerOn())
	{
        *vm.ChecksumOn() = true;
		ImGui::Text("Execution Time     : ");
		ImGui::SameLine();
		ImGui::InputScalar("##ExecutionTime", ImGuiDataType_U32, vm.ExecutionTime(), NULL, NULL, "%u");
		ImGui::Text("Execution Window   : ");
		ImGui::SameLine();
		ImGui::InputScalar("##ExecutionWindow", ImGuiDataType_U32, vm.ExecutionWindow(), NULL, NULL, "%u");
		ImGui::Text("Entry ID           : ");
		ImGui::SameLine();
		ImGui::InputScalar("##EntryID", ImGuiDataType_U16, vm.EntryId(), NULL, NULL, "%u");
		ImGui::Text("GroupID            : ");
		ImGui::SameLine();
		ImGui::InputScalar("##GroupID", ImGuiDataType_U16, vm.GroupId(), NULL, NULL, "%u");
	}
	if(ImGui::Button("Generate", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
	{
		vm.Generate(msgid, fnccode, Requested, RequestedSize);
	}
	ImGui::SameLine();
	if(ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
	{
		vm.ClearRequest(Requested, RequestedSize);
		return 0;
	}
    return 1;
	
}
