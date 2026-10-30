// View - 명령 생성기 입력 폼: BEE1000 ES 앱 (명령 번호 217, 218, 219, 220, 221)
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

bool DrawBee1000EsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 217: { // es start app cmd
        static uint16_t msgid = CFE_ES_CMD_MID;
        static uint8_t fnccode = 4;
        static char app_buf[20] = "ADCS";
        static char entry_buf[20] = "ADCS2App_Main";
        static char filename_buf[64] = "/cf/adcs3.so";

        static uint32_t StackSize = 8192;
        static uint8_t  ExceptionAction = 0;
        static uint16_t Priority = 15;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);


        ImGui::InputText("application", app_buf, sizeof(app_buf));
        ImGui::InputText("app entry point", entry_buf, sizeof(entry_buf));
        ImGui::InputText("app file name", filename_buf, sizeof(filename_buf));

        ImGui::InputScalar("Stack Size u32", ImGuiDataType_U32, &StackSize);
        ImGui::InputScalar("Exception Action u8",   ImGuiDataType_U8, &ExceptionAction);
        ImGui::InputScalar("Priority u16",    ImGuiDataType_U16,  &Priority);





        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);


            memset(command->cfeesstartappcmd.Application, 0, sizeof(command->cfeesstartappcmd.Application));
            strncpy(command->cfeesstartappcmd.Application, app_buf, sizeof(command->cfeesstartappcmd.Application) - 1);


            memset(command->cfeesstartappcmd.AppEntryPoint, 0, sizeof(command->cfeesstartappcmd.AppEntryPoint));
            strncpy(command->cfeesstartappcmd.AppEntryPoint, entry_buf, sizeof(command->cfeesstartappcmd.AppEntryPoint) - 1);

            memset(command->cfeesstartappcmd.AppFileName, 0, sizeof(command->cfeesstartappcmd.AppFileName));
            strncpy(command->cfeesstartappcmd.AppFileName, filename_buf, sizeof(command->cfeesstartappcmd.AppFileName) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(CFE_ES_StartAppCmd_t) - 7)};
            memcpy(command->cfeesstartappcmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->cfeesstartappcmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->cfeesstartappcmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->cfeesstartappcmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->cfeesstartappcmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(CFE_ES_StartAppCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(CFE_ES_StartAppCmd_t);
            uint16_t len = sizeof(CFE_ES_StartAppCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->cfeesstartappcmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->cfeesstartappcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->cfeesstartappcmd, sizeof(CFE_ES_StartAppCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }






    case 218: { // ES stop app
        static uint16_t msgid = CFE_ES_CMD_MID;
        static uint8_t fnccode = CFE_ES_STOP_APP_CC;
        static char app_buf[20] = "ADCS";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("application", app_buf, sizeof(app_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->esstopapp.Application, 0, sizeof(command->esstopapp.Application));
            strncpy(command->esstopapp.Application, app_buf, sizeof(command->esstopapp.Application) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(CFE_ES_StopAppCmd_t) - 7)};
            memcpy(command->esstopapp.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->esstopapp.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->esstopapp.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->esstopapp.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->esstopapp.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(CFE_ES_StopAppCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(CFE_ES_StopAppCmd_t);
            uint16_t len = sizeof(CFE_ES_StopAppCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->esstopapp);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->esstopapp.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->esstopapp, sizeof(CFE_ES_StopAppCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }








    case 219: { // ADDED FOR FTP TEST 1210 - ES reload app
        static uint16_t msgid = CFE_ES_CMD_MID;
        static uint8_t fnccode = CFE_ES_RELOAD_APP_CC;
        static char app_buf[20] = "TO_LAB_APP";
        static char path_buf[64] = "/cf/to_lab23.so";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("application", app_buf, sizeof(app_buf));
        ImGui::InputText("app file name", path_buf, sizeof(path_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->toloadcmd.app, 0, sizeof(command->toloadcmd.app));
            strncpy(command->toloadcmd.app, app_buf, sizeof(command->toloadcmd.app) - 1);

            memset(command->toloadcmd.filename, 0, sizeof(command->toloadcmd.filename));
            strncpy(command->toloadcmd.filename, path_buf, sizeof(command->toloadcmd.filename) - 1);

            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(ES_LoadCmd_t) - 7)};
            memcpy(command->toloadcmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->toloadcmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->toloadcmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->toloadcmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->toloadcmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(ES_LoadCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(ES_LoadCmd_t);
            uint16_t len = sizeof(ES_LoadCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->toloadcmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->toloadcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->toloadcmd, sizeof(ES_LoadCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }

    case 220: { // ES QueryOneCmd
        static uint16_t msgid = CFE_ES_CMD_MID;
        static uint8_t fnccode = CFE_ES_QUERY_ONE_CC;
        static char app_buf[20] = "TO_LAB_APP";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("application", app_buf, sizeof(app_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->esqueryonecmd.Application, 0, sizeof(command->esqueryonecmd.Application));
            strncpy(command->esqueryonecmd.Application, app_buf, sizeof(command->esqueryonecmd.Application) - 1);


            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(CFE_ES_StopAppCmd_t) - 7)};
            memcpy(command->esqueryonecmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->esqueryonecmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->esqueryonecmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->esqueryonecmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->esqueryonecmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(CFE_ES_StopAppCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(CFE_ES_StopAppCmd_t);
            uint16_t len = sizeof(CFE_ES_StopAppCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->esqueryonecmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->esqueryonecmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->esqueryonecmd, sizeof(CFE_ES_StopAppCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }

    case 221: { // ES QueryAllCmd
        static uint16_t msgid = CFE_ES_CMD_MID;
        static uint8_t fnccode = CFE_ES_QUERY_ALL_CC;
        static char app_buf[64] = "TO_LAB_APP";

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("application", app_buf, sizeof(app_buf));


        if (ImGui::Button("Generate CMD")) {
            uint16_t msgid_be = htons(msgid);
            memset(command->esqueryallcmd.FileName, 0, sizeof(command->esqueryallcmd.FileName));
            strncpy(command->esqueryallcmd.FileName, app_buf, sizeof(command->esqueryallcmd.FileName) - 1);


            uint8_t sequence[2] = {0xC0, 0x00};
            uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(CFE_ES_StopAppCmd_t) - 7)};
            memcpy(command->esqueryallcmd.CmdHeader + 0, &msgid_be, sizeof(uint16_t));
            memcpy(command->esqueryallcmd.CmdHeader + 2, sequence, sizeof(uint16_t));
            memcpy(command->esqueryallcmd.CmdHeader + 4, length, sizeof(uint16_t));
            memcpy(command->esqueryallcmd.CmdHeader + 6, &fnccode, sizeof(uint8_t));


            command->esqueryallcmd.CmdHeader[7] = 0x00;

            mim::svc::WaitTx();
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + sizeof(CFE_ES_StopAppCmd_t));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;

            TestPacket->Length = sizeof(CFE_ES_StopAppCmd_t);
            uint16_t len = sizeof(CFE_ES_StopAppCmd_t);


            const uint8_t* byteptr = reinterpret_cast<const uint8_t*>(&command->esqueryallcmd);

            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            memcpy(command->esqueryallcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));
            memcpy(TestPacket->Data, &command->esqueryallcmd, sizeof(CFE_ES_StopAppCmd_t));
            mim::svc::SendUplink((void*)TestPacket);
        }


        break;
    }

    default:
        return false;
    }
    return true;
}
