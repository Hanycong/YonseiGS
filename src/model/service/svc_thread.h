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
    SLOT_FTP             = 8,   // FTP downlink/uplink (ftp_start_worker 가 만들고 join)
    SLOT_FTP_STOP        = 9,   // FTP Stop 뒤 워커가 끝날 때까지 ftp_abort() 를 다시 세우는 스레드
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

// pthread_join(p_thread[slot], NULL).
// StartThread 로 만든 스레드는 한 번만 join 한다: 이미 join 했거나 만든 적이 없으면 바로 0 을 돌려준다
// (같은 스레드를 두 번 join 하면 정의되지 않은 동작 - Stop 후 종료 시 크래시가 났다).
int JoinThread(Slot slot);

// 슬롯의 pthread_t 주소 (pthread_t* 를 받는 기존 API 용. 지금은 쓰는 곳이 없다 - FTP 도 StartThread/JoinThread)
// 이 슬롯은 밖에서 만들고 join 하므로, JoinThread 는 예전처럼 항상 pthread_join 한다.
pthread_t * ThreadHandle(Slot slot);

} // namespace svc
} // namespace mim
