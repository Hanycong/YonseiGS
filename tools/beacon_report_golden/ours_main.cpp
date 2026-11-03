// Beacon / Report 탭 화면 지문 하네스 (이 저장소 쪽)
//
// 본 프로그램 오브젝트(main.cpp 제외)에 링크해 ImGui_BeaconWindow 를 화면 없이 그리고
// 글자 출력을 기록한다. GS_nuri5 쪽(n5_main.cpp)과 같은 사례(cases.h)를 돌려 결과를 비교한다.
// 빌드/실행: tools/beacon_report_golden.sh
#include <signal.h>
#include <pthread.h>
#include "rec_imgui.h"
#define STB_IMAGE_IMPLEMENTATION     // main.cpp 가 하던 것
#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "beacon_view.h"
#include "sat_mode.h"

// ---- main.cpp 가 정의하던 전역 (main.cpp 는 링크하지 않는다) ----
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

// 5차 전역이 없는 예전 커밋과도 링크되게 (있으면 그쪽 정의가 쓰인다)
__attribute__((weak)) RPT_OpsData_t g_rpt_ops_data;
__attribute__((weak)) bool g_rpt_ops_data_valid;
__attribute__((weak)) EPS_Report_State_t g_eps_report_state;
__attribute__((weak)) EPS_RParam_Reassembly_State_t g_eps_rparam_reassembly;
__attribute__((weak)) BEE1012_Beacon_t * bee1012_beacon = (BEE1012_Beacon_t *)calloc(1, sizeof(BEE1012_Beacon_t));
__attribute__((weak)) UELYSYS_Beacon_t * uelysys_beacon = (UELYSYS_Beacon_t *)calloc(1, sizeof(UELYSYS_Beacon_t));
__attribute__((weak)) SD1SAT_Beacon_t * sd1sat_beacon = (SD1SAT_Beacon_t *)calloc(1, sizeof(SD1SAT_Beacon_t));

extern Beacon * beacon;
static Beacon * g_save_beacon;

static void SideSetSat(int sat) { mim::sat::SetCurrent(sat); }
static void SideBee1000Beacon(bool null)
{
    if (!g_save_beacon) g_save_beacon = beacon;
    beacon = null ? NULL : g_save_beacon;
}
static void * SideBee1000BeaconPtr(size_t * sz) { *sz = sizeof(Beacon); return g_save_beacon; }
static void SideDrawBeaconWindow() { ImGui_BeaconWindow(1.0f); }

#include "cases.h"

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    return RunCases(argc, argv);
}
