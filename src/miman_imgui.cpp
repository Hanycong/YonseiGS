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


using namespace mim;

// Showstate는 main.cpp에 정의된 전역 변수
extern bool Showstate;


extern FILE *log_ptr;
bool raw_data = true;
Console console;
bool p_open;
ImGuiWindowFlags mim_winflags = ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoSavedSettings |
                                ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoBringToFrontOnFocus|
                                // ImGuiWindowFlags_HorizontalScrollbar|
                                ImGuiWindowFlags_NoTitleBar;
// p_thread[] 는 UI 에서 직접 쓰지 않는다 -> model/service/svc_*.h 사용
extern Ptable_0 param0;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern StateCheckUnit State;
extern Command * command;
extern Beacon * beacon;
extern MissionBeacon * missionbeacon;
extern GETFILEINFO * getfileinfo;
extern Event * event;
extern int BeaconCounter;
extern int PingCounter;
char callsignbuf[10];

int temptarget = 1;
int tempstatus = 1;
int tempfilenum = 1;
int tempoffset = 0;
int tempstep = 0;

extern dlreqdata * request;
static int freeze_cols = 1;
static int freeze_rows = 1;

bool booking;
uint32_t gen_msgid = 0;
uint16_t gen_fnccode = 0;

extern CmdGenerator_GS * SatCMD[256];
ReportPacket_t g_last_report = {};                  // RPT


// SchedulerState / ChecksumState / ExecutionTimeBuf / ExecutionWindowBuf / EntryIDBuf / GroupIDBuf -> view/autopilot_view.cpp
int NowCMD = 0;

extern struct Antenna Now;
int NowTLE = 0;
int Requested = -2;
extern int NowFTP;
extern Setup * setup;

Ptable_0 rparam0;
Ptable_1 rparam1;
Ptable_5 rparam5;
uint8_t remote_activeconf = 99;
int16_t remote_lastrssi = 999;
int16_t remote_rssibusy = 0;
uint16_t remote_bootcount = 0;

int Satellite_row;
char TrackingButtonTextBuffer[32];
char InfoButtonTextBuffer[32];

char Sendinglabelbuf[16];
char Deletelabelbuf[16];
uint8_t msgidbuf[2];
uint16_t msgidinfo;

// SelectButtonTextBuf / DeleteButtonTextBuf / SearchBuf / tlepopupindex -> viewmodel/tle_vm (TleVM 멤버, 행 버튼 라벨은 View 지역 버퍼)
bool sgp4check = false;
// FDSFilePath / SatWindowLabelBuf / inputbuf -> viewmodel/fds_vm, satinfo_vm, s_band_vm (멤버)
extern Sband * sgs;

char ComboboxLabel[32];

char AmpTimeBuf[32];

bool show_typingWindow = false;
typedef struct CMDInfo {
    uint16_t msgid;
    uint16_t fnccode;
}__attribute__((packed));

//CMDInfo *info = new CMDInfo();

cmd_packet_t * shellcmd = new cmd_packet_t;
char repeated_label[64];

/********************************************5차 추가***************************************************/

static int chosen_sat_mode = 0;
// sat_names / hmac_keys / is_switching / SwitchSatelliteTask -> model/service/svc_radio.cpp 로 이동


/***********************************************************************************************************/

void ImGui_ClearColorBuf(GLFWwindow * window, ImVec4 clear_color)
{
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}


// 같은 파일 하단(L14224)에 정의된 헬퍼 — 앞에서 사용하기 위한 forward decl
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int width, int height);

// void ImGui_MainMenu -> src/app/ 로 이동

