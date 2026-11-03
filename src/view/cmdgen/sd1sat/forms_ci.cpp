// View - 명령 생성기 입력 폼: SD1-SAT ci (명령 번호 10000, 10001, 10002, 10003, 10004, 10005, 10006)
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

bool DrawSd1SatCiForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10000: { // ci NOOP
            static uint16_t msgid = SD1SAT_CI_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10000", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10000", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10000")) {
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

        case 10001: { // ci RESET_COUNTERS
            static uint16_t msgid = SD1SAT_CI_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10001", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10001", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10001")) {
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

        case 10002: { // ci GATE_NOOP
            static uint16_t msgid = SD1SAT_CI_GATE_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_GATE_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10002", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10002", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10002")) {
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

        case 10003: { // ci GATE_RESET_INGEST
            static uint16_t msgid = SD1SAT_CI_GATE_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_GATE_RESET_INGEST_CC;
            ImGui::InputScalar("msgid##direct_10003", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10003", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10003")) {
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

        case 10004: { // ci GATE_SET_UPLINK
            static uint16_t msgid = SD1SAT_CI_GATE_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_GATE_SET_UPLINK_CC;
            static SD1SAT_CI_Enable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10004", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10004", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Enable##direct_10004", ImGuiDataType_U8, &payload.Enable);
            if (ImGui::Button("Generate CMD##direct_10004")) {
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

        case 10005: { // ci GATE_SET_CHECKSUM
            static uint16_t msgid = SD1SAT_CI_GATE_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_GATE_SET_CHECKSUM_CC;
            static SD1SAT_CI_Enable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10005", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10005", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Enable##direct_10005", ImGuiDataType_U8, &payload.Enable);
            if (ImGui::Button("Generate CMD##direct_10005")) {
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

        case 10006: { // ci GATE_SET_FRAMING
            static uint16_t msgid = SD1SAT_CI_GATE_CMD_ID;
            static uint8_t fnccode = SD1SAT_CI_GATE_SET_FRAMING_CC;
            static SD1SAT_CI_Framing_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10006", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10006", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Enable##direct_10006", ImGuiDataType_U32, &payload.Magic);
            if (ImGui::Button("Generate CMD##direct_10006")) {
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

        // ==================== SD1-SAT: cr ====================
    default:
        return false;
    }
    return true;
}
