// Service 계층 - 패스 TC 스크립트 (AutoPilot 이 한 패스에서 순서대로 보내는 명령 목록)
//
// 한 단계 = 명령 바이트(위성으로 나가는 CCSDS 패킷 그대로) + 반복 횟수 + 송신 후 대기 시간.
// 송신은 기존 수동 송신과 같은 경로(SendUplink → WaitTx, HVD_TEST / MIM_PT_TMTC_TEST)를 쓴다.
#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace mim {
namespace svc {

struct TcStep {
    std::string name;
    std::vector<uint8_t> bytes;
    int repeat = 2;                     // 같은 명령을 몇 번 보낼지
    int delay_s = 10;                   // 한 번 보낸 뒤 기다리는 시간 [s]
};

// 예전 AutoPilot_task 에 하드코딩돼 있던 명령 순서
// (EPS HK 3종 → AX100 / EPS DOCK GND WDT 리셋 → EPS HK 3종, 각 2회, 10 s 간격)
std::vector<TcStep> DefaultPassScript();

// step 을 repeat 번 보낸다. keep_going() 이 false 가 되면 그 자리에서 멈춘다.
// 보낸 횟수를 돌려준다.
int SendTcStep(const TcStep & step, bool (*keep_going)());

} // namespace svc
} // namespace mim
