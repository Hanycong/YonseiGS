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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_NoopCmd_t) - 7};

            memcpy(command->epsnoopcmd.CmdHeader, &mid, 2);
            memcpy(command->epsnoopcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsnoopcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsnoopcmd.CmdHeader + 6, &fnccode, 1);

            command->epsnoopcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_NoopCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsnoopcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsnoopcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_NoopCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_NoopCmd_t);
            memcpy(pkt->Data, &command->epsnoopcmd, sizeof(EPS_NoopCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_ResetCountersCmd_t) - 7};

            memcpy(command->epsresetcounterscmd.CmdHeader, &mid, 2);
            memcpy(command->epsresetcounterscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsresetcounterscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsresetcounterscmd.CmdHeader + 6, &fnccode, 1);

            command->epsresetcounterscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_ResetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsresetcounterscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsresetcounterscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_ResetCountersCmd_t);
            memcpy(pkt->Data, &command->epsresetcounterscmd, sizeof(EPS_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_GetCountersCmd_t) - 7};

            memcpy(command->epsgetcounterscmd.CmdHeader, &mid, 2);
            memcpy(command->epsgetcounterscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsgetcounterscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsgetcounterscmd.CmdHeader + 6, &fnccode, 1);

            command->epsgetcounterscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_GetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsgetcounterscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsgetcounterscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_GetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_GetCountersCmd_t);
            memcpy(pkt->Data, &command->epsgetcounterscmd, sizeof(EPS_GetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_GetAppDataCmd_t) - 7};

            memcpy(command->epsgetappdatacmd.CmdHeader, &mid, 2);
            memcpy(command->epsgetappdatacmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsgetappdatacmd.CmdHeader + 4, len, 2);
            memcpy(command->epsgetappdatacmd.CmdHeader + 6, &fnccode, 1);

            command->epsgetappdatacmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_GetAppDataCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsgetappdatacmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsgetappdatacmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_GetAppDataCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_GetAppDataCmd_t);
            memcpy(pkt->Data, &command->epsgetappdatacmd, sizeof(EPS_GetAppDataCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_ReportAppDataCmd_t) - 7};

            memcpy(command->epsreportappdatacmd.CmdHeader, &mid, 2);
            memcpy(command->epsreportappdatacmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsreportappdatacmd.CmdHeader + 4, len, 2);
            memcpy(command->epsreportappdatacmd.CmdHeader + 6, &fnccode, 1);

            command->epsreportappdatacmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_ReportAppDataCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsreportappdatacmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsreportappdatacmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_ReportAppDataCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_ReportAppDataCmd_t);
            memcpy(pkt->Data, &command->epsreportappdatacmd, sizeof(EPS_ReportAppDataCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_SetChannelSingleCmd_t) - 7};

            memcpy(command->epsp60docksetchannelsinglecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60docksetchannelsinglecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60docksetchannelsinglecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60docksetchannelsinglecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60docksetchannelsinglecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_SetChannelSingleCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60docksetchannelsinglecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60docksetchannelsinglecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_SetChannelSingleCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_SetChannelSingleCmd_t);
            memcpy(pkt->Data, &command->epsp60docksetchannelsinglecmd,
                   sizeof(EPS_P60_Dock_SetChannelSingleCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_GetChannelSingleCmd_t) - 7};

            memcpy(command->epsp60dockgetchannelsinglecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60dockgetchannelsinglecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60dockgetchannelsinglecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60dockgetchannelsinglecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60dockgetchannelsinglecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_GetChannelSingleCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60dockgetchannelsinglecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60dockgetchannelsinglecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_GetChannelSingleCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_GetChannelSingleCmd_t);
            memcpy(pkt->Data, &command->epsp60dockgetchannelsinglecmd,
                   sizeof(EPS_P60_Dock_GetChannelSingleCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_SetChannelsCmd_t) - 7};

            memcpy(command->epsp60docksetchannelscmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60docksetchannelscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60docksetchannelscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60docksetchannelscmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60docksetchannelscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_SetChannelsCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60docksetchannelscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60docksetchannelscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_SetChannelsCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_SetChannelsCmd_t);
            memcpy(pkt->Data, &command->epsp60docksetchannelscmd,
                   sizeof(EPS_P60_Dock_SetChannelsCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_GetChannelsCmd_t) - 7};

            memcpy(command->epsp60dockgetchannelscmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60dockgetchannelscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60dockgetchannelscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60dockgetchannelscmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60dockgetchannelscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_GetChannelsCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60dockgetchannelscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60dockgetchannelscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_GetChannelsCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_GetChannelsCmd_t);
            memcpy(pkt->Data, &command->epsp60dockgetchannelscmd,
                   sizeof(EPS_P60_Dock_GetChannelsCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_SetChannelSingleCmd_t) - 7};

            memcpy(command->epsp60pdusetchannelsinglecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdusetchannelsinglecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdusetchannelsinglecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdusetchannelsinglecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdusetchannelsinglecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_SetChannelSingleCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdusetchannelsinglecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdusetchannelsinglecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_SetChannelSingleCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_SetChannelSingleCmd_t);
            memcpy(pkt->Data, &command->epsp60pdusetchannelsinglecmd,
                   sizeof(EPS_P60_PDU_SetChannelSingleCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_GetChannelSingleCmd_t) - 7};

            memcpy(command->epsp60pdugetchannelsinglecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdugetchannelsinglecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdugetchannelsinglecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdugetchannelsinglecmd.CmdHeader + 6, &fnccode, 1);


            command->epsp60pdugetchannelsinglecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_GetChannelSingleCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdugetchannelsinglecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdugetchannelsinglecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_GetChannelSingleCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_GetChannelSingleCmd_t);
            memcpy(pkt->Data, &command->epsp60pdugetchannelsinglecmd,
                   sizeof(EPS_P60_PDU_GetChannelSingleCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_SetChannelsCmd_t) - 7};

            memcpy(command->epsp60pdusetchannelscmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdusetchannelscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdusetchannelscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdusetchannelscmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdusetchannelscmd.CmdHeader[7] = 0;

            uint16_t total = sizeof(EPS_P60_PDU_SetChannelsCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdusetchannelscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdusetchannelscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_SetChannelsCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_SetChannelsCmd_t);
            memcpy(pkt->Data, &command->epsp60pdusetchannelscmd,
                   sizeof(EPS_P60_PDU_SetChannelsCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_GetChannelsCmd_t) - 7};

            memcpy(command->epsp60pdugetchannelscmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdugetchannelscmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdugetchannelscmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdugetchannelscmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdugetchannelscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_GetChannelsCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdugetchannelscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdugetchannelscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_GetChannelsCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_GetChannelsCmd_t);
            memcpy(pkt->Data, &command->epsp60pdugetchannelscmd,
                   sizeof(EPS_P60_PDU_GetChannelsCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_ACU_SetMpptModeCmd_t) - 7};

            memcpy(command->epsp60acusetmpptmodecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60acusetmpptmodecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60acusetmpptmodecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60acusetmpptmodecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60acusetmpptmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ACU_SetMpptModeCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60acusetmpptmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60acusetmpptmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_ACU_SetMpptModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_ACU_SetMpptModeCmd_t);
            memcpy(pkt->Data, &command->epsp60acusetmpptmodecmd,
                   sizeof(EPS_P60_ACU_SetMpptModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_ACU_GetMpptModeCmd_t) - 7};

            memcpy(command->epsp60acugetmpptmodecmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60acugetmpptmodecmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60acugetmpptmodecmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60acugetmpptmodecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60acugetmpptmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ACU_GetMpptModeCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60acugetmpptmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60acugetmpptmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_ACU_GetMpptModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_ACU_GetMpptModeCmd_t);
            memcpy(pkt->Data, &command->epsp60acugetmpptmodecmd,
                   sizeof(EPS_P60_ACU_GetMpptModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_GetTableHkCmd_t) - 7};

            memcpy(command->epsp60dockgettablehkcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60dockgettablehkcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60dockgettablehkcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60dockgettablehkcmd.CmdHeader + 6, &fnccode, 1);


            command->epsp60dockgettablehkcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_GetTableHkCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60dockgettablehkcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60dockgettablehkcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_GetTableHkCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_GetTableHkCmd_t);
            memcpy(pkt->Data, &command->epsp60dockgettablehkcmd,
                   sizeof(EPS_P60_Dock_GetTableHkCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_GetTableConfCmd_t) - 7};

            memcpy(command->epsp60dockgettableconfcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60dockgettableconfcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60dockgettableconfcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60dockgettableconfcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60dockgettableconfcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_GetTableConfCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60dockgettableconfcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60dockgettableconfcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_GetTableConfCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_GetTableConfCmd_t);
            memcpy(pkt->Data, &command->epsp60dockgettableconfcmd,
                   sizeof(EPS_P60_Dock_GetTableConfCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_Dock_GetTableCalCmd_t) - 7};

            memcpy(command->epsp60dockgettablecalcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60dockgettablecalcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60dockgettablecalcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60dockgettablecalcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60dockgettablecalcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_Dock_GetTableCalCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60dockgettablecalcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60dockgettablecalcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_Dock_GetTableCalCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_Dock_GetTableCalCmd_t);
            memcpy(pkt->Data, &command->epsp60dockgettablecalcmd,
                   sizeof(EPS_P60_Dock_GetTableCalCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_GetTableHkCmd_t) - 7};

            memcpy(command->epsp60pdugettablehkcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdugettablehkcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdugettablehkcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdugettablehkcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdugettablehkcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_GetTableHkCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdugettablehkcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdugettablehkcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_GetTableHkCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_GetTableHkCmd_t);
            memcpy(pkt->Data, &command->epsp60pdugettablehkcmd,
                   sizeof(EPS_P60_PDU_GetTableHkCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_GetTableConfCmd_t) - 7};

            memcpy(command->epsp60pdugettableconfcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdugettableconfcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdugettableconfcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdugettableconfcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdugettableconfcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_GetTableConfCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdugettableconfcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdugettableconfcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_GetTableConfCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_GetTableConfCmd_t);
            memcpy(pkt->Data, &command->epsp60pdugettableconfcmd,
                   sizeof(EPS_P60_PDU_GetTableConfCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_PDU_GetTableCalCmd_t) - 7};

            memcpy(command->epsp60pdugettablecalcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60pdugettablecalcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60pdugettablecalcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60pdugettablecalcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60pdugettablecalcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_PDU_GetTableCalCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60pdugettablecalcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60pdugettablecalcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_PDU_GetTableCalCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_PDU_GetTableCalCmd_t);
            memcpy(pkt->Data, &command->epsp60pdugettablecalcmd,
                   sizeof(EPS_P60_PDU_GetTableCalCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P60_ACU_GetTableHkCmd_t) - 7};

            memcpy(command->epsp60acugettablehkcmd.CmdHeader, &mid, 2);
            memcpy(command->epsp60acugettablehkcmd.CmdHeader + 2, seq, 2);
            memcpy(command->epsp60acugettablehkcmd.CmdHeader + 4, len, 2);
            memcpy(command->epsp60acugettablehkcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60acugettablehkcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ACU_GetTableHkCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60acugettablehkcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command->epsp60acugettablehkcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P60_ACU_GetTableHkCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P60_ACU_GetTableHkCmd_t);
            memcpy(pkt->Data, &command->epsp60acugettablehkcmd,
                   sizeof(EPS_P60_ACU_GetTableHkCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00,
                               (uint8_t)(sizeof(EPS_P60_ACU_GetTableConfCmd_t) - 7)};

            memcpy(command->epsp60acugettableconfcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60acugettableconfcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60acugettableconfcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60acugettableconfcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60acugettableconfcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ACU_GetTableConfCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60acugettableconfcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60acugettableconfcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 +
                                 sizeof(EPS_P60_ACU_GetTableConfCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_ACU_GetTableConfCmd_t);
            memcpy(pkt->Data, &command->epsp60acugettableconfcmd,
                   sizeof(EPS_P60_ACU_GetTableConfCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00,
                               (uint8_t)(sizeof(EPS_P60_ACU_GetTableCalCmd_t) - 7)};

            memcpy(command->epsp60acugettablecalcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60acugettablecalcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60acugettablecalcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60acugettablecalcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60acugettablecalcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ACU_GetTableCalCmd_t);
            const uint8_t *p = (const uint8_t *)&command->epsp60acugettablecalcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60acugettablecalcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 +
                                 sizeof(EPS_P60_ACU_GetTableCalCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_ACU_GetTableCalCmd_t);
            memcpy(pkt->Data, &command->epsp60acugettablecalcmd,
                   sizeof(EPS_P60_ACU_GetTableCalCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_Dock_ResetGndWdtCmd_t) - 7)
            };

            memcpy(command->epsp60dockresetgndwdtcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60dockresetgndwdtcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60dockresetgndwdtcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60dockresetgndwdtcmd.CmdHeader + 6, &fnccode, 1);

            uint16_t total = sizeof(EPS_P60_Dock_ResetGndWdtCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60dockresetgndwdtcmd;

            command->epsp60dockresetgndwdtcmd.CmdHeader[7] = 0;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60dockresetgndwdtcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_Dock_ResetGndWdtCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_Dock_ResetGndWdtCmd_t);
            memcpy(pkt->Data, &command->epsp60dockresetgndwdtcmd,
                   sizeof(EPS_P60_Dock_ResetGndWdtCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_PDU_ResetGndWdtCmd_t) - 7)
            };

            memcpy(command->epsp60pduresetgndwdtcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60pduresetgndwdtcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60pduresetgndwdtcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60pduresetgndwdtcmd.CmdHeader + 6, &fnccode, 1);

            uint16_t total = sizeof(EPS_P60_PDU_ResetGndWdtCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60pduresetgndwdtcmd;

            command->epsp60pduresetgndwdtcmd.CmdHeader[7] = 0;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60pduresetgndwdtcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_PDU_ResetGndWdtCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_PDU_ResetGndWdtCmd_t);
            memcpy(pkt->Data, &command->epsp60pduresetgndwdtcmd,
                   sizeof(EPS_P60_PDU_ResetGndWdtCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_ACU_ResetGndWdtCmd_t) - 7)
            };

            memcpy(command->epsp60acuresetgndwdtcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60acuresetgndwdtcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60acuresetgndwdtcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60acuresetgndwdtcmd.CmdHeader + 6, &fnccode, 1);

            uint16_t total = sizeof(EPS_P60_ACU_ResetGndWdtCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60acuresetgndwdtcmd;

            command->epsp60acuresetgndwdtcmd.CmdHeader[7] = 0;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60acuresetgndwdtcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_ACU_ResetGndWdtCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_ACU_ResetGndWdtCmd_t);
            memcpy(pkt->Data, &command->epsp60acuresetgndwdtcmd,
                   sizeof(EPS_P60_ACU_ResetGndWdtCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_GetParamCmd_t) - 7)
            };

            memcpy(command->epsp60getparamcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60getparamcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60getparamcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60getparamcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60getparamcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_GetParamCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60getparamcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60getparamcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_GetParamCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_GetParamCmd_t);
            memcpy(pkt->Data, &command->epsp60getparamcmd,
                   sizeof(EPS_P60_GetParamCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_GetParamArrayCmd_t) - 7)
            };

            memcpy(command->epsp60getparamarraycmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60getparamarraycmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60getparamarraycmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60getparamarraycmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60getparamarraycmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_GetParamArrayCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60getparamarraycmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60getparamarraycmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_GetParamArrayCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_GetParamArrayCmd_t);
            memcpy(pkt->Data, &command->epsp60getparamarraycmd,
                   sizeof(EPS_P60_GetParamArrayCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_SetParamCmd_t) - 7)
            };

            memcpy(command->epsp60setparamcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60setparamcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60setparamcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60setparamcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60setparamcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_SetParamCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60setparamcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60setparamcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_SetParamCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_SetParamCmd_t);
            memcpy(pkt->Data, &command->epsp60setparamcmd,
                   sizeof(EPS_P60_SetParamCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_TableGetCmd_t) - 7)
            };

            memcpy(command->epsp60tablegetcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60tablegetcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60tablegetcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60tablegetcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60tablegetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_TableGetCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60tablegetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60tablegetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_TableGetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_TableGetCmd_t);
            memcpy(pkt->Data, &command->epsp60tablegetcmd,
                   sizeof(EPS_P60_TableGetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_TableLoadCmd_t) - 7)
            };

            memcpy(command->epsp60tableloadcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60tableloadcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60tableloadcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60tableloadcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60tableloadcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_TableLoadCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60tableloadcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60tableloadcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_TableLoadCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_TableLoadCmd_t);
            memcpy(pkt->Data, &command->epsp60tableloadcmd,
                   sizeof(EPS_P60_TableLoadCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_TableSaveCmd_t) - 7)
            };

            memcpy(command->epsp60tablesavecmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60tablesavecmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60tablesavecmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60tablesavecmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60tablesavecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_TableSaveCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60tablesavecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60tablesavecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_TableSaveCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_TableSaveCmd_t);
            memcpy(pkt->Data, &command->epsp60tablesavecmd,
                   sizeof(EPS_P60_TableSaveCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_TableGetStaticCmd_t) - 7)
            };

            memcpy(command->epsp60tablegetstaticcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60tablegetstaticcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60tablegetstaticcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60tablegetstaticcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60tablegetstaticcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_TableGetStaticCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60tablegetstaticcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60tablegetstaticcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_TableGetStaticCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_TableGetStaticCmd_t);
            memcpy(pkt->Data, &command->epsp60tablegetstaticcmd,
                   sizeof(EPS_P60_TableGetStaticCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_TableDumpStaticCmd_t) - 7)
            };

            memcpy(command->epsp60tabledumpstaticcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60tabledumpstaticcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60tabledumpstaticcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60tabledumpstaticcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60tabledumpstaticcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_TableDumpStaticCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60tabledumpstaticcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60tabledumpstaticcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_TableDumpStaticCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_TableDumpStaticCmd_t);
            memcpy(pkt->Data, &command->epsp60tabledumpstaticcmd,
                   sizeof(EPS_P60_TableDumpStaticCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_GetDockInfoCmd_t) - 7)
            };

            memcpy(command->epsp60getdockinfocmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60getdockinfocmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_GetDockInfoCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60getdockinfocmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60getdockinfocmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_GetDockInfoCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_GetDockInfoCmd_t);
            memcpy(pkt->Data, &command->epsp60getdockinfocmd,
                   sizeof(EPS_P60_GetDockInfoCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_ResetCmd_t) - 7)
            };

            memcpy(command->epsp60resetcmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60resetcmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60resetcmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60resetcmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60resetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_ResetCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60resetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60resetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_ResetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_ResetCmd_t);
            memcpy(pkt->Data, &command->epsp60resetcmd,
                   sizeof(EPS_P60_ResetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
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
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(EPS_P60_GetDockInfoCmd_t) - 7)
            };

            memcpy(command->epsp60getdockinfocmd.CmdHeader,     &mid, 2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 2, seq,  2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 4, len,  2);
            memcpy(command->epsp60getdockinfocmd.CmdHeader + 6, &fnccode, 1);

            command->epsp60getdockinfocmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P60_GetDockInfoCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command->epsp60getdockinfocmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command->epsp60getdockinfocmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(EPS_P60_GetDockInfoCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(EPS_P60_GetDockInfoCmd_t);
            memcpy(pkt->Data, &command->epsp60getdockinfocmd,
                   sizeof(EPS_P60_GetDockInfoCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }


 //8192, 0, 15

    default:
        return false;
    }
    return true;
}
