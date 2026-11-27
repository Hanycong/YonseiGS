// View - 명령 생성기 입력 폼: UELYSYS payuel_aos (명령 번호 6401, 6400, 6402, 6403, 6404, 6405)
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

bool DrawUelysysPayuelAosForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 6401: { // payuel_aos RESET_COUNTERS
            static uint16_t msgid=0x1847; static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_6401",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_6401",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_6401")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        // ==================== UELYSYS payuel_lgbat ====================
        case 6400: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_AOS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_AOS_NOOP_CC);
            ImGui::InputScalar("msgid##direct_6400", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6400", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6400")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6402: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_AOS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_AOS_RESET_CC);
            ImGui::InputScalar("msgid##direct_6402", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6402", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6402")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6403: {
            UELYSYS_PAYUEL_AOS_WriteRegisterCmd_t &cmd = command5->uelysys_aos_writeregistercmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_AOS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_AOS_WRITE_REGISTER_CC);
            ImGui::InputScalar("msgid##direct_6403", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6403", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("reg_addr", ImGuiDataType_U8, &cmd.reg_addr);
            ImGui::InputScalar("data", ImGuiDataType_U16, &cmd.data);
            if (ImGui::Button("Generate CMD##direct_6403")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6404: {
            UELYSYS_PAYUEL_AOS_ReadRegisterCmd_t &cmd = command5->uelysys_aos_readregistercmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_AOS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_AOS_READ_REGISTER_CC);
            ImGui::InputScalar("msgid##direct_6404", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6404", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("reg_addr", ImGuiDataType_U8, &cmd.reg_addr);
            if (ImGui::Button("Generate CMD##direct_6404")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6405: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_AOS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC);
            ImGui::InputScalar("msgid##direct_6405", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6405", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6405")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }


    /********************************************5차 추가******************************************************/
    default:
        return false;
    }
    return true;
}
