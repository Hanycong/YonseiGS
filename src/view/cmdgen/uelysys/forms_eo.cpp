// View - 명령 생성기 입력 폼: UELYSYS eo (명령 번호 4800, 4801, 4802, 4803, 4804, 4806)
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

bool DrawUelysysEoForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 4800: { // eo NOOP (CC0)
            static uint16_t msgid=0x1890; static uint8_t fnccode=0;
            ImGui::InputScalar("msgid##direct_4800",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4800",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_4800")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 4801: { // eo RESET_COUNTER (CC1)
            static uint16_t msgid=0x1890; static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_4801",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4801",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_4801")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 4802: { // eo RESET_PHASE (CC2)
            static uint16_t msgid=0x1890; static uint8_t fnccode=2;
            ImGui::InputScalar("msgid##direct_4802",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4802",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_4802")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 4803: { // eo SEND_STATE (CC3)
            static uint16_t msgid=0x1890; static uint8_t fnccode=3;
            ImGui::InputScalar("msgid##direct_4803",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4803",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_4803")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 4804: { // eo FINISH_PHASE (CC4)
            static uint16_t msgid=0x1890; static uint8_t fnccode=4;
            ImGui::InputScalar("msgid##direct_4804",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4804",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_4804")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 4806: { // eo START_CHILD_TASK (CC6)
            static uint16_t msgid=0x1890; static uint8_t fnccode=6;
            struct __attribute__((packed)) Payload { uint32_t ArmMagic; uint16_t SequenceId; uint16_t Reserved; };
            static_assert(sizeof(Payload)==8,"eo START_CHILD_TASK payload size");
            static Payload payload{0x454F4152u,1u,0u};
            ImGui::InputScalar("msgid##direct_4806",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4806",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("ArmMagic##direct_4806",ImGuiDataType_U32,&payload.ArmMagic); ImGui::InputScalar("SequenceId##direct_4806",ImGuiDataType_U16,&payload.SequenceId);
            if(ImGui::Button("Generate CMD##direct_4806")){ WriteSystemName(msgid); uint8_t cmd[16]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        // ==================== UELYSYS payuel_aos ====================
    default:
        return false;
    }
    return true;
}
