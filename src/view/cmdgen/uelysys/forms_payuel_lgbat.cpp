// View - 명령 생성기 입력 폼: UELYSYS payuel_lgbat (명령 번호 6801, 6802, 6800, 6803, 6804)
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

bool DrawUelysysPayuelLgbatForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 6801: { // payuel_lgbat RESET_COUNTER
            static uint16_t msgid=0x18C6; static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_6801",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_6801",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_6801")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }
        case 6802: { // payuel_lgbat SEND_BCN
            static uint16_t msgid=0x18C6; static uint8_t fnccode=2;
            ImGui::InputScalar("msgid##direct_6802",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_6802",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_6802")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        // ==================== UELYSYS payuel_yonsei ====================
        case 6800: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_LGBAT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_LGBAT_NOOP_CC);
            ImGui::InputScalar("msgid##direct_6800", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6800", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6800")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6803: {
            UELYSYS_LGBAT_RequestDataCmd_t &cmd = command5->uelysys_lgbat_requestdatacmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_LGBAT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_LGBAT_REQUEST_DATA_CC);
            ImGui::InputScalar("msgid##direct_6803", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6803", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("DataID", ImGuiDataType_U8, &cmd.DataID);
            if (ImGui::Button("Generate CMD##direct_6803")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6804: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_LGBAT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_LGBAT_REQUEST_ALL_DATA_CC);
            ImGui::InputScalar("msgid##direct_6804", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6804", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6804")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

    default:
        return false;
    }
    return true;
}
