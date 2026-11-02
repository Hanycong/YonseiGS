// Model - 로테이터 연결·읽기·추적(Engage) 상태
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 RotatorFlags (rotator_state.cpp 에 정의).
// miman_config.h 끝에서 include 하므로 기존처럼 miman_config.h 만 include 해도 보인다.
#pragma once

#include "miman_config.h"

struct RotatorState {
    bool RotatorConnection = false;
    bool RotatorInitialize = false;
    bool RotatorReading = false;
    bool RotatorReadReady = false;
    bool Engage = false;
};

extern RotatorState RotatorFlags;