void ImGui_ModelWindow(float fontscale)
{
    
    ImGui::Begin("Orbital Model", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}

// ImGui_TrackWindow_Body / ImGui_TrackWindow -> view/track_view.cpp 로 이동

// ImGui_AutoPilotWindow -> view/autopilot_view.cpp 로 이동 (main.cpp 에서 호출이 주석 처리돼 있어 현재 화면에 없음)


// ImGui_FrequencyWindow_Body / ImGui_FrequencyWindow -> view/frequency_view.cpp 로 이동

// ImGui_RotatorWindow_Body / ImGui_RotatorWindow -> view/rotator_view.cpp 로 이동

// StateWindowColumnManager / BeaconSectionHeader / ImGui_BeaconWindow_Body / ImGui_BeaconWindow
//   -> view/beacon_view.cpp 로 이동








// PingWindow / HMACWindow / RparamWindow -> view/control_view.cpp 로 이동

// 옛 명령 라벨 표(Templabels / Initialize_CMDLabels), FilterRules, INITIAL_CMD_INDICES 는 삭제.
// 명령 목록·분류·초기운용 목록은 model/telecommand 카탈로그가 가진다. 골든 하네스는 옛 표의 고정본
// (tools/cmdgen_golden/legacy_tables.h)과 카탈로그를 대조한다.

// DrawCmdGeneratorBody (명령 생성기) -> view/cmdgen/cmdgen_view.cpp 로 이동


// ImGui_ControlWindow / ImGui_ControlWindow_Body -> view/control_view.cpp 로 이동

// ImGui_CommandWindow_Body / ImGui_CommandWindow (가운데 열 = FTP 창) -> view/ftp_view.cpp 로 이동

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int width, int height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
// Console 구현 -> model/service/console_log.cpp (로그 저장·파일 기록), view/console_view.cpp (창 그리기)









// void ImGuiCustomStyle(ImGuiStyle* style)
// {
//     style->WindowPadding     = ImVec2(12, 12);
//     style->WindowRounding    = 6.0f;
//     style->FramePadding      = ImVec2(6, 4);
//     style->FrameRounding     = 4.0f;
//     style->ItemSpacing       = ImVec2(10, 6);
//     style->ItemInnerSpacing  = ImVec2(6, 4);
//     style->IndentSpacing     = 20.0f;
//     style->ScrollbarSize     = 14.0f;
//     style->ScrollbarRounding = 6.0f;
//     style->GrabMinSize       = 8.0f;
//     style->GrabRounding      = 4.0f;

//     ImVec4* c = style->Colors;

//     // === Base pastel palette (low saturation, soft colors) ===
//     ImVec4 bg_main     = ImVec4(0.84f, 0.84f, 0.95f, 1.00f); // soft lavender-gray
//     ImVec4 window_bg   = ImVec4(0.90f, 0.90f, 0.96f, 0.92f);
//     ImVec4 frame_bg    = ImVec4(0.82f, 0.82f, 0.92f, 0.95f);
//     ImVec4 frame_hover = ImVec4(0.76f, 0.76f, 0.88f, 0.98f);
//     ImVec4 frame_active= ImVec4(0.70f, 0.70f, 0.86f, 1.00f);

//     ImVec4 accent_mint = ImVec4(0.70f, 0.88f, 0.88f, 1.00f);
//     ImVec4 accent_blue = ImVec4(0.60f, 0.78f, 0.96f, 1.00f);
//     ImVec4 accent_lav  = ImVec4(0.75f, 0.68f, 0.90f, 1.00f);

//     ImVec4 title_soft  = ImVec4(0.78f, 0.76f, 0.92f, 1.00f);
//     ImVec4 title_active= ImVec4(0.70f, 0.85f, 0.95f, 1.00f);

//     ImVec4 text_main   = ImVec4(0.15f, 0.13f, 0.25f, 1.00f);  // dark lavender-gray
//     ImVec4 text_muted  = ImVec4(0.45f, 0.43f, 0.55f, 1.00f);

//     ImVec4 border      = ImVec4(0.62f, 0.60f, 0.75f, 1.00f);

//     // === Text ===
//     c[ImGuiCol_Text]         = text_main;
//     c[ImGuiCol_TextDisabled] = text_muted;

//     // === Windows ===
//     c[ImGuiCol_WindowBg]     = window_bg;
//     c[ImGuiCol_ChildBg]      = ImVec4(0,0,0,0);
//     c[ImGuiCol_PopupBg]      = window_bg;

//     c[ImGuiCol_Border]       = border;
//     c[ImGuiCol_BorderShadow] = ImVec4(0,0,0,0);

//     // === Frame ===
//     c[ImGuiCol_FrameBg]        = frame_bg;
//     c[ImGuiCol_FrameBgHovered] = frame_hover;
//     c[ImGuiCol_FrameBgActive]  = frame_active;

//     // === Title bars ===
//     c[ImGuiCol_TitleBg]          = title_soft;
//     c[ImGuiCol_TitleBgActive]    = title_active;
//     c[ImGuiCol_TitleBgCollapsed] = ImVec4(title_soft.x, title_soft.y, title_soft.z, 0.6f);

//     // === Tabs ===
//     c[ImGuiCol_Tab]              = title_soft;
//     c[ImGuiCol_TabHovered]       = accent_blue;
//     c[ImGuiCol_TabActive]        = accent_lav;
//     c[ImGuiCol_TabUnfocused]     = title_soft;
//     c[ImGuiCol_TabUnfocusedActive] = accent_blue;

//     // === Buttons ===
//     c[ImGuiCol_Button]           = accent_lav;
//     c[ImGuiCol_ButtonHovered]    = accent_blue;
//     c[ImGuiCol_ButtonActive]     = accent_mint;

//     // === Scroll ===
//     c[ImGuiCol_ScrollbarBg]      = ImVec4(0,0,0,0);
//     c[ImGuiCol_ScrollbarGrab]    = accent_lav;
//     c[ImGuiCol_ScrollbarGrabHovered] = accent_blue;
//     c[ImGuiCol_ScrollbarGrabActive]  = accent_mint;

//     // === Header ===
//     c[ImGuiCol_Header]           = accent_lav;
//     c[ImGuiCol_HeaderHovered]    = accent_blue;
//     c[ImGuiCol_HeaderActive]     = accent_mint;

//     style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.70f, 0.70f, 0.92f, 0.55f);

//     // === Optional: mild transparency on windows (soft look) ===
//     style->Colors[ImGuiCol_WindowBg].w = 0.90f;
// }

// void ImGuiCustomStyle(ImGuiStyle* style)
// {
//     style->WindowPadding = ImVec2(15, 15);
// 	style->WindowRounding = 5.0f;
// 	style->FramePadding = ImVec2(5, 5);
// 	style->FrameRounding = 4.0f;
// 	style->ItemSpacing = ImVec2(12, 8);
// 	style->ItemInnerSpacing = ImVec2(8, 6);
// 	style->IndentSpacing = 25.0f;
// 	style->ScrollbarSize = 15.0f;
// 	style->ScrollbarRounding = 9.0f;
// 	style->GrabMinSize = 5.0f;
// 	style->GrabRounding = 3.0f;

//     //Window Background
//     ImVec4 Colorset_1 = ImVec4(0.141f, 0.364f, 0.376f, 1.00f);
//     //Disabled
//     ImVec4 Colorset_2 = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
//     //Active Title
//     ImVec4 Colorset_3 = ImVec4(0.235f, 0.603f, 0.623f, 1.00f);
//     //General Title, Button
//     ImVec4 Colorset_4 = ImVec4(0.180f, 0.462f, 0.478f, 1.00f);
//     //ScrollBar
//     ImVec4 Colorset_5 = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
//     //Active Button
//     ImVec4 Colorset_6 = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
 
// 	style->Colors[ImGuiCol_Text] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
// 	style->Colors[ImGuiCol_TextDisabled] = Colorset_2;
// 	style->Colors[ImGuiCol_WindowBg] = Colorset_1;
// 	style->Colors[ImGuiCol_PopupBg] = Colorset_1;
// 	style->Colors[ImGuiCol_Border] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
// 	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
// 	style->Colors[ImGuiCol_FrameBg] = Colorset_4;
// 	style->Colors[ImGuiCol_FrameBgHovered] = Colorset_2;
// 	style->Colors[ImGuiCol_FrameBgActive] = Colorset_6;
// 	style->Colors[ImGuiCol_TitleBg] = Colorset_4;
// 	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
// 	style->Colors[ImGuiCol_TitleBgActive] = Colorset_3;
// 	style->Colors[ImGuiCol_MenuBarBg] = Colorset_4;
// 	style->Colors[ImGuiCol_ScrollbarBg] = Colorset_1;
// 	style->Colors[ImGuiCol_ScrollbarGrab] = Colorset_5 ;
// 	style->Colors[ImGuiCol_ScrollbarGrabHovered] = Colorset_6;
// 	style->Colors[ImGuiCol_ScrollbarGrabActive] = Colorset_5;
// 	style->Colors[ImGuiCol_CheckMark] = Colorset_5 ;
// 	style->Colors[ImGuiCol_SliderGrab] = Colorset_5 ;
// 	style->Colors[ImGuiCol_SliderGrabActive] = Colorset_1;
// 	style->Colors[ImGuiCol_Button] = Colorset_4;
// 	style->Colors[ImGuiCol_ButtonHovered] = Colorset_2;
// 	style->Colors[ImGuiCol_ButtonActive] = Colorset_6;
// 	style->Colors[ImGuiCol_Header] = Colorset_4;
// 	style->Colors[ImGuiCol_HeaderHovered] = Colorset_6;
// 	style->Colors[ImGuiCol_HeaderActive] = Colorset_1;
// 	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
// 	style->Colors[ImGuiCol_ResizeGripHovered] = Colorset_6;
// 	style->Colors[ImGuiCol_ResizeGripActive] = Colorset_1;
// 	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
// 	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
// 	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
// 	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
// 	style->Colors[ImGuiCol_TextSelectedBg] = Colorset_3;
//     style->Colors[ImGuiCol_Tab] = Colorset_4;
//     style->Colors[ImGuiCol_TabActive] = Colorset_3;
// }


// bool popup_setup(Setup * setup) -> view/popups/setup_popup.cpp 로 이동

//Functions
// bool popup_param_table0() -> view/popups/param_table0_popup.cpp 로 이동

// bool popup_param_table1() -> view/popups/param_table1_popup.cpp 로 이동

// bool popup_param_table5() -> view/popups/param_table5_popup.cpp 로 이동

// bool popup_load() -> view/popups/load_popup.cpp 로 이동

//For Temporary multiple tle operation

// bool popup_tle() -> view/popups/tle_popup.cpp 로 이동





// int CMDDataGenerator -> view/autopilot_view.cpp 로 이동 (AutoPilot 창만 쓴다)


// bool popup_fds() -> view/popups/fds_popup.cpp 로 이동



// bool popup_satinfo(int index) -> view/popups/satinfo_popup.cpp 로 이동

// bool popup_s_band() -> view/popups/s_band_popup.cpp 로 이동

#include "miman_theme.h"
using namespace mim;

// void ImGui_SatelliteTabs -> src/app/ 로 이동

// void ImGui_StatusBar_Body -> src/app/ 로 이동

// 기존 함수는 컨테이너(BeginChild)만 담당하고 본체 호출.
// 다른 패턴 (TrackWindow_Body, FrequencyWindow_Body 등)과 동일.
// void ImGui_StatusBar -> src/app/ 로 이동

// void ImGui_MainWorkspace -> src/app/ 로 이동
