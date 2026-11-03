// View - 명령 생성기 입력 폼: UELYSYS sch_lab (명령 번호 8003)
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

bool DrawUelysysSchLabForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 8003: { // sch_lab SET_ENTRY_CONFIG (CC3)
            static uint16_t msgid=0x18A3; static uint8_t fnccode=3;
            struct Payload { uint16_t EntryIndex; uint16_t Enabled; uint32_t PacketRate; uint32_t PhaseTicks; };
            static_assert(sizeof(Payload)==12,"UELYSYS sch_lab SET_ENTRY_CONFIG payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_8003",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8003",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("EntryIndex##direct_8003",ImGuiDataType_U16,&payload.EntryIndex); ImGui::InputScalar("Enabled##direct_8003",ImGuiDataType_U16,&payload.Enabled);
            ImGui::InputScalar("PacketRate##direct_8003",ImGuiDataType_U32,&payload.PacketRate); ImGui::InputScalar("PhaseTicks##direct_8003",ImGuiDataType_U32,&payload.PhaseTicks);
            if(ImGui::Button("Generate CMD##direct_8003")){ WriteSystemName(msgid); uint8_t cmd[20]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }

        // ==================== UELYSYS sp ====================
    default:
        return false;
    }
    return true;
}
