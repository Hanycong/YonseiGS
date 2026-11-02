// View - 명령 생성기 입력 폼: COSMIC UEL_PI 앱 (명령 번호 214, 215)
//
// cmdgen_view.cpp 의 명령 번호 switch 에서 이 앱의 case 를 내용 그대로 옮긴 것.
// 처리한 명령이면 true.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include "ccsds_cmd.h"
#include "telecommand_sender.h"
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawCosmicUelPiForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 214: { //  COSMIC UEL Pi ON
        static uint16_t msgid   = 6277;
        static uint8_t  fnccode = 8;
        ImGui::Text("MsgID   : 0x%04X", msgid);
        ImGui::Text("FncCode : %u",     fnccode);


        if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint8_t cmd[8] = {0,};

                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode);

                for (int i = 0; i < sizeof(cmd); i++)
                    printf("0x%x ", cmd[i]);
                printf("\n");

                mim::tc::SendTelecommand(cmd, sizeof(cmd));
                
            }
            break;
        }
    case 215: { //  COSMIC UEL Pi OFF
        static uint16_t msgid   = 6277;
        static uint8_t  fnccode = 9;
        ImGui::Text("MsgID   : 0x%04X", msgid);
        ImGui::Text("FncCode : %u",     fnccode);


        if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint8_t cmd[8] = {0,};

                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode);

                for (int i = 0; i < sizeof(cmd); i++)
                    printf("0x%x ", cmd[i]);
                printf("\n");

                mim::tc::SendTelecommand(cmd, sizeof(cmd));
                
            }
            break;
        }

    default:
        return false;
    }
    return true;
}
