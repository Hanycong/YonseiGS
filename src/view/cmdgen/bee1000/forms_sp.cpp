// View - 명령 생성기 입력 폼: BEE1000 SP 앱 (명령 번호 164, 165, 166, 167)
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

#include "YonseiGS_config.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_model.h"
#include "YonseiGS_s_control.h"
#include "YonseiGS_ftp.h"
#include "YonseiGS_autopilot.h"

#include "YonseiGS_theme.h"
#include "YonseiGS_satcontext.h"
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

bool DrawBee1000SpForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 164: { // SP1 Deploy


            
            static uint16_t msgid   = SP_CMD_ID;
            static uint8_t  fnccode = SP_Deploy1_CC;
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

        
        case 165: { // SP2


            
            static uint16_t msgid   = SP_CMD_ID;
            static uint8_t  fnccode = SP_Deploy2_CC;
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

        
        case 166: { // SP3


            
            static uint16_t msgid   = SP_CMD_ID;
            static uint8_t  fnccode = SP_Deploy3_CC;
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

        
        case 167: { // SP4


            
            static uint16_t msgid   = SP_CMD_ID;
            static uint8_t  fnccode = SP_Deploy4_CC;
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
