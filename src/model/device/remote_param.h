// Model - GomSpace 원격 파라미터(rparam) 한 개와 그 묶음
//
// GS100(지상국 송수신기)과 위성 AX100 은 설정값을 "표 번호 + 주소 + 타입" 으로 읽고 쓴다.
// 예전에는 파라미터마다 gs_rparam_get_xxx / gs_rparam_set_xxx 호출과 실패 처리를 따로 적었다
// (GS100 표 0/1/5 만 104번). 여기서는 파라미터를 객체 하나로 표현한다:
//
//   RemoteParam       : 주소, 타입, 지상국 쪽 값 칸.  Load(어디) / Save(어디)
//   RemoteParamTable  : 한 표에 속한 파라미터 묶음.  LoadAll / SaveAll (실패한 개수를 돌려줌)
//   RemoteTable       : 어디 = 노드, 표 번호, 요청 대기 시간
//
// 요청은 호출한 스레드에서 응답을 기다린다. 체크섬은 항상 0xB00B (GomSpace 기본 magic).
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <initializer_list>
#include <vector>

namespace mim {
namespace model {

// 파라미터가 있는 곳
struct RemoteTable
{
    uint8_t  node;          // CSP 노드
    uint8_t  table;         // 파라미터 표 번호
    uint32_t timeout_ms;    // 요청 하나의 대기 시간
};

class RemoteParam
{
public:
    // 타입별 생성 (값 칸의 타입과 장비 쪽 타입이 다른 경우가 있어 이름으로 고른다)
    static RemoteParam Int8  (uint16_t addr, int8_t * value);
    static RemoteParam Int8  (uint16_t addr, uint8_t * value);   // 장비는 int8, 지상국은 부호 없는 칸에 보관
    static RemoteParam Uint8 (uint16_t addr, uint8_t * value);
    static RemoteParam Int16 (uint16_t addr, int16_t * value);
    static RemoteParam Uint16(uint16_t addr, uint16_t * value);
    static RemoteParam Int32 (uint16_t addr, int32_t * value);
    static RemoteParam Uint32(uint16_t addr, uint32_t * value);
    static RemoteParam Float (uint16_t addr, float * value);
    static RemoteParam String(uint16_t addr, char * text, size_t size);
    // 장비에는 int8 로 있고 지상국은 bool 로 갖는 값. 읽을 때 0 / 1 로 맞춘다
    static RemoteParam Flag  (uint16_t addr, bool * value);
    // 주소를 따로 두지 않고 값 칸의 현재 값을 주소로 쓴다.
    // (GS100 표 0 의 gosh_usart: 원래 코드가 gosh_usart_addr 대신 gosh_usart 를 넘겼다. 그 동작을 그대로 둔다)
    static RemoteParam Int8AtOwnValue(int8_t * value);

    // 장비에서 읽어 값 칸에 넣는다. 실패하면 값 칸은 그대로다
    bool Load(const RemoteTable & where) const;
    // 값 칸의 값을 장비에 쓴다
    bool Save(const RemoteTable & where) const;

private:
    enum Type { kInt8, kUint8, kInt16, kUint16, kInt32, kUint32, kFloat, kString, kFlag };

    RemoteParam(Type type, uint16_t addr, void * value, size_t size)
        : type_(type), addr_(addr), value_(value), size_(size), addr_from_value_(false) {}

    uint16_t Address() const;
    int      WireType() const;      // 라이브러리의 타입 번호 (gs_param_type_t)

    Type     type_;
    uint16_t addr_;
    void *   value_;
    size_t   size_;
    bool     addr_from_value_;
};

class RemoteParamTable
{
public:
    RemoteParamTable(std::initializer_list<RemoteParam> params) : params_(params) {}

    // 적힌 순서대로 모두 읽는다 / 쓴다. 실패한 개수를 돌려준다 (하나가 실패해도 나머지를 계속한다).
    // ok 를 주면 항목별 성공 여부를 채운다
    int LoadAll(const RemoteTable & where, std::vector<bool> * ok = nullptr) const;
    int SaveAll(const RemoteTable & where) const;

private:
    std::vector<RemoteParam> params_;
};

} // namespace model
} // namespace mim
