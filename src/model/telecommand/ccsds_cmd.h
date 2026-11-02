// Model - cFE 명령 인코더 (플랫폼 공용)
//
// cFS 위성 명령의 앞 8바이트(CCSDS Primary 6바이트 + cFE Secondary 2바이트)를 쓰고 체크섬을 계산한다.
//   [0..1] MsgId (빅엔디안)   [2..3] 시퀀스 0xC0 0x00   [4..5] 길이 0x00, (size - 7)
//   [6] Function Code          [7] 체크섬 (0xFF 에 명령 전체 바이트를 XOR, 계산 전 0 으로 비움)
// payload 는 호출하는 쪽이 미리 채워 둔다 (리틀엔디안 그대로, 바이트 순서 변환 없음).
#pragma once

#include <stdint.h>

namespace mim {
namespace tc {

// cmd: CmdHeader[8] 로 시작하는 명령 전체 (size 바이트)
void EncodeCfeCommand(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode);

// 옛 배치: Function Code 를 [7], 체크섬을 [6] 에 쓴다 (Generic s8/s16/s32/s64/float 만, Known defects D13).
// FSW 가 어느 배치를 기대하는지 확인될 때까지 원래 바이트를 그대로 유지하려고 따로 둔다.
void EncodeCfeCommandFcAt7(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode);

// 길이 바이트를 (size - 7) 대신 주어진 값으로 쓴다. 기존 명령이 길이를 상수로 적어 둔 경우만 쓴다.
void EncodeCfeCommand(void * cmd, uint16_t size, uint16_t msgid, uint8_t fnccode, uint8_t length);

} // namespace tc
} // namespace mim
