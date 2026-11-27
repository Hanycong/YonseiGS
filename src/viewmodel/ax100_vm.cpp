#include "ax100_vm.h"

#include <inttypes.h>
#include <unistd.h>

#include "YonseiGS_config.h"
#include "ax100_radio.h"

extern Console console;

namespace mim {
namespace vm {

using mim::model::Ax100;

int16_t *  Ax100VM::RssiBusy()             { return Ax100().RssiBusy(); }
bool *     Ax100VM::CanEnable()            { return Ax100().CanEnable(); }
uint32_t * Ax100VM::TxInhibit()            { return Ax100().TxInhibit(); }
uint8_t *  Ax100VM::TxPower()              { return Ax100().TxPower(); }
char *     Ax100VM::CspRtable()            { return Ax100().CspRtable(); }
size_t     Ax100VM::CspRtableSize() const  { return Ax100().CspRtableSize(); }
uint16_t * Ax100VM::RxGuard()              { return Ax100().RxGuard(); }
uint16_t * Ax100VM::TxGuard()              { return Ax100().TxGuard(); }
uint32_t * Ax100VM::RxBaud()               { return Ax100().RxBaud(); }
uint32_t * Ax100VM::TxBaud()               { return Ax100().TxBaud(); }

uint16_t Ax100VM::BootCount() const    { return Ax100().BootCount(); }
uint32_t Ax100VM::TotalRxBytes() const { return Ax100().TotalRxBytes(); }
uint8_t  Ax100VM::ActiveConf() const   { return Ax100().ActiveConf(); }
int16_t  Ax100VM::LastRssi() const     { return Ax100().LastRssi(); }

// 버튼 동작: 원격 호출은 Ax100Radio, 로그 문구와 순서는 원래 버튼 블록 그대로

// Save 버튼 공통: 값 칸의 값을 보내고 결과를 콘솔에 남긴다.
// sleep(0.3) 은 정수 초로 잘려 실제로는 쉬지 않는다 (원래 코드 그대로, CLAUDE.md Known defects)
static void SaveAndReport(bool (mim::model::Ax100Radio::*save)(), bool settle = true)
{
    if (settle)
        sleep(0.3);
    if ((Ax100().*save)())
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct4_boot_count"
void Ax100VM::LoadBootCount()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadBootCount())
        console.AddLog("[OK]##AX100 Boot Count : %u", ax.BootCount());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct4_rssi_busy"
void Ax100VM::LoadRssiBusy()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    sleep(0.3);
    if(ax.LoadRssiBusy())
        console.AddLog("[OK]##Remote Param Control OK. rssi_busy : %"PRIu8, ax.CspNode());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct4_rssi_busy"
void Ax100VM::SaveRssiBusy() { SaveAndReport(&mim::model::Ax100Radio::SaveRssiBusy); }

// "Load##ax100_direct4_total_rx_bytes"
void Ax100VM::LoadTotalRxBytes()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadTotalRxBytes())
        console.AddLog("[OK]##Total RX Bytes : %u", ax.TotalRxBytes());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_can_en"
void Ax100VM::LoadCanEnable()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    sleep(0.3);
    if(ax.LoadCanEnable())
        console.AddLog("[OK]##Remote Param Control OK. can_en : %s", *ax.CanEnable() ? "true" : "false");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_can_en"
void Ax100VM::SaveCanEnable() { SaveAndReport(&mim::model::Ax100Radio::SaveCanEnable); }

// "Load##ax100_direct0_tx_inhibit"
void Ax100VM::LoadTxInhibit()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    sleep(0.3);
    if(ax.LoadTxInhibit())
        console.AddLog("[OK]##Remote Param Control OK. tx_inhibit : %"PRIu32, *ax.TxInhibit());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_tx_inhibit"
void Ax100VM::SaveTxInhibit() { SaveAndReport(&mim::model::Ax100Radio::SaveTxInhibit); }

// "Load##ax100_direct0_tx_pwr"
void Ax100VM::LoadTxPower()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadTxPower())
        console.AddLog("[OK]##Remote Param Control OK. tx_pwr : %"PRIu8, *ax.TxPower());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_tx_pwr"
void Ax100VM::SaveTxPower() { SaveAndReport(&mim::model::Ax100Radio::SaveTxPower, false); }

// "Load##ax100_direct0_csp_rtable"
void Ax100VM::LoadCspRtable()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadCspRtable())
        console.AddLog("[OK]##Remote Param Control OK. csp_rtable : %s", ax.CspRtable());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_csp_rtable"
void Ax100VM::SaveCspRtable() { SaveAndReport(&mim::model::Ax100Radio::SaveCspRtable, false); }

// "Load##ax100_direct0_active_conf"
void Ax100VM::LoadActiveConf()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadActiveConf())
        console.AddLog("[OK]##Remote Param Control OK. Active Conf : %"PRId8, ax.ActiveConf());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_last_rssi"
void Ax100VM::LoadLastRssi()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadLastRssi())
        console.AddLog("[OK]##Remote Param Control OK. Last RSSI : %"PRId16, ax.LastRssi());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct1_rx_guard"
void Ax100VM::LoadRxGuard()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadRxGuard())
        console.AddLog("[OK]##Remote Param Control OK. rx_guard : %"PRIu16, *ax.RxGuard());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct1_rx_guard"
void Ax100VM::SaveRxGuard() { SaveAndReport(&mim::model::Ax100Radio::SaveRxGuard, false); }

// "Load##ax100_direct5_tx_guard"
void Ax100VM::LoadTxGuard()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadTxGuard())
        console.AddLog("[OK]##Remote Param Control OK. tx_guard : %"PRIu16, *ax.TxGuard());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct5_tx_guard"
void Ax100VM::SaveTxGuard() { SaveAndReport(&mim::model::Ax100Radio::SaveTxGuard, false); }

// "Load##ax100_direct1_rx_baud"
void Ax100VM::LoadRxBaud()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadRxBaud())
        console.AddLog("[OK]##Remote Param Control OK. rx_baud : %"PRIu16, *ax.RxBaud());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct1_rxbaud"
void Ax100VM::SaveRxBaud() { SaveAndReport(&mim::model::Ax100Radio::SaveRxBaud, false); }

// "Load##ax100_direct5_txbaud"
void Ax100VM::LoadTxBaud()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.LoadTxBaud())
        console.AddLog("[OK]##Remote Param Control OK. tx_baud : %"PRIu16, *ax.TxBaud());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct5_txbaud"
void Ax100VM::SaveTxBaud() { SaveAndReport(&mim::model::Ax100Radio::SaveTxBaud, false); }

// "Save to FRAM##ax100_direct"
void Ax100VM::SaveToFram()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.SaveToFram())
    {
        console.AddLog("[OK]##Remote Param Control OK.");
    }
        
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Reset Watchdog##ax100_direct"
void Ax100VM::ResetGroundWatchdog()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    if(ax.ResetGroundWatchdog())
        console.AddLog("Remote Param Control OK.(No Reply)");
    else
        console.AddLog("[ERROR]##Remote Param Control Failed.(No Reply)");
}

// "Reboot AX100##ax100_direct"
void Ax100VM::Reboot()
{
    mim::model::Ax100Radio & ax = mim::model::Ax100();
    ax.Reboot();
    console.AddLog("Remote Param Control OK.(No Reply)");
}

Ax100VM & Ax100ViewModel()
{
    static Ax100VM vm;
    return vm;
}

} // namespace vm
} // namespace mim
