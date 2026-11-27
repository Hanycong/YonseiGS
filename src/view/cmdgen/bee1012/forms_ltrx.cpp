// View - 명령 생성기 입력 폼: BEE1012 ltrx (명령 번호 3400, 3401, 3402, 3403, 3410, 3411, 3412, 3440, 3441)
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

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1012LtrxForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 3400: { // LTRX NOOP
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_NOOP_CC;
            ImGui::InputScalar("msgid##direct_3400", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3400", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3400")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3401: { // LTRX RESET_COUNTERS
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_3401", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3401", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3401")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3402: { // LTRX RESET_APP_CMD_COUNTERS
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_RESET_APP_CMD_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_3402", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3402", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3402")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3403: { // LTRX RESET_DEVICE_CMD_COUNTERS
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_RESET_DEVICE_CMD_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_3403", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3403", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3403")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3410: { // LTRX SESSION_START_DOWNLINK
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_SESSION_START_DOWNLINK_CC;
            ImGui::InputScalar("msgid##direct_3410", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3410", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3410")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3411: { // LTRX SESSION_ABORT
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_SESSION_ABORT_CC;
            ImGui::InputScalar("msgid##direct_3411", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3411", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3411")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3412: { // LTRX SESSION_RESET_STATE
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_SESSION_RESET_STATE_CC;
            ImGui::InputScalar("msgid##direct_3412", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3412", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3412")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3440: { // LTRX DOWNSTREAM_ENABLE
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_DOWNSTREAM_ENABLE_CC;
            ImGui::InputScalar("msgid##direct_3440", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3440", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3440")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 3441: { // LTRX DOWNSTREAM_DISABLE
            static uint16_t msgid = LTRX_CMD_ID;
            static uint8_t fnccode = LTRX_DOWNSTREAM_DISABLE_CC;
            ImGui::InputScalar("msgid##direct_3441", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3441", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3441")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }



        // ==================== sch_lab ====================
    default:
        return false;
    }
    return true;
}
