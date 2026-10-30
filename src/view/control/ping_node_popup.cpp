// View - Control 창 "Ping each node" 팝업: 지정한 CSP 노드에 핑
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

void PingWindow() {
    static uint8_t node = 0;

    ImGui::InputScalar("node", ImGuiDataType_U8, &node);

    if (ImGui::Button("Send Ping")) {

        mim::svc::WaitTx();

        uint8_t *arg = (uint8_t*)malloc(sizeof(uint8_t));
        *arg = node; 

        mim::svc::StartPingEveryNode((void*)arg);
    }
}
