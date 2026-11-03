// View - 3. Autopilot 탭 (자동 관제: 설정·시작/정지, 진행 상태·송신 조건, 패스 TC 스크립트)
//
// 아래 ImGui_AutoPilotWindow (옛 떠 있는 창)는 main.cpp 의 호출이 주석 처리돼 있어 화면에 나오지 않는다.
// 상태와 동작은 viewmodel/autopilot_vm (AutoPilotVM).
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "YonseiGS_config.h"
#include "ui_kit.h"
#include "YonseiGS_imgui.h"
#include "autopilot_vm.h"
#include "file_dialog.h"
#include "YonseiGS_theme.h"
#include "imgui_internal.h"

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

    mim::ui::RowLabel("iteration", 10.0f);
    ImGui::InputInt("##AutoPilotIter", vm.Iteration());

    mim::ui::RowLabel("Time Span", 10.0f);
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
		mim::ui::RowLabel("Execution Time", 10.0f);
		ImGui::InputScalar("##ExecutionTime", ImGuiDataType_U32, vm.ExecutionTime(), NULL, NULL, "%u");
		mim::ui::RowLabel("Execution Window", 10.0f);
		ImGui::InputScalar("##ExecutionWindow", ImGuiDataType_U32, vm.ExecutionWindow(), NULL, NULL, "%u");
		mim::ui::RowLabel("Entry ID", 10.0f);
		ImGui::InputScalar("##EntryID", ImGuiDataType_U16, vm.EntryId(), NULL, NULL, "%u");
		mim::ui::RowLabel("GroupID", 10.0f);
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

// ════════════════════════════════════════════════════════════════════════
// 3. Autopilot 탭
// ════════════════════════════════════════════════════════════════════════

using namespace mim::ui;

