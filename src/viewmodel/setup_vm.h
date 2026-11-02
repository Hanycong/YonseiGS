// ViewModel - Setup 팝업 (시작 시 장비/지상국 설정, Start / Reset Watchdog / Quit)
//
// 입력 칸이 바로 편집하는 설정값(Setup)은 Config() 로 그대로 내준다 (ImGui 즉시 모드라 바인딩 대신 참조).
#pragma once

#include "YonseiGS_config.h"   // Setup

namespace mim {
namespace vm {

class SetupVM
{
public:
    Setup & Config();                   // main.cpp 의 setup
    bool * UseSband();                  // State.SbandUse
    void Start();                       // GUI 시작 + 초기화 요청 + 시리얼/CSP 타이밍 설정
    void ResetWatchdog();
    void Quit();
};

SetupVM & SetupViewModel();

} // namespace vm
} // namespace mim
