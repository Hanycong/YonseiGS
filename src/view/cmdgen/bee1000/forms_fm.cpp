// View - 명령 생성기 입력 폼: BEE1000 FM 앱 (명령 번호 201, 202, 204, 213)
//
// cmdgen_view.cpp 의 명령 번호 switch 에서 이 앱의 case 를 내용 그대로 옮긴 것.
// 처리한 명령이면 true.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1000FmForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 201: { // ADDED FOR FTP TEST 1210, delete To_lab.so
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 5;
        static char filename_buf[64] = "/cf/to_lab.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Filename (char 64)", filename_buf, sizeof(filename_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);

            memset(command->todeletecmd.filename, 0, sizeof(command->todeletecmd.filename));
            strncpy(command->todeletecmd.filename, filename_buf, sizeof(command->todeletecmd.filename) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(TO_DeleteCmd_t) - 7)};
            memcpy(command->todeletecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->todeletecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->todeletecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->todeletecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->todeletecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(TO_DeleteCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(TO_DeleteCmd_t);
            uint16_t len = sizeof(TO_DeleteCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->todeletecmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->todeletecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->todeletecmd, sizeof(TO_DeleteCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }










    case 202: { // ADDED FOR FTP TEST 1210, rename To_lab.so
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 4;
        static char source_buf[64] = "/cf/to_lab23.so";
        static char target_buf[64] = "/cf/to_lab.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Source (char 64)", source_buf, sizeof(source_buf));
        ImGui::InputText("Target (char 64)", target_buf, sizeof(target_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->torenamecmd.source, 0, sizeof(command->torenamecmd.source));
            strncpy(command->torenamecmd.source, source_buf, sizeof(command->torenamecmd.source) - 1);

            memset(command->torenamecmd.target, 0, sizeof(command->torenamecmd.target));
            strncpy(command->torenamecmd.target, target_buf, sizeof(command->torenamecmd.target) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(TO_RenameCmd_t) - 7)};
            memcpy(command->torenamecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->torenamecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->torenamecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->torenamecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->torenamecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(TO_RenameCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(TO_RenameCmd_t);
            uint16_t len = sizeof(TO_RenameCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->torenamecmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->torenamecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->torenamecmd, sizeof(TO_RenameCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }








    case 204: { // FTP_filenameCmd
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 10;
        static char path_buf[64] = "/cf/adcs.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("path", path_buf, sizeof(path_buf));
        ImGui::InputScalar("padding u32",   ImGuiDataType_U32, &command->ftpfilenamecmd.padding);


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->ftpfilenamecmd.path, 0, sizeof(command->ftpfilenamecmd.path));
            strncpy(command->ftpfilenamecmd.path, path_buf, sizeof(command->ftpfilenamecmd.path) - 1);
            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t length[2] = {0x00, 0x45};
            memcpy(command->ftpfilenamecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->ftpfilenamecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->ftpfilenamecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->ftpfilenamecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));
            command->ftpfilenamecmd.CmdHeader[7] = 0x00;
            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(FTP_filenameCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = sizeof(FTP_filenameCmd_t);
            uint16_t len = sizeof(FTP_filenameCmd_t);
            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->ftpfilenamecmd);
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->ftpfilenamecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->ftpfilenamecmd, sizeof(FTP_filenameCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
            command->ftpfilenamecmd.CmdHeader[0],
            command->ftpfilenamecmd.CmdHeader[1],
            command->ftpfilenamecmd.CmdHeader[2],
            command->ftpfilenamecmd.CmdHeader[3],
            command->ftpfilenamecmd.CmdHeader[4],
            command->ftpfilenamecmd.CmdHeader[5],
            command->ftpfilenamecmd.CmdHeader[6],
            command->ftpfilenamecmd.CmdHeader[7]);

        ImGui::Text("Params: path = %s", path_buf);
        ImGui::Text("  padding    = %u", command->ftpfilenamecmd.padding);

        break;
    }


case 213: { // FTP_filenameCmd
        static uint16_t msgid = 0x1815;
        static uint8_t fnccode = 15;
        static char path_buf[64] = "/cf/sdcard/";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("path", path_buf, sizeof(path_buf));
        ImGui::InputScalar("DirListOffset u32",   ImGuiDataType_U32, &command->ftpdirlistpktcmd.DirListOffset);
        ImGui::InputScalar("GetSizeTimeMode u8",   ImGuiDataType_U8, &command->ftpdirlistpktcmd.GetSizeTimeMode);
        ImGui::InputScalar("Spare01[0] u8",   ImGuiDataType_U8, &command->ftpdirlistpktcmd.Spare01_0);
        ImGui::InputScalar("Spare01[1] u8",   ImGuiDataType_U8, &command->ftpdirlistpktcmd.Spare01_1);
        ImGui::InputScalar("Spare01[2] u8",   ImGuiDataType_U8, &command->ftpdirlistpktcmd.Spare01_2);

        if (ImGui::Button("Generate CMD")) {

            uint16_t msgid_be = htons(msgid);
            memset(command->ftpdirlistpktcmd.path, 0, sizeof(command->ftpdirlistpktcmd.path));
            strncpy(command->ftpdirlistpktcmd.path, path_buf, sizeof(command->ftpdirlistpktcmd.path) - 1);
            
            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t length[2] = {0x00, (uint8_t)(sizeof(FTP_dirlistpktCmd_t) - 7)};
            memcpy(command->ftpdirlistpktcmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->ftpdirlistpktcmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->ftpdirlistpktcmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->ftpdirlistpktcmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));
            command->ftpdirlistpktcmd.CmdHeader[7] = 0x00;
            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(FTP_dirlistpktCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = sizeof(FTP_dirlistpktCmd_t);
            uint16_t len = sizeof(FTP_dirlistpktCmd_t);
            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->ftpdirlistpktcmd);
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->ftpdirlistpktcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->ftpdirlistpktcmd, sizeof(FTP_dirlistpktCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
            command->ftpdirlistpktcmd.CmdHeader[0],
            command->ftpdirlistpktcmd.CmdHeader[1],
            command->ftpdirlistpktcmd.CmdHeader[2],
            command->ftpdirlistpktcmd.CmdHeader[3],
            command->ftpdirlistpktcmd.CmdHeader[4],
            command->ftpdirlistpktcmd.CmdHeader[5],
            command->ftpdirlistpktcmd.CmdHeader[6],
            command->ftpdirlistpktcmd.CmdHeader[7]);

        // ImGui::Text("Params: path = %s", path_buf);
        ImGui::Text("Params:  Path            : %s", command->ftpdirlistpktcmd.path);

        // 2. 주요 설정값 확인
        ImGui::Text("  DirListOffset   : %u", command->ftpdirlistpktcmd.DirListOffset);
        ImGui::Text("  GetSizeTimeMode : %u", command->ftpdirlistpktcmd.GetSizeTimeMode);

        // 3. Spare(Padding) 값 확인 - 16진수로 보는 것이 편함
        ImGui::Text("  Spare Bytes     : [%02X] [%02X] [%02X]", 
            command->ftpdirlistpktcmd.Spare01_0,
            command->ftpdirlistpktcmd.Spare01_1,
            command->ftpdirlistpktcmd.Spare01_2);

        break;
    }
    default:
        return false;
    }
    return true;
}
