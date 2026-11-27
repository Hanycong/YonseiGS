// View - 명령 생성기 입력 폼: BEE1012 to_lab (명령 번호 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2021)
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

bool DrawBee1012ToLabForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 2000: { // TO_LAB NOOP
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_NOOP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);




                mim::tc::EncodeCfeCommand(&command5->tolabnoopcmd, sizeof(TO_LAB_NoopCmd_t), msgid, fnccode);


                mim::tc::SendTelecommand(&command5->tolabnoopcmd, sizeof(TO_LAB_NoopCmd_t), MIM_PT_TMTC_TEST);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->tolabnoopcmd.CmdHeader[0],
                        command5->tolabnoopcmd.CmdHeader[1],
                        command5->tolabnoopcmd.CmdHeader[2],
                        command5->tolabnoopcmd.CmdHeader[3],
                        command5->tolabnoopcmd.CmdHeader[4],
                        command5->tolabnoopcmd.CmdHeader[5],
                        command5->tolabnoopcmd.CmdHeader[6],
                        command5->tolabnoopcmd.CmdHeader[7]);
            break;
        }
case 2001: { // TO_LAB Reset Status
    static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
    static uint8_t  fnccode = TO_LAB_RESET_STATUS_CC;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);




        mim::tc::EncodeCfeCommand(&command5->tolabresetcounterscmd, sizeof(TO_LAB_ResetCountersCmd_t), msgid, fnccode);


        mim::tc::SendTelecommand(&command5->tolabresetcounterscmd, sizeof(TO_LAB_ResetCountersCmd_t), MIM_PT_TMTC_TEST);
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command5->tolabresetcounterscmd.CmdHeader[0],
                command5->tolabresetcounterscmd.CmdHeader[1],
                command5->tolabresetcounterscmd.CmdHeader[2],
                command5->tolabresetcounterscmd.CmdHeader[3],
                command5->tolabresetcounterscmd.CmdHeader[4],
                command5->tolabresetcounterscmd.CmdHeader[5],
                command5->tolabresetcounterscmd.CmdHeader[6],
                command5->tolabresetcounterscmd.CmdHeader[7]);
    break;
}

