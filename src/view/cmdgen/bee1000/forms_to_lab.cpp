// View - 명령 생성기 입력 폼: BEE1000 TO_LAB 앱 (명령 번호 153, 154, 155, 156, 157, 158, 159)
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

bool DrawBee1000ToLabForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 153: { // TO_LAB NOOP
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_NOOP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(TO_LAB_NoopCmd_t) - 7)
                };

                memcpy(command->tolabnoopcmd.CmdHeader,     &mid, 2);
                memcpy(command->tolabnoopcmd.CmdHeader + 2, seq,  2);
                memcpy(command->tolabnoopcmd.CmdHeader + 4, len,  2);
                memcpy(command->tolabnoopcmd.CmdHeader + 6, &fnccode, 1);

                command->tolabnoopcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(TO_LAB_NoopCmd_t);
                const uint8_t *p = (const uint8_t *)&command->tolabnoopcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->tolabnoopcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_NoopCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(TO_LAB_NoopCmd_t);
                memcpy(pkt->Data, &command->tolabnoopcmd, sizeof(TO_LAB_NoopCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->tolabnoopcmd.CmdHeader[0],
                        command->tolabnoopcmd.CmdHeader[1],
                        command->tolabnoopcmd.CmdHeader[2],
                        command->tolabnoopcmd.CmdHeader[3],
                        command->tolabnoopcmd.CmdHeader[4],
                        command->tolabnoopcmd.CmdHeader[5],
                        command->tolabnoopcmd.CmdHeader[6],
                        command->tolabnoopcmd.CmdHeader[7]);
            break;
        }
