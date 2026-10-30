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


#define CMD_LABEL_MAX 300
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
static const int INITIAL_CMD_INDICES[] = {
    0, 1, 2, 3, 4,   // no arg, u8, u16, u32, u64
    72, 73, 74, 76, 77, 79, 81, 82, 85, 86, 58,80, // ADCS sunpointing용
    155,    // Enable Beacon
    200,
    205,    // UANT burn channel
    160,    // UANT Get Status
    25,     // EPS Dock Get HK
    164, 165, 166, 167,   // Deploy SP
    204,       // FM Get File Info
    206,       // UTRX GNDWDT clear
    -1               // 끝 표시
};
ReportPacket_t g_last_report = {};                  // RPT


bool SchedulerState = false;
bool ChecksumState = true;
uint32_t ExecutionTimeBuf = 0;
uint32_t ExecutionWindowBuf = 0;
uint16_t EntryIDBuf = 0;
uint16_t GroupIDBuf = 0;
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

char SelectButtonTextBuf[32];
char DeleteButtonTextBuf[32];
char SearchBuf[256];
int tlepopupindex;
bool sgp4check = false;
char FDSFilePath[256] = "../bin/fds/fds.dat";
char SatWindowLabelBuf[64];
char inputbuf[1024];
extern Sband * sgs;

char ComboboxLabel[32];
char ADCSCMDLabels[128][64];
char CICMDLabels[128][64];
char EPSCMDLabels[128][64];
char FMCMDLabels[128][64];
char GPSCMDLabels[128][64];
char MTQCMDLabels[128][64];
char PAYCMDLabels[128][64];
char RWACMDLabels[128][64];
char SNSRCMDLabels[128][64];
char STXCMDLabels[128][64];
char TOCMDLabels[128][64];
char UTRXCMDLabels[128][64];
char TSCMDLabels[128][64];
char ESCMDLabels[128][64];
char SCHCMDLabels[128][64];
char ECMCMDLabels[128][64];

char Templabels[CMD_LABEL_MAX][64];

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
                    if(ImGui::MenuItem("FDS Manager"))    State.Display_FDS = true;
                    if(ImGui::MenuItem("TLE Manager"))    State.Display_TLE = true;
                    if(ImGui::MenuItem("S-band Manager")) State.Display_Sband = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Hardware"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) State.Display_paramt0 = true;
                    if(ImGui::MenuItem("GS100 Param 1")) State.Display_paramt1 = true;
                    if(ImGui::MenuItem("GS100 Param 5")) State.Display_paramt5 = true;
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
                    if(ImGui::MenuItem("GS100 Param 0")) State.Display_paramt0 = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Debug"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) State.Display_paramt0 = true;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Help"))
                {
                    if(ImGui::MenuItem("GS100 Param 0")) State.Display_paramt0 = true;
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

void ImGui_ModelWindow(float fontscale)
{
    
    ImGui::Begin("Orbital Model", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);
    
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}

// ImGui_TrackWindow_Body / ImGui_TrackWindow -> view/track_view.cpp 로 이동

void ImGui_AutoPilotWindow(float fontscale)
{
    static bool p_open = true;

    ImGui::Begin("AutoPilot", &p_open, mim_winflags);
    ImGui::SetWindowFontScale(fontscale);

    // ===== Autopilot controls =====
    ImGui::Text("AutoPilot Controls");
    ImGui::Checkbox("Beacon Check", &State.DoBeaconCheck);
    // ImGui::SameLine();
    ImGui::Checkbox("Ping", &State.DoPing);
    ImGui::SameLine();
    ImGui::Checkbox("CMD", &State.DoCMD);

    // ImGui::Checkbox("FTP DL", &State.DoFTPDL);
    // ImGui::SameLine();
    // ImGui::Checkbox("FTP UL", &State.DoFTPUL);
    // ImGui::SameLine();
    ImGui::Checkbox("AMP", &State.AMPmode);

    // ImGui::Checkbox("Baud Calibration", &State.DoBaudCalibration);
    // ImGui::SameLine();
    // ImGui::Checkbox("Freq Calibration", &State.DoFreqCalibration);

    ImGui::Text("iteration");
    ImGui::SameLine();
    ImGui::InputInt("##AutoPilotIter", &State.iteration);

    ImGui::Text("Time Span");
    ImGui::SameLine();
    ImGui::InputInt("##AutoPilotSpan", &State.SpanTime);

    if (!State.Autopilot)
    {
        if (ImGui::Button("Start AutoPilot", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.8f)))
        {
            mim::svc::StartAutoPilot();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop AutoPilot", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.8f)))
        {
            mim::svc::StopAutoPilot();
        }
        ImGui::PopStyleColor();
    }

    // ===== Live status =====
    ImGui::Separator();
    ImGui::Text("Live Counters");
    ImGui::Text("BeaconCounter : %d", BeaconCounter);
    ImGui::Text("PingCounter   : %d", PingCounter);
    ImGui::Text("CMDCount      : %d", State.CMDCount);

    // ===== CMD Generator (simple) =====
    ImGui::Separator();
    ImGui::Text("CMD Generator");

    static uint32_t test_msgid;
    static uint16_t test_fnccode;
    static CFE_MSG_CommandHeader dummy_hdr;

    ImGui::InputScalar("MsgID",   ImGuiDataType_U32, &test_msgid,  NULL, NULL, "0x%08X");
    ImGui::InputScalar("FncCode", ImGuiDataType_U16, &test_fnccode, NULL, NULL, "%u");

    CMDDataGenerator(test_msgid, test_fnccode, &dummy_hdr, sizeof(dummy_hdr));

    ImGui::Separator();
    ImGui::Text("SatCMD Queue");
    for (int i = 0; i < 256; i++)
    {
        if (SatCMD[i] != NULL)
            ImGui::Text("SatCMD[%d] : READY", i);
    }

    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}


// ImGui_FrequencyWindow_Body / ImGui_FrequencyWindow -> view/frequency_view.cpp 로 이동

// ImGui_RotatorWindow_Body / ImGui_RotatorWindow -> view/rotator_view.cpp 로 이동

// StateWindowColumnManager / BeaconSectionHeader / ImGui_BeaconWindow_Body / ImGui_BeaconWindow
//   -> view/beacon_view.cpp 로 이동








// PingWindow / HMACWindow / RparamWindow -> view/control_view.cpp 로 이동

struct SubsystemRange {
    int sub_index;
    int start;
    int end;
};

static const SubsystemRange FilterRules[] = {
    { 1, 0, 9 },      // Test & Debug
    { 2, 10, 46 },    // EPS range 1
    { 2, 209, 211 },  // EPS range 2
    { 2, 216, 216 },  // EPS single
    { 3, 47, 88 },    // ADCS
    { 4, 160, 163 },  // UANT
    { 5, 164, 167 },  // SP range 1
    { 5, 212, 212 },  // SP range 1
    { 6, 168, 180 },  // PAYUZUC
    { 7, 214, 215 },  // Cosmic
    { 8, 153, 159 },  // cFE range 1
    { 8, 207, 208 },  // cFE range 2
    { 8, 217, 221 },  // cFE range 3
    { 9, 136, 152 },  // SC APP
    { 10, 201, 204 }, // FTP range 1
    { 10, 213, 213 }, // FTP range 2
    { 11, 206, 206 }, // UTRX
    { 12, 230, 253 }, // LGPM (Case 230~253)
    { 13, 330, 342 }, // LTRX (Case 330~342) 
    { 14, 260, 277 }, // STX (Case 260~277)
    { 15, 280, 310 }, // PAYUEL_ROMA (Case 280~310)
    { 16, 320, 326 }, // PAYUEL_LGBAT (Case 320~326)
};

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


int Console::Stricmp(const char* str1, const char* str2)
{
    int d;
    while (d = toupper(*str2) - toupper(*str1) == 0 && *str1)
    {
        str1++;
        str2++;
    }
    return d;
}

int Console::Strnicmp(const char* str1, const char* str2, int n)
{
    int d = 0;
    while(n > 0 && (d = toupper(*str2) - toupper(*str1)==0 && *str1) )
    {
        str1++;
        str2++;
        n--;
    }
    return d;
}
char* Console::Strdup(const char *str)
{
    size_t len = strlen(str) + 1;
    void* buf = malloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)str, len);
}
void Console::Strtrim(char* str)
{
    char* str_end = str + strlen(str);
    while (str_end > str && str_end[-1] == ' ')
        str_end--;
    *str_end = 0;
}

