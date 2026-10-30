// Service 계층 - 스레드 슬롯 관리
//
// p_thread[16] 의 인덱스에 이름을 붙이고, 스레드 기동/대기를 한 곳으로 모은다.
// 인덱스 값은 main.cpp 의 기존 주석(pthread index)과 완전히 동일해야 한다.
// UI 코드는 이 헤더를 직접 쓰지 않고, svc_tx / svc_tracking / svc_radio 의 함수를 쓴다.
#pragma once

#include <pthread.h>

namespace mim {
namespace svc {

enum Slot
{
    SLOT_RADIO_CONTROL   = 0,   // GS100 파라미터 테이블 load/save
    SLOT_ROTATOR_CONTROL = 1,   // Sattracker (Engage)
    SLOT_ROTATOR_READ    = 2,   // RotatorReadInfo
    SLOT_TLE_TRACKING    = 3,   // TrackingSatellites
    SLOT_TX              = 4,   // TX scheduler (uplink 패킷, ping, shell)
    SLOT_CALIBRATION     = 5,   // baud/freq calibration
    SLOT_AMP_TIMER       = 6,   // AmpTimer
    SLOT_DOPPLER         = 7,   // Doppler shift correction
    SLOT_FTP             = 8,   // FTP downlink/uplink
    SLOT_SIGNALTEST      = 10,  // SignalTest
    SLOT_TRX_CONTROLLER  = 11,  // TRxController
    SLOT_RADIO_INIT      = 12,  // RadInitialize
    SLOT_AUTOPILOT       = 13,  // AutoPilot
    SLOT_SWITCH_SAT      = 14,  // SwitchSatelliteTask
    SLOT_TLE_LOAD        = 15,  // SatelliteInitialize
    SLOT_COUNT           = 16
};

typedef void * (*ThreadFn)(void *);

// pthread_create(&p_thread[slot], NULL, fn, arg) 와 동일
int StartThread(Slot slot, ThreadFn fn, void * arg);

// pthread_join(p_thread[slot], NULL) 와 동일
int JoinThread(Slot slot);

// 슬롯의 pthread_t 주소 (ftp_start_worker 처럼 pthread_t* 를 받는 기존 API 용)
pthread_t * ThreadHandle(Slot slot);

} // namespace svc
} // namespace mim
