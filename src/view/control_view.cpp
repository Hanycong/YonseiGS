// View - Control 창 (Pass Operation 탭 왼쪽 열: 위성 전환, 핑, RParam, 명령 생성기 팝업, 캘리브레이션, AMP)
//
// ImGui 호출과 ControlVM 호출만 있다. 팝업 창 열림 여부(show_*)는 화면 상태라 여기 static 으로 둔다.
#include "imgui.h"
#include "control_vm.h"
#include "control_popups.h"
#include "cmdgen_view.h"
#include "control_view.h"

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

void ImGui_ControlWindow_Body(float fontscale)
{
    mim::vm::ControlVM & vm = mim::vm::ControlViewModel();

    ImGuiStyle& style = ImGui::GetStyle();
    float buttonHeight = ImGui::GetFontSize() * 1.5f;

    float rows = 3.0f;
    float pingRegionHeight =
        buttonHeight * rows
        + style.FramePadding.y * 2.5f
        + style.ItemSpacing.y * (rows + 2);

    ImGui::BeginChild("PingRegion",
                      ImVec2(ImGui::GetContentRegionAvail().x, pingRegionHeight),
                      true,
                      mim_winflags | ImGuiWindowFlags_NoScrollbar);

    float regionW = ImGui::GetContentRegionAvail().x;
    float eachW   = (regionW - style.ItemSpacing.x) * 0.5f; 

    if (ImGui::Button("Switch Satellite", ImVec2(eachW, buttonHeight)))
    {
        show_satellite_select = true;
    }

    if (ImGui::Button("Ping(AX100)", ImVec2(eachW, buttonHeight)))
    {
        vm.PingAx100();
    }

    ImGui::SameLine();
    if (ImGui::Button("Ping(OBC)", ImVec2(eachW, buttonHeight)))
    {
        vm.PingObc();
    }

    if (ImGui::Button("Ping each node", ImVec2(eachW, buttonHeight)))
    {
        show_pingWindow = true;
    }


    ImGui::SameLine();
    if (ImGui::Button("Custom RParam Get", ImVec2(eachW, buttonHeight)))
    {
        show_rparamWindow = true;
    }

    if (ImGui::Button("Send Telecommand", ImVec2(eachW, buttonHeight)))
    {
        show_tmtcWindow = true;
        vm.PrintRoutingTable();
    }

    ImGui::SameLine();
    if (ImGui::Button("Initial Ops", ImVec2(eachW, buttonHeight)))
    {
        show_initialControlWindow = true;
    }

    ImGui::EndChild();

    ImGui::SetWindowFontScale(1.0f);



    if (ImGui::Button("Baud Rate Calibration",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        vm.StartBaudCalibration();
    }

    if (ImGui::Button("Frequency Calibration",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        vm.StartFreqCalibration();
    }

    ImGui::SameLine();
    mim::vm::AmpButton amp = vm.Amp();
    if (!amp.on)
    {
        if (amp.timer)
        {
            if (ImGui::Button(amp.label,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                vm.TurnAmpOff();
            }
        }
        else
        {
            if (ImGui::Button(amp.label,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                vm.TurnAmpOn();
            }
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button(amp.label,
                          ImVec2(ImGui::GetContentRegionAvail().x,
                                 ImGui::GetFontSize() * 1.5f)))
        {
            vm.TurnAmpOff();
        }
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("CMD Manager",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        vm.OpenCommandManager();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear",
                      ImVec2(ImGui::GetContentRegionAvail().x,
                             ImGui::GetFontSize() * 1.5f)))
    {
        vm.ClearCommandQueue();
    }

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

