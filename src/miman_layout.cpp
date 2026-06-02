/* ============================================================
 *  miman_layout.cpp
 *
 *  Phase 3 — 새 레이아웃 구현.
 *  ------------------------------------------------------------
 *  • 위성 탭 (최상위)
 *  • 상태바
 *  • 기능 탭 (Pre-Pass / Pass Operation)
 *  • 3컬럼 워크스페이스
 *  • 콘솔 (하단 고정)
 * ============================================================ */
#include "miman_layout.h"
#include "miman_imgui.h"
#include "miman_theme.h"
#include "miman_satcontext.h"
#include "miman_config.h"

#include <imgui.h>
#include <cstdio>

/* extern globals (기존 miman_imgui.cpp 에 정의됨) */
extern StateCheckUnit         State;
extern Console                console;
extern ImGuiWindowFlags       mim_winflags;
extern bool                   p_open;

/* miman_orbital.cpp 의 전역 */
extern int GetNowTracking(void);

/* 콘솔에서 사용되는 변수 (기존 main.cpp에서 정의) */
extern bool Showstate;

using namespace mim;

/* =========================================================
 *  Helper — 위성 상태 문자열
 *  "● Tracking" / "AOS in HH:MM" 등
 * ========================================================= */
static void format_sat_status(int sat_idx, char* out, size_t cap)
{
    if (sat_idx < 0 || sat_idx >= SAT_MAX_NUM ||
        !State.Satellites[sat_idx]) {
        snprintf(out, cap, "—");
        return;
    }

    bool tracking_this = (sat_idx == GetNowTracking()) && State.Engage;
    if (tracking_this) {
        snprintf(out, cap, "\xE2\x97\x8F Tracking");   /* ● */
        return;
    }

    /* 다음 AOS까지 시간 */
    try {
        DateTime now = DateTime::Now(false);
        double sec   = (State.Satellites[sat_idx]->_nextaos[0] - now).TotalSeconds();
        if (sec < 0) {
            snprintf(out, cap, "now in pass");
        } else if (sec > 3600 * 99) {
            snprintf(out, cap, "—");
        } else {
            int hh = (int)(sec / 3600);
            int mm = (int)((sec - hh * 3600) / 60);
            snprintf(out, cap, "AOS in %02d:%02d", hh, mm);
        }
    } catch (...) {
        snprintf(out, cap, "—");
    }
}

/* =========================================================
 *  ImGui_SatelliteTabs — 최상위 위성 탭
 * ========================================================= */
