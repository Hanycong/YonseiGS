// Model - 텔레커맨드 송신
//
// 인코딩이 끝난 명령 바이트를 지상국 내부 엔벨로프(packetsign: HVD_TEST / MIM_PT_TMTC_TEST)에 싣고
// TX 대기(svc_tx WaitTx) 후 업링크 큐(svc_tx SendUplink)에 넣는다.
// packetsign 은 malloc 하고 해제하지 않는다 (원래 동작 그대로, Known defects D11).
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace mim {
namespace tc {

void SendTelecommand(const void * cmd, uint16_t size);

// 엔벨로프 종류(packetsign PacketType)를 지정한다. 업링크 스레드가 이 값으로 목적지 노드/포트를 고른다
// (MIM_PT_TMTC_TEST: OBC, MIM_PT_NODE_TC: 명령 앞 2바이트의 노드, B12_UL_AIOBC ... 누리호 5차 위성 명령).
void SendTelecommand(const void * cmd, size_t size, uint16_t packet_type);

// TX 대기 없이 업링크 큐에만 넣는다. 여러 패킷을 이어 보내는 절차가 대기 시점을 직접 정할 때 쓴다.
void QueueTelecommand(const void * cmd, size_t size, uint16_t packet_type);

} // namespace tc
} // namespace mim
