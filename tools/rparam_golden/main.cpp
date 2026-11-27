// 원격 파라미터(GomSpace rparam) 호출 기록 하네스
//
// GS100 파라미터 테이블 0/1/5 읽기·쓰기, AX100 원격 파라미터, Control 창 Custom RParam 이
// 라이브러리에 어떤 호출을 하는지(노드, 테이블, 주소, 타입, 체크섬, 대기 시간, 크기, 쓰는 값)와
// 그 결과로 값 칸과 콘솔 로그가 어떻게 되는지를 글자로 남긴다. 리팩토링 전후 결과를 diff 한다.
//
// gs_rparam_get / gs_rparam_set (gs_rparam_get_uint8 등은 모두 이 둘을 부른다)를 가짜로 바꾼다:
//   get: 주소·타입으로 정한 값을 돌려준다. 실패 모드에서는 (addr % 5 == 3) 인 주소가 실패한다
//   set: 받은 값을 기록한다. 실패 모드에서는 같은 규칙으로 실패한다
// 콘솔 로그는 Console::AddLog 를 가짜로 바꿔 받는다.
// 빌드/실행: tools/rparam_golden.sh <OUT_DIR>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_radial.h"
#include <gs/util/error.h>
#include <gs/param/rparam.h>
#include <csp/csp.h>
#include "ax100_radio.h"
#include "ax100_vm.h"
#include "control_vm.h"

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

extern Ptable_0 param0;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;

static bool g_fail_mode = false;

// ---- 가짜 콘솔 ----
void Console::AddLog(const char * fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    printf("  LOG %s\n", buf);
}

// ---- 가짜 rparam ----
static bool Fails(uint16_t addr) { return g_fail_mode && (addr % 5 == 3); }

gs_error_t gs_rparam_get(uint8_t node, gs_param_table_id_t table_id, uint16_t addr, gs_param_type_t type,
                         uint16_t checksum, uint32_t timeout_ms, void * value, size_t value_size)
{
    const bool fail = Fails(addr);
    printf("  GET node=%u table=%u addr=0x%04X type=%d cksum=0x%04X timeout=%u size=%zu -> %s\n",
           node, (unsigned)table_id, addr, (int)type, checksum, timeout_ms, value_size, fail ? "FAIL" : "ok");
    if (fail)
        return GS_ERROR_TIMEOUT;
    uint8_t * p = (uint8_t *)value;
    if (type == GS_PARAM_STRING)
    {
        snprintf((char *)p, value_size, "s%u_%04X", (unsigned)table_id, addr);
        return GS_OK;
    }
    for (size_t i = 0; i < value_size; i++)
        p[i] = (uint8_t)(0x11 + addr * 3 + table_id * 7 + i * 5);
    return GS_OK;
}

gs_error_t gs_rparam_set(uint8_t node, gs_param_table_id_t table_id, uint16_t addr, gs_param_type_t type,
                         uint16_t checksum, uint32_t timeout_ms, const void * value, size_t value_size)
{
    const bool fail = Fails(addr);
    printf("  SET node=%u table=%u addr=0x%04X type=%d cksum=0x%04X timeout=%u size=%zu value=",
           node, (unsigned)table_id, addr, (int)type, checksum, timeout_ms, value_size);
    const uint8_t * p = (const uint8_t *)value;
    for (size_t i = 0; i < value_size; i++)
        printf("%02X", p[i]);
    printf(" -> %s\n", fail ? "FAIL" : "ok");
    return fail ? GS_ERROR_TIMEOUT : GS_OK;
}

// 구조체 내용을 16진수로 (값 칸이 어떻게 바뀌었는지)
template <typename T>
static void Dump(const char * name, const T & v)
{
    const uint8_t * p = (const uint8_t *)&v;
    printf("  %s =", name);
    for (size_t i = 0; i < sizeof(T); i++)
        printf("%s%02X", (i % 32 == 0) ? "\n    " : "", p[i]);
    printf("\n");
}

