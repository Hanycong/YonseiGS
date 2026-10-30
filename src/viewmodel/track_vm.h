// ViewModel - Track 창
//
// 화면(view/track_view.cpp)이 매 프레임 읽어 가는 상태와, 버튼이 부르는 동작을 가진다.
// View 는 이 클래스만 알고 Requested / Satellite_row / 추적 대상 / 패스 표는 모른다.
// 표의 위성 목록은 아직 실제 패스 계산이 아니라 고정 예시 데이터 20개다 (model/orbit/pass_table.h).
#pragma once

#include <stddef.h>
#include "pass_table.h"

namespace mim {
namespace vm {

// 표 한 줄 (위성 이름, 다음 AOS/LOS 시각, 최대 앙각)
using TrackRow = mim::model::PassEntry;

class TrackVM
{
public:
    // 매 프레임 맨 처음 (기존 동작 유지: 전역 Satellite_row 를 0 으로)
    void BeginFrame();

    // ---- 입력 (검색어) ----
    char * SearchBuffer();
    size_t SearchBufferSize() const;

    // ---- 표 ----
    int  RowCount() const;
    const TrackRow & Row(int i) const;
    bool HiddenBySearch(int i) const;   // 검색어가 있고 이름에 포함되지 않으면 true

    bool IsSelected(int i) const;       // 표에서 고른 행 (위성 정보 팝업이 쓰는 값)
    void Select(int i);

    bool IsTracking(int i) const;       // 추적 대상 위성인지
    void Track(int i);                  // 추적 대상 지정

    // ---- 추적 대상 표시 (극좌표 / 수치) ----
    bool  HasTrackTarget() const;
    const char* TrackedName() const;    // 대상이 없거나 표 밖이면 nullptr
    float TrackedAzimuth() const;       // [deg] 예시 값
    float TrackedElevation() const;     // [deg] 예시 값

private:
    char search_[128] = "";
};

// Track 창의 ViewModel (창 하나에 하나)
TrackVM & TrackViewModel();

} // namespace vm
} // namespace mim
