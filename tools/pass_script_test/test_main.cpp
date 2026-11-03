// 패스 TC 스크립트(model/service/pass_script) 단위 테스트
//
//  - 16진수 바이트 읽기/쓰기, 스크립트 파일 저장 → 읽기 왕복, 잘못된 파일 거부
//  - 기본 스크립트를 보냈을 때 나가는 패킷이 예전 AutoPilot 하드코딩 순서와 같은지
//  - Stop(keep_going=false) 이 반복·대기 중에도 바로 먹는지
//  - 송신 조건(앙각·안테나·링크) 판정
//
// svc_tx 대신 아래 가짜 SendUplink 로 패킷을 기록한다 (svc_tx.cpp.o 는 링크하지 않는다).
// 빌드/실행: tools/pass_script_test.sh <OUT_DIR>   (실패가 있으면 종료 코드 1)
// main.cpp 가 하던 stb_image 구현 정의 (YonseiGS_model / YonseiGS_imgui 가 쓴다)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "pass_script.h"
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

extern int PingCounter;
extern int BeaconCounter;
extern Observer Yonsei;

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

using namespace mim::svc;

static int g_fail = 0;
#define CHECK(cond) do { if (cond) printf("ok   %s\n", #cond); else { printf("FAIL %s  (line %d)\n", #cond, __LINE__); g_fail++; } } while (0)

static bool Always() { return true; }
static int g_budget = 0;
static bool Budget() { return g_budget-- > 0; }
static volatile bool g_go = true;
static bool Go() { return g_go; }
static void * StopSoon(void *) { usleep(300000); g_go = false; return NULL; }

