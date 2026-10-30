// View - 명령 생성기 입력 폼: COSMIC SP 앱 (명령 번호 212)
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

bool DrawCosmicSpForms(uint16_t fnccode)
{
    switch (fnccode)
    {
case 212: { // SP DEPLOY (CC 2)
    static uint16_t msgid   = 6232;
    static uint8_t  fnccode = 2;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::Separator();
    ImGui::Text("SP DEPLOY Payload");

    ImGui::InputScalar("SP u8",     ImGuiDataType_U8, &command->spdeploy.SP);
    ImGui::InputScalar("deploy u8", ImGuiDataType_U8, &command->spdeploy.deploy);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        uint16_t mid = htons(msgid);
        uint8_t  seq[2] = {0xC0, 0x00};
        uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SP_DEPLOY) - 7)};

        memcpy(command->spdeploy.CmdHeader,     &mid, 2);
        memcpy(command->spdeploy.CmdHeader + 2, seq,  2);
        memcpy(command->spdeploy.CmdHeader + 4, len,  2);
        memcpy(command->spdeploy.CmdHeader + 6, &fnccode, 1);

        command->spdeploy.CmdHeader[7] = 0;
        uint16_t total = sizeof(SP_DEPLOY);
        const uint8_t *p = (const uint8_t *)&command->spdeploy;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(p++);
        command->spdeploy.CmdHeader[7] = crc;

        packetsign *pkt =
            (packetsign *)malloc(2 + 2 + 4 + sizeof(SP_DEPLOY));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(SP_DEPLOY);
        memcpy(pkt->Data, &command->spdeploy, sizeof(SP_DEPLOY));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->spdeploy.CmdHeader[0],
                command->spdeploy.CmdHeader[1],
                command->spdeploy.CmdHeader[2],
                command->spdeploy.CmdHeader[3],
                command->spdeploy.CmdHeader[4],
                command->spdeploy.CmdHeader[5],
                command->spdeploy.CmdHeader[6],
                command->spdeploy.CmdHeader[7]);

    ImGui::Text("Payload: SP=%u deploy=%u",
                command->spdeploy.SP,
                command->spdeploy.deploy);
    break;
}

    default:
        return false;
    }
    return true;
}
