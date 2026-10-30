// Model - RF 스위치 박스 (KTA-223)
//
// AX100 쪽 RF 경로를 수신/송신으로 전환한다. libcsp switch API 를 감싼다.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {

class SwitchBox
{
public:
    int SwitchToRx(uint8_t node);
    int SwitchToTx(uint8_t node);
};

SwitchBox & RfSwitch();

} // namespace model
} // namespace mim