case 154: { // TO_LAB Reset Status
    static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
    static uint8_t  fnccode = TO_LAB_RESET_STATUS_CC;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        uint16_t mid = htons(msgid);
        uint8_t  seq[2] = {0xC0, 0x00};
        uint8_t  len[2] = {
            0x00,
            (uint8_t)(sizeof(TO_LAB_ResetCountersCmd_t) - 7)
        };

        memcpy(command->tolabresetcounterscmd.CmdHeader,     &mid, 2);
        memcpy(command->tolabresetcounterscmd.CmdHeader + 2, seq,  2);
        memcpy(command->tolabresetcounterscmd.CmdHeader + 4, len,  2);
        memcpy(command->tolabresetcounterscmd.CmdHeader + 6, &fnccode, 1);

        command->tolabresetcounterscmd.CmdHeader[7] = 0;
        uint16_t total = sizeof(TO_LAB_ResetCountersCmd_t);
        const uint8_t *p = (const uint8_t *)&command->tolabresetcounterscmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(p++);

        memcpy(command->tolabresetcounterscmd.CmdHeader + 7, &crc, 1);

        packetsign *pkt =
            (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_ResetCountersCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(TO_LAB_ResetCountersCmd_t);
        memcpy(pkt->Data, &command->tolabresetcounterscmd,
               sizeof(TO_LAB_ResetCountersCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->tolabresetcounterscmd.CmdHeader[0],
                command->tolabresetcounterscmd.CmdHeader[1],
                command->tolabresetcounterscmd.CmdHeader[2],
                command->tolabresetcounterscmd.CmdHeader[3],
                command->tolabresetcounterscmd.CmdHeader[4],
                command->tolabresetcounterscmd.CmdHeader[5],
                command->tolabresetcounterscmd.CmdHeader[6],
                command->tolabresetcounterscmd.CmdHeader[7]);
    break;
}

case 155: { // TO_LAB Add Packet
    static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
    static uint8_t  fnccode = TO_LAB_ADD_PKT_CC;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);


    TO_LAB_AddPacket_Payload_t *pl = &command->tolabaddpacketcmd.Payload;

    ImGui::Separator();
    ImGui::Text("TO_LAB_AddPacket Payload");

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.95f, 0.55f, 1.0f));

    ImGui::Text("To Enable Beacon, Enter:");
    ImGui::Text("Stream MsgId = 2074(hex 0x081A), QoS Pri = 0, QoS Rel = 0, Buflim = 4");

    ImGui::Text("To Subscribe TO Event Msg, Enter:");
    ImGui::Text("Stream MsgId = 2052(hex 0x0804), QoS Pri = 0, QoS Rel = 0, Buflim = 4");
    ImGui::Text("Stream MsgId = 2053(hex 0x0805), QoS Pri = 0, QoS Rel = 0, Buflim = 4");

    ImGui::PopStyleColor();

    ImGui::InputScalar("Stream MsgId (Atom)", ImGuiDataType_U32, &pl->Stream.Value);
    ImGui::InputScalar("QoS Priority",        ImGuiDataType_U8,  &pl->Flags.Priority);
    ImGui::InputScalar("QoS Reliability",     ImGuiDataType_U8,  &pl->Flags.Reliability);
    ImGui::InputScalar("BufLimit",            ImGuiDataType_U8,  &pl->BufLimit);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        uint16_t mid = htons(msgid);
        uint8_t  seq[2] = {0xC0, 0x00};
        uint8_t  len[2] = {
            0x00,
            (uint8_t)(sizeof(TO_LAB_AddPacketCmd_t) - 7)
        };

        memcpy(command->tolabaddpacketcmd.CmdHeader,     &mid, 2);
        memcpy(command->tolabaddpacketcmd.CmdHeader + 2, seq,  2);
        memcpy(command->tolabaddpacketcmd.CmdHeader + 4, len,  2);
        memcpy(command->tolabaddpacketcmd.CmdHeader + 6, &fnccode, 1);


        command->tolabaddpacketcmd.CmdHeader[7] = 0;
        uint16_t total = sizeof(TO_LAB_AddPacketCmd_t);
        const uint8_t *p = (const uint8_t *)&command->tolabaddpacketcmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(p++);

        memcpy(command->tolabaddpacketcmd.CmdHeader + 7, &crc, 1);

        packetsign *pkt =
            (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_AddPacketCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(TO_LAB_AddPacketCmd_t);
        memcpy(pkt->Data, &command->tolabaddpacketcmd,
               sizeof(TO_LAB_AddPacketCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command->tolabaddpacketcmd.CmdHeader[0],
                command->tolabaddpacketcmd.CmdHeader[1],
                command->tolabaddpacketcmd.CmdHeader[2],
                command->tolabaddpacketcmd.CmdHeader[3],
                command->tolabaddpacketcmd.CmdHeader[4],
                command->tolabaddpacketcmd.CmdHeader[5],
                command->tolabaddpacketcmd.CmdHeader[6],
                command->tolabaddpacketcmd.CmdHeader[7]);

    ImGui::Text("Payload: Stream=0x%08X, Priority=%u, Reliability=%u, BufLimit=%u",
                pl->Stream.Value, pl->Flags.Priority, pl->Flags.Reliability, pl->BufLimit);
    break;
}





        case 156: { // TO_LAB Send Data Types
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_SEND_DATA_TYPES_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(TO_LAB_SendDataTypesCmd_t) - 7)
                };

                memcpy(command->tolabsenddatatypescmd.CmdHeader,     &mid, 2);
                memcpy(command->tolabsenddatatypescmd.CmdHeader + 2, seq,  2);
                memcpy(command->tolabsenddatatypescmd.CmdHeader + 4, len,  2);
                memcpy(command->tolabsenddatatypescmd.CmdHeader + 6, &fnccode, 1);


                command->tolabsenddatatypescmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(TO_LAB_SendDataTypesCmd_t);
                const uint8_t *p = (const uint8_t *)&command->tolabsenddatatypescmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->tolabsenddatatypescmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_SendDataTypesCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(TO_LAB_SendDataTypesCmd_t);
                memcpy(pkt->Data, &command->tolabsenddatatypescmd,
                    sizeof(TO_LAB_SendDataTypesCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->tolabsenddatatypescmd.CmdHeader[0],
                        command->tolabsenddatatypescmd.CmdHeader[1],
                        command->tolabsenddatatypescmd.CmdHeader[2],
                        command->tolabsenddatatypescmd.CmdHeader[3],
                        command->tolabsenddatatypescmd.CmdHeader[4],
                        command->tolabsenddatatypescmd.CmdHeader[5],
                        command->tolabsenddatatypescmd.CmdHeader[6],
                        command->tolabsenddatatypescmd.CmdHeader[7]);
            break;
        }



        case 157: { // TO_LAB Remove Packet
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_REMOVE_PKT_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            TO_LAB_RemovePacket_Payload_t *pl = &command->tolabremovepacketcmd.Payload;

            ImGui::Separator();
            ImGui::Text("TO_LAB_RemovePacket Payload");
            ImGui::InputScalar("Stream MsgId (Atom)", ImGuiDataType_U32, &pl->Stream.Value);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(TO_LAB_RemovePacketCmd_t) - 7)
                };

                memcpy(command->tolabremovepacketcmd.CmdHeader,     &mid, 2);
                memcpy(command->tolabremovepacketcmd.CmdHeader + 2, seq,  2);
                memcpy(command->tolabremovepacketcmd.CmdHeader + 4, len,  2);
                memcpy(command->tolabremovepacketcmd.CmdHeader + 6, &fnccode, 1);

                command->tolabremovepacketcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(TO_LAB_RemovePacketCmd_t);
                const uint8_t *p = (const uint8_t *)&command->tolabremovepacketcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->tolabremovepacketcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_RemovePacketCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(TO_LAB_RemovePacketCmd_t);
                memcpy(pkt->Data, &command->tolabremovepacketcmd,
                    sizeof(TO_LAB_RemovePacketCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->tolabremovepacketcmd.CmdHeader[0],
                        command->tolabremovepacketcmd.CmdHeader[1],
                        command->tolabremovepacketcmd.CmdHeader[2],
                        command->tolabremovepacketcmd.CmdHeader[3],
                        command->tolabremovepacketcmd.CmdHeader[4],
                        command->tolabremovepacketcmd.CmdHeader[5],
                        command->tolabremovepacketcmd.CmdHeader[6],
                        command->tolabremovepacketcmd.CmdHeader[7]);

            ImGui::Text("Payload: Stream=0x%08X", pl->Stream.Value);
            break;
        }

        case 158: { // TO_LAB Remove All Packets
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_REMOVE_ALL_PKT_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(TO_LAB_RemoveAllCmd_t) - 7)
                };

                memcpy(command->tolabremoveallcmd.CmdHeader,     &mid, 2);
                memcpy(command->tolabremoveallcmd.CmdHeader + 2, seq,  2);
                memcpy(command->tolabremoveallcmd.CmdHeader + 4, len,  2);
                memcpy(command->tolabremoveallcmd.CmdHeader + 6, &fnccode, 1);

                command->tolabremoveallcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(TO_LAB_RemoveAllCmd_t);
                const uint8_t *p = (const uint8_t *)&command->tolabremoveallcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->tolabremoveallcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_RemoveAllCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(TO_LAB_RemoveAllCmd_t);
                memcpy(pkt->Data, &command->tolabremoveallcmd,
                    sizeof(TO_LAB_RemoveAllCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->tolabremoveallcmd.CmdHeader[0],
                        command->tolabremoveallcmd.CmdHeader[1],
                        command->tolabremoveallcmd.CmdHeader[2],
                        command->tolabremoveallcmd.CmdHeader[3],
                        command->tolabremoveallcmd.CmdHeader[4],
                        command->tolabremoveallcmd.CmdHeader[5],
                        command->tolabremoveallcmd.CmdHeader[6],
                        command->tolabremoveallcmd.CmdHeader[7]);
            break;
        }

        case 159: { // TO_LAB Output Enable
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_OUTPUT_ENABLE_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(TO_LAB_EnableOutputCmd_t) - 7)
                };

                memcpy(command->tolabenableoutputcmd.CmdHeader,     &mid, 2);
                memcpy(command->tolabenableoutputcmd.CmdHeader + 2, seq,  2);
                memcpy(command->tolabenableoutputcmd.CmdHeader + 4, len,  2);
                memcpy(command->tolabenableoutputcmd.CmdHeader + 6, &fnccode, 1);

                command->tolabenableoutputcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(TO_LAB_EnableOutputCmd_t);
                const uint8_t *p = (const uint8_t *)&command->tolabenableoutputcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->tolabenableoutputcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(TO_LAB_EnableOutputCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(TO_LAB_EnableOutputCmd_t);
                memcpy(pkt->Data, &command->tolabenableoutputcmd,
                    sizeof(TO_LAB_EnableOutputCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->tolabenableoutputcmd.CmdHeader[0],
                        command->tolabenableoutputcmd.CmdHeader[1],
                        command->tolabenableoutputcmd.CmdHeader[2],
                        command->tolabenableoutputcmd.CmdHeader[3],
                        command->tolabenableoutputcmd.CmdHeader[4],
                        command->tolabenableoutputcmd.CmdHeader[5],
                        command->tolabenableoutputcmd.CmdHeader[6],
                        command->tolabenableoutputcmd.CmdHeader[7]);

            break;
        }

    default:
        return false;
    }
    return true;
}
