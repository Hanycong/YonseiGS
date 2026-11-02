// View - 명령 생성기 입력 폼: RAW RAW 앱 (명령 번호 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
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

bool DrawRawForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 0:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        ImGui::Text("For ADCS NoOp CMD, MsgID = 6245");
        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::Checkbox("Schedule", &State.Scheduled);
        if (State.Scheduled)
        {
            ImGui::InputScalar("sch_u8ent", ImGuiDataType_U8, &State.entrynum);
            ImGui::InputScalar("sch_u32ent", ImGuiDataType_U32, &State.u32val);
        }

        if (ImGui::Button("Generate CMD"))
        {
            if (State.Scheduled)
            {
                WriteSystemName(msgid);
                mim::svc::WaitTx();

                typedef struct
                {
                    uint8 entnum;
                    uint32 exenum;
                    packetsign TestPacket[];
                } sch_cmd_t;

                sch_cmd_t* schcmd;
                schcmd = (sch_cmd_t*)malloc(5 + 2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE);
                schcmd->TestPacket->Identifier = HVD_TEST;
                schcmd->TestPacket->PacketType = MIM_PT_TMTC_TEST;
                schcmd->TestPacket->Length = CFE_SB_CMD_HDR_SIZE;
                memset(schcmd->TestPacket->Data, 0, schcmd->TestPacket->Length);

                uint8_t cmd2[CFE_SB_CMD_HDR_SIZE] = {0,};
                msgid = htons(msgid);
                memcpy(cmd2, &msgid, sizeof(uint16_t));
                memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
                cmd2[2] = 0xc0;
                cmd2[3] = 0x00;
                cmd2[4] = 0x00;
                cmd2[5] = 0x01;

                uint16_t len = CFE_SB_CMD_HDR_SIZE;
                const uint8_t* byteptr = cmd2;
                uint8_t checksum = 0xFF;
                while (len--)
                    checksum ^= *(byteptr++);
                cmd2[7] = checksum;

                memcpy(schcmd->TestPacket->Data, cmd2, sizeof(cmd2));
                schcmd->entnum = State.entrynum;
                schcmd->exenum = State.u32val;

                for (int i = 0; i < schcmd->TestPacket->Length; i++)
                    printf("0x%x ", cmd2[i]);
                printf("\n");

                mim::svc::SendUplink((void*)schcmd);
                msgid = htons(msgid);
            }
            else
            {
                WriteSystemName(msgid);

                uint8_t cmd[8] = {0,};

                mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode);

                for (int i = 0; i < sizeof(cmd); i++)
                    printf("0x%x ", cmd[i]);
                printf("\n");

                mim::tc::SendTelecommand(cmd, sizeof(cmd));
            }
        }
        break;
    }
    case 1:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint8_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg u8", ImGuiDataType_U8, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint8_t)] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(uint8_t));

            mim::tc::EncodeCfeCommand(cmd2, sizeof(cmd2), msgid, fnccode);

            for (int i = 0; i < sizeof(cmd2); i++)
            {
                if (i < 8)
                    printf("0x%x ", cmd2[i]);
                else
                    printf("%u", cmd2[i]);
            }
            printf("\n");

            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 2:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint16_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg u16", ImGuiDataType_U16, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint16_t)] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(uint16_t));

            mim::tc::EncodeCfeCommand(cmd2, sizeof(cmd2), msgid, fnccode);

            for (int i = 0; i < sizeof(cmd2); i++)
            {
                if (i < 8)
                    printf("0x%x ", cmd2[i]);
                else
                    printf("%u", arg);
            }
            printf("\n");

            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 3:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint32_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg u32", ImGuiDataType_U32, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint32_t)] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(uint32_t));

            mim::tc::EncodeCfeCommand(cmd2, sizeof(cmd2), msgid, fnccode);

            for (int i = 0; i < sizeof(cmd2); i++)
                printf("0x%x ", cmd2[i]);
            printf("\n");

            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 4:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint64_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg u64", ImGuiDataType_U64, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            if (State.Scheduled)
            {
                WriteSystemName(msgid);
                mim::svc::WaitTx();

                typedef struct
                {
                    uint8 entnum;
                    uint32 exenum;
                    packetsign TestPacket[];
                } sch_cmd_t;

                sch_cmd_t* schcmd;
                schcmd = (sch_cmd_t*)malloc(5 + 2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t));
                schcmd->TestPacket->Identifier = HVD_TEST;
                schcmd->TestPacket->PacketType = MIM_PT_TMTC_TEST;
                schcmd->TestPacket->Length = CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t);
                memset(schcmd->TestPacket->Data, 0, schcmd->TestPacket->Length);

                uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t)] = {0,};
                msgid = htons(msgid);
                memcpy(cmd2, &msgid, sizeof(uint16_t));
                memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
                memcpy(cmd2 + 8, &arg, sizeof(uint64_t));
                cmd2[2] = 0xc0;
                cmd2[3] = 0x00;
                cmd2[4] = 0x00;
                cmd2[5] = 0x09;

                uint16_t len = CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t);
                const uint8_t* byteptr = cmd2;
                uint8_t checksum = 0xFF;
                while (len--)
                    checksum ^= *(byteptr++);
                cmd2[7] = checksum;

                memcpy(schcmd->TestPacket->Data, cmd2, sizeof(cmd2));
                schcmd->entnum = State.entrynum;
                schcmd->exenum = State.u32val;

                for (int i = 0; i < schcmd->TestPacket->Length; i++)
                    printf("0x%x ", cmd2[i]);
                printf("\n");

                mim::svc::SendUplink((void*)schcmd->TestPacket);
                msgid = htons(msgid);
            }
            else
            {
                WriteSystemName(msgid);

                uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t)] = {0,};
                memcpy(cmd2 + 8, &arg, sizeof(uint64_t));

                mim::tc::EncodeCfeCommand(cmd2, sizeof(cmd2), msgid, fnccode);

                for (int i = 0; i < sizeof(cmd2); i++)
                    printf("0x%x ", cmd2[i]);
                printf("\n");

                mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
            }
        }
        break;
    }

    case 5:  {// s8
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static int8_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg s8",ImGuiDataType_S8, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE+sizeof(int8_t)] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(int8_t));
            mim::tc::EncodeCfeCommandFcAt7(cmd2, sizeof(cmd2), msgid, fnccode);
            for(int i=0; i<sizeof(cmd2); i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 6:  {// s16
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static int16_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg s16",ImGuiDataType_S16, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE+sizeof(int16_t)] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(int16_t));
            mim::tc::EncodeCfeCommandFcAt7(cmd2, sizeof(cmd2), msgid, fnccode);
            for(int i=0; i<sizeof(cmd2); i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 7:  {// s32
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static int32_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg s32",ImGuiDataType_S32, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint8_t cmd2[12] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(int32_t));
            mim::tc::EncodeCfeCommandFcAt7(cmd2, sizeof(cmd2), msgid, fnccode);
            for(int i=0; i<sizeof(cmd2); i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 8:  {// s64
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static int64_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg s64",ImGuiDataType_S64, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint8_t cmd2[16] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(int64_t));
            mim::tc::EncodeCfeCommandFcAt7(cmd2, sizeof(cmd2), msgid, fnccode);
            for(int i=0; i<sizeof(cmd2); i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }
    case 9:  { // float
        static uint16_t msgid = 0;
        static uint8_t fnccode=0;
        static float arg =0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("arg float",ImGuiDataType_Float, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint8_t cmd2[12] = {0,};
            memcpy(cmd2 + 8, &arg, sizeof(float));
            mim::tc::EncodeCfeCommandFcAt7(cmd2, sizeof(cmd2), msgid, fnccode);
            for(int i=0; i<sizeof(cmd2); i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::tc::SendTelecommand(cmd2, sizeof(cmd2));
        }
        break;
    }

    default:
        return false;
    }
    return true;
}
