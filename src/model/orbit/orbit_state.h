// Model - 궤도 데이터 (위성 객체 목록, 기본 위성(FDS), TLE 목록, 추적·로딩 상태)
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 OrbitData (orbit_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include "YonseiGS_config.h"

struct OrbitState {
    bool NowTracking = true;
    bool TrackThread = true;
    bool ModelRefreshRequired = false;
    int loadindex;
    int tleallindex;
    SatelliteObject * Satellites[32768];
    SatelliteObject * Fatellites;
    TLEinfo * tleinfolistup[64];
};

extern OrbitState OrbitData;
