// Model - OBC 직접 쉘 명령 (cmd_packet_t)
//
// 위성 OBC 로 보내는 쉘 명령 버퍼 하나와 전송. 전송은 TX 슬롯 Service(svc_tx StartDirectShell)를 쓴다.
// 현재는 YonseiGS_imgui.cpp 의 전역 shellcmd 를 감싼다.
#pragma once

#include <stdint.h>
#include "YonseiGS_config.h"     // cmd_packet_t

namespace mim {
namespace model {

class DirectShell
{
public:
    cmd_packet_t * Command();           // 종류별 입력 칸이 직접 편집하는 버퍼
    uint8_t Type() const;               // 버퍼의 명령 종류 (TYPE_SYSCMD, ...)
    void ChangeType(uint8_t type);      // 종류를 바꾸고 종류별 입력 내용(required)을 0 으로 지운다
    void Send();                        // TX 슬롯에서 전송 시작
};

DirectShell & Shell();

} // namespace model
} // namespace mim