void Console::ClearLog()
{
    for (int i = 0; i < _items.Size; i++)
        free(_items[i]);
    _items.clear();
}

void Console::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
    char str[1024];
    char buf[1024];
    char tmp[1024];
    char * pos;
    char * bug;
    _now = time(NULL);
    _now_time = *localtime(&_now);

    strftime(tmp, sizeof(tmp), "[%H:%M:%S]  ", &_now_time);
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);

    if ((pos = strstr(buf, "##")) != 0)
    {
        strcpy(str, tmp);
        strcat(str, pos + 2);
        strcpy(pos + 2, str);
        //Write to IMGUI Console.
        if ((bug = strstr(buf, "[DEBUG]##")) != 0)
        {
            if(State.Debugmode)
            {
                _debug_fs.write(str, strlen(str));
                _debug_fs.write("\n", 1);
            }
                
        }
        else if ((bug = strstr(buf, "[FTP]##")) != 0)
        {
            _history_fs.write(str, strlen(str));
            _history_fs.write("\n", 1);
        }
        else
        {
            _items.push_back(Strdup(buf));
            _history_fs.write(str, strlen(str));
            _history_fs.write("\n", 1);
        }
    }
    else
    {
        strcpy(str, tmp);
        strcat(str, buf);
        //Write to IMGUI Console.
        _items.push_back(Strdup(str));
        _history_fs.write(str, strlen(str));
        _history_fs.write("\n", 1);
    }

    this->_history_fs.flush();
    this->_debug_fs.flush();
   if (log_ptr) {
        fprintf(log_ptr, "%s\n", str);
        fflush(log_ptr);
    }
    
}

void Console::DelStartingCharLog(const char* fmt)
{
    char buf[1024];
    strcpy(buf, fmt);
    for (int i = 0; i < _items.Size; i++)
    {
        char* item = _items[i];
        if(strstr(item, buf))
        {
            _items.erase(&_items[i]);
        }
    }
}

void Console::DelPrefixLog(const char* fmt)
{
    char buf[1024];
    strcpy(buf, fmt);
    for (int i = 0; i < _items.Size; i++)
    {
        char * item = _items[i];
        if (strstr(item, buf))
        {
            _items.insert(&_items[i], item + 1);
            _items.erase(&_items[i + 1]);
        }
    }
}

void Console::Draw(const char* title, bool* p_open, float fontscale)
{



    ImGui::SetNextWindowPos(ImVec2(this -> _x_pos, this -> _y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this -> _width, this -> _height), ImGuiCond_Always);

    // 콘솔 배경 검정, 텍스트 흰색
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg,  ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,     ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (!ImGui::Begin(title, p_open, this -> _window_flags))
    {
        ImGui::End();
        return;
    }
    ImGui::SetWindowFontScale(fontscale);

    bool copy_to_clipboard = false;

    const float footer_height_to_reserve = 0.0;





    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    //ImGui::BeginChild("scrolling");
    if(ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear"))
            ClearLog();
        copy_to_clipboard = ImGui::Selectable("Copy");
        ImGui::EndPopup();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    for(int i = 0; i < _items.Size; i++)
    {
        char*item = _items[i];

        bool pop_color = false;
        if(strstr(item, "[ERROR]##"))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.38f, 0.36f, 1.0f));
            pop_color = true;
            item += 9;
        }
        else if (strstr(item, "[OK]##"))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.42f, 0.84f, 0.66f, 1.0f));
            pop_color = true;
            item += 6;
        }
        else if (strncmp(item, "# ", 2) == 0)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
            pop_color = true;
        }
        else if (strstr(item, "$"))
        {
            item += 1;
        }
        ImGui::SetWindowFontScale(1.1);
        ImGui::TextUnformatted(item);
        if (pop_color)
        {
            ImGui::PopStyleColor();
        }
        ImGui::SetWindowFontScale(1.0);
    }

    if (copy_to_clipboard)
        ImGui::LogFinish();

    if (_push_to_bottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::SetWindowFontScale(1.0);

    ImGui::End();
    ImGui::PopStyleColor(3);   //윈도우 배경, 차일드 배경, 텍스트 색상 팝

}

void Console::ChangeWindowSize(float x_pos, float y_pos, float width, float height)
{
    this -> _x_pos = x_pos;
    this -> _y_pos = y_pos;
    this -> _width = width;
    this -> _height = height;
}
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


