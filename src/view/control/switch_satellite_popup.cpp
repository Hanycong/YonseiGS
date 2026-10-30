// View - Control 창 "Switch Satellite" 팝업: 대상 위성(HMAC 키) 전환
//
// control_view.cpp 에서 내용 그대로 옮긴 것.
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
#include "control_popups.h"

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern char AmpTimeBuf[32];
extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

static int current_sat_mode = 0; // 0: BEE1000, 1: BEE1012, 2: Uelysys
void HMACWindow()
{
    static int mode_arg;

    ImGui::Text("Select Satellite");

    for (int i = 0; i < 3; i++)
    {
        if (ImGui::RadioButton(mim::svc::SwitchSatelliteName(i), &current_sat_mode, i))
        {
            mode_arg = current_sat_mode;
            mim::svc::SwitchSatellite(&mode_arg);
        }
    }
}
