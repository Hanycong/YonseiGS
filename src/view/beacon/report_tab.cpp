// View - Beacon 창 "Report" 탭
//
// beacon_view.cpp 의 ImGui_BeaconWindow_Body 에서 탭 블록을 내용 그대로 옮긴 것.
#include <inttypes.h>
#include <mutex>
#include <gs/param/rparam.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_coms.h"
#include "miman_radial.h"
#include "miman_csp.h"
#include "miman_theme.h"
#include "svc_tx.h"
#include "beacon_tabs.h"

using namespace mim;

extern Setup * setup;
extern StateCheckUnit State;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern Beacon * beacon;
extern MissionBeacon * missionbeacon;
extern GETFILEINFO * getfileinfo;
extern Event * event;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;
extern uint8_t remote_activeconf;
extern int16_t remote_lastrssi;
extern int16_t remote_rssibusy;
extern cmd_packet_t * shellcmd;
// miman_imgui.cpp 의 같은 이름 static 과 같은 값 (표 머리글 고정 행/열 수)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawReportTab(float fontscale)
{
        if (ImGui::BeginTabItem("Report"))
        {
            bool show_hexdump = false;
            ReportView_t view;
            {
                std::lock_guard<std::mutex> lk(g_report_view_mtx);
                view = g_report_view;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
            ImGui::Text("Total RX Bytes (Report) : %" PRIu32, get_rx_bytes());
            ImGui::PopStyleColor();

            ImGui::Separator();
            ImGui::SetWindowFontScale(0.9f * fontscale);

            ImGuiTableFlags rpt_flags =
                ImGuiTableFlags_SizingStretchSame |
                ImGuiTableFlags_ScrollY          |
                ImGuiTableFlags_RowBg            |
                ImGuiTableFlags_Borders          |
                ImGuiTableFlags_Resizable        |
                ImGuiTableFlags_Reorderable;

            if (ImGui::BeginTable("##ReportTable", 2, rpt_flags))
            {
                ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
                ImGui::TableSetupColumn("Data",      ImGuiTableColumnFlags_NoHide, 0.0f);
                ImGui::TableHeadersRow();

                auto ReportRow = [](const char* label, const char* fmt, auto value)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(label);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(fmt, value);
                };

                if (!view.valid)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Report");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("No data");
                }
                else
                {
                    ImGui::TableNextRow();

                    BeaconSectionHeader("[CCSDS]");

                    ReportRow("CCSDS_Message_ID", "0x%04X", (unsigned int)view.CCMessage_ID);
                    ReportRow("CCSDS_Count",      "0x%04X", (unsigned int)view.CCCount);
                    ReportRow("CCSDS_Length",     "0x%04X", (unsigned int)view.CCLength);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CCTime Code");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%02X %02X %02X %02X %02X %02X",
                                view.CCTime_code[0], view.CCTime_code[1],
                                view.CCTime_code[2], view.CCTime_code[3],
                                view.CCTime_code[4], view.CCTime_code[5]);

                    ImGui::TableNextRow();
                    BeaconSectionHeader("[Body]");

                    ReportRow("Reflected Msg Id",       "0x%04X", (unsigned int)view.reflected_msg_id);
                    ReportRow("Reflected Command Code",           "%u",     (unsigned int)view.reflected_cc);
                    ReportRow("Return Type",     "0x%02X", (unsigned int)view.ret_type);
                    ReportRow("Return Code",     "%d",     (int)view.ret_code);
                    ReportRow("Return Value Size", "%u",     (unsigned int)view.ret_val_size);

                    ImGui::TableNextRow();
                    BeaconSectionHeader("[Report Type]");

                    switch (view.kind)
                    {
                        case REPORT_KIND_EPS_P60_PDU_GET_TABLE_HK:
                        {
                            auto &pl = view.u.eps_p60pdugettablehk;

                            auto HeaderRow = [](const char* title)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
                                ImGui::TextUnformatted(title);
                                ImGui::PopStyleColor();
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted("");
                            };

                            auto RowFmt = [](const char* label, const char* fmt, auto v)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::Text(fmt, v);
                            };

                            auto RowArr_i16 = [](const char* label, const int16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIi16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u16 = [](const char* label, const uint16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u8 = [](const char* label, const uint8_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 4);
                                char tmp[16];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu8, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u32 = [](const char* label, const uint32_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 12);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu32, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted("Type");
                            ImGui::TableSetColumnIndex(1);
                            ImGui::TextUnformatted("EPS P60 PDU GET TABLE HK");

                            HeaderRow("[Report Data]");
                            RowArr_i16("c_out[0..8]",  pl.c_out, 9, 4);
                            RowArr_u16("v_out[0..8]",  pl.v_out, 9, 4);
                            RowArr_u8 ("out_en[0..8]", pl.out_en, 9, 4);
                            RowArr_u8 ("conv_en[0..2]", pl.conv_en, 3, 0);

                            HeaderRow("[Power]");
                            RowFmt("vcc",  "%" PRIu16, pl.vcc);
                            RowFmt("vbat", "%" PRIu16, pl.vbat);
                            RowFmt("temp", "%" PRIi16, pl.temp);

                            HeaderRow("[Boot/Time]");
                            RowFmt("bootcause", "%" PRIu32, pl.bootcause);
                            RowFmt("bootcnt",   "%" PRIu32, pl.bootcnt);
                            RowFmt("uptime",    "%" PRIu32, pl.uptime);
                            RowFmt("resetcause","0x%04X", (unsigned int)pl.resetcause);

                            HeaderRow("[Modes]");
                            RowFmt("batt_mode", "%" PRIu8, pl.batt_mode);

                            HeaderRow("[Latchups]");
                            RowArr_u16("latchup[0..8]", pl.latchup, 9, 4);

                            HeaderRow("[Device]");
                            RowArr_u8("device_type[0..7]",   pl.device_type, 8, 0);
                            RowArr_u8("device_status[0..7]", pl.device_status, 8, 0);

                            HeaderRow("[WDT Counters]");
                            RowFmt("wdt_cnt_gnd", "%" PRIu32, pl.wdt_cnt_gnd);
                            RowFmt("wdt_cnt_i2c", "%" PRIu32, pl.wdt_cnt_i2c);
                            RowFmt("wdt_cnt_can", "%" PRIu32, pl.wdt_cnt_can);
                            RowArr_u32("wdt_cnt_csp[0..1]", pl.wdt_cnt_csp, 2, 0);

                            HeaderRow("[WDT Left]");
                            RowFmt("wdt_gnd_left", "%" PRIu32, pl.wdt_gnd_left);
                            RowFmt("wdt_i2c_left", "%" PRIu32, pl.wdt_i2c_left);
                            RowFmt("wdt_can_left", "%" PRIu32, pl.wdt_can_left);
                            RowArr_u8("wdt_csp_left[0..1]", pl.wdt_csp_left, 2, 0);

                            break;
                        }

                        
                        case REPORT_KIND_EPS_P60_ACU_GET_TABLE_HK:
                        {
                            auto &pl = view.u.eps_p60acugettablehk;

                            auto HeaderRow = [](const char* title)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
                                ImGui::TextUnformatted(title);
                                ImGui::PopStyleColor();
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted("");
                            };

                            auto RowFmt = [](const char* label, const char* fmt, auto v)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::Text(fmt, v);
                            };

                            auto RowArr_i16 = [](const char* label, const int16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIi16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u16 = [](const char* label, const uint16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u8 = [](const char* label, const uint8_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 4);
                                char tmp[16];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu8, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u32 = [](const char* label, const uint32_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 12);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu32, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted("Type");
                            ImGui::TableSetColumnIndex(1);
                            ImGui::TextUnformatted("EPS P60 ACU GET TABLE HK");

                            HeaderRow("[Inputs]");
                            RowArr_i16("c_in[0..5]", pl.c_in, 6, 2);
                            RowArr_u16("v_in[0..5]", pl.v_in, 6, 2);

                            HeaderRow("[Bus/Temp]");
                            RowFmt("vbat", "%" PRIu16, pl.vbat);
                            RowFmt("vcc",  "%" PRIu16, pl.vcc);
                            RowArr_i16("temp[0..2]", pl.temp, 3, 0);

                            HeaderRow("[MPPT]");
                            RowFmt("mppt_mode", "%" PRIu8, pl.mppt_mode);
                            RowArr_u16("vboost[0..5]", pl.vboost, 6, 2);
                            RowArr_u16("power[0..5]",  pl.power,  6, 2);

                            HeaderRow("[DAC]");
                            RowArr_u8 ("dac_en[0..2]",  pl.dac_en,  3, 0);
                            RowArr_u16("dac_val[0..5]", pl.dac_val, 6, 2);

                            HeaderRow("[Boot/Time]");
                            RowFmt("bootcause", "%" PRIu32, pl.bootcause);
                            RowFmt("bootcnt",   "%" PRIu32, pl.bootcnt);
                            RowFmt("uptime",    "%" PRIu32, pl.uptime);
                            RowFmt("resetcause","0x%04X", (unsigned int)pl.resetcause);

                            HeaderRow("[MPPT Timing]");
                            RowFmt("mppt_time",   "%" PRIu16, pl.mppt_time);
                            RowFmt("mppt_period", "%" PRIu16, pl.mppt_period);

                            HeaderRow("[Device]");
                            RowArr_u8("device_type[0..7]",   pl.device_type, 8, 0);
                            RowArr_u8("device_status[0..7]", pl.device_status, 8, 0);

                            HeaderRow("[WDT]");
                            RowFmt("wdt_cnt_gnd",  "%" PRIu32, pl.wdt_cnt_gnd);
                            RowFmt("wdt_gnd_left", "%" PRIu32, pl.wdt_gnd_left);

                            break;
                        }





                        case REPORT_KIND_EPS_P60_DOCK_GET_TABLE_HK:
                        {
                            auto &pl = view.u.eps_p60dockgettablehk;

                            auto HeaderRow = [](const char* title)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
                                ImGui::TextUnformatted(title);
                                ImGui::PopStyleColor();
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted("");
                            };

                            auto RowFmt = [](const char* label, const char* fmt, auto v)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::Text(fmt, v);
                            };

                            auto RowText = [](const char* label, const char* text)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(text);
                            };

                            auto RowArr_i16 = [](const char* label, const int16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIi16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u16 = [](const char* label, const uint16_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 8);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu16, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u8 = [](const char* label, const uint8_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 4);
                                char tmp[16];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu8, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            auto RowArr_u32 = [](const char* label, const uint32_t* aa, int n, int break_after)
                            {
                                std::string s;
                                s.reserve(n * 12);
                                char tmp[32];

                                for (int i = 0; i < n; i++)
                                {
                                    snprintf(tmp, sizeof(tmp), "%" PRIu32, aa[i]);

                                    if (i != 0)
                                    {
                                        if (break_after > 0 && i == break_after + 1) s.push_back('\n');
                                        else s.push_back(' ');
                                    }
                                    s += tmp;
                                }

                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(label);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(s.c_str());
                            };

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted("Type");
                            ImGui::TableSetColumnIndex(1);
                            ImGui::TextUnformatted("EPS P60 DOCK GET TABLE HK");

                            HeaderRow("[Report Data]");
                            RowArr_i16("c_out[0..12]",  pl.c_out, 13, 6);
                            RowArr_u16("v_out[0..12]",  pl.v_out, 13, 6);
                            RowArr_u8 ("out_en[0..12]", pl.out_en, 13, 6);

                            HeaderRow("[Temps]");
                            RowArr_i16("temp[0..1]", pl.temp, 2, 0);

                            HeaderRow("[Boot/Time]");
                            RowFmt("bootcause", "%" PRIu32, pl.bootcause);
                            RowFmt("bootcnt",   "%" PRIu32, pl.bootcnt);
                            RowFmt("uptime",    "%" PRIu32, pl.uptime);
                            RowFmt("resetcause","0x%04X", (unsigned int)pl.resetcause);

                            HeaderRow("[Modes/Flags]");
                            RowFmt("batt_mode",  "%" PRIu8, pl.batt_mode);
                            RowFmt("heater_on",  "%" PRIu8, pl.heater_on);
                            RowFmt("conv_5v_en", "%" PRIu8, pl.conv_5v_en);

                            HeaderRow("[Latchups]");
                            RowArr_u16("latchup[0..12]", pl.latchup, 13, 6);

                            HeaderRow("[Battery/Power]");
                            RowFmt("vbat_v", "%" PRIu16, pl.vbat_v);
                            RowFmt("vcc_c",  "%" PRIi16, pl.vcc_c);
                            RowFmt("batt_c", "%" PRIi16, pl.batt_c);
                            RowFmt("batt_v", "%" PRIu16, pl.batt_v);
                            RowArr_i16("batt_temp[0..1]", pl.batt_temp, 2, 0);

                            HeaderRow("[Device]");
                            RowArr_u8("device_type[0..7]",   pl.device_type, 8, 0);
                            RowArr_u8("device_status[0..7]", pl.device_status, 8, 0);
                            RowFmt("dearm_status", "%" PRIu8, pl.dearm_status);

                            HeaderRow("[WDT Counters]");
                            RowFmt("wdt_cnt_gnd", "%" PRIu32, pl.wdt_cnt_gnd);
                            RowFmt("wdt_cnt_i2c", "%" PRIu32, pl.wdt_cnt_i2c);
                            RowFmt("wdt_cnt_can", "%" PRIu32, pl.wdt_cnt_can);
                            RowArr_u32("wdt_cnt_csp[0..1]", pl.wdt_cnt_csp, 2, 0);

                            HeaderRow("[WDT Left]");
                            RowFmt("wdt_gnd_left", "%" PRIu32, pl.wdt_gnd_left);
                            RowFmt("wdt_i2c_left", "%" PRIu32, pl.wdt_i2c_left);
                            RowFmt("wdt_can_left", "%" PRIu32, pl.wdt_can_left);
                            RowArr_u8("wdt_csp_left[0..1]", pl.wdt_csp_left, 2, 0);

                            HeaderRow("[Battery Currents]");
                            RowFmt("batt_chrg",    "%" PRIi16, pl.batt_chrg);
                            RowFmt("batt_dischrg", "%" PRIi16, pl.batt_dischrg);

                            HeaderRow("[Deploy]");
                            RowFmt("ant6_depl", "%" PRIi8, pl.ant6_depl);
                            RowFmt("ar6_depl",  "%" PRIi8, pl.ar6_depl);

                            break;
                        }





                    case REPORT_KIND_ADCS_LOG_MASK:
                    {
                        ImGui::TextUnformatted("ADCS Log Inclusion Mask");

                        auto &pl = view.u.adcs_logmask;

                        ImGui::Separator();
                        ImGui::Text("Fast Inclusion Bitmask:");
                        for (int i = 0; i < 5; i++){
                            ImGui::Text("  [%d] : 0x%02X (byte)", i, pl.FastInclusionBitmask[i]);
                            uint8_t b = pl.FastInclusionBitmask[i];
                            ImGui::Text("[%d] 0x%02X  |  b7 b6 b5 b4 b3 b2 b1 b0 = %d%d%d%d%d%d%d%d",
                                i, b,
                                (b>>7)&1, (b>>6)&1, (b>>5)&1, (b>>4)&1,
                                (b>>3)&1, (b>>2)&1, (b>>1)&1, (b>>0)&1 );
                        }


                        ImGui::Separator();
                        ImGui::Text("Slow Inclusion Bitmask:");
                        for (int i = 0; i < 5; i++){
                            ImGui::Text("  [%d] : 0x%02X (byte)", i, pl.SlowInclusionBitmask[i]);
                            uint8_t b = pl.SlowInclusionBitmask[i];
                            ImGui::Text("[%d] 0x%02X  |  b7 b6 b5 b4 b3 b2 b1 b0 = %d%d%d%d%d%d%d%d",
                                i, b,
                                (b>>7)&1, (b>>6)&1, (b>>5)&1, (b>>4)&1,
                                (b>>3)&1, (b>>2)&1, (b>>1)&1, (b>>0)&1 );
                        }


                        break;
                    }


                    case REPORT_KIND_ADCS_UNSOLICIT_TLM_SETUP_TLM:
                    {
                        auto &pl = view.u.adcs_unsolicited_tlm_tlm;

                        ImGui::TextUnformatted("ADCS Unsolicited TLM Message Setup (TLM)");

                        ImGui::Separator();
                        ImGui::Text("[Return Interval Settings]");
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("UART Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.UARTTlmReturnInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("UART2 Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.UART2TlmReturnInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("CAN Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.CANTlmRetrunInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Reserved");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.Reserved);

                        ImGui::Separator();


                        
                        auto PrintBitmask = [&](const char *title, const uint8_t mask[5])
                        {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted(title);
                            ImGui::TableSetColumnIndex(1);

                            float line_h = ImGui::GetTextLineHeightWithSpacing();
                            float h = line_h * (5 + 1); // 5줄 + 여유

                            ImGui::BeginChild(title, ImVec2(0, h), true, ImGuiWindowFlags_NoScrollbar);

                            for (int i = 0; i < 5; i++)
                            {
                                uint8_t b = mask[i];

                                char bits[9];
                                for (int k = 0; k < 8; k++)
                                    bits[k] = ((b >> (7 - k)) & 1) ? '1' : '0';
                                bits[8] = 0;

                                ImGui::Text("idx %d: 0x%02X | %s", i, b, bits);
                            }

                            ImGui::EndChild();
                        };


                        PrintBitmask("UART ID Inclusion Bitmask",  pl.UARTTlmIDInclusionBitmask);
                        PrintBitmask("UART2 ID Inclusion Bitmask", pl.UART2TlmIDInclusionBitmask);
                        PrintBitmask("CAN ID Inclusion Bitmask",   pl.CANTlmIDInclusionBitmask);

                        break;
                    }

                    case REPORT_KIND_ADCS_RAW_GYR_SENSOR_TLM:
                    {
                        auto &pl = view.u.adcs_raw_gyr_sensor;

                        ImGui::TextUnformatted("ADCS2 Raw GYR Sensor Telemetry");

                        ImGui::Separator();
                        ImGui::Text("[Time]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Seconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeSeconds);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Nanoseconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR0 Raw Rate]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 X");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateX);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Y");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateY);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Z");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR1 Raw Rate]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 X");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateX);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Y");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateY);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Z");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.GYR0ValidFlag,
                                    pl.GYR0ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.GYR1ValidFlag,
                                    pl.GYR1ValidFlag ? "Valid" : "Invalid");

                        break;
                    }


                    case REPORT_KIND_ADCS_RAW_RW_SENSOR_TLM:
                    {
                        auto &pl = view.u.adcs_raw_rw_sensor;

                        ImGui::TextUnformatted("ADCS2 Raw RW Sensor Telemetry");

                        ImGui::Separator();
                        ImGui::Text("[Time]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Seconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeSeconds);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Nanoseconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Reaction Wheel Speed]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL0 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL0MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL1 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL1MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL2 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL2MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL3 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL3MeasSpeed);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL0 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL0ValidFlag,
                                    pl.RWL0ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL1 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL1ValidFlag,
                                    pl.RWL1ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL2 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL2ValidFlag,
                                    pl.RWL2ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL3 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL3ValidFlag,
                                    pl.RWL3ValidFlag ? "Valid" : "Invalid");

                        break;
                    }

                /******************************************************************************************************************************************* */

                case REPORT_KIND_ADCS_GET_WHL_CONFIG :
                {
                    auto &pl = view.u.adcs_whlconfig;

                    ImGui::TextUnformatted("ADCS Whl Config Telemetry");
                    
                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Rwl0]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl1]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl2]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl3]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Ramp Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.WheelRampTorque);
                    
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Scheme");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.WheelScheme);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Failed Wheel ID");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.FailedWheelID);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid Nominal Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.PyramidNominalMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid Tilt Angle");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.PyramidTiltAngle);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SATELLITE_CONFIG :
                {
                    auto &pl = view.u.adcs_satelliteconfig;

                    ImGui::TextUnformatted("ADCS Satellite Config Telemetry");
                    
                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Inertia]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixx");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixx);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Iyy");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Iyy);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Izz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Izz);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixy");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixy);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixz);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Iyz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Iyz);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sun Pointing Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Target Tracking Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sat Tracking Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorZ);

                    break;
                }

                case REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG :
                {
                    auto &pl = view.u.adcs_controllerconfig;

                    ImGui::TextUnformatted("ADCS Controller Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Detumbling Damping Gain (Kd)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.DetumblingDampingGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Spin Gain Sunlit (KDSun)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunSpinGain_Sunlit);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Spin Gain Eclipse (KDecel)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunSpinGain_Eclipse);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Detumbling Spin Gain (Ks)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.DetumblingSpinGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Fast BD Gain (Kdf)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.FastBDotGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Nutation Damping Gain (Kn)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomNutationDampingGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Nutation Damping Quat Gain (Kq)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomNutationDampingQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Quat Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("XGG Quat Gain (Kqx)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.XGGQuatGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("YGG Quat Gain (Kqy)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YGGQuatGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("ZGG Quat Gain (Kqz)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ZGGQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Desat Control Gain (Kh)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.WheelDesatControlGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Proportional Gain (Kp1)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Derivative Gain (Kd1)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("R Wheel Proportional Gain (Kp2)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RWheelProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("R Wheel Derivative Gain (Kd2)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RWheelDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Proportional Gain (Kp3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Derivative Gain (Kd3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Integral Gain (Ki3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingIntegralGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Reference]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reference Spin Rate (deg/s)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ReferenceSpinRate_degps);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reference Wheel Momentum (Nms)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ReferenceWheelMomentum_Nms);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Wheel Bias Momentum (Nms)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YWheelBiasMomentum_Nms);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ref Spin Rate RW (deg/s)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RefSpinRate_RW_degps);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Angle]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Keep Out Angle (deg)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunKeepOutAngle_deg);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Roll Limit Angle (deg)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RollLimitAngle_deg);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Yaw Compensation For Earth Rotation");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.YawCompensationForEarthRotation);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Enable Sun Tracking In Eclipse");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.EnableSunTrackingInEclipse);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Enable Sun Avoidance");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.EnableSunAvoidance);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reserved");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.Reserved);

                    break;

                }

                case REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG :
                {
                    auto &pl = view.u.adcs_defaultmodeconfig;

                    ImGui::TextUnformatted("ADCS Default Mode Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Default Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Run Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultRunMode);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Operational State");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultOperationalState);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode In Op State Safe");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlModeInOpStateSafe);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode In Op State Auto");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlModeInOpStateAuto);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MOUNTING_CONFIG :
                {
                    auto &pl = view.u.adcs_mountingconfig;

                    ImGui::TextUnformatted("ADCS Mounting Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Stack Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack X Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackX_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack Y Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackY_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack Z Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackZ_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ2_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Wheel Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel3_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Pyramid RWL]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[CSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS3_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS4 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS4_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS5 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS5_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS6 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS6_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS7 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS7_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS8 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS8_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS9 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS9_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[FSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[HSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MAG Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[STR Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Sensor Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Gyro Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis3_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis3_mounting);

                    break;
                }

            case REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG :
                {
                    auto &pl = view.u.adcs_estimatorconfig;

                    ImGui::TextUnformatted("ADCS Estimator Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Estimator Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Main Estimator Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultMainEstimatorMode);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Backup Estimator Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultBackupEstimatorMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Measurement Noise]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MAGMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.CSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.FSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.HSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.STRMeasurementNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[System Noise]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MMTRKF System Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MMTRKFSystemNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("EKF System Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.EKFSystemNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Nutation Correction]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Nutation Epsilon Correction");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.NutationEpsilonCorrection);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Nutation Psi Correction");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.NutationPsiCorrection);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use FSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseFSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use CSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseCSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use HSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseHSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use STR in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseSTRinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Triad Vector1");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.TriadVector1);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Triad Vector2");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.TriadVector2);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Spare");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG :
                {
                    auto &pl = view.u.adcs_nodeselectionconfig;

                    ImGui::TextUnformatted("ADCS Node Selection Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Selection Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("RWL Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.RWLSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MAGSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.FSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.HSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("GYR Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.GYRSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.STRSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("GNSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.GNSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtSensorSelectionFlags);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MTQ_CONFIG :
                {
                    auto &pl = view.u.adcs_mtqconfig;

                    ImGui::TextUnformatted("ADCS MTQ Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Max Dipole Moment]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ0 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ0MaxDipoleMoment);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ1 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ1MaxDipoleMoment);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ2 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ2MaxDipoleMoment);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ On Time]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Max MTQ On Time");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MaxMTQOnTime);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Min MTQ On Time");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MinMTQOnTime);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Filter]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Magnetic Control Filter Factor");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MagneticControlFilterFactor);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG :
                {
                    auto &pl = view.u.adcs_magsensingelmconfig;

                    ImGui::TextUnformatted("ADCS Mag Sensing Element Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Mag Sensing Element]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mag0 Sensing Element");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Mag0SensingElement);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mag1 Sensing Element");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Mag1SensingElement);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Spare");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG :
                {
                    auto &pl = view.u.adcs_satorbitparamconfig;

                    ImGui::TextUnformatted("ADCS Sat Orbit Param Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Orbit Parameters]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Epoch");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Epoch);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Inclination");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Inclination);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("RAAN");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.RAAN);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Eccentricity");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Eccentricity);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("AOP");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.AOP);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mean Anomaly");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.MeanAnomaly);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mean Motion");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.MeanMotion);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("B Star Drag");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.B_StarDrag);

                    break;
                }

                /******************************************************************************************************************************************** */

                case REPORT_KIND_UANT_GET_STATUS_TLM:
                {
                    auto &pl = view.u.uant_getstatus;

                    ImGui::TextUnformatted("UANT ANT6 Release Status");

                    ImGui::Separator();
                    ImGui::Text("[Channel 0]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 0 State");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u (%s)", pl.channel_0_state,
                                 pl.channel_0_state ? "Burning" : "Idle");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 0 Status");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u (%s)", pl.channel_0_status,
                                 pl.channel_0_status ? "Released" : "Not released");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 0 Burn Time Left [s]");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.channel_0_burn_time_left);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 0 Burn Tries");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.channel_0_burn_tries);

                    ImGui::Separator();
                    ImGui::Text("[Channel 1]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 1 State");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u (%s)", pl.channel_1_state,
                                 pl.channel_1_state ? "Burning" : "Idle");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 1 Status");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u (%s)", pl.channel_1_status,
                                 pl.channel_1_status ? "Released" : "Not released");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 1 Burn Time Left [s]");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.channel_1_burn_time_left);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Channel 1 Burn Tries");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.channel_1_burn_tries);

                    break;
                }



                    case REPORT_KIND_SC_GENERIC:
                    {
                        show_hexdump = true;
                        ImGui::Text("Generic payload (%u bytes)", view.ret_val_size);
                        break;      
                    }

                    
                    default:
                        show_hexdump = true;
                        ImGui::Text("Default type payload (%u bytes)", view.ret_val_size);
                        break;


                    }

                }

                ImGui::EndTable();
            }

            if (view.valid &&
                show_hexdump)
            {
                ImGui::Separator();
                ImGui::Text("Payload (Hex dump):");

                ImGui::BeginChild("##ReportPayload",
                                ImVec2(ImGui::GetContentRegionAvail().x,
                                        ImGui::GetContentRegionAvail().y),
                                true, ImGuiWindowFlags_HorizontalScrollbar);

                int bytes_per_line = 16;
                uint16_t dump_size = view.ret_val_size;
                if (dump_size > sizeof(view.u.generic.bytes))
                    dump_size = sizeof(view.u.generic.bytes);

                for (int i = 0; i < (int)dump_size; i++)
                {
                    ImGui::SameLine(0.0f, 0.0f);
                    ImGui::Text("%02X ", view.u.generic.bytes[i]);
                    if ((i + 1) % bytes_per_line == 0)
                        ImGui::NewLine();
                }

                if (dump_size % bytes_per_line != 0)
                    ImGui::NewLine();

                ImGui::EndChild();
            }

            ImGui::SetWindowFontScale(1.0f * fontscale);
            ImGui::EndTabItem();
        }
}
