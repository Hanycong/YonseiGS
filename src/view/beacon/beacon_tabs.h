// View - Beacon 창 탭 공통: 표 도우미와 탭 함수 선언
//
// Beacon 창(view/beacon_view.cpp)은 탭 6개로 이루어지고, 탭마다 파일 하나다.
//   beacon_tab, mission_beacon_tab, report_tab, event_tab, response_tab, ax100_rparam_tab
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "YonseiGS_coms.h"     // 5차 위성 비콘 구조체

// 2열(Parameter / Data) 표의 한 줄: 왼쪽 칸에 이름을 쓰고 오른쪽 칸으로 넘어간다
void StateWindowColumnManager(const char * info);
// 표 안의 구역 머리글 줄
void BeaconSectionHeader(const char* title);
// 배열 한 줄: 이름 | 값들을 공백으로 이어서 (16진 바이트 / u8 / i16 / u16). 길이 제한 없음
void BeaconRawBytesRow(const char* label, const uint8_t* data, size_t len);
void BeaconU8ArrayRow(const char* label, const uint8_t* data, size_t len);
void BeaconI16ArrayRow(const char* label, const int16_t* data, size_t len);
void BeaconU16ArrayRow(const char* label, const uint16_t* data, size_t len);

// "Beacon" 탭의 5차 위성 비콘 표 (b 가 NULL 이면 "No data")
void DrawBEE1012Beacon(const BEE1012_Beacon_t * b);   // beacon_bee1012.cpp
void DrawUELYSYSBeacon(const UELYSYS_Beacon_t * b);   // beacon_uelysys.cpp
void DrawSD1SATBeacon(const SD1SAT_Beacon_t * b);     // beacon_sd1sat.cpp

// 탭 하나씩 (BeginTabItem 부터 EndTabItem 까지)
void DrawBeaconTab(float fontscale);   // "Beacon"
void DrawMissionBeaconTab(float fontscale);   // "Mission Beacon"
void DrawReportTab(float fontscale);   // "Report"
void DrawEventTab(float fontscale);   // "Event"
void DrawResponseTab(float fontscale);   // "Response"
void DrawAx100RparamTab(float fontscale);   // "AX100 RPARAM"