// 실행 중에는 바꿀 수 없는 입력을 흐리게 막는다 (ImGui 1.83 에는 BeginDisabled 가 없다)
static void BeginLocked(bool locked)
{
    if (!locked)
        return;
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

static void EndLocked(bool locked)
{
    if (!locked)
        return;
    ImGui::PopStyleVar();
    ImGui::PopItemFlag();
}

// 왼쪽 열: 대상 패스, 송신 조건 설정, 패스 작업, 시작/정지
void ImGui_AutoPilotControl_Body()
{
    mim::vm::AutoPilotVM & vm = mim::vm::AutoPilotViewModel();
    const float label_em = 9.0f;

    SectionHeader("TARGET PASS");
    ValueRow("Satellite", vm.TargetName());
    char aos[32], los[32], maxel[16];
    vm.PassTimes(aos, los, sizeof(aos));
    if (vm.HasPass())
        snprintf(maxel, sizeof(maxel), "%.0f°", vm.PassMaxElevation());
    else
        snprintf(maxel, sizeof(maxel), "-");
    ValueRow("AOS", vm.HasPass() ? aos : "-");
    ValueRow("LOS", vm.HasPass() ? los : "-");
    ValueRow("Max elevation", maxel);
    if (!vm.HasTarget())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, mim::TEXT_MUTE);
        ImGui::TextWrapped("Press Track on a satellite in 1. Pre-Pass to choose the target.");
        ImGui::PopStyleColor();
    }

    SectionHeader("TC CONDITIONS");
    RowLabel("Min elevation", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##ap_minel", vm.MinElevation(), 0.0f, 60.0f, "%.0f deg");
    ImGui::Checkbox("Require satellite reply (ping or beacon)", vm.RequireLink());
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("TC is sent only after the satellite answered a ping or a beacon arrived in this pass.");

    SectionHeader("PASS TASKS");
    bool running = vm.Running();
    BeginLocked(running);
    ImGui::Checkbox("Beacon check", vm.DoBeaconCheck());
    ImGui::SameLine();
    ImGui::Checkbox("Ping", vm.DoPing());
    ImGui::SameLine();
    ImGui::Checkbox("Send TC script", vm.DoCmd());
    ImGui::SameLine();
    ImGui::Checkbox("AMP", vm.AmpMode());
    RowLabel("Beacons needed", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##ap_iter", vm.Iteration());
    RowLabel("Ping interval [s]", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##ap_span", vm.SpanTime());
    EndLocked(running);
    if (*vm.Iteration() < 1) *vm.Iteration() = 1;
    if (*vm.SpanTime() < 1) *vm.SpanTime() = 1;

    ImGui::Spacing();
    if (running)
    {
        if (ToggleWideButton("Stop AutoPilot", true))
            vm.Stop();
    }
    else
    {
        bool can = vm.CanStart();
        BeginLocked(!can);
        if (WideButton("Start AutoPilot") && can)
            vm.Start();
        EndLocked(!can);
    }
}

// 가운데 열: 진행 단계, 송신 조건 점등, 스크립트 진행
void ImGui_AutoPilotStatus_Body()
{
    mim::vm::AutoPilotVM & vm = mim::vm::AutoPilotViewModel();

    SectionHeader("STATUS");
    int stage = vm.Stage();
    StatusLamp(mim::vm::AutoPilotVM::StageName(stage), vm.Running());

    SectionHeader("TC CONDITIONS (ALL REQUIRED)");
    mim::vm::AutoPilotVM::Gate g = vm.CurrentGate();
    char label[96];
    if (g.has_elevation)
        snprintf(label, sizeof(label), "Elevation %.1f° (min %.0f°)", g.elevation, *vm.MinElevation());
    else
        snprintf(label, sizeof(label), "Elevation - (no target)");
    StatusLamp(label, g.elevation_ok);
    StatusLamp("Ground antenna", g.antenna_ok);
    ImGui::Indent(ImGui::GetFontSize() * 1.2f);
    StatusLamp("Rotator connected", g.rotator_ok);
    StatusLamp("Rotator tracking (Engage)", g.engage_ok);
    StatusLamp("GS100 connected", g.radio_ok);
    ImGui::Unindent(ImGui::GetFontSize() * 1.2f);
    snprintf(label, sizeof(label), "Satellite link (ping %d, beacon %d)%s",
             vm.PingCount(), vm.BeaconCount(), *vm.RequireLink() ? "" : " - not required");
    StatusLamp(label, g.link_ok);
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, g.ok ? mim::OK : mim::TEXT_DIM);
    ImGui::TextUnformatted(g.ok ? "Ready to send TC" : "TC is held until all conditions are met");
    ImGui::PopStyleColor();

    SectionHeader("TC SCRIPT PROGRESS");
    char v[48];
    int step = vm.ScriptStep();
    if (step >= 0)
        snprintf(v, sizeof(v), "%d / %d", step + 1, vm.StepCount());
    else
        snprintf(v, sizeof(v), "-");
    ValueRow("Step", v);
    snprintf(v, sizeof(v), "%d", vm.ScriptSent());
    ValueRow("TC sent this pass", v);
    snprintf(v, sizeof(v), "%d", vm.CmdCount());
    ValueRow("Command replies", v);
}

// 오른쪽 열: TC 스크립트 표 + 편집
void ImGui_PassScript_Body()
{
    mim::vm::AutoPilotVM & vm = mim::vm::AutoPilotViewModel();
    static mim::ui::FileDialog script_dialog;
    bool editable = vm.Editable();

    SectionHeader("TC SCRIPT");
    ImGui::PushStyleColor(ImGuiCol_Text, mim::TEXT_MUTE);
    ImGui::TextWrapped("%s", vm.ScriptPath());
    ImGui::PopStyleColor();

    // 편집 칸이 들어갈 자리를 남기고 표를 그린다
    float sp = ImGui::GetStyle().ItemSpacing.y;
    float editH = ImGui::GetFrameHeightWithSpacing() * 3 + (ButtonHeight() + sp) * 2 + ImGui::GetTextLineHeightWithSpacing() * 2 + sp;
    float tableH = ImGui::GetContentRegionAvail().y - editH;
    if (tableH < ImGui::GetFrameHeight() * 4)
        tableH = ImGui::GetFrameHeight() * 4;

    int running_step = vm.ScriptStep();
    if (ImGui::BeginTable("##passscript", 5,
            ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit,
            ImVec2(ImGui::GetContentRegionAvail().x, tableH)))
    {
        ImGui::TableSetupColumn("#",     ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("00").x);
        ImGui::TableSetupColumn("NAME",  ImGuiTableColumnFlags_WidthStretch, 1.4f);
        ImGui::TableSetupColumn("x",     ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("00").x);
        ImGui::TableSetupColumn("WAIT",  ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("WAIT").x);
        ImGui::TableSetupColumn("BYTES", ImGuiTableColumnFlags_WidthStretch, 1.0f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        for (int i = 0; i < vm.StepCount(); i++)
        {
            mim::vm::AutoPilotVM::StepRow r = vm.Row(i);
            ImGui::TableNextRow();
            if (i == running_step)
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, mim::U32(mim::Alpha(mim::OK, 0.25f)));

            ImGui::TableNextColumn();
            ImGui::TextDisabled("%02d", i + 1);
            ImGui::TableNextColumn();
            char sel[96];
            snprintf(sel, sizeof(sel), "%s##step%d", r.name, i);
            if (ImGui::Selectable(sel, vm.SelectedStep() == i, ImGuiSelectableFlags_SpanAllColumns))
                vm.SelectStep(i);
            ImGui::TableNextColumn();
            ImGui::Text("%d", r.repeat);
            ImGui::TableNextColumn();
            ImGui::Text("%ds", r.delay_s);
            ImGui::TableNextColumn();
            ImGui::PushFont(mim::font_mono);
            ImGui::TextUnformatted(r.bytes);
            ImGui::PopFont();
        }
        ImGui::EndTable();
    }

    // ── 편집 (실행 중에는 잠근다)
    BeginLocked(!editable);
    const float label_em = 5.0f;
    RowLabel("Name", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##ps_name", vm.EditName(), vm.EditNameSize());
    RowLabel("Bytes", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::PushFont(mim::font_mono);
    ImGui::InputTextWithHint("##ps_bytes", "18 75 C0 00 00 01 14 47", vm.EditBytes(), vm.EditBytesSize(),
                             ImGuiInputTextFlags_CharsUppercase);
    ImGui::PopFont();
    RowLabel("Repeat", label_em);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.35f);
    ImGui::InputInt("##ps_rep", vm.EditRepeat());
    ImGui::SameLine();
    ImGui::TextUnformatted("Wait [s]");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##ps_delay", vm.EditDelay());

    float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 4) / 5.0f;
    ImVec2 bs(w, ButtonHeight());
    if (ImGui::Button("Add", bs))        vm.AddStep();
    ImGui::SameLine();
    if (ImGui::Button("Update", bs))     vm.ApplyEdit();
    ImGui::SameLine();
    if (ImGui::Button("Remove", bs))     vm.RemoveStep();
    ImGui::SameLine();
    if (ImGui::Button("Up", bs))         vm.MoveStep(-1);
    ImGui::SameLine();
    if (ImGui::Button("Down", bs))       vm.MoveStep(+1);

    float w3 = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f;
    ImVec2 bs3(w3, ButtonHeight());
    if (ImGui::Button("Load...", bs3))
        script_dialog.Open(vm.ScriptPath(), ".txt");
    ImGui::SameLine();
    if (ImGui::Button("Save", bs3))      vm.SaveScript();
    ImGui::SameLine();
    if (ImGui::Button("Default", bs3))   vm.ResetScript();
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Restore the built-in script (EPS HK, GND WDT reset)");
    EndLocked(!editable);

    if (!editable)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, mim::TEXT_MUTE);
        ImGui::TextUnformatted("Stop AutoPilot to edit the script.");
        ImGui::PopStyleColor();
    }
    else if (vm.Message()[0])
    {
        ImGui::PushStyleColor(ImGuiCol_Text, vm.MessageIsError() ? mim::ERR : mim::TEXT_DIM);
        ImGui::TextWrapped("%s", vm.Message());
        ImGui::PopStyleColor();
    }

    if (script_dialog.Draw("Open TC Script", vm.DialogPath(), vm.DialogPathSize()))
        vm.LoadScript(vm.DialogPath());
}
