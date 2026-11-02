// Model - 지상국 무선 링크 상태 (GS100·스위치 연결, TX/RX 모드, 도플러, AMP, 예약 명령·쉘 설정)
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 LinkFlags (link_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include "YonseiGS_config.h"

struct LinkState {
    bool RadioInitialize = false;
    bool SwitchConnection = false;
    bool GS100_Connection = false;
    bool TRx_mode = false;
    bool downlink_mode = false;
    bool uplink_mode = false;
    bool Doppler = false;
    bool Signaltest = false;
    bool AMPON = false;
    time_t AmpTime;
    struct tm * AmpTM;
    bool Scheduled = false;
    uint8_t entrynum;
    uint32_t u32val;
    int Paramtype = 0;
    int Shelltype = TYPE_SYSCMD;
};

extern LinkState LinkFlags;
