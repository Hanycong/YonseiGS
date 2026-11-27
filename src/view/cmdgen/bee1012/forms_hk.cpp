// View - 명령 생성기 입력 폼: BEE1012 hk (명령 번호 1900, 1901)
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

bool DrawBee1012HkForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 1900: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_HK_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_HK_NOOP_CC);
            ImGui::InputScalar("msgid##direct_1900", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1900", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1900")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_NoArgCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 1901: { // HK RESET_COUNTERS
            static uint16_t msgid = BEE1012_HK_CMD_ID;
            static uint8_t fnccode = BEE1012_HK_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_1901", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_1901", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_1901")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }


        // ==================== to_lab ====================
    default:
        return false;
    }
    return true;
}
