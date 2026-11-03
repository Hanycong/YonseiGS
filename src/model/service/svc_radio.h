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
// 앰프(Arduino, /dev/ttyACM0)와 제어 프로그램(../amp/<program>)을 쓸 수 있는지 확인.
// 못 쓰면 이유를 콘솔에 남기고 false. 장치는 열지 않는다(열면 Arduino 가 리셋될 수 있음)
bool AmpReady(const char * program);

// 위성 전환 (mim::sat 위성 모드 + GS100 HMAC 키 + TC 로그 폴더, SLOT_SWITCH_SAT)
// 위성 모드는 바로 바뀌고, GS100 설정은 스레드에서 한다. 앞 전환이 진행 중이면 false
const char * SwitchSatelliteName(int mode);
bool SwitchSatellite(int mode);

} // namespace svc
} // namespace mim