void ImGui_SatelliteTabs(float W)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_SATTAB);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(2, 0));

    ImGui::BeginChild("##sattabs_inner", ImVec2(W, 0), false,
                      ImGuiWindowFlags_NoScrollbar |
                      ImGuiWindowFlags_NoScrollWithMouse);

    int n      = GetVisibleSatCount();
    int active = GetActiveSat();

    for (int slot = 0; slot < n; slot++) {
        int sat = GetVisibleSatAt(slot);
        if (sat < 0 || !State.Satellites[sat]) continue;

        bool   is_active = (sat == active);
        ImVec4 sat_col   = GetVisibleSatColor(slot);

        ImGui::PushID(slot);

        /* 탭 배경 */
        ImVec4 tab_bg = is_active ? BG_DEEP
                                  : ImVec4(0.094f, 0.220f, 0.255f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Button,        tab_bg);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BG_MAIN);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  tab_bg);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        const ImVec2 btn_size(200, 44);
        if (ImGui::Button("##sattab", btn_size)) {
            SetActiveSat(sat);
        }
        bool hovered = ImGui::IsItemHovered();

        /* 그 위에 컨텐츠 그리기 */
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        auto*  dl    = ImGui::GetWindowDrawList();

        /* 활성 탭 상단 컬러 라인 (3px) */
        if (is_active) {
            dl->AddRectFilled(p_min,
                ImVec2(p_max.x, p_min.y + 3),
                U32(sat_col), 2.0f, ImDrawFlags_RoundCornersTop);
        }

        /* 컬러 dot + glow */
        float dot_cx = p_min.x + 14;
        float dot_cy = p_min.y + 14;
        dl->AddCircleFilled(ImVec2(dot_cx, dot_cy), 7, U32(Alpha(sat_col, 0.25f)));
        dl->AddCircleFilled(ImVec2(dot_cx, dot_cy), 4.5f, U32(sat_col));

        /* 위성명 */
        const char* name = State.Satellites[sat]->Name();
        dl->AddText(NULL, 14,
            ImVec2(p_min.x + 30, p_min.y + 6),
            U32(is_active ? TEXT_HI : TEXT),
            name);

        /* 상태 */
        char status[64];
        format_sat_status(sat, status, sizeof(status));
        dl->AddText(NULL, 11,
            ImVec2(p_min.x + 30, p_min.y + 25),
            U32(is_active ? TEXT_DIM : TEXT_MUTE),
            status);

        /* 닫기 버튼 (호버 시) */
        if (hovered && n > 1) {
            ImVec2 close_min(p_max.x - 20, p_min.y + 6);
            ImVec2 close_max(p_max.x - 6,  p_min.y + 20);
            bool close_hovered = ImGui::IsMouseHoveringRect(close_min, close_max);

            if (close_hovered) {
                dl->AddRectFilled(close_min, close_max,
                    U32(Alpha(ERR, 0.30f)), 3.0f);
            }
            dl->AddText(NULL, 12,
                ImVec2(close_min.x + 3, close_min.y - 1),
                U32(close_hovered ? ERR : TEXT_MUTE),
                "x");

            if (close_hovered && ImGui::IsMouseClicked(0)) {
                RemoveVisibleSat(slot);
            }
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::SameLine(0.0f, 2.0f);
    }

    /* + Add Satellite */
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Alpha(BG_MAIN, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text,          TEXT_DIM);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

    if (ImGui::Button(" + Add Satellite ", ImVec2(0, 44))) {
        State.Display_TLE = true;
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

/* =========================================================
 *  ImGui_StatusBar — 영구 상태바
 * ========================================================= */
void ImGui_StatusBar(float W)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.086f, 0.220f, 0.259f, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(14, 0));

    ImGui::BeginChild("##statusbar_inner", ImVec2(W, 0), false,
                      ImGuiWindowFlags_NoScrollbar);

    /* 연결 상태 4개 */
    StatusPill("GS100",   State.GS100_Connection);   ImGui::SameLine();
    StatusPill("Rotator", State.RotatorConnection);  ImGui::SameLine();
    StatusPill("Switch",  State.SwitchConnection);   ImGui::SameLine();
    StatusPill("S-band",  State.SbandUse);

    /* 구분선 */
    ImGui::SameLine();
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        float  h = ImGui::GetTextLineHeight();
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(p.x, p.y + 2), ImVec2(p.x, p.y + h - 2),
            U32(BORDER));
        ImGui::Dummy(ImVec2(2, h));
    }
    ImGui::SameLine();

    /* 추적 위성 */
    int sat = GetActiveSat();
    if (sat >= 0 && sat < SAT_MAX_NUM && State.Satellites[sat]) {
        ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
        ImGui::TextUnformatted("Track:");
        ImGui::PopStyleColor();
        ImGui::SameLine(0, 6);

        int slot = FindVisibleSlot(sat);
        ImVec4 col = (slot >= 0) ? GetVisibleSatColor(slot) : ACCENT;
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::Text("\xE2\x97\x8F %s", State.Satellites[sat]->Name());
        ImGui::PopStyleColor();
    }

    /* 우측: 활성 모드 */
    ImGui::SameLine();
    float right_x = W - 380;
    if (ImGui::GetCursorPosX() < right_x)
        ImGui::SetCursorPosX(right_x);

    ModeFlag("Engage",    State.Engage);     ImGui::SameLine();
    ModeFlag("Doppler",   State.Doppler);    ImGui::SameLine();
    ModeFlag("AutoPilot", State.Autopilot);  ImGui::SameLine();
    ModeFlag("AMP",       State.AMPON);

    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

/* =========================================================
 *  Helper — 3컬럼 워크스페이스 (Pre-Pass / Pass Operation 공통)
 * ========================================================= */
