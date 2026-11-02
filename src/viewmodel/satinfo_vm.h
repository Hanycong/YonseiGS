// ViewModel - 위성 상세(Details) 팝업: 앞으로의 AOS / LOS / 최대 고도 표
//
// index -1 은 기본 위성(State.Fatellites, FDS), 0 이상은 State.Satellites[index].
#pragma once

#include "tle_vm.h"     // LocalTime

namespace mim {
namespace vm {

struct PassRow {
    LocalTime aos;
    LocalTime los;
    double max_elevation_deg;
};

class SatInfoVM
{
public:
    const char * Title(int index);      // "Details : <이름>" (팝업 ID 로도 쓴다)
    // i 번째 패스가 있으면 row 를 채우고 true. 끝 조건은 원래 코드 그대로
    // (index == -1 일 때 상한 검사를 State.Satellites[i] 로 하는 것까지)
    bool Pass(int index, int i, PassRow * row) const;

private:
    char title_[64];
};

SatInfoVM & SatInfoViewModel();

} // namespace vm
} // namespace mim
