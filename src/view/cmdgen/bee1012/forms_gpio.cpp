// View - 명령 생성기 입력 폼: BEE1012 gpio (명령 번호 2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2210, 2211, 2212, 2213, 2214, 2215, 2216, 2217)
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

bool DrawBee1012GpioForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 2200: { // gpio NOOP
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=0;
            ImGui::InputScalar("msgid##direct_2200",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2200",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2200")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2201: { // gpio RESET_COUNTERS
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_2201",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2201",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2201")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2202: { // gpio PROCESS
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=2;
            ImGui::InputScalar("msgid##direct_2202",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2202",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2202")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2203: { // gpio DIGPIOLAY_PARAM
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=3;
            struct Payload { uint32_t ValU32; int16_t ValI16; char ValStr[10]; };
            static_assert(sizeof(Payload)==16,"gpio DIGPIOLAY_PARAM payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2203",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2203",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("ValU32##direct_2203",ImGuiDataType_U32,&payload.ValU32);
            ImGui::InputScalar("ValI16##direct_2203",ImGuiDataType_S16,&payload.ValI16);
            ImGui::InputText("ValStr##direct_2203",payload.ValStr,sizeof(payload.ValStr));
            if(ImGui::Button("Generate CMD##direct_2203")){
                WriteSystemName(msgid); uint8_t cmd[24]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2204: { // gpio LTRX_EN_ON
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=4;
            ImGui::InputScalar("msgid##direct_2204",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2204",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2204")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2205: { // gpio LTRX_EN_OFF
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=5;
            ImGui::InputScalar("msgid##direct_2205",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2205",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2205")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

    case 2206: {
        static uint16_t msgid = GPIO_CMD_ID;
        static uint8_t fnccode = GPIO_DEP1_EN_ON_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(GPIO_Dep1EnOnCmd_t) - 7};

            memcpy(command5->gpiodep1enoncmd.CmdHeader, &mid, 2);
            memcpy(command5->gpiodep1enoncmd.CmdHeader + 2, seq, 2);
            memcpy(command5->gpiodep1enoncmd.CmdHeader + 4, len, 2);
            memcpy(command5->gpiodep1enoncmd.CmdHeader + 6, &fnccode, 1);

            command5->gpiodep1enoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(GPIO_Dep1EnOnCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->gpiodep1enoncmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->gpiodep1enoncmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(GPIO_Dep1EnOnCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(GPIO_Dep1EnOnCmd_t);
            memcpy(pkt->Data, &command5->gpiodep1enoncmd, sizeof(GPIO_Dep1EnOnCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2207: {
        static uint16_t msgid = GPIO_CMD_ID;
        static uint8_t fnccode = GPIO_DEP1_EN_OFF_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(GPIO_Dep1EnOffCmd_t) - 7};

            memcpy(command5->gpiodep1enoffcmd.CmdHeader, &mid, 2);
            memcpy(command5->gpiodep1enoffcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->gpiodep1enoffcmd.CmdHeader + 4, len, 2);
            memcpy(command5->gpiodep1enoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->gpiodep1enoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(GPIO_Dep1EnOffCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->gpiodep1enoffcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->gpiodep1enoffcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(GPIO_Dep1EnOffCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(GPIO_Dep1EnOffCmd_t);
            memcpy(pkt->Data, &command5->gpiodep1enoffcmd, sizeof(GPIO_Dep1EnOffCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2208: {
        static uint16_t msgid = GPIO_CMD_ID;
        static uint8_t fnccode = GPIO_DEP2_EN_ON_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(GPIO_Dep2EnOnCmd_t) - 7};

            memcpy(command5->gpiodep2enoncmd.CmdHeader, &mid, 2);
            memcpy(command5->gpiodep2enoncmd.CmdHeader + 2, seq, 2);
            memcpy(command5->gpiodep2enoncmd.CmdHeader + 4, len, 2);
            memcpy(command5->gpiodep2enoncmd.CmdHeader + 6, &fnccode, 1);

            command5->gpiodep2enoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(GPIO_Dep2EnOnCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->gpiodep2enoncmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->gpiodep2enoncmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(GPIO_Dep2EnOnCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(GPIO_Dep2EnOnCmd_t);
            memcpy(pkt->Data, &command5->gpiodep2enoncmd, sizeof(GPIO_Dep2EnOnCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2209: {
        static uint16_t msgid = GPIO_CMD_ID;
        static uint8_t fnccode = GPIO_DEP2_EN_OFF_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(GPIO_Dep2EnOffCmd_t) - 7};

            memcpy(command5->gpiodep2enoffcmd.CmdHeader, &mid, 2);
            memcpy(command5->gpiodep2enoffcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->gpiodep2enoffcmd.CmdHeader + 4, len, 2);
            memcpy(command5->gpiodep2enoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->gpiodep2enoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(GPIO_Dep2EnOffCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->gpiodep2enoffcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->gpiodep2enoffcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(GPIO_Dep2EnOffCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(GPIO_Dep2EnOffCmd_t);
            memcpy(pkt->Data, &command5->gpiodep2enoffcmd, sizeof(GPIO_Dep2EnOffCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2210: {
        static uint16_t msgid = GPIO_CMD_ID;
        static uint8_t fnccode = GPIO_SP_IN_READ_5S_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(GPIO_SpInRead5sCmd_t) - 7};

            memcpy(command5->gpiospinread5scmd.CmdHeader, &mid, 2);
            memcpy(command5->gpiospinread5scmd.CmdHeader + 2, seq, 2);
            memcpy(command5->gpiospinread5scmd.CmdHeader + 4, len, 2);
            memcpy(command5->gpiospinread5scmd.CmdHeader + 6, &fnccode, 1);

            command5->gpiospinread5scmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(GPIO_SpInRead5sCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->gpiospinread5scmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->gpiospinread5scmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(GPIO_SpInRead5sCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(GPIO_SpInRead5sCmd_t);
            memcpy(pkt->Data, &command5->gpiospinread5scmd, sizeof(GPIO_SpInRead5sCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }


        case 2211: { // gpio STX_EN_ON
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=11;
            ImGui::InputScalar("msgid##direct_2211",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2211",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2211")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2212: { // gpio STX_EN_OFF
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=12;
            ImGui::InputScalar("msgid##direct_2212",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2212",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2212")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2213: { // gpio ADCS_EN_ON
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=13;
            ImGui::InputScalar("msgid##direct_2213",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2213",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2213")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2214: { // gpio ADCS_EN_OFF
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=14;
            ImGui::InputScalar("msgid##direct_2214",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2214",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2214")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2215: { // gpio ADCS_BOOT_ON
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=15;
            ImGui::InputScalar("msgid##direct_2215",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2215",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2215")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2216: { // gpio ADCS_BOOT_OFF
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=16;
            ImGui::InputScalar("msgid##direct_2216",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2216",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2216")){
                WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 2217: { // gpio DEP_BURN
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=17;
            struct Payload { uint8_t Channel; uint8_t Reserved[3]; uint32_t BurnTimeSeconds; };
            static_assert(sizeof(Payload)==8,"gpio DEP_BURN payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2217",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2217",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Channel##direct_2217",ImGuiDataType_U8,&payload.Channel);
            ImGui::InputScalar("BurnTimeSeconds##direct_2217",ImGuiDataType_U32,&payload.BurnTimeSeconds);
            if(ImGui::Button("Generate CMD##direct_2217")){
                WriteSystemName(msgid); uint8_t cmd[16]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        // ==================== pay_slt ====================
    default:
        return false;
    }
    return true;
}
