// View - 명령 생성기 입력 폼: UELYSYS gpio (명령 번호 5804, 5805, 5806, 5807, 5808, 5809, 5810, 5811, 5812, 5813)
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

bool DrawUelysysGpioForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 5804: { // gpio STX_PAY_HIGH
            static uint16_t msgid=0x182B; static uint8_t fnccode=4; ImGui::InputScalar("msgid##direct_5804",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5804",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5804")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5805: { // gpio STX_PAY_LOW
            static uint16_t msgid=0x182B; static uint8_t fnccode=5; ImGui::InputScalar("msgid##direct_5805",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5805",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5805")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5806: { // gpio LGE_MCU_RESET_HIGH
            static uint16_t msgid=0x182B; static uint8_t fnccode=6; ImGui::InputScalar("msgid##direct_5806",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5806",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5806")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5807: { // gpio LGE_MCU_RESET_LOW
            static uint16_t msgid=0x182B; static uint8_t fnccode=7; ImGui::InputScalar("msgid##direct_5807",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5807",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5807")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5808: { // gpio STX_BUS_HIGH
            static uint16_t msgid=0x182B; static uint8_t fnccode=8; ImGui::InputScalar("msgid##direct_5808",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5808",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5808")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5809: { // gpio STX_BUS_LOW
            static uint16_t msgid=0x182B; static uint8_t fnccode=9; ImGui::InputScalar("msgid##direct_5809",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5809",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5809")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5810: { // gpio ADCS_EN_HIGH
            static uint16_t msgid=0x182B; static uint8_t fnccode=10; ImGui::InputScalar("msgid##direct_5810",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5810",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5810")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5811: { // gpio ADCS_EN_LOW
            static uint16_t msgid=0x182B; static uint8_t fnccode=11; ImGui::InputScalar("msgid##direct_5811",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5811",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5811")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5812: { // gpio ADCS_BOOT_HIGH
            static uint16_t msgid=0x182B; static uint8_t fnccode=12; ImGui::InputScalar("msgid##direct_5812",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5812",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5812")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }
        case 5813: { // gpio ADCS_BOOT_LOW
            static uint16_t msgid=0x182B; static uint8_t fnccode=13; ImGui::InputScalar("msgid##direct_5813",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_5813",ImGuiDataType_U8,&fnccode); if(ImGui::Button("Generate CMD##direct_5813")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);} break;
        }

        // ==================== UELYSYS sch_lab ====================
    default:
        return false;
    }
    return true;
}
