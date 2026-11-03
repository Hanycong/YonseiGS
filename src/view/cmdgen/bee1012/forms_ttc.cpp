// View - 명령 생성기 입력 폼: BEE1012 ttc (명령 번호 3000, 3001, 3010, 3011, 3012, 3013, 3014, 3015, 3016, 3017, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3025, 3026, 3027, 3028)
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

bool DrawBee1012TtcForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 3000: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_NOOP_CC);
            ImGui::InputScalar("msgid##direct_3000", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3000", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3000")) {
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
        case 3001: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_RESET_COUNTERS_CC);
            ImGui::InputScalar("msgid##direct_3001", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3001", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3001")) {
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
        case 3010: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_GET_TIMELINE_HK_CC);
            ImGui::InputScalar("msgid##direct_3010", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3010", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3010")) {
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
        case 3011: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_RESET_TIMELINE_HK_CC);
            ImGui::InputScalar("msgid##direct_3011", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3011", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3011")) {
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
        case 3012: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC);
            ImGui::InputScalar("msgid##direct_3012", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3012", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3012")) {
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
        case 3013: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC);
            ImGui::InputScalar("msgid##direct_3013", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3013", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3013")) {
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
        case 3014: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC);
            ImGui::InputScalar("msgid##direct_3014", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3014", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3014")) {
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
        case 3015: {
            UELYSYS_TTC_InsertAbsCmdEntryCmd_t &cmd = command5->uelysys_ttc_insertabscmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3015", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3015", ImGuiDataType_U8, &fnccode);
            static char command_hex[2048] = {};
            ImGui::InputScalar("ExecutionTimeAbsolute", ImGuiDataType_U32, &cmd.ExecutionTimeAbsolute);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            ImGui::InputScalar("CommandSize", ImGuiDataType_U16, &cmd.CommandSize);
            ImGui::InputTextMultiline("Command hex", command_hex, sizeof(command_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Command, sizeof(cmd.Command), command_hex);
            if (cmd.CommandSize > copied) cmd.CommandSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3015")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3016: {
            UELYSYS_TTC_InsertRelCmdEntryCmd_t &cmd = command5->uelysys_ttc_insertrelcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3016", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3016", ImGuiDataType_U8, &fnccode);
            static char command_hex[2048] = {};
            ImGui::InputScalar("ExecutionTimeRelative", ImGuiDataType_U16, &cmd.ExecutionTimeRelative);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            ImGui::InputScalar("CommandSize", ImGuiDataType_U16, &cmd.CommandSize);
            ImGui::InputTextMultiline("Command hex", command_hex, sizeof(command_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Command, sizeof(cmd.Command), command_hex);
            if (cmd.CommandSize > copied) cmd.CommandSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3016")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3017: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_DELETE_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3017", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3017", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3017")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_TTC_DeleteEntryCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3018: {
            UELYSYS_TTC_DeleteGroupCmd_t &cmd = command5->uelysys_ttc_deletegroupcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_DELETE_CMD_GROUP_CC);
            ImGui::InputScalar("msgid##direct_3018", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3018", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3018")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3019: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_DELETE_ALL_ENTRIES_CC);
            ImGui::InputScalar("msgid##direct_3019", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3019", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3019")) {
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
        case 3020: {
            UELYSYS_TTC_ExecuteEntryCmd_t &cmd = command5->uelysys_ttc_executeentrycmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3020", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3020", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::Checkbox("PersistentExecution", &cmd.PersistentExecution);
            if (ImGui::Button("Generate CMD##direct_3020")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3021: {
            UELYSYS_TTC_ExecuteGroupCmd_t &cmd = command5->uelysys_ttc_executegroupcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_EXECUTE_CMD_GROUP_CC);
            ImGui::InputScalar("msgid##direct_3021", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3021", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::Checkbox("PersistentExecution", &cmd.PersistentExecution);
            if (ImGui::Button("Generate CMD##direct_3021")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3022: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC);
            ImGui::InputScalar("msgid##direct_3022", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3022", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3022")) {
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
        case 3023: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC);
            ImGui::InputScalar("msgid##direct_3023", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3023", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3023")) {
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
        case 3024: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_INIT_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3024", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3024", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3024")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_TTC_DeleteEntryCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3025: {
            UELYSYS_TTC_PlumbEntryWriteCmd_t &cmd = command5->uelysys_ttc_plumbwritecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3025", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3025", ImGuiDataType_U8, &fnccode);
            static char data_hex[1024] = {};
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("Offset", ImGuiDataType_U16, &cmd.Offset);
            ImGui::InputScalar("ChunkSize", ImGuiDataType_U16, &cmd.ChunkSize);
            ImGui::InputTextMultiline("Data hex", data_hex, sizeof(data_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Data, sizeof(cmd.Data), data_hex);
            if (cmd.ChunkSize > copied) cmd.ChunkSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3025")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3026: {
            UELYSYS_TTC_PlumbEntryFinalizeCmd_t &cmd = command5->uelysys_ttc_plumbfinalizecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3026", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3026", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("TimeTag", ImGuiDataType_U32, &cmd.TimeTag);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("CmdSize", ImGuiDataType_U16, &cmd.CmdSize);
            ImGui::InputScalar("TimeTagType", ImGuiDataType_U8, &cmd.TimeTagType);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            if (ImGui::Button("Generate CMD##direct_3026")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3027: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC);
            ImGui::InputScalar("msgid##direct_3027", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3027", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3027")) {
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

        case 3028: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3028", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3028", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3028")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_TTC_DeleteEntryCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_TTC_DeleteEntryCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        // ==================== meow ====================
    default:
        return false;
    }
    return true;
}
