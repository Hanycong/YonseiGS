// View - 명령 생성기 입력 폼: SD1-SAT utrx (명령 번호 10700, 10701, 10710, 10711, 10712, 10713, 10714, 10715, 10716, 10717, 10720, 10721, 10722, 10723, 10724, 10725, 10726, 10727, 10728, 10730, 10731)
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

bool DrawSd1SatUtrxForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10700: { // utrx NOOP
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10700", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10700", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10700")) {
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

        case 10701: { // utrx RESET_COUNTERS
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10701", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10701", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10701")) {
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

        case 10710: { // utrx AX100_PING
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_PING_CC;
            ImGui::InputScalar("msgid##direct_10710", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10710", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10710")) {
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

        case 10711: { //SD1SAT_UTRX_AX100_GNDWDT_CLEAR_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_GNDWDT_CLEAR_CC;
            ImGui::InputScalar("msgid##direct_10711", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10711", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10711")) {
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

        case 10712: { // SD1SAT_UTRX_AX100_GNDWDT_GET_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_GNDWDT_GET_CC;
            ImGui::InputScalar("msgid##direct_10712", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10712", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10712")) {
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

        case 10713: { // SD1SAT_UTRX_AX100_GET_DIAG_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_GET_DIAG_CC;
            ImGui::InputScalar("msgid##direct_10713", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10713", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10713")) {
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

        case 10714: { // SD1SAT_UTRX_AX100_TABLE_GET_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_TABLE_GET_CC;
            static UTRX_AX100_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10714", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10714", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table##direct_10714", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10714")) {
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

        case 10715: { // SD1SAT_UTRX_AX100_SET_TX_BAUD_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_SET_TX_BAUD_CC;
            static UTRX_AX100_SetTxBaud_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10715", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10715", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Baud##direct_10715", ImGuiDataType_U8, &payload.Baud);
            if (ImGui::Button("Generate CMD##direct_10715")) {
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

        case 10716: { // SD1SAT_UTRX_AX100_SET_RX_BAUD_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_SET_RX_BAUD_CC;
            static UTRX_AX100_SetRxBaud_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10716", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10716", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Baud##direct_10716", ImGuiDataType_U8, &payload.Baud);
            if (ImGui::Button("Generate CMD##direct_10716")) {
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
        
        case 10717: { // SD1SAT_UTRX_AX100_REBOOT_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_REBOOT_CC;
            ImGui::InputScalar("msgid##direct_10717", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10717", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10717")) {
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

        case 10720: { // SD1SAT_UTRX_AX100_IDENT_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_IDENT_CC;
            ImGui::InputScalar("msgid##direct_10720", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10720", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10720")) {
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

        case 10721: { // SD1SAT_UTRX_AX100_MEMFREE_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_MEMFREE_CC;
            ImGui::InputScalar("msgid##direct_10721", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10721", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10721")) {
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

        case 10722: { // SD1SAT_UTRX_AX100_BUFFREE_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_BUFFREE_CC;
            ImGui::InputScalar("msgid##direct_10722", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10722", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10722")) {
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

        case 10723: { // SD1SAT_UTRX_AX100_TABLE_SAVE_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_TABLE_SAVE_CC;
            static UTRX_AX100_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10723", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10723", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table##direct_10723", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10723")) {
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

        case 10724: { // SD1SAT_UTRX_AX100_TABLE_LOAD_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_TABLE_LOAD_CC;
            static UTRX_AX100_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10724", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10724", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table##direct_10724", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10724")) {
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

        case 10725: { // SD1SAT_UTRX_AX100_GET_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_GET_CC;
            static UTRX_AX100_Get_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10725", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10725", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table##direct_10725", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Table##direct_10725", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("Table##direct_10725", ImGuiDataType_U16, &payload.OutSize);
            if (ImGui::Button("Generate CMD##direct_10725")) {
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

        case 10726: { // SD1SAT_UTRX_AX100_SET_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_SET_CC;
            static UTRX_AX100_Set_Payload_t payload{};
            static char valhex[2 * SD1_EPS_VAL_MAX + 1] = "";
            ImGui::InputScalar("msgid##direct_10726", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10726", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table ##direct_10726", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Addr ##direct_10726", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("Valsize##direct_10726", ImGuiDataType_U16, &payload.ValSize);
            ImGui::InputText("Val (hex bytes)##direct_10726", valhex, sizeof(valhex));
            if (ImGui::Button("Generate CMD##direct_10726")) {
                WriteSystemName(msgid);
                size_t n = UelysysCopyHexBytes(payload.Val, sizeof(payload.Val), valhex);
                payload.ValSize = (uint16_t)n;
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

        case 10727: { // SD1SAT_UTRX_AX100_GET_RAW_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_GET_RAW_CC;
            static SD1SAT_UTRX_GetRaw_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10727", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10727", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table##direct_10727", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Type##direct_10727", ImGuiDataType_U8, &payload.Type);
            ImGui::InputScalar("Addr##direct_10727", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("ElemSize##direct_10727", ImGuiDataType_U16, &payload.ElemSize);
            ImGui::InputScalar("Count##direct_10727", ImGuiDataType_U16, &payload.Count);
            if (ImGui::Button("Generate CMD##direct_10727")) {
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

        case 10728: { // SD1SAT_UTRX_AX100_SET_RAW_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_SET_RAW_CC;
            static SD1SAT_UTRX_SetRaw_Payload_t payload{};
            static char valhex[2 * SD1_EPS_VAL_MAX + 1] = "";
            ImGui::InputScalar("msgid##direct_10728", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10728", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Table ##direct_10728", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Type##direct_10728", ImGuiDataType_U8, &payload.Type);
            ImGui::InputScalar("Addr##direct_10728", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("Valsize##direct_10728", ImGuiDataType_U16, &payload.ValSize);
            ImGui::InputText("Val (hex bytes)##direct_10728", valhex, sizeof(valhex));
            if (ImGui::Button("Generate CMD##direct_10728")) {
                WriteSystemName(msgid);
                size_t n = UelysysCopyHexBytes(payload.Val, sizeof(payload.Val), valhex);
                payload.ValSize = (uint16_t)n;
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

        case 10730: { // SD1SAT_UTRX_AX100_HK_SET_MODE_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_HK_SET_MODE_CC;
            static SD1SAT_UTRX_SetMode_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10730", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10730", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Mode##direct_10730", ImGuiDataType_U8, &payload.Mode);
            if (ImGui::Button("Generate CMD##direct_10730")) {
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

        case 10731: { // SD1SAT_UTRX_AX100_HK_GET_MODE_CC
            static uint16_t msgid = SD1SAT_UTRX_CMD_ID;
            static uint8_t fnccode = SD1SAT_UTRX_AX100_HK_GET_MODE_CC;
            ImGui::InputScalar("msgid##direct_10731", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10731", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10731")) {
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

        // ==================== SD1-SAT: to ====================
    default:
        return false;
    }
    return true;
}
