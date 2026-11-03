// 운용 기능 테스트 (하드웨어 없이 도는 부분만)
//
//  - AMP: 타이머 재시작(AmpOn 이 쓰는 RestartAmpTimer)을 연달아 불러도 바로 돌아오는지,
//         Stop / 종료(InitializeRequired) 때 타이머 스레드가 100 ms 단위로 끝나는지
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

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    State.InitializeRequired = false;

    TestAmpTimer();

    printf("%s: %d failure(s)\n", g_fail ? "FAILED" : "PASSED", g_fail);
    return g_fail ? 1 : 0;
}
