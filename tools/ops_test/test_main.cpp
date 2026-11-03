// 운용 기능 테스트 (하드웨어 없이 도는 부분만)
//
//  - AMP: 타이머 재시작(AmpOn 이 쓰는 RestartAmpTimer)을 연달아 불러도 바로 돌아오는지,
//         Stop / 종료(InitializeRequired) 때 타이머 스레드가 100 ms 단위로 끝나는지
//  - FTP: 엔진(V1/V2/Ryu)별 워커가 TX 모드를 기다리는 동안 Stop 하면 바로 끝나는지, 중복 실행 차단,
//         사용자 Stop 뒤에는 실패 쿨다운이 없는지, 실패 뒤에는 30 초 쿨다운, 워커 인자가 시작 순간 경로를 갖는지
//         (CSP/무선 장치 없이: 실제 전송은 하지 않는다)
//  - filelisthandler (V1 다운링크 파일 경로) 경로 모양 / 모르는 종류 거부, 빈 FTP 경로의 file:// 기본값
//  - Control "UTRX WDT CLEAR" / "EPS WDT CLEAR ALL" 이 누리호 5차 GS 버튼과 같은 바이트를 보내는지
//
// svc_tx 대신 아래 가짜 SendUplink 로 패킷을 기록한다 (svc_tx.cpp.o 는 링크하지 않는다).
// 빌드/실행: tools/ops_test.sh <OUT_DIR>   (실패가 있으면 종료 코드 1)
// main.cpp 가 하던 stb_image 구현 정의 (YonseiGS_model / YonseiGS_imgui 가 쓴다)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "ftp_transfer.h"
#include <gs/util/error.h>
#include "control_vm.h"
#include "ftp_vm.h"
#include "YonseiGS_ftp.h"
#include <csp/csp.h>

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

extern Console console;

static std::vector<std::vector<uint8_t>> g_sent;
static std::vector<std::string> g_head;

namespace mim {
namespace svc {
int WaitTx() { return 0; }
int SendUplink(void * packet)
{
    packetsign * ps = (packetsign *)packet;
    char head[48];
    snprintf(head, sizeof(head), "id=%u type=%u len=%u", ps->Identifier, ps->PacketType, ps->Length);
    g_head.push_back(head);
    g_sent.push_back(std::vector<uint8_t>(ps->Data, ps->Data + ps->Length));
    return 0;
}
int StartPingScan() { return 0; }
int StartPingConsole() { return 0; }
int StartPingEveryNode(void *) { return 0; }
int StartDirectShell(void *) { return 0; }
} // namespace svc
} // namespace mim

static int g_fail = 0;
#define CHECK(cond) do { if (cond) printf("ok   %s\n", #cond); else { printf("FAIL %s  (line %d)\n", #cond, __LINE__); g_fail++; } } while (0)

