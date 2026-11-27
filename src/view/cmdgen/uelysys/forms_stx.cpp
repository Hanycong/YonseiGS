// View - 명령 생성기 입력 폼: UELYSYS stx (명령 번호 8460, 8461)
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

bool DrawUelysysStxForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 8460: { // stx FILESYS_CC_QUICKFORMAT (CC60)
            static uint16_t msgid=0x1854; static uint8_t fnccode=60;
            ImGui::InputScalar("msgid##direct_8460",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8460",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_8460")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }
        case 8461: { // stx FILESYS_CC_QUICKRESULT (CC61)
            static uint16_t msgid=0x1854; static uint8_t fnccode=61;
            ImGui::InputScalar("msgid##direct_8461",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8461",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_8461")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

    default:
        return false;
    }
    return true;
}
