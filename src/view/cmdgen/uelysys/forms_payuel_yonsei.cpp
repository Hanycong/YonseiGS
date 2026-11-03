// View - 명령 생성기 입력 폼: UELYSYS payuel_yonsei (명령 번호 7600, 7601, 7607)
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

bool DrawUelysysPayuelYonseiForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 7600: { // payuel_yonsei NOOP
            static uint16_t msgid=0x18CC; static uint8_t fnccode=0; ImGui::InputScalar("msgid##direct_7600",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7600",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_7600")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7601: { // payuel_yonsei RESET_COUNTERS
            static uint16_t msgid=0x18CC; static uint8_t fnccode=1; ImGui::InputScalar("msgid##direct_7601",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7601",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_7601")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7607: { // payuel_yonsei FORWARD_SB
            static uint16_t msgid=0x18CC; static uint8_t fnccode=7; struct Payload { uint16_t TargetMid; uint16_t PayloadLength; uint8_t TargetFcnCode; uint8_t TargetNode; uint8_t TargetPort; uint8_t Spare[1]; uint8_t PayloadBytes[192]; }; static_assert(sizeof(Payload)==200,"payuel_yonsei FORWARD_SB payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_7607",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7607",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("TargetMid##direct_7607",ImGuiDataType_U16,&payload.TargetMid); ImGui::InputScalar("PayloadLength##direct_7607",ImGuiDataType_U16,&payload.PayloadLength); ImGui::InputScalar("TargetFcnCode##direct_7607",ImGuiDataType_U8,&payload.TargetFcnCode); ImGui::InputScalar("TargetNode##direct_7607",ImGuiDataType_U8,&payload.TargetNode); ImGui::InputScalar("TargetPort##direct_7607",ImGuiDataType_U8,&payload.TargetPort); ImGui::InputText("Payload (ASCII, max 191)##direct_7607",reinterpret_cast<char*>(payload.PayloadBytes),sizeof(payload.PayloadBytes));
            if(ImGui::Button("Generate CMD##direct_7607")){ WriteSystemName(msgid); uint8_t cmd[208]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=sizeof(cmd)-7; cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }

        // ==================== UELYSYS payuel_roma ====================
    default:
        return false;
    }
    return true;
}
