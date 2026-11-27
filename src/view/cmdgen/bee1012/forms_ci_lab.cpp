// View - 명령 생성기 입력 폼: BEE1012 ci_lab (명령 번호 1600, 1601, 1602)
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

bool DrawBee1012CiLabForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 1600: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_CI_LAB_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_CI_LAB_NOOP_CC);
            ImGui::InputScalar("msgid##direct_1600", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1600", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1600")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_NoArgCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1601: { // CI_LAB RESET_COUNTERS
            static uint16_t msgid = BEE1012_CI_LAB_CMD_ID;
            static uint8_t fnccode = BEE1012_CI_LAB_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_1601", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1601", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1601")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1602: { // CI_LAB CREATE_CHILD_TASK
            static uint16_t msgid = BEE1012_CI_LAB_CMD_ID;
            static uint8_t fnccode = BEE1012_CI_LAB_CREATE_CHILD_TASK_CC;
            ImGui::InputScalar("msgid##direct_1602", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1602", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1602")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }


        // ==================== ds ====================
    default:
        return false;
    }
    return true;
}
