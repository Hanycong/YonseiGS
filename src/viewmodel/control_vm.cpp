#include "control_vm.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gs/param/rparam.h>
#include <csp/csp.h>

#include "miman_config.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "svc_tx.h"
#include "svc_radio.h"

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
extern char AmpTimeBuf[32];
extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

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
    FSWTle* MIM_TLEinfo = (FSWTle*)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
    int NowPingInfo = GetNowTracking();

    if (NowPingInfo < -1)
    {
        console.AddLog("[ERROR]##You Tried Ping without Tracking. Please Check Again.");
    }
    else if (NowPingInfo == -1)
    {
        FSWTleConverter(State.Fatellites->tle, MIM_TLEinfo);
        packetsign* TestPacket = PingInit(MIM_TLEinfo);
        mim::svc::SendUplink((void*)TestPacket);
    }
    else
    {
        FSWTleConverter(State.Satellites[NowPingInfo]->tle, MIM_TLEinfo);
        packetsign* TestPacket = PingInit(MIM_TLEinfo);
        mim::svc::SendUplink((void*)TestPacket);
    }
}

// "Send Telecommand" (팝업을 열 때 함께)
void ControlVM::PrintRoutingTable() { csp_rtable_print(); }

void ControlVM::StartBaudCalibration() { mim::svc::StartBaudCalibration(); }
void ControlVM::StartFreqCalibration() { mim::svc::StartFreqCalibration(); }

AmpButton ControlVM::Amp()
{
    AmpButton amp;
    amp.on = State.AMPON;
    if (!amp.on)
    {
        amp.timer = State.AmpTime > 0;
        if (amp.timer)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "Disabled  %M:%S", State.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP ON");
    }
    else
    {
        amp.timer = State.AmpTime > 0;
        if (amp.timer)
            strftime(AmpTimeBuf, sizeof(AmpTimeBuf), "AMP OFF %M:%S", State.AmpTM);
        else
            sprintf(AmpTimeBuf, "AMP OFF");
    }
    amp.label = AmpTimeBuf;
    return amp;
}

void ControlVM::TurnAmpOn()  { mim::svc::AmpOn(); }
void ControlVM::TurnAmpOff() { mim::svc::AmpOff(); }

// "CMD Manager"
void ControlVM::OpenCommandManager() { State.Display_CMD = true; }

// "Clear"
void ControlVM::ClearCommandQueue()
{
    NowCMD = 0;
    for (int i = 0; i < 256; i++)
    {
        if (SatCMD[i] != NULL)
        {
            delete SatCMD[i];
            SatCMD[i] = NULL;
        }
    }
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

int ControlVM::SatelliteCount() const              { return 3; }
const char * ControlVM::SatelliteName(int i) const { return mim::svc::SwitchSatelliteName(i); }
int * ControlVM::SelectedSatellite()               { return &selected_sat_; }

void ControlVM::SwitchToSelectedSatellite()
{
    switch_arg_ = selected_sat_;
    mim::svc::SwitchSatellite(&switch_arg_);
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

// "Load##rparam_load"
void CustomRparamVM::Load()
{
    if (type_index == 0)
    {
        // eps 안불러와지면 0x0BB0 시도!
        if (gs_rparam_get_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u8) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }
    else if (type_index == 1)
    {
        if (gs_rparam_get_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u16) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }
    else if (type_index == 2)
    {
        if (gs_rparam_get_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u32) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }
    else if (type_index == 3)
    {
        if (gs_rparam_get_int8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s8) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi8, node, table_id, addr, value_s8);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }

    else if (type_index == 4)
    {
        if (gs_rparam_get_int16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s16) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi16, node, table_id, addr, value_s16);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }

    else if (type_index == 5)
    {
        if (gs_rparam_get_int32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s32) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi32, node, table_id, addr, value_s32);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }

    else if (type_index == 6)
    {
        if (gs_rparam_get_float(node, table_id, addr, 0xB00B, setup->default_timeout, &value_float) == GS_OK)
            console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%f", node, table_id, addr, value_float);
        else
            console.AddLog("[ERROR]##Rparam Load Failed.");
    }




}

// "Save##rparam_save"
void CustomRparamVM::Save()
{
    if (type_index == 0)
    {
        if (gs_rparam_set_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, value_u8) == GS_OK)
            console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
        else
            console.AddLog("[ERROR]##Rparam Save Failed.");
    }
    else if (type_index == 1)
    {
        if (gs_rparam_set_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, value_u16) == GS_OK)
            console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
        else
            console.AddLog("[ERROR]##Rparam Save Failed.");
    }
    else if (type_index == 2)
    {
        if (gs_rparam_set_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, value_u32) == GS_OK)
            console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
        else
            console.AddLog("[ERROR]##Rparam Save Failed.");
    }
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
