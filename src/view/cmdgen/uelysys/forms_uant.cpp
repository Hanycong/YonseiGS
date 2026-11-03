// View - 명령 생성기 입력 폼: UELYSYS uant (명령 번호 9001, 9006, 9008, 9009, 9010, 9023, 9024, 9000, 9002, 9003, 9004, 9005, 9007, 9011)
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

bool DrawUelysysUantForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 9001: { // uant RESET_COUNTERS
            static uint16_t msgid=0x1840; static uint8_t fnccode=1; ImGui::InputScalar("msgid##direct_9001",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_9001",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_9001")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 9006: case 9008: case 9009: { // GET_BACKUP_STATUS / GET_TEMPERATURE / GET_SETTINGS
            const int selected_case=gen_fnccode; static uint16_t msgid=0x1840; static uint8_t fnccode_by_case[3]={6,8,9}; static uint8_t addr_by_case[3]={0,0,0}; const int slot=(selected_case==9006)?0:(selected_case==9008?1:2); uint8_t &fnccode=fnccode_by_case[slot]; uint8_t &addr=addr_by_case[slot];
            ImGui::InputScalar("msgid##direct_9006_9009",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_9006_9009",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Addr##direct_9006_9009",ImGuiDataType_U8,&addr);
            if(ImGui::Button("Generate CMD##direct_9006_9009")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; cmd[8]=addr; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 9010: { // uant SET_SETTINGS (natural alignment)
            static uint16_t msgid=0x1840; static uint8_t fnccode=10; struct Payload { uint8_t Addr; uint8_t AlignmentPadding; uint16_t MinutesUntilDeploy; uint8_t BackupActive; uint8_t MaxBurnDuration; }; static_assert(sizeof(Payload)==6,"uant SET_SETTINGS payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_9010",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_9010",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Addr##direct_9010",ImGuiDataType_U8,&payload.Addr); ImGui::InputScalar("MinutesUntilDeploy##direct_9010",ImGuiDataType_U16,&payload.MinutesUntilDeploy); ImGui::InputScalar("BackupActive##direct_9010",ImGuiDataType_U8,&payload.BackupActive); ImGui::InputScalar("MaxBurnDuration##direct_9010",ImGuiDataType_U8,&payload.MaxBurnDuration);
            if(ImGui::Button("Generate CMD##direct_9010")){ WriteSystemName(msgid); uint8_t cmd[14]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; payload.AlignmentPadding=0; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 9023: { // uant BURN_CHANNEL_INTERNAL
            static uint16_t msgid=0x1840; static uint8_t fnccode=23; struct __attribute__((packed)) Payload { uint8_t Addr; uint8_t Channel; uint8_t Duration; }; static_assert(sizeof(Payload)==3,"uant BURN_CHANNEL_INTERNAL payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_9023",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_9023",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Addr##direct_9023",ImGuiDataType_U8,&payload.Addr); ImGui::InputScalar("Channel##direct_9023",ImGuiDataType_U8,&payload.Channel); ImGui::InputScalar("Duration##direct_9023",ImGuiDataType_U8,&payload.Duration);
            if(ImGui::Button("Generate CMD##direct_9023")){ WriteSystemName(msgid); uint8_t cmd[11]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 9024: { // uant GET_STATUS_INTERNAL
            static uint16_t msgid=0x1840; static uint8_t fnccode=24; struct __attribute__((packed)) Payload { uint8_t address; uint8_t channel; }; static_assert(sizeof(Payload)==2,"uant GET_STATUS_INTERNAL payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_9024",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_9024",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("address##direct_9024",ImGuiDataType_U8,&payload.address); ImGui::InputScalar("channel##direct_9024",ImGuiDataType_U8,&payload.channel);
            if(ImGui::Button("Generate CMD##direct_9024")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }

        // ==================== UELYSYS gpio ====================
        case 9000: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_NOOP_CC);
            ImGui::InputScalar("msgid##direct_9000", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9000", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_9000")) {
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
        case 9002: {
            UELYSYS_UANT_AddrCmd_t &cmd = command5->uelysys_uant_addrcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_RESET_STATUS_COUNT_CC);
            ImGui::InputScalar("msgid##direct_9002", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9002", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Addr", ImGuiDataType_U8, &cmd.Addr);
            if (ImGui::Button("Generate CMD##direct_9002")) {
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
        case 9003: {
            UELYSYS_UANT_BurnChannelCmd_t &cmd = command5->uelysys_uant_burnchannelcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_BURN_CHANNEL_CC);
            ImGui::InputScalar("msgid##direct_9003", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9003", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Addr", ImGuiDataType_U8, &cmd.Addr);
            ImGui::InputScalar("Channel", ImGuiDataType_U8, &cmd.Channel);
            ImGui::InputScalar("Duration", ImGuiDataType_U8, &cmd.Duration);
            if (ImGui::Button("Generate CMD##direct_9003")) {
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
        case 9004: {
            UELYSYS_UANT_AddrCmd_t &cmd = command5->uelysys_uant_addrcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_STOP_BURN_CC);
            ImGui::InputScalar("msgid##direct_9004", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9004", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Addr", ImGuiDataType_U8, &cmd.Addr);
            if (ImGui::Button("Generate CMD##direct_9004")) {
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
        case 9005: {
            UELYSYS_UANT_AddrCmd_t &cmd = command5->uelysys_uant_addrcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_GET_STATUS_CC);
            ImGui::InputScalar("msgid##direct_9005", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9005", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Addr", ImGuiDataType_U8, &cmd.Addr);
            if (ImGui::Button("Generate CMD##direct_9005")) {
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
        case 9007: {
            UELYSYS_UANT_AddrCmd_t &cmd = command5->uelysys_uant_addrcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_GET_BOARD_STATUS_CC);
            ImGui::InputScalar("msgid##direct_9007", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9007", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Addr", ImGuiDataType_U8, &cmd.Addr);
            if (ImGui::Button("Generate CMD##direct_9007")) {
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
        case 9011: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UANT_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UANT_AUTODEPLOY_CC);
            ImGui::InputScalar("msgid##direct_9011", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_9011", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_9011")) {
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
