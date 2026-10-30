// Model - GS100 지상국 송수신기 (UHF)
//
// RX(파라미터 테이블 1) / TX(테이블 5) 주파수와 보드레이트를 설정하고 읽는다.
// 현재는 miman_radial.cpp 의 함수와 테이블 사본(param1, param5)을 감싼다.
// miman_radial.cpp 를 분해할 때 테이블 사본과 rparam 호출이 이 클래스 안으로 들어온다.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {

class Gs100Radio
{
public:
    // 설정 (GS100 에 rparam 쓰기)
    void SetRxFrequency(uint32_t hz);
    void SetTxFrequency(uint32_t hz);
    void SetRxBaud(uint32_t bps);
    void SetTxBaud(uint32_t bps);

    // GS100 에서 다시 읽어 사본 갱신
    void ReadFrequencies();
    void ReadBauds();

    // 마지막으로 읽은 사본
    uint32_t RxFrequencyHz() const;
    uint32_t TxFrequencyHz() const;
    uint32_t RxBaud() const;
    uint32_t TxBaud() const;
    int16_t  LastRssi() const;          // [dBm]
};

Gs100Radio & Gs100();

} // namespace model
} // namespace mim
