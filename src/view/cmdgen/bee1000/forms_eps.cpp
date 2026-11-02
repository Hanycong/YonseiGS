// View - 명령 생성기 입력 폼: BEE1000 EPS 앱 (명령 번호 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 216)
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
#include "ccsds_cmd.h"
#include "telecommand_sender.h"
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1000EpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 10: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_NOOP_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsnoopcmd, sizeof(EPS_NoopCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsnoopcmd, sizeof(EPS_NoopCmd_t));
        }
        break;
    }

    case 11: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RESET_COUNTERS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsresetcounterscmd, sizeof(EPS_ResetCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsresetcounterscmd, sizeof(EPS_ResetCountersCmd_t));
        }
        break;
    }

    case 12: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_GET_COUNTERS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsgetcounterscmd, sizeof(EPS_GetCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsgetcounterscmd, sizeof(EPS_GetCountersCmd_t));
        }
        break;
    }

    case 13: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_GET_APPDATA_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsgetappdatacmd, sizeof(EPS_GetAppDataCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsgetappdatacmd, sizeof(EPS_GetAppDataCmd_t));
        }
        break;
    }

    case 14: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_REPORT_APPDATA_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsreportappdatacmd, sizeof(EPS_ReportAppDataCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsreportappdatacmd, sizeof(EPS_ReportAppDataCmd_t));
        }
        break;
    }

    case 15: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_SET_CHANNEL_SINGLE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("channel", ImGuiDataType_U8,
                           &command->epsp60docksetchannelsinglecmd.Payload.channel);
        ImGui::InputScalar("value", ImGuiDataType_U8,
                           &command->epsp60docksetchannelsinglecmd.Payload.value);
        ImGui::InputScalar("timeout", ImGuiDataType_U16,
                           &command->epsp60docksetchannelsinglecmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60docksetchannelsinglecmd, sizeof(EPS_P60_Dock_SetChannelSingleCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60docksetchannelsinglecmd, sizeof(EPS_P60_Dock_SetChannelSingleCmd_t));
        }
        break;
    }

    case 16: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_GET_CHANNEL_SINGLE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("channel", ImGuiDataType_U8,
                           &command->epsp60dockgetchannelsinglecmd.Payload.channel);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockgetchannelsinglecmd, sizeof(EPS_P60_Dock_GetChannelSingleCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockgetchannelsinglecmd, sizeof(EPS_P60_Dock_GetChannelSingleCmd_t));
        }
        break;
    }

    case 17: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_SET_CHANNELS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        for (uint8_t i = 0; i < 13; i++) {
            char label[16];
            snprintf(label, sizeof(label), "ch[%u]", i);
            ImGui::InputScalar(label, ImGuiDataType_U8,
                               &command->epsp60docksetchannelscmd.Payload.channels[i]);
        }
        ImGui::InputScalar("timeout", ImGuiDataType_U16,
                           &command->epsp60docksetchannelscmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60docksetchannelscmd, sizeof(EPS_P60_Dock_SetChannelsCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60docksetchannelscmd, sizeof(EPS_P60_Dock_SetChannelsCmd_t));
        }
        break;
    }

    case 18: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_GET_CHANNELS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockgetchannelscmd, sizeof(EPS_P60_Dock_GetChannelsCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockgetchannelscmd, sizeof(EPS_P60_Dock_GetChannelsCmd_t));
        }
        break;
    }

    case 19: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_SET_CHANNEL_SINGLE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("channel", ImGuiDataType_U8,
                           &command->epsp60pdusetchannelsinglecmd.Payload.channel);
        ImGui::InputScalar("value", ImGuiDataType_U8,
                           &command->epsp60pdusetchannelsinglecmd.Payload.value);
        ImGui::InputScalar("timeout", ImGuiDataType_U32,
                           &command->epsp60pdusetchannelsinglecmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdusetchannelsinglecmd, sizeof(EPS_P60_PDU_SetChannelSingleCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdusetchannelsinglecmd, sizeof(EPS_P60_PDU_SetChannelSingleCmd_t));
        }
        break;
    }

    case 20: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_GET_CHANNEL_SINGLE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("channel", ImGuiDataType_U8,
                           &command->epsp60pdugetchannelsinglecmd.Payload.channel);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdugetchannelsinglecmd, sizeof(EPS_P60_PDU_GetChannelSingleCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdugetchannelsinglecmd, sizeof(EPS_P60_PDU_GetChannelSingleCmd_t));
        }
        break;
    }

    case 21: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_SET_CHANNELS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        for (uint8_t i = 0; i < 9; i++) {
            char label[16];
            snprintf(label, sizeof(label), "ch[%u]", i);
            ImGui::InputScalar(label, ImGuiDataType_U8,
                               &command->epsp60pdusetchannelscmd.Payload.channels[i]);
        }
        ImGui::InputScalar("timeout", ImGuiDataType_U32,
                           &command->epsp60pdusetchannelscmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdusetchannelscmd, sizeof(EPS_P60_PDU_SetChannelsCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdusetchannelscmd, sizeof(EPS_P60_PDU_SetChannelsCmd_t));
        }
        break;
    }

    case 22: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_GET_CHANNELS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdugetchannelscmd, sizeof(EPS_P60_PDU_GetChannelsCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdugetchannelscmd, sizeof(EPS_P60_PDU_GetChannelsCmd_t));
        }
        break;
    }

    case 23: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_ACU_SET_MPPT_MODE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("mode", ImGuiDataType_U8,
                           &command->epsp60acusetmpptmodecmd.Payload.mode);
        ImGui::InputScalar("timeout", ImGuiDataType_U32,
                           &command->epsp60acusetmpptmodecmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acusetmpptmodecmd, sizeof(EPS_P60_ACU_SetMpptModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acusetmpptmodecmd, sizeof(EPS_P60_ACU_SetMpptModeCmd_t));
        }
        break;
    }

    case 24: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_ACU_GET_MPPT_MODE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acugetmpptmodecmd, sizeof(EPS_P60_ACU_GetMpptModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acugetmpptmodecmd, sizeof(EPS_P60_ACU_GetMpptModeCmd_t));
        }
        break;
    }

    case 25: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_GET_TABLE_HK_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockgettablehkcmd, sizeof(EPS_P60_Dock_GetTableHkCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockgettablehkcmd, sizeof(EPS_P60_Dock_GetTableHkCmd_t));
        }
        break;
    }

    case 26: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_GET_TABLE_CONF_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockgettableconfcmd, sizeof(EPS_P60_Dock_GetTableConfCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockgettableconfcmd, sizeof(EPS_P60_Dock_GetTableConfCmd_t));
        }
        break;
    }

    case 27: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_DOCK_GET_TABLE_CAL_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockgettablecalcmd, sizeof(EPS_P60_Dock_GetTableCalCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockgettablecalcmd, sizeof(EPS_P60_Dock_GetTableCalCmd_t));
        }
        break;
    }

    case 28: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_GET_TABLE_HK_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdugettablehkcmd, sizeof(EPS_P60_PDU_GetTableHkCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdugettablehkcmd, sizeof(EPS_P60_PDU_GetTableHkCmd_t));
        }
        break;
    }

    case 29: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_GET_TABLE_CONF_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdugettableconfcmd, sizeof(EPS_P60_PDU_GetTableConfCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdugettableconfcmd, sizeof(EPS_P60_PDU_GetTableConfCmd_t));
        }
        break;
    }

    case 30: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_PDU_GET_TABLE_CAL_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pdugettablecalcmd, sizeof(EPS_P60_PDU_GetTableCalCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pdugettablecalcmd, sizeof(EPS_P60_PDU_GetTableCalCmd_t));
        }
        break;
    }

    case 31: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P60_ACU_GET_TABLE_HK_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acugettablehkcmd, sizeof(EPS_P60_ACU_GetTableHkCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acugettablehkcmd, sizeof(EPS_P60_ACU_GetTableHkCmd_t));
        }
        break;
    }

    // 32 : EPS_P60_ACU_GET_TABLE_CONF_CC  (No-Arg)
    case 32: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_ACU_GET_TABLE_CONF_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acugettableconfcmd, sizeof(EPS_P60_ACU_GetTableConfCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acugettableconfcmd, sizeof(EPS_P60_ACU_GetTableConfCmd_t));
        }
        break;
    }

    // 33 : EPS_P60_ACU_GET_TABLE_CAL_CC  (No-Arg)
    case 33: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_ACU_GET_TABLE_CAL_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acugettablecalcmd, sizeof(EPS_P60_ACU_GetTableCalCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acugettablecalcmd, sizeof(EPS_P60_ACU_GetTableCalCmd_t));
        }
        break;
    }

    // 34 : EPS_P60_DOCK_RESET_GND_WDT_CC
    case 34: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_DOCK_RESET_GND_WDT_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("timeout (Dock)", ImGuiDataType_U32,
                           &command->epsp60dockresetgndwdtcmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60dockresetgndwdtcmd, sizeof(EPS_P60_Dock_ResetGndWdtCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60dockresetgndwdtcmd, sizeof(EPS_P60_Dock_ResetGndWdtCmd_t));
        }
        break;
    }

    // 35 : EPS_P60_PDU_RESET_GND_WDT_CC
    case 35: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_PDU_RESET_GND_WDT_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("timeout (PDU)", ImGuiDataType_U32,
                           &command->epsp60pduresetgndwdtcmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60pduresetgndwdtcmd, sizeof(EPS_P60_PDU_ResetGndWdtCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60pduresetgndwdtcmd, sizeof(EPS_P60_PDU_ResetGndWdtCmd_t));
        }
        break;
    }

    // 36 : EPS_P60_ACU_RESET_GND_WDT_CC
    case 36: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_ACU_RESET_GND_WDT_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("timeout (ACU)", ImGuiDataType_U32,
                           &command->epsp60acuresetgndwdtcmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60acuresetgndwdtcmd, sizeof(EPS_P60_ACU_ResetGndWdtCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60acuresetgndwdtcmd, sizeof(EPS_P60_ACU_ResetGndWdtCmd_t));
        }
        break;
    }

    // 37 : EPS_P60_GET_PARAM_CC
    case 37: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_GET_PARAM_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60getparamcmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60getparamcmd.Payload.tableId);
        ImGui::InputScalar("addr",     ImGuiDataType_U16,
                           &command->epsp60getparamcmd.Payload.addr);
        ImGui::InputScalar("type",     ImGuiDataType_U8,
                           &command->epsp60getparamcmd.Payload.type);
        ImGui::InputScalar("size",     ImGuiDataType_U16,
                           &command->epsp60getparamcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60getparamcmd, sizeof(EPS_P60_GetParamCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60getparamcmd, sizeof(EPS_P60_GetParamCmd_t));
        }
        break;
    }

    // 38 : EPS_P60_GET_PARAM_ARRAY_CC
    case 38: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_GET_PARAM_ARRAY_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60getparamarraycmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60getparamarraycmd.Payload.tableId);
        ImGui::InputScalar("addr",     ImGuiDataType_U16,
                           &command->epsp60getparamarraycmd.Payload.addr);
        ImGui::InputScalar("type",     ImGuiDataType_U8,
                           &command->epsp60getparamarraycmd.Payload.type);
        ImGui::InputScalar("size",     ImGuiDataType_U16,
                           &command->epsp60getparamarraycmd.Payload.size);
        ImGui::InputScalar("count",    ImGuiDataType_U16,
                           &command->epsp60getparamarraycmd.Payload.count);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60getparamarraycmd, sizeof(EPS_P60_GetParamArrayCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60getparamarraycmd, sizeof(EPS_P60_GetParamArrayCmd_t));
        }
        break;
    }

    // 39 : EPS_P60_SET_PARAM_CC
    case 39: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_SET_PARAM_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60setparamcmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60setparamcmd.Payload.tableId);
        ImGui::InputScalar("addr",     ImGuiDataType_U16,
                           &command->epsp60setparamcmd.Payload.addr);
        ImGui::InputScalar("type",     ImGuiDataType_U8,
                           &command->epsp60setparamcmd.Payload.type);
        ImGui::InputScalar("size",     ImGuiDataType_U16,
                           &command->epsp60setparamcmd.Payload.size);
        ImGui::InputScalar("timeout",  ImGuiDataType_U16,
                           &command->epsp60setparamcmd.Payload.timeout);

        for (uint8_t i = 0; i < sizeof(command->epsp60setparamcmd.Payload.data); ++i) {
            char label[32];
            snprintf(label, sizeof(label), "data[%u]", i);
            ImGui::InputScalar(label, ImGuiDataType_U8,
                               &command->epsp60setparamcmd.Payload.data[i]);
        }

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60setparamcmd, sizeof(EPS_P60_SetParamCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60setparamcmd, sizeof(EPS_P60_SetParamCmd_t));
        }
        break;
    }

    // 40 : EPS_P60_GET_TABLE_CC
    case 40: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_GET_TABLE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60tablegetcmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60tablegetcmd.Payload.tableId);
        ImGui::InputScalar("rowCount", ImGuiDataType_U8,
                           &command->epsp60tablegetcmd.Payload.rowCount);
        ImGui::InputScalar("size",     ImGuiDataType_U16,
                           &command->epsp60tablegetcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60tablegetcmd, sizeof(EPS_P60_TableGetCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60tablegetcmd, sizeof(EPS_P60_TableGetCmd_t));
        }
        break;
    }

    // 41 : EPS_P60_LOAD_TABLE_CC
    case 41: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_LOAD_TABLE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60tableloadcmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60tableloadcmd.Payload.tableId);
        ImGui::InputScalar("from",     ImGuiDataType_U8,
                           &command->epsp60tableloadcmd.Payload.from);
        ImGui::InputScalar("timeout",  ImGuiDataType_U16,
                           &command->epsp60tableloadcmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60tableloadcmd, sizeof(EPS_P60_TableLoadCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60tableloadcmd, sizeof(EPS_P60_TableLoadCmd_t));
        }
        break;
    }

    // 42 : EPS_P60_SAVE_TABLE_CC
    case 42: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_SAVE_TABLE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60tablesavecmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60tablesavecmd.Payload.tableId);
        ImGui::InputScalar("to",       ImGuiDataType_U8,
                           &command->epsp60tablesavecmd.Payload.to);
        ImGui::InputScalar("timeout",  ImGuiDataType_U16,
                           &command->epsp60tablesavecmd.Payload.timeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60tablesavecmd, sizeof(EPS_P60_TableSaveCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60tablesavecmd, sizeof(EPS_P60_TableSaveCmd_t));
        }
        break;
    }

    // 43 : EPS_P60_TABLE_GET_STATIC_CC
    case 43: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_TABLE_GET_STATIC_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("node",     ImGuiDataType_U8,
                           &command->epsp60tablegetstaticcmd.Payload.node);
        ImGui::InputScalar("tableId",  ImGuiDataType_U8,
                           &command->epsp60tablegetstaticcmd.Payload.tableId);
        ImGui::InputScalar("rowCount", ImGuiDataType_U8,
                           &command->epsp60tablegetstaticcmd.Payload.rowCount);
        ImGui::InputScalar("size",     ImGuiDataType_U16,
                           &command->epsp60tablegetstaticcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60tablegetstaticcmd, sizeof(EPS_P60_TableGetStaticCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60tablegetstaticcmd, sizeof(EPS_P60_TableGetStaticCmd_t));
        }
        break;
    }

    // 44 : EPS_P60_TABLE_DUMP_STATIC_CC
    case 44: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_TABLE_DUMP_STATIC_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("size",    ImGuiDataType_U16,
                           &command->epsp60tabledumpstaticcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60tabledumpstaticcmd, sizeof(EPS_P60_TableDumpStaticCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60tabledumpstaticcmd, sizeof(EPS_P60_TableDumpStaticCmd_t));
        }
        break;
    }

    // 45 : EPS_P60_GET_DOCK_INFO_CC (No-Arg)
    case 45: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_GET_DOCK_INFO_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60getdockinfocmd, sizeof(EPS_P60_GetDockInfoCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60getdockinfocmd, sizeof(EPS_P60_GetDockInfoCmd_t));
        }
        break;
    }

    // 46 : EPS_P60_RESET_CC (No-Arg)
    case 46: {
        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_RESET_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60resetcmd, sizeof(EPS_P60_ResetCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60resetcmd, sizeof(EPS_P60_ResetCmd_t));
        }
        break;
    }


        /********************************************************************/
    /*                       ADCS Commands (47~)                        */
    /********************************************************************/

    /* 47 : ADCS NoOp (CC 0) */
    case 216: { // es noop

        static uint16_t msgid   = 0x1875;
        static uint8_t  fnccode = EPS_P60_GET_DOCK_INFO_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->epsp60getdockinfocmd, sizeof(EPS_P60_GetDockInfoCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->epsp60getdockinfocmd, sizeof(EPS_P60_GetDockInfoCmd_t));
        }
        break;
    }


 //8192, 0, 15

    default:
        return false;
    }
    return true;
}
