// View - Beacon 창 "Report" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"

// 표 머리글 고정 행/열 수 (YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawReportTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
        if (ImGui::BeginTabItem("Report"))
        {
            bool show_hexdump = false;
            ReportView_t view;
            view = vm.ReportSnapshot();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
            ImGui::Text("Total RX Bytes (Report) : %" PRIu32, vm.TotalRxBytes());
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

                    // EPS / ADCS / UANT 리포트는 view/beacon/report/report_<종류>.cpp
                    if (DrawEpsReport(view) || DrawAdcsReport(view) || DrawUantReport(view))
                    {
                    }
                    else switch (view.kind)
                    {
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
