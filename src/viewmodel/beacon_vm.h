// ViewModel - Beacon 창 (수신 텔레메트리 표시 탭 5개: Beacon, Mission Beacon, Report, Event, Response)
//
// 화면(view/beacon/*_tab.cpp)이 매 프레임 읽어 가는 "마지막으로 받은" 텔레메트리를 내준다.
// View 는 이 클래스만 알고 수신 스레드가 채우는 전역(beacon, missionbeacon, ...)은 모른다.
// 값의 형식(Beacon, MissionBeacon, ...)은 YonseiGS_coms.h 의 패킷 구조체 그대로다.
// (AX100 RPARAM 탭은 Ax100VM / ShellVM 이 맡는다)
#pragma once

#include <stdint.h>
#include "YonseiGS_coms.h"

namespace mim {
namespace vm {

class BeaconVM
{
public:
    // 마지막으로 받은 패킷 (수신 스레드가 같은 버퍼를 덮어쓴다. 아직 받은 적 없으면 0 으로 채워진 버퍼)
    Beacon *        LastBeacon() const;
    MissionBeacon * LastMissionBeacon() const;
    Event *         LastEvent() const;
    GETFILEINFO *   LastFileInfo() const;     // Response 탭 (FM Get File Info 응답)

    // 누리호 5차 위성 비콘 (수신 스레드가 채우는 전역 bee1012_beacon 등. 위성 모드에 따라 Beacon 탭이 고른다)
    const BEE1012_Beacon_t * LastBee1012Beacon() const;
    const UELYSYS_Beacon_t * LastUelysysBeacon() const;
    const SD1SAT_Beacon_t *  LastSd1satBeacon() const;

    // Report 탭: 수신 스레드가 갱신하는 리포트 해석 결과를 잠금 아래에서 복사해 온다
    ReportView_t ReportSnapshot() const;

    // AX100 Total RX Bytes (AX100 RPARAM 탭의 Load 로 갱신되는 값)
    uint32_t TotalRxBytes() const;
};

// Beacon 창의 ViewModel (창 하나에 하나)
BeaconVM & BeaconViewModel();

} // namespace vm
} // namespace mim
