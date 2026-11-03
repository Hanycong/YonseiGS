// View - 명령 생성기 입력 폼: SD1-SAT paysd1_roma (명령 번호 10400, 10401, 10402, 10403, 10404, 10405, 10406, 10407, 10408, 10409, 10410, 10411, 10412, 10413, 10414, 10415, 10416, 10417, 10418, 10419, 10420, 10421, 10422, 10423, 10424, 10425, 10426, 10427, 10428, 10429, 10430, 10431, 10432, 10433, 10434, 10435, 10436, 10437, 10438, 10439, 10440)
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

bool DrawSd1SatPaysd1RomaForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 10400: { // paysd1_roma NOOP
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_NOOP_CC;
            ImGui::InputScalar("msgid##direct_10400", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10400", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10400")) {
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

        case 10401: { // paysd1_roma RESET_COUNTERS
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_RESET_COUNTERS_CC;
            ImGui::InputScalar("msgid##direct_10401", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10401", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10401")) {
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

        case 10402: { // paysd1_roma PING
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PING_CC;
            ImGui::InputScalar("msgid##direct_10402", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10402", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10402")) {
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

        case 10403: { // paysd1_roma CLOCK_SYNC
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_CLOCK_SYNC_CC;
            ImGui::InputScalar("msgid##direct_10403", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10403", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10403")) {
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

        case 10404: { // paysd1_roma GET_SPECIFIC_LINE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_SPECIFIC_LINE_CC;
            static SD1SAT_PAYSD1_ROMA_GetSpecificLine_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10404", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10404", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("line_no##direct_10404", ImGuiDataType_U16, &payload.line_no);
            if (ImGui::Button("Generate CMD##direct_10404")) {
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

        case 10405: { // paysd1_roma GET_MULTIPLE_LINES
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_MULTIPLE_LINES_CC;
            static SD1SAT_PAYSD1_ROMA_GetMultipleLines_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10405", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10405", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("line_start##direct_10405", ImGuiDataType_U16, &payload.line_start);
            ImGui::InputScalar("line_stop##direct_10405", ImGuiDataType_U16, &payload.line_stop);
            if (ImGui::Button("Generate CMD##direct_10405")) {
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

        case 10406: { // paysd1_roma GET_LATEST_LINE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_LATEST_LINE_CC;
            ImGui::InputScalar("msgid##direct_10406", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10406", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10406")) {
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

        case 10407: { // paysd1_roma GET_LATEST_N_LINES
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_LATEST_N_LINES_CC;
            static SD1SAT_PAYSD1_ROMA_GetLatestNLines_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10407", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10407", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("n_lines##direct_10407", ImGuiDataType_U16, &payload.n_lines);
            if (ImGui::Button("Generate CMD##direct_10407")) {
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

        case 10408: { // paysd1_roma CLEAR_ALL_LINES
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_CLEAR_ALL_LINES_CC;
            static SD1SAT_PAYSD1_ROMA_ClearAllLines_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10408", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10408", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("confirmation (0xB00B)##direct_10408", ImGuiDataType_U16, &payload.confirmation);
            if (ImGui::Button("Generate CMD##direct_10408")) {
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

        case 10409: { // paysd1_roma GET_SINGLE_ENTRY
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_SINGLE_ENTRY_CC;
            static SD1SAT_PAYSD1_ROMA_GetSingleEntry_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10409", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10409", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("entry_index##direct_10409", ImGuiDataType_U16, &payload.entry_index);
            if (ImGui::Button("Generate CMD##direct_10409")) {
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

        case 10410: { // paysd1_roma GET_MULTIPLE_ENTRIES
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_MULTIPLE_ENTRIES_CC;
            static SD1SAT_PAYSD1_ROMA_GetMultipleEntries_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10410", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10410", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("start_index##direct_10410", ImGuiDataType_U16, &payload.start_index);
            ImGui::InputScalar("end_index##direct_10410", ImGuiDataType_U16, &payload.end_index);
            if (ImGui::Button("Generate CMD##direct_10410")) {
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

        case 10411: { // paysd1_roma ADD_ENTRY
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_ADD_ENTRY_CC;
            static SD1SAT_PAYSD1_ROMA_AddEntry_Payload_t payload{};
            static char cmdhex[2 * SD1SAT_PAYSD1_ROMA_MAX_SCHEDULE_COMMAND_BYTES + 1] = "";
            ImGui::InputScalar("msgid##direct_10411", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10411", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("time_ms##direct_10411", ImGuiDataType_U64, &payload.time_ms);
            ImGui::InputScalar("repeat_every_ms##direct_10411", ImGuiDataType_U32, &payload.repeat_every_ms);
            ImGui::InputScalar("repeat_max##direct_10411", ImGuiDataType_S8, &payload.repeat_max);
            ImGui::InputText("command (hex bytes)##direct_10411", cmdhex, sizeof(cmdhex));
            if (ImGui::Button("Generate CMD##direct_10411")) {
                WriteSystemName(msgid);
                UelysysCopyHexBytes(payload.command, sizeof(payload.command), cmdhex);
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

        case 10412: { // paysd1_roma REMOVE_ENTRY
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_REMOVE_ENTRY_CC;
            static SD1SAT_PAYSD1_ROMA_RemoveEntry_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10412", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10412", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("entry_index##direct_10412", ImGuiDataType_U8, &payload.entry_index);
            if (ImGui::Button("Generate CMD##direct_10412")) {
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

        case 10413: { // paysd1_roma GET_USED_SLOTS
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_GET_USED_SLOTS_CC;
            ImGui::InputScalar("msgid##direct_10413", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10413", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10413")) {
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

        case 10414: { // paysd1_roma ADD_RELATIVE_ENTRY
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_ADD_RELATIVE_ENTRY_CC;
            static SD1SAT_PAYSD1_ROMA_AddRelativeEntry_Payload_t payload{};
            static char cmdhex[2 * SD1SAT_PAYSD1_ROMA_MAX_SCHEDULE_COMMAND_BYTES + 1] = "";
            ImGui::InputScalar("msgid##direct_10414", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10414", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("delay_ms##direct_10414", ImGuiDataType_U64, &payload.delay_ms);
            ImGui::InputScalar("repeat_every_ms##direct_10414", ImGuiDataType_U32, &payload.repeat_every_ms);
            ImGui::InputScalar("repeat_max##direct_10414", ImGuiDataType_S8, &payload.repeat_max);
            ImGui::InputText("command (hex bytes)##direct_10414", cmdhex, sizeof(cmdhex));
            if (ImGui::Button("Generate CMD##direct_10414")) {
                WriteSystemName(msgid);
                UelysysCopyHexBytes(payload.command, sizeof(payload.command), cmdhex);
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

        case 10415: { // paysd1_roma SET_ROUTE_DEFAULT
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SET_ROUTE_DEFAULT_CC;
            ImGui::InputScalar("msgid##direct_10415", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10415", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10415")) {
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

        case 10416: { // paysd1_roma RESET_ROUTE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_RESET_ROUTE_CC;
            ImGui::InputScalar("msgid##direct_10416", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10416", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10416")) {
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

        case 10417: { // paysd1_roma LOAD_ROUTE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_LOAD_ROUTE_CC;
            ImGui::InputScalar("msgid##direct_10417", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10417", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10417")) {
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

        case 10418: { // paysd1_roma SAVE_ROUTE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SAVE_ROUTE_CC;
            ImGui::InputScalar("msgid##direct_10418", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10418", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10418")) {
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

        case 10419: { // paysd1_roma SEND_ROUTE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SEND_ROUTE_CC;
            ImGui::InputScalar("msgid##direct_10419", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10419", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10419")) {
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

        case 10420: { // paysd1_roma SET_ROUTE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SET_ROUTE_CC;
            static SD1SAT_PAYSD1_ROMA_SetRoute_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10420", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10420", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("route##direct_10420", payload.route, sizeof(payload.route));
            if (ImGui::Button("Generate CMD##direct_10420")) {
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

        case 10421: { // paysd1_roma PAR_GET
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_GET_CC;
            static SD1SAT_PAYSD1_ROMA_ParGet_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10421", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10421", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10421", ImGuiDataType_U8, &payload.table);
            ImGui::InputScalar("param##direct_10421", ImGuiDataType_U8, &payload.param);
            if (ImGui::Button("Generate CMD##direct_10421")) {
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

        case 10422: { // paysd1_roma PAR_SET
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_SET_CC;
            static SD1SAT_PAYSD1_ROMA_ParSet_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10422", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10422", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10422", ImGuiDataType_U8, &payload.table);
            ImGui::InputScalar("param##direct_10422", ImGuiDataType_U8, &payload.param);
            ImGui::InputScalar("value##direct_10422", ImGuiDataType_S32, &payload.value);
            if (ImGui::Button("Generate CMD##direct_10422")) {
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

        case 10423: { // paysd1_roma PAR_DEFAULTS
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_DEFAULTS_CC;
            static SD1SAT_PAYSD1_ROMA_ParTable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10423", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10423", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10423", ImGuiDataType_U8, &payload.table);
            if (ImGui::Button("Generate CMD##direct_10423")) {
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

        case 10424: { // paysd1_roma PAR_SAVE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_SAVE_CC;
            static SD1SAT_PAYSD1_ROMA_ParTable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10424", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10424", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10424", ImGuiDataType_U8, &payload.table);
            if (ImGui::Button("Generate CMD##direct_10424")) {
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

        case 10425: { // paysd1_roma PAR_RESTORE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_RESTORE_CC;
            static SD1SAT_PAYSD1_ROMA_ParTable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10425", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10425", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10425", ImGuiDataType_U8, &payload.table);
            if (ImGui::Button("Generate CMD##direct_10425")) {
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

        case 10426: { // paysd1_roma PAR_LOAD
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_LOAD_CC;
            static SD1SAT_PAYSD1_ROMA_ParTable_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10426", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10426", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10426", ImGuiDataType_U8, &payload.table);
            if (ImGui::Button("Generate CMD##direct_10426")) {
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

        case 10427: { // paysd1_roma PAR_SET_OOB
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAR_SET_OOB_CC;
            static SD1SAT_PAYSD1_ROMA_ParSetOob_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10427", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10427", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("table##direct_10427", ImGuiDataType_U8, &payload.table);
            ImGui::InputScalar("enable##direct_10427", ImGuiDataType_U8, &payload.enable);
            if (ImGui::Button("Generate CMD##direct_10427")) {
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

        case 10428: { // paysd1_roma SEND_COMMAND
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SEND_COMMAND_CC;
            static SD1SAT_PAYSD1_ROMA_SendCommand_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10428", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10428", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd##direct_10428", payload.cmd, sizeof(payload.cmd));
            if (ImGui::Button("Generate CMD##direct_10428")) {
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

        case 10429: { // paysd1_roma SEND_RF_BYTES
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SEND_RF_BYTES_CC;
            static SD1SAT_PAYSD1_ROMA_SendRfBytes_Payload_t payload{};
            static char byteshex[2 * SD1SAT_PAYSD1_ROMA_RF_MAX_PAYLOAD_BYTES + 1] = "";
            ImGui::InputScalar("msgid##direct_10429", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10429", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("Bytes (hex)##direct_10429", byteshex, sizeof(byteshex));
            if (ImGui::Button("Generate CMD##direct_10429")) {
                WriteSystemName(msgid);
                size_t n = UelysysCopyHexBytes(payload.Bytes, sizeof(payload.Bytes), byteshex);
                payload.Length = (uint8_t)n;
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

        case 10430: { // paysd1_roma SYNC_RX
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SYNC_RX_CC;
            ImGui::InputScalar("msgid##direct_10430", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10430", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10430")) {
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

        case 10431: { // paysd1_roma SYNC_TX
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SYNC_TX_CC;
            ImGui::InputScalar("msgid##direct_10431", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10431", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10431")) {
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

        case 10432: { // paysd1_roma PAY_INIT
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_PAY_INIT_CC;
            ImGui::InputScalar("msgid##direct_10432", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10432", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10432")) {
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

        case 10433: { // paysd1_roma LISTEN_START
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_LISTEN_START_CC;
            ImGui::InputScalar("msgid##direct_10433", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10433", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10433")) {
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

        case 10434: { // paysd1_roma LISTEN_STOP
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_LISTEN_STOP_CC;
            ImGui::InputScalar("msgid##direct_10434", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10434", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10434")) {
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

        case 10435: { // paysd1_roma SHOUT_START
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SHOUT_START_CC;
            static SD1SAT_PAYSD1_ROMA_ShoutStart_Payload_t payload{};
            static char byteshex[2 * SD1SAT_PAYSD1_ROMA_ISL_MAX_COMMAND_BYTES + 1] = "";
            ImGui::InputScalar("msgid##direct_10435", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10435", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("StartTimeMs##direct_10435", ImGuiDataType_U64, &payload.StartTimeMs);
            ImGui::InputScalar("RepeatEveryMs##direct_10435", ImGuiDataType_U32, &payload.RepeatEveryMs);
            ImGui::InputScalar("RepeatCount##direct_10435", ImGuiDataType_U16, &payload.RepeatCount);
            ImGui::InputText("Bytes (hex)##direct_10435", byteshex, sizeof(byteshex));
            if (ImGui::Button("Generate CMD##direct_10435")) {
                WriteSystemName(msgid);
                size_t n = UelysysCopyHexBytes(payload.Bytes, sizeof(payload.Bytes), byteshex);
                payload.Length = (uint8_t)n;
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

        case 10436: { // paysd1_roma SHOUT_START_RELATIVE
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SHOUT_START_RELATIVE_CC;
            static SD1SAT_PAYSD1_ROMA_ShoutStartRelative_Payload_t payload{};
            static char byteshex[2 * SD1SAT_PAYSD1_ROMA_ISL_MAX_COMMAND_BYTES + 1] = "";
            ImGui::InputScalar("msgid##direct_10436", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10436", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("DelayMs##direct_10436", ImGuiDataType_U64, &payload.DelayMs);
            ImGui::InputScalar("RepeatEveryMs##direct_10436", ImGuiDataType_U32, &payload.RepeatEveryMs);
            ImGui::InputScalar("RepeatCount##direct_10436", ImGuiDataType_U16, &payload.RepeatCount);
            ImGui::InputText("Bytes (hex)##direct_10436", byteshex, sizeof(byteshex));
            if (ImGui::Button("Generate CMD##direct_10436")) {
                WriteSystemName(msgid);
                size_t n = UelysysCopyHexBytes(payload.Bytes, sizeof(payload.Bytes), byteshex);
                payload.Length = (uint8_t)n;
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

        case 10437: { // paysd1_roma SHOUT_STOP
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SHOUT_STOP_CC;
            ImGui::InputScalar("msgid##direct_10437", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10437", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10437")) {
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

        case 10438: { // paysd1_roma ISL_TEST_COUNT_UP
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_ISL_TEST_COUNT_UP_CC;
            ImGui::InputScalar("msgid##direct_10438", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10438", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10438")) {
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

        case 10439: { // paysd1_roma ISL_TEST_COUNT_LOAD
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_ISL_TEST_COUNT_LOAD_CC;
            ImGui::InputScalar("msgid##direct_10439", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10439", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_10439")) {
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

        case 10440: { // paysd1_roma SET_DIAGNOSTIC
            static uint16_t msgid = SD1SAT_PAYSD1_ROMA_CMD_ID;
            static uint8_t fnccode = SD1SAT_PAYSD1_ROMA_SET_DIAGNOSTIC_CC;
            static SD1SAT_PAYSD1_ROMA_SetDiagnostic_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_10440", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_10440", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Enabled##direct_10440", ImGuiDataType_U8, &payload.Enabled);
            if (ImGui::Button("Generate CMD##direct_10440")) {
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

    default:
        return false;
    }
    return true;
}
