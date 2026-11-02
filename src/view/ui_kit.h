// View 공용 UI 부품 - 섹션 머리글, 라벨 열 정렬, 버튼 격자, 상태 점
//
// 모든 창이 같은 간격·높이·색을 쓰도록 여기 한 곳에 모았다. 색은 YonseiGS_theme.h 토큰을 쓴다.
#pragma once
#include "imgui.h"

namespace mim { namespace ui {

// 섹션 머리글: 작은 굵은 대문자 라벨 + 구분선
void SectionHeader(const char * title);

// 행 라벨: 왼쪽 라벨 열(label_em 글자폭)에 맞춰 다음 위젯을 같은 줄에 놓는다
void RowLabel(const char * label, float label_em = 6.0f);

// 표준 버튼 높이 (입력칸 높이의 1.3배)
float ButtonHeight();

// 2열 버튼 격자: col 0 은 줄 시작(왼쪽 절반), col 1 은 같은 줄 남은 칸
bool GridButton(const char * label, int col);

// 전체 폭 버튼
bool WideButton(const char * label);

// 켜짐/꺼짐 전환 버튼: active 이면 위험색(적색)으로 그린다 (Stop/Off 동작을 뜻함)
bool ToggleWideButton(const char * label, bool active);

// 상태 점 + 라벨 (ok 녹색 / 아니면 적색)
void StatusLamp(const char * label, bool ok);

// 라벨 + 오른쪽 정렬 값 한 줄
void ValueRow(const char * label, const char * value);

}} // namespace mim::ui
