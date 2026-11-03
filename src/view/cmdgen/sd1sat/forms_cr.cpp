// View - 명령 생성기 입력 폼: SD1-SAT cr (명령 번호 10100, 10101, 10102, 10103, 10104, 10105, 10106, 10107)
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

bool DrawSd1SatCrForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10100: { // cr NOOP
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10100", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10100", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10100")) {
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

        case 10101: { // cr RESET_COUNTERS
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10101", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10101", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10101")) {
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

        case 10102: { // cr RETRANSMIT_LATEST
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_RETRANSMIT_LATEST_CC;
            ImGui::InputScalar("msgid##direct_10102", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10102", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10102")) {
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

        case 10103: { // cr RETRANSMIT_APP
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_RETRANSMIT_APP_CC;
            static SD1SAT_CR_App_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10103", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10103", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CmdMsgId##direct_10103", ImGuiDataType_U16, &payload.CmdMsgId);
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

        case 10104: { // cr RETRANSMIT_APPCMD
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_RETRANSMIT_APPCMD_CC;
            static SD1SAT_CR_AppCmd_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10104", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10104", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CmdMsgId##direct_10104", ImGuiDataType_U16, &payload.CmdMsgId);
            ImGui::InputScalar("CmdCode##direct_10104", ImGuiDataType_U8, &payload.CmdCode);
            if (ImGui::Button("Generate CMD##direct_10104")) {
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

        case 10105: { // cr DUMP_QUEUE
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_DUMP_QUEUE_CC;
            static SD1SAT_CR_Dump_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10105", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10105", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("Path##direct_10105", payload.Path, sizeof(payload.Path));
            if (ImGui::Button("Generate CMD##direct_10105")) {
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

        case 10106: { // cr SET_DOWNLINK_MODE
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_SET_DOWNLINK_MODE_CC;
            static SD1SAT_CR_Mode_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10106", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10106", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Mode##direct_10106", ImGuiDataType_U8, &payload.Mode);
            if (ImGui::Button("Generate CMD##direct_10106")) {
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

        case 10107 : {
            static uint16_t msgid = SD1SAT_CR_CMD_ID;
            static uint8_t fnccode = SD1SAT_CR_GET_FULL_STATES_CC;
            ImGui::InputScalar("msgid##direct_10107", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10107", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10107")) {
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

        // ==================== SD1-SAT: ops (all commands no-arg) ====================
    default:
        return false;
    }
    return true;
}
