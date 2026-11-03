// Model - 자동 운용(AutoPilot) 설정과 진행 상태
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 AutoPilotCfg (autopilot_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include <atomic>
#include "YonseiGS_config.h"

// AutoPilot 이 지금 하고 있는 일 (3. Autopilot 탭의 진행 표시)
enum AutoPilotStage {
    AP_IDLE = 0,
    AP_WAIT_PASS,       // 다음 패스 AOS 5분 전까지 대기
    AP_ENGAGE,          // 로테이터 추적·도플러 시작
    AP_BEACON,          // 비콘 수신 확인
    AP_PING,            // 핑으로 링크 확인
    AP_WAIT_GATE,       // 송신 조건(앙각·안테나·링크) 대기
    AP_TC_SCRIPT,       // TC 스크립트 송신 중
    AP_FTP,             // UL 신호 / FTP
    AP_WAIT_LOS,        // LOS 까지 대기
    AP_POST_PASS,       // 패스 종료 처리 (추적 해제, TLE 갱신)
    AP_STOPPING,        // (화면 표시용) Stop 후 스레드가 정리 중
};

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

    // TC 스크립트 송신 조건
    float MinElevation = 10.0f;         // [deg] 이 앙각 이상일 때만 보낸다 (AutoPilot 은 이보다 높이 오는 패스를 고른다)
    bool RequireLink = true;            // 이번 패스에 핑/비콘 응답이 있어야 보낸다

    // 진행 상태 (AutoPilot 스레드가 쓰고 화면이 읽는다)
    std::atomic<int> Stage{AP_IDLE};
    std::atomic<int> PassSat{-1};       // 이번 패스 위성 (OrbitData.Satellites 인덱스)
    std::atomic<int> PassIndex{-1};     // 그 위성의 _nextaos / _nextlos 인덱스
    std::atomic<int> ScriptStep{-1};    // 보내고 있는 단계 (0 부터), 없으면 -1
    std::atomic<int> ScriptSteps{0};    // 이번 패스 스크립트 단계 수
    std::atomic<int> ScriptSent{0};     // 이번 패스에 보낸 TC 수
};

extern AutoPilotState AutoPilotCfg;
