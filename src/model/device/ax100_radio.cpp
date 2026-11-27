#include "ax100_radio.h"

#include <gs/param/rparam.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_csp.h"
#include "remote_param.h"

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

// AX100 의 파라미터 표가 있는 곳 (노드와 대기 시간은 지상국 설정)
static RemoteTable Ax100Table(uint8_t table)
{
    return RemoteTable{ setup->ax100_node, table, setup->default_timeout };
}

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
    return RemoteParam::Uint16(0x0020, get_boot_count_address()).Load(Ax100Table(4));
}

bool Ax100Radio::LoadRssiBusy()
{
    return RemoteParam::Int16(0x0048, &remote_rssibusy).Load(Ax100Table(5));
}

bool Ax100Radio::SaveRssiBusy()
{
    return RemoteParam::Int16(0x0048, &remote_rssibusy).Save(Ax100Table(5));
}

bool Ax100Radio::LoadTotalRxBytes()
{
    return RemoteParam::Uint32(0x003C, get_rx_bytes_address()).Load(Ax100Table(4));
}

bool Ax100Radio::LoadCanEnable()
{
    return RemoteParam::Int8(rparam0.can_en_addr, (int8_t *)&rparam0.can_en).Load(Ax100Table(0));
}

bool Ax100Radio::SaveCanEnable()
{
    return RemoteParam::Int8(rparam0.can_en_addr, (int8_t *)&rparam0.can_en).Save(Ax100Table(0));
}

bool Ax100Radio::LoadTxInhibit()
{
    return RemoteParam::Uint32(rparam0.tx_inhibit_addr, &rparam0.tx_inhibit).Load(Ax100Table(0));
}

bool Ax100Radio::SaveTxInhibit()
{
    return RemoteParam::Uint32(rparam0.tx_inhibit_addr, &rparam0.tx_inhibit).Save(Ax100Table(0));
}

bool Ax100Radio::LoadTxPower()
{
    return RemoteParam::Uint8(rparam0.tx_pwr_addr, &rparam0.tx_pwr).Load(Ax100Table(0));
}

bool Ax100Radio::SaveTxPower()
{
    return RemoteParam::Uint8(rparam0.tx_pwr_addr, &rparam0.tx_pwr).Save(Ax100Table(0));
}

bool Ax100Radio::LoadCspRtable()
{
    return RemoteParam::String(rparam0.csp_rtable_addr, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)).Load(Ax100Table(0));
}

bool Ax100Radio::SaveCspRtable()
{
    return RemoteParam::String(rparam0.csp_rtable_addr, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)).Save(Ax100Table(0));
}

bool Ax100Radio::LoadActiveConf()
{
    return RemoteParam::Uint8(0x0018, &remote_activeconf).Load(Ax100Table(4));
}

bool Ax100Radio::LoadLastRssi()
{
    return RemoteParam::Int16(0x0004, &remote_lastrssi).Load(Ax100Table(4));
}

bool Ax100Radio::LoadRxGuard()
{
    return RemoteParam::Uint16(rparam1.guard_addr, &rparam1.guard).Load(Ax100Table(1));
}

bool Ax100Radio::SaveRxGuard()
{
    return RemoteParam::Uint16(rparam1.guard_addr, &rparam1.guard).Save(Ax100Table(1));
}

bool Ax100Radio::LoadTxGuard()
{
    return RemoteParam::Uint16(rparam5.guard_addr, &rparam5.guard).Load(Ax100Table(5));
}

bool Ax100Radio::SaveTxGuard()
{
    return RemoteParam::Uint16(rparam5.guard_addr, &rparam5.guard).Save(Ax100Table(5));
}

bool Ax100Radio::LoadRxBaud()
{
    return RemoteParam::Uint32(rparam1.baud_addr, &rparam1.baud).Load(Ax100Table(1));
}

bool Ax100Radio::SaveRxBaud()
{
    return RemoteParam::Uint32(rparam1.baud_addr, &rparam1.baud).Save(Ax100Table(1));
}

bool Ax100Radio::LoadTxBaud()
{
    return RemoteParam::Uint32(rparam5.baud_addr, &rparam5.baud).Load(Ax100Table(5));
}

bool Ax100Radio::SaveTxBaud()
{
    return RemoteParam::Uint32(rparam5.baud_addr, &rparam5.baud).Save(Ax100Table(5));
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