int main()
{
    log_ptr = fopen("/dev/null", "w");
    setup->gs100_node = 20;
    setup->ax100_node = 5;
    setup->default_timeout = 1234;

    const Ptable_0 p0 = param0; const Ptable_1 p1 = param1; const Ptable_5 p5 = param5;
    const Ptable_0 r0 = rparam0; const Ptable_1 r1 = rparam1; const Ptable_5 r5 = rparam5;

    for (int mode = 0; mode < 2; mode++)
    {
        g_fail_mode = (mode == 1);
        printf("######## %s ########\n", g_fail_mode ? "FAIL MODE (addr %% 5 == 3 fails)" : "ALL OK");
        // 값 칸을 처음 상태로 (Ptable 은 주소 칸이 const 라 대입이 안 되므로 바이트로 되돌린다)
        memcpy((void *)&param0, &p0, sizeof(p0)); memcpy((void *)&param1, &p1, sizeof(p1)); memcpy((void *)&param5, &p5, sizeof(p5));
        memcpy((void *)&rparam0, &r0, sizeof(r0)); memcpy((void *)&rparam1, &r1, sizeof(r1)); memcpy((void *)&rparam5, &r5, sizeof(r5));

        printf("== Load_Paramtable0\n"); Load_Paramtable0(NULL); Dump("param0", param0);
        printf("== Load_Paramtable1\n"); Load_Paramtable1(NULL); Dump("param1", param1);
        printf("== Load_Paramtable5\n"); Load_Paramtable5(NULL); Dump("param5", param5);
        printf("== Save_Paramtable0\n"); Save_Paramtable0(NULL);
        printf("== Save_Paramtable1\n"); Save_Paramtable1(NULL);
        printf("== Save_Paramtable5\n"); Save_Paramtable5(NULL);

        mim::model::Ax100Radio & ax = mim::model::Ax100();
#define AX(m) printf("== Ax100Radio::" #m " -> %d\n", (int)ax.m())
        AX(LoadBootCount); AX(LoadRssiBusy); AX(SaveRssiBusy); AX(LoadTotalRxBytes); AX(LoadCanEnable); AX(SaveCanEnable);
        AX(LoadTxInhibit); AX(SaveTxInhibit); AX(LoadTxPower); AX(SaveTxPower); AX(LoadCspRtable); AX(SaveCspRtable);
        AX(LoadActiveConf); AX(LoadLastRssi); AX(LoadRxGuard); AX(SaveRxGuard); AX(LoadTxGuard); AX(SaveTxGuard);
        AX(LoadRxBaud); AX(SaveRxBaud); AX(LoadTxBaud); AX(SaveTxBaud);
#undef AX
        printf("  ax values: rssi_busy=%d can=%d inhibit=%u power=%u rtable=\"%s\" rxguard=%u txguard=%u rxbaud=%u txbaud=%u node=%u boot=%u rxbytes=%u conf=%u rssi=%d\n",
               *ax.RssiBusy(), (int)*ax.CanEnable(), *ax.TxInhibit(), *ax.TxPower(), ax.CspRtable(), *ax.RxGuard(), *ax.TxGuard(),
               *ax.RxBaud(), *ax.TxBaud(), ax.CspNode(), ax.BootCount(), ax.TotalRxBytes(), ax.ActiveConf(), ax.LastRssi());
        Dump("rparam0", rparam0); Dump("rparam1", rparam1); Dump("rparam5", rparam5);

        mim::vm::Ax100VM & vm = mim::vm::Ax100ViewModel();
#define VM(m) printf("== Ax100VM::" #m "\n"); vm.m()
        VM(LoadBootCount); VM(LoadRssiBusy); VM(SaveRssiBusy); VM(LoadTotalRxBytes); VM(LoadCanEnable); VM(SaveCanEnable);
        VM(LoadTxInhibit); VM(SaveTxInhibit); VM(LoadTxPower); VM(SaveTxPower); VM(LoadCspRtable); VM(SaveCspRtable);
        VM(LoadActiveConf); VM(LoadLastRssi); VM(LoadRxGuard); VM(SaveRxGuard); VM(LoadTxGuard); VM(SaveTxGuard);
        VM(LoadRxBaud); VM(SaveRxBaud); VM(LoadTxBaud); VM(SaveTxBaud);
#undef VM

        // Control 창 Custom RParam: 타입 7가지 x (실패 안 하는 주소, 실패하는 주소)
        mim::vm::CustomRparamVM cr;
        const uint16_t addrs[2] = { 0x0010, 0x0017 };   // 0x17 = 23, 23 % 5 == 3
        for (int a = 0; a < 2; a++)
            for (int t = 0; t < 7; t++)
            {
                *cr.Node() = 7; *cr.TableId() = 4; *cr.Address() = addrs[a]; *cr.TypeIndex() = t;
                printf("== CustomRparam Load type=%d addr=0x%04X\n", t, addrs[a]); cr.Load();
                printf("   values u8=%u u16=%u u32=%u s8=%d s16=%d s32=%d f=%08X\n", *cr.ValueU8(), *cr.ValueU16(), *cr.ValueU32(),
                       *cr.ValueS8(), *cr.ValueS16(), *cr.ValueS32(), *(uint32_t *)cr.ValueFloat());
                printf("== CustomRparam Save type=%d addr=0x%04X\n", t, addrs[a]); cr.Save();
            }

    }
    return 0;
}
