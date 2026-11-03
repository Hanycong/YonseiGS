// ViewModel - Track 창
//
// 화면(view/track_view.cpp)이 매 프레임 읽어 가는 상태와, 버튼이 부르는 동작을 가진다.
// View 는 이 클래스만 알고 Requested / OrbitData / 추적 대상은 모른다.
// 표에는 TLE Manager 에서 "Select" 한 위성(SatelliteObject::use)만 나온다.
// 시각·각도는 SGP4 계산 결과(_nextaos/_nextlos/_max_elevation, 추적 스레드가 갱신하는 topo)다.
#pragma once

#include <stddef.h>
#include <vector>

namespace mim {
namespace vm {

// 표 한 줄
struct TrackRow {
    int  sat;                           // OrbitData.Satellites 인덱스
    const char * name;
    bool has_pass;                      // 다음 패스가 계산돼 있는지 (없으면 시각·앙각 칸은 "-")
    int  aos_mon, aos_day, aos_h, aos_m, aos_s;   // 한국 시각 (UTC+9)
    int  los_mon, los_day, los_h, los_m, los_s;
    float max_el;                       // [deg]
};

class TrackVM
{
public:
    // 매 프레임 맨 처음: 표에 보일 위성 목록을 다시 만든다
    void BeginFrame();

    // ---- 입력 (검색어, 대소문자 구분 없음) ----
    char * SearchBuffer();
    size_t SearchBufferSize() const;

    // ---- 표 (검색으로 걸러진 뒤의 줄) ----
    bool Loading() const;               // TLE 를 다시 읽는 중이면 true (표를 비운다)
    int  RowCount() const;
    const TrackRow & Row(int r) const;
    int  SelectedCount() const;         // 검색 전, TLE Manager 에서 고른 위성 수

    bool IsSelected(int r) const;       // 표에서 고른 행 (위성 정보 팝업이 쓰는 값)
    void Select(int r);

    bool IsTracking(int r) const;       // 추적 대상 위성인지
    void Track(int r);                  // 추적 대상 지정

    void OpenTleManager();              // "TLE" 버튼

    // ---- 추적 대상 표시 (극좌표 / 수치) ----
    bool  HasTrackTarget() const;       // 추적 대상이 TLE Manager 에서 고른 위성일 때만 true
    const char* TrackedName() const;    // 대상이 없으면 nullptr
    float TrackedAzimuth() const;       // [deg] 현재 방위각 (SGP4)
    float TrackedElevation() const;     // [deg] 현재 앙각 (SGP4, 지평선 아래면 음수)

private:
    char search_[128] = "";
    int  selected_count_ = 0;
    std::vector<TrackRow> rows_;
};

// Track 창의 ViewModel (창 하나에 하나)
TrackVM & TrackViewModel();

} // namespace vm
} // namespace mim
