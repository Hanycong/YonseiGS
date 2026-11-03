// Model - 누리호 5차 위성 리포트 저장 (model/telemetry/n5_report_logger.cpp)
//
// 리포트 하나를 해석해 ../data/<위성>/report/report_parsed/Report--<시각>--.txt 로 저장한다.
// 성공 0, rpt 가 NULL 이면 -1, 파일을 못 열면 -2
#pragma once

#include "YonseiGS_coms.h"     // Report

int BEE1012ReportSaver(Report* rpt);    // PAY_SLT 는 ../data/BEE1012/PAY_SLT/report_parsed
int UELYSYSReportSaver(Report* rpt);
