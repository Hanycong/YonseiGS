// View - 명령 생성기 입력 폼: BEE1012 gps (명령 번호 3300, 3301, 3312, 3313, 3314, 3315, 3316, 3317, 3318, 3319, 3320, 3321, 3322, 3330, 3331, 3332, 3333, 3334, 3335, 3336, 3337, 3338, 3339, 3350, 3351, 3352, 3353, 3354, 3355, 3356, 3357, 3358, 3359, 3360, 3398, 3399)
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

bool DrawBee1012GpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 3300: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_GPS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_GPS_NOOP_CC);
            ImGui::InputScalar("msgid##direct_3300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3300")) {
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


        case 3301: { // gps RESET_COUNTERS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=1;
            ImGui::InputScalar("msgid##direct_3301",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3301",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_3301")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3312: { // gps OEM_CMD_LOG
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=12;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint8_t type;
                uint8_t padding;
                uint32_t port;
                uint32_t trigger;
                uint32_t hold;
                double period;
                double offset;
            };
            static_assert(sizeof(Payload)==36,"gps OEM_CMD_LOG payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3312",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3312",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3312",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3312",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("type##direct_3312",ImGuiDataType_U8,&payload.type);
            ImGui::InputScalar("port##direct_3312",ImGuiDataType_U32,&payload.port);
            ImGui::InputScalar("trigger##direct_3312",ImGuiDataType_U32,&payload.trigger);
            ImGui::InputScalar("hold##direct_3312",ImGuiDataType_U32,&payload.hold);
            ImGui::InputDouble("period##direct_3312",&payload.period);
            ImGui::InputDouble("offset##direct_3312",&payload.offset);
            if(ImGui::Button("Generate CMD##direct_3312")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3313: { // gps OEM_CMD_LOG_ONCE
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=13;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint32_t port;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3313",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3313",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3313",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3313",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("port##direct_3313",ImGuiDataType_U32,&payload.port);
            if(ImGui::Button("Generate CMD##direct_3313")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3314: { // gps OEM_CMD_LOG_ONTIME
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=14;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint32_t port;
                double period;
                double offset;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3314",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3314",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3314",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3314",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("port##direct_3314",ImGuiDataType_U32,&payload.port);
            ImGui::InputDouble("period##direct_3314",&payload.period);
            ImGui::InputDouble("offset##direct_3314",&payload.offset);
            if(ImGui::Button("Generate CMD##direct_3314")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3315: { // gps OEM_CMD_LOG_ONCHANGED
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=15;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint32_t port;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3315",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3315",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3315",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3315",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("port##direct_3315",ImGuiDataType_U32,&payload.port);
            if(ImGui::Button("Generate CMD##direct_3315")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3316: { // gps OEM_CMD_LOG_ONNEW
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=16;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint32_t port;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3316",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3316",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3316",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3316",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("port##direct_3316",ImGuiDataType_U32,&payload.port);
            if(ImGui::Button("Generate CMD##direct_3316")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3317: { // gps OEM_CMD_UNLOG
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=17;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint8_t type;
                uint32_t port;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3317",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3317",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3317",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3317",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("type##direct_3317",ImGuiDataType_U8,&payload.type);
            ImGui::InputScalar("port##direct_3317",ImGuiDataType_U32,&payload.port);
            if(ImGui::Button("Generate CMD##direct_3317")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3318: { // gps OEM_CMD_UNLOGALL
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=18;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint32_t port;
                bool held;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3318",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3318",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3318",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("port##direct_3318",ImGuiDataType_U32,&payload.port);
            ImGui::Checkbox("held##direct_3318",&payload.held);
            if(ImGui::Button("Generate CMD##direct_3318")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3319: { // gps OEM_CMD_ELEVATION_CUTOFF
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=19;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint32_t constellation;
                float cutoff;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3319",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3319",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3319",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("constellation##direct_3319",ImGuiDataType_U32,&payload.constellation);
            ImGui::InputFloat("cutoff##direct_3319",&payload.cutoff);
            if(ImGui::Button("Generate CMD##direct_3319")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3320: { // gps OEM_CMD_INTERFACE_MODE
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=20;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint32_t port;
                uint32_t rxType;
                uint32_t txType;
                uint32_t responses;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3320",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3320",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3320",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("port##direct_3320",ImGuiDataType_U32,&payload.port);
            ImGui::InputScalar("rxType##direct_3320",ImGuiDataType_U32,&payload.rxType);
            ImGui::InputScalar("txType##direct_3320",ImGuiDataType_U32,&payload.txType);
            ImGui::InputScalar("responses##direct_3320",ImGuiDataType_U32,&payload.responses);
            if(ImGui::Button("Generate CMD##direct_3320")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3321: { // gps OEM_CMD_SERIAL_CONFIG
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=21;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint32_t port;
                uint32_t baud;
                uint32_t parity;
                uint32_t databits;
                uint32_t stopbits;
                uint32_t handshake;
                uint32_t breakValue;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3321",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3321",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3321",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("port##direct_3321",ImGuiDataType_U32,&payload.port);
            ImGui::InputScalar("baud##direct_3321",ImGuiDataType_U32,&payload.baud);
            ImGui::InputScalar("parity##direct_3321",ImGuiDataType_U32,&payload.parity);
            ImGui::InputScalar("databits##direct_3321",ImGuiDataType_U32,&payload.databits);
            ImGui::InputScalar("stopbits##direct_3321",ImGuiDataType_U32,&payload.stopbits);
            ImGui::InputScalar("handshake##direct_3321",ImGuiDataType_U32,&payload.handshake);
            ImGui::InputScalar("breakValue##direct_3321",ImGuiDataType_U32,&payload.breakValue);
            if(ImGui::Button("Generate CMD##direct_3321")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3322: { // gps OEM_CMD_PUBLISH
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=22;
            struct __attribute__((packed)) Payload {
                int32_t interfaceIndex;
                uint16_t msgId;
                uint16_t bodylength;
                uint8_t body[256];
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3322",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3322",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3322",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3322",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("bodylength##direct_3322",ImGuiDataType_U16,&payload.bodylength);
            ImGui::InputText("body (ASCII)##direct_3322",reinterpret_cast<char*>(payload.body),sizeof(payload.body));
            if(ImGui::Button("Generate CMD##direct_3322")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3330: { // gps OEM_LOG_GET_HANDLER_HK
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=30;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_GET_HANDLER_HK payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3330",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3330",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3330",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3330")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3331: { // gps OEM_LOG_GET_STAT
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=31;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_GET_STAT payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3331",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3331",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3331",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3331")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3332: { // gps OEM_LOG_HANDLER_SET_STATUS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=32;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
                uint8_t status;
                bool overrideValue;
            };
            static_assert(sizeof(Payload)==4,"gps OEM_LOG_HANDLER_SET_STATUS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3332",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3332",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3332",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("status##direct_3332",ImGuiDataType_U8,&payload.status);
            ImGui::Checkbox("override##direct_3332",&payload.overrideValue);
            if(ImGui::Button("Generate CMD##direct_3332")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3333: { // gps OEM_LOG_HANDLER_GET_STATUS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=33;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_GET_STATUS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3333",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3333",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3333",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3333")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3334: { // gps OEM_LOG_HANDLER_ACTIVATE
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=34;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_ACTIVATE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3334",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3334",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3334",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3334")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3335: { // gps OEM_LOG_HANDLER_DEACTIVATE
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=35;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_DEACTIVATE payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3335",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3335",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3335",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3335")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3336: { // gps OEM_LOG_HANDLER_GO_DORMANT
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=36;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_GO_DORMANT payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3336",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3336",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3336",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3336")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3337: { // gps OEM_LOG_HANDLER_WAKEUP
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=37;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_WAKEUP payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3337",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3337",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3337",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3337")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3338: { // gps OEM_LOG_HANDLER_ACTIVATE_ALL
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=38;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_ACTIVATE_ALL payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3338",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3338",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3338",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3338")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3339: { // gps OEM_LOG_HANDLER_DEACTIVATE_ALL
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=39;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_DEACTIVATE_ALL payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3339",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3339",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3339",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3339")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3350: { // gps OEM_LOG_HANDLER_REGISTER
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=50;
            struct __attribute__((packed)) Payload {
                char name[16];
                uint16_t msgId;
                uint16_t msgLength;
            };
            static_assert(sizeof(Payload)==20,"gps OEM_LOG_HANDLER_REGISTER payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3350",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3350",ImGuiDataType_U8,&fnccode);
            ImGui::InputText("name##direct_3350",payload.name,sizeof(payload.name));
            ImGui::InputScalar("msgId##direct_3350",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("msgLength##direct_3350",ImGuiDataType_U16,&payload.msgLength);
            if(ImGui::Button("Generate CMD##direct_3350")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3351: { // gps OEM_LOG_HANDLER_UNREGISTER
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=51;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_UNREGISTER payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3351",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3351",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3351",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3351")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3352: { // gps OEM_LOG_ADD_CALLBACK
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=52;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
                uint16_t padding;
                int32_t options;
                char libpath[64];
                char funcName[32];
            };
            static_assert(sizeof(Payload)==104,"gps OEM_LOG_ADD_CALLBACK payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3352",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3352",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3352",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("options##direct_3352",ImGuiDataType_S32,&payload.options);
            ImGui::InputText("libpath##direct_3352",payload.libpath,sizeof(payload.libpath));
            ImGui::InputText("funcName##direct_3352",payload.funcName,sizeof(payload.funcName));
            if(ImGui::Button("Generate CMD##direct_3352")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3353: { // gps OEM_LOG_CLEAR_CALLBACKS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=53;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_CLEAR_CALLBACKS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3353",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3353",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3353",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3353")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3354: { // gps OEM_LOG_HANDLER_MARK_BROKEN
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=54;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_HANDLER_MARK_BROKEN payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3354",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3354",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3354",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3354")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3355: { // gps OEM_LOG_GET_MESSAGE_LENGTH
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=55;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_GET_MESSAGE_LENGTH payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3355",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3355",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3355",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3355")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3356: { // gps OEM_LOG_GET_HANDLER_NAME
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=56;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_GET_HANDLER_NAME payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3356",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3356",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3356",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3356")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3357: { // gps OEM_LOG_RESET_STAT
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=57;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_RESET_STAT payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3357",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3357",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3357",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3357")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3358: { // gps OEM_LOG_GET_RECENT_MESSAGE
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=58;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
                uint32_t offset;
            };
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3358",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3358",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3358",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("offset##direct_3358",ImGuiDataType_U32,&payload.offset);
            if(ImGui::Button("Generate CMD##direct_3358")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3359: { // gps OEM_LOG_REJECT_MISSING_CRC
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=59;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_REJECT_MISSING_CRC payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3359",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3359",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3359",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3359")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3360: { // gps OEM_LOG_IGNORE_MISSING_CRC
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=60;
            struct __attribute__((packed)) Payload {
                uint16_t msgId;
            };
            static_assert(sizeof(Payload)==2,"gps OEM_LOG_IGNORE_MISSING_CRC payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3360",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3360",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3360",ImGuiDataType_U16,&payload.msgId);
            if(ImGui::Button("Generate CMD##direct_3360")){
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3398: { // gps OEM_LOG_LOCK_HANDLERS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=100;
            struct __attribute__((packed)) Payload {
                uint32_t Magic;
            };
            static_assert(sizeof(Payload)==4,"gps OEM_LOG_LOCK_HANDLERS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3398",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3398",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Magic##direct_3398",ImGuiDataType_U32,&payload.Magic);
            if(ImGui::Button("Generate CMD##direct_3398")){
                if(payload.Magic==0) payload.Magic=0xC01DCAFEu;
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 3399: { // gps OEM_LOG_UNLOCK_HANDLERS
            static uint16_t msgid=0x1894;
            static uint8_t fnccode=101;
            struct __attribute__((packed)) Payload {
                uint32_t Magic;
            };
            static_assert(sizeof(Payload)==4,"gps OEM_LOG_UNLOCK_HANDLERS payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_3399",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3399",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Magic##direct_3399",ImGuiDataType_U32,&payload.Magic);
            if(ImGui::Button("Generate CMD##direct_3399")){
                if(payload.Magic==0) payload.Magic=0xA7EDECAFu;
                WriteSystemName(msgid); uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; const uint16_t wl=(uint16_t)(sizeof(cmd)-7); cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        // ==================== ltrx ====================
    default:
        return false;
    }
    return true;
}