bool popup_setup(Setup * setup)
{
    ImGui::OpenPopup("MIMAN Setup Page");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 540), ImGuiCond_Appearing);

    if ( ImGui::BeginPopupModal("MIMAN Setup Page", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::SetWindowFontScale(0.8f);
        ImGui::Text("Transceiver");
        ImGui::BeginChild("##setup_transceiver", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 13.5), true, mim_winflags);
        ImGui::Text("Device Name");
        ImGui::InputText("##GS100_name", &setup->Transciever_devname[0], sizeof(setup->Transciever_devname));
        ImGui::Text("Default Frequency(Hz)");
        ImGui::InputDouble("##GS100_freq", &setup->default_freq);
        ImGui::Text("Baudrate");
        ImGui::InputScalar("##GS100_baud", ImGuiDataType_U32, &setup->Transceiver_baud, NULL, NULL, "%u");
        ImGui::Text("Delay(ms)");
        ImGui::InputScalar("##GS100_delay", ImGuiDataType_U32, &setup->default_timeout, NULL, NULL, "%u");
        ImGui::EndChild();

        ImGui::Text("Switch Box");
        ImGui::BeginChild("##setup_switch", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 7.5), true, mim_winflags);
        ImGui::Text("Device Name");
        ImGui::InputText("##KTA223_name", &setup->Switch_devname[0], sizeof(setup->Switch_devname));
        ImGui::Text("Baudrate");
        ImGui::InputScalar("##KTA223_baud", ImGuiDataType_U32, &setup->Switch_baud, NULL, NULL, "%u");
        ImGui::EndChild();

        ImGui::Text("Rotator Controller");
        ImGui::BeginChild("##setup_rotator", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 7.5), true, mim_winflags);
        ImGui::Text("Device Name");
        ImGui::InputText("##GS232B_name", &setup->Rotator_devname[0], sizeof(setup->Rotator_devname));
        ImGui::Text("Baudrate");
        ImGui::InputScalar("##GS232B_baud", ImGuiDataType_U32, &setup->Rotator_baud, NULL, NULL, "%u");
        ImGui::EndChild();

        ImGui::Text("Route Table");
        ImGui::BeginChild("##setup_node", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 8.0), true, mim_winflags);
        ImGui::Text("GS100 node");
        ImGui::SameLine();
        ImGui::InputScalar("##gs100_node", ImGuiDataType_U8, &setup->gs100_node, NULL, NULL, "%u");
        ImGui::Text("GS PC node");
        ImGui::SameLine();
        ImGui::InputScalar("##gspc_node", ImGuiDataType_U8, &setup->kiss_node, NULL, NULL, "%u");
        ImGui::Text("AX100 node");
        ImGui::SameLine();
        ImGui::InputScalar("##ax100_node", ImGuiDataType_U8, &setup->ax100_node, NULL, NULL, "%u");
        ImGui::Text("OBC   node");
        ImGui::SameLine();
        ImGui::InputScalar("##obc_node", ImGuiDataType_U8, &setup->obc_node, NULL, NULL, "%u");
        ImGui::EndChild();

        ImGui::Text("Use S-Band GS");
        ImGui::SameLine();
        ImGui::Checkbox("##s_band_use_state_check", &State.SbandUse);
        if(State.SbandUse)
        {
            ImGui::Text("S-Band");
            ImGui::BeginChild("##setup_s_band", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 6.5), true, mim_winflags);
            ImGui::Text("Username");
            ImGui::SameLine();
            ImGui::InputText("##S-BandUsername", &setup->S_Username[0], sizeof(setup->S_Username));
            ImGui::Text("IP Address");
            ImGui::SameLine();
            ImGui::InputText("##S-BandIP", &setup->S_Address[0], sizeof(setup->S_Address));
            ImGui::Text("Password");
            ImGui::SameLine();
            ImGui::InputText("##S-BandPW", &setup->S_passwd[0], sizeof(setup->S_passwd));
            ImGui::EndChild();
        }
        ImGui::SetWindowFontScale(1.0f);
        

        if (ImGui::Button("Start", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetFontSize() * 1.5)))
        {
            State.GUI = true;
            State.InitializeRequired = true;
            set_serial_spec_micsec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);


            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset Watchdog", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            ResetWatchdog();
        }
        ImGui::SameLine();
        if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            State.GUI = false;
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        
        ImGui::EndPopup();
        return true;
    }
    return true;
}

//Functions
bool popup_param_table0()
{
    ImGui::OpenPopup("GS100 Parameter Table 0");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("GS100 Parameter Table 0", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::BeginChild("##Ptable0", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::PushItemWidth(300);
        ImGui::Text("RSSI Offset        : ");
        ImGui::SameLine();
        ImGui::InputScalar("##rssi_offset", ImGuiDataType_S8, &param0.rssi_offset, NULL, NULL, "%d");
        ImGui::Text("Max Temperature    : ");
        ImGui::SameLine();
        ImGui::InputScalar("°C##max_temp", ImGuiDataType_S16, &param0.max_temp, NULL, NULL, "%d");
        ImGui::Text("bgndrssl_ema       : ");
        ImGui::SameLine();
        ImGui::InputScalar("##bgndrssl_ema", ImGuiDataType_Float, &param0.bgndrssl_ema);
        ImGui::Text("CSP Node           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##csp_node", ImGuiDataType_U8, &param0.csp_node, NULL, NULL, "%u");
        ImGui::Text("I2C                : ");
        ImGui::SameLine();
        ImGui::Checkbox("##i2c_enable", &param0.i2c_en);
        ImGui::Text("CAN                : ");
        ImGui::SameLine();
        ImGui::Checkbox("##can_enable", &param0.can_en);
        ImGui::Text("EXTPTT             : ");
        ImGui::SameLine();
        ImGui::Checkbox("##extptt_enable", &param0.extptt_en);
        ImGui::SameLine();
        ImGui::InputScalar("##max_temp", ImGuiDataType_S8, &param0.max_temp, NULL, NULL, "%d");
        ImGui::Text("I2C Address        : ");
        ImGui::SameLine();
        ImGui::InputScalar("##i2c_addr", ImGuiDataType_U8, &param0.i2c_addr, NULL, NULL, "%u");
        ImGui::Text("I2C Frequency      : ");
        ImGui::SameLine();
        ImGui::InputScalar("kHz##i2c_khz", ImGuiDataType_U16, &param0.i2c_khz, NULL, NULL, "%u");
        ImGui::Text("CAN Frequency      : ");
        ImGui::SameLine();
        ImGui::InputScalar("kHz##can_khz", ImGuiDataType_U16, &param0.can_khz, NULL, NULL, "%u");
        ImGui::Text("Reboot in          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##reboot_in", ImGuiDataType_U16, &param0.reboot_in, NULL, NULL, "%u");
        ImGui::Text("TX Inhibit         : ");
        ImGui::SameLine();
        ImGui::InputScalar("##tx_inhibit", ImGuiDataType_U32, &param0.tx_inhibit, NULL, NULL, "%u");
        ImGui::Text("Store Log          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##log_store", ImGuiDataType_U8, &param0.log_store, NULL, NULL, "%u");
        ImGui::Text("TX Power           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##tx_power", ImGuiDataType_U8, &param0.tx_pwr, NULL, NULL, "%d");
        ImGui::Text("Maximum TX Time    : ");
        ImGui::SameLine();
        ImGui::InputScalar("second##max_tx_time", ImGuiDataType_U16, &param0.max_tx_time, NULL, NULL, "%u");
        ImGui::Text("Maximum IDLE Time  : ");
        ImGui::SameLine();
        ImGui::InputScalar("second##max_idle_time", ImGuiDataType_U16, &param0.max_idle_time, NULL, NULL, "%u");
        ImGui::Text("CSP Routing Table  : ");
        ImGui::SameLine();
        ImGui::InputText("##csp_rtable", param0.csp_rtable, sizeof(param0.csp_rtable));
        ImGui::PopItemWidth();
        ImGui::EndChild();

        if (ImGui::Button("Update", ImVec2(ImGui::GetContentRegionAvail().x * 0.25, 40)))
            mim::svc::LoadParamTable(0);
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, 40)))
            mim::svc::SaveParamTable(0);
        ImGui::SameLine();
        if (ImGui::Button("Save to FRAM", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40)))
        {
            int8_t id = 0;
            mim::svc::SaveParamToFRAM(&id);
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        
        ImGui::EndPopup();
        return true;
    }
    return true;
}

