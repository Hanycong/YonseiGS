// View - 명령 생성기 입력 폼: UELYSYS MEOW (명령 번호 4000, 4001, 4010, 4011, 4012, 4013, 4014, 4015, 4016, 4017, 4018, 4019, 4020, 4021, 4022, 4023, 4024, 4025, 4026, 4027, 4028, 4100, 4101, 4110, 4111, 4112, 4113, 4114, 4115, 4120, 4121, 4122, 4123, 4124, 4125, 4126, 4127, 4128, 4129, 4140, 4141, 4142, 4143, 4144, 4145)
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
#include "noarg_form.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

// 인자 없는 명령 (머리 8바이트뿐): { 명령 번호, 기본 MID, 기본 CC, 위젯 ID 꼬리표 }. 폼은 noarg_form 이 그린다
static mim::ui::NoArgCommandForm kNoArgForms[] = {
    {  4000, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_NOOP_CC,                       "##direct_3000" },
    {  4001, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_RESET_COUNTERS_CC,             "##direct_3001" },
    {  4010, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_GET_TIMELINE_HK_CC,            "##direct_3010" },
    {  4011, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_RESET_TIMELINE_HK_CC,          "##direct_3011" },
    {  4012, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC,    "##direct_3012" },
    {  4013, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC,          "##direct_3013" },
    {  4014, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC,         "##direct_3014" },
    {  4019, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_DELETE_ALL_ENTRIES_CC,         "##direct_3019" },
    {  4022, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC,  "##direct_3022" },
    {  4023, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC, "##direct_3023" },
    {  4027, SD1SAT_TTC_CMD_ID,  UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC,       "##direct_3027" },
    {  4100, SD1SAT_MEOW_CMD_ID, UELYSYS_MEOW_NOOP_CC,                      "##direct_3100" },
    {  4101, SD1SAT_MEOW_CMD_ID, UELYSYS_MEOW_RESET_COUNTERS_CC,            "##direct_3101" },
    {  4141, SD1SAT_MEOW_CMD_ID, UELYSYS_MEOW_SYS_SYNC_CC,                  "##direct_3141" },
    {  4142, SD1SAT_MEOW_CMD_ID, UELYSYS_MEOW_SYS_INFO_CC,                  "##direct_3142" },
    {  4143, SD1SAT_MEOW_CMD_ID, UELYSYS_MEOW_SYS_TIME_GET_CC,              "##direct_3143" },
};

