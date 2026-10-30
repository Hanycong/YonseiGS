// Model - CSP 원격 파라미터 (GomSpace rparam) 읽기/쓰기, 임의 노드
//
// gs_rparam_get_* / gs_rparam_set_* 를 한 형식씩 감싼다. 체크섬 0xB00B, 대기 시간은 지상국 설정
// (setup->default_timeout). 성공하면 true. 호출한 스레드에서 응답을 기다린다.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {

class CspRemoteParam
{
public:
    bool GetU8 (uint8_t node, uint8_t table, uint16_t addr, uint8_t  * value);
    bool GetU16(uint8_t node, uint8_t table, uint16_t addr, uint16_t * value);
    bool GetU32(uint8_t node, uint8_t table, uint16_t addr, uint32_t * value);
    bool GetS8 (uint8_t node, uint8_t table, uint16_t addr, int8_t   * value);
    bool GetS16(uint8_t node, uint8_t table, uint16_t addr, int16_t  * value);
    bool GetS32(uint8_t node, uint8_t table, uint16_t addr, int32_t  * value);
    bool GetFloat(uint8_t node, uint8_t table, uint16_t addr, float  * value);

    bool SetU8 (uint8_t node, uint8_t table, uint16_t addr, uint8_t  value);
    bool SetU16(uint8_t node, uint8_t table, uint16_t addr, uint16_t value);
    bool SetU32(uint8_t node, uint8_t table, uint16_t addr, uint32_t value);
};

CspRemoteParam & RemoteParam();

} // namespace model
} // namespace mim