static void DrawThreeColumns(const char* id,
                             void (*left_body)(float),
                             void (*center_body)(float),
                             void (*right_body)(float),
                             float fontscale)
{
    float aw  = ImGui::GetContentRegionAvail().x;
    float ah  = ImGui::GetContentRegionAvail().y;
    float gap = 10.0f;
    /* 비율: 좌 1 / 중 1.2 / 우 1 → 합 3.2 */
    float colW_unit = (aw - gap * 2) / 3.2f;
    float col1 = colW_unit;
    float col2 = colW_unit * 1.2f;
    float col3 = colW_unit;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_MAIN);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

    /* Left */
    ImGui::PushID(id);
    ImGui::BeginChild("##col_l", ImVec2(col1, ah), true);
    if (left_body) left_body(fontscale);
    ImGui::EndChild();
    ImGui::SameLine(0, gap);

    /* Center */
    ImGui::BeginChild("##col_c", ImVec2(col2, ah), true);
    if (center_body) center_body(fontscale);
    ImGui::EndChild();
    ImGui::SameLine(0, gap);

    /* Right */
    ImGui::BeginChild("##col_r", ImVec2(col3, ah), true);
    if (right_body) right_body(fontscale);
    ImGui::EndChild();
    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

/* =========================================================
 *  ImGui_FunctionTabs — 기능 탭 + 워크스페이스
 * ========================================================= */
void ImGui_FunctionTabs(float W, float H)
{
    ImGui::PushStyleColor(ImGuiCol_Tab,         ACCENT_2);
    ImGui::PushStyleColor(ImGuiCol_TabHovered,  ACCENT);
    ImGui::PushStyleColor(ImGuiCol_TabActive,   ACCENT);
    ImGui::PushStyleColor(ImGuiCol_TabUnfocused,        BG_PANEL2);
    ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive,  ACCENT_2);

    if (ImGui::BeginTabBar("##functabs",
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {

        /* ───── ① Pre-Pass ───── */
        if (ImGui::BeginTabItem("\xE2\x91\xA0 Pre-Pass")) {
            State.ActiveFuncTab = 0;
            DrawThreeColumns("pre",
                &ImGui_TrackWindow_Body,
                &ImGui_FrequencyWindow_Body,
                &ImGui_RotatorWindow_Body,
                /*fontscale*/ 0.5f);
            ImGui::EndTabItem();
        }

        /* ───── ② Pass Operation ───── */
        if (ImGui::BeginTabItem("\xE2\x91\xA1 Pass Operation")) {
            State.ActiveFuncTab = 1;
            DrawThreeColumns("pass",
                &ImGui_ControlWindow_Body,
                &ImGui_CommandWindow_Body,
                &ImGui_BeaconWindow_Body,
                /*fontscale*/ 0.6f);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::PopStyleColor(5);
}

/* =========================================================
 *  ImGui_MainLayout — 전체 진입점
 * ========================================================= */
void ImGui_MainLayout(float W, float H)
{
    float top         = MENUPADDING;
    float sattabs_h   = MIM_SATTAB_H;
    float status_h    = MIM_STATUSBAR_H;
    float console_h   = MIM_CONSOLE_H;
    float workspace_h = H - top - sattabs_h - status_h - console_h;
    if (workspace_h < 200) workspace_h = 200;

    /* ───── 1. 위성 탭 ───── */
    ImGui::SetNextWindowPos (ImVec2(0, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(W, sattabs_h), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##host_sattabs", &p_open, mim_winflags);
    ImGui_SatelliteTabs(W);
    ImGui::End();
    ImGui::PopStyleVar();
    top += sattabs_h;

    /* ───── 2. 상태바 ───── */
    ImGui::SetNextWindowPos (ImVec2(0, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(W, status_h), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##host_statusbar", &p_open, mim_winflags);
    ImGui_StatusBar(W);
    ImGui::End();
    ImGui::PopStyleVar();
    top += status_h;

    /* ───── 3. 기능 탭 + 워크스페이스 ───── */
    ImGui::SetNextWindowPos (ImVec2(0, top), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(W, workspace_h), ImGuiCond_Always);
    ImGui::Begin("##host_workspace", &p_open, mim_winflags);
    ImGui_FunctionTabs(W, workspace_h);
    ImGui::End();
    top += workspace_h;

    /* ───── 4. 콘솔 (탭 무관, 하단 고정) ───── */
    console.ChangeWindowSize(0, top, W, console_h);
    console.Draw("General Console", &Showstate, 0.6f);
}
