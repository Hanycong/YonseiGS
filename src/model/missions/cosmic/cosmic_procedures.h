// Model - COSMIC 운용 절차 (여러 패킷을 정해진 순서와 간격으로 보내는 명령 207, 208)
//
// 명령 생성기 폼(view/cmdgen/cosmic/forms_to_lab.cpp)의 Generate CMD 본문을 그대로 옮긴 것.
// 바이트와 CRC 가 상수로 박혀 있고, 호출한 스레드(UI)에서 WaitTx / sleep 으로 기다린다
// (Known defects D10: sleep(0.1) 은 정수 0초). 절차를 TX 스레드로 옮기는 것은 타이밍이 바뀌는 동작 변경이라 별도.
#pragma once

#include <stdint.h>

namespace mim {
namespace model {
namespace cosmic {

// 207 "To COSMIC... From Yonsei...": TO_LAB Add Packet → 1초 뒤 HK1 Send Combined Packet
void RunAddPacketProcedure(uint16_t msgid, uint8_t fnccode);

// 208 "COSMIC S/UHF Dual Emission For DoSsam": Set Dual Emission → TO_LAB Add Packet → HK1 Send Combined Packet
void RunDualEmissionProcedure(uint16_t msgid, uint8_t fnccode);

} // namespace cosmic
} // namespace model
} // namespace mim
