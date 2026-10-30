// Model - 수신 텔레메트리 저장소 (마지막으로 받은 패킷)
//
// 수신 스레드(miman_coms 의 task_downlink_onorbit)가 패킷을 해석해 채우는 버퍼를 읽는다.
// 현재는 miman_coms.cpp 의 전역(beacon, missionbeacon, event, getfileinfo, g_report_view)을 감싼다.
// miman_coms 를 분해할 때 버퍼와 해석 코드(비콘/리포트 파서)가 이 디렉토리로 들어온다.
#pragma once

#include "miman_coms.h"     // 패킷 구조체 (Beacon, MissionBeacon, Event, GETFILEINFO, ReportView_t)

namespace mim {
namespace model {

class TelemetryStore
{
public:
    // 수신 스레드가 같은 버퍼를 덮어쓴다 (시작 시 한 번 할당, 받은 적 없으면 할당 직후 내용)
    Beacon *        LastBeacon() const;
    MissionBeacon * LastMissionBeacon() const;
    Event *         LastEvent() const;
    GETFILEINFO *   LastFileInfo() const;     // FM Get File Info 응답

    // 리포트 해석 결과를 잠금(g_report_view_mtx) 아래에서 복사해 온다
    ReportView_t ReportSnapshot() const;
};

TelemetryStore & Telemetry();

} // namespace model
} // namespace mim
