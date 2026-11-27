// View - 명령 생성기 입력 폼: UELYSYS ttc (명령 번호 8829)
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

bool DrawUelysysTtcForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 8829: { // ttc START_BEACON_BURST (UELYSYS CC29)
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_START_BEACON_BURST_CC);
            ImGui::InputScalar("msgid##direct_8829", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_8829", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_8829")) {
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
