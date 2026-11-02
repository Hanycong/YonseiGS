// View - 명령 생성기 입력 폼: COSMIC EPS 앱 (명령 번호 209, 210, 211)
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

bool DrawCosmicEpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
case 209: { // EPS P31U SET CONFIG2 (CC 54)
    static uint16_t msgid   = 6257;
    static uint8_t  fnccode = 54;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::Separator();
    ImGui::Text("EPS P31U SET CONFIG2 Payload");

    ImGui::InputScalar("batt_maxvoltage u16",      ImGuiDataType_U16, &command->epsp31usetconfig2.batt_maxvoltage);
    ImGui::InputScalar("batt_safevoltage u16",     ImGuiDataType_U16, &command->epsp31usetconfig2.batt_safevoltage);
    ImGui::InputScalar("batt_criticalvoltage u16", ImGuiDataType_U16, &command->epsp31usetconfig2.batt_criticalvoltage);
    ImGui::InputScalar("batt_normalvoltage u16",   ImGuiDataType_U16, &command->epsp31usetconfig2.batt_normalvoltage);

    ImGui::InputScalar("reserved1_0 u32", ImGuiDataType_U32, &command->epsp31usetconfig2.reserved1_0);
    ImGui::InputScalar("reserved1_1 u32", ImGuiDataType_U32, &command->epsp31usetconfig2.reserved1_1);

    ImGui::InputScalar("reserved2_0 u8", ImGuiDataType_U8, &command->epsp31usetconfig2.reserved2_0);
    ImGui::InputScalar("reserved2_1 u8", ImGuiDataType_U8, &command->epsp31usetconfig2.reserved2_1);
    ImGui::InputScalar("reserved2_2 u8", ImGuiDataType_U8, &command->epsp31usetconfig2.reserved2_2);
    ImGui::InputScalar("reserved2_3 u8", ImGuiDataType_U8, &command->epsp31usetconfig2.reserved2_3);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        mim::tc::EncodeCfeCommand(&command->epsp31usetconfig2, sizeof(EPS_P31U_SETCONFIG2), msgid, fnccode);
        mim::tc::SendTelecommand(&command->epsp31usetconfig2, sizeof(EPS_P31U_SETCONFIG2));
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->epsp31usetconfig2.CmdHeader[0],
                command->epsp31usetconfig2.CmdHeader[1],
                command->epsp31usetconfig2.CmdHeader[2],
                command->epsp31usetconfig2.CmdHeader[3],
                command->epsp31usetconfig2.CmdHeader[4],
                command->epsp31usetconfig2.CmdHeader[5],
                command->epsp31usetconfig2.CmdHeader[6],
                command->epsp31usetconfig2.CmdHeader[7]);

    ImGui::Text("Payload: max=%u safe=%u critical=%u normal=%u r1_0=%u r1_1=%u r2={%u,%u,%u,%u}",
                command->epsp31usetconfig2.batt_maxvoltage,
                command->epsp31usetconfig2.batt_safevoltage,
                command->epsp31usetconfig2.batt_criticalvoltage,
                command->epsp31usetconfig2.batt_normalvoltage,
                command->epsp31usetconfig2.reserved1_0,
                command->epsp31usetconfig2.reserved1_1,
                command->epsp31usetconfig2.reserved2_0,
                command->epsp31usetconfig2.reserved2_1,
                command->epsp31usetconfig2.reserved2_2,
                command->epsp31usetconfig2.reserved2_3);
    break;
}

case 210: { // EPS P31U CONFIG2 (CC 55)
    static uint16_t msgid   = 6257;
    static uint8_t  fnccode = 55;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::Separator();
    ImGui::Text("EPS P31U CONFIG2 Payload");
    ImGui::InputScalar("cmd u8", ImGuiDataType_U8, &command->epsp31uconfig2.cmd);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        mim::tc::EncodeCfeCommand(&command->epsp31uconfig2, sizeof(EPS_P31U_CONFIG2), msgid, fnccode);
        mim::tc::SendTelecommand(&command->epsp31uconfig2, sizeof(EPS_P31U_CONFIG2));
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->epsp31uconfig2.CmdHeader[0],
                command->epsp31uconfig2.CmdHeader[1],
                command->epsp31uconfig2.CmdHeader[2],
                command->epsp31uconfig2.CmdHeader[3],
                command->epsp31uconfig2.CmdHeader[4],
                command->epsp31uconfig2.CmdHeader[5],
                command->epsp31uconfig2.CmdHeader[6],
                command->epsp31uconfig2.CmdHeader[7]);

    ImGui::Text("Payload: cmd=%u", command->epsp31uconfig2.cmd);
    break;
}

case 211: { // EPS P31U SET OUT SINGLE (CC 10)
    static uint16_t msgid   = 6257;
    static uint8_t  fnccode = 10;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::Separator();
    ImGui::Text("EPS P31U SET OUT SINGLE Payload");

    ImGui::InputScalar("channel u8", ImGuiDataType_U8,  &command->epsp31usetoutsingle.channel);
    ImGui::InputScalar("value u8",   ImGuiDataType_U8,  &command->epsp31usetoutsingle.value);
    ImGui::InputScalar("delay u16",  ImGuiDataType_U16, &command->epsp31usetoutsingle.delay);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        mim::tc::EncodeCfeCommand(&command->epsp31usetoutsingle, sizeof(EPS_P31U_SET_OUT_SINGLE), msgid, fnccode);
        mim::tc::SendTelecommand(&command->epsp31usetoutsingle, sizeof(EPS_P31U_SET_OUT_SINGLE));
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->epsp31usetoutsingle.CmdHeader[0],
                command->epsp31usetoutsingle.CmdHeader[1],
                command->epsp31usetoutsingle.CmdHeader[2],
                command->epsp31usetoutsingle.CmdHeader[3],
                command->epsp31usetoutsingle.CmdHeader[4],
                command->epsp31usetoutsingle.CmdHeader[5],
                command->epsp31usetoutsingle.CmdHeader[6],
                command->epsp31usetoutsingle.CmdHeader[7]);

    ImGui::Text("Payload: channel=%u value=%u delay=%u",
                command->epsp31usetoutsingle.channel,
                command->epsp31usetoutsingle.value,
                command->epsp31usetoutsingle.delay);
    break;
}

    default:
        return false;
    }
    return true;
}
