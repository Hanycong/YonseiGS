// Model - GS100 파라미터 테이블 0/1/5 읽기·쓰기 스레드 (gs_rparam)
//
// 표마다 파라미터 목록(주소, 타입, 값 칸)을 RemoteParamTable 로 적어 두고 한꺼번에 읽고 쓴다
// (model/device/remote_param.h). 스레드 함수 Load_Paramtable* / Save_Paramtable* 의 선언은 YonseiGS_radial.h.
// 요청 조건은 예전과 같다: 노드 setup->gs100_node, 체크섬 0xB00B, 요청 하나에 100 ms.
#include <vector>

#include "YonseiGS_config.h"
#include "YonseiGS_radial.h"
#include "remote_param.h"

extern Console console;
extern Ptable_0 param0;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Setup * setup;

using mim::model::RemoteParam;
using mim::model::RemoteParamTable;
using mim::model::RemoteTable;

// 표 0: 시스템 설정
static const RemoteParamTable & Table0()
{
    static const RemoteParamTable table = {
        RemoteParam::Int8(param0.rssi_offset_addr, &param0.rssi_offset),
        RemoteParam::Int16(param0.max_temp_addr, &param0.max_temp),
        RemoteParam::Float(param0.bgndrssl_ema_addr, &param0.bgndrssl_ema),
        RemoteParam::Uint8(param0.csp_node_addr, &param0.csp_node),
        RemoteParam::Flag(param0.i2c_en_addr, &param0.i2c_en),
        RemoteParam::Flag(param0.can_en_addr, &param0.can_en),
        RemoteParam::Flag(param0.extptt_en_addr, &param0.extptt_en),
        RemoteParam::Flag(param0.led_en_addr, &param0.led_en),
        RemoteParam::Int8(param0.kiss_usart_addr, &param0.kiss_usart),
        RemoteParam::Int8AtOwnValue(&param0.gosh_usart),
        RemoteParam::Int8(param0.i2c_addr_addr, &param0.i2c_addr),
        RemoteParam::Uint16(param0.i2c_khz_addr, &param0.i2c_khz),
        RemoteParam::Uint16(param0.can_khz_addr, &param0.can_khz),
        RemoteParam::Uint16(param0.reboot_in_addr, &param0.reboot_in),
        RemoteParam::Uint32(param0.tx_inhibit_addr, &param0.tx_inhibit),
        RemoteParam::Int8(param0.log_store_addr, &param0.log_store),
        RemoteParam::Uint8(param0.tx_pwr_addr, &param0.tx_pwr),
        RemoteParam::Uint16(param0.max_tx_time_addr, &param0.max_tx_time),
        RemoteParam::Uint16(param0.max_idle_time_addr, &param0.max_idle_time),
        RemoteParam::String(param0.csp_rtable_addr, param0.csp_rtable, sizeof(param0.csp_rtable)),
    };
    return table;
}

// 표 1: 수신(RX) 설정
static const RemoteParamTable & Table1()
{
    static const RemoteParamTable table = {
        RemoteParam::Uint32(param1.freq_addr, &param1.freq),
        RemoteParam::Uint32(param1.baud_addr, &param1.baud),
        RemoteParam::Float(param1.modindex_addr, &param1.modindex),
        RemoteParam::Uint16(param1.guard_addr, &param1.guard),
        RemoteParam::Uint8(param1.pllrang_addr, &param1.pllrang),
        RemoteParam::Uint8(param1.mode_addr, &param1.mode),
        RemoteParam::Flag(param1.csp_hmac_addr, &param1.csp_hmac),
        RemoteParam::Flag(param1.csp_rs_addr, &param1.csp_rs),
        RemoteParam::Flag(param1.csp_crc_addr, &param1.csp_crc),
        RemoteParam::Flag(param1.csp_rand_addr, &param1.csp_rand),
        RemoteParam::String(param1.ax25_call_addr, param1.ax25_call, sizeof(param1.ax25_call)),
        RemoteParam::Uint32(param1.bw_addr, &param1.bw),
        RemoteParam::Int32(param1.afcrange_addr, &param1.afcrange),
    };
    return table;
}