bool DrawUelysysMeowForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 4015: {
            UELYSYS_TTC_InsertAbsCmdEntryCmd_t &cmd = command5->uelysys_ttc_insertabscmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3015", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3015", ImGuiDataType_U8, &fnccode);
            static char command_hex[2048] = {};
            ImGui::InputScalar("ExecutionTimeAbsolute", ImGuiDataType_U32, &cmd.ExecutionTimeAbsolute);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            ImGui::InputScalar("CommandSize", ImGuiDataType_U16, &cmd.CommandSize);
            ImGui::InputTextMultiline("Command hex", command_hex, sizeof(command_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Command, sizeof(cmd.Command), command_hex);
            if (cmd.CommandSize > copied) cmd.CommandSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3015")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4016: {
            UELYSYS_TTC_InsertRelCmdEntryCmd_t &cmd = command5->uelysys_ttc_insertrelcmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3016", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3016", ImGuiDataType_U8, &fnccode);
            static char command_hex[2048] = {};
            ImGui::InputScalar("ExecutionTimeRelative", ImGuiDataType_U16, &cmd.ExecutionTimeRelative);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            ImGui::InputScalar("CommandSize", ImGuiDataType_U16, &cmd.CommandSize);
            ImGui::InputTextMultiline("Command hex", command_hex, sizeof(command_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Command, sizeof(cmd.Command), command_hex);
            if (cmd.CommandSize > copied) cmd.CommandSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3016")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4017: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_DELETE_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3017", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3017", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3017")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4018: {
            UELYSYS_TTC_DeleteGroupCmd_t &cmd = command5->uelysys_ttc_deletegroupcmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_DELETE_CMD_GROUP_CC);
            ImGui::InputScalar("msgid##direct_3018", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3018", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3018")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4020: {
            UELYSYS_TTC_ExecuteEntryCmd_t &cmd = command5->uelysys_ttc_executeentrycmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3020", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3020", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::Checkbox("PersistentExecution", &cmd.PersistentExecution);
            if (ImGui::Button("Generate CMD##direct_3020")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4021: {
            UELYSYS_TTC_ExecuteGroupCmd_t &cmd = command5->uelysys_ttc_executegroupcmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_EXECUTE_CMD_GROUP_CC);
            ImGui::InputScalar("msgid##direct_3021", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3021", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::Checkbox("PersistentExecution", &cmd.PersistentExecution);
            if (ImGui::Button("Generate CMD##direct_3021")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4024: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_INIT_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3024", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3024", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3024")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4025: {
            UELYSYS_TTC_PlumbEntryWriteCmd_t &cmd = command5->uelysys_ttc_plumbwritecmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3025", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3025", ImGuiDataType_U8, &fnccode);
            static char data_hex[1024] = {};
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("Offset", ImGuiDataType_U16, &cmd.Offset);
            ImGui::InputScalar("ChunkSize", ImGuiDataType_U16, &cmd.ChunkSize);
            ImGui::InputTextMultiline("Data hex", data_hex, sizeof(data_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.Data, sizeof(cmd.Data), data_hex);
            if (cmd.ChunkSize > copied) cmd.ChunkSize = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3025")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4026: {
            UELYSYS_TTC_PlumbEntryFinalizeCmd_t &cmd = command5->uelysys_ttc_plumbfinalizecmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3026", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3026", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            ImGui::InputScalar("TimeTag", ImGuiDataType_U32, &cmd.TimeTag);
            ImGui::InputScalar("StalenessThreshold", ImGuiDataType_U16, &cmd.StalenessThreshold);
            ImGui::InputScalar("CmdSize", ImGuiDataType_U16, &cmd.CmdSize);
            ImGui::InputScalar("TimeTagType", ImGuiDataType_U8, &cmd.TimeTagType);
            ImGui::InputScalar("ExecutionType", ImGuiDataType_U8, &cmd.ExecutionType);
            if (ImGui::Button("Generate CMD##direct_3026")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 4028: {
            UELYSYS_TTC_DeleteEntryCmd_t &cmd = command5->uelysys_ttc_deleteentrycmd;
            static uint16_t msgid = (uint16_t)(SD1SAT_TTC_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC);
            ImGui::InputScalar("msgid##direct_3028", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3028", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("EntryId", ImGuiDataType_U16, &cmd.EntryId);
            ImGui::InputScalar("GroupId", ImGuiDataType_U16, &cmd.GroupId);
            if (ImGui::Button("Generate CMD##direct_3028")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_TTC_DeleteEntryCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        // ==================== meow ====================
        case 4110: {
            static SD1SAT_MEOW_ShellExecSyncCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_SYNC_CC);
            ImGui::InputScalar("msgid##direct_3110", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3110", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            if (ImGui::Button("Generate CMD##direct_3110")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(SD1SAT_MEOW_ShellExecSyncCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(SD1SAT_MEOW_ShellExecSyncCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4111: {
            static SD1SAT_MEOW_ShellExecAsyncCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC);
            ImGui::InputScalar("msgid##direct_3111", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3111", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            if (ImGui::Button("Generate CMD##direct_3111")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4112: {
            static SD1SAT_MEOW_ShellPollCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_POLL_CC);
            ImGui::InputScalar("msgid##direct_3112", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3112", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("pid", ImGuiDataType_S32, &cmd.pid);
            if (ImGui::Button("Generate CMD##direct_3112")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4113: {
            static SD1SAT_MEOW_ShellKillCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_KILL_CC);
            ImGui::InputScalar("msgid##direct_3113", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3113", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("pid", ImGuiDataType_S32, &cmd.pid);
            ImGui::InputScalar("sig", ImGuiDataType_S32, &cmd.sig);
            if (ImGui::Button("Generate CMD##direct_3113")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4114: {
            static SD1SAT_MEOW_ShellExecSyncForceCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC);
            ImGui::InputScalar("msgid##direct_3114", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3114", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("timeout_ms", ImGuiDataType_U32, &cmd.timeout_ms);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3114")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_ShellExecSyncForceCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4115: {
            static SD1SAT_MEOW_ShellExecAsyncForceCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC);
            ImGui::InputScalar("msgid##direct_3115", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3115", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("cmd", cmd.cmd, sizeof(cmd.cmd));
            ImGui::InputText("redir_path", cmd.redir_path, sizeof(cmd.redir_path));
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3115")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_ShellExecAsyncForceCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4120: {
            static SD1SAT_MEOW_FileReadCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_READ_CC);
            ImGui::InputScalar("msgid##direct_3120", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3120", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("offset", ImGuiDataType_U32, &cmd.offset);
            ImGui::InputScalar("size", ImGuiDataType_U32, &cmd.size);
            if (ImGui::Button("Generate CMD##direct_3120")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4121: {
            static SD1SAT_MEOW_FileWriteCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_WRITE_CC);
            ImGui::InputScalar("msgid##direct_3121", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3121", ImGuiDataType_U8, &fnccode);
            static char data_hex[1024] = {};
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("offset", ImGuiDataType_U32, &cmd.offset);
            ImGui::InputScalar("data_len", ImGuiDataType_U16, &cmd.data_len);
            ImGui::InputScalar("flags", ImGuiDataType_U8, &cmd.flags);
            ImGui::InputTextMultiline("data hex", data_hex, sizeof(data_hex), ImVec2(0, ImGui::GetTextLineHeight() * 4));
            size_t copied = UelysysCopyHexBytes(cmd.data, sizeof(cmd.data), data_hex);
            if (cmd.data_len > copied) cmd.data_len = (uint16_t)copied;
            if (ImGui::Button("Generate CMD##direct_3121")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4122: {
            static SD1SAT_MEOW_FilePathCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_REMOVE_CC);
            ImGui::InputScalar("msgid##direct_3122", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3122", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3122")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4123: {
            static SD1SAT_MEOW_FileSrcDstCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_COPY_CC);
            ImGui::InputScalar("msgid##direct_3123", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3123", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("src", cmd.src, sizeof(cmd.src));
            ImGui::InputText("dst", cmd.dst, sizeof(cmd.dst));
            if (ImGui::Button("Generate CMD##direct_3123")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4124: {
            static SD1SAT_MEOW_FileSrcDstCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_MOVE_CC);
            ImGui::InputScalar("msgid##direct_3124", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3124", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("src", cmd.src, sizeof(cmd.src));
            ImGui::InputText("dst", cmd.dst, sizeof(cmd.dst));
            if (ImGui::Button("Generate CMD##direct_3124")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FileSrcDstCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4125: {
            static SD1SAT_MEOW_FilePathCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_STAT_CC);
            ImGui::InputScalar("msgid##direct_3125", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3125", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3125")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4126: {
            static SD1SAT_MEOW_FilePathCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_TRUNCATE_CC);
            ImGui::InputScalar("msgid##direct_3126", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3126", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3126")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4127: {
            static SD1SAT_MEOW_FileTailCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_TAIL_CC);
            ImGui::InputScalar("msgid##direct_3127", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3127", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            ImGui::InputScalar("n_bytes", ImGuiDataType_U32, &cmd.n_bytes);
            ImGui::InputScalar("size", ImGuiDataType_U32, &cmd.size);
            if (ImGui::Button("Generate CMD##direct_3127")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4128: {
            static SD1SAT_MEOW_FilePathCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_FILE_CHECKSUM_CC);
            ImGui::InputScalar("msgid##direct_3128", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3128", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3128")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4129: {
            static SD1SAT_MEOW_FilePathCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_DISK_STAT_CC);
            ImGui::InputScalar("msgid##direct_3129", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3129", ImGuiDataType_U8, &fnccode);
            ImGui::InputText("path", cmd.path, sizeof(cmd.path));
            if (ImGui::Button("Generate CMD##direct_3129")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, sizeof(UELYSYS_MEOW_FilePathCmd_t), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4140: {
            static SD1SAT_MEOW_SysShutdownCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_SHUTDOWN_CC);
            ImGui::InputScalar("msgid##direct_3140", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3140", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("type", ImGuiDataType_S32, &cmd.type);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            if (ImGui::Button("Generate CMD##direct_3140")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4144: {
            static SD1SAT_MEOW_SysTimeSetCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_TIME_SET_CC);
            ImGui::InputScalar("msgid##direct_3144", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3144", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("sec", ImGuiDataType_S64, &cmd.sec);
            ImGui::InputScalar("nsec", ImGuiDataType_U32, &cmd.nsec);
            if (ImGui::Button("Generate CMD##direct_3144")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 4145: {
            static SD1SAT_MEOW_SysForceKillCmd_t cmd{};
            static uint16_t msgid = (uint16_t)(SD1SAT_MEOW_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_MEOW_SYS_FORCE_KILL_CC);
            ImGui::InputScalar("msgid##direct_3145", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_3145", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("magic", ImGuiDataType_U32, &cmd.magic);
            ImGui::InputScalar("mode", ImGuiDataType_S32, &cmd.mode);
            if (ImGui::Button("Generate CMD##direct_3145")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        // ==================== SD1-SAT: uant ====================
    default:
        return false;
    }
    return true;
}
