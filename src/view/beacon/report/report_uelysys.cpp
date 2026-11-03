// View - Beacon 창 "Report" 탭: UELYSYS 앱 리포트의 ReturnValue 해석
//
// GS_nuri5 miman_imgui.cpp Report 탭의 UELYSYS 앱 리포트 (TTC/MEOW/STX/UANT/UTRX/GPIO/PAYUEL/EO/SCH_LAB/SP/GPS) 를 옮긴 것 (행 이름/형식 그대로).
// report_app.cpp 의 DrawAppReport 가 머리(Type, Parsed payload size)를 그린 뒤 부른다. 처리한 종류면 true.
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawUelysysAppBody(ReportCtx & c)
{
    ReportView_t & view = c.view;
    const EPS_Report_State_t & eps_state = c.eps_state;
    const EPS_RParam_Reassembly_State_t & eps_rparam = c.eps_rparam;
    bool & show_hexdump = c.show_hexdump;
    (void)eps_state; (void)eps_rparam; (void)show_hexdump;
    auto RowEPSHKAllSummary = [&]() { rpt5::RowEPSHKAllSummary(eps_state); };
    (void)RowEPSHKAllSummary;
    const uint8_t *p = view.raw_bytes;
    const uint16_t n = view.raw_size;
    auto CopyLocal = [&](void *dst, size_t size) {
        if (n < size) return false;
        memcpy(dst, p, size);
        return true;
    };
    auto CopyExactLocal = [&](void *dst, size_t size) {
        if (n != size) return false;
        memcpy(dst, p, size);
        return true;
    };
    (void)CopyLocal; (void)CopyExactLocal;

    if (view.kind == REPORT_KIND_UELYSYS_TTC)
    {
        switch (view.reflected_cc)
        {
            case UELYSYS_TTC_NOOP_CC:
            case UELYSYS_TTC_RESET_COUNTERS_CC:
            case UELYSYS_TTC_GET_TIMELINE_HK_CC:
            case UELYSYS_TTC_RESET_TIMELINE_HK_CC:
            case UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC:
            case UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC: {
                UELYSYS_TTC_Counters_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("CommandCounter", "%" PRIu16, x.counters[0]);
                    RowFmt("ErrorCounter", "%" PRIu16, x.counters[1]);
                }
                break;
            }
            case UELYSYS_TTC_REPORT_CC: {
                UELYSYS_TTC_ReportStatus_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("CommandCounter", "%" PRIu8, x.cmd_counter);
                    RowFmt("CommandErrorCounter", "%" PRIu8, x.cmd_error_counter);
                    RowFmt("PendingEntryCount", "%" PRIu16, x.pending_entry_count);
                    RowFmt("ExecutionPaused", "%" PRIu8, x.execution_paused);
                }
                break;
            }
            case UELYSYS_TTC_START_BEACON_BURST_CC: {
                UELYSYS_TTC_BeaconBurst_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("FirstEntryId", "%" PRIu16, x.first_entry_id);
                    RowFmt("ScheduledEntries", "%" PRIu16, x.scheduled_entries);
                    RowFmt("FailCount", "%" PRIu16, x.fail_count);
                }
                break;
            }
            case UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC:
            case UELYSYS_TTC_DELETE_ALL_ENTRIES_CC:
            case UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC: {
                uint16_t x = 0;
                if (CopyExactLocal(&x, sizeof(x))) RowFmt("Count", "%" PRIu16, x);
                break;
            }
            case UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC: {
                UELYSYS_TTC_NextEntry_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("HasEntry", "%" PRIu8, x.has_entry);
                }
                break;
            }
            case UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC: {
                UELYSYS_TTC_NextExecution_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("NextExecutionTime", "%" PRIu32, x.next_execution_time);
                    RowFmt("HasEntry", "%" PRIu8, x.has_entry);
                }
                break;
            }
            case UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC: {
                UELYSYS_TTC_InsertAbs_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("ExecutionTimeAbsolute", "%" PRIu32, x.execution_time_absolute);
                    RowFmt("StalenessThreshold", "%" PRIu16, x.staleness_threshold);
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("ExecutionType", "%" PRIu8, x.execution_type);
                    RowFmt("CommandSize", "%" PRIu16, x.command_size);
                }
                break;
            }
            case UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC: {
                UELYSYS_TTC_InsertRel_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("ExecutionTimeRelative", "%" PRIu16, x.execution_time_relative);
                    RowFmt("StalenessThreshold", "%" PRIu16, x.staleness_threshold);
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("ExecutionType", "%" PRIu8, x.execution_type);
                    RowFmt("CommandSize", "%" PRIu16, x.command_size);
                }
                break;
            }
            case UELYSYS_TTC_DELETE_CMD_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_INIT_ENTRY_CC:
            case UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC: {
                UELYSYS_TTC_EntryGroup_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                }
                break;
            }
            case UELYSYS_TTC_DELETE_CMD_GROUP_CC: {
                UELYSYS_TTC_DeleteGroup_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("DeleteCount", "%" PRIu16, x.delete_count);
                }
                break;
            }
            case UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC: {
                UELYSYS_TTC_ExecuteEntry_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("Persistent", "%" PRIu8, x.persistent);
                }
                break;
            }
            case UELYSYS_TTC_EXECUTE_CMD_GROUP_CC: {
                UELYSYS_TTC_ExecuteGroup_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("Persistent", "%" PRIu8, x.persistent);
                }
                break;
            }
            case UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC: {
                UELYSYS_TTC_PlumbWrite_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("Offset", "%" PRIu16, x.offset);
                    RowFmt("ChunkSize", "%" PRIu16, x.chunk_size);
                    RowArr_Hex("Data", x.data,
                               (x.chunk_size > sizeof(x.data)) ? sizeof(x.data) : x.chunk_size, 16);
                }
                break;
            }
            case UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC: {
                UELYSYS_TTC_PlumbFinalize_Report_t x{};
                if (CopyExactLocal(&x, sizeof(x))) {
                    RowFmt("EntryId", "%" PRIu16, x.entry_id);
                    RowFmt("GroupId", "%" PRIu16, x.group_id);
                    RowFmt("TimeTag", "%" PRIu32, x.time_tag);
                    RowFmt("StalenessThreshold", "%" PRIu16, x.staleness_threshold);
                    RowFmt("CommandSize", "%" PRIu16, x.command_size);
                    RowFmt("TimeTagType", "%" PRIu8, x.time_tag_type);
                    RowFmt("ExecutionType", "%" PRIu8, x.execution_type);
                }
                break;
            }
            default: RowText("Payload", n ? "See full raw bytes below" : "No return payload"); break;
        }
    }
    else if (view.kind == REPORT_KIND_UELYSYS_MEOW)
    {
        const bool uelysys_meow =
            (view.reflected_msg_id == UELYSYS_MEOW_CMD_ID || view.reflected_msg_id == 0x8618);
        const bool success = (view.ret_type == 0);
        if (uelysys_meow &&
            (view.reflected_cc == UELYSYS_MEOW_NOOP_CC ||
             view.reflected_cc == UELYSYS_MEOW_RESET_COUNTERS_CC) && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if (uelysys_meow &&
                   (view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_SYNC_CC ||
                    view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC) &&
                   n == sizeof(UELYSYS_MEOW_ShellResultErrno_Report_t)) {
            UELYSYS_MEOW_ShellResultErrno_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("exit_code", "%" PRId32, x.exit_code);
            RowFmt("pid", "%" PRId32, x.pid);
            RowFmt("os_errno", "%" PRId32, x.os_errno);
        } else if (uelysys_meow &&
                   (view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_SYNC_CC ||
                    view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC) &&
                   n == sizeof(UELYSYS_MEOW_ShellResult_Report_t)) {
            UELYSYS_MEOW_ShellResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("exit_code", "%" PRId32, x.exit_code);
            RowFmt("pid", "%" PRId32, x.pid);
        } else if (uelysys_meow &&
                   (view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC ||
                    view.reflected_cc == UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC) &&
                   n == sizeof(UELYSYS_MEOW_ValueErrno_Report_t)) {
            UELYSYS_MEOW_ValueErrno_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("pid", "%" PRId32, x.value);
            RowFmt("os_errno", "%" PRId32, x.os_errno);
        } else if (uelysys_meow && view.reflected_cc == UELYSYS_MEOW_SHELL_POLL_CC &&
                   n == sizeof(UELYSYS_MEOW_ShellPoll_Report_t)) {
            UELYSYS_MEOW_ShellPoll_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("pid", "%" PRId32, x.pid);
            RowFmt("exit_code", "%" PRId32, x.exit_code);
        } else if (uelysys_meow &&
                   (view.reflected_cc == UELYSYS_MEOW_FILE_READ_CC ||
                    view.reflected_cc == UELYSYS_MEOW_FILE_TAIL_CC) && success) {
            RowArr_Hex("file_data", p, n, 16);
        } else if (view.reflected_cc == UELYSYS_MEOW_FILE_STAT_CC &&
                   n == sizeof(UELYSYS_MEOW_FileStat_Report_t)) {
            UELYSYS_MEOW_FileStat_Report_t x{};
            if (CopyLocal(&x, sizeof(x))) {
                RowFmt("size", "%" PRIu64, x.size);
                RowFmt("mtime", "%" PRId64, x.mtime);
                RowFmt("ctime", "%" PRId64, x.ctime);
                RowFmt("mode", "0x%08X", (unsigned)x.mode);
                RowFmt("is_dir", "%" PRId32, x.is_dir);
                RowFmt("preview_len", "%" PRIu64, x.preview_len);
                RowArr_Hex("preview", x.preview,
                           (x.preview_len > sizeof(x.preview)) ? sizeof(x.preview) : (int)x.preview_len, 16);
            }
        } else if (view.reflected_cc == UELYSYS_MEOW_DISK_STAT_CC &&
                   n == sizeof(UELYSYS_MEOW_DiskStat_Report_t)) {
            UELYSYS_MEOW_DiskStat_Report_t x{};
            if (CopyLocal(&x, sizeof(x))) {
                RowFmt("total_bytes", "%" PRIu64, x.total_bytes);
                RowFmt("avail_bytes", "%" PRIu64, x.avail_bytes);
            }
        } else if (view.reflected_cc == UELYSYS_MEOW_SYS_INFO_CC &&
                   n == sizeof(UELYSYS_MEOW_SysInfo_Report_t)) {
            UELYSYS_MEOW_SysInfo_Report_t x{};
            if (CopyLocal(&x, sizeof(x))) {
                RowFmt("uptime", "%" PRIu32, x.uptime);
                RowFmt("load1", "%" PRIu32, x.load1);
                RowFmt("load5", "%" PRIu32, x.load5);
                RowFmt("load15", "%" PRIu32, x.load15);
                RowFmt("total_ram", "%" PRIu64, x.total_ram);
                RowFmt("free_ram", "%" PRIu64, x.free_ram);
                RowFmt("num_procs", "%" PRIu16, x.num_procs);
            }
        } else if (view.reflected_cc == UELYSYS_MEOW_SYS_TIME_GET_CC &&
                   n == sizeof(UELYSYS_MEOW_SysTime_Report_t)) {
            UELYSYS_MEOW_SysTime_Report_t x{};
            if (CopyLocal(&x, sizeof(x))) {
                RowFmt("sec", "%" PRId64, x.sec);
                RowFmt("nsec", "%" PRIu32, x.nsec);
            }
        } else if (view.reflected_cc == UELYSYS_MEOW_CSP_IFSTATS_CC &&
                   n == sizeof(UELYSYS_MEOW_CspIfStats_Report_t)) {
            UELYSYS_MEOW_CspIfStats_Report_t x{};
            if (CopyLocal(&x, sizeof(x))) {
                RowFmt("tx", "%" PRIu32, x.tx);
                RowFmt("rx", "%" PRIu32, x.rx);
                RowFmt("tx_error", "%" PRIu32, x.tx_error);
                RowFmt("rx_error", "%" PRIu32, x.rx_error);
                RowFmt("drop", "%" PRIu32, x.drop);
            }
        } else if (view.reflected_cc == UELYSYS_MEOW_FILE_CHECKSUM_CC &&
                   n == sizeof(uint32_t) && success) {
            uint32_t crc = 0; memcpy(&crc, p, sizeof(crc));
            RowFmt("crc32", "0x%08X", (unsigned)crc);
        } else if (!uelysys_meow && n == 8) {
            int32_t pair[2]{}; memcpy(pair, p, sizeof(pair));
            RowFmt("value0", "%" PRId32, pair[0]);
            RowFmt("value1", "%" PRId32, pair[1]);
        } else if (n == sizeof(int32_t)) {
            int32_t value = 0; memcpy(&value, p, sizeof(value));
            RowFmt(uelysys_meow ? "error_detail" : "value", "%" PRId32, value);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_UELYSYS_STX)
    {
        if (view.reflected_cc == UELYSYS_STX_NOOP_CC){
                STX_NOOP_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowFmt("CmdCounter", "%" PRIu16, report.CmdCounter);
                RowFmt("ErrCounter", "%" PRIu16, report.ErrCounter);
            } else if (view.reflected_cc == UELYSYS_STX_RESET_COUNTERS_CC){
                STX_ResetCounters_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowFmt("CmdCounter", "%" PRIu16, report.CmdCounter);
                RowFmt("ErrCounter", "%" PRIu16, report.ErrCounter);
            } else if (view.reflected_cc == UELYSYS_STX_INITIALIZE_CC ) {
            STX_Initialize_Report_t report;
                memcpy(&report, p, sizeof(report));
                HeaderRow("Module 1");
                RowSTXRet(report.ret[0]);
                HeaderRow("Module 2");
                RowSTXRet(report.ret[1]);   // GS_nuri5 는 ret[2] (배열 밖)
        } else if (view.reflected_cc == UELYSYS_STX_SET_MODULE_ID_CC) {
                STX_SetModuleID_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowFmt("Module ID", "%" PRIu16, report.module_id);
            } else if (view.reflected_cc == UELYSYS_STX_PARAM_INIT_CC) {
                STX_ParamInit_Report_t report;
                memcpy(&report, p, sizeof(report));
                HeaderRow("Symbol Rate");
                RowSTXRet(report.SymbolRateStatus);
                HeaderRow("Center Freq");
                RowSTXRet(report.CenterFrequencyStatus);
                HeaderRow("Mod Code");
                RowSTXRet(report.ModcodStatus);
                HeaderRow("Roll Off");
                RowSTXRet(report.RollOffStatus);
                HeaderRow("Tx Power");
                RowSTXRet(report.TxPowerStatus);
            } else if (view.reflected_cc == UELYSYS_STX_SET_SYMBOL_RATE_CC) {
                STX_SetSymbolRate_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_TX_POWER_CC) {
                STX_SetTxPower_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_CENTER_FREQ_CC) {
                STX_SetCenterFreq_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_MODCOD_CC) {
                STX_SetModCode_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_ROLL_OFF_CC) {
                STX_SetRollOff_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_PILOT_SIGNAL_CC) {
                STX_SetPilotSig_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_FEC_FRAME_SIZE_CC) {
                STX_SetFECFrame_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_PRETX_DELAY_CC) {
                STX_SetPreTxDelay_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_ALL_PARAMETERS_CC) {
                STX_SetAllParam_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_RS485_BAUD_CC) {
                STX_SetRS485_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SET_MOD_DATA_IFACE_CC) {
                STX_SetModInterface_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_GET_SYMBOL_RATE_CC) {
                STX_GetSymbolRate_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("symbol rate", "%" PRIu8, report.msps);
            } else if (view.reflected_cc == UELYSYS_STX_GET_TX_POWER_CC) {
                STX_GetTxPower_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("tx power", "%" PRIu8, report.dbm);
            } else if (view.reflected_cc == UELYSYS_STX_GET_CENTER_FREQ_CC) {
                STX_GetCenterFreq_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("center freq", "%f", report.mhz);
            } else if (view.reflected_cc == UELYSYS_STX_GET_MODCOD_CC) {
                STX_GetModCode_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("modcod", "%" PRIu8, report.modcod);
            } else if (view.reflected_cc == UELYSYS_STX_GET_ROLL_OFF_CC) {
                STX_GetRollOff_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("roll off", "%" PRIu8, report.roll_off);
            } else if (view.reflected_cc == UELYSYS_STX_GET_PILOT_SIGNAL_CC) {
                STX_GetPilot_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("pilot", "%" PRIu8, report.pilot);
            } else if (view.reflected_cc == UELYSYS_STX_GET_FEC_FRAME_SIZE_CC) {
                STX_GetFECFrame_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("frame", "%" PRIu8, report.frame);
            } else if (view.reflected_cc == UELYSYS_STX_GET_PRETX_DELAY_CC) {
                STX_GetPreTxDelay_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("pretx delay", "%" PRIu16, report.ms);
            } else if (view.reflected_cc == UELYSYS_STX_GET_ALL_PRAMETERS_CC) {
                STX_GetAllParams_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("symbol rate", "%" PRIu8, report.params.symbol_rate);
                RowFmt("tx power", "%" PRIu8, report.params.tx_power_dbm);
                RowFmt("modcod", "%" PRIu8, report.params.modcod);
                RowFmt("roll off", "%" PRIu8, report.params.roll_off);
                RowFmt("pilot", "%" PRIu8, report.params.pilot);
                RowFmt("fec frame", "%" PRIu8, report.params.fec_frame_short);
                RowFmt("pretx delay", "%" PRIu16, report.params.pretx_delay_ms);
                RowFmt("center freq", "%f", report.params.center_freq_mhz);
            } else if (view.reflected_cc == UELYSYS_STX_GET_REPORT_CC) {
                STX_GetReport_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("state", "%" PRIu8, report.report.state);
                RowFmt("Flags", "%" PRIu8, report.report.flags);
                RowFmt("cpu_temp", "%f", report.report.cpu_temp_c);
                RowFmt("FW version", "%" PRIu32, report.report.fw_version);
            } else if (view.reflected_cc == UELYSYS_STX_GET_MOD_DATA_IFACE_CC) {
                STX_GetDataIface_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("interface type", "%" PRIu8, report.iface.interface_type);
                RowFmt("lvds_io_type", "%" PRIu8, report.iface.lvds_io_type);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_DIR) {
                // GS_nuri5 와 달리: 받은 크기(n)만 복사, n < 4 이면 항목 0개, 항목 수는 배열 크기까지,
                // 이름은 31자까지 (널 문자가 없어도 옆 필드를 읽지 않게)
                STX_DIR_Report_t report{};
                memcpy(&report, p, n < sizeof(report) ? n : sizeof(report));
                RowSTXRet(report.ret);
                int count = (n >= sizeof(esup_ret_t)) ? (int)((n - sizeof(esup_ret_t)) / sizeof(stx_dir_entry_t)) : 0;
                if (count > (int)(sizeof(report.entries) / sizeof(report.entries[0])))
                    count = (int)(sizeof(report.entries) / sizeof(report.entries[0]));
                for (int i = 0; i < count; i ++){
                    RowFmt("file count", "%d", i);
                    RowFmt("file name", "%.*s", (int)sizeof(report.entries[i].name), report.entries[i].name);
                    RowFmt("file size", "%" PRIu32, report.entries[i].length);
                }
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_DIRNEXT) {
                // GS_nuri5 와 달리: 받은 크기(n)만 복사, n < 4 이면 항목 0개, 항목 수는 배열 크기까지,
                // 이름은 31자까지 (널 문자가 없어도 옆 필드를 읽지 않게)
                STX_DIRNext_Report_t report{};
                memcpy(&report, p, n < sizeof(report) ? n : sizeof(report));
                RowSTXRet(report.ret);
                int count = (n >= sizeof(esup_ret_t)) ? (int)((n - sizeof(esup_ret_t)) / sizeof(stx_dir_entry_t)) : 0;
                if (count > (int)(sizeof(report.entries) / sizeof(report.entries[0])))
                    count = (int)(sizeof(report.entries) / sizeof(report.entries[0]));
                for (int i = 0; i < count; i ++){
                    RowFmt("file count", "%d", i);
                    RowFmt("file name", "%.*s", (int)sizeof(report.entries[i].name), report.entries[i].name);
                    RowFmt("file size", "%" PRIu32, report.entries[i].length);
                }
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_DELFILE) {
                STX_FileAction_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_DELALLFILE) {
                STX_DelAllFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_CREATEFILE) {
                STX_CreateFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.DeviceResult);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
                RowFmt("Request Size", "%" PRIu32, report.RequestedSize);
                RowFmt("File Handle", "%" PRIx32, report.FileHandle);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_WRITEFILE) {
                STX_WriteFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.DeviceResult);
                RowFmt("Path", "%.*s", (int)sizeof(report.LocalPath), report.LocalPath);
                RowFmt("File Handle", "%" PRIx32, report.FileHandle);
                RowFmt("Remaining", "%" PRIu64, report.Remaining);
                RowFmt("Packet Num", "%" PRIu32, report.PacketNumber);
                RowFmt("Err Count", "%" PRIu8, report.ErrorCount);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_OPENFILE) {
                STX_OpenFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.DeviceResult);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
                RowFmt("File Handle", "%" PRIi32, report.FileHandle);
                RowFmt("File Length", "%" PRIu32, report.FileLength);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_READFILE) {
                STX_ReadFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("File Handle", "%" PRIu32, report.file_handle);
                RowFmt("Packet_len", "%" PRIu16, report.packet_len);
                RowFmt("Packet Num", "%" PRIu32, report.packet_number);
                // packet_len 은 장치가 알린 길이라 data[] 보다 클 수 있다 (GS_nuri5 는 그대로 읽었다)
                RowArr_Hex("Data", report.data, (int)(report.packet_len < sizeof(report.data) ? report.packet_len : sizeof(report.data)), 16);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_SENDFILE) {
                STX_SendFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_SENDFILERTI) {
                STX_SendFileWithError_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
            } else if (view.reflected_cc == UELYSYS_STX_SYSCONF_CC_TRANSMITMODE) {
                STX_TxMode_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SYSCONF_CC_IDLEMODE) {
                STX_IdleMode_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_FILESYS_CC_DELALLFILE) {
                STX_DelAllFile_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            } else if (view.reflected_cc == UELYSYS_STX_SYSCONF_CC_UPDATEFW) {
                STX_UpdateFW_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
                RowFmt("File Name", "%.*s", (int)sizeof(report.FileName), report.FileName);
            } else if (view.reflected_cc == UELYSYS_STX_SYSCONF_CC_SAFESHUTDOWN) {
                STX_ShutDown_Report_t report;
                memcpy(&report, p, sizeof(report));
                RowSTXRet(report.ret);
            }
        else RowText("Payload", n ? "See full raw bytes below" : "No return payload");
    }
    else if (view.kind == REPORT_KIND_UELYSYS_UANT)
    {
        if (view.reflected_cc == UELYSYS_UANT_GET_STATUS_CC && n == sizeof(UELYSYS_UANT_ReleaseStatus_Report_t)) {
            RowFmt("channel_0_state", "%" PRIu8, p[0]);
            RowFmt("channel_0_status", "%" PRIu8, p[1]);
            RowFmt("channel_0_burn_time_left", "%" PRIu8, p[2]);
            RowFmt("channel_0_burn_tries", "%" PRIu8, p[3]);
            RowFmt("channel_1_state", "%" PRIu8, p[4]);
            RowFmt("channel_1_status", "%" PRIu8, p[5]);
            RowFmt("channel_1_burn_time_left", "%" PRIu8, p[6]);
            RowFmt("channel_1_burn_tries", "%" PRIu8, p[7]);
        } else if (view.reflected_cc == UELYSYS_UANT_GET_BACKUP_STATUS_CC &&
                   n == sizeof(UELYSYS_UANT_BackupStatus_Report_t)) {
            UELYSYS_UANT_BackupStatus_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("state", "%" PRIu8, x.state);
            RowFmt("seconds_to_deploy", "%" PRIu32, x.seconds_to_deploy);
        } else if (view.reflected_cc == UELYSYS_UANT_GET_BOARD_STATUS_CC &&
                   n == sizeof(UELYSYS_UANT_BoardStatus_Report_t)) {
            UELYSYS_UANT_BoardStatus_Report_t x{};
            memcpy(&x, p, sizeof(x));
            RowFmt("seconds_since_boot", "%" PRIu32, x.seconds_since_boot);
            RowFmt("reboot_count", "%" PRIu8, x.reboot_count);
        } else if (view.reflected_cc == UELYSYS_UANT_GET_TEMPERATURE_CC &&
                   n == sizeof(UELYSYS_UANT_Temperature_Report_t)) {
            UELYSYS_UANT_Temperature_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("temperature", "%" PRId16, x.temperature);
        } else if (view.reflected_cc == UELYSYS_UANT_GET_SETTINGS_CC &&
                   n == sizeof(UELYSYS_UANT_Settings_Report_t)) {
            UELYSYS_UANT_Settings_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("minutes", "%" PRIu16, x.minutes);
            RowFmt("backup_active", "%" PRIu8, x.backup_active);
            RowFmt("max_burn_duration", "%" PRIu8, x.max_burn_duration);
        } else if (view.reflected_cc == UELYSYS_UANT_AUTODEPLOY_CC && n == 1)
            RowFmt("autodeploy_result", "%" PRId8, (int8_t)p[0]);
        else RowText("Payload", n ? "See full raw bytes below" : "Status only (no return payload)");
    }
    else if (view.kind == REPORT_KIND_UELYSYS_UTRX)
    {
        if ((view.reflected_cc == UELYSYS_UTRX_NOOP_CC ||
             view.reflected_cc == UELYSYS_UTRX_RESET_COUNTERS_CC ||
             view.reflected_cc == UELYSYS_UTRX_RESET_APP_COUNTERS_CC ||
             view.reflected_cc == UELYSYS_UTRX_RESET_DEVICE_COUNTERS_CC) &&
            n == sizeof(view.u.utrx_noop)) {
            RowFmt("cmd_counter", "%" PRIu8, view.u.utrx_noop.cmd_counter);
            RowFmt("app_err_counter", "%" PRIu8, view.u.utrx_noop.app_err_counter);
            RowFmt("device_err_counter", "%" PRIu8, view.u.utrx_noop.device_err_counter);
        } else if ((view.reflected_cc == UELYSYS_UTRX_RXCONF_GET_BAUD_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TXCONF_GET_BAUD_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_BOOT_CAUSE_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_LAST_CONTACT_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_TOT_TX_BYTES_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_TOT_RX_BYTES_CC ||
                    view.reflected_cc == UELYSYS_UTRX_RXCONF_GET_FREQ_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TXCONF_GET_FREQ_CC) && n == sizeof(uint32_t)) {
            uint32_t value = 0; memcpy(&value, p, sizeof(value)); RowFmt("value", "%" PRIu32, value);
        } else if ((view.reflected_cc == UELYSYS_UTRX_RXCONF_GET_GUARD_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_BOOT_COUNT_CC) && n == sizeof(uint16_t)) {
            uint16_t value = 0; memcpy(&value, p, sizeof(value)); RowFmt("value", "%" PRIu16, value);
        } else if ((view.reflected_cc == UELYSYS_UTRX_TLM_GET_TEMP_BRD_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_LAST_RSSI_CC ||
                    view.reflected_cc == UELYSYS_UTRX_TLM_GET_LAST_RFERR_CC) && n == sizeof(int16_t)) {
            int16_t value = 0; memcpy(&value, p, sizeof(value)); RowFmt("value", "%" PRId16, value);
        } else if (view.reflected_cc == UELYSYS_UTRX_TLM_GET_ACTIVE_CONF_CC && n == 1)
            RowFmt("active_configuration", "%" PRIu8, p[0]);
        else
            RowText("Payload", n ? "Invalid unexpected payload" : "Status only (no return payload)");
    }
    else if (view.kind == REPORT_KIND_UELYSYS_GPIO)
    {
        if ((view.reflected_cc == UELYSYS_GPIO_NOOP_CC ||
             view.reflected_cc == UELYSYS_GPIO_RESET_COUNTERS_CC) && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if (view.reflected_cc == UELYSYS_GPIO_DIGPIOLAY_PARAM_CC &&
                   n == sizeof(UELYSYS_GPIO_Digpiolay_Report_t)) {
            UELYSYS_GPIO_Digpiolay_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("ValU32", "%" PRIu32, x.val_u32);
            RowFmt("ValI16", "%" PRId16, x.val_i16);
            char str[sizeof(x.val_str) + 1]{};
            memcpy(str, x.val_str, sizeof(x.val_str));
            RowText("ValStr", str);
        } else if (view.reflected_cc >= UELYSYS_GPIO_STX_EN1_HIGH_CC &&
                   view.reflected_cc <= UELYSYS_GPIO_ADCS_BOOT_LOW_CC && n == 1) {
            RowFmt("pin_state", "%" PRIu8, p[0]);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC ||
             view.kind == REPORT_KIND_UELYSYS_PAYUEL_CAM)
    {
        const bool is_obc = view.kind == REPORT_KIND_UELYSYS_PAYUEL_OBC;
        const bool meta = (is_obc && (view.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC ||
                                    view.reflected_cc == UELYSYS_PAYUEL_OBC_SENSOR_META_CC)) ||
                          (!is_obc && view.reflected_cc == UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC);
        const bool download = (is_obc && (view.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC ||
                                        view.reflected_cc == UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC)) ||
                              (!is_obc && view.reflected_cc == UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC);
        const bool chunk = (is_obc && (view.reflected_cc == UELYSYS_PAYUEL_OBC_CHUNK_DOWNLOAD_CC ||
                                     view.reflected_cc == UELYSYS_PAYUEL_OBC_SENSOR_CHUNK_CC)) ||
                           (!is_obc && view.reflected_cc == UELYSYS_PAYUEL_CAM_CHUNK_DOWNLOAD_CC);
        const bool status = (is_obc && view.reflected_cc == UELYSYS_PAYUEL_OBC_CAM_SHOT_CC) ||
                            (!is_obc && (view.reflected_cc == UELYSYS_PAYUEL_CAM_SHOT_CC ||
                                         view.reflected_cc == UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC ||
                                         view.reflected_cc == UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC));
        if ((meta || download) && n == sizeof(view.u.image_meta)) {
            const auto &x = view.u.image_meta;
            const uint16_t chunks = (uint16_t)((x.chunk_count_be[0] << 8) | x.chunk_count_be[1]);
            const uint32_t crc = ((uint32_t)x.file_crc32_be[0] << 24) |
                                 ((uint32_t)x.file_crc32_be[1] << 16) |
                                 ((uint32_t)x.file_crc32_be[2] << 8) |
                                 x.file_crc32_be[3];
            RowFmt(is_obc ? "camera_id" : "image_slot", "0x%02X", x.object_id);
            RowFmt("image_index", "%" PRIu8, x.image_index);
            RowFmt("chunk_count", "%" PRIu16, chunks);
            RowFmt("last_chunk_size", "%" PRIu8, x.last_chunk_size);
            RowFmt("file_crc32", "0x%08X", crc);
        } else if (download && n == sizeof(view.u.download_request)) {
            RowFmt(is_obc ? "camera_id" : "image_slot", "0x%02X", view.u.download_request.object_id);
            RowFmt("image_index", "%" PRIu8, view.u.download_request.image_index);
            RowText("download_state", "Queue/start failure report");
        } else if (status && n == sizeof(view.u.payload_status)) {
            RowFmt("hardware_status", "0x%02X", view.u.payload_status.status);
        } else if ((meta || status || chunk) && n == sizeof(view.u.payload_error)) {
            const auto &x = view.u.payload_error;
            RowFmt("error_command_id", "0x%02X", x.command_id);
            RowFmt("error_status", "0x%02X", x.status);
            RowFmt("error_crc16", "0x%04X",
                   (uint16_t)((x.crc16_be[0] << 8) | x.crc16_be[1]));
            RowArr_Hex("payload_error",
                       reinterpret_cast<const uint8_t *>(&x), sizeof(x), 4);
        } else if (view.reflected_cc == UELYSYS_PAYUEL_CAM_REPORT_BCN_CC) show_hexdump = true;
            else if (view.reflected_cc ==UELYSYS_PAYUEL_OBC_DOWNLOAD_RANGE_CC){
                uint8_t x[6]; memcpy(&x, p, sizeof(x));
                RowFmt("Camera ID", "%" PRIu8, x[0]);
                RowFmt("ImageNumber", "%" PRIu8, x[1]);
                RowFmt("Start Chunk >> 8", "%" PRIu8, x[2]);
                RowFmt("Start Chunk & 0xFF", "%" PRIu8, x[3]);
                RowFmt("End Chunk >> 8", "%" PRIu8, x[4]);
                RowFmt("End Chunk & 0xFF", "%" PRIu8, x[5]);
            }
            else if (view.reflected_cc == UELYSYS_PAYUEL_OBC_MOTOR_MODE_CC && view.ret_val_size == sizeof(PAYUEL_OBC_MotorMode_Report_Payload_t)){
                RowFmt("Left Speed", "%" PRIi8, view.u.payuel_obc_motormode.LeftSpeed);
                RowFmt("Rigth Speed", "%" PRIi8, view.u.payuel_obc_motormode.RightSpeed);
                RowFmt("Duration", "%" PRIu8, view.u.payuel_obc_motormode.Duration);
                RowFmt("ResponseValid", "%" PRIu8, view.u.payuel_obc_motormode.ResponseValid);
                RowFmt("Error Detail [0]", "%" PRIu8, view.u.payuel_obc_motormode.ErrorDetail[0]);
                RowFmt("Error Detail [1]", "%" PRIu8, view.u.payuel_obc_motormode.ErrorDetail[1]);
                RowFmt("Error Detail [2]", "%" PRIu8, view.u.payuel_obc_motormode.ErrorDetail[2]);
                RowFmt("Error Detail [3]", "%" PRIu8, view.u.payuel_obc_motormode.ErrorDetail[3]);
                RowFmt("Current Milliamp [0]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[0]);
                RowFmt("Current Milliamp [1]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[1]);
                RowFmt("Current Milliamp [2]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[2]);
                RowFmt("Current Milliamp [3]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[3]);
                RowFmt("Current Milliamp [4]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[4]);
                RowFmt("Current Milliamp [5]", "%" PRIu16, view.u.payuel_obc_motormode.CurrentMilliamp[5]);
            }
            else {
            RowText("Payload", n ? "Invalid or command-specific response" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_UELYSYS_LGBAT)
    {
        auto ShowBlock = [&](const UELYSYS_LGBAT_Block_Report_t &b) {
            RowFmt("ID", "0x%02X", b.raw[0]);
            switch (b.raw[0]) {
                case 0x01: RowFmt("Pack_Voltage", "%" PRIu16, b.Data01.Pack_Voltage); RowFmt("Pack_Current", "%" PRId16, b.Data01.Pack_Current); RowFmt("Average_Time_To_Empty", "%" PRIu16, b.Data01.Average_Time_To_Empty); RowFmt("Average_Time_To_Full", "%" PRIu16, b.Data01.Average_Time_To_Full); RowFmt("CheckSum", "0x%02X", b.Data01.CheckSum); break;
                case 0x02: RowFmt("Power_Supply_Status", "%" PRIu8, b.Data02.Power_Supply_Status); RowFmt("SOH", "%" PRIu8, b.Data02.SOH); RowFmt("SOC", "%" PRIu16, b.Data02.SOC); RowFmt("RC_Remaining_Capacity", "%" PRIu16, b.Data02.RC_Remaining_Capacity); RowFmt("AE_Available_Energy", "%" PRIu16, b.Data02.AE_Available_Energy); RowFmt("CheckSum", "0x%02X", b.Data02.CheckSum); break;
                case 0x03: RowFmt("System_Max_Voltage", "%" PRIu16, b.Data03.System_Max_Voltage); RowFmt("Power_Supply_Health", "%" PRIu8, b.Data03.Power_Supply_Health); RowFmt("FETTestRequiredVoltage", "%" PRIu16, b.Data03.FETTestRequiredVoltage); RowArr_u8("Reserved", b.Data03.Battery_Information_Reserved, 3, 3); RowFmt("CheckSum", "0x%02X", b.Data03.CheckSum); break;
                case 0x04: RowFmt("Percentage", "%" PRIu16, b.Data04.Percentage); RowFmt("Design_Capacity", "%" PRIu16, b.Data04.Design_Capacity); RowFmt("Capacity", "%" PRIu16, b.Data04.Capacity); RowFmt("Charge", "%" PRIu16, b.Data04.Charge); RowFmt("CheckSum", "0x%02X", b.Data04.CheckSum); break;
                case 0x05: RowFmt("Cell_Voltage_Max", "%" PRIu16, b.Data05.Cell_Voltage_Max); RowFmt("Cell_Voltage_Min", "%" PRIu16, b.Data05.Cell_Voltage_Min); RowFmt("Cell_Temperature_Max", "%" PRId16, b.Data05.Cell_Temperature_Max); RowFmt("Cell_Temperature_Min", "%" PRId16, b.Data05.Cell_Temperature_Min); RowFmt("CheckSum", "0x%02X", b.Data05.CheckSum); break;
                case 0x06: RowFmt("Cell_Voltage_01", "%" PRIu16, b.Data06.Cell_Voltage_01); RowFmt("Cell_Voltage_02", "%" PRIu16, b.Data06.Cell_Voltage_02); RowFmt("Reserved_01", "%" PRIu16, b.Data06.Reserved_01); RowFmt("Reserved_02", "%" PRIu16, b.Data06.Reserved_02); RowFmt("CheckSum", "0x%02X", b.Data06.CheckSum); break;
                case 0x07: RowFmt("Cell_Temperature_02", "%" PRId16, b.Data07.Cell_Temperature_02); RowFmt("Cell_Temperature_01", "%" PRId16, b.Data07.Cell_Temperature_01); RowFmt("Balancing_R_Temperature", "%" PRId16, b.Data07.Balancing_R_Temperature); RowFmt("PreCharge_R_Temperature", "%" PRId16, b.Data07.PreCharge_R_Temperature); RowFmt("CheckSum", "0x%02X", b.Data07.CheckSum); break;
                case 0x08: RowFmt("FET_Down_Temperature", "%" PRId16, b.Data08.FET_Down_Temperature); RowFmt("FET_Up_Temperature", "%" PRId16, b.Data08.FET_Up_Temperature); RowFmt("CtrlCBStatus", "%" PRIu16, b.Data08.CtrlCBStatus); RowFmt("Temperature_002_Reserved", "%" PRIu8, b.Data08.Temperature_002_Reserved); RowFmt("SoftVersion", "%" PRIu8, b.Data08.SoftVersion); RowFmt("CheckSum", "0x%02X", b.Data08.CheckSum); break;
                case 0x09: RowFmt("BMS_Wakeup", "%" PRIu8, b.Data09.BMS_Wakeup); RowFmt("WakeupHoldStatus", "%" PRIu8, b.Data09.WakeupHoldStatus); RowFmt("VoltCurrDiag", "%" PRIu8, b.Data09.VoltCurrDiag); RowFmt("TempFailLevel", "%" PRIu8, b.Data09.TempFailLevel); RowFmt("FETStatus", "%" PRIu8, b.Data09.FETStatus); RowArr_u8("Reserved", b.Data09.Reserved, 3, 3); RowFmt("CheckSum", "0x%02X", b.Data09.CheckSum); break;
                case 0x0A: RowFmt("FailStatus2", "0x%02X", b.Data0A.FailStatus2); RowFmt("FailStatus3", "0x%02X", b.Data0A.FailStatus3); RowArr_u8("Reserved", b.Data0A.Reserved, 6, 6); RowFmt("CheckSum", "0x%02X", b.Data0A.CheckSum); break;
                case 0x0B: RowFmt("BOOST_Out_Voltage", "%" PRIu16, b.Data0B.BOOST_Out_Voltage); RowFmt("MCU_B_Plus_Volt", "%" PRIu16, b.Data0B.MCU_B_Plus_Volt); RowFmt("MCU_P_Plus_Volt", "%" PRIu16, b.Data0B.MCU_P_Plus_Volt); RowFmt("MCU_BMIC_REG_Out_Volt", "%" PRIu16, b.Data0B.MCU_BMIC_REG_Out_Volt); RowFmt("CheckSum", "0x%02X", b.Data0B.CheckSum); break;
                case 0x0C: RowFmt("BMS_Current", "%" PRId16, b.Data0C.BMS_Current); RowFmt("PCB_Temperature", "%" PRId16, b.Data0C.PCB_Temperature); RowFmt("BOOST_Temperature", "%" PRId16, b.Data0C.BOOST_Temperature); RowFmt("MCU_SBC_AMUX_Voltage", "%" PRIu16, b.Data0C.MCU_SBC_AMUX_Voltage); RowFmt("CheckSum", "0x%02X", b.Data0C.CheckSum); break;
            }
        };
        if (view.reflected_cc == UELYSYS_LGBAT_REQUEST_DATA_CC && n == 10) {
            ShowBlock(view.u.lgbat_block);
        } else if (view.reflected_cc == UELYSYS_LGBAT_REQUEST_ALL_DATA_CC &&
                   n == sizeof(UELYSYS_LGBAT_AllData_Report_t)) {
            const uint8_t *all_blocks =
                reinterpret_cast<const uint8_t *>(&view.u.lgbat_all);
            for (int i = 0; i < 12; ++i) {
                char section[32]; snprintf(section, sizeof(section), "[BMS Block 0x%02X]", i + 1); HeaderRow(section);
                UELYSYS_LGBAT_Block_Report_t b{};
                memcpy(b.raw, all_blocks + i * sizeof(b.raw), sizeof(b.raw));
                ShowBlock(b);
            }
        } else if (view.reflected_cc == UELYSYS_LGBAT_CRITICAL_ALERT_CC &&
                   n == sizeof(view.u.lgbat_critical)) {
            RowFmt("failure_level", "0x%02X", view.u.lgbat_critical.failure_level);
            RowFmt("voltage_diag", "0x%02X", view.u.lgbat_critical.voltage_diag);
            RowFmt("current_diag", "0x%02X", view.u.lgbat_critical.current_diag);
            RowFmt("temperature_diag", "0x%02X", view.u.lgbat_critical.temperature_diag);
            RowFmt("fail_status_2", "0x%02X", view.u.lgbat_critical.fail_status_2);
            RowFmt("fail_status_3", "0x%02X", view.u.lgbat_critical.fail_status_3);
        }
    }
    else if (view.kind == REPORT_KIND_UELYSYS_AOS)
    {
        if ((view.reflected_cc == UELYSYS_PAYUEL_AOS_NOOP_CC ||
             view.reflected_cc == UELYSYS_PAYUEL_AOS_RESET_COUNTERS_CC) &&
            n == sizeof(UELYSYS_AOS_Noop_Report_t)) {
            RowFmt("cmd_counter", "%" PRIu8, view.u.aos_noop.cmd_counter);
            RowFmt("err_counter", "%" PRIu8, view.u.aos_noop.err_counter);
        } else if (view.reflected_cc == UELYSYS_PAYUEL_AOS_READ_REGISTER_CC &&
            n == sizeof(UELYSYS_AOS_ReadRegister_Report_t)) {
            RowFmt("MSB", "0x%02X", view.u.aos_read_register.msb);
            RowFmt("LSB", "0x%02X", view.u.aos_read_register.lsb);
            RowFmt("raw big-endian", "%" PRId16, (int16_t)(((uint16_t)view.u.aos_read_register.msb << 8) | view.u.aos_read_register.lsb));
        } else if (view.reflected_cc == UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC &&
                   n == sizeof(UELYSYS_AOS_ReadAll_Report_t)) {
            for (int i = 0; i < 4; ++i) {
                char label[32];
                snprintf(label, sizeof(label), "resistance[%d]", i); RowFmt(label, "%f", view.u.aos_read_all.resistance[i]);
            }
        } else RowText("Payload", n ? "See full raw bytes below" : "No return payload");
    }
    else if (view.kind == REPORT_KIND_UELYSYS_PAYUEL_YONSEI)
    {
        if ((view.reflected_cc == UELYSYS_PAYUEL_YONSEI_NOOP_CC ||
             view.reflected_cc == UELYSYS_PAYUEL_YONSEI_RESET_COUNTERS_CC) &&
            n == sizeof(view.u.yonsei_status)) {
            RowFmt("command_counter", "%" PRIu8, view.u.yonsei_status.command_counter);
            RowFmt("command_error_counter", "%" PRIu8, view.u.yonsei_status.command_error_counter);
        } else if (view.reflected_cc == UELYSYS_PAYUEL_YONSEI_FORWARD_SB_CC &&
                   n == sizeof(view.u.yonsei_forward)) {
            RowFmt("target_mid", "0x%04X", view.u.yonsei_forward.target_mid);
            RowFmt("argument_length", "%" PRIu16, view.u.yonsei_forward.arg_length);
            RowFmt("target_cc", "%" PRIu8, view.u.yonsei_forward.target_fcn_code);
            RowFmt("target_node", "%" PRIu8, view.u.yonsei_forward.target_node);
            RowFmt("target_port", "%" PRIu8, view.u.yonsei_forward.target_port);
        } else RowText("Payload", n ? "See full raw bytes below" : "No return payload");
    }
    else if (view.kind == REPORT_KIND_UELYSYS_EO &&
             view.reflected_cc == UELYSYS_EO_SEND_STATE_CC &&
             n == sizeof(view.u.eo_state))
    {
        RowFmt("current_phase", "%" PRIu8, view.u.eo_state.current_phase);
        RowFmt("burn_channel_on", "%" PRIu8, view.u.eo_state.burn_channel_on);
        RowFmt("uant_release_verified", "%" PRIu8, view.u.eo_state.uant_release_verified);
        RowFmt("burn_channel_off", "%" PRIu8, view.u.eo_state.burn_channel_off);
        RowFmt("detumble_cmd_sent", "%" PRIu8, view.u.eo_state.detumble_cmd_sent);
        RowFmt("to_lab_output_enabled", "%" PRIu8, view.u.eo_state.to_lab_output_enabled);
        RowFmt("deployment_holdoff_complete", "%" PRIu8, view.u.eo_state.deployment_holdoff_complete);
        RowFmt("holdoff_elapsed_sec", "%" PRIu32, view.u.eo_state.holdoff_elapsed_accumulated_sec);
        RowFmt("state_version", "%" PRIu16, view.u.eo_state.state_version);
        RowFmt("crc", "0x%08" PRIX32, view.u.eo_state.crc);
    }
    else if (view.kind == REPORT_KIND_UELYSYS_SCH_LAB &&
             (view.reflected_cc == UELYSYS_SCH_LAB_SET_ENTRY_STATE_CC ||
              view.reflected_cc == UELYSYS_SCH_LAB_SET_ENTRY_CONFIG_CC) &&
             n == sizeof(view.u.sch_entry))
    {
        RowFmt("entry_index", "%" PRIu16, view.u.sch_entry.entry_index);
        RowFmt("enabled", "%" PRIu16, view.u.sch_entry.enabled);
        RowFmt("message_id", "0x%08" PRIX32, view.u.sch_entry.message_id);
        RowFmt("packet_rate", "%" PRIu32, view.u.sch_entry.packet_rate);
        RowFmt("phase_ticks", "%" PRIu32, view.u.sch_entry.phase_ticks);
        RowFmt("not_before_tick", "%" PRIu32, view.u.sch_entry.not_before_tick);
    }
    else if (view.kind == REPORT_KIND_UELYSYS_SP &&
             (view.reflected_cc == UELYSYS_SP_GET_AR6_STATUS_CC ||
              view.reflected_cc == UELYSYS_SP_RESET_AR6_COUNTERS_CC) &&
             n == sizeof(view.u.sp_readback))
    {
        RowFmt("ar6_addr", "%" PRIu8, view.u.sp_readback.ar6_addr);
        RowFmt("reachable", "%" PRIu8, view.u.sp_readback.reachable);
        RowFmt("burn_state", "%" PRIu8, view.u.sp_readback.burn_state);
        RowFmt("release_status", "%" PRIu8, view.u.sp_readback.release_status);
        RowFmt("burn_time_left", "%" PRIu8, view.u.sp_readback.burn_time_left);
        RowFmt("burn_tries", "%" PRIu8, view.u.sp_readback.burn_tries);
        RowFmt("temperature", "%" PRId16, view.u.sp_readback.temperature);
        RowFmt("seconds_since_boot", "%" PRIu32, view.u.sp_readback.seconds_since_boot);
        RowFmt("reboot_count", "%" PRIu8, view.u.sp_readback.reboot_count);
    }
    else if (view.kind == REPORT_KIND_UELYSYS_GPS)
    {
        if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_GET_HANDLER_HK_CC &&
            n == sizeof(UELYSYS_GPS_OemLogHandlerHk_Report_t)) {
            UELYSYS_GPS_OemLogHandlerHk_Report_t x{}; memcpy(&x, p, sizeof(x));
            char name[sizeof(x.name) + 1]{}; memcpy(name, x.name, sizeof(x.name));
            RowFmt("message_id", "%" PRIu16, x.message_id);
            RowFmt("message_length", "%" PRIu16, x.message_length);
            RowFmt("recent_msg_timestamp", "%" PRIu32, x.recent_msg_timestamp);
            RowFmt("callbacks", "%" PRId32, x.callbacks);
            RowFmt("log_count", "%" PRIu32, x.log_count);
            RowFmt("log_err_count", "%" PRIu32, x.log_err_count);
            RowFmt("error_cause", "%" PRId32, x.error_cause);
            RowFmt("status", "%" PRIu8, x.status);
            RowFmt("ignore_checksum", "%" PRIu8, x.ignore_checksum);
            RowText("name", name);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_GET_STAT_CC &&
                   n == sizeof(UELYSYS_GPS_OemLogStat_Report_t)) {
            UELYSYS_GPS_OemLogStat_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("log_count", "%" PRIu32, x.log_count);
            RowFmt("log_err_count", "%" PRIu32, x.log_err_count);
            RowFmt("recent_msg_timestamp", "%" PRIu32, x.recent_msg_timestamp);
            RowFmt("error_cause", "%" PRId32, x.error_cause);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_HANDLER_GET_STATUS_CC && n == 1) {
            RowFmt("status", "%" PRIu8, p[0]);
        } else if ((view.reflected_cc == UELYSYS_GPS_OEM_LOG_HANDLER_REGISTER_CC ||
                    view.reflected_cc == UELYSYS_GPS_OEM_LOG_GET_HANDLER_NAME_CC) &&
                   n == sizeof(UELYSYS_GPS_OemLogHandlerName_Report_t)) {
            char name[sizeof(UELYSYS_GPS_OemLogHandlerName_Report_t) + 1]{};
            memcpy(name, p, sizeof(UELYSYS_GPS_OemLogHandlerName_Report_t));
            RowText("name", name);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_GET_MESSAGE_LENGTH_CC && n == 2) {
            uint16_t value = 0; memcpy(&value, p, sizeof(value));
            RowFmt("message_length", "%" PRIu16, value);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_ADD_CALLBACK_CC && n == 64) {
            char err[65]{}; memcpy(err, p, 64);
            RowText("load_error", err);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_CMD_PUBLISH_CC && n == 2) {
            uint16_t body_len = 0; memcpy(&body_len, p, sizeof(body_len));
            RowFmt("rejected_bodylength", "%" PRIu16, body_len);
        } else if (view.reflected_cc == UELYSYS_GPS_OEM_LOG_GET_RECENT_MESSAGE_CC) {
            RowFmt("message_bytes", "%" PRIu16, n);
            RowArr_Hex("Message", p, n, 16);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else
        return false;
    return true;
}
