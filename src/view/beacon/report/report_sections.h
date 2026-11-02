// View - Beacon 창 "Report" 탭의 리포트 종류별 표 행 (view/beacon/report/report_<종류>.cpp)
//
// 각 함수는 view.kind 가 자기 종류면 행을 그리고 true, 아니면 아무것도 하지 않고 false.
#pragma once

#include "YonseiGS_coms.h"     // ReportView_t

bool DrawEpsReport(ReportView_t & view);
bool DrawAdcsReport(ReportView_t & view);
bool DrawUantReport(ReportView_t & view);
