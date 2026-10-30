// ViewModel - AX100 원격 파라미터 (Beacon 창 "AX100 RPARAM" 탭 위쪽)
//
// 지상국에서 CSP 로 위성 AX100(UHF 송수신기)의 원격 파라미터(rparam)를 읽고 쓴다.
// 버튼 하나 = 함수 하나. 원격 호출은 model/device/ax100_radio (Ax100Radio), 로그는 여기서 남긴다.
// (UI 스레드에서 CSP 응답을 기다린다. 원래 동작 그대로)
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace mim {
namespace vm {

class Ax100VM
{
public:
    // ---- 입력 (원격 파라미터 값: Load 로 채워지고 Save 로 보낸다) ----
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

    // ---- 표시 (읽기 전용 원격 값) ----
    uint16_t BootCount() const;
    uint32_t TotalRxBytes() const;
    uint8_t  ActiveConf() const;
    int16_t  LastRssi() const;

    // ---- 동작 ----
    void LoadBootCount();
    void LoadRssiBusy();
    void SaveRssiBusy();
    void LoadTotalRxBytes();
    void LoadCanEnable();
    void SaveCanEnable();
    void LoadTxInhibit();
    void SaveTxInhibit();
    void LoadTxPower();
    void SaveTxPower();
    void LoadCspRtable();
    void SaveCspRtable();
    void LoadActiveConf();
    void LoadLastRssi();
    void LoadRxGuard();
    void SaveRxGuard();
    void LoadTxGuard();
    void SaveTxGuard();
    void LoadRxBaud();
    void SaveRxBaud();
    void LoadTxBaud();
    void SaveTxBaud();
    void SaveToFram();
    void ResetGroundWatchdog();
    void Reboot();
};

Ax100VM & Ax100ViewModel();

} // namespace vm
} // namespace mim
