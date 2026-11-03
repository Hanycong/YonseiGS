// View - Beacon 창 "Report" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
//
// BEE-1000 모드는 예전 화면 그대로 (리포트 종류별 행은 report/report_{eps,adcs,uant}.cpp).
// 누리호 5차 위성 모드(BEE1012 / UELYSYS / SD1-SAT)는 GS_nuri5 Report 탭을 옮긴 것:
//   - [Body] 에 "Payload Decode" 행, 해석 실패면 필드 대신 크기만 보이고 원시 바이트 덤프를 강제
//   - 리포트 종류별 행은 report/report_rows.h 의 Draw*Report (report_app/uelysys/bee1012/sd1sat/
//     eps_p80/adcs_get/roma_lgpm.cpp) 와 BEE-1000 과 같은 report_{eps,adcs,uant}.cpp
//   - 원시 바이트 덤프는 raw_bytes/raw_size 기준, 비었으면 (empty), 잘렸으면 truncated 표시
// 모델이 값을 주면 RPT Ops Data 머리와 [Critical] 구역을 보인다 (BEE-1000 포함. 지금 BEE-1000 수신은 채우지 않음).
// [CR Envelope] 은 5차 모드의 SD1-SAT 리포트 종류에서만.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"
#include "report_rows.h"
#include "sat_mode.h"

// 표 머리글 고정 행/열 수 (YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawReportTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
        if (ImGui::BeginTabItem("Report"))
        {
            bool show_hexdump = false;
            // EPS RPARAM 재조립 버퍼가 커서(128KB) 스택 대신 static
            static ReportView_t view;
            static EPS_Report_State_t eps_state;
            static EPS_RParam_Reassembly_State_t eps_rparam;
            RPT_OpsData_t rpt_ops;
            bool rpt_ops_valid;
            vm.ReportSnapshot5(view, eps_state, eps_rparam, rpt_ops, rpt_ops_valid);
            const bool bee1000 = mim::sat::Current() == mim::sat::SAT_BEE1000;
            ReportCtx ctx = { view, eps_state, eps_rparam, show_hexdump };

            if (rpt_ops_valid && ImGui::CollapsingHeader("RPT Ops Data (RPT_OPS_TLM_MID)"))
            {
                ImGui::Text("BootCount    : %u", (unsigned)rpt_ops.boot_count);
                ImGui::Text("ResetCause   : 0x%02X", (unsigned)rpt_ops.reset_cause);
                ImGui::Text("S/C Time     : %u.%06u sec", (unsigned)rpt_ops.time_sec, (unsigned)rpt_ops.time_subsec);
                ImGui::Text("Epoch        : %u.%06u sec", (unsigned)rpt_ops.epoch_sec, (unsigned)rpt_ops.epoch_subsec);
                ImGui::Text("Sequence     : %u", (unsigned)rpt_ops.sequence);
                ImGui::Text("CRC          : 0x%08X", (unsigned)rpt_ops.crc);
                ImGui::Separator();
            }

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
                    if (!bee1000)
                        ReportRow("Payload Decode", "%s", view.payload_valid ? "valid" : "INVALID / size mismatch");

                    // SD1-SAT CR 봉투: 모델이 CrSeq 를 crit_time_sec 에, IsRetransmit 을 is_critical 에 담아 준다
                    const bool is_sd1sat_kind = !bee1000 && (view.kind >= REPORT_KIND_SD1SAT_CR &&
                                                 view.kind <= REPORT_KIND_SD1SAT_UTRX);
                    if (is_sd1sat_kind)
                    {
                        ImGui::TableNextRow();
                        BeaconSectionHeader("[CR Envelope]");
                        ReportRow("CrSeq", "%u", (unsigned)view.crit_time_sec);
                        ReportRow("IsRetransmit", "%s", view.is_critical ? "1 (replay)" : "0 (first send)");
                    }
                    else if (view.is_critical)
                    {
                        ImGui::TableNextRow();
                        BeaconSectionHeader("[Critical]");
                        ReportRow("Source", "%s", "RPT_CRITICAL_TLM_MID (RPT_Data.CritPipe)");
                        ReportRow("Report Time (sec)", "%u", (unsigned)view.crit_time_sec);
                        ReportRow("Report Time (subsec)", "%u", (unsigned)view.crit_time_subsec);
                    }

                    ImGui::TableNextRow();
                    BeaconSectionHeader("[Report Type]");

                    // 해석에 실패한 리포트는 필드(0 이거나 지난 값)를 그리지 않고 아래 원시 바이트만 보인다
                    if (!bee1000 && !view.payload_valid)
                    {
                        rpt5::RowText("Type", "Decode failed (size/shape mismatch)");
                        rpt5::RowFmt("Payload Size", "%" PRIu16, view.raw_size);
                        rpt5::RowText("Payload", "See hex dump below");
                    }
                    // EPS / ADCS / UANT 리포트는 view/beacon/report/report_<종류>.cpp
                    // BEE-1000: 예전 종류 먼저 (그대로), 5차 위성: 5차 형식 먼저 (ADCS 설정 리포트 형식이 다름)
                    else if (bee1000 && (DrawEpsReport(view) || DrawAdcsReport(view) || DrawUantReport(view)))
                    {
                    }
                    else if ((bee1000 ? view.payload_valid : true) &&
                             (DrawAppReport(ctx) || DrawEpsP80Report(ctx) || DrawEpsReport(view) ||
                              DrawAdcsGetReport(ctx) || DrawAdcsReport(view) || DrawUantReport(view) ||
                              DrawRomaLgpmReport(ctx)))
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

            // 5차 위성: 해석에 실패한 리포트는 어떤 종류든 보낸 바이트를 그대로 보인다
            if (!bee1000 && view.valid && !view.payload_valid)
                show_hexdump = true;

            if (view.valid &&
                show_hexdump && !bee1000)
            {
                ImGui::Separator();
                ImGui::Text("Payload (Hex dump):");

                ImGui::BeginChild("##ReportPayload",
                                ImVec2(ImGui::GetContentRegionAvail().x,
                                        ImGui::GetContentRegionAvail().y),
                                true, ImGuiWindowFlags_HorizontalScrollbar);

                int bytes_per_line = 16;
                uint16_t dump_size = view.raw_size;
                bool truncated = false;
                if (dump_size == 0 && view.ret_val_size > 0)
                    truncated = true;
                if (dump_size > sizeof(view.raw_bytes))
                {
                    dump_size = sizeof(view.raw_bytes);
                    truncated = true;
                }
                if (view.ret_val_size > dump_size)
                    truncated = true;

                if (dump_size == 0)
                {
                    ImGui::TextUnformatted("(empty)");
                }
                else
                {
                    for (int i = 0; i < (int)dump_size; i++)
                    {
                        ImGui::SameLine(0.0f, 0.0f);
                        ImGui::Text("%02X ", (unsigned int)view.raw_bytes[i]);
                        if ((i + 1) % bytes_per_line == 0)
                            ImGui::NewLine();
                    }

                    if (dump_size % bytes_per_line != 0)
                        ImGui::NewLine();
                }
                if (truncated)
                    ImGui::Text("truncated: showing %u of %u bytes",
                                (unsigned int)dump_size,
                                (unsigned int)view.ret_val_size);

                ImGui::EndChild();
            }
            else if (view.valid &&
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
