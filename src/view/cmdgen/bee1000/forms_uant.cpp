// View - 명령 생성기 입력 폼: BEE1000 UANT 앱 (명령 번호 160, 161, 162, 163)
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

bool DrawBee1000UantForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 160: { // UANT Get Status


            
            static uint16_t msgid   = UANT_APP_CMD_ID;
            static uint8_t  fnccode = UANT_APP_GET_STATUS_CC;
            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u",     fnccode);


    if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + 8);
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_TMTC_TEST;
            TestPacket->Length = 8;
            memset(TestPacket->Data, 0, TestPacket->Length);

            uint8_t cmd[8] = {0,};
            msgid = htons(msgid);
            memcpy(cmd, &msgid, sizeof(uint16_t));
            memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
            cmd[2] = 0xc0;
            cmd[3] = 0x00;
            cmd[4] = 0x00;
            cmd[5] = 0x01;

            uint16_t len = 8;
            const uint8_t* byteptr = cmd;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd[7] = checksum;

            memcpy(TestPacket->Data, cmd, sizeof(cmd));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            
            msgid = htons(msgid);
    }


            break;
        }







        case 161: { // BEE-1000 UANT uantburnchannelcmd
            static uint16_t msgid   = UANT_APP_CMD_ID;
            static uint8_t  fnccode = UANT_APP_BURN_CHANNEL_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("addr u8",     ImGuiDataType_U8,  &command->uantburnchannelcmd.addr);
            ImGui::InputScalar("channel u8",  ImGuiDataType_U8,  &command->uantburnchannelcmd.channel);
            ImGui::InputScalar("duration u8", ImGuiDataType_U8,  &command->uantburnchannelcmd.duration);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  sequence[2] = {0xC0, 0x00};
                uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(UANT_BurnChannelCmd_t) - 7)};

                memcpy(command->uantburnchannelcmd.CmdHeader,       &mid,      sizeof(uint16_t));
                memcpy(command->uantburnchannelcmd.CmdHeader + 2,   sequence,  sizeof(uint16_t));
                memcpy(command->uantburnchannelcmd.CmdHeader + 4,   length,    sizeof(uint16_t));
                memcpy(command->uantburnchannelcmd.CmdHeader + 6,   &fnccode,  sizeof(uint8_t));

                command->uantburnchannelcmd.CmdHeader[7] = 0x00;
                uint16_t len = sizeof(UANT_BurnChannelCmd_t);
                const uint8_t *byteptr = (const uint8_t *)&command->uantburnchannelcmd;
                uint8_t checksum = 0xFF;
                while (len--) {
                    checksum ^= *(byteptr++);
                }
                memcpy(command->uantburnchannelcmd.CmdHeader + 7, &checksum, sizeof(uint8_t));

                // 래핑해서 uplink
                packetsign *TestPacket =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(UANT_BurnChannelCmd_t));
                TestPacket->Identifier = HVD_TEST;
                TestPacket->PacketType = MIM_PT_TMTC_TEST;
                TestPacket->Length     = sizeof(UANT_BurnChannelCmd_t);
                memcpy(TestPacket->Data, &command->uantburnchannelcmd,
                    sizeof(UANT_BurnChannelCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)TestPacket);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->uantburnchannelcmd.CmdHeader[0],
                        command->uantburnchannelcmd.CmdHeader[1],
                        command->uantburnchannelcmd.CmdHeader[2],
                        command->uantburnchannelcmd.CmdHeader[3],
                        command->uantburnchannelcmd.CmdHeader[4],
                        command->uantburnchannelcmd.CmdHeader[5],
                        command->uantburnchannelcmd.CmdHeader[6],
                        command->uantburnchannelcmd.CmdHeader[7]);

            ImGui::Text("Params: addr=%u, channel=%u, duration=%u",
                        command->uantburnchannelcmd.addr,
                        command->uantburnchannelcmd.channel,
                        command->uantburnchannelcmd.duration);

            break;
        }

        case 162: { // BEE-1000 UANT uantsetsettingscmd
            static uint16_t msgid   = UANT_APP_CMD_ID;
            static uint8_t  fnccode = UANT_APP_SET_SETTINGS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("addr u8",               ImGuiDataType_U8,  &command->uantsetsettingscmd.addr);
            ImGui::InputScalar("MinutesUntilDeploy u16",ImGuiDataType_U16, &command->uantsetsettingscmd.MinutesUntilDeploy);
            ImGui::InputScalar("BackupActive u8",       ImGuiDataType_U8,  &command->uantsetsettingscmd.BackupActive);
            ImGui::InputScalar("MaxBurnDuration u8",    ImGuiDataType_U8,  &command->uantsetsettingscmd.MaxBurnDuration);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  sequence[2] = {0xC0, 0x00};
                uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(UANT_SetSettingsCmd_t) - 7)};

                memcpy(command->uantsetsettingscmd.CmdHeader,       &mid,      sizeof(uint16_t));
                memcpy(command->uantsetsettingscmd.CmdHeader + 2,   sequence,  sizeof(uint16_t));
                memcpy(command->uantsetsettingscmd.CmdHeader + 4,   length,    sizeof(uint16_t));
                memcpy(command->uantsetsettingscmd.CmdHeader + 6,   &fnccode,  sizeof(uint8_t));

                // CRC 계산
                command->uantsetsettingscmd.CmdHeader[7] = 0x00;
                uint16_t len = sizeof(UANT_SetSettingsCmd_t);
                const uint8_t *byteptr = (const uint8_t *)&command->uantsetsettingscmd;
                uint8_t checksum = 0xFF;
                while (len--) {
                    checksum ^= *(byteptr++);
                }
                memcpy(command->uantsetsettingscmd.CmdHeader + 7, &checksum, sizeof(uint8_t));

                packetsign *TestPacket =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(UANT_SetSettingsCmd_t));
                TestPacket->Identifier = HVD_TEST;
                TestPacket->PacketType = MIM_PT_TMTC_TEST;
                TestPacket->Length     = sizeof(UANT_SetSettingsCmd_t);
                memcpy(TestPacket->Data, &command->uantsetsettingscmd,
                    sizeof(UANT_SetSettingsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)TestPacket);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->uantsetsettingscmd.CmdHeader[0],
                        command->uantsetsettingscmd.CmdHeader[1],
                        command->uantsetsettingscmd.CmdHeader[2],
                        command->uantsetsettingscmd.CmdHeader[3],
                        command->uantsetsettingscmd.CmdHeader[4],
                        command->uantsetsettingscmd.CmdHeader[5],
                        command->uantsetsettingscmd.CmdHeader[6],
                        command->uantsetsettingscmd.CmdHeader[7]);

            ImGui::Text("Params: addr=%u, MinutesUntilDeploy=%u, BackupActive=%u, MaxBurnDuration=%u",
                        command->uantsetsettingscmd.addr,
                        command->uantsetsettingscmd.MinutesUntilDeploy,
                        command->uantsetsettingscmd.BackupActive,
                        command->uantsetsettingscmd.MaxBurnDuration);

            break;
        }


        case 163: { // BEE-1000 UANT uantautodeploycmd
            static uint16_t msgid   = UANT_APP_CMD_ID;
            static uint8_t  fnccode = UANT_APP_AUTODEPLOY_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("SecondsDelay u16", ImGuiDataType_U16, &command->uantautodeploycmd.SecondsDelay);
            ImGui::InputScalar("addrslave1 u8",    ImGuiDataType_U8,  &command->uantautodeploycmd.addrslave1);
            ImGui::InputScalar("addrslave2 u8",    ImGuiDataType_U8,  &command->uantautodeploycmd.addrslave2);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  sequence[2] = {0xC0, 0x00};
                uint8_t  length[2]   = {0x00, (uint8_t)(sizeof(UANT_AutoDeployCmd_t) - 7)};

                memcpy(command->uantautodeploycmd.CmdHeader,       &mid,      sizeof(uint16_t));
                memcpy(command->uantautodeploycmd.CmdHeader + 2,   sequence,  sizeof(uint16_t));
                memcpy(command->uantautodeploycmd.CmdHeader + 4,   length,    sizeof(uint16_t));
                memcpy(command->uantautodeploycmd.CmdHeader + 6,   &fnccode,  sizeof(uint8_t));

                // CRC 계산
                command->uantautodeploycmd.CmdHeader[7] = 0x00;
                uint16_t len = sizeof(UANT_AutoDeployCmd_t);
                const uint8_t *byteptr = (const uint8_t *)&command->uantautodeploycmd;
                uint8_t checksum = 0xFF;
                while (len--) {
                    checksum ^= *(byteptr++);
                }
                memcpy(command->uantautodeploycmd.CmdHeader + 7, &checksum, sizeof(uint8_t));

                packetsign *TestPacket =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(UANT_AutoDeployCmd_t));
                TestPacket->Identifier = HVD_TEST;
                TestPacket->PacketType = MIM_PT_TMTC_TEST;
                TestPacket->Length     = sizeof(UANT_AutoDeployCmd_t);
                memcpy(TestPacket->Data, &command->uantautodeploycmd,
                    sizeof(UANT_AutoDeployCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)TestPacket);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->uantautodeploycmd.CmdHeader[0],
                        command->uantautodeploycmd.CmdHeader[1],
                        command->uantautodeploycmd.CmdHeader[2],
                        command->uantautodeploycmd.CmdHeader[3],
                        command->uantautodeploycmd.CmdHeader[4],
                        command->uantautodeploycmd.CmdHeader[5],
                        command->uantautodeploycmd.CmdHeader[6],
                        command->uantautodeploycmd.CmdHeader[7]);

            ImGui::Text("Params: SecondsDelay=%u, addrslave1=%u, addrslave2=%u",
                        command->uantautodeploycmd.SecondsDelay,
                        command->uantautodeploycmd.addrslave1,
                        command->uantautodeploycmd.addrslave2);

            break;
        }





    default:
        return false;
    }
    return true;
}
