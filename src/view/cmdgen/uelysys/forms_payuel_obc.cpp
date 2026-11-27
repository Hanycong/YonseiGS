// View - 명령 생성기 입력 폼: UELYSYS payuel_obc (명령 번호 7200, 7201, 7202, 7203, 7204, 7205, 7206, 7207, 7208, 7209, 7210, 7211, 7212)
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
#include "telecommand_sender.h"
#include "ccsds_cmd.h"
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

bool DrawUelysysPayuelObcForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 7200: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_NOOP_CC);
            ImGui::InputScalar("msgid##direct_7200", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7200", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_7200")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 7201: { // payuel_obc RESET_COUNTERS
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_RESET_COUNTERS_CC); 
            ImGui::InputScalar("msgid##direct_7201",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7201",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_7201")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[8]={0}; 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7202: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_SEND_OBC_BCN_CC);
            ImGui::InputScalar("msgid##direct_7202", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7202", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_7202")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 7203: { // payuel_obc MOTOR_MODE
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_MOTOR_MODE_CC);
            struct __attribute__((packed)) Payload { 
                int8_t L_Speed; 
                int8_t R_Speed; 
                uint8_t Duration; 
            }; 
            static_assert(sizeof(Payload)==3,"payuel_obc MOTOR_MODE payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_7203",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7203",ImGuiDataType_U8,&fnccode); 
            ImGui::InputScalar("L_Speed##direct_7203",ImGuiDataType_S8,&payload.L_Speed); 
            ImGui::InputScalar("R_Speed##direct_7203",ImGuiDataType_S8,&payload.R_Speed); 
            ImGui::InputScalar("Duration##direct_7203",ImGuiDataType_U8,&payload.Duration);
            if(ImGui::Button("Generate CMD##direct_7203")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[11]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7204: {
            UELYSYS_PAYUEL_OBC_CamImageCmd_t &cmd = command5->uelysys_obc_camimagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_CAM_SHOT_CC);
            ImGui::InputScalar("msgid##direct_7204", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7204", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CameraID", ImGuiDataType_U8, &cmd.CameraID);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_7204")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 7205: {
            UELYSYS_PAYUEL_OBC_CamImageCmd_t &cmd = command5->uelysys_obc_camimagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC);
            ImGui::InputScalar("msgid##direct_7205", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7205", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CameraID", ImGuiDataType_U8, &cmd.CameraID);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_7205")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 7206: {
            UELYSYS_PAYUEL_OBC_ChunkDownloadCmd_t &cmd = command5->uelysys_obc_chunkdownloadcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_CHUNK_DOWNLOAD_CC);
            ImGui::InputScalar("msgid##direct_7206", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7206", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CameraID", ImGuiDataType_U8, &cmd.CameraID);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            ImGui::InputScalar("ChunkNumber", ImGuiDataType_U16, &cmd.ChunkNumber);
            if (ImGui::Button("Generate CMD##direct_7206")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 7207: { // payuel_obc SENSOR_META
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_SENSOR_META_CC);
            struct __attribute__((packed)) Payload { 
                uint8_t SensorSlot; 
                uint8_t SensorNumber; 
            }; 
            static_assert(sizeof(Payload)==2,"payuel_obc SENSOR_META payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_7207",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7207",ImGuiDataType_U8,&fnccode); 
            ImGui::InputScalar("SensorSlot##direct_7207",ImGuiDataType_U8,&payload.SensorSlot); 
            ImGui::InputScalar("SensorNumber##direct_7207",ImGuiDataType_U8,&payload.SensorNumber);
            if(ImGui::Button("Generate CMD##direct_7207")){ 
                WriteSystemName(msgid); uint8_t cmd[10]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7208: { // payuel_obc SENSOR_CHUNK
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_SENSOR_CHUNK_CC);
            struct __attribute__((packed)) Payload { 
                uint8_t SensorSlot; 
                uint8_t SensorNumber; 
                uint16_t ChunkNumber; 
            }; 
            static_assert(sizeof(Payload)==4,"payuel_obc SENSOR_CHUNK payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_7208",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7208",ImGuiDataType_U8,&fnccode); 
            ImGui::InputScalar("SensorSlot##direct_7208",ImGuiDataType_U8,&payload.SensorSlot); 
            ImGui::InputScalar("SensorNumber##direct_7208",ImGuiDataType_U8,&payload.SensorNumber); 
            ImGui::InputScalar("ChunkNumber##direct_7208",ImGuiDataType_U16,&payload.ChunkNumber);
            if(ImGui::Button("Generate CMD##direct_7208")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[12]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7209: {
            UELYSYS_PAYUEL_OBC_CamImageCmd_t &cmd = command5->uelysys_obc_camimagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC);
            ImGui::InputScalar("msgid##direct_7209", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_7209", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("CameraID", ImGuiDataType_U8, &cmd.CameraID);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_7209")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 7210: { // payuel_obc DOWNLOAD_SENSOR
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC);
            struct __attribute__((packed)) Payload { 
                uint8_t SensorSlot; 
                uint8_t SensorNumber; 
            }; 
            static_assert(sizeof(Payload)==2,"payuel_obc DOWNLOAD_SENSOR payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_7210",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7210",ImGuiDataType_U8,&fnccode); 
            ImGui::InputScalar("SensorSlot##direct_7210",ImGuiDataType_U8,&payload.SensorSlot); 
            ImGui::InputScalar("SensorNumber##direct_7210",ImGuiDataType_U8,&payload.SensorNumber);
            if(ImGui::Button("Generate CMD##direct_7210")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[10]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7211: { // payuel_obc REPORT_OBC_BCN
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_REPORT_BCN_CC);
            ImGui::InputScalar("msgid##direct_7211",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7211",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_7211")){ 
                WriteSystemName(msgid);
                uint8_t cmd[8]={0}; 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

        case 7212: { // payuel_obc DOWNLOAD_IMAGE_RANGE
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_OBC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_OBC_DOWNLOAD_RANGE_CC);
            struct __attribute__((packed)) Payload { 
                uint8_t ImageSlot; 
                uint8_t ImageNumber; 
                uint16_t StartChunk; 
                uint16_t EndChunk; 
            }; 
            static_assert(sizeof(Payload)==6,"payuel_obc DOWNLOAD_IMAGE_RANGE payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_7212",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_7212",ImGuiDataType_U8,&fnccode); 
            ImGui::InputScalar("ImageSlot##direct_7212",ImGuiDataType_U8,&payload.ImageSlot); 
            ImGui::InputScalar("ImageNumber##direct_7212",ImGuiDataType_U8,&payload.ImageNumber); 
            ImGui::InputScalar("StartChunk##direct_7212",ImGuiDataType_U16,&payload.StartChunk); 
            ImGui::InputScalar("EndChunk##direct_7212",ImGuiDataType_U16,&payload.EndChunk);
            if(ImGui::Button("Generate CMD##direct_7212")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[14]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            } 
            break;
        }

    default:
        return false;
    }
    return true;
}
