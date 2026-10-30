// Model - 지상국 CSP/KISS 링크 타이밍
//
// GS100 과 PC 사이 KISS 시리얼 링크의 지연값(Default/Guard/Queue)을 libcsp 에 설정한다.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {

class GsLink
{
public:
    void ApplyDelaySpec(uint32_t kiss_baud, uint32_t switch_baud, uint32_t can_baud,
                        uint32_t queue_delay, uint8_t gs100_node,
                        uint32_t default_delay, uint32_t guard_delay);
};

GsLink & Link();

} // namespace model
} // namespace mim
