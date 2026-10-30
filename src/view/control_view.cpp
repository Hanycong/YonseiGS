// View - Control 창 (Pass Operation 탭 왼쪽 열: 위성 전환, 핑, RParam, 명령 생성기 팝업, 캘리브레이션, AMP)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것 (MVVM 1단계: View 추출).
// 다음 단계에서 State. / console / svc 호출 / 명령 큐(SatCMD)를 ControlVM 으로 옮긴다.
#include <inttypes.h>
#include <time.h>
#include <gs/param/rparam.h>
#include <csp/csp.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_csp.h"
#include "svc_tx.h"
#include "svc_radio.h"
#include "control_view.h"
#include "control_popups.h"

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern char AmpTimeBuf[32];
extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

// 명령 생성기 본체 (아직 miman_imgui.cpp 에 있다. Command 단계에서 옮긴다)
void DrawCmdGeneratorBody(bool initial_mode);

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
        mim::svc::WaitTx();
        mim::svc::StartPingConsole();
    }

    ImGui::SameLine();
    if (ImGui::Button("Ping(OBC)", ImVec2(eachW, buttonHeight)))
    {
        mim::svc::WaitTx();
        FSWTle* MIM_TLEinfo = (FSWTle*)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
        int NowPingInfo = GetNowTracking();

        if (NowPingInfo < -1)
        {
            console.AddLog("[ERROR]##You Tried Ping without Tracking. Please Check Again.");
        }
        else if (NowPingInfo == -1)
        {
            FSWTleConverter(State.Fatellites->tle, MIM_TLEinfo);
            packetsign* TestPacket = PingInit(MIM_TLEinfo);
            mim::svc::SendUplink((void*)TestPacket);
        }
        else
        {
            FSWTleConverter(State.Satellites[NowPingInfo]->tle, MIM_TLEinfo);
            packetsign* TestPacket = PingInit(MIM_TLEinfo);
            mim::svc::SendUplink((void*)TestPacket);
        }
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
        csp_rtable_print();
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
        mim::svc::StartBaudCalibration();
    }

    if (ImGui::Button("Frequency Calibration",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        mim::svc::StartFreqCalibration();
    }

    ImGui::SameLine();
    if (!State.AMPON)
    {
        if (State.AmpTime > 0)
        {
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "Disabled  %M:%S", State.AmpTM);
            if (ImGui::Button(AmpTimeBuf,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                mim::svc::AmpOff();
            }
        }
        else
        {
            sprintf(AmpTimeBuf, "AMP ON");
            if (ImGui::Button(AmpTimeBuf,
                              ImVec2(ImGui::GetContentRegionAvail().x,
                                     ImGui::GetFontSize() * 1.5f)))
            {
                mim::svc::AmpOn();
            }
        }
    }
    else
    {
        if (State.AmpTime > 0)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "AMP OFF %M:%S", State.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP OFF");

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button(AmpTimeBuf,
                          ImVec2(ImGui::GetContentRegionAvail().x,
                                 ImGui::GetFontSize() * 1.5f)))
        {
            mim::svc::AmpOff();
        }
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("CMD Manager",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.33f,
                             ImGui::GetFontSize() * 1.5f)))
    {
        State.Display_CMD = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear",
                      ImVec2(ImGui::GetContentRegionAvail().x,
                             ImGui::GetFontSize() * 1.5f)))
    {
        NowCMD = 0;
        for (int i = 0; i < 256; i++)
        {
            if (SatCMD[i] != NULL)
            {
                delete SatCMD[i];
                SatCMD[i] = NULL;
            }
        }
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

