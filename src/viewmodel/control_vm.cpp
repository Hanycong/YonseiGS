#include "control_vm.h"
#include "sat_mode.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <csp/csp.h>

#include "YonseiGS_config.h"
#include "svc_tx.h"
#include "svc_radio.h"
#include "remote_param.h"
#include "obc_ping.h"
#include "command_queue.h"
#include "track_target.h"
#include "ccsds_cmd.h"
#include "telecommand_sender.h"

extern StateCheckUnit State;
extern Console console;
extern Setup * setup;
extern char AmpTimeBuf[32];

namespace mim {
namespace vm {

// "Ping(AX100)"
void ControlVM::PingAx100()
{
    mim::svc::WaitTx();
    mim::svc::StartPingConsole();
}

// "Ping(OBC)"
void ControlVM::PingObc()
{
    mim::svc::WaitTx();
    FSWTle* MIM_TLEinfo = mim::model::NewFswTleBuffer();
    int NowPingInfo = mim::model::Tracking().Index();

    if (NowPingInfo < -1)
    {
        console.AddLog("[ERROR]##You Tried Ping without Tracking. Please Check Again.");
    }
    else
    {
        packetsign* TestPacket = mim::model::BuildObcPing(MIM_TLEinfo, NowPingInfo);
        mim::svc::SendUplink((void*)TestPacket);
    }
}

// UTRX 지상 와치독 클리어 CC (누리호 5차 GS 버튼 값. mission_defs.h UELYSYS_UTRX_GNDWDT_CLEAR_CC 와 같음)
static const uint8_t UTRX_GNDWDT_CLEAR_CC = 6;

// "UTRX WDT CLEAR" (누리호 5차 GS): 헤더만 있는 8바이트 명령. TC 기록은 업링크 스레드가 한다
void ControlVM::ClearUtrxWdt()
{
    const uint16_t msgid = UTRX_CMD_ID;     // 0x1850
    WriteSystemName(msgid);
    uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
    mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, UTRX_GNDWDT_CLEAR_CC);
    mim::tc::SendTelecommand(cmd, sizeof(cmd));
}

// "EPS WDT CLEAR ALL" (누리호 5차 GS): EPS P80 GND WDT CLEAR ALL
void ControlVM::ClearEpsWdtAll()
{
    const uint16_t msgid = EPS_CMD_ID;      // 0x1875
    WriteSystemName(msgid);
    EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t cmd;
    memset(&cmd, 0, sizeof(cmd));
    mim::tc::EncodeCfeCommand(&cmd, sizeof(cmd), msgid, EPS_P80_GND_WDT_CLEAR_ALL_CC);
    mim::tc::SendTelecommand(&cmd, sizeof(cmd));
}

// "Send Telecommand" (팝업을 열 때 함께)
void ControlVM::PrintRoutingTable() { csp_rtable_print(); }

void ControlVM::StartBaudCalibration() { mim::svc::StartBaudCalibration(); }
void ControlVM::StartFreqCalibration() { mim::svc::StartFreqCalibration(); }

AmpButton ControlVM::Amp()
{
    AmpButton amp;
    amp.on = LinkFlags.AMPON;
    if (!amp.on)
    {
        amp.timer = LinkFlags.AmpTime > 0;
        if (amp.timer)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "Disabled  %M:%S###AmpButton", LinkFlags.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP ON###AmpButton");
    }
    else
    {
        amp.timer = LinkFlags.AmpTime > 0;
        if (amp.timer)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "AMP OFF %M:%S###AmpButton", LinkFlags.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP OFF###AmpButton");
    }
    amp.label = AmpTimeBuf;
    return amp;
}

void ControlVM::TurnAmpOn()  { mim::svc::AmpOn(); }
void ControlVM::TurnAmpOff() { mim::svc::AmpOff(); }

// AMP 버튼 클릭 (누리호 5차 GS 와 같이): 켜져 있으면 끄고, 꺼져 있으면 켠다.
// "Disabled MM:SS" (꺼졌지만 타이머가 남음) 를 누르면 AmpOn 으로 다시 켜고 타이머를 새로 시작한다
// (예전에는 ampoff 를 한 번 더 실행했다).
void ControlVM::PressAmp(const AmpButton & amp)
{
    if (amp.on)
        TurnAmpOff();
    else
        TurnAmpOn();
}

// "CMD Manager"
void ControlVM::OpenCommandManager() { AppUi.Display_CMD = true; }

// "Clear"
void ControlVM::ClearCommandQueue()
{
    mim::model::Commands().Clear();
}

uint8_t * ControlVM::PingTargetNode() { return &ping_node_; }

// "Send Ping" ("Ping each node" 팝업)
void ControlVM::PingNode()
{
    mim::svc::WaitTx();

    uint8_t *arg = (uint8_t*)malloc(sizeof(uint8_t));
    *arg = ping_node_; 

    mim::svc::StartPingEveryNode((void*)arg);
}

int ControlVM::SatelliteCount() const              { return mim::sat::SAT_MODE_COUNT; }
const char * ControlVM::SatelliteName(int i) const { return mim::svc::SwitchSatelliteName(i); }

int * ControlVM::SelectedSatellite()
{
    selected_sat_ = mim::sat::Current();      // 다른 곳(AutoPilot 등)에서 바뀌어도 라디오 버튼이 따라간다
    return &selected_sat_;
}

void ControlVM::SwitchToSelectedSatellite()
{
    mim::svc::SwitchSatellite(selected_sat_);
}

uint8_t *  CustomRparamVM::Node()       { return &node; }
uint8_t *  CustomRparamVM::TableId()    { return &table_id; }
uint16_t * CustomRparamVM::Address()    { return &addr; }
int *      CustomRparamVM::TypeIndex()  { return &type_index; }
uint8_t *  CustomRparamVM::ValueU8()    { return &value_u8; }
uint16_t * CustomRparamVM::ValueU16()   { return &value_u16; }
uint32_t * CustomRparamVM::ValueU32()   { return &value_u32; }
int8_t *   CustomRparamVM::ValueS8()    { return &value_s8; }
int16_t *  CustomRparamVM::ValueS16()   { return &value_s16; }
int32_t *  CustomRparamVM::ValueS32()   { return &value_s32; }
float *    CustomRparamVM::ValueFloat() { return &value_float; }

// 형식 콤보의 순서: 0 uint8, 1 uint16, 2 uint32, 3 int8, 4 int16, 5 int32, 6 float
enum { kTypeU8, kTypeU16, kTypeU32, kTypeS8, kTypeS16, kTypeS32, kTypeFloat, kTypeCount };

// 고른 형식의 파라미터 (주소는 입력 칸, 값 칸은 형식별로 따로)
mim::model::RemoteParam CustomRparamVM::Selected()
{
    using mim::model::RemoteParam;
    switch (type_index)
    {
        case kTypeU8:    return RemoteParam::Uint8 (addr, &value_u8);
        case kTypeU16:   return RemoteParam::Uint16(addr, &value_u16);
        case kTypeU32:   return RemoteParam::Uint32(addr, &value_u32);
        case kTypeS8:    return RemoteParam::Int8  (addr, &value_s8);
        case kTypeS16:   return RemoteParam::Int16 (addr, &value_s16);
        case kTypeS32:   return RemoteParam::Int32 (addr, &value_s32);
        default:         return RemoteParam::Float (addr, &value_float);
    }
}

// 고른 형식의 값을 로그용 글자로
void CustomRparamVM::ValueText(char * out, size_t size) const
{
    switch (type_index)
    {
        case kTypeU8:    snprintf(out, size, "%" PRIu8,  value_u8);  break;
        case kTypeU16:   snprintf(out, size, "%" PRIu16, value_u16); break;
        case kTypeU32:   snprintf(out, size, "%" PRIu32, value_u32); break;
        case kTypeS8:    snprintf(out, size, "%" PRIi8,  value_s8);  break;
        case kTypeS16:   snprintf(out, size, "%" PRIi16, value_s16); break;
        case kTypeS32:   snprintf(out, size, "%" PRIi32, value_s32); break;
        default:         snprintf(out, size, "%f", value_float);     break;
    }
}

// "Load##rparam_load"
void CustomRparamVM::Load()
{
    if (type_index < 0 || type_index >= kTypeCount)
        return;
    // eps 안불러와지면 0x0BB0 시도!
    const mim::model::RemoteTable where{ node, table_id, setup->default_timeout };
    if (Selected().Load(where))
    {
        char value[48];
        ValueText(value, sizeof(value));
        console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%s", node, table_id, addr, value);
    }
    else
        console.AddLog("[ERROR]##Rparam Load Failed.");
}

// "Save##rparam_save" - uint8 / uint16 / uint32 만 보낸다 (원래 동작 그대로: 다른 형식은 아무 일도 하지 않는다)
void CustomRparamVM::Save()
{
    if (type_index != kTypeU8 && type_index != kTypeU16 && type_index != kTypeU32)
        return;
    const mim::model::RemoteTable where{ node, table_id, setup->default_timeout };
    if (Selected().Save(where))
    {
        char value[48];
        ValueText(value, sizeof(value));
        console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%s", node, table_id, addr, value);
    }
    else
        console.AddLog("[ERROR]##Rparam Save Failed.");
}

ControlVM & ControlViewModel()
{
    static ControlVM vm;
    return vm;
}

CustomRparamVM & CustomRparamViewModel()
{
    static CustomRparamVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
