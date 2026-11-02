// View - Beacon 창 "Report" 탭: EPS P60 (PDU / ACU / DOCK) 테이블 HK 리포트
//
// report_tab.cpp 의 리포트 종류 switch 에서 이 종류의 case 를 내용 그대로 옮긴 것.
// 처리한 종류면 true. 행은 report_tab.cpp 의 표(##ReportTable) 안에 그려진다.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"

bool DrawEpsReport(ReportView_t & view)
{
                auto ReportRow = [](const char* label, const char* fmt, auto value)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(label);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(fmt, value);
                };

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






                    default:
                        return false;
                    }
                    return true;
}
