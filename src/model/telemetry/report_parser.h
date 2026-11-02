// Model - 리포트(RPT) 수신 파서 (model/telemetry/report_parser.cpp)
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "YonseiGS_coms.h"     // Report, ReportView_t

static constexpr size_t REPORT_WIRE_SIZE = 540;     // 리포트 와이어 프레임 길이

// 540바이트 프레임을 Report 로. 형식이 맞지 않으면 false
bool ParseReportWire540(const uint8_t *buf, size_t len, Report &out);

// g_report_view(화면용 리포트 값)를 rpt 로 갱신 (g_report_view_mtx 잠금)
void UpdateReportViewFromReport(const Report &rpt);
