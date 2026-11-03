// View - 명령 생성기 입력 폼: SD1-SAT to (명령 번호 10500, 10501, 10502, 10503, 10504, 10505, 10506, 10507, 10508, 10509, 10510, 10511, 10512, 10513, 10514, 10515, 10516, 10517, 10518, 10519, 10520, 10521, 10522, 10523)
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

bool DrawSd1SatToForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10500: { // to NOOP
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10500", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10500", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10500")) {
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

        case 10501: { // to RESET
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_RESET_CC;
            ImGui::InputScalar("msgid##direct_10501", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10501", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10501")) {
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

        case 10502: { // to ENABLE_OUTPUT
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ENABLE_OUTPUT_CC;
            ImGui::InputScalar("msgid##direct_10502", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10502", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10502")) {
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

        case 10503: { // to DISABLE_OUTPUT
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_DISABLE_OUTPUT_CC;
            ImGui::InputScalar("msgid##direct_10503", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10503", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10503")) {
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

        case 10504: { // to ADD_TBL_ENTRY
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ADD_TBL_ENTRY_CC;
            static SD1SAT_TO_AddTblEntry_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10504", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10504", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MsgId##direct_10504", ImGuiDataType_U16, &payload.MsgId);
            ImGui::InputScalar("MsgLimit##direct_10504", ImGuiDataType_U16, &payload.MsgLimit);
            ImGui::InputScalar("RouteMask##direct_10504", ImGuiDataType_U16, &payload.RouteMask);
            ImGui::InputScalar("GroupData##direct_10504", ImGuiDataType_U32, &payload.GroupData);
            ImGui::InputScalar("Flag##direct_10504", ImGuiDataType_U16, &payload.Flag);
            ImGui::InputScalar("State##direct_10504", ImGuiDataType_U16, &payload.State);
            if (ImGui::Button("Generate CMD##direct_10504")) {
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

        case 10505: { // to REMOVE_TBL_ENTRY
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_REMOVE_TBL_ENTRY_CC;
            static SD1SAT_TO_MsgId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10505", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10505", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MsgId##direct_10505", ImGuiDataType_U16, &payload.MsgId);
            if (ImGui::Button("Generate CMD##direct_10505")) {
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

        case 10506: { // to ENABLE_MSG
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ENABLE_MSG_CC;
            static SD1SAT_TO_MsgId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10506", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10506", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MsgId##direct_10506", ImGuiDataType_U16, &payload.MsgId);
            if (ImGui::Button("Generate CMD##direct_10506")) {
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

        case 10507: { // to DISABLE_MSG
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_DISABLE_MSG_CC;
            static SD1SAT_TO_MsgId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10507", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10507", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MsgId##direct_10507", ImGuiDataType_U16, &payload.MsgId);
            if (ImGui::Button("Generate CMD##direct_10507")) {
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

        case 10508: { // to ENABLE_GROUP
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ENABLE_GROUP_CC;
            static SD1SAT_TO_Group_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10508", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10508", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupData##direct_10508", ImGuiDataType_U32, &payload.GroupData);
            if (ImGui::Button("Generate CMD##direct_10508")) {
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

        case 10509: { // to DISABLE_GROUP
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_DISABLE_GROUP_CC;
            static SD1SAT_TO_Group_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10509", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10509", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupData##direct_10509", ImGuiDataType_U32, &payload.GroupData);
            if (ImGui::Button("Generate CMD##direct_10509")) {
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

        case 10510: { // to ENABLE_ALL
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ENABLE_ALL_CC;
            ImGui::InputScalar("msgid##direct_10510", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10510", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10510")) {
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

        case 10511: { // to DISABLE_ALL
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_DISABLE_ALL_CC;
            ImGui::InputScalar("msgid##direct_10511", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10511", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10511")) {
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

        case 10512: { // to SET_ROUTE_BY_MID
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_ROUTE_BY_MID_CC;
            static SD1SAT_TO_RouteByMid_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10512", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10512", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MsgId##direct_10512", ImGuiDataType_U16, &payload.MsgId);
            ImGui::InputScalar("RouteMask##direct_10512", ImGuiDataType_U16, &payload.RouteMask);
            if (ImGui::Button("Generate CMD##direct_10512")) {
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

        case 10513: { // to SET_ROUTE_BY_GROUP
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_ROUTE_BY_GROUP_CC;
            static SD1SAT_TO_RouteByGroup_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10513", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10513", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupData##direct_10513", ImGuiDataType_U32, &payload.GroupData);
            ImGui::InputScalar("RouteMask##direct_10513", ImGuiDataType_U16, &payload.RouteMask);
            if (ImGui::Button("Generate CMD##direct_10513")) {
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

        case 10514: { // to MANAGE_TABLE
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_MANAGE_TABLE_CC;
            ImGui::InputScalar("msgid##direct_10514", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10514", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10514")) {
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

        case 10515: { // to ACTIVATE_ROUTES
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_ACTIVATE_ROUTES_CC;
            static SD1SAT_TO_RouteMask_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10515", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10515", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("RouteMask##direct_10515", ImGuiDataType_U16, &payload.RouteMask);
            if (ImGui::Button("Generate CMD##direct_10515")) {
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

        case 10516: { // to DEACTIVATE_ROUTES
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_DEACTIVATE_ROUTES_CC;
            static SD1SAT_TO_RouteMask_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10516", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10516", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("RouteMask##direct_10516", ImGuiDataType_U16, &payload.RouteMask);
            if (ImGui::Button("Generate CMD##direct_10516")) {
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

        case 10517: { // to SET_ROUTE_PERIOD
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_ROUTE_PERIOD_CC;
            static SD1SAT_TO_RoutePeriod_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10517", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10517", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("RouteMask##direct_10517", ImGuiDataType_U16, &payload.RouteMask);
            ImGui::InputScalar("WakePeriod##direct_10517", ImGuiDataType_U16, &payload.WakePeriod);
            if (ImGui::Button("Generate CMD##direct_10517")) {
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

        case 10518: { // to SET_WAKEUP_TIMEOUT
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_WAKEUP_TIMEOUT_CC;
            static SD1SAT_TO_WakeupTimeout_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10518", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10518", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("WakeupTimeout (ms)##direct_10518", ImGuiDataType_U32, &payload.WakeupTimeout);
            if (ImGui::Button("Generate CMD##direct_10518")) {
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

        case 10519: { // to OPEN_WINDOW
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_OPEN_WINDOW_CC;
            static SD1SAT_TO_Window_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10519", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10519", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Bytes##direct_10519", ImGuiDataType_U32, &payload.Bytes);
            ImGui::InputScalar("Profile##direct_10519", ImGuiDataType_U8, &payload.Profile);
            if (ImGui::Button("Generate CMD##direct_10519")) {
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

        case 10520: { // to CLOSE_WINDOW
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_CLOSE_WINDOW_CC;
            ImGui::InputScalar("msgid##direct_10520", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10520", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10520")) {
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

        case 10521: { // to SET_BEACON_PATTERN
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_BEACON_PATTERN_CC;
            static SD1SAT_TO_BeaconPattern_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10521", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10521", ImGuiDataType_U8, &fnccode);
            for (int bi = 0; bi < SD1SAT_TO_BEACON_PATTERN_MAX; ++bi) {
                char label[48];
                snprintf(label, sizeof(label), "Interval[%d]##direct_10521", bi);
                ImGui::InputScalar(label, ImGuiDataType_U16, &payload.Interval[bi]);
            }
            if (ImGui::Button("Generate CMD##direct_10521")) {
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

        case 10522: { // to BEACON_MUZZLE
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_BEACON_MUZZLE_CC;
            static SD1SAT_TO_BeaconMuzzle_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10522", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10522", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Ticks##direct_10522", ImGuiDataType_U16, &payload.Ticks);
            if (ImGui::Button("Generate CMD##direct_10522")) {
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

        case 10523 : {
            static uint16_t msgid = SD1SAT_TO_CMD_ID;
            static uint8_t fnccode = SD1SAT_TO_SET_FRAMING_CC;
            static SD1SAT_TO_Framing_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10522", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10522", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Ticks##direct_10522", ImGuiDataType_U32, &payload.Magic);
            if (ImGui::Button("Generate CMD##direct_10522")) {
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

        // ==================== SD1-SAT: eps ====================
    default:
        return false;
    }
    return true;
}
