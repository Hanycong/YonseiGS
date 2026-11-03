// View - Control 창 (Pass Operation 탭 왼쪽 열: 위성 전환, 핑, RParam, 명령 생성기 팝업, 캘리브레이션, AMP)
//
// ImGui 호출과 ControlVM 호출만 있다. 팝업 창 열림 여부(show_*)는 화면 상태라 여기 static 으로 둔다.
#include "imgui.h"
#include "control_vm.h"
#include "control_popups.h"
#include "cmdgen_view.h"
#include "control_view.h"
#include "ui_kit.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

static bool show_rparamWindow         = false;      // rparam 가져오기 custom
static bool show_pingWindow           = false;      // ping 난사
static bool show_tmtcWindow           = false;      // 초기관제
static bool show_initialControlWindow = false;      // 초기관제
static bool show_satellite_select = false;

void ImGui_ControlWindow(float fontscale)
{
    ImGui::Begin("Control GUI", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_ControlWindow_Body(fontscale);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

using namespace mim::ui;

void ImGui_ControlWindow_Body(float fontscale)
{
    mim::vm::ControlVM & vm = mim::vm::ControlViewModel();

    SectionHeader("SATELLITE");
    if (WideButton("Switch Satellite"))
        show_satellite_select = true;

    SectionHeader("LINK TEST");
    if (GridButton("Ping(AX100)", 0))
        vm.PingAx100();
    if (GridButton("Ping(OBC)", 1))
        vm.PingObc();
    if (GridButton("Ping each node", 0))
        show_pingWindow = true;
    if (GridButton("Custom RParam Get", 1))
        show_rparamWindow = true;

    SectionHeader("COMMAND");
    if (GridButton("Send Telecommand", 0))
    {
        show_tmtcWindow = true;
        vm.PrintRoutingTable();
    }
    if (GridButton("Initial Ops", 1))
        show_initialControlWindow = true;
    if (GridButton("CMD Manager", 0))
        vm.OpenCommandManager();
    if (GridButton("Clear CMD Queue", 1))
        vm.ClearCommandQueue();

    SectionHeader("CALIBRATION");
    if (GridButton("Baud Rate", 0))
        vm.StartBaudCalibration();
    if (GridButton("Frequency", 1))
        vm.StartFreqCalibration();

    SectionHeader("AMPLIFIER");
    mim::vm::AmpButton amp = vm.Amp();
    if (ToggleWideButton(amp.label, amp.on))
        vm.PressAmp(amp);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 popup_size(600.0f, 380.0f);
    ImVec2 popup_pos((io.DisplaySize.x - popup_size.x) * 0.5f,
                     (io.DisplaySize.y - popup_size.y) * 0.5f);

    if (show_tmtcWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("TMTC Command Test##msgid_fnccode_TypeWindow", &show_tmtcWindow);
        DrawCmdGeneratorBody(false);
        ImGui::End();
    }

    if (show_initialControlWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Initial Operations##INITIAL_CMD_WINDOW", &show_initialControlWindow);
        DrawCmdGeneratorBody(true);
        ImGui::End();
    }

    if (show_pingWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Ping each node##PING_WINDOW", &show_pingWindow);
        PingWindow();
        ImGui::End();
    }

    if (show_rparamWindow)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Get Custom Rparam##RPARAM_WINDOW", &show_rparamWindow);
        RparamWindow();
        ImGui::End();
    }

    if (show_satellite_select)
    {
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_FirstUseEver);
        ImGui::Begin("Switch HMAC Key##SATELLITE_SELECT", &show_satellite_select);
        HMACWindow();
        ImGui::End();
    }
}
