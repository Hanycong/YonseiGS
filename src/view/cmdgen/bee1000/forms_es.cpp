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

#include "YonseiGS_config.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_model.h"
#include "YonseiGS_s_control.h"
#include "YonseiGS_ftp.h"
#include "YonseiGS_autopilot.h"

#include "YonseiGS_theme.h"
#include "YonseiGS_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include "ccsds_cmd.h"
#include "telecommand_sender.h"
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

            memset(command->cfeesstartappcmd.Application, 0, sizeof(command->cfeesstartappcmd.Application));
            strncpy(command->cfeesstartappcmd.Application, app_buf, sizeof(command->cfeesstartappcmd.Application) - 1);

            memset(command->cfeesstartappcmd.AppEntryPoint, 0, sizeof(command->cfeesstartappcmd.AppEntryPoint));
            strncpy(command->cfeesstartappcmd.AppEntryPoint, entry_buf, sizeof(command->cfeesstartappcmd.AppEntryPoint) - 1);

            memset(command->cfeesstartappcmd.AppFileName, 0, sizeof(command->cfeesstartappcmd.AppFileName));
            strncpy(command->cfeesstartappcmd.AppFileName, filename_buf, sizeof(command->cfeesstartappcmd.AppFileName) - 1);

            mim::tc::EncodeCfeCommand(&command->cfeesstartappcmd, sizeof(CFE_ES_StartAppCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->cfeesstartappcmd, sizeof(CFE_ES_StartAppCmd_t));
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
            memset(command->esstopapp.Application, 0, sizeof(command->esstopapp.Application));
            strncpy(command->esstopapp.Application, app_buf, sizeof(command->esstopapp.Application) - 1);

            mim::tc::EncodeCfeCommand(&command->esstopapp, sizeof(CFE_ES_StopAppCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->esstopapp, sizeof(CFE_ES_StopAppCmd_t));
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
            memset(command->toloadcmd.app, 0, sizeof(command->toloadcmd.app));
            strncpy(command->toloadcmd.app, app_buf, sizeof(command->toloadcmd.app) - 1);

            memset(command->toloadcmd.filename, 0, sizeof(command->toloadcmd.filename));
            strncpy(command->toloadcmd.filename, path_buf, sizeof(command->toloadcmd.filename) - 1);

            mim::tc::EncodeCfeCommand(&command->toloadcmd, sizeof(ES_LoadCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->toloadcmd, sizeof(ES_LoadCmd_t));
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
            memset(command->esqueryonecmd.Application, 0, sizeof(command->esqueryonecmd.Application));
            strncpy(command->esqueryonecmd.Application, app_buf, sizeof(command->esqueryonecmd.Application) - 1);

            mim::tc::EncodeCfeCommand(&command->esqueryonecmd, sizeof(CFE_ES_StopAppCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->esqueryonecmd, sizeof(CFE_ES_StopAppCmd_t));
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
            memset(command->esqueryallcmd.FileName, 0, sizeof(command->esqueryallcmd.FileName));
            strncpy(command->esqueryallcmd.FileName, app_buf, sizeof(command->esqueryallcmd.FileName) - 1);

            mim::tc::EncodeCfeCommand(&command->esqueryallcmd, sizeof(CFE_ES_StopAppCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->esqueryallcmd, sizeof(CFE_ES_StopAppCmd_t));
        }


        break;
    }

    default:
        return false;
    }
    return true;
}