// 표 5: 송신(TX) 설정
static const RemoteParamTable & Table5()
{
    static const RemoteParamTable table = {
        RemoteParam::Uint32(param5.freq_addr, &param5.freq),
        RemoteParam::Uint32(param5.baud_addr, &param5.baud),
        RemoteParam::Float(param5.modindex_addr, &param5.modindex),
        RemoteParam::Uint16(param5.guard_addr, &param5.guard),
        RemoteParam::Uint8(param5.pllrang_addr, &param5.pllrang),
        RemoteParam::Uint8(param5.mode_addr, &param5.mode),
        RemoteParam::Flag(param5.csp_hmac_addr, &param5.csp_hmac),
        RemoteParam::Flag(param5.csp_rs_addr, &param5.csp_rs),
        RemoteParam::Flag(param5.csp_crc_addr, &param5.csp_crc),
        RemoteParam::Flag(param5.csp_rand_addr, &param5.csp_rand),
        RemoteParam::String(param5.ax25_call_addr, param5.ax25_call, sizeof(param5.ax25_call)),
        RemoteParam::Uint8(param5.preamblen_addr, &param5.preamblen),
        RemoteParam::Uint8(param5.preambflags_addr, &param5.preambflags),
        RemoteParam::Uint8(param5.intfrmlen_addr, &param5.intfrmlen),
        RemoteParam::Uint8(param5.intfrmflags_addr, &param5.intfrmflags),
        RemoteParam::Int16(param5.rssibusy_addr, &param5.rssibusy),
        RemoteParam::Uint8(param5.kup_delay_addr, &param5.kup_delay),
        RemoteParam::Int16(param5.pa_level_addr, &param5.pa_level),
    };
    return table;
}

static RemoteTable Gs100Table(uint8_t table)
{
    return RemoteTable{ setup->gs100_node, table, 100 };
}

void * Load_Paramtable0(void*)
{
    RadBufFree();
    std::vector<bool> ok;
    int failed = Table0().LoadAll(Gs100Table(0), &ok);

    // 원래 코드의 결함을 그대로 둔다 (CLAUDE.md Known defects): i2c_addr 와 log_store 는 int8 로 읽은 값이 아니라
    // 그때의 uint8 임시 변수 값이 들어갔다. 그 값은 앞에서 읽은 csp_node (못 읽었으면 0) 이다.
    enum { kCspNode = 3, kI2cAddr = 10, kLogStore = 15 };
    const uint8_t last_uint8 = ok[kCspNode] ? param0.csp_node : 0;
    if (ok[kI2cAddr])  param0.i2c_addr = last_uint8;
    if (ok[kLogStore]) param0.log_store = last_uint8;

    RadBufFree();
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Read %d parameters of Table 0.", failed);
    return NULL;
}

void * Load_Paramtable1(void*)
{
    RadBufFree();
    int failed = Table1().LoadAll(Gs100Table(1));
    RadBufFree();
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Read %d parameters of Table 1.", failed);
    return NULL;
}

void * Load_Paramtable5(void*)
{
    RadBufFree();
    int failed = Table5().LoadAll(Gs100Table(5));
    RadBufFree();
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Read %d parameters of Table 5.", failed);
    return NULL;
}

void * Save_Paramtable0(void *)
{
    int failed = Table0().SaveAll(Gs100Table(0));
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Set %d parameters of Table 0.", failed);
    return NULL;
}

void * Save_Paramtable1(void *)
{
    int failed = Table1().SaveAll(Gs100Table(1));
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Set %d parameters of Table 1.", failed);
    return NULL;
}

void * Save_Paramtable5(void *)
{
    int failed = Table5().SaveAll(Gs100Table(5));
    if (failed > 0)
        console.AddLog("[ERROR]##Cannot Set %d parameters of Table 5.", failed);
    return NULL;
}