static double Now()
{
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

// cond 가 될 때까지 최대 timeout_s 기다린다
template <typename F>
static bool WaitFor(F cond, double timeout_s)
{
    double end = Now() + timeout_s;
    while (Now() < end)
    {
        if (cond())
            return true;
        usleep(10000);
    }
    return cond();
}

static void TestAmpTimer()
{
    using namespace mim::svc;
    printf("# AMP timer\n");
    double t0 = Now();
    RestartAmpTimer();
    double t1 = Now();
    CHECK(t1 - t0 < 0.5);
    CHECK(AmpTimerRunning());
    CHECK(LinkFlags.AMPON);
    CHECK(WaitFor([] { return LinkFlags.AmpTime > 13 * 60; }, 1.0));

    // 타이머가 도는 중 다시 켜기 (예전: 남은 14 분 동안 join 해서 GUI 가 멈췄다)
    usleep(300000);
    t0 = Now();
    RestartAmpTimer();
    t1 = Now();
    printf("     second RestartAmpTimer took %.6f s\n", t1 - t0);
    CHECK(t1 - t0 < 0.5);
    CHECK(AmpTimerRunning());
    CHECK(LinkFlags.AMPON);

    // 세 번 연속
    t0 = Now();
    for (int i = 0; i < 3; i++)
        RestartAmpTimer();
    CHECK(Now() - t0 < 1.0);
    CHECK(AmpTimerRunning());

    // 정지: 스레드가 끝나고 표시가 꺼진다
    t0 = Now();
    StopAmpTimer();
    t1 = Now();
    CHECK(t1 - t0 < 0.5);
    CHECK(!AmpTimerRunning());
    CHECK(LinkFlags.AmpTime == 0);
    CHECK(!LinkFlags.AMPON);
    CHECK(!LinkFlags.AmpTimerStop);
    // 앞서 띄운 타이머가 하나라도 살아 있으면 0.3 초 안에 AmpTime 을 다시 쓴다
    usleep(300000);
    CHECK(LinkFlags.AmpTime == 0);
    CHECK(!LinkFlags.AMPON);

    // 정지 후 다시 정지 (돌고 있는 타이머 없음) 도 바로
    t0 = Now();
    StopAmpTimer();
    CHECK(Now() - t0 < 0.2);

    // 종료 경로: InitializeRequired 로도 끝난다 (StopAllThreads 가 join 하기 전)
    RestartAmpTimer();
    CHECK(AmpTimerRunning());
    State.InitializeRequired = true;
    CHECK(WaitFor([] { return !AmpTimerRunning(); }, 0.5));
    State.InitializeRequired = false;
    StopAmpTimer();     // join 만 (이미 끝났다)
    CHECK(!AmpTimerRunning());

    // 앰프 장치가 없으면 AmpOn 은 타이머를 건드리지 않고 바로 돌아온다
    t0 = Now();
    AmpOn();
    CHECK(Now() - t0 < 0.2);
    CHECK(!AmpTimerRunning());
}

static miman_ftp_transfer_status_t LastFtp()
{
    miman_ftp_transfer_status_t st;
    memset(&st, 0, sizeof(st));
    miman_ftp_get_last_transfer_status(&st);
    return st;
}

// TX 모드를 기다리는 워커를 띄우고 Stop -> 0.5 초 안에 끝나는지
static void StopWaitingTransfer(int version, bool upload, const char * engine, int port)
{
    using namespace mim::svc;
    printf("# FTP stop while waiting for TX: version %d %s\n", version, upload ? "upload" : "download");
    FtpShared.ftp_version = version;
    LinkFlags.uplink_mode = false;
    ftpinfo info;
    snprintf(info.local_path, sizeof(info.local_path), "file://local_%d", version);
    snprintf(info.remote_path, sizeof(info.remote_path), "file://remote_%d", version);

    CHECK(upload ? FtpUpload(&info) : FtpDownload(&info));
    // 시작 뒤 칸을 바꿔도 워커는 시작 순간 경로를 쓴다
    snprintf(info.local_path, sizeof(info.local_path), "file://changed");
    CHECK(FtpBusy());
    CHECK(!FtpStopping());
    usleep(200000);
    CHECK(FtpBusy());           // TX 모드를 기다리는 중
    CHECK(FtpShared.ftp_mode);
    // 중복 실행 차단
    CHECK(!FtpUpload(&info));
    CHECK(!FtpDownload(&info));

    double t0 = Now();
    FtpStop();
    CHECK(WaitFor([] { return !FtpBusy(); }, 0.5));
    printf("     stopped in %.3f s\n", Now() - t0);
    CHECK(!FtpShared.ftp_mode);
    CHECK(!FtpStopping());
    miman_ftp_transfer_status_t st = LastFtp();
    CHECK(st.user_stopped == 1);
    CHECK(st.cleanup_done == 1);
    CHECK(st.return_code == GS_ERROR_BREAK);
    CHECK(strcmp(st.engine, engine) == 0);
    CHECK(strcmp(st.direction, upload ? "UPLOAD" : "DOWNLOAD") == 0);
    char expect[64];
    snprintf(expect, sizeof(expect), "file://local_%d", version);
    CHECK(strcmp(st.local_path, expect) == 0);
    CHECK(st.host == FtpShared.dest_node);      // 목적지 = Dest Node 입력값
    CHECK(st.port == port);
}

static void TestFtp()
{
    using namespace mim::svc;
    CHECK(FtpShared.ftp_version == 2);      // 기본 엔진은 V2 (Ryu 는 고를 때만)
    CHECK(FtpShared.dest_node == 10);       // 기본 목적지 = OBC 기본 노드

    StopWaitingTransfer(2, true, "V2", 9);
    // 사용자 Stop 뒤에는 쿨다운 없이 바로 다시 (이어받기)
    StopWaitingTransfer(3, false, "RYU", 19);
    FtpShared.dest_node = 23;               // Dest Node 를 바꾸면 다음 전송부터 그 노드로
    StopWaitingTransfer(1, true, "V1", 15);
    StopWaitingTransfer(1, false, "V1", 15);
    StopWaitingTransfer(3, true, "RYU", 19);
    StopWaitingTransfer(2, false, "V2", 9);

    // 시작 직후 (워커가 돌기 전) Stop
    printf("# FTP stop right after start\n");
    FtpShared.ftp_version = 2;
    ftpinfo info;
    CHECK(FtpUpload(&info));
    FtpStop();
    CHECK(WaitFor([] { return !FtpBusy(); }, 0.5));
    CHECK(LastFtp().user_stopped == 1);

    // Stop 이 끝난 뒤 새 전송은 (abort 유지 스레드에) 멈추지 않는다
    printf("# FTP new transfer after stop is not aborted\n");
    usleep(200000);
    CHECK(FtpUpload(&info));
    usleep(300000);
    CHECK(FtpBusy());
    CHECK(!FtpStopping());
    FtpStop();
    CHECK(WaitFor([] { return !FtpBusy(); }, 0.5));

    // Stop 은 돌고 있는 FTP 가 없으면 아무것도 안 한다
    FtpStop();
    CHECK(!FtpBusy());
    CHECK(!FtpStopping());

    // 실패(로컬 파일 없음)는 30 초 쿨다운 -> 다음 시작 차단
    printf("# FTP failure cooldown\n");
    LinkFlags.uplink_mode = true;
    snprintf(info.local_path, sizeof(info.local_path), "file:///nonexistent_ops_test/none.bin");
    snprintf(info.remote_path, sizeof(info.remote_path), "file://none.bin");
    CHECK(FtpUpload(&info));
    CHECK(WaitFor([] { return !FtpBusy(); }, 5.0));
    miman_ftp_transfer_status_t st = LastFtp();
    printf("     failed upload retcode %d\n", st.return_code);
    CHECK(st.return_code != 0);
    CHECK(st.user_stopped == 0);
    CHECK(!FtpUpload(&info));       // 쿨다운
    LinkFlags.uplink_mode = false;
}

// 누리호 5차 GS 버튼 코드와 같은 계산 (헤더 8바이트, 체크섬 = 0xFF ^ 모든 바이트, [7] 은 0 으로 두고 계산)
static std::vector<uint8_t> Nuri5Cmd(uint16_t mid, uint8_t cc, size_t size)
{
    std::vector<uint8_t> c(size, 0);
    c[0] = mid >> 8; c[1] = mid & 0xFF; c[2] = 0xC0; c[3] = 0x00;
    c[4] = 0x00; c[5] = (uint8_t)(size - 7); c[6] = cc; c[7] = 0;
    uint8_t cs = 0xFF;
    for (uint8_t b : c) cs ^= b;
    c[7] = cs;
    return c;
}

static void TestWdtButtons()
{
    printf("# Control WDT buttons\n");
    g_sent.clear();
    g_head.clear();
    mim::vm::ControlViewModel().ClearUtrxWdt();
    mim::vm::ControlViewModel().ClearEpsWdtAll();
    CHECK(g_sent.size() == 2);
    if (g_sent.size() != 2)
        return;
    std::vector<uint8_t> utrx = {0x18, 0x50, 0xC0, 0x00, 0x00, 0x01, 0x06, 0x70};
    CHECK(g_sent[0] == utrx);
    CHECK(g_sent[0] == Nuri5Cmd(0x1850, 6, 8));
    CHECK(g_sent[1] == Nuri5Cmd(0x1875, EPS_P80_GND_WDT_CLEAR_ALL_CC, sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t)));
    CHECK(g_sent[1].size() == 8 && g_sent[1][6] == 31);
    char head[48];
    snprintf(head, sizeof(head), "id=%u type=%u len=%u", HVD_TEST, MIM_PT_TMTC_TEST, 8);
    CHECK(g_head[0] == head);
    CHECK(g_head[1] == head);
    for (size_t i = 0; i < g_sent.size(); i++)
    {
        printf("     ");
        for (uint8_t b : g_sent[i]) printf("%02X ", b);
        printf("\n");
    }
}

