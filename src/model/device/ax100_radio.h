// Model - 위성 AX100 UHF 송수신기 (CSP 원격 파라미터)
//
// 지상국에서 CSP 로 위성 AX100 의 원격 파라미터(rparam)를 읽고 쓴다. 노드 번호와 대기 시간은
// 지상국 설정(setup->ax100_node, setup->default_timeout)을 쓴다.
// 현재는 miman_imgui.cpp 의 전역(rparam0/1/5, remote_*)과 miman_coms 의 get_boot_count 등을 감싼다.
// Load* 는 원격 값을 읽어 아래 값 칸에 채우고, Save* 는 값 칸의 값을 보낸다. 성공하면 true.
// (모두 호출한 스레드에서 CSP 응답을 기다린다)
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace mim {
namespace model {

class Ax100Radio
{
public:
    // ---- 원격 파라미터 값 칸 ----
    int16_t *  RssiBusy();              // table 5, 0x0048
    bool *     CanEnable();             // table 0
    uint32_t * TxInhibit();             // table 0
    uint8_t *  TxPower();               // table 0
    char *     CspRtable();             // table 0
    size_t     CspRtableSize() const;
    uint16_t * RxGuard();               // table 1
    uint16_t * TxGuard();               // table 5
    uint32_t * RxBaud();                // table 1
    uint32_t * TxBaud();                // table 5
    uint8_t    CspNode() const;         // table 0 csp_node (읽기만)

    // ---- 읽기 전용 원격 값 (table 4) ----
    uint16_t BootCount() const;
    uint32_t TotalRxBytes() const;
    uint8_t  ActiveConf() const;
    int16_t  LastRssi() const;

    // ---- 원격 동작 ----
    bool LoadBootCount();
    bool LoadRssiBusy();
    bool SaveRssiBusy();
    bool LoadTotalRxBytes();
    bool LoadCanEnable();
    bool SaveCanEnable();
    bool LoadTxInhibit();
    bool SaveTxInhibit();
    bool LoadTxPower();
    bool SaveTxPower();
    bool LoadCspRtable();
    bool SaveCspRtable();
    bool LoadActiveConf();
    bool LoadLastRssi();
    bool LoadRxGuard();
    bool SaveRxGuard();
    bool LoadTxGuard();
    bool SaveTxGuard();
    bool LoadRxBaud();
    bool SaveRxBaud();
    bool LoadTxBaud();
    bool SaveTxBaud();
    bool SaveToFram();
    bool ResetGroundWatchdog();     // 응답 없는 CSP 트랜잭션 (최대 1초 대기)
    void Reboot();
};

Ax100Radio & Ax100();

} // namespace model
} // namespace mim
