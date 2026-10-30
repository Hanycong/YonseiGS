// View - 명령 생성기 입력 폼: BEE1000 UNKNOWN 앱 (명령 번호 205)
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

bool DrawBee1000UnknownForms(uint16_t fnccode)
{
    switch (fnccode)
    {
case 205: {
    static uint16_t msgid   = 0x1819;
    static uint8_t  fnccode = 0;
    static uint32_t payload = 0x0000081A;  
    static char     last_cmd_hex[128] = "";

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);
    ImGui::InputScalar("arg (uint32 LE)", ImGuiDataType_U32, &payload);

    if (ImGui::Button("Generate CMD"))
    {
        WriteSystemName(msgid);
        mim::svc::WaitTx();


        uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(uint32_t)] = {0};


        uint16_t mid_be = htons(msgid);
        memcpy(cmd, &mid_be, sizeof(uint16_t));   

        cmd[2] = 0xC0;
        cmd[3] = 0x00;


        uint16_t ccsds_len = htons(sizeof(cmd) - 7); 
        memcpy(cmd + 4, &ccsds_len, sizeof(uint16_t)); 

        // CC
        cmd[6] = fnccode;   // 0


        cmd[7] = 0x00;


        cmd[8]  = (uint8_t)( payload        & 0xFF);
        cmd[9]  = (uint8_t)((payload >> 8 ) & 0xFF);
        cmd[10] = (uint8_t)((payload >> 16) & 0xFF);
        cmd[11] = (uint8_t)((payload >> 24) & 0xFF);

        uint8_t checksum = 0xFF;
        for (size_t i = 0; i < sizeof(cmd); ++i)
            checksum ^= cmd[i];
        cmd[7] = checksum;


        char *ptr    = last_cmd_hex;
        size_t remain = sizeof(last_cmd_hex);
        int written = snprintf(
            ptr, remain,
            "%02X %02X %02X %02X %02X %02X %02X %02X\n"
            "%02X %02X %02X %02X",
            cmd[0], cmd[1], cmd[2], cmd[3],
            cmd[4], cmd[5], cmd[6], cmd[7],
            cmd[8], cmd[9], cmd[10], cmd[11]
        );
        (void)written; 


        packetsign* TestPacket =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd));
        TestPacket->Identifier = HVD_TEST;
        TestPacket->PacketType = MIM_PT_TMTC_TEST;
        TestPacket->Length     = sizeof(cmd);   
        memcpy(TestPacket->Data, cmd, sizeof(cmd));

        mim::svc::SendUplink((void*)TestPacket);
    }

    ImGui::Text("Last CMD:");
    ImGui::TextUnformatted(last_cmd_hex);
    break;
}


// UTRX GNDWDT clear

    default:
        return false;
    }
    return true;
}