case 2002: { // TO_LAB Add Packet
    static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
    static uint8_t  fnccode = TO_LAB_ADD_PKT_CC;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);


    static TO_LAB_AddPacket5_Payload_t payload = {};

    ImGui::Separator();
    ImGui::Text("TO_LAB_AddPacket Payload");

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.95f, 0.55f, 1.0f));

    ImGui::Text("To Enable Beacon, Enter:");
    ImGui::Text("Stream MsgId = 2074(hex 0x081A), QoS Pri = 0, QoS Rel = 0, Buflim = 4");

    ImGui::Text("To Subscribe TO Event Msg, Enter:");
    ImGui::Text("Stream MsgId = 2052(hex 0x0804), QoS Pri = 0, QoS Rel = 0, Buflim = 4");
    ImGui::Text("Stream MsgId = 2053(hex 0x0805), QoS Pri = 0, QoS Rel = 0, Buflim = 4");

    ImGui::PopStyleColor();

    ImGui::InputScalar("Stream MsgId (Atom)", ImGuiDataType_U32, &payload.Stream.Value);
    ImGui::InputScalar("QoS Priority",        ImGuiDataType_U8,  &payload.Flags.Priority);
    ImGui::InputScalar("QoS Reliability",     ImGuiDataType_U8,  &payload.Flags.Reliability);
    ImGui::InputScalar("BufLimit",            ImGuiDataType_U8,  &payload.BufLimit);

    if (ImGui::Button("Generate CMD")) {
        memset(&command5->tolabaddpacketcmd, 0, sizeof(TO_LAB_AddPacket5Cmd_t));
        uint8_t *command_bytes =
            reinterpret_cast<uint8_t *>(&command5->tolabaddpacketcmd);
        memcpy(command_bytes + offsetof(TO_LAB_AddPacket5Cmd_t, Payload),
               &payload, sizeof(payload));

        WriteSystemName(msgid);





        mim::tc::EncodeCfeCommand(&command5->tolabaddpacketcmd, sizeof(TO_LAB_AddPacket5Cmd_t), msgid, fnccode);


        mim::tc::SendTelecommand(&command5->tolabaddpacketcmd, sizeof(TO_LAB_AddPacket5Cmd_t), MIM_PT_TMTC_TEST);
    }

    ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                command5->tolabaddpacketcmd.CmdHeader[0],
                command5->tolabaddpacketcmd.CmdHeader[1],
                command5->tolabaddpacketcmd.CmdHeader[2],
                command5->tolabaddpacketcmd.CmdHeader[3],
                command5->tolabaddpacketcmd.CmdHeader[4],
                command5->tolabaddpacketcmd.CmdHeader[5],
                command5->tolabaddpacketcmd.CmdHeader[6],
                command5->tolabaddpacketcmd.CmdHeader[7]);

    ImGui::Text("Payload: Stream=0x%08X, Priority=%u, Reliability=%u, BufLimit=%u",
                payload.Stream.Value, payload.Flags.Priority,
                payload.Flags.Reliability, payload.BufLimit);
    break;
}





        case 2003: { // TO_LAB Send Data Types
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_SEND_DATA_TYPES_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);





                mim::tc::EncodeCfeCommand(&command5->tolabsenddatatypescmd, sizeof(TO_LAB_SendDataTypesCmd_t), msgid, fnccode);


                mim::tc::SendTelecommand(&command5->tolabsenddatatypescmd, sizeof(TO_LAB_SendDataTypesCmd_t), MIM_PT_TMTC_TEST);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->tolabsenddatatypescmd.CmdHeader[0],
                        command5->tolabsenddatatypescmd.CmdHeader[1],
                        command5->tolabsenddatatypescmd.CmdHeader[2],
                        command5->tolabsenddatatypescmd.CmdHeader[3],
                        command5->tolabsenddatatypescmd.CmdHeader[4],
                        command5->tolabsenddatatypescmd.CmdHeader[5],
                        command5->tolabsenddatatypescmd.CmdHeader[6],
                        command5->tolabsenddatatypescmd.CmdHeader[7]);
            break;
        }



        case 2004: { // TO_LAB Remove Packet
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_REMOVE_PKT_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            TO_LAB_RemovePacket_Payload_t *pl = &command5->tolabremovepacketcmd.Payload;

            ImGui::Separator();
            ImGui::Text("TO_LAB_RemovePacket Payload");
            ImGui::InputScalar("Stream MsgId (Atom)", ImGuiDataType_U32, &pl->Stream.Value);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);




                mim::tc::EncodeCfeCommand(&command5->tolabremovepacketcmd, sizeof(TO_LAB_RemovePacketCmd_t), msgid, fnccode);


                mim::tc::SendTelecommand(&command5->tolabremovepacketcmd, sizeof(TO_LAB_RemovePacketCmd_t), MIM_PT_TMTC_TEST);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->tolabremovepacketcmd.CmdHeader[0],
                        command5->tolabremovepacketcmd.CmdHeader[1],
                        command5->tolabremovepacketcmd.CmdHeader[2],
                        command5->tolabremovepacketcmd.CmdHeader[3],
                        command5->tolabremovepacketcmd.CmdHeader[4],
                        command5->tolabremovepacketcmd.CmdHeader[5],
                        command5->tolabremovepacketcmd.CmdHeader[6],
                        command5->tolabremovepacketcmd.CmdHeader[7]);

            ImGui::Text("Payload: Stream=0x%08X", pl->Stream.Value);
            break;
        }

        case 2005: { // TO_LAB Remove All Packets
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_REMOVE_ALL_PKT_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);




                mim::tc::EncodeCfeCommand(&command5->tolabremoveallcmd, sizeof(TO_LAB_RemoveAllCmd_t), msgid, fnccode);


                mim::tc::SendTelecommand(&command5->tolabremoveallcmd, sizeof(TO_LAB_RemoveAllCmd_t), MIM_PT_TMTC_TEST);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->tolabremoveallcmd.CmdHeader[0],
                        command5->tolabremoveallcmd.CmdHeader[1],
                        command5->tolabremoveallcmd.CmdHeader[2],
                        command5->tolabremoveallcmd.CmdHeader[3],
                        command5->tolabremoveallcmd.CmdHeader[4],
                        command5->tolabremoveallcmd.CmdHeader[5],
                        command5->tolabremoveallcmd.CmdHeader[6],
                        command5->tolabremoveallcmd.CmdHeader[7]);
            break;
        }

        case 2006: { // TO_LAB Output Enable
            static uint16_t msgid   = CFE_MISSION_TO_LAB_CMD_ID;
            static uint8_t  fnccode = TO_LAB_OUTPUT_ENABLE_CC;
            static char dest_ip[16] = "127.0.0.1";

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);
            ImGui::InputText("Destination IP", dest_ip, sizeof(dest_ip));

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                memset(command5->tolabenableoutputcmd.Payload.dest_IP, 0,
                       sizeof(command5->tolabenableoutputcmd.Payload.dest_IP));
                strncpy(command5->tolabenableoutputcmd.Payload.dest_IP, dest_ip,
                        sizeof(command5->tolabenableoutputcmd.Payload.dest_IP) - 1);




                mim::tc::EncodeCfeCommand(&command5->tolabenableoutputcmd, sizeof(TO_LAB_EnableOutputIpCmd_t), msgid, fnccode);


                mim::tc::SendTelecommand(&command5->tolabenableoutputcmd, sizeof(TO_LAB_EnableOutputIpCmd_t), MIM_PT_TMTC_TEST);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->tolabenableoutputcmd.CmdHeader[0],
                        command5->tolabenableoutputcmd.CmdHeader[1],
                        command5->tolabenableoutputcmd.CmdHeader[2],
                        command5->tolabenableoutputcmd.CmdHeader[3],
                        command5->tolabenableoutputcmd.CmdHeader[4],
                        command5->tolabenableoutputcmd.CmdHeader[5],
                        command5->tolabenableoutputcmd.CmdHeader[6],
                        command5->tolabenableoutputcmd.CmdHeader[7]);

            break;
        }


        case 2007: { // to_lab OUTPUT_DISABLE
            static uint16_t msgid=0x1823; static uint8_t fnccode=7;
            ImGui::InputScalar("msgid##direct_2007",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2007",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2007")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        case 2021: { // to_lab TO_CREATE_CHILD
            static uint16_t msgid=0x1823; static uint8_t fnccode=21;
            ImGui::InputScalar("msgid##direct_2021",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_2021",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_2021")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        // ==================== rpt ====================
    default:
        return false;
    }
    return true;
}
