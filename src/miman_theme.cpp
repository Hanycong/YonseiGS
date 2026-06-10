/* ============================================================
 *  miman_theme.cpp
 *
 *  miman_theme.h 의 구현부.
 *  - 외부 의존성: ImGui 만.
 *  - imgui_internal.h 불필요 (모두 public API 사용).
 * ============================================================ */
#include "miman_theme.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace mim {

ImFont* font_title    = nullptr;
ImFont* font_subtitle = nullptr;

/* =========================================================
 *  Color utility — name hash → palette index
 * ========================================================= */
ImVec4 SatColorByName(const char* name)
{
    if (!name || !*name) return SAT_COLORS[0];
    /* djb2 hash (간단하고 충분) */
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*name++))
        h = ((h << 5) + h) + c;
    return SAT_COLORS[(int)(h % SAT_PALETTE_COUNT)];
}

/* =========================================================
 *  ApplyTheme — ImGuiStyle 전체 세팅
 *  기존 ImGuiCustomStyle()의 컬러를 mim:: 토큰으로 재정의.
 *  호출 시점: main()에서 ImGui::CreateContext() 직후.
 * ========================================================= */
void ApplyTheme(ImGuiStyle* style_arg)
{
    ImGuiStyle& s = style_arg ? *style_arg : ImGui::GetStyle();

    /* ── Layout / rounding ── */
    s.WindowRounding    = 5.0f;
    s.ChildRounding     = 4.0f;
    s.FrameRounding     = 4.0f;
    s.PopupRounding     = 5.0f;
    s.ScrollbarRounding = 6.0f;
    s.GrabRounding      = 4.0f;
    s.TabRounding       = 4.0f;

    s.WindowPadding     = ImVec2(12, 12);
    s.FramePadding      = ImVec2(8, 5);
    s.ItemSpacing       = ImVec2(8, 6);
    s.ItemInnerSpacing  = ImVec2(6, 4);
    s.IndentSpacing     = 18.0f;

    s.WindowBorderSize  = 1.0f;
    s.ChildBorderSize   = 1.0f;
    s.FrameBorderSize   = 0.0f;
    s.PopupBorderSize   = 1.0f;
    s.TabBorderSize     = 0.0f;

    s.WindowTitleAlign  = ImVec2(0.0f, 0.5f);

    /* ── Colors ── */
    ImVec4* c = s.Colors;
    c[ImGuiCol_Text]                  = TEXT;
    c[ImGuiCol_TextDisabled]          = TEXT_MUTE;
    c[ImGuiCol_WindowBg]              = BG_MAIN;
    c[ImGuiCol_ChildBg]               = BG_MAIN;
    c[ImGuiCol_PopupBg]               = BG_MAIN;
    c[ImGuiCol_Border]                = BORDER;
    c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

    c[ImGuiCol_FrameBg]               = BG_PANEL2;
    c[ImGuiCol_FrameBgHovered]        = Alpha(ACCENT, 0.25f);
    c[ImGuiCol_FrameBgActive]         = Alpha(ACCENT, 0.40f);

    c[ImGuiCol_TitleBg]               = ACCENT_2;
    c[ImGuiCol_TitleBgActive]         = ACCENT;
    c[ImGuiCol_TitleBgCollapsed]      = BG_PANEL2;

    c[ImGuiCol_MenuBarBg]             = ACCENT_2;

    c[ImGuiCol_ScrollbarBg]           = BG_PANEL2;
    c[ImGuiCol_ScrollbarGrab]         = ACCENT_2;
    c[ImGuiCol_ScrollbarGrabHovered]  = ACCENT;
    c[ImGuiCol_ScrollbarGrabActive]   = ACCENT;

    c[ImGuiCol_CheckMark]             = OK;
    c[ImGuiCol_SliderGrab]            = ACCENT;
    c[ImGuiCol_SliderGrabActive]      = ACCENT;

    c[ImGuiCol_Button]                = ACCENT_2;
    c[ImGuiCol_ButtonHovered]         = ACCENT;
    c[ImGuiCol_ButtonActive]          = ImVec4(0.561f, 0.561f, 0.580f, 1.0f);

    c[ImGuiCol_Header]                = ACCENT_2;
    c[ImGuiCol_HeaderHovered]         = ACCENT;
    c[ImGuiCol_HeaderActive]          = ACCENT;

    c[ImGuiCol_Separator]             = BORDER;
    c[ImGuiCol_SeparatorHovered]      = ACCENT;
    c[ImGuiCol_SeparatorActive]       = ACCENT;

    c[ImGuiCol_ResizeGrip]            = Alpha(ACCENT, 0.30f);
    c[ImGuiCol_ResizeGripHovered]     = Alpha(ACCENT, 0.60f);
    c[ImGuiCol_ResizeGripActive]      = ACCENT;

    c[ImGuiCol_Tab]                   = ACCENT_2;
    c[ImGuiCol_TabHovered]            = ACCENT;
    c[ImGuiCol_TabActive]             = ACCENT;
    c[ImGuiCol_TabUnfocused]          = BG_PANEL2;
    c[ImGuiCol_TabUnfocusedActive]    = ACCENT_2;

    c[ImGuiCol_TableHeaderBg]         = BG_PANEL2;
    c[ImGuiCol_TableBorderStrong]     = BORDER_HI;
    c[ImGuiCol_TableBorderLight]      = BORDER;
    c[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_TableRowBgAlt]         = Alpha(ACCENT, 0.05f);

    c[ImGuiCol_TextSelectedBg]        = Alpha(ACCENT, 0.40f);
    c[ImGuiCol_DragDropTarget]        = WARN;

    c[ImGuiCol_NavHighlight]          = ACCENT;
    c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.7f);
    c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0, 0, 0, 0.5f);
    c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0, 0, 0, 0.5f);
}

