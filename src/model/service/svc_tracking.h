// Service 계층 - 위성 추적 / 로테이터 / 도플러 / 오토파일럿 / TLE 로딩
//
// 각 함수는 UI 버튼이 하던 "상태 플래그 변경 + 스레드 시작/대기 + 로그" 시퀀스를
// 원래 순서 그대로 옮긴 것이다.
#pragma once

namespace mim {
namespace svc {

// Rotator Engage (Sattracker, SLOT_ROTATOR_CONTROL)
void StartEngage();
void StopEngage();      // State.Doppler 도 끈다 (기존 동작)

// Rotator 읽기 (RotatorReadInfo, SLOT_ROTATOR_READ)
void RotatorOn();       // init_rotator() 후 읽기 스레드 시작
void RotatorOff();

// Doppler 보정 (Doppler, SLOT_DOPPLER)
void StartDoppler();    // GS100 연결이 없으면 에러 로그만 남김
void StopDoppler();

// AutoPilot (AutoPilot, SLOT_AUTOPILOT)
void StartAutoPilot();
void StopAutoPilot();

// TLE 로드 후 위성 객체 초기화 (SatelliteInitialize, SLOT_TLE_LOAD)
int StartSatelliteInitialize(void * sgp4check);

} // namespace svc
} // namespace mim
