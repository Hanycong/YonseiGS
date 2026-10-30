// ViewModel - OBC 직접 쉘 명령 (Beacon 창 "AX100 RPARAM" 탭 아래쪽)
//
// 쉘 명령 종류(State.Shelltype, 화면 상태)를 가지고, 명령 버퍼와 전송은 model/telecommand/direct_shell.
#pragma once

#include "miman_config.h"     // cmd_packet_t

namespace mim {
namespace vm {

class ShellVM
{
public:
    int * Type();                       // 쉘 명령 종류 (TYPE_SYSCMD, ...). RadioButton 이 직접 편집
    cmd_packet_t * Command();           // 종류별 입력 칸이 직접 편집하는 명령 버퍼

    // 종류가 바뀌었으면 명령 버퍼의 종류를 맞추고 입력 내용을 지운다 (매 프레임, RadioButton 다음)
    void SyncTypeChange();
    void Send();
};

ShellVM & ShellViewModel();

} // namespace vm
} // namespace mim
