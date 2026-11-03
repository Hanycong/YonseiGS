// Model - 누리호 5차 위성 리포트 조립 (model/telemetry/n5_report_assembler.cpp)
//
// 수신 스레드(task_downlink_onorbit)가 리포트 포트에서 읽은 CSP 패킷을 하나씩 넘긴다.
#pragma once

#include "YonseiGS_coms.h"     // Report, csp_packet_t

// BEE1012(포트 15) / UELYSYS(포트 10): CCSDS 길이 필드(오프셋 4, 전체 = 값 + 7)만큼 모이면
// raw_save_dir 에 원본을 저장하고 MsgId 로 나눠 해석한다.
//   0x0825 = 524 바이트 리포트 N 개, 0x0826 = 532 바이트 critical 리포트 N 개, 0x0827 = RPT OPS (g_rpt_ops_data)
// 리포트마다 g_report_view 를 갱신하고 report_saver 로 저장한다.
void AssembleN5ReportChunk(const char *sat_tag, int dport, const csp_packet_t *packet,
                           const char *raw_save_dir, const char *raw_filename_prefix,
                           int (*report_saver)(Report *));

// SD1SAT(포트 20): 4 바이트 조각 헤더(StreamID BE16, Index, Count)를 순서대로 이어 붙여
// 마지막 조각에서 ../data/SD1SAT/report 에 저장하고 해석해 g_report_view 를 갱신한다
void AssembleSD1SATReportChunk(int dport, const csp_packet_t *packet);
