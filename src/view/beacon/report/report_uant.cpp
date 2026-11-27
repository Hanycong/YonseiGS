// View - Beacon 창 "Report" 탭: UANT 상태 리포트
//
// report_tab.cpp 의 리포트 종류 switch 에서 이 종류의 case 를 내용 그대로 옮긴 것.
// 처리한 종류면 true. 행은 report_tab.cpp 의 표(##ReportTable) 안에 그려진다.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"
#include "report_rows.h"

bool DrawUantReport(ReportView_t & view)
{
                    switch (view.kind)
                    {
                case REPORT_KIND_UANT_GET_STATUS_TLM:
                {
                    auto &pl = view.u.uant_getstatus;

                    ImGui::TextUnformatted("UANT ANT6 Release Status");

                    ImGui::Separator();
                    ImGui::Text("[Channel 0]");

                    rpt5::RowFmt("Channel 0 State", "%u (%s)", pl.channel_0_state, pl.channel_0_state ? "Burning" : "Idle");

                    rpt5::RowFmt("Channel 0 Status", "%u (%s)", pl.channel_0_status, pl.channel_0_status ? "Released" : "Not released");

                    rpt5::RowFmt("Channel 0 Burn Time Left [s]", "%u", pl.channel_0_burn_time_left);

                    rpt5::RowFmt("Channel 0 Burn Tries", "%u", pl.channel_0_burn_tries);

                    ImGui::Separator();
                    ImGui::Text("[Channel 1]");

                    rpt5::RowFmt("Channel 1 State", "%u (%s)", pl.channel_1_state, pl.channel_1_state ? "Burning" : "Idle");

                    rpt5::RowFmt("Channel 1 Status", "%u (%s)", pl.channel_1_status, pl.channel_1_status ? "Released" : "Not released");

                    rpt5::RowFmt("Channel 1 Burn Time Left [s]", "%u", pl.channel_1_burn_time_left);

                    rpt5::RowFmt("Channel 1 Burn Tries", "%u", pl.channel_1_burn_tries);

                    break;
                }




                    default:
                        return false;
                    }
                    return true;
}