bool popup_param_table1()
{
    ImGui::OpenPopup("GS100 Parameter Table 1");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 540), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("GS100 Parameter Table 1", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::BeginChild("##Ptable1", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::PushItemWidth(300);
        ImGui::Text("Rx Frequency       : ");
        ImGui::SameLine();
        ImGui::InputScalar("##freq", ImGuiDataType_U32, &param1.freq, NULL, NULL, "%u");
        ImGui::Text("BaudRate           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##baud", ImGuiDataType_U32, &param1.baud, NULL, NULL, "%u");
        ImGui::Text("modindex           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##modeindex", ImGuiDataType_Float, &param1.modindex, NULL, NULL, "%f");
        ImGui::Text("Guard              : ");
        ImGui::SameLine();
        ImGui::InputScalar("##guard", ImGuiDataType_U16, &param1.guard, NULL, NULL, "%u");
        ImGui::Text("pllrang            : ");
        ImGui::SameLine();
        ImGui::InputScalar("##pllrang", ImGuiDataType_U8, &param1.pllrang, NULL, NULL, "%u");
        ImGui::Text("mode               : ");
        ImGui::SameLine();
        ImGui::InputScalar("##mode", ImGuiDataType_U8, &param1.mode, NULL, NULL, "%u");
        ImGui::Text("csp_hmac           : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_hmac", &param1.csp_hmac);
        ImGui::Text("csp_rs             : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_rs", &param1.csp_rs);
        ImGui::Text("csp_crc            : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_crc", &param1.csp_crc);
        ImGui::Text("csp_rand           : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_rand", &param1.csp_rand);
        ImGui::Text("AX25 Call          : ");
        ImGui::SameLine();
        ImGui::InputText("##ax25_call", param1.ax25_call, sizeof(param1.ax25_call));
        ImGui::Text("Bandwidth          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##bw", ImGuiDataType_U32, &param1.bw, NULL, NULL, "%u");
        ImGui::Text("AFC Range          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##afcrange", ImGuiDataType_S32, &param1.afcrange, NULL, NULL, "%d");
        ImGui::PopItemWidth();
        ImGui::EndChild();
        

        if (ImGui::Button("Update", ImVec2(ImGui::GetContentRegionAvail().x * 0.25, 40)))
            mim::svc::LoadParamTable(1);
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, 40)))
            mim::svc::SaveParamTable(1);
        ImGui::SameLine();
        if (ImGui::Button("Save to FRAM", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40)))
        {
            int8_t id = 1;
            mim::svc::SaveParamToFRAM(&id);
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}

bool popup_param_table5()
{
    ImGui::OpenPopup("GS100 Parameter Table 5");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 540), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("GS100 Parameter Table 5", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::BeginChild("##Ptable5", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::PushItemWidth(300);
        ImGui::Text("Tx Frequency       : ");
        ImGui::SameLine();
        ImGui::InputScalar("##freq", ImGuiDataType_U32, &param5.freq, NULL, NULL, "%u");
        ImGui::Text("BaudRate           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##baud", ImGuiDataType_U32, &param5.baud, NULL, NULL, "%u");
        ImGui::Text("modindex           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##modeindex", ImGuiDataType_Float, &param5.modindex, NULL, NULL, "%f");
        ImGui::Text("Guard              : ");
        ImGui::SameLine();
        ImGui::InputScalar("##guard", ImGuiDataType_U16, &param5.guard, NULL, NULL, "%u");
        ImGui::Text("pllrang            : ");
        ImGui::SameLine();
        ImGui::InputScalar("##pllrang", ImGuiDataType_U8, &param5.pllrang, NULL, NULL, "%u");
        ImGui::Text("mode               : ");
        ImGui::SameLine();
        ImGui::InputScalar("##mode", ImGuiDataType_U8, &param5.mode, NULL, NULL, "%u");
        ImGui::Text("csp_hmac           : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_hmac", &param5.csp_hmac);
        ImGui::Text("csp_rs             : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_rs", &param5.csp_rs);
        ImGui::Text("csp_crc            : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_crc", &param5.csp_crc);
        ImGui::Text("csp_rand           : ");
        ImGui::SameLine();
        ImGui::Checkbox("##csp_rand", &param5.csp_rand);
        ImGui::Text("AX25 Call          : ");
        ImGui::SameLine();
        ImGui::InputText("##ax25_call", param5.ax25_call, sizeof(param5.ax25_call));
        ImGui::Text("preamblen          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##preamblen", ImGuiDataType_U8, &param5.preamblen, NULL, NULL, "%u");
        ImGui::Text("preambflags        : ");
        ImGui::SameLine();
        ImGui::InputScalar("##preambflags", ImGuiDataType_U8, &param5.preambflags, NULL, NULL, "%u");
        ImGui::Text("intfrmlen          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##intfrmlen", ImGuiDataType_U8, &param5.intfrmlen, NULL, NULL, "%u");
        ImGui::Text("intfrmflags        : ");
        ImGui::SameLine();
        ImGui::InputScalar("##intfrmflags", ImGuiDataType_U8, &param5.intfrmflags, NULL, NULL, "%u");
        ImGui::Text("RSSI busy          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##rssibusy", ImGuiDataType_S16, &param5.rssibusy, NULL, NULL, "%d");
        ImGui::Text("kup delay          : ");
        ImGui::SameLine();
        ImGui::InputScalar("##kup_delay", ImGuiDataType_U8, &param5.kup_delay, NULL, NULL, "%u");
        ImGui::Text("PA Level           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##pa_level", ImGuiDataType_S16, &param5.pa_level, NULL, NULL, "%d");
        ImGui::PopItemWidth();
        ImGui::EndChild();

        if (ImGui::Button("Update", ImVec2(ImGui::GetContentRegionAvail().x * 0.25, 40)))
            mim::svc::LoadParamTable(5);
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, 40)))
            mim::svc::SaveParamTable(5);
        ImGui::SameLine();
        if (ImGui::Button("Save to FRAM", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40)))
        {
            int8_t id = 5;
            mim::svc::SaveParamToFRAM(&id);
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}

bool popup_load()
{
    ImGui::OpenPopup("Load");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::Text("Progress : %d%c", State.loadindex * 100 / State.tleallindex, '%');
        ImGui::EndPopup();
    }
    return true;
}

//For Temporary multiple tle operation
char localtleurl[256] = "/home/hyyang/GS_refactoring/YonseiGS/home/TLE/NORAD_TLE.txt";

