// View - 명령 생성기 입력 폼: BEE1012 rpt (명령 번호 2100, 2101, 2102, 2103, 2104)
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

bool DrawBee1012RptForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 2100: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_RPT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_RPT_NOOP_CC);
            ImGui::InputScalar("msgid##direct_2100", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2100", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2100")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_NoArgCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 2101: { // RPT RESET_COUNTER
            static uint16_t msgid = BEE1012_RPT_CMD_ID;
            static uint8_t fnccode = BEE1012_RPT_RESET_COUNTER_CC;
            ImGui::InputScalar("msgid##direct_2101", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2101", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2101")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 2102: { // rpt REPORT
            static uint16_t msgid=0x1825; static uint8_t fnccode=2;
            struct Payload { uint8_t StartIdx; uint8_t TotalNumber; uint8_t IsCritical; }; static_assert(sizeof(Payload)==3,"rpt REPORT payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2102",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2102",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("StartIdx##direct_2102",ImGuiDataType_U8,&payload.StartIdx);
            ImGui::InputScalar("TotalNumber##direct_2102",ImGuiDataType_U8,&payload.TotalNumber);
            ImGui::InputScalar("IsCritical##direct_2102",ImGuiDataType_U8,&payload.IsCritical);
            if(ImGui::Button("Generate CMD##direct_2102")){ WriteSystemName(msgid); uint8_t cmd[11]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2103: { // rpt CLEAR_QUEUE
            static uint16_t msgid=0x1825; static uint8_t fnccode=3;
            struct Payload { uint8_t IsCritical; }; static_assert(sizeof(Payload)==1,"rpt CLEAR_QUEUE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_2103",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2103",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("IsCritical##direct_2103",ImGuiDataType_U8,&payload.IsCritical);
            if(ImGui::Button("Generate CMD##direct_2103")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2104: { // RPT GET_OPS_DATA
            static uint16_t msgid = BEE1012_RPT_CMD_ID;
            static uint8_t fnccode = BEE1012_RPT_GET_OPS_DATA_CC;
            ImGui::InputScalar("msgid##direct_2104", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2104", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2104")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }


        // ==================== gpio ====================
    default:
        return false;
    }
    return true;
}
