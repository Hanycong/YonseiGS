// ViewModel - AutoPilot 창 (자동 운용 설정, 시작/정지, 카운터, 간이 명령 생성기)
//
// 체크박스·입력 칸이 바로 편집하는 값은 포인터로 내준다 (State 의 운용 플래그, 생성기 입력 버퍼).
#pragma once

#include <stdint.h>
#include <stddef.h>

namespace mim {
namespace vm {

class AutoPilotVM
{
public:
    // ---- 자동 운용 설정 ----
    bool * DoBeaconCheck();
    bool * DoPing();
    bool * DoCmd();
    bool * AmpMode();
    int *  Iteration();
    int *  SpanTime();

    bool Running() const;               // State.Autopilot
    void Start();
    void Stop();

    // ---- 카운터 ----
    int BeaconCount() const;
    int PingCount() const;
    int CmdCount() const;
    bool QueuedCommandReady(int i) const;   // SatCMD[i] 가 있으면

    // ---- 간이 명령 생성기 (옛 CMDDataGenerator) ----
    uint32_t * TestMsgId();
    uint16_t * TestFncCode();
    void * TestHeader();                // 명령 헤더 버퍼 (CFE_MSG_CommandHeader)
    size_t TestHeaderSize() const;

    bool * ChecksumOn();
    bool * SchedulerOn();
    uint32_t * ExecutionTime();
    uint32_t * ExecutionWindow();
    uint16_t * EntryId();
    uint16_t * GroupId();
    void Generate(uint32_t msgid, uint16_t fnccode, void * Requested, size_t RequestedSize);   // "Generate"
    void ClearRequest(void * Requested, size_t RequestedSize);                                  // "Close"
};

AutoPilotVM & AutoPilotViewModel();

} // namespace vm
} // namespace mim
