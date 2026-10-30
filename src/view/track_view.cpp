// View - Track 창 (Pre-Pass 탭 왼쪽 열: 위성 패스 표 + 극좌표 표시)
//
// ImGui 호출과 TrackVM 호출만 있다. 상태와 동작은 viewmodel/track_vm.h 참고.
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "track_vm.h"
#include "track_view.h"

extern bool p_open;
extern ImGuiWindowFlags mim_winflags;

void ImGui_TrackWindow_Body(float fontscale)
{
    mim::vm::TrackVM & vm = mim::vm::TrackViewModel();

    float scale = fontscale * 1.2f;
    ImGui::SetWindowFontScale(scale);
    vm.BeginFrame();

    float totalW = ImGui::GetContentRegionAvail().x;
    float totalH = ImGui::GetContentRegionAvail().y;
    float topH   = totalH * 0.50f;
    float botH   = totalH - topH - ImGui::GetStyle().ItemSpacing.y;

    // ════════════════════════════════════════════
    // TOP PANEL – 검색창 + 테이블
    // ════════════════════════════════════════════
    ImGui::BeginChild("##top_panel", ImVec2(totalW, topH), false,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetWindowFontScale(scale);

    // ── 검색창 + TLE 버튼 ──
    float btnW = ImGui::CalcTextSize("TLE").x + ImGui::GetStyle().FramePadding.x * 4;
    ImGui::SetNextItemWidth(totalW - btnW - ImGui::GetStyle().ItemSpacing.x * 2);
    ImGui::InputTextWithHint("##search", "Search satellites...", vm.SearchBuffer(), vm.SearchBufferSize());
    ImGui::SameLine();
    if (ImGui::Button("TLE", ImVec2(btnW, 0)))
    {
        // TLE 업데이트
    }

    ImGui::Spacing();

    float searchBarH = ImGui::GetFrameHeightWithSpacing()
                     + ImGui::GetStyle().ItemSpacing.y * 2;
    float tableH     = topH - searchBarH;
    if (tableH < 50.f) tableH = 50.f;

    // ── 테이블 ──
    if (ImGui::BeginTable("##SatelliteListup", 6,
        ImGuiTableFlags_ScrollY       |
        ImGuiTableFlags_RowBg         |
        ImGuiTableFlags_Borders       |
        ImGuiTableFlags_Resizable     |
        ImGuiTableFlags_SizingStretchProp,
        ImVec2(ImGui::GetContentRegionAvail().x, tableH)))
    {

        auto CenterText = [](const char* fmt, ...) {
            char buf[128];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            float colW = ImGui::GetColumnWidth();
            float textW = ImGui::CalcTextSize(buf).x;
            if (textW < colW)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - textW) * 0.5f);
            ImGui::TextUnformatted(buf);
        };

        ImGui::SetWindowFontScale(scale);

        ImGui::TableSetupColumn("#",        ImGuiTableColumnFlags_WidthFixed,   24.f);
        ImGui::TableSetupColumn("NAME",     ImGuiTableColumnFlags_WidthFixed,   150.f);
        ImGui::TableSetupColumn("NEXT AOS", ImGuiTableColumnFlags_WidthFixed,   200.f);
        ImGui::TableSetupColumn("NEXT LOS", ImGuiTableColumnFlags_WidthFixed,   200.f);
        ImGui::TableSetupColumn("MAX EL",   ImGuiTableColumnFlags_WidthFixed,   100.f);
        ImGui::TableSetupColumn("##Track",  ImGuiTableColumnFlags_WidthFixed,    55.f);
        ImGui::TableSetupScrollFreeze(0, 1);

        // 커스텀 헤더
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        const char* headers[] = {"#", "NAME", "NEXT AOS", "NEXT LOS", "MAX EL", ""};
        for (int col = 0; col < 6; col++)
        {
            ImGui::TableSetColumnIndex(col);
            ImGui::TableHeader(headers[col]);
        }

        // ── 예시 데이터 20개 출력 ──
        for (int i = 0; i < vm.RowCount(); i++)
        {
            const mim::vm::TrackRow& d = vm.Row(i);

            // 검색 필터
            if (vm.HiddenBySearch(i)) continue;

            bool isSelected = vm.IsSelected(i);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextDisabled("%02d", i + 1);

            ImGui::TableNextColumn();
            char selLabel[64];
            sprintf(selLabel, "%s##sel%d", d.name, i);
            if (ImGui::Selectable(selLabel, isSelected,
                ImGuiSelectableFlags_SpanAllColumns |
                ImGuiSelectableFlags_AllowItemOverlap,
                ImVec2(0, ImGui::GetFontSize() * 1.3f)))
            {
                vm.Select(i);
            }

            ImGui::TableNextColumn();
            ImGui::Text("6/3 %02d:%02d:%02d", d.aos_h, d.aos_m, d.aos_s);

            ImGui::TableNextColumn();
            ImGui::Text("6/3 %02d:%02d:%02d", d.los_h, d.los_m, d.los_s);

            ImGui::TableNextColumn();
            ImGui::Text("%.0f°", d.max_el);

            // ── Track 버튼 ──
            ImGui::TableNextColumn();
            bool isTracking = vm.IsTracking(i);
            if (isTracking)
            {
                ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.1f,0.55f,0.25f,1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f,0.65f,0.3f,1.f));
                char onLabel[32];
                sprintf(onLabel, "ON##trk%d", i);
                ImGui::Button(onLabel, ImVec2(ImGui::GetContentRegionAvail().x,
                                              ImGui::GetFontSize() * 1.3f));
                ImGui::PopStyleColor(2);
            }
            else
            {
                char trkLabel[32];
                sprintf(trkLabel, "Track##trk%d", i);
                if (ImGui::Button(trkLabel, ImVec2(ImGui::GetContentRegionAvail().x,
                                                   ImGui::GetFontSize() * 1.3f)))
                    vm.Track(i);
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndChild(); // top panel

    // ════════════════════════════════════════════
    // BOTTOM PANEL
    // ════════════════════════════════════════════
    ImGui::BeginChild("##bottom_panel", ImVec2(totalW, botH), false,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetWindowFontScale(scale);

    // 선택된 위성 이름 - Track 버튼 기준 (추적 대상)
    const char* selName = vm.TrackedName();

    float dummyAz = vm.TrackedAzimuth();
    float dummyEl = vm.TrackedElevation();

    // ── 구분선 + SELECTED ──
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1.f), "SELECTED");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.35f,0.65f,1.f,1.f), selName ? selName : "—");

    ImGui::Spacing();

    // ── 레이아웃: 좌 2/3 (극좌표계 영역) + 우 1/3 (수치) ──
    float headerUsed = ImGui::GetCursorPosY();
    float availH     = botH - headerUsed - ImGui::GetStyle().ItemSpacing.y * 2;
    float leftW      = totalW * (2.f / 3.f);  // 좌 2/3
    float rightW     = totalW - leftW;         // 우 1/3

    // 극좌표계 크기: 좌 영역 안에서 정사각형, 상하좌우 여백 포함
    float polarSize  = ImMin(leftW * 0.75f, availH * 0.90f);
    float polarPadX  = (leftW - polarSize) * 0.5f;
    float polarPadY  = (availH - polarSize) * 0.5f;

    // ── 좌 영역: 회색 테두리 빈 공간 + 그 안에 극좌표계 ──
    ImVec2 leftAreaPos = ImGui::GetCursorScreenPos();
    ImDrawList* draw   = ImGui::GetWindowDrawList();

    // 회색 실선 테두리 박스
    draw->AddRect(
        leftAreaPos,
        ImVec2(leftAreaPos.x + leftW, leftAreaPos.y + availH),
        IM_COL32(120, 120, 120, 180), 4.f, 0, 1.5f);

    // 극좌표계 위치 (테두리 박스 안 중앙)
    ImVec2 polarPos = ImVec2(leftAreaPos.x + polarPadX, leftAreaPos.y + polarPadY);
    ImVec2 ctr      = ImVec2(polarPos.x + polarSize * 0.5f,
                             polarPos.y + polarSize * 0.5f);
    float  R        = polarSize * 0.42f;

    // 동심원
    const char* rlbl[] = {"0°","30°","60°"};
    for (int r = 1; r <= 3; r++) {
        float rad = R * (float)(4-r) / 3.f;
        draw->AddCircle(ctr, rad, IM_COL32(0,0,0,200), 64, 1.f);
        draw->AddText(ImVec2(ctr.x + 3, ctr.y - rad - ImGui::GetFontSize() - 1),
                      IM_COL32(0,0,0,200), rlbl[r-1]);
    }

    // 십자선
    draw->AddLine(ImVec2(ctr.x, ctr.y-R), ImVec2(ctr.x, ctr.y+R),
                  IM_COL32(0,0,0,200));
    draw->AddLine(ImVec2(ctr.x-R, ctr.y), ImVec2(ctr.x+R, ctr.y),
                  IM_COL32(0,0,0,200));

    // N/E/S/W
    float lo = R + 6.f;
    float fh = ImGui::GetFontSize();
    draw->AddText(ImVec2(ctr.x - fh*0.3f, ctr.y - lo - fh), IM_COL32(0,0,0,200), "N");
    draw->AddText(ImVec2(ctr.x + lo,      ctr.y - fh*0.5f), IM_COL32(0,0,0,200), "E");
    draw->AddText(ImVec2(ctr.x - fh*0.3f, ctr.y + lo),      IM_COL32(0,0,0,200), "S");
    draw->AddText(ImVec2(ctr.x - lo - fh, ctr.y - fh*0.5f), IM_COL32(0,0,0,200), "W");

    // 위성 점
    if (vm.HasTrackTarget()) {
        float azRad = dummyAz * 3.14159f / 180.f;
        float sr    = R * (1.f - ImClamp(dummyEl, 0.f, 90.f) / 90.f);
        float sx    = ctr.x + sr * sinf(azRad);
        float sy    = ctr.y - sr * cosf(azRad);
        draw->AddCircleFilled(ImVec2(sx,sy), 5.f, IM_COL32(50,220,100,255));
        draw->AddCircle(      ImVec2(sx,sy), 8.f, IM_COL32(50,220,100,160), 20, 1.5f);
    }

    // 좌 영역 InvisibleButton으로 공간 차지
    ImGui::InvisibleButton("##leftarea", ImVec2(leftW, availH));

    // ── 우 1/3 영역: Azimuth / Elevation ──
    ImGui::SameLine();
    ImGui::BeginChild("##rightinfo", ImVec2(rightW, availH), false,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetWindowFontScale(scale);
    {
        // 세로 중앙 정렬
        float lineH  = ImGui::GetFrameHeightWithSpacing();
        float groupH = lineH * 2;
        float padY2  = (availH - groupH) * 0.5f;
        if (padY2 > 0) ImGui::Dummy(ImVec2(0, padY2));

        char buf[64];
        float rw = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().WindowPadding.x;

        sprintf(buf, "%.1f°", dummyAz);
        ImGui::TextColored(ImVec4(0.f,0.f,0.f,1.f), "Azimuth");
        ImGui::SameLine(rw - ImGui::CalcTextSize(buf).x);
        ImGui::TextColored(ImVec4(0.f,0.f,0.f,1.f), "%s", buf);

        sprintf(buf, "%.1f°", dummyEl);
        ImGui::TextColored(ImVec4(0.f,0.f,0.f,1.f), "Elevation");
        ImGui::SameLine(rw - ImGui::CalcTextSize(buf).x);
        ImGui::TextColored(ImVec4(0.f,0.f,0.f,1.f), "%s", buf);
    }
    ImGui::EndChild();

    ImGui::EndChild(); // bottom panel
}


void ImGui_TrackWindow(float fontscale)
{
    ImGui::Begin("Track Window", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    ImGui_TrackWindow_Body(fontscale);   // 본체는 여기서 호출
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}
