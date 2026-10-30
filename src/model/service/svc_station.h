// Service 계층 - 지상국 하드웨어 초기화 / 종료 시퀀스 (main.cpp 에서 이동)
#pragma once

namespace mim {
namespace svc {

// Setup 팝업에서 Start 를 누른 뒤(State.InitializeRequired) 한 번 실행되는 초기화.
// Switch Box -> Rotator -> GS100 (+ 라디오 초기화, TRx 컨트롤러) -> S-band -> 추적/로테이터 읽기 스레드.
// sigint_handler: GS100 연결 성공 시 SIGINT 에 등록할 핸들러 (기존 main.cpp 동작)
void InitializeHardware(void (*sigint_handler)(int));

// GUI 루프를 빠져나온 뒤 장치 연결 해제 (fin_rotator, fin_transceiver)
void FinishHardware();

// 모든 동작 플래그를 내리고 16개 스레드 슬롯을 전부 join
void StopAllThreads();

} // namespace svc
} // namespace mim
