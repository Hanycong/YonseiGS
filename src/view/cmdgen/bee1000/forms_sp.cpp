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
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + 8);
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = 8;
            memset(TestPacket->Data, 0, TestPacket->Length);

            uint8_t cmd[8] = {0,};
            msgid = htons(msgid);
            memcpy(cmd, &msgid, sizeof(uint16_t));
            memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
            cmd[2] = 0xc0;
            cmd[3] = 0x00;
            cmd[4] = 0x00;
            cmd[5] = 0x01;

            uint16_t len = 8;
            const uint8_t* byteptr = cmd;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd[7] = checksum;

            memcpy(TestPacket->Data, cmd, sizeof(cmd));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            
            msgid = htons(msgid);
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
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + 8);
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = 8;
            memset(TestPacket->Data, 0, TestPacket->Length);

            uint8_t cmd[8] = {0,};
            msgid = htons(msgid);
            memcpy(cmd, &msgid, sizeof(uint16_t));
            memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
            cmd[2] = 0xc0;
            cmd[3] = 0x00;
            cmd[4] = 0x00;
            cmd[5] = 0x01;

            uint16_t len = 8;
            const uint8_t* byteptr = cmd;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd[7] = checksum;

            memcpy(TestPacket->Data, cmd, sizeof(cmd));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            
            msgid = htons(msgid);
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
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + 8);
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = 8;
            memset(TestPacket->Data, 0, TestPacket->Length);

            uint8_t cmd[8] = {0,};
            msgid = htons(msgid);
            memcpy(cmd, &msgid, sizeof(uint16_t));
            memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
            cmd[2] = 0xc0;
            cmd[3] = 0x00;
            cmd[4] = 0x00;
            cmd[5] = 0x01;

            uint16_t len = 8;
            const uint8_t* byteptr = cmd;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd[7] = checksum;

            memcpy(TestPacket->Data, cmd, sizeof(cmd));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            
            msgid = htons(msgid);
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
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + 8);
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = 8;
            memset(TestPacket->Data, 0, TestPacket->Length);

            uint8_t cmd[8] = {0,};
            msgid = htons(msgid);
            memcpy(cmd, &msgid, sizeof(uint16_t));
            memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
            cmd[2] = 0xc0;
            cmd[3] = 0x00;
            cmd[4] = 0x00;
            cmd[5] = 0x01;

            uint16_t len = 8;
            const uint8_t* byteptr = cmd;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd[7] = checksum;

            memcpy(TestPacket->Data, cmd, sizeof(cmd));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            
            msgid = htons(msgid);
    }


            break;
        }

    default:
        return false;
    }
    return true;
}
