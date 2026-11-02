// App - 위성 탭 상태 (활성 위성, 탭에 띄울 위성 목록, 활성 기능 탭)
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 SatTabs (miman_satcontext.cpp 에 정의).
// miman_config.h 끝에서 include 하므로 기존처럼 miman_config.h 만 include 해도 보인다.
#pragma once

#include "miman_config.h"

struct SatTabState {
    int  ActiveSatIndex   = 0;          // 현재 활성 위성 탭 (Satellites[] 인덱스)
    int  VisibleSats[8]   = {0,-1,-1,-1,-1,-1,-1,-1};  // 탭으로 띄울 위성들 (-1=빈슬롯)
    int  VisibleSatsCount = 1;
    int  ActiveFuncTab    = 1;          // 0=Pre-Pass, 1=Pass Operation, 2=Post-Pass
};

extern SatTabState SatTabs;
