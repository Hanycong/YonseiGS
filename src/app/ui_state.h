// App - 화면 표시 상태 (어떤 팝업/창을 띄울지)
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 AppUi (ui_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include "YonseiGS_config.h"

struct UiState {
    bool ImGuiLoadState = true;
    bool Display_Setup = true;
    bool Display_paramt0 = false;
    bool Display_paramt1 = false;
    bool Display_paramt5 = false;
    bool Display_CMD = false;
    bool Display_TLE = false;
    bool Display_FDS = false;
    bool Display_Satinfo = false;
    bool Display_load = false;
    bool Display_Sband = false;
    bool Display_Autopilot = true;
};

extern UiState AppUi;
