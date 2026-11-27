// View - 명령 생성기 입력 폼: SD1-SAT ops (명령 번호 10300, 10301, 10302, 10303, 10304, 10305)
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
    { 10300, SD1SAT_OPS_CMD_ID, SD1SAT_OPS_NOOP_CC,           "##direct_10300" },   // ops NOOP
    { 10301, SD1SAT_OPS_CMD_ID, SD1SAT_OPS_RESET_COUNTERS_CC, "##direct_10301" },   // ops RESET_COUNTERS
    { 10302, SD1SAT_OPS_CMD_ID, SD1SAT_OPS_GET_RECORD_CC,     "##direct_10302" },   // ops GET_RECORD
    { 10303, SD1SAT_OPS_CMD_ID, SD1SAT_OPS_DEPLOY_INHIBIT_CC, "##direct_10303" },   // ops DEPLOY_INHIBIT
    { 10304, SD1SAT_OPS_CMD_ID, SD1SAT_OPS_DEPLOY_ENABLE_CC,  "##direct_10304" },   // ops DEPLOY_ENABLE
};

bool DrawSd1SatOpsForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 10305: {// ops Set Seconds Since First Boot
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_SET_SECONDS_SINCE_FIRST_BOOT_CC;
            static OPS_SetSecondsSinceFirstBootCmd_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10103", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10103", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("SecondsSinceFirstBoot##direct_10103", ImGuiDataType_U32, &payload.SecondsSinceFirstBoot);
            if (ImGui::Button("Generate CMD##direct_10103")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)] = {0};
                memcpy(cmd + 8, &payload, sizeof(payload));
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        // ==================== SD1SAT-ttc ====================
    default:
        return false;
    }
    return true;
}