/* =========================================================
 *  Dot — 컬러 점 + 가로 공간 차지
 * ========================================================= */
void Dot(const ImVec4& color, float radius)
{
    ImVec2 p  = ImGui::GetCursorScreenPos();
    float  lh = ImGui::GetTextLineHeight();
    auto*  dl = ImGui::GetWindowDrawList();
    float  cy = p.y + lh * 0.5f;
    dl->AddCircleFilled(ImVec2(p.x + radius + 1, cy), radius, U32(color));
    /* 빛 효과 (옵션) — 약한 글로우 */
    dl->AddCircleFilled(ImVec2(p.x + radius + 1, cy), radius + 2.5f,
                        U32(Alpha(color, 0.20f)));
    ImGui::Dummy(ImVec2(radius * 2 + 4, lh));
}

/* =========================================================
 *  StatusPill — "● GS100"  (가로 흐름에 들어감)
 * ========================================================= */
void StatusPill(const char* label, bool ok)
{
    Dot(ok ? OK : ERR);
    ImGui::SameLine(0.0f, 4.0f);
    ImGui::TextUnformatted(label);
}

/* =========================================================
 *  ModeFlag — "● Engage" (on=true) / "○ Doppler" (on=false)
 * ========================================================= */
void ModeFlag(const char* label, bool on)
{
    ImGui::PushStyleColor(ImGuiCol_Text, on ? OK : TEXT_DIM);
    ImGui::Text("%s %s", on ? "\xE2\x97\x8F" : "\xE2\x97\x8B", label);
    /* UTF-8: ● = E2 97 8F, ○ = E2 97 8B */
    ImGui::PopStyleColor();
}

/* =========================================================
 *  Badge — 캡슐 모양 배지
 * ========================================================= */
void Badge(const char* text, const ImVec4& bg, const ImVec4& fg)
{
    ImVec2 sz   = ImGui::CalcTextSize(text);
    ImVec2 p    = ImGui::GetCursorScreenPos();
    auto*  dl   = ImGui::GetWindowDrawList();
    float  padX = 8.0f, padY = 2.0f;
    float  w    = sz.x + padX * 2;
    float  h    = sz.y + padY * 2;

    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), U32(bg), h * 0.5f);

    ImGui::PushStyleColor(ImGuiCol_Text, fg);
    ImGui::SetCursorScreenPos(ImVec2(p.x + padX, p.y + padY));
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();

    /* 배지 외곽 공간 확보 */
    ImGui::SetCursorScreenPos(p);
    ImGui::Dummy(ImVec2(w, h));
}

/* =========================================================
 *  SectionHeader — 섹션 구분
 * ========================================================= */
void SectionHeader(const char* label)
{
    SpacerY(4);
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Separator, BORDER);
    ImGui::Separator();
    ImGui::PopStyleColor();
    SpacerY(2);
}

/* =========================================================
 *  KeyboardHint — 단축키 라벨 "[F2]"
 * ========================================================= */
void KeyboardHint(const char* keys)
{
    ImVec2 sz   = ImGui::CalcTextSize(keys);
    ImVec2 p    = ImGui::GetCursorScreenPos();
    auto*  dl   = ImGui::GetWindowDrawList();
    float  padX = 5.0f, padY = 1.0f;
    float  w    = sz.x + padX * 2;
    float  h    = sz.y + padY * 2;

    dl->AddRect(p, ImVec2(p.x + w, p.y + h), U32(BORDER), 3.0f);

    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_MUTE);
    ImGui::SetCursorScreenPos(ImVec2(p.x + padX, p.y + padY));
    ImGui::TextUnformatted(keys);
    ImGui::PopStyleColor();

    ImGui::SetCursorScreenPos(p);
    ImGui::Dummy(ImVec2(w, h));
}

/* =========================================================
 *  Toggle — 토글 스위치
 *  Checkbox 자리 대체 가능. v가 nullable이면 표시만.
 * ========================================================= */
