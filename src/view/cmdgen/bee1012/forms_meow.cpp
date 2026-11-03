// View - 명령 생성기 입력 폼: BEE1012 meow (명령 번호 3100, 3101, 3110, 3111, 3112, 3113, 3114, 3115, 3120, 3121, 3122, 3123, 3124, 3125, 3126, 3127, 3128, 3129, 3140, 3141, 3142, 3143, 3144, 3145, 3150, 3151, 3152, 3153, 3154, 3155, 3156, 3157, 3158, 3159, 3160, 3161)
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

bool DrawBee1012MeowForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 3100: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_NOOP_CC);
            ImGui::InputScalar("msgid##direct_3100", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3100", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3100")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3101: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_RESET_COUNTERS_CC);
            ImGui::InputScalar("msgid##direct_3101", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3101", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3101")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3110: {
            UELYSYS_MEOW_ShellExecSyncCmd_t &cmd = command5->uelysys_meow_shellexecsynccmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_SYNC_CC);
            ImGui::InputScalar("msgid##direct_3110", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3110", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            if (ImGui::Button("Generate CMD##direct_3110")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_ShellExecSyncCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecSyncCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_ShellExecSyncCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecSyncCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_ShellExecSyncCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3111: {
            UELYSYS_MEOW_ShellExecAsyncCmd_t &cmd = command5->uelysys_meow_shellexecasynccmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC);
            ImGui::InputScalar("msgid##direct_3111", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3111", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            if (ImGui::Button("Generate CMD##direct_3111")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3112: {
            UELYSYS_MEOW_ShellPollCmd_t &cmd = command5->uelysys_meow_shellpollcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_POLL_CC);
            ImGui::InputScalar("msgid##direct_3112", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3112", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("pid", ImGuiDataType_S32, &cmd.pid);
            if (ImGui::Button("Generate CMD##direct_3112")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3113: {
            UELYSYS_MEOW_ShellKillCmd_t &cmd = command5->uelysys_meow_shellkillcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_KILL_CC);
            ImGui::InputScalar("msgid##direct_3113", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3113", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("pid", ImGuiDataType_S32, &cmd.pid);
            ImGui::InputScalar("sig", ImGuiDataType_S32, &cmd.sig);
            if (ImGui::Button("Generate CMD##direct_3113")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3114: {
            UELYSYS_MEOW_ShellExecSyncForceCmd_t &cmd = command5->uelysys_meow_shellexecsyncforcecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC);
            ImGui::InputScalar("msgid##direct_3114", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3114", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3114")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3115: {
            UELYSYS_MEOW_ShellExecAsyncForceCmd_t &cmd = command5->uelysys_meow_shellexecasyncforcecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC);
            ImGui::InputScalar("msgid##direct_3115", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3115", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3115")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3120: {
            UELYSYS_MEOW_FileReadCmd_t &cmd = command5->uelysys_meow_filereadcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_READ_CC);
            ImGui::InputScalar("msgid##direct_3120", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3120", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("offset", ImGuiDataType_U32, &cmd.offset);
            ImGui::InputScalar("size", ImGuiDataType_U32, &cmd.size);
            if (ImGui::Button("Generate CMD##direct_3120")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3121: {
            UELYSYS_MEOW_FileWriteCmd_t &cmd = command5->uelysys_meow_filewritecmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_WRITE_CC);
            ImGui::InputScalar("msgid##direct_3121", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3121", ImGuiDataType_U8, &fnccode);
            static char data_hex[1024] = {};
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("offset", ImGuiDataType_U32, &cmd.offset);
            ImGui::InputScalar("data_len", ImGuiDataType_U16, &cmd.data_len);
            ImGui::InputScalar("flags", ImGuiDataType_U8, &cmd.flags);
            ImGui::InputScalar("spare", ImGuiDataType_U8, &cmd.spare);
            ImGui::InputTextMultiline("data hex", data_hex, sizeof(data_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.data, sizeof(cmd.data), data_hex);
            if (cmd.data_len > copied) cmd.data_len = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3121")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3122: {
            UELYSYS_MEOW_FilePathCmd_t &cmd = command5->uelysys_meow_filepathcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_REMOVE_CC);
            ImGui::InputScalar("msgid##direct_3122", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3122", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3122")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FilePathCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FilePathCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3123: {
            UELYSYS_MEOW_FileSrcDstCmd_t &cmd = command5->uelysys_meow_filesrcdstcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_COPY_CC);
            ImGui::InputScalar("msgid##direct_3123", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3123", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("src", cmd.src, sizeof(cmd.src));
            ImGui::InputText("dst", cmd.dst, sizeof(cmd.dst));
            if (ImGui::Button("Generate CMD##direct_3123")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FileSrcDstCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FileSrcDstCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FileSrcDstCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FileSrcDstCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3124: {
            UELYSYS_MEOW_FileSrcDstCmd_t &cmd = command5->uelysys_meow_filesrcdstcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_MOVE_CC);
            ImGui::InputScalar("msgid##direct_3124", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3124", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("src", cmd.src, sizeof(cmd.src));
            ImGui::InputText("dst", cmd.dst, sizeof(cmd.dst));
            if (ImGui::Button("Generate CMD##direct_3124")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FileSrcDstCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FileSrcDstCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FileSrcDstCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FileSrcDstCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3125: {
            UELYSYS_MEOW_FilePathCmd_t &cmd = command5->uelysys_meow_filepathcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_STAT_CC);
            ImGui::InputScalar("msgid##direct_3125", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3125", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3125")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FilePathCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FilePathCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3126: {
            UELYSYS_MEOW_FilePathCmd_t &cmd = command5->uelysys_meow_filepathcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_TRUNCATE_CC);
            ImGui::InputScalar("msgid##direct_3126", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3126", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3126")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3127: {
            UELYSYS_MEOW_FileTailCmd_t &cmd = command5->uelysys_meow_filetailcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_TAIL_CC);
            ImGui::InputScalar("msgid##direct_3127", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3127", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("n_bytes", ImGuiDataType_U32, &cmd.n_bytes);
            ImGui::InputScalar("size", ImGuiDataType_U32, &cmd.size);
            if (ImGui::Button("Generate CMD##direct_3127")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3128: {
            UELYSYS_MEOW_FilePathCmd_t &cmd = command5->uelysys_meow_filepathcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_CHECKSUM_CC);
            ImGui::InputScalar("msgid##direct_3128", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3128", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3128")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FilePathCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FilePathCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3129: {
            UELYSYS_MEOW_FilePathCmd_t &cmd = command5->uelysys_meow_filepathcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_DISK_STAT_CC);
            ImGui::InputScalar("msgid##direct_3129", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3129", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3129")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_FilePathCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_FilePathCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_FilePathCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3140: {
            UELYSYS_MEOW_SysShutdownCmd_t &cmd = command5->uelysys_meow_sysshutdowncmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_SHUTDOWN_CC);
            ImGui::InputScalar("msgid##direct_3140", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3140", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("type", ImGuiDataType_S32, &cmd.type);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3140")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3141: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_SYNC_CC);
            ImGui::InputScalar("msgid##direct_3141", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3141", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3141")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3142: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_INFO_CC);
            ImGui::InputScalar("msgid##direct_3142", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3142", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3142")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3143: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_TIME_GET_CC);
            ImGui::InputScalar("msgid##direct_3143", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3143", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3143")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3144: {
            UELYSYS_MEOW_SysTimeSetCmd_t &cmd = command5->uelysys_meow_systimesetcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_TIME_SET_CC);
            ImGui::InputScalar("msgid##direct_3144", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3144", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("sec", ImGuiDataType_S64, &cmd.sec);
            ImGui::InputScalar("nsec", ImGuiDataType_U32, &cmd.nsec);
            if (ImGui::Button("Generate CMD##direct_3144")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3145: {
            UELYSYS_MEOW_SysForceKillCmd_t &cmd = command5->uelysys_meow_sysforcekillcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_FORCE_KILL_CC);
            ImGui::InputScalar("msgid##direct_3145", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3145", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            ImGui::InputScalar("mode", ImGuiDataType_S32, &cmd.mode);
            if (ImGui::Button("Generate CMD##direct_3145")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3150: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_SERVER_START_CC);
            ImGui::InputScalar("msgid##direct_3150", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3150", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3150")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3151: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_SERVER_STOP_CC);
            ImGui::InputScalar("msgid##direct_3151", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3151", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_3151")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(UELYSYS_NoArgCmd_t)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(UELYSYS_NoArgCmd_t)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t));
                memcpy(pkt->Data, &cmd, (sizeof(UELYSYS_NoArgCmd_t)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3152: {
            UELYSYS_MEOW_CspSetReadTimeoutCmd_t &cmd = command5->uelysys_meow_cspsetreadtimeoutcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_SET_READ_TIMEOUT_CC);
            ImGui::InputScalar("msgid##direct_3152", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3152", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            if (ImGui::Button("Generate CMD##direct_3152")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3153: {
            UELYSYS_MEOW_CspHandlerLoadCmd_t &cmd = command5->uelysys_meow_csphandlerloadcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_HANDLER_LOAD_CC);
            ImGui::InputScalar("msgid##direct_3153", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3153", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("port", ImGuiDataType_U8, &cmd.port);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputText("symbol", cmd.symbol, sizeof(cmd.symbol));
            if (ImGui::Button("Generate CMD##direct_3153")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3154: {
            UELYSYS_MEOW_CspHandlerClearCmd_t &cmd = command5->uelysys_meow_csphandlerclearcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_HANDLER_CLEAR_CC);
            ImGui::InputScalar("msgid##direct_3154", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3154", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("port", ImGuiDataType_U8, &cmd.port);
            if (ImGui::Button("Generate CMD##direct_3154")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3155: {
            UELYSYS_MEOW_CspSendCmd_t &cmd = command5->uelysys_meow_cspsendcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_SEND_CC);
            ImGui::InputScalar("msgid##direct_3155", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3155", ImGuiDataType_U8, &fnccode);
            static char data_hex[1024] = {};
            ImGui::InputScalar("dst", ImGuiDataType_U8, &cmd.dst);
            ImGui::InputScalar("dst_port", ImGuiDataType_U8, &cmd.dst_port);
            ImGui::InputScalar("src_port", ImGuiDataType_U8, &cmd.src_port);
            ImGui::InputScalar("prio", ImGuiDataType_U8, &cmd.prio);
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            ImGui::InputScalar("len", ImGuiDataType_U16, &cmd.len);
            ImGui::InputTextMultiline("data hex", data_hex, sizeof(data_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.data, sizeof(cmd.data), data_hex);
            if (cmd.len > copied) cmd.len = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3155")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3156: {
            UELYSYS_MEOW_CspFtpCmd_t &cmd = command5->uelysys_meow_cspftpcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_FTP_UPLOAD_CC);
            ImGui::InputScalar("msgid##direct_3156", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3156", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("host", ImGuiDataType_U8, &cmd.host);
            ImGui::InputScalar("port", ImGuiDataType_U8, &cmd.port);
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U16, &cmd.timeout_ms);
            ImGui::InputScalar("chunk_size", ImGuiDataType_U16, &cmd.chunk_size);
            ImGui::InputText("local_path", cmd.local_path, sizeof(cmd.local_path));
            ImGui::InputText("remote_path", cmd.remote_path, sizeof(cmd.remote_path));
            if (ImGui::Button("Generate CMD##direct_3156")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_CspFtpCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_CspFtpCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_CspFtpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_CspFtpCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_CspFtpCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3157: {
            UELYSYS_MEOW_CspFtpCmd_t &cmd = command5->uelysys_meow_cspftpcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_FTP_DOWNLOAD_CC);
            ImGui::InputScalar("msgid##direct_3157", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3157", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("host", ImGuiDataType_U8, &cmd.host);
            ImGui::InputScalar("port", ImGuiDataType_U8, &cmd.port);
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U16, &cmd.timeout_ms);
            ImGui::InputScalar("chunk_size", ImGuiDataType_U16, &cmd.chunk_size);
            ImGui::InputText("local_path", cmd.local_path, sizeof(cmd.local_path));
            ImGui::InputText("remote_path", cmd.remote_path, sizeof(cmd.remote_path));
            if (ImGui::Button("Generate CMD##direct_3157")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_MEOW_CspFtpCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_MEOW_CspFtpCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_MEOW_CspFtpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_MEOW_CspFtpCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_MEOW_CspFtpCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3158: {
            UELYSYS_MEOW_CspIfstatsCmd_t &cmd = command5->uelysys_meow_cspifstatscmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_IFSTATS_CC);
            ImGui::InputScalar("msgid##direct_3158", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3158", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("iface_name", cmd.iface_name, sizeof(cmd.iface_name));
            if (ImGui::Button("Generate CMD##direct_3158")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3159: {
            UELYSYS_MEOW_CspRouteSetCmd_t &cmd = command5->uelysys_meow_csproutesetcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_ROUTE_SET_CC);
            ImGui::InputScalar("msgid##direct_3159", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3159", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("dst", ImGuiDataType_U8, &cmd.dst);
            ImGui::InputScalar("mask", ImGuiDataType_U8, &cmd.mask);
            ImGui::InputScalar("via", ImGuiDataType_U8, &cmd.via);
            ImGui::InputScalar("spare", ImGuiDataType_U8, &cmd.spare);
            ImGui::InputText("iface_name", cmd.iface_name, sizeof(cmd.iface_name));
            if (ImGui::Button("Generate CMD##direct_3159")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3160: {
            UELYSYS_MEOW_CspRerouteSetCmd_t &cmd = command5->uelysys_meow_cspreroutesetcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_REROUTE_SET_CC);
            ImGui::InputScalar("msgid##direct_3160", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3160", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("idx", ImGuiDataType_U8, &cmd.idx);
            ImGui::InputScalar("dst_port", ImGuiDataType_U8, &cmd.dst_port);
            ImGui::InputScalar("src_node", ImGuiDataType_U8, &cmd.src_node);
            ImGui::InputScalar("fwd_dst", ImGuiDataType_U8, &cmd.fwd_dst);
            ImGui::InputScalar("fwd_dst_port", ImGuiDataType_U8, &cmd.fwd_dst_port);
            ImGui::InputScalar("fwd_src_port", ImGuiDataType_U8, &cmd.fwd_src_port);
            ImGui::InputScalar("active", ImGuiDataType_U8, &cmd.active);
            ImGui::InputScalar("spare", ImGuiDataType_U8, &cmd.spare);
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U16, &cmd.timeout_ms);
            if (ImGui::Button("Generate CMD##direct_3160")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 3161: {
            UELYSYS_MEOW_CspRerouteClearCmd_t &cmd = command5->uelysys_meow_csprerouteclearcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_MEOW_CMD_ID);
            static int last_meow_sat_mode = -1;
            const uint16_t default_meow_msgid = (mim::sat::Current() == mim::sat::SAT_BEE1012) ? BEE1012_MEOW_CMD_ID : UELYSYS_MEOW_CMD_ID;
            if (last_meow_sat_mode != mim::sat::Current()) {
                msgid = default_meow_msgid;
                last_meow_sat_mode = mim::sat::Current();
            }
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_CSP_REROUTE_CLEAR_CC);
            ImGui::InputScalar("msgid##direct_3161", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3161", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("idx", ImGuiDataType_U8, &cmd.idx);
            if (ImGui::Button("Generate CMD##direct_3161")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)((sizeof(cmd)) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)(sizeof(cmd));
                const uint8_t *p = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + (sizeof(cmd)));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)(sizeof(cmd));
                memcpy(pkt->Data, &cmd, (sizeof(cmd)));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        // ==================== mission ====================
    default:
        return false;
    }
    return true;
}
