// App - 메인 메뉴 / 헤더 (브랜드, 메뉴 막대, 상태 표시줄 자리)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "app_common.h"

using namespace mim;

void ImGui_MainMenu()
{

    // 로고 텍스처 로드
    static GLuint logo_tex = 0;
    static bool   logo_loaded = false;
    if (!logo_loaded) {
        logo_loaded = true;
        LoadTextureFromFile("../bin/image/yonsei_logo.png", &logo_tex, 0, 0);
    }

    float W = ImGui::GetIO().DisplaySize.x;
    float H = ImGui::GetIO().DisplaySize.y;

    // ───── 헤더 높이 / 콘텐츠 상수 ─────
    const float header_h  = H * 0.10f;                    // 0.08 → 0.10 (헤더 25% 더 두꺼움)
    const float strip_h   = header_h * 0.5f;
    const float pad       = 12.0f;
    const float gap       = 24.0f;
    const float logo_size = header_h * 0.85f;              // 비율 기반 (= 116 at H=1370)
    const float logo_pad  = (header_h - logo_size) * 0.5f; // 자동 계산 (= 10.5)
    const float zone_pad  = 16.0f;   // Zone 2/3 내부 좌측 마진


    // ───── 콘텐츠 폭 측정 → brand_w 결정 (콘텐츠에 딱 맞춤) ─────
    ImGui::PushFont(mim::font_title);
    const float title_w = ImGui::CalcTextSize("Yonsei Cubesat GS").x;
    ImGui::PopFont();

    ImGui::PushFont(mim::font_subtitle);
    const float subtitle_w = ImGui::CalcTextSize("Yonsei University - Astrodynamics & Control Lab").x;
    ImGui::PopFont();

    const float group_w   = (title_w > subtitle_w ? title_w : subtitle_w);
    const float content_w = logo_size + gap + group_w;
    const float brand_w   = content_w + logo_size * 0.3 + 2 * pad;  // 우측에 로고 폭만큼 마진 추가

    // ───── 최상위 헤더 윈도우 ─────
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(W, header_h));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, BG_MAIN);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGuiWindowFlags hflags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove   | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("##header", NULL, hflags);

    // ════════════════════════════════════════════════
    //  Zone 1 — 로고 + 타이틀 (왼쪽, 전체 높이)
    // ════════════════════════════════════════════════
    ImGui::BeginChild("##zone_brand", ImVec2(brand_w, header_h), false,
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        // 로고 위치 — 좌측 정렬 (pad만 띄움)
        ImGui::SetCursorPos(ImVec2(pad, pad));

        // 로고 placeholder — border + 옅은 fill로 정사각형 가시화 (이미지 import 시 교체)
        if (logo_tex) 
        {
            ImGui::Image((ImTextureID)(intptr_t)logo_tex, ImVec2(logo_size, logo_size));
        } 
        else 
        {
            // 폴백: 파일 못 찾으면 placeholder 사각형 (D4 코드 그대로)
            ImVec2 logo_tl = ImGui::GetCursorScreenPos();
            ImVec2 logo_br = ImVec2(logo_tl.x + logo_size, logo_tl.y + logo_size);
            ImGui::GetWindowDrawList()->AddRectFilled(logo_tl, logo_br, U32(BG_PANEL2), 6.0f);
            ImGui::GetWindowDrawList()->AddRect      (logo_tl, logo_br, U32(BORDER),   6.0f, 0, 1.5f);
            ImGui::Dummy(ImVec2(logo_size, logo_size));
        }
        ImGui::SameLine(0, gap);

        // 타이틀+부제 그룹 세로 중앙 정렬 (D2 그대로)
        const float title_h    = mim::font_title    ? mim::font_title->FontSize    : ImGui::GetFontSize();
        const float subtitle_h = mim::font_subtitle ? mim::font_subtitle->FontSize : ImGui::GetFontSize();
        const float group_h    = title_h + subtitle_h + ImGui::GetStyle().ItemSpacing.y;
        ImGui::SetCursorPosY((header_h - group_h) * 0.5f);

        ImGui::BeginGroup();
            ImGui::PushFont(mim::font_title);
            ImGui::PushStyleColor(ImGuiCol_Text, TEXT_HI);
            ImGui::TextUnformatted("Yonsei Cubesat GS");
            ImGui::PopStyleColor();
            ImGui::PopFont();

            ImGui::PushFont(mim::font_subtitle);
            ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
            ImGui::TextUnformatted("Yonsei University - Astrodynamics & Control Lab");
            ImGui::PopStyleColor();
            ImGui::PopFont();
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine(0, 0);

    // ════════════════════════════════════════════════
    //  Zone 2 + Zone 3 — 우측 컬럼 (메뉴 위, 스테이터스 아래)
    // ════════════════════════════════════════════════
    ImGui::BeginGroup();
    {
        float fp_y = (strip_h - ImGui::GetFontSize()) * 0.5f;
        if (fp_y < 4.0f) fp_y = 4.0f;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, fp_y));
        ImGui::BeginChild("##zone_menu", ImVec2(0, strip_h),
                false,
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar
                | ImGuiWindowFlags_NoScrollWithMouse);
        {
            if (ImGui::BeginMenuBar())
            {
                // 좌측 마진 — 구분선과 메뉴 텍스트 사이 공백
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + zone_pad);

                // 메뉴 항목을 strip 세로 중앙에 정렬 (메뉴바 자체는 tall, 항목은 normal 높이)
                float item_h = ImGui::GetFrameHeight();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (strip_h - item_h) * 0.5f);

                if (ImGui::BeginMenu("File"))
                {
                    if(ImGui::MenuItem("FDS Manager"))    AppUi.Display_FDS = true;
                    if(ImGui::MenuItem("TLE Manager"))    AppUi.Display_TLE = true;
                    if(ImGui::MenuItem("S-band Manager")) AppUi.Display_Sband = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Hardware"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) AppUi.Display_paramt0 = true;
                    if(ImGui::MenuItem("GS100 Param 1")) AppUi.Display_paramt1 = true;
                    if(ImGui::MenuItem("GS100 Param 5")) AppUi.Display_paramt5 = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Test"))
                {
                    if (ImGui::MenuItem("Scan AX100"))
                        mim::svc::StartPingScan();
                    if (ImGui::MenuItem("Start Signaltest"))
                        mim::svc::StartSignalTest();
                    if (ImGui::MenuItem("Stop Signaltest"))
                    {
                        mim::svc::StopSignalTest();
                    }
                    if (State.Debugmode)
                    {
                        if (ImGui::MenuItem("Debug Mode Off"))
                        {
                            csp_log_info("Set Debug : False.");
                            State.Debugmode = false;
                            csp_debug_set_level(CSP_INFO, false);
                            csp_debug_set_level(CSP_PACKET, false);
                            csp_debug_set_level(CSP_PROTOCOL, false);
                        }
                    }
                    else
                    {
                        if (ImGui::MenuItem("Debug Mode On"))
                        {
                            State.Debugmode = true;
                            csp_debug_set_level(CSP_INFO, true);
                            csp_debug_set_level(CSP_PACKET, true);
                            csp_debug_set_level(CSP_PROTOCOL, true);
                            csp_log_info("Set Debug : True.");
                        }
                    }
                    if (ImGui::MenuItem("TLE Update(Auto)")) TLE_Autoupdate_Test();
                    if (ImGui::MenuItem("Segfault"))
                    {
                        int* ptr = nullptr;
                        *ptr = 42;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) AppUi.Display_paramt0 = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Debug"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) AppUi.Display_paramt0 = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Help"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) AppUi.Display_paramt0 = true;
                    ImGui::EndMenu();
                }
                
                ImGui::EndMenuBar();
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();   // ← BeginChild가 strip 높이 확정 후 즉시 Pop

        // ── Zone 3 — 스테이터스 ──
        ImGui::BeginChild("##zone_status", ImVec2(0, strip_h), false,
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            // 스테이터스 콘텐츠 정렬
            float content_h = ImGui::GetTextLineHeight();
            ImGui::SetCursorPos(ImVec2(zone_pad, (strip_h - content_h) * 0.5f));
            float zone_w = ImGui::GetContentRegionAvail().x;
            ImGui_StatusBar_Body(zone_w);
        }
        ImGui::EndChild();
    }
    ImGui::EndGroup();

    // ── Zone 구분선 (BG_DEEP, 얇은 1px) ──
    ImVec2 hpos       = ImGui::GetWindowPos();
    ImDrawList* hdl   = ImGui::GetWindowDrawList();
    const ImU32 sep_c = U32(BORDER_HI);
    const float sep_t = 2.0f;

    // 세로선: Zone 1 (brand) ↔ Zone 2/3
    hdl->AddLine(
        ImVec2(hpos.x + brand_w, hpos.y),
        ImVec2(hpos.x + brand_w, hpos.y + header_h),
        sep_c, sep_t);

    // 가로선: Zone 2 (menu) ↔ Zone 3 (status)
    hdl->AddLine(
        ImVec2(hpos.x + brand_w, hpos.y + strip_h),
        ImVec2(hpos.x + W,       hpos.y + strip_h),
        sep_c, sep_t);

    ImGui::End();
    ImGui::PopStyleVar();    // WindowPadding
    ImGui::PopStyleColor();  // WindowBg
}
