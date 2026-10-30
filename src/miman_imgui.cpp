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

// void ImGui_MainMenu -> src/app/ 로 이동

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


// bool popup_setup(Setup * setup) -> view/popups/setup_popup.cpp 로 이동

//Functions
// bool popup_param_table0() -> view/popups/param_table0_popup.cpp 로 이동

// bool popup_param_table1() -> view/popups/param_table1_popup.cpp 로 이동

// bool popup_param_table5() -> view/popups/param_table5_popup.cpp 로 이동

// bool popup_load() -> view/popups/load_popup.cpp 로 이동

//For Temporary multiple tle operation

// bool popup_tle() -> view/popups/tle_popup.cpp 로 이동




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
