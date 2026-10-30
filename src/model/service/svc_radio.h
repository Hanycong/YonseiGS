// Service 계층 - GS100 라디오 / 앰프 / 캘리브레이션 / 위성(HMAC) 전환
//
// 각 함수는 UI 버튼이 하던 시퀀스를 원래 순서 그대로 옮긴 것이다.
#pragma once

namespace mim {
namespace svc {

// GS100 파라미터 테이블 (SLOT_RADIO_CONTROL). table: 0, 1, 5
int LoadParamTable(int table);
int SaveParamTable(int table);
// Save2FRAM(param_id). 주의: 기존 호출부는 곧 사라지는 지역변수 주소를 넘긴다 (Known defects)
int SaveParamToFRAM(void * param_id);

// CSP 캘리브레이션 (SLOT_CALIBRATION): 이전 작업 대기 후 시작
void StartBaudCalibration();
void StartFreqCalibration();

// 신호 테스트 (SLOT_SIGNALTEST)
int StartSignalTest();
void StopSignalTest();

// 앰프: ../amp/ampcontrol, ../amp/ampoff 프로세스 실행 (+ AmpTimer, SLOT_AMP_TIMER)
void AmpOn();
void AmpOff();

// 위성 전환 (GS100 HMAC 키 변경, SLOT_SWITCH_SAT)
const char * SwitchSatelliteName(int mode);
void SwitchSatellite(int * mode_arg);

} // namespace svc
} // namespace mim
