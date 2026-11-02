// Model - 자동 운용(AutoPilot) 설정과 진행 상태
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 AutoPilotCfg (autopilot_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include "YonseiGS_config.h"

struct AutoPilotState {
    bool Autopilot = false;
    bool AMPmode = true;
    int iteration = 3;
    int SpanTime = 5;
    int CMDCount = 0;
    int Successed = 0;
    bool DoBeaconCheck = false;
    bool DoPing = true;
    bool DoCMD = true;
    bool DoFTPDL = false;
    bool DoFTPUL = false;
    bool DoBaudCalibration = false;
    bool DoFreqCalibration = false;
};

extern AutoPilotState AutoPilotCfg;
