// Model - OBC 핑 패킷
//
// 지상국이 가진 TLE 를 FSW 형식(FSWTle)으로 바꿔 실은 핑 패킷(packetsign)을 만든다.
// 전송은 호출하는 쪽이 TX Service(svc_tx SendUplink)로 한다.
#pragma once

#include "miman_coms.h"     // FSWTle, packetsign

namespace mim {
namespace model {

// TLE 변환 버퍼 할당 (sizeof(FSWTle) + MIM_HAND_DATAFDSTART). 해제하지 않는다 (원래 동작 그대로)
FSWTle * NewFswTleBuffer();

// tracking_index: 추적 대상 위성 번호, -1 이면 기본 위성(State.Fatellites)의 TLE
packetsign * BuildObcPing(FSWTle * buffer, int tracking_index);

} // namespace model
} // namespace mim