static double Now()
{
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

static bool SameScript(const std::vector<TcStep> & a, const std::vector<TcStep> & b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if (a[i].name != b[i].name || a[i].bytes != b[i].bytes || a[i].repeat != b[i].repeat || a[i].delay_s != b[i].delay_s)
            return false;
    return true;
}

static void WriteFile(const char * path, const char * text)
{
    FILE * f = fopen(path, "w");
    fputs(text, f);
    fclose(f);
}

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    std::string dir = argc > 1 ? argv[1] : "/tmp";
    std::string err;
    std::vector<uint8_t> b;

    // ---- 16진수
    CHECK(ParseHexBytes("18 75 c0 0x00, 01", b, err) && b == std::vector<uint8_t>({0x18, 0x75, 0xC0, 0x00, 0x01}));
    CHECK(ParseHexBytes("A", b, err) && b == std::vector<uint8_t>({0x0A}));
    CHECK(!ParseHexBytes("1G", b, err));
    CHECK(!ParseHexBytes("123", b, err));
    CHECK(!ParseHexBytes("   ", b, err));
    std::string many;
    for (int i = 0; i < TC_MAX_BYTES + 1; i++) many += "00 ";
    CHECK(!ParseHexBytes(many, b, err));
    CHECK(FormatHexBytes({0x18, 0x0a, 0xff}) == "18 0A FF");

    // ---- 저장 → 읽기 왕복
    PassScript & ps = CurrentPassScript();
    CHECK(SameScript(ps.Snapshot(), DefaultPassScript()));
    TcStep extra;
    extra.name = "NAME | WITH BAR";
    extra.bytes = {0x01, 0x02};
    extra.repeat = 1;
    extra.delay_s = 0;
    ps.Insert(ps.Count(), extra);
    std::string path = dir + "/script_roundtrip.txt";
    CHECK(ps.Save(path, err));
    std::vector<TcStep> saved = ps.Snapshot();
    ps.ResetToDefault();
    CHECK(ps.Load(path, err));
    std::vector<TcStep> loaded = ps.Snapshot();
    CHECK(loaded.size() == saved.size());
    CHECK(loaded.back().name == "NAME   WITH BAR");             // 구분자 '|' 는 저장할 때 공백으로
    loaded.back().name = saved.back().name;
    CHECK(SameScript(loaded, saved));
    CHECK(ps.Path() == path);

    // ---- 잘못된 파일은 거부하고 지금 스크립트를 그대로 둔다
    std::vector<TcStep> before = ps.Snapshot();
    const char * bad[] = {
        "A | 2 | 10\n",                         // 칸 부족
        "A | 0 | 10 | 01\n",                    // 반복 0
        "A | 2 | 601 | 01\n",                   // 대기 초과
        "A | 2 | 10 | 0Z\n",                    // 16진수 아님
        " | 2 | 10 | 01\n",                     // 이름 없음
        "# only comment\n",                     // 단계 없음
        "OK | 1 | 0 | 01\nB | x | 1 | 01\n",    // 2번째 줄 오류
    };
    for (const char * text : bad)
    {
        std::string p = dir + "/bad.txt";
        WriteFile(p.c_str(), text);
        bool ok = ps.Load(p, err);
        CHECK(!ok && SameScript(ps.Snapshot(), before));
    }
    WriteFile((dir + "/bad.txt").c_str(), "OK | 1 | 0 | 01\nB | x | 1 | 01\n");
    ps.Load(dir + "/bad.txt", err);
    CHECK(err.find("line 2") == 0);
    CHECK(!ps.Load(dir + "/no_such_file.txt", err));

    // ---- 기본 스크립트 송신 = 예전 AutoPilot 하드코딩 순서 (각 2회)
    const std::vector<std::vector<uint8_t>> legacy = {
        {0x18,0x75,0xC0,0x00,0x00,0x01,0x14,0x47}, {0x18,0x75,0xC0,0x00,0x00,0x01,0x17,0x44},
        {0x18,0x75,0xC0,0x00,0x00,0x01,0x1a,0x00}, {0x18,0x50,0xC0,0x00,0x00,0x01,0x06,0x70},
        {0x18,0x75,0xC0,0x00,0x00,0x05,0x1e,0xa2,0xe8,0x03,0x00,0x00},
        {0x18,0x75,0xC0,0x00,0x00,0x01,0x14,0x47}, {0x18,0x75,0xC0,0x00,0x00,0x01,0x17,0x44},
        {0x18,0x75,0xC0,0x00,0x00,0x01,0x1a,0x00},
    };
    std::vector<std::vector<uint8_t>> expect;
    for (const auto & cmd : legacy) { expect.push_back(cmd); expect.push_back(cmd); }
    g_sent.clear();
    g_head.clear();
    std::atomic<int> sent{0};
    for (TcStep st : DefaultPassScript())
    {
        CHECK(st.repeat == 2 && st.delay_s == 10);
        st.delay_s = 0;
        SendTcStep(st, Always, &sent);
    }
    CHECK(g_sent == expect);
    CHECK(sent == 16);
    bool heads_ok = true;
    for (size_t i = 0; i < g_head.size(); i++)
    {
        char want[48];
        snprintf(want, sizeof(want), "id=%u type=%u len=%zu", HVD_TEST, MIM_PT_TMTC_TEST, expect[i].size());
        heads_ok = heads_ok && g_head[i] == want;
    }
    CHECK(heads_ok);

    // ---- Stop: 반복 도중, 대기 도중
    TcStep five;
    five.name = "FIVE";
    five.bytes = {0x01};
    five.repeat = 5;
    five.delay_s = 0;
    g_budget = 3;
    CHECK(SendTcStep(five, Budget) == 3);
    five.repeat = 2;
    five.delay_s = 5;
    g_go = true;
    pthread_t t;
    pthread_create(&t, NULL, StopSoon, NULL);
    double t0 = Now();
    int n = SendTcStep(five, Go);
    double took = Now() - t0;
    pthread_join(t, NULL);
    CHECK(n == 1);
    CHECK(took < 1.0);

    // ---- 송신 조건
    OrbitData.NowTracking = false;
    RotatorFlags.RotatorConnection = true;
    RotatorFlags.Engage = true;
    LinkFlags.GS100_Connection = true;
    AutoPilotCfg.RequireLink = true;
    PingCounter = 0;
    BeaconCounter = 0;
    PassGate g = EvaluatePassGate(0);
    CHECK(!g.has_elevation && !g.elevation_ok);
    CHECK(g.Antenna() && !g.link_ok && !g.Ok());
    PingCounter = 1;
    CHECK(EvaluatePassGate(0).link_ok);
    PingCounter = 0;
    BeaconCounter = 2;
    CHECK(EvaluatePassGate(0).link_ok);
    BeaconCounter = 0;
    AutoPilotCfg.RequireLink = false;
    CHECK(EvaluatePassGate(0).link_ok);
    RotatorFlags.Engage = false;
    CHECK(!EvaluatePassGate(0).Antenna());
    RotatorFlags.Engage = true;
    LinkFlags.GS100_Connection = false;
    CHECK(!EvaluatePassGate(0).Antenna());
    LinkFlags.GS100_Connection = true;

    // 앙각: 실제 TLE 로 만든 위성의 SGP4 앙각과 비교, 최저 앙각 경계
    Tle tle("UELYSYS",
            "1 A0959U 26229E   26305.84573193  .00001059  00000-0  95767-4 0  9993",
            "2 A0959  97.6992  41.6264 0009539  11.5698 348.5742 14.97513189  3864");
    Observer obs(37.5640, 126.9370, 0.08);
    SatelliteObject * sat = new SatelliteObject(obs);
    sat->Refresh(tle, obs, false, false);
    OrbitData.Satellites[0] = sat;
    OrbitData.NowTracking = true;
    g = EvaluatePassGate(0);
    SGP4 sgp4(tle);
    Observer o2(37.5640, 126.9370, 0.08);
    double el = o2.GetLookAngle(sgp4.FindPosition(DateTime::Now(true))).elevation * RAD_TO_DEG;
    printf("     elevation gate %.3f / direct %.3f\n", g.elevation_deg, el);
    CHECK(g.has_elevation && fabs(g.elevation_deg - el) < 0.05);
    AutoPilotCfg.MinElevation = (float)(g.elevation_deg - 1.0);
    CHECK(EvaluatePassGate(0).elevation_ok && EvaluatePassGate(0).Ok());
    AutoPilotCfg.MinElevation = (float)(g.elevation_deg + 1.0);
    CHECK(!EvaluatePassGate(0).elevation_ok && !EvaluatePassGate(0).Ok());
    CHECK(!EvaluatePassGate(1).has_elevation);
    CHECK(!EvaluatePassGate(-1).has_elevation);
    OrbitData.NowTracking = false;

    printf("%s (%d failed)\n", g_fail ? "FAILED" : "ALL PASSED", g_fail);
    return g_fail ? 1 : 0;
}