bool Toggle(const char* label, bool* v)
{
    ImGui::PushID(label);

    bool   on   = (v && *v);
    float  h    = ImGui::GetFrameHeight();
    float  w    = h * 1.8f;
    ImVec2 p    = ImGui::GetCursorScreenPos();

    bool clicked = ImGui::InvisibleButton("##sw", ImVec2(w, h));
    if (clicked && v) { *v = !*v; on = *v; }

    auto*  dl   = ImGui::GetWindowDrawList();
    ImU32  bg   = on ? U32(Alpha(OK, 0.35f))   : IM_COL32(58, 77, 82, 255);
    ImU32  knob = on ? U32(OK)                  : IM_COL32(124, 104, 99, 255);
    float  cy   = p.y + h * 0.5f;
    float  cx   = on ? (p.x + w - h * 0.5f) : (p.x + h * 0.5f);

    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), bg, h * 0.5f);
    dl->AddCircleFilled(ImVec2(cx, cy), h * 0.36f, knob);

    ImGui::SameLine(0.0f, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, on ? OK : TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();

    ImGui::PopID();
    return clicked;
}

/* =========================================================
 *  PillButton — Subsystem 선택용 캡슐 버튼
 * ========================================================= */
bool PillButton(const char* label, bool selected, const ImVec4& active_col)
{
    ImGui::PushID(label);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(11, 3));

    if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button,        active_col);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active_col);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  active_col);
        ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.06f, 0.16f, 0.19f, 1));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button,        BG_PANEL2);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Alpha(active_col, 0.35f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  Alpha(active_col, 0.55f));
        ImGui::PushStyleColor(ImGuiCol_Text,          TEXT_DIM);
    }

    bool clicked = ImGui::SmallButton(label);

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
    ImGui::PopID();
    return clicked;
}

/* =========================================================
 *  BigReadout — 큰 숫자 카드
 * ========================================================= */
void BigReadout(const char* label, const char* value,
                const char* sub, const ImVec4& value_color)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_PANEL2);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

    float h = ImGui::GetTextLineHeight() * (sub ? 4.4f : 3.4f);
    ImGui::BeginChild(label, ImVec2(0, h), true,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    /* 상단 작은 라벨 */
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    float tw = ImGui::CalcTextSize(label).x;
    float aw = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (aw - tw) * 0.5f);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();

    /* 큰 숫자 (font scale 1.7배) */
    ImGui::SetWindowFontScale(1.7f);
    ImGui::PushStyleColor(ImGuiCol_Text, value_color);
    float vw = ImGui::CalcTextSize(value).x;
    aw = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (aw - vw) * 0.5f);
    ImGui::TextUnformatted(value);
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);

    /* 보조 라인 */
    if (sub) {
        ImGui::PushStyleColor(ImGuiCol_Text, TEXT_MUTE);
        float sw = ImGui::CalcTextSize(sub).x;
        aw = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (aw - sw) * 0.5f);
        ImGui::TextUnformatted(sub);
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

/* =========================================================
 *  DangerButton / PrimaryButton
 * ========================================================= */
bool DangerButton(const char* label, const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_Button,        DANGER);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, DANGER_HOV);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  DANGER_HOV);
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1, 1, 1, 1));
    bool r = ImGui::Button(label, size);
    ImGui::PopStyleColor(4);
    return r;
}

bool PrimaryButton(const char* label, const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_Button,        ACCENT);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Alpha(ACCENT, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ACCENT);
    ImGui::PushStyleColor(ImGuiCol_Text,          TEXT_HI);
    bool r = ImGui::Button(label, size);
    ImGui::PopStyleColor(4);
    return r;
}

/* =========================================================
 *  CtrlGuardedButton — Ctrl 누른 상태에서만 활성
 * ========================================================= */
bool CtrlGuardedButton(const char* label, bool hint_below, const ImVec2& size)
{
    bool armed = ImGui::GetIO().KeyCtrl;
    bool clicked = false;

    // ImGui 1.85+ 의 BeginDisabled/EndDisabled 미지원 환경 호환
    if (!armed) {
    // 알파 절반 + 클릭 무시
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
        ImGui::GetStyle().Alpha * 0.5f);
    }
    ImGui::PushStyleColor(ImGuiCol_Button,
        armed ? DANGER : ImVec4(0.35f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, DANGER_HOV);

    bool button_clicked = ImGui::Button(label, size);
    clicked = armed && button_clicked;   // armed가 아니면 클릭 무시

    ImGui::PopStyleColor(2);
    if (!armed) {
        ImGui::PopStyleVar();
    }

    if (hint_below) {
        ImGui::PushStyleColor(ImGuiCol_Text, armed ? OK : WARN);
        if (armed)
            ImGui::TextUnformatted("\xE2\x9A\xA0 Ctrl held — click to execute");
        else
            ImGui::TextUnformatted("\xE2\x9A\xA0 Hold Ctrl to enable");
        ImGui::PopStyleColor();
    }
    return clicked;
}

/* =========================================================
 *  KVRow / KVRowf
 * ========================================================= */
void KVRow(const char* label, const char* value, const ImVec4& value_col)
{
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    ImGui::SameLine();
    /* 우측 정렬 */
    float vw = ImGui::CalcTextSize(value).x;
    float aw = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (aw - vw));
    ImGui::PushStyleColor(ImGuiCol_Text, value_col);
    ImGui::TextUnformatted(value);
    ImGui::PopStyleColor();
}

void KVRowf(const char* label, const char* fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    KVRow(label, buf);
}

} // namespace mim
