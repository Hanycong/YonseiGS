// View - Beacon 창 탭 공통: 표 도우미와 탭 함수 선언
//
// Beacon 창(view/beacon_view.cpp)은 탭 6개로 이루어지고, 탭마다 파일 하나다.
//   beacon_tab, mission_beacon_tab, report_tab, event_tab, response_tab, ax100_rparam_tab
#pragma once

// 2열(Parameter / Data) 표의 한 줄: 왼쪽 칸에 이름을 쓰고 오른쪽 칸으로 넘어간다
void StateWindowColumnManager(const char * info);
// 표 안의 구역 머리글 줄
void BeaconSectionHeader(const char* title);

// 탭 하나씩 (BeginTabItem 부터 EndTabItem 까지)
void DrawBeaconTab(float fontscale);   // "Beacon"
void DrawMissionBeaconTab(float fontscale);   // "Mission Beacon"
void DrawReportTab(float fontscale);   // "Report"
void DrawEventTab(float fontscale);   // "Event"
void DrawResponseTab(float fontscale);   // "Response"
void DrawAx100RparamTab(float fontscale);   // "AX100 RPARAM"
