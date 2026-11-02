// Model - 텔레커맨드 송신
//
// 인코딩이 끝난 명령 바이트를 지상국 내부 엔벨로프(packetsign: HVD_TEST / MIM_PT_TMTC_TEST)에 싣고
// TX 대기(svc_tx WaitTx) 후 업링크 큐(svc_tx SendUplink)에 넣는다.
// packetsign 은 malloc 하고 해제하지 않는다 (원래 동작 그대로, Known defects D11).
#pragma once

#include <stdint.h>

namespace mim {
namespace tc {

void SendTelecommand(const void * cmd, uint16_t size);

} // namespace tc
} // namespace mim
