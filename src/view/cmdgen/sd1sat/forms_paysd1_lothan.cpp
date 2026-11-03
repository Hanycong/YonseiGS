// View - 명령 생성기 입력 폼: SD1-SAT paysd1_lothan (명령 번호 10800, 10801, 10802, 10803, 10804, 10805, 10806, 10807, 10808, 10809, 10810, 10811, 10812, 10813, 10814, 10815, 10816, 10817, 10818, 10819, 10820, 10821, 10822, 10823, 10824, 10825, 10826, 10827)
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

bool DrawSd1SatPaysd1LothanForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10800: { // paysd1_lothan NOOP
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10800", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10800", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10800")) {
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

        case 10801: { // paysd1_lothan RESET_COUNTERS
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_RESET_COUNTERS;
            ImGui::InputScalar("msgid##direct_10801", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10801", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10801")) {
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

        case 10802: { // paysd1_lothan TRIGGER_CAPTURE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_TRIGGER_CAPTURE;
            static SD1SAT_PAYSD1_LOTHAN_TriggerCapture_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10802", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10802", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("LocId##direct_10802", ImGuiDataType_U16, &payload.LocId);
            ImGui::InputScalar("AltitudeM##direct_10802", ImGuiDataType_U32, &payload.AltitudeM);
            ImGui::InputScalar("Flags##direct_10802", ImGuiDataType_U8, &payload.Flags);
            if (ImGui::Button("Generate CMD##direct_10802")) {
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

        case 10803: { // paysd1_lothan REGISTER_LOCATION
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REGISTER_LOCATION;
            static SD1SAT_PAYSD1_LOTHAN_RegisterLocation_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10803", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10803", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("LocId##direct_10803", ImGuiDataType_U16, &payload.LocId);
            ImGui::InputScalar("LatCentideg##direct_10803", ImGuiDataType_S16, &payload.LatCentideg);
            ImGui::InputScalar("LonCentideg##direct_10803", ImGuiDataType_S16, &payload.LonCentideg);
            if (ImGui::Button("Generate CMD##direct_10803")) {
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

        case 10804: { // paysd1_lothan ROLLBACK_BASELINE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_ROLLBACK_BASELINE;
            static SD1SAT_PAYSD1_LOTHAN_LocId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10804", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10804", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("LocId##direct_10804", ImGuiDataType_U16, &payload.LocId);
            if (ImGui::Button("Generate CMD##direct_10804")) {
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

        case 10805: { // paysd1_lothan MARK_DOWNLINKED
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_MARK_DOWNLINKED;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10805", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10805", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10805", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10805")) {
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

        case 10806: { // paysd1_lothan SET_POLICY
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_POLICY;
            static SD1SAT_PAYSD1_LOTHAN_SetPolicy_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10806", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10806", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("PolicyId##direct_10806", ImGuiDataType_U8, &payload.PolicyId);
            ImGui::InputScalar("Value##direct_10806", ImGuiDataType_U8, &payload.Value);
            if (ImGui::Button("Generate CMD##direct_10806")) {
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

        case 10807: { // paysd1_lothan REQUEST_STATUS
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_STATUS;
            ImGui::InputScalar("msgid##direct_10807", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10807", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10807")) {
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

        case 10808: { // paysd1_lothan REQUEST_DIGEST
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_DIGEST;
            ImGui::InputScalar("msgid##direct_10808", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10808", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10808")) {
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

        case 10809: { // paysd1_lothan REQUEST_THUMBNAIL
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_THUMBNAIL;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10809", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10809", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10809", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10809")) {
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

        case 10810: { // paysd1_lothan GET_POLICY
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_GET_POLICY;
            static SD1SAT_PAYSD1_LOTHAN_GetPolicy_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10810", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10810", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("PolicyId##direct_10810", ImGuiDataType_U8, &payload.PolicyId);
            if (ImGui::Button("Generate CMD##direct_10810")) {
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

        case 10811: { // paysd1_lothan REQUEST_DOWNLINK_IMAGE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_DOWNLINK_IMAGE;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10811", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10811", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10811", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10811")) {
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

        case 10812: { // paysd1_lothan UPLINK_FILE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPLINK_FILE;
            static SD1SAT_PAYSD1_LOTHAN_TransferId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10812", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10812", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("TransferId##direct_10812", ImGuiDataType_U16, &payload.TransferId);
            if (ImGui::Button("Generate CMD##direct_10812")) {
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

        case 10813: { // paysd1_lothan UPDATE_APPLY
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPDATE_APPLY;
            static SD1SAT_PAYSD1_LOTHAN_TransferId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10813", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10813", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("TransferId##direct_10813", ImGuiDataType_U16, &payload.TransferId);
            if (ImGui::Button("Generate CMD##direct_10813")) {
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

        case 10814: { // paysd1_lothan UPDATE_ABORT
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPDATE_ABORT;
            static SD1SAT_PAYSD1_LOTHAN_TransferId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10814", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10814", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("TransferId##direct_10814", ImGuiDataType_U16, &payload.TransferId);
            if (ImGui::Button("Generate CMD##direct_10814")) {
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

        case 10815: { // paysd1_lothan REQUEST_UPDATE_STATUS
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_UPDATE_STATUS;
            ImGui::InputScalar("msgid##direct_10815", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10815", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10815")) {
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

        case 10816: { // paysd1_lothan SET_TIME
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_TIME;
            static SD1SAT_PAYSD1_LOTHAN_SetTime_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10816", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10816", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EpochMs##direct_10816", ImGuiDataType_U64, &payload.EpochMs);
            if (ImGui::Button("Generate CMD##direct_10816")) {
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

        case 10817: { // paysd1_lothan SET_DIAGNOSTIC
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_DIAGNOSTIC;
            static SD1SAT_PAYSD1_LOTHAN_SetDiagnostic_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10817", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10817", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Enabled##direct_10817", ImGuiDataType_U8, &payload.Enabled);
            if (ImGui::Button("Generate CMD##direct_10817")) {
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

        case 10818: { // paysd1_lothan RUN_GEOFENCE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_RUN_GEOFENCE;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10818", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10818", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10818", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10818")) {
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

        case 10819: { // paysd1_lothan SYNC_TIME_FROM_CFS
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_SYNC_TIME_FROM_CFS;
            ImGui::InputScalar("msgid##direct_10819", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10819", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10819")) {
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

        case 10820: { // paysd1_lothan REQUEST_ORIGINAL_SIZE
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_ORIGINAL_SIZE;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10820", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10820", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10820", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10820")) {
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

        case 10821: { // paysd1_lothan SPLIT_ORIGINAL
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_SPLIT_ORIGINAL;
            static SD1SAT_PAYSD1_LOTHAN_SplitOriginal_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10821", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10821", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10821", ImGuiDataType_U16, &payload.CaptureId);
            ImGui::InputScalar("PartSize##direct_10821", ImGuiDataType_U32, &payload.PartSize);
            if (ImGui::Button("Generate CMD##direct_10821")) {
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

        case 10822: { // paysd1_lothan DELETE_SPLIT
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_DELETE_SPLIT;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10822", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10822", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10822", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10822")) {
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

        case 10823: { // paysd1_lothan CountSplitFiles Cmd
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_COUNT_SPLIT_FILES;
            static SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10822", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10822", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CaptureId##direct_10822", ImGuiDataType_U16, &payload.CaptureId);
            if (ImGui::Button("Generate CMD##direct_10822")) {
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

        case 10824: { // paysd1_lothan CaptureStart Cmd
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_START;
            static SD1SAT_PAYSD1_LOTHAN_CaptureStart_Payload_t payload{};
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("StartTimeMs", ImGuiDataType_U64, &payload.StartTimeMs);
            ImGui::InputScalar("RepeatEveryMs", ImGuiDataType_U32, &payload.RepeatEveryMs);
            ImGui::InputScalar("RepeatCount", ImGuiDataType_U16, &payload.RepeatCount);
            ImGui::InputScalar("LocId", ImGuiDataType_U16, &payload.LocId);
            ImGui::InputScalar("AltitudeM", ImGuiDataType_U32, &payload.AltitudeM);
            ImGui::InputScalar("Flags", ImGuiDataType_U8, &payload.Flags);
            if (ImGui::Button("Generate CMD")) {
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

        case 10825: { // paysd1_lothan CaptureStartRelative Cmd
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_START_RELATIVE;
            static PAYSD1_LOTHAN_CaptureStartRelative_Payload_t payload{};
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("StartTimeMs", ImGuiDataType_U64, &payload.DelayMs);
            ImGui::InputScalar("RepeatEveryMs", ImGuiDataType_U32, &payload.RepeatEveryMs);
            ImGui::InputScalar("RepeatCount", ImGuiDataType_U16, &payload.RepeatCount);
            ImGui::InputScalar("LocId", ImGuiDataType_U16, &payload.LocId);
            ImGui::InputScalar("AltitudeM", ImGuiDataType_U32, &payload.AltitudeM);
            ImGui::InputScalar("Flags", ImGuiDataType_U8, &payload.Flags);
            if (ImGui::Button("Generate CMD")) {
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

        case 10826: { // paysd1_lothan CaptureStop Cmd
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_STOP;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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

        case 10827: { // paysd1_lothan OTA Patch TEst Cmd
            static uint16_t msgid = SD1SAT_PAYSD1_LOTHAN_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_LOTHAN_FunctionCode_OTA_PATCH_TEST;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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

    default:
        return false;
    }
    return true;
}
