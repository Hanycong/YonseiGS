// Model - 누리호 5차 위성 리포트 해석 (model/telemetry/n5_report_decoder.cpp)
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "YonseiGS_coms.h"     // Report, SD1SATReport, ReportView_t

// v.kind / reflected_msg_id / reflected_cc 를 채운 뒤 부른다. payload 를 v.u.* 로 해석, 형식이 맞으면 true
// (GS_nuri5 DecodePayloadToView 그대로. raw_bytes 도 채운다)
bool DecodeN5PayloadToView(ReportView_t &v, const uint8_t *payload, uint16_t payload_len);

// BEE1012(포트 15) / UELYSYS(포트 10) 리포트 하나로 g_report_view 갱신 (g_report_view_mtx 잠금, EPS 상태 포함).
// 종류는 DetermineReportKind, 단 포트 15 의 PAY_SLT MID 는 REPORT_KIND_BEE1012_PAY_SLT
void UpdateN5ReportView(const Report &rpt, int report_dport);

// SD1SAT(포트 20) 리포트 하나로 g_report_view 갱신 (DetermineSD1SATReportKind)
void UpdateSD1SATReportView(const SD1SATReport &rpt);

// 저장(n5_report_logger.cpp)에서도 쓰는 도우미
bool DecodeEPSQueryHeader(EPS_Report_Metadata_t &meta, const uint8_t *payload, uint16_t payload_len,
                          uint8_t command_code, int32_t return_code);
const char *PaySltCommandName(uint8_t cc);

// PAY_SLT PAR_GET 의 값 형식
enum : uint8_t {
    PAY_SLT_PARAM_UINT8  = 0,
    PAY_SLT_PARAM_UINT16 = 1,
    PAY_SLT_PARAM_UINT32 = 2,
    PAY_SLT_PARAM_INT8   = 4,
    PAY_SLT_PARAM_INT16  = 5,
    PAY_SLT_PARAM_FLOAT  = 13,
    PAY_SLT_PARAM_STRING = 14
};
size_t PaySltParamElementSize(uint8_t type);   // 형식 하나의 바이트 수, 모르는 형식은 0
