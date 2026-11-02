// Model - 예약(Schedule) Raw 명령 송신
//
// 명령 생성기 Raw "no arg" / "u64" 폼에서 Schedule 을 체크했을 때의 Generate CMD 본문을 그대로 옮긴 것.
// 엔트리 번호·실행 번호(State.entrynum / State.u32val)를 packetsign 앞에 붙인 sch_cmd_t 를 업링크 큐에 넣는다.
// Known defects D2: no arg 는 sch_cmd_t* 를, u64 는 그 안의 packetsign 을 넘기는 등 송신 태스크가 기대하는
// 형식과 맞지 않는다. FSW/송신 태스크 쪽 확인 전까지 원래 코드 그대로 둔다.
#pragma once

#include <stdint.h>

namespace mim {
namespace tc {

void SendScheduledNoArg(uint16_t msgid, uint8_t fnccode);
void SendScheduledU64(uint16_t msgid, uint8_t fnccode, uint64_t arg);

} // namespace tc
} // namespace mim
