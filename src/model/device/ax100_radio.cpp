#include "ax100_radio.h"

#include <gs/param/rparam.h>

#include "miman_config.h"
#include "miman_coms.h"
#include "miman_radial.h"
#include "miman_csp.h"

extern Setup * setup;
extern Ptable_1 param1;
extern Ptable_5 param5;
extern Ptable_0 rparam0;
extern Ptable_1 rparam1;
extern Ptable_5 rparam5;
extern uint8_t remote_activeconf;
extern int16_t remote_lastrssi;
extern int16_t remote_rssibusy;

namespace mim {
namespace model {

int16_t *  Ax100Radio::RssiBusy()             { return &remote_rssibusy; }
bool *     Ax100Radio::CanEnable()            { return &rparam0.can_en; }
uint32_t * Ax100Radio::TxInhibit()            { return &rparam0.tx_inhibit; }
uint8_t *  Ax100Radio::TxPower()              { return &rparam0.tx_pwr; }
char *     Ax100Radio::CspRtable()            { return rparam0.csp_rtable; }
size_t     Ax100Radio::CspRtableSize() const  { return sizeof(rparam0.csp_rtable); }
uint16_t * Ax100Radio::RxGuard()              { return &rparam1.guard; }
uint16_t * Ax100Radio::TxGuard()              { return &rparam5.guard; }
uint32_t * Ax100Radio::RxBaud()               { return &rparam1.baud; }
uint32_t * Ax100Radio::TxBaud()               { return &rparam5.baud; }
uint8_t    Ax100Radio::CspNode() const        { return rparam0.csp_node; }

uint16_t Ax100Radio::BootCount() const    { return get_boot_count(); }
uint32_t Ax100Radio::TotalRxBytes() const { return get_rx_bytes(); }
uint8_t  Ax100Radio::ActiveConf() const   { return remote_activeconf; }
int16_t  Ax100Radio::LastRssi() const     { return remote_lastrssi; }

bool Ax100Radio::LoadBootCount()
{
    return gs_rparam_get_uint16(setup->ax100_node, 4, 0x0020, 0xB00B, setup->default_timeout, get_boot_count_address()) == GS_OK;
}

bool Ax100Radio::LoadRssiBusy()
{
    return gs_rparam_get_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, &remote_rssibusy) == GS_OK;
}

bool Ax100Radio::SaveRssiBusy()
{
    return gs_rparam_set_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, remote_rssibusy) == GS_OK;
}

bool Ax100Radio::LoadTotalRxBytes()
{
    return gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK;
}

bool Ax100Radio::LoadCanEnable()
{
    return gs_rparam_get_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (int8_t *)&rparam0.can_en) == GS_OK;
}

bool Ax100Radio::SaveCanEnable()
{
    return gs_rparam_set_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (bool)rparam0.can_en) == GS_OK;
}

bool Ax100Radio::LoadTxInhibit()
{
    return gs_rparam_get_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, &rparam0.tx_inhibit) == GS_OK;
}

bool Ax100Radio::SaveTxInhibit()
{
    return gs_rparam_set_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, rparam0.tx_inhibit) == GS_OK;
}

bool Ax100Radio::LoadTxPower()
{
    return gs_rparam_get_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, &rparam0.tx_pwr) == GS_OK;
}

bool Ax100Radio::SaveTxPower()
{
    return gs_rparam_set_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, rparam0.tx_pwr) == GS_OK;
}

bool Ax100Radio::LoadCspRtable()
{
    return gs_rparam_get_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK;
}

bool Ax100Radio::SaveCspRtable()
{
    return gs_rparam_set_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK;
}

bool Ax100Radio::LoadActiveConf()
{
    return gs_rparam_get_uint8(setup->ax100_node, 4, 0x0018, 0xB00B, setup->default_timeout, &remote_activeconf) == GS_OK;
}

bool Ax100Radio::LoadLastRssi()
{
    return gs_rparam_get_int16(setup->ax100_node, 4, 0x0004, 0xB00B, setup->default_timeout, &remote_lastrssi) == GS_OK;
}

bool Ax100Radio::LoadRxGuard()
{
    return gs_rparam_get_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, &rparam1.guard) == GS_OK;
}

bool Ax100Radio::SaveRxGuard()
{
    return gs_rparam_set_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, rparam1.guard) == GS_OK;
}

bool Ax100Radio::LoadTxGuard()
{
    return gs_rparam_get_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, &rparam5.guard) == GS_OK;
}

bool Ax100Radio::SaveTxGuard()
{
    return gs_rparam_set_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, rparam5.guard) == GS_OK;
}

bool Ax100Radio::LoadRxBaud()
{
    return gs_rparam_get_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, &rparam1.baud) == GS_OK;
}

bool Ax100Radio::SaveRxBaud()
{
    return gs_rparam_set_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, rparam1.baud) == GS_OK;
}

bool Ax100Radio::LoadTxBaud()
{
    return gs_rparam_get_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, &rparam5.baud) == GS_OK;
}

bool Ax100Radio::SaveTxBaud()
{
    return gs_rparam_set_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, rparam5.baud) == GS_OK;
}

bool Ax100Radio::SaveToFram()
{
    // 테이블 0 저장이 성공하면 1, 5 도 저장하고 지상국 GS100 의 guard 시간 설정을 다시 계산한다
    if(gs_rparam_save(setup->ax100_node, setup->default_timeout, 0, 0) == GS_OK)
    {
        gs_rparam_save(setup->ax100_node, setup->default_timeout, 1, 1);
        gs_rparam_save(setup->ax100_node, setup->default_timeout, 5, 5);
        set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
        return true;
    }
    return false;
}

bool Ax100Radio::ResetGroundWatchdog()
{
    return csp_transaction(CSP_PRIO_HIGH, setup->ax100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0;
}

void Ax100Radio::Reboot()
{
    csp_reboot(setup->ax100_node);
}

Ax100Radio & Ax100()
{
    static Ax100Radio ax100;
    return ax100;
}

} // namespace model
} // namespace mim
