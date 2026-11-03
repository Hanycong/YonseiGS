// View - 명령 생성기 입력 폼: BEE1012 sch_lab (명령 번호 3500, 3501, 3502, 3503, 3504)
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

bool DrawBee1012SchLabForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 3500: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_SCH_LAB_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_SCH_LAB_NOOP_CC);
            ImGui::InputScalar("msgid##direct_3500", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3500", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3500")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_NoArgCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_NoArgCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        // ==================== LEGACY / UNCLASSIFIED ====================
        case 3501: { // sch_lab RESET_COUNTERS
            static uint16_t msgid=0x18A3; static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_3501",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_3501",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_3501")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        case 3502: { // sch_lab SET_ENTRY_STATE
            static uint16_t msgid=0x18A3; static uint8_t fnccode=2;
            struct Payload { uint16_t EntryIndex; uint16_t Enabled; }; static_assert(sizeof(Payload)==4,"sch_lab SET_ENTRY_STATE payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_3502",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_3502",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("EntryIndex##direct_3502",ImGuiDataType_U16,&payload.EntryIndex); ImGui::InputScalar("Enabled##direct_3502",ImGuiDataType_U16,&payload.Enabled);
            if(ImGui::Button("Generate CMD##direct_3502")){ WriteSystemName(msgid); uint8_t cmd[12]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        case 3503: { // sch_lab ADD_ENTRY
            static uint16_t msgid=0x18A3; static uint8_t fnccode=3;
            struct Payload { uint16_t EntryIndex; uint16_t FcnCode; uint32_t MessageID; uint32_t PacketRate; uint16_t PayloadLength; uint16_t Enabled; uint8_t MessageBuffer[64]; }; static_assert(sizeof(Payload)==80,"sch_lab ADD_ENTRY payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_3503",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_3503",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("EntryIndex##direct_3503",ImGuiDataType_U16,&payload.EntryIndex);
            ImGui::InputScalar("FcnCode##direct_3503",ImGuiDataType_U16,&payload.FcnCode);
            ImGui::InputScalar("MessageID##direct_3503",ImGuiDataType_U32,&payload.MessageID);
            ImGui::InputScalar("PacketRate##direct_3503",ImGuiDataType_U32,&payload.PacketRate);
            ImGui::InputScalar("PayloadLength##direct_3503",ImGuiDataType_U16,&payload.PayloadLength);
            ImGui::InputScalar("Enabled##direct_3503",ImGuiDataType_U16,&payload.Enabled);
            ImGui::InputText("MessageBuffer (ASCII)##direct_3503",reinterpret_cast<char*>(payload.MessageBuffer),sizeof(payload.MessageBuffer));
            if(ImGui::Button("Generate CMD##direct_3503")){ WriteSystemName(msgid); uint8_t cmd[88]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        case 3504: { // sch_lab DELETE_ENTRY
            static uint16_t msgid=0x18A3; static uint8_t fnccode=4;
            struct Payload { uint16_t EntryIndex; }; static_assert(sizeof(Payload)==2,"sch_lab DELETE_ENTRY payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_3504",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_3504",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("EntryIndex##direct_3504",ImGuiDataType_U16,&payload.EntryIndex);
            if(ImGui::Button("Generate CMD##direct_3504")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        // ==================== UELYSYS eo ====================
    default:
        return false;
    }
    return true;
}
