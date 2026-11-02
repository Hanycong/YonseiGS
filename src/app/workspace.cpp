// App - 메인 작업 공간 배치: 위성 탭, 상태 표시줄, 기능 탭(Pre-Pass / Pass Operation)의 3열
//
// YonseiGS_imgui.cpp 에서 내용 그대로 옮긴 것. 각 열은 view/ 의 창 Body 함수를 부른다.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "YonseiGS_config.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_model.h"
#include "YonseiGS_s_control.h"
#include "YonseiGS_ftp.h"
#include "YonseiGS_autopilot.h"

#include "YonseiGS_theme.h"
#include "YonseiGS_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "app_common.h"
#include "ui_kit.h"

using namespace mim;

void ImGui_SatelliteTabs(float W) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_DEEP);
    ImGui::BeginChild("##sattabs", ImVec2(W, 56), false,
                      ImGuiWindowFlags_NoScrollbar);

    for (int slot = 0; slot < SatTabs.VisibleSatsCount; slot++) {
        int sat = SatTabs.VisibleSats[slot];
        if (sat < 0 || !OrbitData.Satellites[sat]) continue;

        bool active = (sat == SatTabs.ActiveSatIndex);
        ImVec4 col  = GetVisibleSatColor(slot);

        ImGui::PushStyleColor(ImGuiCol_Button,
            active ? BG_DEEP : BG_PANEL2);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BG_MAIN);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  BG_DEEP);

        ImGui::PushID(slot);
        ImVec2 btn_size(200, 46);
        if (ImGui::Button("##sat", btn_size)) SetActiveSat(sat);

        // 버튼 위에 컬러 dot + 위성명 + 상태 두 줄을 직접 그림
        ImVec2 p = ImGui::GetItemRectMin();
        auto* dl = ImGui::GetWindowDrawList();
        if (active) {
            dl->AddRectFilled(p, ImVec2(p.x + btn_size.x, p.y + 3),
                              ImGui::ColorConvertFloat4ToU32(col));
        }
        dl->AddCircleFilled(ImVec2(p.x + 14, p.y + 14), 5,
                            ImGui::ColorConvertFloat4ToU32(col));

        // dl->AddText(NULL, 14, ImVec2(p.x + 28, p.y + 8),
        //             ImGui::ColorConvertFloat4ToU32(active ? TEXT_HI : TEXT),
        //             State.Satellites[sat]->Name());

        // char status[64];
        // if (sat == GetNowTracking() && State.Engage) {
        //     snprintf(status, sizeof(status), "● Tracking");
        // } else {
        //     // 다음 AOS까지 시간 계산
        //     DateTime now = DateTime::Now(false);
        //     double sec = (State.Satellites[sat]->_nextaos[0] - now).TotalSeconds();
        //     int hh = (int)(sec/3600), mm = (int)((sec - hh*3600)/60);
        //     snprintf(status, sizeof(status), "AOS in %02d:%02d", hh, mm);
        // }
        // dl->AddText(NULL, 11, ImVec2(p.x + 28, p.y + 26),
        //             ImGui::ColorConvertFloat4ToU32(TEXT_DIM), status);

        // 위성 이름 — DroidSans-Bold 18pt (네이티브 42pt를 다운스케일이지만 bold 유지)
        dl->AddText(mim::font_title, 18.0f, ImVec2(p.x + 24, p.y + 6),
                    ImGui::ColorConvertFloat4ToU32(active ? TEXT_HI : TEXT),
                    OrbitData.Satellites[sat]->Name());

        char status[64];
        if (sat == GetNowTracking() && RotatorFlags.Engage) {
            snprintf(status, sizeof(status), "TRACKING");
        } else {
            DateTime now = DateTime::Now(false);
            double sec = (OrbitData.Satellites[sat]->_nextaos[0] - now).TotalSeconds();
            if (sec < 0) sec = 0;                          // CLAUDE.md known defect 가드
            int hh = (int)(sec / 3600);
            int mm = (int)((sec - hh * 3600) / 60);
            int ss = (int)(sec - hh * 3600 - mm * 60);
            snprintf(status, sizeof(status), "NEXT AOS IN %02d:%02d:%02d", hh, mm, ss);
        }
        // 부제 — Inconsolata 11pt (네이티브 20pt 축소)
        dl->AddText(mim::font_subtitle, 11.0f, ImVec2(p.x + 14, p.y + 30),
                    ImGui::ColorConvertFloat4ToU32(TEXT_DIM), status);

        ImGui::PopID();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
    }

    // [+ Add] 버튼
    if (ImGui::Button(" + Add Satellite ", ImVec2(0, 46))) {
        AppUi.Display_TLE = true;   // 기존 popup 재활용
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void ImGui_StatusBar_Body(float W) {
    // 장비 연결 상태와 운용 모드를 같은 점등 규칙으로 표시: 녹색 = 연결/동작 중, 적색 = 끊김/정지
    const float gap = ImGui::GetFontSize() * 1.1f;
    mim::ui::StatusLamp("GS100",   LinkFlags.GS100_Connection);    ImGui::SameLine(0, gap);
    mim::ui::StatusLamp("Rotator", RotatorFlags.RotatorConnection); ImGui::SameLine(0, gap);
    mim::ui::StatusLamp("Switch",  LinkFlags.SwitchConnection);    ImGui::SameLine(0, gap);
    mim::ui::StatusLamp("S-band",  State.SbandUse);

    int sat = SatTabs.ActiveSatIndex;
    if (sat >= 0 && OrbitData.Satellites[sat]) {
        ImGui::SameLine(0, gap * 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
        ImGui::TextUnformatted("Track");
        ImGui::PopStyleColor();
        ImGui::SameLine(0, 6);
        Dot(GetVisibleSatColor(sat));
        ImGui::SameLine(0, 4);
        ImGui::PushStyleColor(ImGuiCol_Text, GetVisibleSatColor(sat));
        const char * nm = OrbitData.Satellites[sat]->Name();
        ImGui::Text("%s", (nm && nm[0]) ? nm : "(no TLE)");
        ImGui::PopStyleColor();
    }

    // 운용 모드는 오른쪽 정렬
    const char * modes[] = {"Engage", "Doppler", "AutoPilot", "AMP"};
    bool on[] = {RotatorFlags.Engage, LinkFlags.Doppler, AutoPilotCfg.Autopilot, LinkFlags.AMPON};
    float lampW = ImGui::GetFontSize() * 0.44f + ImGui::GetFontSize() * 0.22f * 1.6f;
    float groupW = 0;
    for (int i = 0; i < 4; i++)
        groupW += lampW + ImGui::CalcTextSize(modes[i]).x + (i ? gap : 0);
    ImGui::SameLine(W - groupW - ImGui::GetFontSize());
    for (int i = 0; i < 4; i++) {
        if (i) ImGui::SameLine(0, gap);
        mim::ui::StatusLamp(modes[i], on[i]);
    }
}

void ImGui_StatusBar(float W) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_MAIN);
    ImGui::BeginChild("##statusbar", ImVec2(W, 30), false,ImGuiWindowFlags_NoScrollbar);
    ImGui_StatusBar_Body(W);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void ImGui_MainWorkspace(float Rw, float Rh) {
    float W = ImGui::GetIO().DisplaySize.x;
    float H = ImGui::GetIO().DisplaySize.y;

    // ───── 레이아웃 비율
    const float menu_h    = H * 0.10f; 
    const float sat_h     = H * 0.07f;
    const float console_h = H * 0.22f;
    // status_h 삭제 — 메뉴바에 흡수

    // ───── 1. 위성 탭
    ImGui::SetNextWindowPos(ImVec2(0, menu_h));
    ImGui::SetNextWindowSize(ImVec2(W, sat_h));
    ImGui::Begin("##sattab_host", NULL,
             mim_winflags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui_SatelliteTabs(W);
    ImGui::End();

    // ───── 2. 기능 탭 + 워크스페이스
    float top = menu_h + sat_h;
    ImGui::SetNextWindowPos(ImVec2(0, top));
    ImGui::SetNextWindowSize(ImVec2(W, H - top - console_h));
    ImGui::Begin("##workspace_host", NULL, mim_winflags);

    // 기능 탭은 화면 전환의 주 진입점이라 굵은 글꼴·넓은 여백으로 크게 그린다
    // (탭 바 높이는 BeginTabBar, 탭 글자는 BeginTabItem 시점의 글꼴을 쓰므로 두 곳 모두 감싼다)
    auto TabStyle = [](bool on) {
        if (on) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 8));
            ImGui::PushFont(mim::font_title);
            ImGui::SetWindowFontScale(0.6f);
        } else {
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopFont();
            ImGui::PopStyleVar();
        }
    };
    TabStyle(true);
    bool functabs = ImGui::BeginTabBar("##functabs", ImGuiTabBarFlags_None);
    TabStyle(false);
    if (functabs) {
        TabStyle(true);
        bool prepass = ImGui::BeginTabItem("1. Pre-Pass");
        TabStyle(false);
        if (prepass) {
            SatTabs.ActiveFuncTab = 0;
            float colW = ImGui::GetContentRegionAvail().x / 3.0f - 8;
            float colH = ImGui::GetContentRegionAvail().y;
            ImGui::BeginChild("##col1", ImVec2(colW, colH), true);
                ImGui_TrackWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col2", ImVec2(colW, colH), true);
                ImGui_FrequencyWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col3", ImVec2(colW, colH), true);
                ImGui_RotatorWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        TabStyle(true);
        bool passop = ImGui::BeginTabItem("2. Pass Operation");
        TabStyle(false);
        if (passop) {
            SatTabs.ActiveFuncTab = 1;
            float colW = ImGui::GetContentRegionAvail().x / 3.0f - 8;
            float colH = ImGui::GetContentRegionAvail().y;
            ImGui::BeginChild("##col1", ImVec2(colW, colH), true);
                ImGui_ControlWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col2", ImVec2(colW, colH), true);
                ImGui_CommandWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col3", ImVec2(colW, colH), true);
                ImGui_BeaconWindow_Body(1.0f);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

    // ───── 4. Console (탭 무관, 하단 고정)
    console.ChangeWindowSize(0, H - console_h, W, console_h);
    console.Draw("General Console", &Showstate, 1.0f);
}
