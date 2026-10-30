// Model - 추적 대상 위성
//
// 지금 어떤 위성을 추적하는지(State.Satellites 의 인덱스, 없으면 -1)를 가진다.
// Track 창, 자동 추적(Sattracker), AutoPilot, 도플러가 함께 쓰는 값이다.
// 현재는 miman_orbital.cpp 의 GetNowTracking / SetNowTracking(전역 NowSat)을 감싼다.
// miman_orbital.cpp 를 분해할 때 NowSat 와 범위 검사가 이 클래스 안으로 들어온다.
#pragma once

namespace mim {
namespace model {

class TrackTarget
{
public:
    int  Index() const;                 // 추적 중인 위성 인덱스, 없으면 -1
    void Set(int index);                // 범위 밖이거나 비어 있는 슬롯이면 거부 로그를 남기고 -1
};

TrackTarget & Tracking();

} // namespace model
} // namespace mim
