// View - 명령 생성기 입력 폼: SD1-SAT eps (명령 번호 10200, 10201, 10202, 10203, 10204, 10205, 10206, 10207, 10208, 10209, 10210, 10211, 10212, 10213, 10214, 10215, 10216, 10217, 10218, 10219, 10220, 10221, 10222, 10223)
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

bool DrawSd1SatEpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10200: { // eps NOOP
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10200", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10200", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10200")) {
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

        case 10201: { // eps RESET_COUNTERS
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10201", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10201", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10201")) {
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

        case 10202: { // eps PING
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_PING_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10202", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10202", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10202", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10202")) {
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

        case 10203: { // eps IDENT
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_IDENT_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10203", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10203", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10203", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10203")) {
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

        case 10204: { // eps REBOOT
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_REBOOT_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10204", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10204", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10204", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10204")) {
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

        case 10205: { // eps MEMFREE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_MEMFREE_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10205", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10205", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10205", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10205")) {
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

        case 10206: { // eps BUFFREE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_BUFFREE_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10206", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10206", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10206", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10206")) {
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

        case 10207: { // eps GNDWDT_CLEAR
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_GNDWDT_CLEAR_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10207", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10207", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10207", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10207")) {
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

        case 10208: { // eps GET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_GET_CC;
            static SD1_EPS_Get_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10208", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10208", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10208", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10208", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Addr##direct_10208", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("OutSize##direct_10208", ImGuiDataType_U16, &payload.OutSize);
            if (ImGui::Button("Generate CMD##direct_10208")) {
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

        case 10209: { // eps SET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_SET_CC;
            static SD1_EPS_Set_Payload_t payload{};
            static char valhex[2 * SD1_EPS_VAL_MAX + 1] = "";
            ImGui::InputScalar("msgid##direct_10209", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10209", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10209", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10209", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Addr##direct_10209", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputText("Val (hex bytes)##direct_10209", valhex, sizeof(valhex));
            if (ImGui::Button("Generate CMD##direct_10209")) {
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

        case 10210: { // eps GET_RAW
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_GET_RAW_CC;
            static SD1_EPS_GetRaw_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10210", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10210", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10210", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10210", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Type##direct_10210", ImGuiDataType_U8, &payload.Type);
            ImGui::InputScalar("Addr##direct_10210", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputScalar("ElemSize##direct_10210", ImGuiDataType_U16, &payload.ElemSize);
            ImGui::InputScalar("Count##direct_10210", ImGuiDataType_U16, &payload.Count);
            if (ImGui::Button("Generate CMD##direct_10210")) {
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

        case 10211: { // eps SET_RAW
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_SET_RAW_CC;
            static SD1_EPS_SetRaw_Payload_t payload{};
            static char valhex[2 * SD1_EPS_VAL_MAX + 1] = "";
            ImGui::InputScalar("msgid##direct_10211", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10211", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10211", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10211", ImGuiDataType_U8, &payload.Table);
            ImGui::InputScalar("Type##direct_10211", ImGuiDataType_U8, &payload.Type);
            ImGui::InputScalar("Addr##direct_10211", ImGuiDataType_U16, &payload.Addr);
            ImGui::InputText("Val (hex bytes)##direct_10211", valhex, sizeof(valhex));
            if (ImGui::Button("Generate CMD##direct_10211")) {
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

        case 10212: { // eps CHANNEL_SET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_CHANNEL_SET_CC;
            static SD1_EPS_ChannelSet_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10212", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10212", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10212", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Channel##direct_10212", ImGuiDataType_U8, &payload.Channel);
            ImGui::InputScalar("On ##direct_10212", ImGuiDataType_U8, &payload.On);
            if (ImGui::Button("Generate CMD##direct_10212")) {
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

        case 10213: { // eps CHANNEL_GET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_CHANNEL_GET_CC;
            static SD1_EPS_ChannelGet_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10213", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10213", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10213", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Channel##direct_10213", ImGuiDataType_U8, &payload.Channel);
            if (ImGui::Button("Generate CMD##direct_10213")) {
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

        case 10214: { // eps CHANNELS_GET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_CHANNELS_GET_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10214", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10214", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10214", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10214")) {
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

        case 10215: { // eps TABLE_SAVE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_SAVE_CC;
            static SD1_EPS_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10215", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10215", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10215", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10215", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10215")) {
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

        case 10216: { // eps TABLE_LOAD
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_LOAD_CC;
            static SD1_EPS_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10216", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10216", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10216", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10216", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10216")) {
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

        case 10217: { // eps HK_SET_MODE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_HK_SET_MODE_CC;
            static SD1_EPS_SetMode_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10217", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10217", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Mode ##direct_10217", ImGuiDataType_U8, &payload.Mode);
            if (ImGui::Button("Generate CMD##direct_10217")) {
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

        case 10218: { // eps HK_GET_MODE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_HK_GET_MODE_CC;
            ImGui::InputScalar("msgid##direct_10218", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10218", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10218")) {
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

        case 10219: { // eps TABLE_ACQUIRE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_ACQUIRE_CC;
            static SD1_EPS_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10219", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10219", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10219", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10219", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10219")) {
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

        case 10220: { // eps TABLE_READBACK
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_READBACK_CC;
            static SD1_EPS_Readback_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10220", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10220", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Offset##direct_10220", ImGuiDataType_U16, &payload.Offset);
            ImGui::InputScalar("Length##direct_10220", ImGuiDataType_U16, &payload.Length);
            if (ImGui::Button("Generate CMD##direct_10220")) {
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

        case 10221: { // eps TABLE_TO_FILE
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_TO_FILE_CC;
            static SD1_EPS_ToFile_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10221", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10221", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10221", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10221", ImGuiDataType_U8, &payload.Table);
            ImGui::InputText("Path##direct_10221", payload.Path, sizeof(payload.Path));
            if (ImGui::Button("Generate CMD##direct_10221")) {
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

        case 10222: { // eps TABLE_GET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_TABLE_GET_CC;
            static SD1_EPS_Table_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10222", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10222", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10222", ImGuiDataType_U8, &payload.Board);
            ImGui::InputScalar("Table ##direct_10222", ImGuiDataType_U8, &payload.Table);
            if (ImGui::Button("Generate CMD##direct_10222")) {
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

        case 10223: { // eps WDT_GET
            static uint16_t msgid = SD1_EPS_CMD_ID;
            static uint8_t fnccode = SD1_EPS_WDT_GET_CC;
            static SD1_EPS_Board_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10223", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10223", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Board ##direct_10223", ImGuiDataType_U8, &payload.Board);
            if (ImGui::Button("Generate CMD##direct_10223")) {
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

        // ==================== SD1-SAT: paysd1_roma ====================
    default:
        return false;
    }
    return true;
}
