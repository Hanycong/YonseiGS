// View - 명령 생성기 입력 폼: UELYSYS payuel_roma (명령 번호 7403, 7404, 7405, 7411, 7412, 7413, 7414, 7415, 7400, 7401, 7402, 7406, 7407, 7408, 7409, 7410, 7416, 7417, 7418, 7419, 7420, 7421, 7422, 7423, 7424, 7425, 7426, 7427, 7428, 7429, 7430, 7431, 7432, 7433, 7434, 7435, 7438, 7439, 7440, 7436, 7437)
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

bool DrawUelysysPayuelRomaForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 7403: case 7404: case 7405: { // CLOCK_SYNC / LOG_TEST / TRANS_TEST
            static uint16_t msgid=0x1830; const int selected_case=gen_fnccode; static uint8_t fnccode_by_case[3]={3,4,5}; uint8_t &fnccode=fnccode_by_case[selected_case-7403];
            ImGui::InputScalar("msgid##direct_7403_7405",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7403_7405",ImGuiDataType_U8,&fnccode);
            if(ImGui::Button("Generate CMD##direct_7403_7405")){ WriteSystemName(msgid); uint8_t cmd[8]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=1; cmd[6]=fnccode; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); }
            break;
        }
        case 7411: { // payuel_roma GET_SINGLE_ENTRY
            static uint16_t msgid=0x1830; static uint8_t fnccode=11; struct __attribute__((packed)) Payload { uint8_t type; uint16_t entry_index; }; static_assert(sizeof(Payload)==3,"payuel_roma GET_SINGLE_ENTRY payload size"); static Payload payload{0,0};
            ImGui::InputScalar("msgid##direct_7411",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7411",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("type##direct_7411",ImGuiDataType_U8,&payload.type); ImGui::InputScalar("entry_index##direct_7411",ImGuiDataType_U16,&payload.entry_index);
            if(ImGui::Button("Generate CMD##direct_7411")){ WriteSystemName(msgid); uint8_t cmd[11]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7412: { // payuel_roma GET_MULTIPLE_ENTRIES
            static uint16_t msgid=0x1830; static uint8_t fnccode=12; struct __attribute__((packed)) Payload { uint8_t type; uint16_t start_index; uint16_t end_index; }; static_assert(sizeof(Payload)==5,"payuel_roma GET_MULTIPLE_ENTRIES payload size"); static Payload payload{1,0,0};
            ImGui::InputScalar("msgid##direct_7412",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7412",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("type##direct_7412",ImGuiDataType_U8,&payload.type); ImGui::InputScalar("start_index##direct_7412",ImGuiDataType_U16,&payload.start_index); ImGui::InputScalar("end_index##direct_7412",ImGuiDataType_U16,&payload.end_index);
            if(ImGui::Button("Generate CMD##direct_7412")){ WriteSystemName(msgid); uint8_t cmd[13]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7413: { // payuel_roma ADD_ENTRY
            static uint16_t msgid=0x1830; static uint8_t fnccode=13; struct __attribute__((packed)) Payload { uint8_t type; uint64_t time_ms; uint32_t repeat_every_ms; int8_t repeat_max; uint8_t command[64]; }; static_assert(sizeof(Payload)==78,"payuel_roma ADD_ENTRY payload size"); static Payload payload{3,0,0,0,{0}};
            ImGui::InputScalar("msgid##direct_7413",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7413",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("type##direct_7413",ImGuiDataType_U8,&payload.type); ImGui::InputScalar("time_ms##direct_7413",ImGuiDataType_U64,&payload.time_ms); ImGui::InputScalar("repeat_every_ms##direct_7413",ImGuiDataType_U32,&payload.repeat_every_ms); ImGui::InputScalar("repeat_max##direct_7413",ImGuiDataType_S8,&payload.repeat_max); ImGui::InputText("command (ASCII, max 63)##direct_7413",reinterpret_cast<char*>(payload.command),sizeof(payload.command));
            if(ImGui::Button("Generate CMD##direct_7413")){ WriteSystemName(msgid); uint8_t cmd[86]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; const uint16_t wl=sizeof(cmd)-7; cmd[4]=(uint8_t)(wl>>8); cmd[5]=(uint8_t)wl; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7414: { // payuel_roma REMOVE_ENTRY
            static uint16_t msgid=0x1830; static uint8_t fnccode=14; struct __attribute__((packed)) Payload { uint8_t type; uint16_t entry_index; }; static_assert(sizeof(Payload)==3,"payuel_roma REMOVE_ENTRY payload size"); static Payload payload{4,0};
            ImGui::InputScalar("msgid##direct_7414",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7414",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("type##direct_7414",ImGuiDataType_U8,&payload.type); ImGui::InputScalar("entry_index##direct_7414",ImGuiDataType_U16,&payload.entry_index);
            if(ImGui::Button("Generate CMD##direct_7414")){ WriteSystemName(msgid); uint8_t cmd[11]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; memcpy(cmd+8,&payload,sizeof(payload)); uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }
        case 7415: { // payuel_roma GET_USED_SLOTS
            static uint16_t msgid=0x1830; static uint8_t fnccode=15; static uint8_t type=5;
            ImGui::InputScalar("msgid##direct_7415",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7415",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("type##direct_7415",ImGuiDataType_U8,&type);
            if(ImGui::Button("Generate CMD##direct_7415")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; uint16_t mid=htons(msgid); memcpy(cmd,&mid,2); cmd[2]=0xC0; cmd[5]=sizeof(cmd)-7; cmd[6]=fnccode; cmd[8]=type; uint8_t checksum=0xFF; for(size_t i=0;i<sizeof(cmd);++i)checksum^=cmd[i]; cmd[7]=checksum; packetsign *pkt=(packetsign*)malloc(8+sizeof(cmd)); pkt->Identifier=HVD_TEST; pkt->PacketType=MIM_PT_TMTC_TEST; pkt->Length=sizeof(cmd); memcpy(pkt->Data,cmd,sizeof(cmd)); mim::svc::WaitTx(); mim::svc::SendUplink((void*)pkt); } break;
        }

        // ==================== UELYSYS uant ====================
    case 7400 :
    {
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_NOOP_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_NoopCmd_t) - 7 };

            memcpy(command5->romanoopcmd.CmdHeader, &mid, 2);
            memcpy(command5->romanoopcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romanoopcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romanoopcmd.CmdHeader + 6, &fnccode, 1);

            command5->romanoopcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_NoopCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romanoopcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romanoopcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_NoopCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_NoopCmd_t);

            memcpy(pkt->Data, &command5->romanoopcmd, sizeof(PAYUEL_ROMA_NoopCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7401 :
    {
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_RESET_COUNTERS_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ResetCountersCmd_t) - 7 };

            memcpy(command5->romaresetcountercmd.CmdHeader, &mid, 2);
            memcpy(command5->romaresetcountercmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaresetcountercmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaresetcountercmd.CmdHeader + 6, &fnccode, 1);

            command5->romaresetcountercmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ResetCountersCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaresetcountercmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaresetcountercmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ResetCountersCmd_t);

            memcpy(pkt->Data, &command5->romaresetcountercmd, sizeof(PAYUEL_ROMA_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
       }
       break;
    }

    case 7402 :
    {
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_COMM_TEST_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_CommTestCmd_t) - 7 };

            memcpy(command5->romacommtestcmd.CmdHeader, &mid, 2);
            memcpy(command5->romacommtestcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romacommtestcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romacommtestcmd.CmdHeader + 6, &fnccode, 1);

            command5->romacommtestcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_CommTestCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romacommtestcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romacommtestcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_CommTestCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_CommTestCmd_t);

            memcpy(pkt->Data, &command5->romacommtestcmd, sizeof(PAYUEL_ROMA_CommTestCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7406 :
    {
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_GET_SPECIFIC_LINE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romagetspcificlinecmd.Payload.type);
        ImGui::InputScalar("Line No", ImGuiDataType_U16, &command5->romagetspcificlinecmd.Payload.line_no);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_GetSpecificLineCmd_t) - 7 };

            memcpy(command5->romagetspcificlinecmd.CmdHeader, &mid, 2);
            memcpy(command5->romagetspcificlinecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romagetspcificlinecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romagetspcificlinecmd.CmdHeader + 6, &fnccode, 1);

            command5->romagetspcificlinecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_GetSpecificLineCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romagetspcificlinecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romagetspcificlinecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_GetSpecificLineCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_GetSpecificLineCmd_t);

            memcpy(pkt->Data, &command5->romagetspcificlinecmd, sizeof(PAYUEL_ROMA_GetSpecificLineCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7407 :
    {
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_GET_MULTIPLE_LINES_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romagetmultiplelinescmd.Payload.type);
        ImGui::InputScalar("Line Start", ImGuiDataType_U16, &command5->romagetmultiplelinescmd.Payload.line_start);
        ImGui::InputScalar("Line Stop", ImGuiDataType_U16, &command5->romagetmultiplelinescmd.Payload.line_stop);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_GetMultipleLinesCmd_t) - 7 };

            memcpy(command5->romagetmultiplelinescmd.CmdHeader, &mid, 2);
            memcpy(command5->romagetmultiplelinescmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romagetmultiplelinescmd.CmdHeader + 4, len, 2);
            memcpy(command5->romagetmultiplelinescmd.CmdHeader + 6, &fnccode, 1);

            command5->romagetmultiplelinescmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_GetMultipleLinesCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romagetmultiplelinescmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romagetmultiplelinescmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_GetMultipleLinesCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_GetMultipleLinesCmd_t);

            memcpy(pkt->Data, &command5->romagetmultiplelinescmd, sizeof(PAYUEL_ROMA_GetMultipleLinesCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7408: { /* ROMA GET_LATEST_LINE (CC 8) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_GET_LATEST_LINE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romagetlatestlinecmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_GetLatestLineCmd_t) - 7 };

            memcpy(command5->romagetlatestlinecmd.CmdHeader, &mid, 2);
            memcpy(command5->romagetlatestlinecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romagetlatestlinecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romagetlatestlinecmd.CmdHeader + 6, &fnccode, 1);

            command5->romagetlatestlinecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_GetLatestLineCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romagetlatestlinecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romagetlatestlinecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_GetLatestLineCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_GetLatestLineCmd_t);

            memcpy(pkt->Data, &command5->romagetlatestlinecmd, sizeof(PAYUEL_ROMA_GetLatestLineCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7409: { /* ROMA GET_LATEST_N_LINES (CC 9) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_GET_LATEST_N_LINES_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romagetlatestNlinescmd.Payload.type);
        ImGui::InputScalar("N Lines", ImGuiDataType_U16, &command5->romagetlatestNlinescmd.Payload.n_lines);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_GetLatestNLinesCmd_t) - 7 };

            memcpy(command5->romagetlatestNlinescmd.CmdHeader, &mid, 2);
            memcpy(command5->romagetlatestNlinescmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romagetlatestNlinescmd.CmdHeader + 4, len, 2);
            memcpy(command5->romagetlatestNlinescmd.CmdHeader + 6, &fnccode, 1);

            command5->romagetlatestNlinescmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_GetLatestNLinesCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romagetlatestNlinescmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romagetlatestNlinescmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_GetLatestNLinesCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_GetLatestNLinesCmd_t);

            memcpy(pkt->Data, &command5->romagetlatestNlinescmd, sizeof(PAYUEL_ROMA_GetLatestNLinesCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7410: { /* ROMA CLEAR_ALL_LINES (CC 10) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_CLEAR_ALL_LINES_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaclearalllinescmd.Payload.type);
        ImGui::InputScalar("Code", ImGuiDataType_U16, &command5->romaclearalllinescmd.Payload.code);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ClearAllLinesCmd_t) - 7 };

            memcpy(command5->romaclearalllinescmd.CmdHeader, &mid, 2);
            memcpy(command5->romaclearalllinescmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaclearalllinescmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaclearalllinescmd.CmdHeader + 6, &fnccode, 1);

            command5->romaclearalllinescmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ClearAllLinesCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaclearalllinescmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaclearalllinescmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ClearAllLinesCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ClearAllLinesCmd_t);

            memcpy(pkt->Data, &command5->romaclearalllinescmd, sizeof(PAYUEL_ROMA_ClearAllLinesCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7416: { /* ROMA SET_ROUTE_DEFAULT (CC 16) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SET_ROUTE_DEFAULT_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romasetroutedefaultcmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SetRouteDefaultCmd_t) - 7 };

            memcpy(command5->romasetroutedefaultcmd.CmdHeader, &mid, 2);
            memcpy(command5->romasetroutedefaultcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasetroutedefaultcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasetroutedefaultcmd.CmdHeader + 6, &fnccode, 1);

            command5->romasetroutedefaultcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SetRouteDefaultCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasetroutedefaultcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasetroutedefaultcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SetRouteDefaultCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SetRouteDefaultCmd_t);

            memcpy(pkt->Data, &command5->romasetroutedefaultcmd, sizeof(PAYUEL_ROMA_SetRouteDefaultCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7417: { /* ROMA RESET_ROUTE (CC 17) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_RESET_ROUTE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaresetroutecmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ResetRouteCmd_t) - 7 };

            memcpy(command5->romaresetroutecmd.CmdHeader, &mid, 2);
            memcpy(command5->romaresetroutecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaresetroutecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaresetroutecmd.CmdHeader + 6, &fnccode, 1);

            command5->romaresetroutecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ResetRouteCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaresetroutecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaresetroutecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ResetRouteCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ResetRouteCmd_t);

            memcpy(pkt->Data, &command5->romaresetroutecmd, sizeof(PAYUEL_ROMA_ResetRouteCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7418: { /* ROMA LOAD_ROUTE (CC 18) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_LOAD_ROUTE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaloadroutecmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_LoadRouteCmd_t) - 7 };

            memcpy(command5->romaloadroutecmd.CmdHeader, &mid, 2);
            memcpy(command5->romaloadroutecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaloadroutecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaloadroutecmd.CmdHeader + 6, &fnccode, 1);

            command5->romaloadroutecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_LoadRouteCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaloadroutecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaloadroutecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_LoadRouteCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_LoadRouteCmd_t);

            memcpy(pkt->Data, &command5->romaloadroutecmd, sizeof(PAYUEL_ROMA_LoadRouteCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7419: { /* ROMA SAVE_ROUTE (CC 19) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SAVE_ROUTE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romasaveroutecmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SaveRouteCmd_t) - 7 };

            memcpy(command5->romasaveroutecmd.CmdHeader, &mid, 2);
            memcpy(command5->romasaveroutecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasaveroutecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasaveroutecmd.CmdHeader + 6, &fnccode, 1);

            command5->romasaveroutecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SaveRouteCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasaveroutecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasaveroutecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SaveRouteCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SaveRouteCmd_t);

            memcpy(pkt->Data, &command5->romasaveroutecmd, sizeof(PAYUEL_ROMA_SaveRouteCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7420: { /* ROMA SEND_ROUTE (CC 20) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SEND_ROUTE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romasendroutecmd.Payload.type);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SendRouteCmd_t) - 7 };

            memcpy(command5->romasendroutecmd.CmdHeader, &mid, 2);
            memcpy(command5->romasendroutecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasendroutecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasendroutecmd.CmdHeader + 6, &fnccode, 1);

            command5->romasendroutecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SendRouteCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasendroutecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasendroutecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SendRouteCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SendRouteCmd_t);

            memcpy(pkt->Data, &command5->romasendroutecmd, sizeof(PAYUEL_ROMA_SendRouteCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7421: { /* ROMA SET_ROUTE (CC 21) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SET_ROUTE_CC;
        static char route_buf[127] = {0,};

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romasetroutecmd.Payload.type);
        ImGui::InputText("Route Buf", route_buf, sizeof(route_buf));

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SetRouteCmd_t) - 7 };

            memset(command5->romasetroutecmd.Payload.route, 0, sizeof(command5->romasetroutecmd.Payload.route));
            strncpy(command5->romasetroutecmd.Payload.route, route_buf, sizeof(command5->romasetroutecmd.Payload.route) - 1);

            memcpy(command5->romasetroutecmd.CmdHeader, &mid, 2);
            memcpy(command5->romasetroutecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasetroutecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasetroutecmd.CmdHeader + 6, &fnccode, 1);

            command5->romasetroutecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SetRouteCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasetroutecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasetroutecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SetRouteCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SetRouteCmd_t);

            memcpy(pkt->Data, &command5->romasetroutecmd, sizeof(PAYUEL_ROMA_SetRouteCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7422: { /* ROMA PAR_GET (CC 22) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_GET_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romapargetcmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romapargetcmd.Payload.table);
        ImGui::InputScalar("Param", ImGuiDataType_U8, &command5->romapargetcmd.Payload.param);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParGetCmd_t) - 7 };

            memcpy(command5->romapargetcmd.CmdHeader, &mid, 2);
            memcpy(command5->romapargetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romapargetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romapargetcmd.CmdHeader + 6, &fnccode, 1);

            command5->romapargetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParGetCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romapargetcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romapargetcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParGetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParGetCmd_t);

            memcpy(pkt->Data, &command5->romapargetcmd, sizeof(PAYUEL_ROMA_ParGetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7423: { /* ROMA PAR_SET (CC 23) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_SET_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaparsetcmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romaparsetcmd.Payload.table);
        ImGui::InputScalar("Param", ImGuiDataType_U8, &command5->romaparsetcmd.Payload.param);
        ImGui::InputScalar("Value", ImGuiDataType_S32, &command5->romaparsetcmd.Payload.value);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParSetCmd_t) - 7 };

            memcpy(command5->romaparsetcmd.CmdHeader, &mid, 2);
            memcpy(command5->romaparsetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaparsetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaparsetcmd.CmdHeader + 6, &fnccode, 1);

            command5->romaparsetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParSetCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaparsetcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaparsetcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParSetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParSetCmd_t);

            memcpy(pkt->Data, &command5->romaparsetcmd, sizeof(PAYUEL_ROMA_ParSetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7424: { /* ROMA PAR_DEFAULTS (CC 24) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_DEFAULTS_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romapardefaultscmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romapardefaultscmd.Payload.table);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParDefaultsCmd_t) - 7 };

            memcpy(command5->romapardefaultscmd.CmdHeader, &mid, 2);
            memcpy(command5->romapardefaultscmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romapardefaultscmd.CmdHeader + 4, len, 2);
            memcpy(command5->romapardefaultscmd.CmdHeader + 6, &fnccode, 1);

            command5->romapardefaultscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParDefaultsCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romapardefaultscmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romapardefaultscmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParDefaultsCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParDefaultsCmd_t);

            memcpy(pkt->Data, &command5->romapardefaultscmd, sizeof(PAYUEL_ROMA_ParDefaultsCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7425: { /* ROMA PAR_SAVE (CC 25) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_SAVE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaparsavecmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romaparsavecmd.Payload.table);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParSaveCmd_t) - 7 };

            memcpy(command5->romaparsavecmd.CmdHeader, &mid, 2);
            memcpy(command5->romaparsavecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaparsavecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaparsavecmd.CmdHeader + 6, &fnccode, 1);

            command5->romaparsavecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParSaveCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaparsavecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaparsavecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParSaveCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParSaveCmd_t);

            memcpy(pkt->Data, &command5->romaparsavecmd, sizeof(PAYUEL_ROMA_ParSaveCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7426: { /* ROMA PAR_RESTORE (CC 26) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_RESTORE_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaparrestorecmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romaparrestorecmd.Payload.table);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParRestoreCmd_t) - 7 };

            memcpy(command5->romaparrestorecmd.CmdHeader, &mid, 2);
            memcpy(command5->romaparrestorecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaparrestorecmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaparrestorecmd.CmdHeader + 6, &fnccode, 1);

            command5->romaparrestorecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParRestoreCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaparrestorecmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaparrestorecmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParRestoreCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParRestoreCmd_t);

            memcpy(pkt->Data, &command5->romaparrestorecmd, sizeof(PAYUEL_ROMA_ParRestoreCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7427: { /* ROMA PAR_LOAD (CC 27) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_LOAD_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaparloadcmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romaparloadcmd.Payload.table);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParLoadCmd_t) - 7 };

            memcpy(command5->romaparloadcmd.CmdHeader, &mid, 2);
            memcpy(command5->romaparloadcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaparloadcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaparloadcmd.CmdHeader + 6, &fnccode, 1);

            command5->romaparloadcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParLoadCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaparloadcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaparloadcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParLoadCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParLoadCmd_t);

            memcpy(pkt->Data, &command5->romaparloadcmd, sizeof(PAYUEL_ROMA_ParLoadCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7428: { /* ROMA PAR_SET_OOB (CC 28) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAR_SET_OOB_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Type", ImGuiDataType_U8, &command5->romaparsetOOBcmd.Payload.type);
        ImGui::InputScalar("Table", ImGuiDataType_U8, &command5->romaparsetOOBcmd.Payload.table);
        ImGui::InputScalar("Enable", ImGuiDataType_U8, &command5->romaparsetOOBcmd.Payload.enable);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_ParSetOOBCmd_t) - 7 };

            memcpy(command5->romaparsetOOBcmd.CmdHeader, &mid, 2);
            memcpy(command5->romaparsetOOBcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romaparsetOOBcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romaparsetOOBcmd.CmdHeader + 6, &fnccode, 1);

            command5->romaparsetOOBcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_ParSetOOBCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romaparsetOOBcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romaparsetOOBcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_ParSetOOBCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_ParSetOOBCmd_t);

            memcpy(pkt->Data, &command5->romaparsetOOBcmd, sizeof(PAYUEL_ROMA_ParSetOOBCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7429: { /* ROMA SEND_COMMAND (CC 29) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SEND_COMMAND_CC;
        static char cmd_buf[128] = {0,};

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Command", cmd_buf, sizeof(cmd_buf));

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SendCommandCmd_t) - 7 };

            memset(command5->romasendcommandcmd.Payload.cmd, 0, sizeof(command5->romasendcommandcmd.Payload.cmd));
            strncpy(command5->romasendcommandcmd.Payload.cmd, cmd_buf, sizeof(command5->romasendcommandcmd.Payload.cmd) - 1);

            memcpy(command5->romasendcommandcmd.CmdHeader, &mid, 2);
            memcpy(command5->romasendcommandcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasendcommandcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasendcommandcmd.CmdHeader + 6, &fnccode, 1);

            command5->romasendcommandcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SendCommandCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasendcommandcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasendcommandcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SendCommandCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SendCommandCmd_t);

            memcpy(pkt->Data, &command5->romasendcommandcmd, sizeof(PAYUEL_ROMA_SendCommandCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7430: { /* ROMA SEND_MSG (CC 30) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SEND_MSG_CC;
        static char msg_buf[128] = {0,};

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputText("Message", msg_buf, sizeof(msg_buf));

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SendMsgCmd_t) - 7 };

            memset(command5->romasendmsgcmd.Payload.msg, 0, sizeof(command5->romasendmsgcmd.Payload.msg));
            strncpy(command5->romasendmsgcmd.Payload.msg, msg_buf, sizeof(command5->romasendmsgcmd.Payload.msg) - 1);

            memcpy(command5->romasendmsgcmd.CmdHeader, &mid, 2);
            memcpy(command5->romasendmsgcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasendmsgcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasendmsgcmd.CmdHeader + 6, &fnccode, 1);

            command5->romasendmsgcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SendMsgCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasendmsgcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasendmsgcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SendMsgCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SendMsgCmd_t);

            memcpy(pkt->Data, &command5->romasendmsgcmd, sizeof(PAYUEL_ROMA_SendMsgCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7431: { /* ROMA SYNC_RX (CC 31) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SYNC_RX_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SyncRxCmd_t) - 7 };

            memcpy(command5->romasyncrxcmd.CmdHeader, &mid, 2);
            memcpy(command5->romasyncrxcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasyncrxcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasyncrxcmd.CmdHeader + 6, &fnccode, 1);

            command5->romasyncrxcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SyncRxCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasyncrxcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasyncrxcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SyncRxCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SyncRxCmd_t);

            memcpy(pkt->Data, &command5->romasyncrxcmd, sizeof(PAYUEL_ROMA_SyncRxCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7432: { /* ROMA SYNC_TX (CC 32) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SYNC_TX_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_SyncTxCmd_t) - 7 };

            memcpy(command5->romasynctxcmd.CmdHeader, &mid, 2);
            memcpy(command5->romasynctxcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romasynctxcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romasynctxcmd.CmdHeader + 6, &fnccode, 1);

            command5->romasynctxcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_SyncTxCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romasynctxcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romasynctxcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_SyncTxCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_SyncTxCmd_t);

            memcpy(pkt->Data, &command5->romasynctxcmd, sizeof(PAYUEL_ROMA_SyncTxCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
    }

    case 7433: { /* ROMA PAY_INIT (CC 33) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_PAY_INIT_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = { 0xC0, 0x00 };
            uint8_t  len[2] = { 0x00, sizeof(PAYUEL_ROMA_PayInitCmd_t) - 7 };

            memcpy(command5->romapayinitcmd.CmdHeader, &mid, 2);
            memcpy(command5->romapayinitcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->romapayinitcmd.CmdHeader + 4, len, 2);
            memcpy(command5->romapayinitcmd.CmdHeader + 6, &fnccode, 1);

            command5->romapayinitcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_ROMA_PayInitCmd_t);
            const uint8_t* p = (const uint8_t*)&command5->romapayinitcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->romapayinitcmd.CmdHeader[7] = crc;

            packetsign* pkt = (packetsign*)malloc(2 + 2 + 4 + sizeof(PAYUEL_ROMA_PayInitCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(PAYUEL_ROMA_PayInitCmd_t);

            memcpy(pkt->Data, &command5->romapayinitcmd, sizeof(PAYUEL_ROMA_PayInitCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void*)pkt);
        }
        break;
        }

        // ==================== SD1-SAT: ci ====================
    case 7434: /* ROMA ISL LISTEN_START (CC 34) */
    case 7435: /* ROMA ISL LISTEN_STOP (CC 35) */
    case 7438: /* ROMA ISL SHOUT_STOP (CC 38) */
    case 7439: /* ROMA ISL TEST_COUNT_UP (CC 39) */
    case 7440: { /* ROMA ISL TEST_COUNT_LOAD (CC 40) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccodes[7] = {
            PAYUEL_ROMA_LISTEN_START_CC,
            PAYUEL_ROMA_LISTEN_STOP_CC,
            PAYUEL_ROMA_SHOUT_START_CC,
            PAYUEL_ROMA_SHOUT_START_RELATIVE_CC,
            PAYUEL_ROMA_SHOUT_STOP_CC,
            PAYUEL_ROMA_ISL_TEST_COUNT_UP_CC,
            PAYUEL_ROMA_ISL_TEST_COUNT_LOAD_CC
        };
        uint8_t &fnccode = fnccodes[gen_fnccode - 7434];

        ImGui::InputScalar("Msg ID##roma_isl_noarg", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)##roma_isl_noarg", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD##roma_isl_noarg")) {
            WriteSystemName(msgid);
            uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
            uint16_t mid = htons(msgid);
            memcpy(cmd, &mid, sizeof(mid));
            cmd[2] = 0xC0;
            cmd[3] = 0x00;
            const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7U);
            cmd[4] = (uint8_t)(wire_length >> 8);
            cmd[5] = (uint8_t)(wire_length & 0xFFU);
            cmd[6] = fnccode;
            cmd[7] = 0;
            uint8_t checksum = 0xFF;
            for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
            cmd[7] = checksum;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = (uint16_t)sizeof(cmd);
            memcpy(pkt->Data, cmd, sizeof(cmd));
            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7436: { /* ROMA ISL SHOUT_START (CC 36) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SHOUT_START_CC;
        static PAYUEL_ROMA_ShoutStart_WirePayload_t payload{0, 100, 1, 0, {0}};
        static char byteshex[2 * PAYUEL_ROMA_ISL_MAX_COMMAND_BYTES + 1] = "";

        ImGui::InputScalar("Msg ID##roma_isl_shout_start", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)##roma_isl_shout_start", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Start Time (ms)", ImGuiDataType_U64, &payload.StartTimeMs);
        ImGui::InputScalar("Repeat Every (ms)##roma_isl_shout_start", ImGuiDataType_U32, &payload.RepeatEveryMs);
        ImGui::InputScalar("Repeat Count##roma_isl_shout_start", ImGuiDataType_U16, &payload.RepeatCount);
        ImGui::InputText("Embedded Command (hex)##roma_isl_shout_start", byteshex, sizeof(byteshex));

        const size_t byte_count = UelysysCopyHexBytes(payload.Bytes, sizeof(payload.Bytes), byteshex);
        payload.Length = (uint8_t)byte_count;
        ImGui::Text("Length: %u", (unsigned)payload.Length);

        if (ImGui::Button("Generate CMD##roma_isl_shout_start")) {
            const bool repeat_valid = !((payload.RepeatCount == 0U || payload.RepeatCount > 1U) &&
                                        payload.RepeatEveryMs < 100U);
            if (byte_count < 8U || !repeat_valid) {
                console.AddLog("[ERROR]## ROMA SHOUT_START requires 8..235 command bytes and repeat interval >= 100 ms for repeated schedules.");
                break;
            }

            WriteSystemName(msgid);
            const size_t payload_size = offsetof(PAYUEL_ROMA_ShoutStart_WirePayload_t, Bytes) + byte_count;
            const size_t command_size = CFE_SB_CMD_HDR_SIZE + payload_size;
            uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(PAYUEL_ROMA_ShoutStart_WirePayload_t)] = {0};
            uint16_t mid = htons(msgid);
            memcpy(cmd, &mid, sizeof(mid));
            cmd[2] = 0xC0;
            cmd[3] = 0x00;
            const uint16_t wire_length = (uint16_t)(command_size - 7U);
            cmd[4] = (uint8_t)(wire_length >> 8);
            cmd[5] = (uint8_t)(wire_length & 0xFFU);
            cmd[6] = fnccode;
            memcpy(cmd + CFE_SB_CMD_HDR_SIZE, &payload, payload_size);
            cmd[7] = 0;
            uint8_t checksum = 0xFF;
            for (size_t i = 0; i < command_size; ++i) checksum ^= cmd[i];
            cmd[7] = checksum;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + command_size);
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = (uint16_t)command_size;
            memcpy(pkt->Data, cmd, command_size);
            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7437: { /* ROMA ISL SHOUT_START_RELATIVE (CC 37) */
        static uint16_t msgid = PAYUEL_ROMA_CMD_MID;
        static uint8_t fnccode = PAYUEL_ROMA_SHOUT_START_RELATIVE_CC;
        static PAYUEL_ROMA_ShoutStartRelative_WirePayload_t payload{0, 100, 1, 0, {0}};
        static char byteshex[2 * PAYUEL_ROMA_ISL_MAX_COMMAND_BYTES + 1] = "";

        ImGui::InputScalar("Msg ID##roma_isl_shout_relative", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
        ImGui::InputScalar("Command Code(CC)##roma_isl_shout_relative", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Delay (ms)", ImGuiDataType_U64, &payload.DelayMs);
        ImGui::InputScalar("Repeat Every (ms)##roma_isl_shout_relative", ImGuiDataType_U32, &payload.RepeatEveryMs);
        ImGui::InputScalar("Repeat Count##roma_isl_shout_relative", ImGuiDataType_U16, &payload.RepeatCount);
        ImGui::InputText("Embedded Command (hex)##roma_isl_shout_relative", byteshex, sizeof(byteshex));

        const size_t byte_count = UelysysCopyHexBytes(payload.Bytes, sizeof(payload.Bytes), byteshex);
        payload.Length = (uint8_t)byte_count;
        ImGui::Text("Length: %u", (unsigned)payload.Length);

        if (ImGui::Button("Generate CMD##roma_isl_shout_relative")) {
            const bool repeat_valid = !((payload.RepeatCount == 0U || payload.RepeatCount > 1U) &&
                                        payload.RepeatEveryMs < 100U);
            if (byte_count < 8U || !repeat_valid) {
                console.AddLog("[ERROR]## ROMA SHOUT_START_RELATIVE requires 8..235 command bytes and repeat interval >= 100 ms for repeated schedules.");
                break;
            }

            WriteSystemName(msgid);
            const size_t payload_size = offsetof(PAYUEL_ROMA_ShoutStartRelative_WirePayload_t, Bytes) + byte_count;
            const size_t command_size = CFE_SB_CMD_HDR_SIZE + payload_size;
            uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(PAYUEL_ROMA_ShoutStartRelative_WirePayload_t)] = {0};
            uint16_t mid = htons(msgid);
            memcpy(cmd, &mid, sizeof(mid));
            cmd[2] = 0xC0;
            cmd[3] = 0x00;
            const uint16_t wire_length = (uint16_t)(command_size - 7U);
            cmd[4] = (uint8_t)(wire_length >> 8);
            cmd[5] = (uint8_t)(wire_length & 0xFFU);
            cmd[6] = fnccode;
            memcpy(cmd + CFE_SB_CMD_HDR_SIZE, &payload, payload_size);
            cmd[7] = 0;
            uint8_t checksum = 0xFF;
            for (size_t i = 0; i < command_size; ++i) checksum ^= cmd[i];
            cmd[7] = checksum;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + command_size);
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = (uint16_t)command_size;
            memcpy(pkt->Data, cmd, command_size);
            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }
    default:
        return false;
    }
    return true;
}
