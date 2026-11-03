// View - 명령 생성기 입력 폼: SD1-SAT GPS (명령 번호 12000, 12001, 12002, 12003, 12010, 12011, 12012, 12013, 12014, 12015, 12016, 12017, 12018, 12019, 12029, 12030, 12031, 12032, 12033, 12034, 12035, 12036, 12037, 12038, 12039, 12040, 12050, 12051, 12052, 12053, 12054, 12055, 12056, 12057, 12058, 12059, 12100, 12101)
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

bool DrawSd1SatGpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 12000: { //gps noop
            static uint16_t msgid = (uint16_t)(SD1SAT_GPS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(SD1SAT_GPS_NOOP_CC);
            ImGui::InputScalar("msgid##direct_3300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3300")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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


        case 12001: { // gps RESET_COUNTERS
            static uint16_t msgid = (uint16_t)(SD1SAT_GPS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(SD1SAT_GPS_RESET_COUNTERS_CC);
            ImGui::InputScalar("msgid##direct_3300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3300")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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

        case 12002: { // gps get counters
            static uint16_t msgid = (uint16_t)(SD1SAT_GPS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(SD1SAT_GPS_GET_COUNTERS_CC);
            ImGui::InputScalar("msgid##direct_3300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3300")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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

        case 12003: { // gps get app data
            static uint16_t msgid = (uint16_t)(SD1SAT_GPS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(SD1SAT_GPS_GET_APPDATA_CC);
            ImGui::InputScalar("msgid##direct_3300", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3300", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3300")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, 2);
                cmd[2] = 0xC0; cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8); cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
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

        case 12010: { // gps oem cmd log
            static uint16_t msgid = SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode = SD1SAT_GPS_GET_APPDATA_CC;
            static GPS_OEM_Cmd_Log_Payload_t payload{};
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("interfaceIndex", ImGuiDataType_U32, &payload.interfaceIndex);
            ImGui::InputScalar("msgId", ImGuiDataType_U16, &payload.msgId);
            ImGui::InputScalar("type", ImGuiDataType_U8, &payload.type);
            ImGui::InputScalar("port", ImGuiDataType_U32, &payload.port);
            ImGui::InputScalar("trigger", ImGuiDataType_U32, &payload.trigger);
            ImGui::InputScalar("hold", ImGuiDataType_U32, &payload.hold);
            ImGui::InputScalar("period", ImGuiDataType_Double, &payload.period);
            ImGui::InputScalar("offset", ImGuiDataType_Double, &payload.offset);
            if (ImGui::Button("Generate CMD")) {
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

        case 12011: { // gps OEM_CMD_LOG_ONCE
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_LOG_ONCE_CC;
            static GPS_OEM_Cmd_LogOnce_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_3313",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3313",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3313",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("msgId##direct_3313",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("port##direct_3313",ImGuiDataType_U32,&payload.port);
            if(ImGui::Button("Generate CMD##direct_3313")){
                WriteSystemName(msgid); 
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; 
                uint16_t mid=htons(msgid); 
                memcpy(cmd,&mid,2);
                cmd[2]=0xC0; cmd[3]=0; 
                const uint16_t wl=(uint16_t)(sizeof(cmd)-7); 
                cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; 
                cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; 
                uint8_t checksum=0xFF; 
                for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; 
                cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=(uint16_t)sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 12012: { // gps OEM_CMD_LOG_ONTIME
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_LOG_ONTIME_CC;
            static GPS_OEM_Cmd_LogOnTime_Payload_t payload{};
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

        case 12013: { // gps OEM_CMD_LOG_ONCHANGED
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_LOG_ONCHANGED_CC;
            static GPS_OEM_Cmd_LogOnChanged_Payload_t payload{};
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

        case 12014: { // gps OEM_CMD_LOG_ONNEW
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_LOG_ONNEW_CC;
            static GPS_OEM_Cmd_LogOnNew_Payload_t payload{};
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

        case 12015: { // gps OEM_CMD_UNLOG
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_UNLOG_CC;
            static GPS_OEM_Cmd_Unlog_Payload_t payload{};
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

        case 12016: { // gps OEM_CMD_UNLOGALL
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_UNLOGALL_CC;
            static GPS_OEM_Cmd_UnlogAll_Payload_t payload{};
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

        case 12017: { // gps OEM_CMD_ELEVATION_CUTOFF
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_ELEVATION_CUTOFF_CC;
            static GPS_OEM_Cmd_ElevationCutoff_Payload_t payload{};
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

        case 12018: { // gps OEM_CMD_INTERFACE_MODE
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_INTERFACE_MODE_CC;
            static GPS_OEM_Cmd_InterfaceMode_Payload_t payload{};
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

        case 12019: { // gps OEM_CMD_SERIAL_CONFIG
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_SERIAL_CONFIG_CC;
            static GPS_OEM_Cmd_SerialConfig_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_3321",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3321",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("interfaceIndex##direct_3321",ImGuiDataType_S32,&payload.interfaceIndex);
            ImGui::InputScalar("port##direct_3321",ImGuiDataType_U32,&payload.port);
            ImGui::InputScalar("baud##direct_3321",ImGuiDataType_U32,&payload.baud);
            ImGui::InputScalar("parity##direct_3321",ImGuiDataType_U32,&payload.parity);
            ImGui::InputScalar("databits##direct_3321",ImGuiDataType_U32,&payload.databits);
            ImGui::InputScalar("stopbits##direct_3321",ImGuiDataType_U32,&payload.stopbits);
            ImGui::InputScalar("handshake##direct_3321",ImGuiDataType_U32,&payload.handshake);
            ImGui::InputScalar("break##direct_3321",ImGuiDataType_U32,&payload._break);
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

        case 12029: { // gps OEM_CMD_PUBLISH
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_CMD_PUBLISH_CC;
            static GPS_OEM_Cmd_Publish_Payload_t payload{};
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

        case 12030: { // gps OEM_LOG_GET_HANDLER_HK
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_GET_HANDLER_HK_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12031: { // gps OEM_LOG_GET_STAT
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_GET_STAT_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12032: { // gps OEM_LOG_RESET_STAT
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_RESET_STAT_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12033: { // gps OEM_LOG_GET_RECENT_MESSAGE
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_GET_RECENT_MESSAGE_CC;
            static GPS_OEM_Log_GetRecentMessage_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_3331",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3331",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3331",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("offset##direct_3331",ImGuiDataType_U32,&payload.offset);
            if(ImGui::Button("Generate CMD##direct_3331")){
                WriteSystemName(msgid); 
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(payload)]={0}; 
                uint16_t mid=htons(msgid); 
                memcpy(cmd,&mid,2);
                cmd[2]=0xC0; 
                cmd[3]=0;
                const uint16_t wl=(uint16_t)(sizeof(cmd)-7); 
                cmd[4]=(uint8_t)(wl>>8); 
                cmd[5]=(uint8_t)wl; cmd[6]=fnccode;
                memcpy(cmd+8,&payload,sizeof(payload));
                cmd[7]=0; 
                uint8_t checksum=0xFF; 
                for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; 
                cmd[7]=checksum;
                packetsign *pkt=(packetsign*)malloc(2+2+4+sizeof(cmd)); 
                pkt->Identifier=HVD_TEST; 
                pkt->PacketType=MIM_PT_TMTC_TEST; 
                pkt->Length=(uint16_t)sizeof(cmd); 
                memcpy(pkt->Data,cmd,sizeof(cmd));
                mim::svc::WaitTx(); 
                mim::svc::SendUplink((void*)pkt);
            }
            break;
        }

        case 12034: { // gps OEM_LOG_HANDLER_GET_STATUS
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_GET_STATUS_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12035: { // gps OEM_LOG_HANDLER_ACTIVATE
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_ACTIVATE_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12036: { // gps OEM_LOG_HANDLER_DEACTIVATE
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_DEACTIVATE_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12037: { // gps OEM_LOG_HANDLER_GO_DORMANT
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_GO_DORMANT_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12038: { // gps OEM_LOG_HANDLER_WAKEUP
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_WAKEUP_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12039: { // gps OEM_LOG_HANDLER_ACTIVATE_ALL
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_ACTIVATE_ALL_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12040: { // gps OEM_LOG_HANDLER_DEACTIVATE_ALL
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_DEACTIVATE_ALL_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12050: { // gps OEM_LOG_HANDLER_REGISTER
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_REGISTER_CC;
            static GPS_OEM_Log_HandlerRegister_Payload_t payload{};
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

        case 12051: { // gps OEM_LOG_HANDLER_UNREGISTER
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_UNREGISTER_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12052: { // gps OEM_LOG_ADD_CALLBACK
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_ADD_CALLBACK_CC;
            static GPS_OEM_Log_AddCallback_Payload_t payload{};
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

        case 12053: { // gps OEM_LOG_CLEAR_CALLBACKS
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_CLEAR_CALLBACKS_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12054: { // gps OEM_LOG_HANDLER_MARK_BROKEN
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_MARK_BROKEN_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12055: { // gps OEM_LOG_GET_MESSAGE_LENGTH
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_GET_MESSAGE_LENGTH_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12056: { // gps OEM_LOG_GET_HANDLER_NAME
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_GET_HANDLER_NAME_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12057 : { //gps oem log handler set status
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_HANDLER_SET_STATUS_CC;
            static GPS_OEM_Log_HandlerSetStatus_Payload_t payload{};
            ImGui::InputScalar("msgid##direct_3356",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_3356",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("msgId##direct_3356",ImGuiDataType_U16,&payload.msgId);
            ImGui::InputScalar("status##direct_3356",ImGuiDataType_U8,&payload.status);
            ImGui::InputScalar("override##direct_3356",ImGuiDataType_S8,&payload.override);
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

        case 12058: { // gps OEM_LOG_IGNORE_MISSING_CRC
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_IGNORE_MISSING_CRC_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12059: { // gps OEM_LOG_REJECT_MISSING_CRC
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_REJECT_MISSING_CRC_CC;
            static GPS_OEM_HandlerNoArgCmd_Payload_t payload{};
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

        case 12100: { // gps OEM_LOG_LOCK_HANDLERS
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_LOCK_HANDLERS_CC;
            static GPS_OEM_Log_LockHandlers_Payload_t payload{};
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

        case 12101: { // gps OEM_LOG_UNLOCK_HANDLERS
            static uint16_t msgid=SD1SAT_GPS_CMD_ID;
            static uint8_t fnccode=SD1SAT_GPS_OEM_LOG_UNLOCK_HANDLERS_CC;
            static GPS_OEM_Log_UnlockHandlers_Payload_t payload{};
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

    default:
        return false;
    }
    return true;
}
