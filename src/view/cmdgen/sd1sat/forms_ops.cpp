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

bool DrawSd1SatOpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10300: { // ops NOOP
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10300")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode; cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 10301: { // ops RESET_COUNTERS
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10301", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10301", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10301")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode; cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 10302: { // ops GET_RECORD
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_GET_RECORD_CC;
            ImGui::InputScalar("msgid##direct_10302", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10302", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10302")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode; cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 10303: { // ops DEPLOY_INHIBIT
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_DEPLOY_INHIBIT_CC;
            ImGui::InputScalar("msgid##direct_10303", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10303", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10303")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode; cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 10304: { // ops DEPLOY_ENABLE
            static uint16_t msgid = SD1SAT_OPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_OPS_DEPLOY_ENABLE_CC;
            ImGui::InputScalar("msgid##direct_10304", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10304", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10304")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode; cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

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
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // ==================== SD1SAT-ttc ====================
    default:
        return false;
    }
    return true;
}
