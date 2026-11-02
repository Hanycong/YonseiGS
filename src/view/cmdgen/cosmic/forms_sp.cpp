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

        mim::tc::EncodeCfeCommand(&command->spdeploy, sizeof(SP_DEPLOY), msgid, fnccode);
        mim::tc::SendTelecommand(&command->spdeploy, sizeof(SP_DEPLOY));
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
