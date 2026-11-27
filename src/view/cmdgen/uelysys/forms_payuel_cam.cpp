// View - 명령 생성기 입력 폼: UELYSYS payuel_cam (명령 번호 6600, 6601, 6602, 6603, 6604, 6605, 6606, 6607, 6608, 6609, 6610)
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

bool DrawUelysysPayuelCamForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 6600: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_NOOP_CC);
            ImGui::InputScalar("msgid##direct_6600", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6600", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6600")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        // ==================== UELYSYS payuel_cam ====================
        case 6601: { // payuel_cam RESET_COUNTERS
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_RESET_COUNTERS_CC);
            ImGui::InputScalar("msgid##direct_6601",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_6601",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_6601")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[8]={0}; 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            }
            break;
        }
        
        case 6602: {
            UELYSYS_PAYUEL_CAM_ShotCmd_t &cmd = command5->uelysys_cam_shotcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_SHOT_CC);
            ImGui::InputScalar("msgid##direct_6602", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6602", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ImageSlot", ImGuiDataType_U8, &cmd.ImageSlot);
            ImGui::InputScalar("CameraNumber", ImGuiDataType_U8, &cmd.CameraNumber);
            if (ImGui::Button("Generate CMD##direct_6602")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 6603: { // payuel_cam HEALTH_CHECK
            static uint16_t msgid=(uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID); 
            static uint8_t fnccode=(uint8_t)(UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC);
            ImGui::InputScalar("msgid##direct_6603",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_6603",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_6603")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[8]={0}; 
                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            }
            break;
        }

        case 6604: {
            UELYSYS_PAYUEL_CAM_ImageCmd_t &cmd = command5->uelysys_cam_imagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC);
            ImGui::InputScalar("msgid##direct_6604", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6604", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ImageSlot", ImGuiDataType_U8, &cmd.ImageSlot);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_6604")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6605: {
            UELYSYS_PAYUEL_CAM_ImageCmd_t &cmd = command5->uelysys_cam_imagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC);
            ImGui::InputScalar("msgid##direct_6605", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6605", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ImageSlot", ImGuiDataType_U8, &cmd.ImageSlot);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_6605")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 6606: {
            UELYSYS_PAYUEL_CAM_ChunkDownloadCmd_t &cmd = command5->uelysys_cam_chunkdownloadcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_CHUNK_DOWNLOAD_CC);
            ImGui::InputScalar("msgid##direct_6606", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6606", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ImageSlot", ImGuiDataType_U8, &cmd.ImageSlot);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            ImGui::InputScalar("ChunkNumber", ImGuiDataType_U16, &cmd.ChunkNumber);
            if (ImGui::Button("Generate CMD##direct_6606")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 6607: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_SEND_BCN_CC);
            ImGui::InputScalar("msgid##direct_6607", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6607", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6607")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 6608: {
            UELYSYS_PAYUEL_CAM_ImageCmd_t &cmd = command5->uelysys_cam_imagecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC);
            ImGui::InputScalar("msgid##direct_6608", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6608", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("ImageSlot", ImGuiDataType_U8, &cmd.ImageSlot);
            ImGui::InputScalar("ImageNumber", ImGuiDataType_U8, &cmd.ImageNumber);
            if (ImGui::Button("Generate CMD##direct_6608")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_PAYUEL_CAM_ImageCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 6609: { // payuel_cam DOWNLOAD_IMAGE_RANGE
            static uint16_t msgid=(uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);; 
            static uint8_t fnccode=(uint8_t)(UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC);
            struct __attribute__((packed)) Payload { 
                uint8_t ImageSlot; 
                uint8_t ImageNumber; 
                uint16_t StartChunk; 
                uint16_t EndChunk; 
            }; 
            static_assert(sizeof(Payload)==6,"payuel_cam DOWNLOAD_IMAGE_RANGE payload size"); 
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_6609",ImGuiDataType_U16,&msgid); 
            ImGui::InputScalar("fnccode##direct_6609",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("ImageSlot##direct_6609",ImGuiDataType_U8,&payload.ImageSlot); 
            ImGui::InputScalar("ImageNumber##direct_6609",ImGuiDataType_U8,&payload.ImageNumber); 
            ImGui::InputScalar("StartChunk##direct_6609",ImGuiDataType_U16,&payload.StartChunk); 
            ImGui::InputScalar("EndChunk##direct_6609",ImGuiDataType_U16,&payload.EndChunk);
            if(ImGui::Button("Generate CMD##direct_6609")){ 
                WriteSystemName(msgid); 
                uint8_t cmd[14]={0}; 
                memcpy(cmd+8,&payload,sizeof(payload)); 
                mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); 
                mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); 
            }
            break;
        }

        case 6610 : {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_PAYUEL_CAM_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_PAYUEL_CAM_REPORT_BCN_CC);
            ImGui::InputScalar("msgid##direct_6607", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_6607", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_6607")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_NoArgCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(UELYSYS_NoArgCmd_t)), MIM_PT_TMTC_TEST);
            }
            break;
        }

    default:
        return false;
    }
    return true;
}