static bool EndsWith(const char * s, const char * tail)
{
    size_t a = strlen(s), b = strlen(tail);
    return a >= b && strcmp(s + a - b, tail) == 0;
}

static void TestFtpPaths()
{
    printf("# FTP paths\n");
    uint8_t buf[sizeof(filelist) + 2 * sizeof(uint32_t)];
    filelist * list = (filelist *)buf;
    list->filenum = 2;
    list->file[0] = 5;
    list->file[1] = 12345678;
    ftpinfo out;
    CHECK(filelisthandler(list, MIM_DLTYPE_HK, MIM_DLSTAT_NEW, 0, &out));
    CHECK(strcmp(out.remote_path, "/mnt/ds/hk/hktlm00000005.hk") == 0);
    CHECK(strncmp(out.local_path, "data/hk/__", 10) == 0 && EndsWith(out.local_path, "__hktlm00000005.hk"));
    CHECK(strcmp(out.name, "00000005") == 0);
    printf("     %s <- %s\n", out.local_path, out.remote_path);
    CHECK(filelisthandler(list, MIM_DLTYPE_CTRLO, MIM_DLSTAT_OLD, 1, &out));
    CHECK(strcmp(out.remote_path, "/mnt/ds/ctrlo/sent/raw12345678.raw") == 0);
    CHECK(filelisthandler(list, MIM_DLTYPE_AOD, MIM_DLSTAT_ING, 0, &out));
    CHECK(strcmp(out.remote_path, "/mnt/ds/aod/input/aodtlm00000005.aod") == 0);
    CHECK(!filelisthandler(list, 99, MIM_DLSTAT_NEW, 0, &out));
    CHECK(!filelisthandler(list, MIM_DLTYPE_HK, 99, 0, &out));
    CHECK(!filelisthandler(list, MIM_DLTYPE_HK, MIM_DLSTAT_NEW, 0, NULL));

    // 빈 경로만 file:// 로 (칸의 기본 경로는 그대로)
    mim::vm::FtpVM & vm = mim::vm::FtpViewModel();
    char keep[256];
    snprintf(keep, sizeof(keep), "%s", vm.SelectedFile()->local_path);
    vm.SelectedFile()->remote_path[0] = 0;
    vm.DefaultEmptyPaths();
    CHECK(strcmp(vm.SelectedFile()->local_path, keep) == 0);
    CHECK(strcmp(vm.SelectedFile()->remote_path, "file://") == 0);
}

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    State.InitializeRequired = false;

    TestAmpTimer();
    TestFtp();
    TestWdtButtons();
    TestFtpPaths();

    printf("%s: %d failure(s)\n", g_fail ? "FAILED" : "PASSED", g_fail);
    return g_fail ? 1 : 0;
}
