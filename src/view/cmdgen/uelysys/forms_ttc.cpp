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
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

    default:
        return false;
    }
    return true;
}
