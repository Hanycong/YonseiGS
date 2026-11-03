// GS_nuri5 쪽 하네스: miman_coms.cpp 를 그대로 include 해서 static 함수(조립기/해석/덤프)를 부른다
#define STB_IMAGE_IMPLEMENTATION   // main.cpp 대신 stb_image 구현을 둔다
#include "miman_coms.cpp"

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

#define T_ASSEMBLE(tag, port, pkt, dir, prefix, saver) AssembleReportPortChunk(tag, port, pkt, dir, prefix, saver)
#define T_SD1SAT_ASSEMBLE(port, pkt) AssembleSD1SATReportPortChunk(port, pkt)
#define T_VIEW(rpt, port) UpdateReportViewFromReport(rpt, port)
#define T_SD1SAT_VIEW(rpt) UpdateReportViewFromSD1SATReport(rpt)
#define T_DUMP(fp, rpt, port) DumpReportPayloadParsed_ByMidCc(fp, rpt, port)
#include "driver.inc"

int main()
{
    g_out = fdopen(dup(1), "w");
    freopen("/dev/null", "w", stdout);   // 테스트 대상의 디버그 출력은 버린다
    RunCommon();
    fflush(g_out);
    return 0;
}
