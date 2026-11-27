// View - 명령 생성기 입력 폼: SD1-SAT uant (명령 번호 10600, 10601, 10602, 10603, 10604, 10605, 10606, 10607, 10608)
//
// 누리호 5차 GS(GS_nuri5) DrawCmdGeneratorBody 의 case 를 내용 그대로 옮긴 것.
// 송신은 svc_tx(WaitTx/SendUplink), TC 패킷 기록은 업링크 스레드가 한다. 처리한 명령이면 true.
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <string.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "YonseiGS_config.h"
#include "mission_defs.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"
#include "telecommand_sender.h"
#include "ccsds_cmd.h"
#include "sat_mode.h"
#include "cmdgen5_common.h"
#include "cmdgen_forms.h"
#include "noarg_form.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

// 인자 없는 명령 (머리 8바이트뿐): { 명령 번호, 기본 MID, 기본 CC, 위젯 ID 꼬리표 }. 폼은 noarg_form 이 그린다
static mim::ui::NoArgCommandForm kNoArgForms[] = {
    { 10600, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_NOOP_CC,           "##direct_10600" },   // uant NOOP
    { 10601, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_RESET_COUNTERS_CC, "##direct_10601" },   // uant RESET_COUNTERS
    { 10603, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_ABORT_CC,          "##direct_10603" },   // uant ABORT
    { 10604, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_DEPLOY_NORMAL_CC,  "##direct_10604" },   // uant DEPLOY_NORMAL
    { 10605, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_DEPLOY_HARD_CC,    "##direct_10605" },   // uant DEPLOY_HARD
    { 10606, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_READ_RAW_CC,       "##direct_10606" },   // uant READ_RAW
    { 10608, SD1SAT_UANT_CMD_ID, SD1SAT_UANT_DEPLOY_AUTO_CC,    "##direct_10608" },   // uant DEPLOY_AUTO
};

bool DrawSd1SatUantForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 10602: { // uant DEPLOY
            static uint16_t msgid = SD1SAT_UANT_CMD_ID;
            static uint8_t fnccode = SD1SAT_UANT_DEPLOY_CC;
            static SD1SAT_UANT_Deploy_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10602", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10602", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Algorithm##direct_10602", ImGuiDataType_U8, &payload.Algorithm);
            ImGui::InputScalar("Antennas (bitmask)##direct_10602", ImGuiDataType_U8, &payload.Antennas);
            if (ImGui::Button("Generate CMD##direct_10602")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)] = {0};
                memcpy(cmd + 8, &payload, sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 10607: { // uant WRITE_RAW
            static uint16_t msgid = SD1SAT_UANT_CMD_ID;
            static uint8_t fnccode = SD1SAT_UANT_WRITE_RAW_CC;
            static SD1SAT_UANT_WriteRaw_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10607", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10607", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Command##direct_10607", ImGuiDataType_U8, &payload.Command);
            if (ImGui::Button("Generate CMD##direct_10607")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)] = {0};
                memcpy(cmd + 8, &payload, sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        // ==================== SD1-SAT: utrx ====================
    default:
        return false;
    }
    return true;
}
