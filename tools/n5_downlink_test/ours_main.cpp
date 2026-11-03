// 이 저장소 쪽 하네스: 5차 수신 함수(n5_*)를 부른다. 덤프/버퍼 함수는 static 이라 .cpp 를 include 한다
#define STB_IMAGE_IMPLEMENTATION   // main.cpp 대신 stb_image 구현을 둔다
#include "../../src/model/telemetry/n5_report_logger.cpp"
#include "../../src/model/telemetry/n5_beacon.cpp"
#include "../../src/model/telemetry/n5_report_assembler.cpp"
#include "n5_report_decoder.h"
#include "n5_report_assembler.h"

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

#define T_ASSEMBLE(tag, port, pkt, dir, prefix, saver) AssembleN5ReportChunk(tag, port, pkt, dir, prefix, saver)
#define T_SD1SAT_ASSEMBLE(port, pkt) AssembleSD1SATReportChunk(port, pkt)
#define T_VIEW(rpt, port) UpdateN5ReportView(rpt, port)
#define T_SD1SAT_VIEW(rpt) UpdateSD1SATReportView(rpt)
#define T_DUMP(fp, rpt, port) DumpReportPayloadParsed_ByMidCc(fp, rpt, port)
#include "driver.inc"

static int g_fail = 0;
static void Check(bool ok, const char * what)
{
    printf("check %s: %s\n", ok ? "OK  " : "FAIL", what);
    if (!ok) g_fail++;
}

static size_t CountFiles(const char * dir)
{
    std::vector<std::string> v; Walk(dir, v); return v.size();
}

// GS_nuri5 와 일부러 다르게 고친 부분
static void RunOursOnly()
{
    system("rm -rf ../data");

    // FTP 중 같은 초에 받은 비콘 3개 -> FTP 후 파일 3개 (GS_nuri5 는 초 단위 이름이라 1개)
    {
        uint8_t d[sizeof(BEE1012_Beacon_t)];
        for (int k = 0; k < 3; k++) {
            for (auto & x : d) x = Rnd();
            csp_packet_t * p = MakePkt(d, sizeof(d));
            BufferFtpBeacon(&g_ftp_bcn_bee1012, p);
            free(p);
        }
        FlushFtpBeaconBuffers();
        Check(CountFiles("../data/BEE1012/beacon/beacon_parsed") == 3, "FTP flush keeps 3 parsed beacons of the same second");
        Check(CountFiles("../data/BEE1012/beacon/beacon_raw") == 3, "FTP flush keeps 3 raw beacons");
        BEE1012_Beacon_t b;
        Check(CopyBEE1012Beacon(b) && memcmp(&b, d, sizeof(b)) == 0, "CopyBEE1012Beacon returns the last beacon");
        system("rm -rf ../data");
    }

    // 0x0825 리포트 3개 묶음 (UELYSYS) -> 해석 파일 3개 (GS_nuri5 는 초 단위 이름이라 1개)
    {
        Feed("case10", 10, ReportWire(0x0825, PAYUEL_ROMA_CMD_MID, 0, 2, 3, 524), 200,
             "../data/UELYSYS/report/report_raw", "report_raw_", UELYSYSReportSaver);
        Check(CountFiles("../data/UELYSYS/report/report_parsed") == 3, "multi-entry UELYSYS report saves 3 files");
        Check(CountFiles("../data/UELYSYS/report/report_raw") == 1, "multi-entry raw wire saved once");
        system("rm -rf ../data");
    }

    // 조립 중 포트가 바뀌면 (15 -> 10) 조각을 버리고 새로 시작
    {
        std::vector<uint8_t> a = ReportWire(0x0825, BEE1012_PAY_SLT_CMD_ID, 0, 32, 1, 524);
        std::vector<uint8_t> b = ReportWire(0x0825, PAYUEL_ROMA_CMD_MID, 1, 2, 1, 524);
        csp_packet_t * p = MakePkt(a.data(), 200);
        AssembleN5ReportChunk("case15", 15, p, "../data/T", "raw--", HookSaver);
        free(p);
        memset(&g_report_view, 0, sizeof(g_report_view));
        Feed("case10", 10, b, 200, "../data/T", "raw--", NULL);
        Check(g_report_view.valid && g_report_view.reflected_msg_id == PAYUEL_ROMA_CMD_MID,
              "port change during assembly drops partial report");
        system("rm -rf ../data");
    }

    // SD1SAT: DataLength 가 받은 길이보다 크면 받은 만큼만 (오프셋 16 부터)
    {
        uint8_t w[16 + 20];
        memset(w, 0xAB, sizeof(w));
        uint16_t mid = SD1SAT_CR_CMD_ID, dl = 300;
        memcpy(w + 4, &mid, 2); w[12] = 0; memcpy(w + 14, &dl, 2);
        SD1SATReport r;
        memset(&r, 0x55, sizeof(r));
        Check(ParseSD1SATReportWire(w, sizeof(w), r) && r.Data[19] == 0xAB && r.Data[20] == 0 && r.DataLength == 300,
              "SD1SAT parse copies only len-16 data bytes");
        Check(!ParseSD1SATReportWire(w, 15, r), "SD1SAT parse rejects wire shorter than 16 bytes");
    }

    // 파일 이름: 같은 밀리초여도 덮어쓰지 않는다
    {
        struct timeval tv = { 1700000000, 123000 };
        FILE * a = OpenTelemetryFile("../data/T", "x--", "--", &tv, "w");
        FILE * b = OpenTelemetryFile("../data/T", "x--", "--", &tv, "w");
        Check(a && b && CountFiles("../data/T") == 2, "same-ms names get a counter suffix");
        if (a) fclose(a);
        if (b) fclose(b);
        system("rm -rf ../data");
    }
    printf("ours-only: %d failure(s)\n", g_fail);
}

int main()
{
    csp_buffer_init(20, 512);
    g_out = fdopen(dup(1), "w");
    freopen("/dev/null", "w", stdout);   // 테스트 대상의 디버그 출력은 버린다
    RunCommon();
    RunOursOnly();
    fflush(g_out);
    return g_fail ? 1 : 0;
}
