// View - Beacon 창 "Report" 탭: SD1-SAT 앱 리포트의 ReturnValue 해석
//
// GS_nuri5 miman_imgui.cpp Report 탭의 SD1SAT 앱 리포트 (CR/EPS/SNBP/UTRX/PAYSD1_LOTHAN, 나머지 앱은 Data 바이트) 를 옮긴 것 (행 이름/형식 그대로).
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

bool DrawSd1satAppBody(ReportCtx & c)
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

        if (view.kind == REPORT_KIND_SD1SAT_CR)
        {
            if (view.reflected_cc == SD1SAT_CR_DUMP_QUEUE_CC){
                if (n == sizeof(SD1SAT_CR_DumpQueue_tlm_Payload_t)) {
                    SD1SAT_CR_DumpQueue_tlm_Payload_t x{};
                    memcpy(&x, p, sizeof(x));
                    RowFmt("OsError", "%" PRIi32, x.OsError);
                } else {
                    RowText("Data", "No return payload");
                }
            }
            else if (view.reflected_cc == SD1SAT_CR_SET_DOWNLINK_MODE_CC){
                SD1SAT_CR_SetDownlinkMode_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Mode", "%" PRIu8, x.Mode);
            }
            else if (view.reflected_cc == SD1SAT_CR_GET_FULL_STATES_CC){
                SD1SAT_CR_GetFullReportState_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Cmd Counter", "%" PRIu32, x.CommandCounter);
                RowFmt("Err Counter", "%" PRIu32, x.CommandErrorCounter);
                RowFmt("Report Received", "%" PRIu32, x.ReportsReceived);
                RowFmt("Report Success", "%" PRIu32, x.ReportsSuccess);
                RowFmt("Report Error", "%" PRIu32, x.ReportsError);
                RowFmt("Report Malformed", "%" PRIu32, x.ReportsMalformed);
                RowFmt("Queue Overwrites", "%" PRIu32, x.QueueOverwrites);
                RowFmt("Retransmit Count", "%" PRIu32, x.RetransmitCount);
                RowFmt("Retransmit Not Found", "%" PRIu32, x.RetransmitNotFound);
                RowFmt("Downlink Sent", "%" PRIu32, x.DownlinkSent);
                RowFmt("Downlink Errors", "%" PRIu32, x.DownlinkErrors);
                RowFmt("Dump Count", "%" PRIu32, x.DumpCount);
                RowFmt("Dump Errors", "%" PRIu32, x.DumpErrors);
                RowFmt("Downlink Mode", "%" PRIu8, x.DownlinkMode);
            }
            else if (n == 0) {
            RowText("Data", "No return data (RetVal alone carries the outcome)");
        } else {
            RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
        }
        }

        else if (view.kind == REPORT_KIND_SD1SAT_EPS)
        {
            if (view.reflected_cc == SD1_EPS_PING_CC){
                SD1_EPS_PING_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("ReturnMs", "%" PRIu8, x.ReturnMs);
            }
            if (view.reflected_cc == SD1_EPS_IDENT_CC){
                SD1_EPS_IDENT_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("hostname", "%.*s", (int)sizeof(x.hostname), x.hostname);
                RowFmt("model", "%.*s", (int)sizeof(x.model), x.model);
                RowFmt("revision", "%.*s", (int)sizeof(x.revision), x.revision);
                RowFmt("date", "%.*s", (int)sizeof(x.date), x.date);
                RowFmt("time", "%.*s", (int)sizeof(x.time), x.time);
            }
            if (view.reflected_cc == SD1_EPS_MEMFREE_CC){
                SD1_EPS_MEMFREE_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("FreeBytes", "%" PRIu32, x.FreeBytes);
            }
            if (view.reflected_cc == SD1_EPS_BUFFREE_CC){
                SD1_EPS_BUFFREE_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("FreeBuffers", "%" PRIu32, x.FreeBuffers);
            }
            if (view.reflected_cc == SD1_EPS_CHANNEL_GET_CC) {
                SD1_EPS_CHANNEL_GET_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowText("IsOn", x.IsOn ? "True" : "False");
            }
            if (view.reflected_cc == SD1_EPS_CHANNELS_GET_CC) {
                SD1_EPS_CHANNELS_GET_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowText("Channel State 0", x.ChannelStates[0] ? "True" : "False");
                RowText("Channel State 1", x.ChannelStates[1] ? "True" : "False");
                RowText("Channel State 2", x.ChannelStates[2] ? "True" : "False");
                RowText("Channel State 3", x.ChannelStates[3] ? "True" : "False");
                RowText("Channel State 4", x.ChannelStates[4] ? "True" : "False");
                RowText("Channel State 5", x.ChannelStates[5] ? "True" : "False");
                RowText("Channel State 6", x.ChannelStates[6] ? "True" : "False");
                RowText("Channel State 7", x.ChannelStates[7] ? "True" : "False");
                RowText("Channel State 8", x.ChannelStates[8] ? "True" : "False");
                RowText("Channel State 9", x.ChannelStates[9] ? "True" : "False");
                RowText("Channel State 10", x.ChannelStates[10] ? "True" : "False");
                RowText("Channel State 11", x.ChannelStates[11] ? "True" : "False");
                RowText("Channel State 12", x.ChannelStates[12] ? "True" : "False");
            }
            if (view.reflected_cc == SD1_EPS_TABLE_ACQUIRE_CC) {
                SD1_EPS_TABLE_ACQUIRE_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Report Size", "%" PRIu16, x.ReportSize);
            }
            if (view.reflected_cc == SD1_EPS_WDT_GET_CC) {
                SD1_EPS_WDT_GET_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Wdt Gnd Left", "%" PRIu32, x.WdtGndLeft);
                RowFmt("Wdt Can Left", "%" PRIu32, x.WdtCanLeft);
                RowFmt("Cnt Wdt Gnd", "%" PRIu32, x.CntWdtGnd);
                RowFmt("Cnt Wdt Can", "%" PRIu32, x.CntWdtCan);
            }
            if (view.reflected_cc == SD1_EPS_TABLE_TO_FILE_CC) {
                SD1_EPS_TABLE_TO_FILE_tlm_Payload_t x{};
                if (view.ret_type == 0) {
                    SD1_EPS_TABLE_TO_FILE_tlm_Payload_t x{};
                    memcpy(&x, p, sizeof(x));
                    RowFmt("Image Size", "%" PRIu32, (uint32_t)x.ImageSize);
                }
                else if (view.ret_type == 3){
                    char ErrMsg[20] = {0};
                    // GS_nuri5 는 raw_size 만큼 복사해 20바이트를 넘으면 스택을 덮어썼다
                    memcpy(ErrMsg, p, view.raw_size < sizeof(ErrMsg) - 1 ? view.raw_size : sizeof(ErrMsg) - 1);
                    RowFmt("Err Msg", "%s", ErrMsg);
                }
                else RowText("Data", "No return data (RetVal alone carries the outcome)");
            }
            if (view.reflected_cc == SD1_EPS_HK_SET_MODE_CC){
                SD1_EPS_HkSetMode_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Mode", "%" PRIu8, x.mode);
            }
            else if (view.reflected_cc == SD1_EPS_HK_GET_MODE_CC){
                SD1_EPS_HkGetMode_tlm_Payload_t x{};
                memcpy(&x, p, sizeof(x));
                RowFmt("Mode", "%" PRIu8, x.mode);
            }
            else if (n == 0) {
                RowText("Data", "No return data (RetVal alone carries the outcome)");
            }
            else {
                RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
            }
        }

        else if (view.kind == REPORT_KIND_SD1SAT_SNBP)
        {
            if (view.reflected_cc == SD1SAT_SNBP_RAMM_GET_DPSD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_RAMM_GET_ADSD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_RAMM_GET_HKSD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_RAMM_GET_BESD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_REMM_GET_POSD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_REMM_GET_BESD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (view.reflected_cc == SD1SAT_SNBP_REMM_GET_ADSD_CC) {
                SD1SAT_SNBP_Reading_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("Seconds", "%" PRIu32, x.Seconds);
                RowFmt("Length", "%" PRIu8, x.Length);
                RowFmt("Text", "%.*s", (int)(x.Length < sizeof(x.Text) ? x.Length : sizeof(x.Text)), x.Text);
            }
            else if (n == 0) {
            RowText("Data", "No return data (RetVal alone carries the outcome)");
        } else {
            RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
        }
        }

        else if (view.kind == REPORT_KIND_SD1SAT_UTRX)
        {
            if (view.reflected_cc == SD1SAT_UTRX_AX100_PING_CC) {
                SD1SAT_UTRX_PingPayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("return_ms", "%" PRIu32, x.return_ms);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_GNDWDT_GET_CC) {
                SD1SAT_UTRX_GndWdtGetPayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("gnd_wdt_ms", "%" PRIu32, x.gnd_wdt_ms);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_GET_DIAG_CC) {
                SD1SAT_UTRX_Diagnosis_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                HeaderRow("[Counters Information]");
                RowFmt("TxCount", "%" PRIu32, x.TxCount);
                RowFmt("RxCount", "%" PRIu32, x.RxCount);
                RowFmt("TxBytes", "%" PRIu32, x.TxBytes);
                RowFmt("RxBytes", "%" PRIu32, x.RxBytes);
                RowFmt("TotalTxBytes", "%" PRIu32, x.TotalTxBytes);
                RowFmt("TotalRxBytes", "%" PRIu32, x.TotalRxBytes);
                RowFmt("TotalTxCount", "%" PRIu32, x.TotalTxCount);
                RowFmt("TotalRxCount", "%" PRIu32, x.TotalRxCount);

                HeaderRow("[Payload Info]");
                RowFmt("BootCause", "0x%08" PRIX32, x.BootCause);
                RowFmt("BootCount", "%" PRIu16, x.BootCount);
                RowFmt("LastRSSI", "%" PRId16, x.LastRSSI);
                RowFmt("ActiveConf", "%" PRIu8, x.ActiveConf);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_TABLE_GET_CC) {
                SD1SAT_UTRX_TableGetPayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("data_length", "%" PRIu16, view.raw_size);
                if (view.raw_size > 0) RowArr_Hex("Data", x.buf, (int)(view.raw_size < sizeof(x.buf) ? view.raw_size : sizeof(x.buf)), 16);   // buf 크기까지
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_IDENT_CC) {
                SD1SAT_UTRX_ident_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("hostname", "%.*s", (int)sizeof(x.hostname), x.hostname);
                RowFmt("model", "%.*s", (int)sizeof(x.model), x.model);
                RowFmt("revision", "%.*s", (int)sizeof(x.revision), x.revision);
                RowFmt("date", "%.*s", (int)sizeof(x.date), x.date);
                RowFmt("time", "%.*s", (int)sizeof(x.time), x.time);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_MEMFREE_CC) {
                SD1SAT_UTRX_MemFreePayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("bytes", "%" PRIu32, x.bytes);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_BUFFREE_CC) {
                SD1SAT_UTRX_BufFreePayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("count", "%" PRIu32, x.count);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_GET_CC) {
                SD1SAT_UTRX_GetPayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("data_length", "%" PRIu16, view.raw_size);
                if (view.raw_size > 0) RowArr_Hex("Data", x.buf, (int)(view.raw_size < sizeof(x.buf) ? view.raw_size : sizeof(x.buf)), 16);   // buf 크기까지
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_GET_RAW_CC) {
                SD1SAT_UTRX_GetRawPayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("data_length", "%" PRIu16, view.raw_size);
                if (view.raw_size > 0) RowArr_Hex("Data", x.buf, (int)(view.raw_size < sizeof(x.buf) ? view.raw_size : sizeof(x.buf)), 16);   // buf 크기까지
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_HK_SET_MODE_CC) {
                SD1SAT_UTRX_HkSetModePayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("mode", "%" PRIu8, x.mode);
            }
            else if (view.reflected_cc == SD1SAT_UTRX_AX100_HK_GET_MODE_CC) {
                SD1SAT_UTRX_HkGetModePayload_t x{};
                memcpy(&x, p, view.raw_size < sizeof(x) ? view.raw_size : sizeof(x));   // GS_nuri5: raw_size 만큼 (스택 덮어씀)
                RowFmt("mode", "%" PRIu8, x.mode);
            }
            else if (n == 0) {
            RowText("Data", "No return data (RetVal alone carries the outcome)");
        } else {
            RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
        }
        }

        else if (view.kind == REPORT_KIND_SD1SAT_PAYSD1_LOTHAN)
    {
        /* CC 23/27 carry a dedicated result payload beyond plain RetVal;
         * CAPTURE_START/CAPTURE_START_RELATIVE/CAPTURE_STOP (CC 24-26) send
         * no Data at all - see paysd1_lothan_dispatch.c CompleteGroundCommand(). */
        if (view.reflected_cc == SD1SAT_PAYSD1_LOTHAN_FunctionCode_COUNT_SPLIT_FILES &&
            n == sizeof(SD1SAT_PAYSD1_LOTHAN_SplitFileCountPayload_t)) {
            SD1SAT_PAYSD1_LOTHAN_SplitFileCountPayload_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("CaptureId", "%" PRIu16, x.CaptureId);
            RowFmt("FileCount", "%" PRIu32, x.FileCount);
        } else if (view.reflected_cc == SD1SAT_PAYSD1_LOTHAN_FunctionCode_OTA_PATCH_TEST &&
                   n == sizeof(SD1SAT_PAYSD1_LOTHAN_OtaPatchTestPayload_t)) {
            SD1SAT_PAYSD1_LOTHAN_OtaPatchTestPayload_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("Value", "%" PRIu8, x.Value);
        } else if (n == 0) {
            RowText("Data", "No return data (RetVal alone carries the outcome)");
        } else {
            RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
        }
    }

    else if (
             view.kind == REPORT_KIND_SD1SAT_GPS || view.kind == REPORT_KIND_SD1SAT_MEOW ||
             view.kind == REPORT_KIND_SD1SAT_OPS ||
             view.kind == REPORT_KIND_SD1SAT_PAYSD1_ROMA || view.kind == REPORT_KIND_SD1SAT_SNBP ||
             view.kind == REPORT_KIND_SD1SAT_TTC || view.kind == REPORT_KIND_SD1SAT_UTRX)
    {
        /* CmdMsgId/CmdCode/RetVal/RetType already shown above as Reflected Msg Id/
         * Reflected Command Code/Return Code/Return Type - only Data[] is app- and
         * command-specific, and no per-command struct has been added yet (see
         * 4-Rpt-Own in the pipeline guide for the recipe to add one). */
        if (n == 0) {
            RowText("Data", "No return data (RetVal alone carries the outcome)");
        } else {
            RowFmt("data_length", "%" PRIu16, n);
            RowArr_Hex("Data", p, n, 16);
        }
    }
    else
        return false;
    return true;
}
