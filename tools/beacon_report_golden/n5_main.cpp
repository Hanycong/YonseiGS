// Beacon / Report 탭 화면 지문 하네스 (GS_nuri5 쪽)
//
// GS_nuri5 의 miman_imgui.cpp 를 직접 include 해서 ImGui_BeaconWindow 를 화면 없이 그리고
// 글자 출력을 기록한다 (사례는 cases.h, 이 저장소 쪽은 ours_main.cpp).
// 빌드/실행: tools/beacon_report_golden.sh
#include "rec_imgui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "miman_imgui.cpp"

FILE *log_ptr;
StateCheckUnit State;
Command * command;
Setup * setup = new Setup;
dlreqdata * request = (dlreqdata *)malloc(sizeof(dlreqdata));
bool Showstate = true;
volatile sig_atomic_t g_shutdown_requested = 0;
pthread_t p_thread[16];
pthread_mutex_t conn_lock = PTHREAD_MUTEX_INITIALIZER;
csp_debug_level_t debug_level;
char * fontfile = (char *)"../bin/font/Inconsolata.ttf";
void LockTcLog() {}
void UnlockTcLog() {}
bool SwitchTcLogFile(const char*) { return true; }

static BEE1000_Beacon_t * g_save_beacon;

static void SideSetSat(int sat) { current_sat_mode = sat; }
static void SideBee1000Beacon(bool null)
{
    if (!g_save_beacon) g_save_beacon = bee1000_beacon;
    bee1000_beacon = null ? NULL : g_save_beacon;
}
static void * SideBee1000BeaconPtr(size_t * sz) { *sz = sizeof(BEE1000_Beacon_t); return g_save_beacon; }
static void SideDrawBeaconWindow() { ImGui_BeaconWindow(1.0f); }

#include "cases.h"

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    return RunCases(argc, argv);
}
