#include "ax100_vm.h"

#include <inttypes.h>
#include <unistd.h>
#include <gs/param/rparam.h>

#include "miman_config.h"
#include "miman_coms.h"
#include "miman_radial.h"
#include "miman_csp.h"

extern Setup * setup;
extern Console console;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;
extern uint8_t remote_activeconf;
extern int16_t remote_lastrssi;
extern int16_t remote_rssibusy;

namespace mim {
namespace vm {

int16_t *  Ax100VM::RssiBusy()             { return &remote_rssibusy; }
bool *     Ax100VM::CanEnable()            { return &rparam0.can_en; }
uint32_t * Ax100VM::TxInhibit()            { return &rparam0.tx_inhibit; }
uint8_t *  Ax100VM::TxPower()              { return &rparam0.tx_pwr; }
char *     Ax100VM::CspRtable()            { return rparam0.csp_rtable; }
size_t     Ax100VM::CspRtableSize() const  { return sizeof(rparam0.csp_rtable); }
uint16_t * Ax100VM::RxGuard()              { return &rparam1.guard; }
uint16_t * Ax100VM::TxGuard()              { return &rparam5.guard; }
uint32_t * Ax100VM::RxBaud()               { return &rparam1.baud; }
uint32_t * Ax100VM::TxBaud()               { return &rparam5.baud; }

uint16_t Ax100VM::BootCount() const    { return get_boot_count(); }
uint32_t Ax100VM::TotalRxBytes() const { return get_rx_bytes(); }
uint8_t  Ax100VM::ActiveConf() const   { return remote_activeconf; }
int16_t  Ax100VM::LastRssi() const     { return remote_lastrssi; }

// "Load##ax100_direct4_boot_count"
void Ax100VM::LoadBootCount()
{
    if(gs_rparam_get_uint16(setup->ax100_node, 4, 0x0020, 0xB00B, setup->default_timeout, get_boot_count_address()) == GS_OK)
        console.AddLog("[OK]##AX100 Boot Count : %u", get_boot_count());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct4_rssi_busy"
void Ax100VM::LoadRssiBusy()
{
    sleep(0.3);
    if(gs_rparam_get_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, &remote_rssibusy) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. rssi_busy : %"PRIu8, rparam0.csp_node);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct4_rssi_busy"
void Ax100VM::SaveRssiBusy()
{
    sleep(0.3);
    if(gs_rparam_set_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, remote_rssibusy) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct4_total_rx_bytes"
void Ax100VM::LoadTotalRxBytes()
{
    if(gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK)
        console.AddLog("[OK]##Total RX Bytes : %u", get_rx_bytes());
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_can_en"
void Ax100VM::LoadCanEnable()
{
    sleep(0.3);
    if(gs_rparam_get_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (int8_t *)&rparam0.can_en) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. can_en : %s", rparam0.can_en ? "true" : "false");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_can_en"
void Ax100VM::SaveCanEnable()
{
    sleep(0.3);
    if(gs_rparam_set_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (bool)rparam0.can_en) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_tx_inhibit"
void Ax100VM::LoadTxInhibit()
{
    sleep(0.3);
    if(gs_rparam_get_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, &rparam0.tx_inhibit) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. tx_inhibit : %"PRIu32, rparam0.tx_inhibit);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_tx_inhibit"
void Ax100VM::SaveTxInhibit()
{
    sleep(0.3);
    if(gs_rparam_set_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, rparam0.tx_inhibit) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
    
}

// "Load##ax100_direct0_tx_pwr"
void Ax100VM::LoadTxPower()
{
    if(gs_rparam_get_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, &rparam0.tx_pwr) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. tx_pwr : %"PRIu8, rparam0.tx_pwr);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_tx_pwr"
void Ax100VM::SaveTxPower()
{
    if(gs_rparam_set_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, rparam0.tx_pwr) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_csp_rtable"
void Ax100VM::LoadCspRtable()
{
    if(gs_rparam_get_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. csp_rtable : %s", rparam0.csp_rtable);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct0_csp_rtable"
void Ax100VM::SaveCspRtable()
{
    if(gs_rparam_set_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct0_active_conf"
void Ax100VM::LoadActiveConf()
{
    if(gs_rparam_get_uint8(setup->ax100_node, 4, 0x0018, 0xB00B, setup->default_timeout, &remote_activeconf) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. Active Conf : %"PRId8, remote_activeconf);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
    
}

// "Load##ax100_direct0_last_rssi"
void Ax100VM::LoadLastRssi()
{
    if(gs_rparam_get_int16(setup->ax100_node, 4, 0x0004, 0xB00B, setup->default_timeout, &remote_lastrssi) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. Last RSSI : %"PRId16, remote_lastrssi);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
    
}

// "Load##ax100_direct1_rx_guard"
void Ax100VM::LoadRxGuard()
{
    if(gs_rparam_get_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, &rparam1.guard) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. rx_guard : %"PRIu16, rparam1.guard);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct1_rx_guard"
void Ax100VM::SaveRxGuard()
{
    if(gs_rparam_set_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, rparam1.guard) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct5_tx_guard"
void Ax100VM::LoadTxGuard()
{
    if(gs_rparam_get_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, &rparam5.guard) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. tx_guard : %"PRIu16, rparam5.guard);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct5_tx_guard"
void Ax100VM::SaveTxGuard()
{
    if(gs_rparam_set_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, rparam5.guard) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct1_rx_baud"
void Ax100VM::LoadRxBaud()
{
    if(gs_rparam_get_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, &rparam1.baud) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. rx_baud : %"PRIu16, rparam1.baud);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct1_rxbaud"
void Ax100VM::SaveRxBaud()
{
    if(gs_rparam_set_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, rparam1.baud) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Load##ax100_direct5_txbaud"
void Ax100VM::LoadTxBaud()
{
    if(gs_rparam_get_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, &rparam5.baud) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK. tx_baud : %"PRIu16, rparam5.baud);
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save##ax100_direct5_txbaud"
void Ax100VM::SaveTxBaud()
{
    if(gs_rparam_set_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, rparam5.baud) == GS_OK)
        console.AddLog("[OK]##Remote Param Control OK.");
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Save to FRAM##ax100_direct"
void Ax100VM::SaveToFram()
{
    if(gs_rparam_save(setup->ax100_node, setup->default_timeout, 0, 0) == GS_OK)
    {
        gs_rparam_save(setup->ax100_node, setup->default_timeout, 1, 1);
        gs_rparam_save(setup->ax100_node, setup->default_timeout, 5, 5);
        set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
        console.AddLog("[OK]##Remote Param Control OK.");
    }
        
    else
        console.AddLog("[ERROR]##Cannot Load Remote Parameter.");
}

// "Reset Watchdog##ax100_direct"
void Ax100VM::ResetGroundWatchdog()
{
    if(csp_transaction(CSP_PRIO_HIGH, setup->ax100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0)
        console.AddLog("Remote Param Control OK.(No Reply)");
    else
        console.AddLog("[ERROR]##Remote Param Control Failed.(No Reply)");
}

// "Reboot AX100##ax100_direct"
void Ax100VM::Reboot()
{
    csp_reboot(setup->ax100_node);
    console.AddLog("Remote Param Control OK.(No Reply)");
}

Ax100VM & Ax100ViewModel()
{
    static Ax100VM vm;
    return vm;
}

} // namespace vm
} // namespace mim
