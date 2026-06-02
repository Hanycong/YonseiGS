/* ============================================================
 *  miman_theme.h
 *
 *  BEE-1000 GS — 디자인 토큰 & 위젯 헬퍼
 *  ------------------------------------------------------------
 *  • 모든 컬러는 namespace mim:: 의 constexpr 상수.
 *  • 헬퍼 위젯(Dot/Badge/Toggle/PillButton 등)은 ImGui 위에서
 *    별도 의존성 없이 동작.
 *  • ApplyTheme() 한 번 호출로 ImGui 스타일 전체 적용.
 *  • 기존 ImGuiCustomStyle()과 공존 가능 — 점진 마이그레이션.
 * ============================================================ */
#pragma once
#ifndef __MIMAN_THEME_H_
#define __MIMAN_THEME_H_

#include <imgui.h>

namespace mim {

/* =========================================================
 *  1. 컬러 토큰
 *     (V1.1 목업의 CSS 변수와 1:1 대응)
 * ========================================================= */
inline const ImVec4 BG_DEEP     = ImVec4(0.110f, 0.251f, 0.286f, 1.00f);
inline const ImVec4 BG_MAIN     = ImVec4(0.141f, 0.310f, 0.376f, 1.00f);
inline const ImVec4 BG_PANEL    = ImVec4(0.173f, 0.365f, 0.424f, 1.00f);
inline const ImVec4 BG_PANEL2   = ImVec4(0.102f, 0.239f, 0.282f, 1.00f);
inline const ImVec4 BG_SATTAB   = ImVec4(0.078f, 0.176f, 0.204f, 1.00f);

inline const ImVec4 ACCENT      = ImVec4(0.235f, 0.603f, 0.624f, 1.00f);
inline const ImVec4 ACCENT_2    = ImVec4(0.180f, 0.462f, 0.478f, 1.00f);
inline const ImVec4 ACCENT_SOFT = ImVec4(0.235f, 0.603f, 0.624f, 0.18f);

inline const ImVec4 BORDER      = ImVec4(0.972f, 0.733f, 0.701f, 0.25f);
inline const ImVec4 BORDER_HI   = ImVec4(0.972f, 0.733f, 0.701f, 0.45f);

/* 텍스트 — 기존 살구색 유지 + 위계 추가 */
inline const ImVec4 TEXT        = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
inline const ImVec4 TEXT_HI     = ImVec4(1.000f, 0.898f, 0.859f, 1.00f);
inline const ImVec4 TEXT_DIM    = ImVec4(0.722f, 0.565f, 0.541f, 1.00f);
inline const ImVec4 TEXT_MUTE   = ImVec4(0.486f, 0.408f, 0.388f, 1.00f);

/* 시맨틱 컬러 */
inline const ImVec4 OK          = ImVec4(0.373f, 0.812f, 0.647f, 1.00f);
inline const ImVec4 WARN        = ImVec4(0.910f, 0.769f, 0.463f, 1.00f);
inline const ImVec4 ERR         = ImVec4(0.902f, 0.420f, 0.420f, 1.00f);
inline const ImVec4 INFO        = ImVec4(0.498f, 0.737f, 1.000f, 1.00f);

inline const ImVec4 DANGER      = ImVec4(0.650f, 0.270f, 0.270f, 1.00f);
inline const ImVec4 DANGER_HOV  = ImVec4(0.760f, 0.330f, 0.330f, 1.00f);

/* =========================================================
 *  2. 위성별 컬러 팔레트
 *     (탭/배지/필터 chip/Beacon line에 일관 적용)
 * ========================================================= */
enum { SAT_PALETTE_COUNT = 6 };
inline const ImVec4 SAT_COLORS[SAT_PALETTE_COUNT] = {
    {0.373f, 0.812f, 0.647f, 1.0f},  /* 0 mint     — BEE-1000 기본 */
    {0.910f, 0.769f, 0.463f, 1.0f},  /* 1 amber    — SNUGLITE-II  */
    {0.722f, 0.643f, 0.949f, 1.0f},  /* 2 lavender — ISS          */
    {0.498f, 0.749f, 1.000f, 1.0f},  /* 3 sky                     */
    {0.957f, 0.612f, 0.510f, 1.0f},  /* 4 coral                   */
    {0.616f, 0.871f, 0.690f, 1.0f},  /* 5 jade                    */
};

/* =========================================================
 *  3. 유틸리티 (inline)
 * ========================================================= */
inline ImU32 U32(const ImVec4& c) {
    return ImGui::ColorConvertFloat4ToU32(c);
}

inline ImVec4 Alpha(const ImVec4& c, float a) {
    return ImVec4(c.x, c.y, c.z, a);
}

/* 위성 인덱스(0 ~ N) → 팔레트 컬러 (순환) */
inline ImVec4 SatColor(int slot_idx) {
    if (slot_idx < 0) slot_idx = 0;
    return SAT_COLORS[slot_idx % SAT_PALETTE_COUNT];
}

/* 위성명을 hash해서 안정적인 컬러 부여 (탭 순서 바뀌어도 색 유지) */
ImVec4 SatColorByName(const char* name);

/* =========================================================
 *  4. 테마 적용
 *     main()에서 ImGui::CreateContext() 직후 한 번 호출.
 *     기존 ImGuiCustomStyle()을 호출해도 충돌 없음 (덮어씀).
 * ========================================================= */
void ApplyTheme(ImGuiStyle* style = nullptr);

/* =========================================================
 *  5. 위젯 헬퍼
 * ========================================================= */

/* 현재 커서 위치에 컬러 점 1개 + 점만큼 가로 공간 차지.
 * 이후 ImGui::SameLine()으로 텍스트 잇기 가능.                */
void Dot(const ImVec4& color, float radius = 4.5f);

/* "● GS100" 형식 — 점 + 라벨 한 묶음. SameLine 없이 호출 직후
 * 가로 흐름에 자연스럽게 들어감.                                */
void StatusPill(const char* label, bool ok);

/* "● Engage" / "○ Doppler" 형식 — 활성 모드 표시용.
 * on=true 일 때만 OK 컬러, false면 dim.                        */
void ModeFlag(const char* label, bool on);

/* 캡슐 모양 배지 (TC 프로파일 표시 등) */
void Badge(const char* text, const ImVec4& bg,
           const ImVec4& fg = TEXT_HI);

/* 섹션 헤더 — "── LABEL ──" 형태 (Separator 포함, 위/아래 spacing) */
void SectionHeader(const char* label);

/* 단축키 힌트 — "[F2]" 같은 작은 라벨. SameLine 후 호출 권장. */
void KeyboardHint(const char* keys);

/* 토글 스위치 (커스텀 DrawList 위젯).
 * - Checkbox 대체 가능.
 * - 변경되면 true 반환 + *v 토글.                              */
bool Toggle(const char* label, bool* v);

/* Pill 모양 버튼 — Command Window의 Subsystem 선택용.
 * - selected=true면 active_col 배경 + 어두운 텍스트.
 * - SameLine으로 가로 배치 권장.                               */
bool PillButton(const char* label, bool selected,
                const ImVec4& active_col = ACCENT);

/* 큰 숫자 readout (Battery 8.21V 같은 카드)
 * - label: 상단 작은 라벨
 * - value: 큰 숫자 (예: "8.21 V")
 * - sub:   하단 보조 텍스트 (nullable)
 * - value_color: 큰 숫자 컬러 (OK/WARN/ERR 등)                 */
void BigReadout(const char* label, const char* value,
                const char* sub = nullptr,
                const ImVec4& value_color = TEXT_HI);

/* 위험 액션용 버튼 (빨강) — Stop Engage / Park 등 */
bool DangerButton(const char* label, const ImVec2& size = ImVec2(0, 0));

/* 기본 강조 버튼 (Accent) — Send Now / Engage 시작 등 */
bool PrimaryButton(const char* label, const ImVec2& size = ImVec2(0, 0));

/* Ctrl 키를 누른 상태에서만 활성화되는 버튼 (Park 등 위험 액션).
 * - 키 안 눌렀을 땐 dim + 클릭 무시.
 * - hint_below=true면 버튼 아래에 "⚠ Ctrl 누른 상태에서만…" 표시. */
bool CtrlGuardedButton(const char* label, bool hint_below = true,
                       const ImVec2& size = ImVec2(0, 0));

/* =========================================================
 *  6. 레이아웃 헬퍼
 * ========================================================= */

/* 세로 공간 spacer */
inline void SpacerY(float h = 4.0f) { ImGui::Dummy(ImVec2(0, h)); }

/* 가로 공간 spacer */
inline void SpacerX(float w = 6.0f) { ImGui::Dummy(ImVec2(w, 0)); ImGui::SameLine(); }

/* "label : value" 한 줄 (key/value row).
 * - label은 dim 색, value는 hi 색.
 * - value_col로 컬러 오버라이드 가능 (status 표시 등).          */
void KVRow(const char* label, const char* value,
           const ImVec4& value_col = TEXT_HI);

/* printf 스타일 KV row */
void KVRowf(const char* label, const char* fmt, ...);

} // namespace mim

#endif /* __MIMAN_THEME_H_ */