bool popup_tle()
{
    ImGui::OpenPopup("TLE Manager");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1080, 720), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("TLE Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        if (ImGui::BeginCombo("##TLECombobox", State.tleinfolistup[NowTLE]->label))
        {
            for (int TLEindex = 0; TLEindex < IM_ARRAYSIZE(State.ftplistup); TLEindex++)
            {
                if (!State.tleinfolistup[TLEindex])
                    break;
                bool SelectedTLE = (NowTLE == TLEindex);
                char Selectedlabel[64];
                strcpy(Selectedlabel, State.tleinfolistup[TLEindex]->label);
                if (ImGui::Selectable(Selectedlabel, SelectedTLE))
                {
                    NowTLE = TLEindex;
                }
                if (SelectedTLE)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        ImGui::InputText("##", SearchBuf, sizeof(SearchBuf));
        ImGui::SameLine();
        ImGui::Text("SGP4");
        ImGui::SameLine();
        ImGui::Checkbox("##sgp4checkbox", &sgp4check);

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6);
        ImGui::InputText("##localtleimport", localtleurl, sizeof(localtleurl), 0, NULL, NULL);
        ImGui::SameLine();
        if(ImGui::Button("Load from local", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            
            State.NowTracking = false;
            memset(State.tleinfolistup[NowTLE]->local, 0, sizeof(State.tleinfolistup[NowTLE]->local));
            memcpy(State.tleinfolistup[NowTLE]->local, localtleurl, sizeof(localtleurl));
            
            console.AddLog("[OK]##TLE \"%s\" Download Completed.", State.tleinfolistup[NowTLE]->label);

            mim::svc::StartSatelliteInitialize(&sgp4check);
            // SatelliteInitialize(NULL);
                
            
        }
        
        if (ImGui::BeginTable("##TLESatelliteListup", 4, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.9)))
        {
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableSetupColumn("Name##tle",                ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Use##tle",                 ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Next AOS##tle",            ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Max Elevation##tle",       ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableHeadersRow();
            tlepopupindex = 0;

            // if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
            // {
            //     if (sorts_specs->SpecsDirty)
            //     {
            //         // If the sorting specs have changed, update the sorting state
            //         if (items.Size > 1)
            //             qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
            //         MyItem::s_current_sort_specs = NULL;
            //         sorts_specs->SpecsDirty = false;
            //         sort_column = sorts_specs->Specs[0].ColumnIndex;
            //         sort_ascending = sorts_specs->Specs[0].SortDirection == ImGuiSortDirection_Ascending;
            //     }
            // }

            while(1)
            {
                    
                if(strlen(State.Satellites[tlepopupindex]->Name())== NULL)
                    break;
                else
                {
                    if(State.Satellites[tlepopupindex]->_max_elevation[0] < 0 && sgp4check)
                    {
                        tlepopupindex += 1;
                        continue;
                    }
                    if(strlen(SearchBuf) != 0 && strstr(State.Satellites[tlepopupindex]->Name(), SearchBuf) == NULL)
                    {
                        tlepopupindex += 1;
                        continue;
                    }
                        
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", State.Satellites[tlepopupindex]->Name());
                    ImGui::TableSetColumnIndex(1);
                    if(State.Satellites[tlepopupindex]->use == false)
                    {
                        sprintf(SelectButtonTextBuf, "Select##TLE%d", tlepopupindex);
                        if(ImGui::Button(SelectButtonTextBuf, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                        {
                           State.Satellites[tlepopupindex]->use = true;
                           if(State.Satellites[tlepopupindex]->cal == false)
                                State.Satellites[tlepopupindex]->Refresh(State.Satellites[tlepopupindex]->tle, State.Satellites[tlepopupindex]->obs, true, true);
                           SatelliteModelInitialize(tlepopupindex);
                           mim::Initialize_SatContext();   // ← VisibleSats 재계산
                        }
                    }
                    else
                    {
                        sprintf(DeleteButtonTextBuf, "Delete##TLE%d", tlepopupindex);
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
                        if(ImGui::Button(DeleteButtonTextBuf, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                        {
                            State.Satellites[tlepopupindex]->use = false;
                            SatelliteModelDelete(tlepopupindex);
                            mim::Initialize_SatContext();   // ← VisibleSats 재계산
                        }
                        ImGui::PopStyleColor();
                    }
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d/%d %d:%d:%d",   State.Satellites[tlepopupindex]->_nextaos[0].AddHours(9).Month(), 
                                                    State.Satellites[tlepopupindex]->_nextaos[0].AddHours(9).Day(),
                                                    State.Satellites[tlepopupindex]->_nextaos[0].AddHours(9).Hour(),
                                                    State.Satellites[tlepopupindex]->_nextaos[0].AddHours(9).Minute(),
                                                    State.Satellites[tlepopupindex]->_nextaos[0].AddHours(9).Second());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%.3lf°", State.Satellites[tlepopupindex]->_max_elevation[0] * RAD_TO_DEG);
                }
                tlepopupindex += 1;
            }

            
            ImGui::EndTable();
        }
        if(ImGui::Button("Load TLE", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
        {
            
            if(DownloadTLE(State.tleinfolistup[NowTLE]->remote, State.tleinfolistup[NowTLE]->local))
            {
                State.NowTracking = false;
                
                console.AddLog("[OK]##TLE \"%s\" Download Completed.", State.tleinfolistup[NowTLE]->label);

                mim::svc::StartSatelliteInitialize(&sgp4check);
                // SatelliteInitialize(NULL);
                
            }
            else
            {
                console.AddLog("[ERROR]##TLE Download failed. Please check again.");
            }
            
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}




void Initialize_CMDLabels()
{   // Common type cmd
    snprintf(Templabels[0], 64, "no arg");
    snprintf(Templabels[1], 64, "u8");
    snprintf(Templabels[2], 64, "u16");
    snprintf(Templabels[3], 64, "u32");
    snprintf(Templabels[4], 64, "u64");
    snprintf(Templabels[5], 64, "s8");
    snprintf(Templabels[6], 64, "s16");
    snprintf(Templabels[7], 64, "s32");
    snprintf(Templabels[8], 64, "s64");
    snprintf(Templabels[9], 64, "float");

    snprintf(Templabels[10], 64, "EPS NoOp");
    snprintf(Templabels[11], 64, "EPS Reset Counters");
    snprintf(Templabels[12], 64, "EPS Get Counters");
    snprintf(Templabels[13], 64, "EPS Get App Data");
    snprintf(Templabels[14], 64, "EPS Report App Data");
    snprintf(Templabels[15], 64, "EPS_P60_Dock Set Channel Single");
    snprintf(Templabels[16], 64, "EPS_P60_Dock Get Channel Single");
    snprintf(Templabels[17], 64, "EPS_P60_Dock Set Channels");
    snprintf(Templabels[18], 64, "EPS_P60_Dock Get Channels");

    snprintf(Templabels[19], 64, "EPS_P60_PDU Set Channel Single");
    snprintf(Templabels[20], 64, "EPS_P60_PDU Get Channel Single");
    snprintf(Templabels[21], 64, "EPS_P60_PDU Set Channels");
    snprintf(Templabels[22], 64, "EPS_P60_PDU Get Channels");

    snprintf(Templabels[23], 64, "EPS_P60_ACU Set MPPT Mode");
    snprintf(Templabels[24], 64, "EPS_P60_ACU Get MPPT Mode");

    snprintf(Templabels[25], 64, "EPS_P60_Dock Get Table HK");
    snprintf(Templabels[26], 64, "EPS_P60_Dock Get Table Conf");
    snprintf(Templabels[27], 64, "EPS_P60_Dock Get Table Cal");

    snprintf(Templabels[28], 64, "EPS_P60_PDU Get Table HK");
    snprintf(Templabels[29], 64, "EPS_P60_PDU Get Table Conf");
    snprintf(Templabels[30], 64, "EPS_P60_PDU Get Table Cal");

    snprintf(Templabels[31], 64, "EPS_P60_ACU Get Table HK");
    snprintf(Templabels[32], 64, "EPS_P60_ACU Get Table Conf");
    snprintf(Templabels[33], 64, "EPS_P60_ACU Get Table Cal");

    snprintf(Templabels[34], 64, "EPS_P60_Dock Reset GND WDT");
    snprintf(Templabels[35], 64, "EPS_P60_PDU Reset GND WDT");
    snprintf(Templabels[36], 64, "EPS_P60_ACU Reset GND WDT");

    snprintf(Templabels[37], 64, "EPS_P60 Get Param");
    snprintf(Templabels[38], 64, "EPS_P60 Get Param Array");
    snprintf(Templabels[39], 64, "EPS_P60 Set Param");

    snprintf(Templabels[40], 64, "EPS_P60 Get Table");
    snprintf(Templabels[41], 64, "EPS_P60 Load Table");
    snprintf(Templabels[42], 64, "EPS_P60 Save Table");
    snprintf(Templabels[43], 64, "EPS_P60 Table Get Static");
    snprintf(Templabels[44], 64, "EPS_P60 Table Dump Static");

    snprintf(Templabels[45], 64, "EPS_P60 Get Dock Info");
    snprintf(Templabels[46], 64, "EPS_P60 Reset");


    // ADCS 

    snprintf(Templabels[47],  64, "ADCS NoOp");
    snprintf(Templabels[48],  64, "ADCS Reset Counters");
    snprintf(Templabels[49],  64, "ADCS Reset App Cmd Counters");
    snprintf(Templabels[50],  64, "ADCS Reset Device Cmd Counters");
    snprintf(Templabels[51],  64, "ADCS Set Communication Mode (CAN)");

    snprintf(Templabels[52],  64, "ADCS GPIO Enable High");
    snprintf(Templabels[53],  64, "ADCS GPIO Enable Low");
    snprintf(Templabels[54],  64, "ADCS GPIO Boot High");
    snprintf(Templabels[55],  64, "ADCS GPIO Boot Low");
    snprintf(Templabels[56],  64, "ADCS Exit Bootloader");

    snprintf(Templabels[57],  64, "ADCS Reset");
    snprintf(Templabels[58],  64, "ADCS Set Current Unix Time");
    snprintf(Templabels[59],  64, "ADCS Set Error Log Setting");
    snprintf(Templabels[60],  64, "ADCS Persist Config");

    snprintf(Templabels[61],  64, "ADCS Set Control & Estimation Mode");
    snprintf(Templabels[62],  64, "ADCS Disable Mag/RWL Maintenance");
    snprintf(Templabels[63],  64, "ADCS Set Reference IRC Vector");
    snprintf(Templabels[64],  64, "ADCS Set Reference LLH Target");
    snprintf(Templabels[65],  64, "ADCS Set Orbit Mode");
    snprintf(Templabels[66],  64, "ADCS Set Mag Deploy");
    snprintf(Templabels[67],  64, "ADCS Set Reference RPY");
    snprintf(Templabels[68],  64, "ADCS Set Open-Loop MTQ Command");
    snprintf(Templabels[69],  64, "ADCS Set Power State");
    snprintf(Templabels[70],  64, "ADCS Set Run Mode");
    snprintf(Templabels[71],  64, "ADCS Set Control Mode");
    snprintf(Templabels[72],  64, "ADCS Set Wheel Config");
    snprintf(Templabels[73],  64, "ADCS Set Satellite Config");
    snprintf(Templabels[74],  64, "ADCS Set Controller Config");
    snprintf(Templabels[75],  64, "ADCS Set MAG0 MMT Calib Config");
    snprintf(Templabels[76],  64, "ADCS Set Default Mode Config");
    snprintf(Templabels[77],  64, "ADCS Set Mounting Config");
    snprintf(Templabels[78],  64, "ADCS Set MAG1 MMT Calib Config");
    snprintf(Templabels[79],  64, "ADCS Set Estimator Config");
    snprintf(Templabels[80],  64, "ADCS Set Sat Orbit Param Config");
    snprintf(Templabels[81],  64, "ADCS Set Node Selection Config");
    snprintf(Templabels[82],  64, "ADCS Set MTQ Config");
    snprintf(Templabels[83],  64, "ADCS Set Estimation Mode");
    snprintf(Templabels[84],  64, "ADCS Set Operational State");
    snprintf(Templabels[85],  64, "ADCS Set Mag Sensing Element Config");
    snprintf(Templabels[86],  64, "ADCS Set Unsolicited TLM Msg Setup");
    snprintf(Templabels[87],  64, "ADCS Set Unsolicited Event Msg Setup");
    // Telemetry GET commands
    snprintf(Templabels[88],  64, "ADCS CC 101");
    snprintf(Templabels[89],  64, "ADCS Open-Loop Reaction Wheel Command");



    // Sequences
    snprintf(Templabels[126], 64, "ADCS Sequence: Detumbling");
    snprintf(Templabels[127], 64, "ADCS Sequence: Sun Pointing");
    snprintf(Templabels[128], 64, "ADCS Sequence: Velocity Pointing");
    snprintf(Templabels[129], 64, "ADCS Sequence: KSC Earth Pointing");
    snprintf(Templabels[130], 64, "ADCS Sequence: LGC Earth Pointing");
    snprintf(Templabels[131], 64, "ADCS Sequence: RPY Pointing");

    // Extra
    snprintf(Templabels[132], 64, "ADCS Error Log Clear");
    snprintf(Templabels[133], 64, "ADCS Get Current Unix Time (Internal)");

    snprintf(Templabels[134], 64, "ADCS Send HK");
    snprintf(Templabels[135], 64, "ADCS Send Beacon");


    // SC
    snprintf(Templabels[136], 64, "SC NoOp");
    snprintf(Templabels[137], 64, "SC Reset Counters");
    snprintf(Templabels[138], 64, "SC Start ATS");
    snprintf(Templabels[139], 64, "SC Stop ATS");
    snprintf(Templabels[140], 64, "SC Start RTS");
    snprintf(Templabels[141], 64, "SC Stop RTS");
    snprintf(Templabels[142], 64, "SC Disable RTS");
    snprintf(Templabels[143], 64, "SC Enable RTS");
    snprintf(Templabels[144], 64, "SC Switch ATS");
    snprintf(Templabels[145], 64, "SC Jump ATS Time");
    snprintf(Templabels[146], 64, "SC Set Continue-ATS-On-Failure");
    snprintf(Templabels[147], 64, "SC Append ATS");
    snprintf(Templabels[148], 64, "SC Manage Table");
    snprintf(Templabels[149], 64, "SC Start RTS Group");
    snprintf(Templabels[150], 64, "SC Stop RTS Group");
    snprintf(Templabels[151], 64, "SC Disable RTS Group");
    snprintf(Templabels[152], 64, "SC Enable RTS Group");

    // EO
    snprintf(Templabels[153], 64, "TO LAB NoOp");
    snprintf(Templabels[154], 64, "TO LAB Reset Status");
    snprintf(Templabels[155], 64, "TO LAB Add Packet(Enable Beacon RN)");
    snprintf(Templabels[156], 64, "TO LAB Send Data Types");
    snprintf(Templabels[157], 64, "TO LAB Remove Packet(Stop Beacon)");
    snprintf(Templabels[158], 64, "TO LAB Remove All Packet");    
    snprintf(Templabels[159], 64, "TO LAB Output Enable");

    snprintf(Templabels[160], 64, "UANT Get Status");
    snprintf(Templabels[161], 64, "UANT Burn Channel Command");
    snprintf(Templabels[162], 64, "UANT Set Settings Command");
    snprintf(Templabels[163], 64, "UANT Auto Deploy Command");

    snprintf(Templabels[164], 64, "Deploy Solar Panel 1");
    snprintf(Templabels[165], 64, "Deploy Solar Panel 2");
    snprintf(Templabels[166], 64, "Deploy Solar Panel 3");
    snprintf(Templabels[167], 64, "Deploy Solar Panel 4");   

    // KISSCAM
    snprintf(Templabels[168], 64, "PAYUZUC NoOp");
    snprintf(Templabels[169], 64, "PAYUZUC Reset Counters");
    snprintf(Templabels[170], 64, "PAYUZUC Ping");
    snprintf(Templabels[171], 64, "PAYUZUC Set Mode");
    snprintf(Templabels[172], 64, "PAYUZUC Memory Status");
    snprintf(Templabels[173], 64, "PAYUZUC Set Exposure");
    snprintf(Templabels[174], 64, "PAYUZUC Capture");
    snprintf(Templabels[175], 64, "PAYUZUC Download");
    snprintf(Templabels[176], 64, "PAYUZUC Read Register");
    snprintf(Templabels[177], 64, "PAYUZUC Write Register");
    snprintf(Templabels[178], 64, "PAYUZUC Download All");
    snprintf(Templabels[179], 64, "PAYUZUC Mosaic");
    snprintf(Templabels[180], 64, "PAYUZUC Download All Child");



 
    snprintf(Templabels[201], 64, "TO Delete");  
    snprintf(Templabels[202], 64, "TO Rename");  



    snprintf(Templabels[203], 64, "COSMIC FTP Send File Command");
    snprintf(Templabels[204], 64, "FM Get File Info");
    snprintf(Templabels[205], 64, "Beacon Mukbang (Useless but may try)");
    snprintf(Templabels[206], 64, "UTRX GNDWDT Clear");


    snprintf(Templabels[207], 64, "To COSMIC... From Yonsei...");
    snprintf(Templabels[208], 64, "COSMIC S/UHF Dual Emission For DoSsam");
    

    
    snprintf(Templabels[209], 64, "COSMIC EPS P31U SET CONFIG2");
    
    snprintf(Templabels[210], 64, "COSMIC EPS P31U CONFIG2");
    
    snprintf(Templabels[211], 64, "COSMIC EPS P31U SET OUT SINGLE");
    
    snprintf(Templabels[212], 64, "COSMIC SP DEPLOY");

    snprintf(Templabels[213], 64, "FM Get Dir List Pkt");

    snprintf(Templabels[214], 64, "COSMIC UEL Pi ON");
    snprintf(Templabels[215], 64, "COSMIC UEL Pi OFF");

    snprintf(Templabels[216], 64, "ES NoOp");
    snprintf(Templabels[217], 64, "ES Start App");
    snprintf(Templabels[218], 64, "ES Stop App");
    snprintf(Templabels[219], 64, "ES Reload App");
    snprintf(Templabels[220], 64, "ES Query One"); 
    snprintf(Templabels[221], 64, "ES Query All");


}

int CMDDataGenerator(uint32_t msgid, uint16_t fnccode, void *Requested, size_t RequestedSize) 
{
    ImGui::Checkbox("Checksum", &ChecksumState);
    ImGui::SameLine();
	ImGui::Checkbox("Scheduler", &SchedulerState);
	if(SchedulerState)
	{
        ChecksumState = true;
		ImGui::Text("Execution Time     : ");
		ImGui::SameLine();
		ImGui::InputScalar("##ExecutionTime", ImGuiDataType_U32, &ExecutionTimeBuf, NULL, NULL, "%u");
		ImGui::Text("Execution Window   : ");
		ImGui::SameLine();
		ImGui::InputScalar("##ExecutionWindow", ImGuiDataType_U32, &ExecutionWindowBuf, NULL, NULL, "%u");
		ImGui::Text("Entry ID           : ");
		ImGui::SameLine();
		ImGui::InputScalar("##EntryID", ImGuiDataType_U16, &EntryIDBuf, NULL, NULL, "%u");
		ImGui::Text("GroupID            : ");
		ImGui::SameLine();
		ImGui::InputScalar("##GroupID", ImGuiDataType_U16, &GroupIDBuf, NULL, NULL, "%u");
	}
	if(ImGui::Button("Generate", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
	{
		SatCMD[NowCMD] = new CmdGenerator_GS();
        console.AddLog("[DEBUG]##Generate : %u, %u, %d\n", msgid, fnccode, RequestedSize);
        if(RequestedSize == sizeof(CFE_MSG_CommandHeader))
            SatCMD[NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, NULL);
        else
		    SatCMD[NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, Requested + 8);
		if(SchedulerState)
        {
            SatCMD[NowCMD]->Scheduled = true;
            SatCMD[NowCMD]->Scheduling(ExecutionTimeBuf, ExecutionWindowBuf, EntryIDBuf, GroupIDBuf);
        }
        else if(!ChecksumState)
        {
            SatCMD[NowCMD]->Checksum = false;
        }
			
		NowCMD ++;

		console.AddLog("Generated Command Message.");
        memset(Requested, 0, RequestedSize);
		uint32_t ExecutionTimeBuf = 0;
		uint32_t ExecutionWindowBuf = 0;
		uint16_t EntryIDBuf = 0;
		uint16_t GroupIDBuf = 0;
        ChecksumState = true;
        SchedulerState = false;
	}
	ImGui::SameLine();
	if(ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
	{
		memset(Requested, 0, RequestedSize);
		return 0;
	}
    return 1;
	
}


bool popup_fds()
{
    ImGui::OpenPopup("FDS Manager");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(640, 300), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("FDS Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::Text("FDS Path");
        ImGui::InputText("##FDSPath", FDSFilePath, sizeof(FDSFilePath), 0, NULL);
        if (ImGui::Button("Load FDS", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
        {
            ReadTLELines(FDSFilePath, true);
            if(strlen(State.Fatellites->Name()) != 0)
            {
                State.Fatellites->use = true;
            }
            else
                console.AddLog("[ERROR]##Cannot Load FDS File.");
            // State.Fatellites = new SatelliteObject(Tle(), Yonsei, false);
            SatelliteModelInitialize(-1);
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}



bool popup_satinfo(int index)
{
    if(index < -1)
        return false;
    else if(index == -1)
        sprintf(SatWindowLabelBuf, "Details : %s", State.Fatellites->Name());
    else
        sprintf(SatWindowLabelBuf, "Details : %s", State.Satellites[index]->Name());

    ImGui::OpenPopup(SatWindowLabelBuf);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1080, 640), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal(SatWindowLabelBuf, NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        if (ImGui::BeginTable("##State.SatellitesInformations", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, 480)));
        {
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableSetupColumn("AOS",           ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("LOS",       ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Max Elevation",  ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableHeadersRow();
            
            // ImGui::TableNextColumn();
            if(index == -1)
            {
                for(int i = 0; i < 64; i++)
                {
                    if(State.Fatellites->_max_elevation[i] <= 0.0f || State.Satellites[i]->_max_elevation[i] > 90.0f)
                        break;
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d/%d %d:%d:%d",   State.Fatellites->_nextaos[i].AddHours(9).Month(), 
                                                    State.Fatellites->_nextaos[i].AddHours(9).Day(),
                                                    State.Fatellites->_nextaos[i].AddHours(9).Hour(),
                                                    State.Fatellites->_nextaos[i].AddHours(9).Minute(),
                                                    State.Fatellites->_nextaos[i].AddHours(9).Second());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d/%d %d:%d:%d",   State.Fatellites->_nextlos[i].AddHours(9).Month(), 
                                                    State.Fatellites->_nextlos[i].AddHours(9).Day(),
                                                    State.Fatellites->_nextlos[i].AddHours(9).Hour(),
                                                    State.Fatellites->_nextlos[i].AddHours(9).Minute(),
                                                    State.Fatellites->_nextlos[i].AddHours(9).Second());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.2f", State.Fatellites->_max_elevation[i] * RAD_TO_DEG);
                }
            }
            else
            {
                for(int i = 0; i < 64; i++)
                {
                    if(State.Satellites[index]->_max_elevation[i] <= 0.0f || State.Satellites[index]->_max_elevation[i] > 90.0f)
                        break;
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d/%d %d:%d:%d",   State.Satellites[index]->_nextaos[i].AddHours(9).Month(), 
                                                    State.Satellites[index]->_nextaos[i].AddHours(9).Day(),
                                                    State.Satellites[index]->_nextaos[i].AddHours(9).Hour(),
                                                    State.Satellites[index]->_nextaos[i].AddHours(9).Minute(),
                                                    State.Satellites[index]->_nextaos[i].AddHours(9).Second());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d/%d %d:%d:%d",   State.Satellites[index]->_nextlos[i].AddHours(9).Month(), 
                                                    State.Satellites[index]->_nextlos[i].AddHours(9).Day(),
                                                    State.Satellites[index]->_nextlos[i].AddHours(9).Hour(),
                                                    State.Satellites[index]->_nextlos[i].AddHours(9).Minute(),
                                                    State.Satellites[index]->_nextlos[i].AddHours(9).Second());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.2f", State.Satellites[index]->_max_elevation[i] * RAD_TO_DEG);
                }
            }
            ImGui::EndTable();
            
        }
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}

bool popup_s_band()
{
    ImGui::OpenPopup("S-Band Manager");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 540), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("S-Band Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::Text("S-Band GS PC Remote Control");
        
        ImGui::InputText("##s-bandcmd", inputbuf, sizeof(inputbuf), NULL, NULL);
        ImGui::SameLine();
        if(ImGui::Button("Send##s-band"))
        {
            sgs->cmd(inputbuf);
        }
        ImGui::BeginChild("##s-band", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::Text(sgs->buffer);
        ImGui::EndChild();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return true;
}

#include "miman_theme.h"
using namespace mim;

void ImGui_SatelliteTabs(float W) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, BG_DEEP);
    ImGui::BeginChild("##sattabs", ImVec2(W, 56), false,
                      ImGuiWindowFlags_NoScrollbar);

    for (int slot = 0; slot < State.VisibleSatsCount; slot++) {
        int sat = State.VisibleSats[slot];
        if (sat < 0 || !State.Satellites[sat]) continue;

        bool active = (sat == State.ActiveSatIndex);
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
                    State.Satellites[sat]->Name());

        char status[64];
        if (sat == GetNowTracking() && State.Engage) {
            snprintf(status, sizeof(status), "TRACKING");
        } else {
            DateTime now = DateTime::Now(false);
            double sec = (State.Satellites[sat]->_nextaos[0] - now).TotalSeconds();
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
        State.Display_TLE = true;   // 기존 popup 재활용
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void ImGui_StatusBar_Body(float W) {
    auto pill = [](const char* name, bool ok) {
        Dot(ok ? OK : ERR);
        ImGui::SameLine(0, 4);
        ImGui::TextUnformatted(name);
        ImGui::SameLine(0, 14);
    };
    pill("GS100",   State.GS100_Connection);
    pill("Rotator", State.RotatorConnection);
    pill("Switch",  State.SwitchConnection);
    pill("S-band",  State.SbandUse);

    ImGui::SameLine(0, 20);
    int sat = State.ActiveSatIndex;
    if (sat >= 0 && State.Satellites[sat]) {
        ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
        ImGui::Text("Track:");
        ImGui::PopStyleColor();
        ImGui::SameLine(0, 6);
        Dot(GetVisibleSatColor(sat));
        ImGui::SameLine(0, 4);
        ImGui::PushStyleColor(ImGuiCol_Text, GetVisibleSatColor(sat));
        ImGui::Text("%s", State.Satellites[sat]->Name());
        ImGui::PopStyleColor();
    }

    ImGui::SameLine(W - 520);
    auto mode = [](const char* n, bool on) {
        Dot(on ? OK : TEXT_DIM);
        ImGui::SameLine(0, 4);
        ImGui::PushStyleColor(ImGuiCol_Text, on ? OK : TEXT_DIM);
        ImGui::Text("%s", n);
        ImGui::PopStyleColor();
        ImGui::SameLine(0, 10);
    };
    mode("Engage",    State.Engage);
    mode("Doppler",   State.Doppler);
    mode("AutoPilot", State.Autopilot);
    mode("AMP",       State.AMPON);
}

// 기존 함수는 컨테이너(BeginChild)만 담당하고 본체 호출.
// 다른 패턴 (TrackWindow_Body, FrequencyWindow_Body 등)과 동일.
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

    if (ImGui::BeginTabBar("##functabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("1. Pre-Pass")) {
            State.ActiveFuncTab = 0;
            float colW = ImGui::GetContentRegionAvail().x / 3.0f - 8;
            float colH = ImGui::GetContentRegionAvail().y;
            ImGui::BeginChild("##col1", ImVec2(colW, colH), true);
                ImGui_TrackWindow_Body(Rh * 0.5f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col2", ImVec2(colW, colH), true);
                ImGui_FrequencyWindow_Body(Rh * 0.5f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col3", ImVec2(colW, colH), true);
                ImGui_RotatorWindow_Body(Rh * 0.5f);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("2. Pass Operation")) {
            State.ActiveFuncTab = 1;
            float colW = ImGui::GetContentRegionAvail().x / 3.0f - 8;
            float colH = ImGui::GetContentRegionAvail().y;
            ImGui::BeginChild("##col1", ImVec2(colW, colH), true);
                ImGui_ControlWindow_Body(Rh * 0.7f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col2", ImVec2(colW, colH), true);
                ImGui_CommandWindow_Body(Rh * 0.5f);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##col3", ImVec2(colW, colH), true);
                ImGui_BeaconWindow_Body(Rh * 0.65f);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

    // ───── 4. Console (탭 무관, 하단 고정)
    console.ChangeWindowSize(0, H - console_h, W, console_h);
    console.Draw("General Console", &Showstate, Rh * 0.6f);
}