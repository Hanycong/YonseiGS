// View - Beacon 창 "Report" 탭: PAYUEL ROMA / LGPM 리포트
//
// GS_nuri5 miman_imgui.cpp Report 탭의 PAYUEL_ROMA_* / PAYUEL_LGPM_* case 를 옮긴 것 (행 이름/형식 그대로).
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawRomaLgpmReport(ReportCtx & c)
{
    ReportView_t & view = c.view;
    const EPS_Report_State_t & eps_state = c.eps_state;
    const EPS_RParam_Reassembly_State_t & eps_rparam = c.eps_rparam;
    bool & show_hexdump = c.show_hexdump;
    (void)eps_state; (void)eps_rparam; (void)show_hexdump;
    auto RowEPSHKAllSummary = [&]() { rpt5::RowEPSHKAllSummary(eps_state); };
    (void)RowEPSHKAllSummary;

    switch (view.kind)
    {
    /***************************************5차 추가************************************* */
    // 1. PAYUEL_ROMA Report
    case REPORT_KIND_PAYUEL_ROMA_NOOP: // CC 0, 1
    {
        auto &pl = view.u.roma_noop;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA NOOP (CC 0)");

        HeaderRow("[Counters Information]");
        RowFmt("Command Counter", "%" PRIu8, pl.CmdCounter);
        RowFmt("Error Counter",   "%" PRIu8, pl.ErrCounter);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_RESETCOUNTERS :
    {
        auto &pl = view.u.roma_resetcounters;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA RESET COUNTERS (CC 1)");

        HeaderRow("[Counters Information]");
        RowFmt("Command Counter", "%" PRIu8, pl.CmdCounter);
        RowFmt("Error Counter",   "%" PRIu8, pl.ErrCounter);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_COMMTEST: // CC 2
    {
        auto &pl = view.u.roma_commtest;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA COMM TEST (CC 2)");

        HeaderRow("[Comm Test Result]");
        RowArr_Hex("RxBuf [16 Bytes]", pl.RxBuf, 16, 8);

        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_GETSPECIFICLINE: // CC 6, 8
    {
        auto &pl = view.u.roma_getspecificline;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA GET SPECIFIC LINE (CC 6)");

        HeaderRow("[Line Information]");
        RowFmt("Line Number", "%" PRIu16, pl.line_no);
        RowFmt("Info",        "%" PRIu8,  pl.info);
        RowFmt("Length (len)","%" PRIu8,  pl.len);

        HeaderRow("[Time & Increment]");
        RowFmt("Time (Seconds)", "%" PRIu32, pl.time_s);
        RowFmt("Time (ms)",      "%" PRIu16, pl.time_ms);
        RowFmt("Increment",      "%" PRIu16, pl.incr);

        HeaderRow("[Message]");
        RowFmt("Line Data", "%.*s", (int)sizeof(pl.line), pl.line);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_GETMULTIPLELINES: // CC 7, 9
    {
        auto &pl = view.u.roma_getmultiplelines;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA GET MULTIPLE LINES (CC 7)");

        HeaderRow("[Log Accumulator Data]");
        RowFmt("Buffer Data", "%.*s", (int)sizeof(pl.buffer), (const char*)pl.buffer);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_GETLATESTLINE: // CC 6, 8
    {
        auto &pl = view.u.roma_getlatestline;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA GET LATEST LINE (CC 8)");

        HeaderRow("[Line Information]");
        RowFmt("Line Number", "%" PRIu16, pl.line_no);
        RowFmt("Info",        "%" PRIu8,  pl.info);
        RowFmt("Length (len)","%" PRIu8,  pl.len);

        HeaderRow("[Time & Increment]");
        RowFmt("Time (Seconds)", "%" PRIu32, pl.time_s);
        RowFmt("Time (ms)",      "%" PRIu16, pl.time_ms);
        RowFmt("Increment",      "%" PRIu16, pl.incr);

        HeaderRow("[Message]");
        RowFmt("Line Data", "%.*s", (int)sizeof(pl.line), pl.line);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_GETLATESTNLINES: // CC 7, 9
    {
        auto &pl = view.u.roma_getlatestNlines;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA GET MULTIPLE LINES (CC 9)");

        HeaderRow("[Log Accumulator Data]");
        RowFmt("Buffer Data", "%.*s", (int)sizeof(pl.buffer), (const char*)pl.buffer);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SETROUTEDEFAULT:
    {
        auto &pl = view.u.roma_setroutedefault;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_RESETROUTE :
    {
        auto &pl = view.u.roma_resetroute;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_LOADROUTE :
    {
        auto &pl = view.u.roma_loadroute;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SAVEROUTE :
    {
        auto &pl = view.u.roma_saveroute;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA SAVE ROUTE (CC 19)");

        HeaderRow("[Route Information]");
        RowFmt("Route Data", "%.*s", (int)sizeof(pl.route), pl.route);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SENDROUTE :
    {
        auto &pl = view.u.roma_sendroute;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SETROUTE :
    {
        auto &pl = view.u.roma_setroute;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARGET :
    {
        auto &pl = view.u.roma_parget;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("PAYUEL ROMA PAR GET (CC 22)");

        HeaderRow("[Parameter Information]");
        RowFmt("Value", "%" PRId32, pl.value);
        RowText("Requested parameter",
                view.roma_parget_index == 9 ? "TX frequency kHz (table 0, index 9)" :
                view.roma_parget_index == 7 ? "TX power dBm (table 0, index 7)" :
                "unknown PAR_GET preset");
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARSET :
    {
        auto &pl = view.u.roma_parset;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARDEFAULTS :
    {
        auto &pl = view.u.roma_pardefaults;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARSAVE :
    {
        auto &pl = view.u.roma_parsave;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARRESTORE :
    {
        auto &pl = view.u.roma_parrestore;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARLOAD :
    {
        auto &pl = view.u.roma_parload;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_PARSETOOB :
    {
        auto &pl = view.u.roma_parsetOOB;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SENDCOMMAND :
    {
        auto &pl = view.u.roma_sendcommand;

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("PAYUEL ROMA COMMAND RESULT (CC %d)", view.reflected_cc);

        HeaderRow("[Result Information]");
        RowFmt("Result", "%" PRIu8, pl.result);
        break;
    }

    case REPORT_KIND_PAYUEL_ROMA_SENDMSG:
        RowText("Type", "PAYUEL ROMA SEND_MSG ACK");
        RowText("Payload", view.payload_valid ? "No return payload" : "Unexpected payload");
        break;

    case REPORT_KIND_PAYUEL_ROMA_PAYINIT:
        RowText("Type", "PAYUEL ROMA PAY_INIT ACK");
        RowText("Payload", view.payload_valid ? "No return payload; 25 sec wait required" : "Unexpected payload");
        break;

    case REPORT_KIND_PAYUEL_ROMA_ISL_TEST_COUNT:
        RowText("Type", "PAYUEL ROMA ISL TEST COUNT");
        RowFmt("Count", "%" PRIu32, view.u.roma_isl_count.count);
        break;

    case REPORT_KIND_PAYUEL_ROMA_GENERIC:
        RowText("Type", "PAYUEL ROMA STATUS / VARIABLE RESPONSE");
        RowFmt("Command Code", "%" PRIu8, view.reflected_cc);
        RowFmt("Payload Size", "%" PRIu16, view.raw_size);
        if (view.raw_size > 0)
            RowArr_Hex("Payload", view.raw_bytes, view.raw_size, 16);
        break;

    case REPORT_KIND_PAYUEL_LGPM_NOOP: // CC 0
    {
        auto &pl = view.u.lgpm_noop;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM NOOP (CC 0)");

        HeaderRow("[Counters Information]");
        RowFmt("Command Counter", "%" PRIu8, pl.CmdCounter);
        RowFmt("Error Counter",   "%" PRIu8, pl.ErrCounter);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RESETCOUNTERS: // CC 1
    {
        auto &pl = view.u.lgpm_resetcounters;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RESET COUNTERS (CC 1)");

        HeaderRow("[Counters Information]");
        RowFmt("Command Counter", "%" PRIu8, pl.CmdCounter);
        RowFmt("Error Counter",   "%" PRIu8, pl.ErrCounter);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_MCU_ALIVE: // CC 2
    {
        auto &pl = view.u.lgpm_mcualivecheck;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM MCU ALIVE CHECK (CC 2)");

        HeaderRow("[Payload Info]");
        RowFmt("MCU_ALIVE_CHECK", "%" PRIu8, pl.MCU_ALIVE_CHECK);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_3V3_PWR_ON: // CC 3
    {
        auto &pl = view.u.lgpm_3v3pwron;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM 3V3 PWR ON (CC 3)");

        HeaderRow("[Payload Info]");
        RowFmt("PWR_ON_3V3", "%" PRIu8, pl.PWR_ON_3V3);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_3V3_PWR_OFF: // CC 4
    {
        auto &pl = view.u.lgpm_3v3pwroff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM 3V3 PWR OFF (CC 4)");

        HeaderRow("[Payload Info]");
        RowFmt("PWR_OFF_3V3", "%" PRIu8, pl.PWR_OFF_3V3);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_MAIN_BOOST_SW_ON: // CC 5
    {
        auto &pl = view.u.lgpm_mainboostswon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM MAIN BOOST SW ON (CC 5)");

        HeaderRow("[Payload Info]");
        RowFmt("MAIN_BOOST_SW_ON", "%" PRIu8, pl.MAIN_BOOST_SW_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_MAIN_BOOST_SW_OFF: // CC 6
    {
        auto &pl = view.u.lgpm_mainboostswoff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM MAIN BOOST SW OFF (CC 6)");

        HeaderRow("[Payload Info]");
        RowFmt("MAIN_BOOST_SW_OFF", "%" PRIu8, pl.MAIN_BOOST_SW_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_SUB_BOOST_SW_ON: // CC 7
    {
        auto &pl = view.u.lgpm_subboostswon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM SUB BOOST SW ON (CC 7)");

        HeaderRow("[Payload Info]");
        RowFmt("SUB_BOOST_SW_ON", "%" PRIu8, pl.SUB_BOOST_SW_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_SUB_BOOST_SW_OFF: // CC 8
    {
        auto &pl = view.u.lgpm_subboostswoff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM SUB BOOST SW OFF (CC 8)");

        HeaderRow("[Payload Info]");
        RowFmt("SUB_BOOST_SW_OFF", "%" PRIu8, pl.SUB_BOOST_SW_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V28_MAIN_ON: // CC 9
    {
        auto &pl = view.u.lgpm_v28mainon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V28 MAIN ON (CC 9)");

        HeaderRow("[Payload Info]");
        RowFmt("V28_MAIN_ON", "%" PRIu8, pl.V28_MAIN_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V28_MAIN_OFF: // CC 10
    {
        auto &pl = view.u.lgpm_v28mainoff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V28 MAIN OFF (CC 10)");

        HeaderRow("[Payload Info]");
        RowFmt("V28_MAIN_OFF", "%" PRIu8, pl.V28_MAIN_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V28_SUB_ON: // CC 11
    {
        auto &pl = view.u.lgpm_v28subon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V28 SUB ON (CC 11)");

        HeaderRow("[Payload Info]");
        RowFmt("V28_SUB_ON", "%" PRIu8, pl.V28_SUB_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V28_SUB_OFF: // CC 12
    {
        auto &pl = view.u.lgpm_v28suboff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V28 SUB OFF (CC 12)");

        HeaderRow("[Payload Info]");
        RowFmt("V28_SUB_OFF", "%" PRIu8, pl.V28_SUB_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V12_MAIN_ON: // CC 13
    {
        auto &pl = view.u.lgpm_v12mainon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V12 MAIN ON (CC 13)");

        HeaderRow("[Payload Info]");
        RowFmt("V12_MAIN_ON", "%" PRIu8, pl.V12_MAIN_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_V12_MAIN_OFF: // CC 14
    {
        auto &pl = view.u.lgpm_v12mainoff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM V12 MAIN OFF (CC 14)");

        HeaderRow("[Payload Info]");
        RowFmt("V12_MAIN_OFF", "%" PRIu8, pl.V12_MAIN_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_PWR_SENSE_INFO: // CC 15
    {
        auto &pl = view.u.lgpm_pwrsenseinfo;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM PWR SENSE INFO (CC 15)");

        HeaderRow("[Payload Info]");
        RowFmt("PWR_SENSE_INFO", "%" PRIu8, pl.PWR_SENSE_INFO);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_PWR_SEQ_ON: // CC 16
    {
        auto &pl = view.u.lgpm_pwrseqon;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM PWR SEQ ON (CC 16)");

        HeaderRow("[Payload Info]");
        RowFmt("PWR_SEQ_ON", "%" PRIu8, pl.PWR_SEQ_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_PWR_SEQ_OFF: // CC 17
    {
        auto &pl = view.u.lgpm_pwrseqoff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM PWR SEQ OFF (CC 17)");

        HeaderRow("[Payload Info]");
        RowFmt("PWR_SEQ_OFF", "%" PRIu8, pl.PWR_SEQ_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX1: // CC 18
    {
        auto &pl = view.u.lgpm_rwacontrol_idx1;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA CONTROL idx1 (CC 18)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_CONTROL", "%" PRIu8, pl.RWA_CONTROL);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX2: // CC 19
    {
        auto &pl = view.u.lgpm_rwacontrol_idx2;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA CONTROL idx2 (CC 19)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_CONTROL", "%" PRIu8, pl.RWA_CONTROL);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_CONTROL_IDX3: // CC 20
    {
        auto &pl = view.u.lgpm_rwacontrol_idx3;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA CONTROL idx3 (CC 20)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_CONTROL", "%" PRIu8, pl.RWA_CONTROL);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_PWR_ON: // CC 21
    {
        auto &pl = view.u.lgpm_rwapwron;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA PWR ON (CC 21)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_PWR_ON", "%" PRIu8, pl.RWA_PWR_ON);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_PWR_OFF: // CC 22
    {
        auto &pl = view.u.lgpm_rwapwroff;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA PWR OFF (CC 22)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_PWR_OFF", "%" PRIu8, pl.RWA_PWR_OFF);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    case REPORT_KIND_PAYUEL_LGPM_RWA_SENSE_INFO: // CC 23
    {
        auto &pl = view.u.lgpm_rwasenseinfo;
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Type");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted("LGPM RWA SENSE INFO (CC 23)");

        HeaderRow("[Payload Info]");
        RowFmt("RWA_SENSE_INFO", "%" PRIu8, pl.RWA_SENSE_INFO);
        RowFmt("Execution Timestamp", "%" PRIu32, pl.Execution_Timestamp);
        RowFmt("Reply Message", "%.*s", (int)sizeof(pl.Reply_Message), pl.Reply_Message);
        RowFmt("CRC16", "0x%04X", pl.CRC16);
        break;
    }

    default:
        return false;
    }
    return true;
}
