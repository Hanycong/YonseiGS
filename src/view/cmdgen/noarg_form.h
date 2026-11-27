// View - 명령 생성기: 인자 없는 명령의 공용 입력 폼
//
// payload 없이 머리 8바이트만 보내는 명령(NOOP, RESET_COUNTERS, 각종 요청/토글)은 폼이 모두 같다:
// MID 입력, CC 입력, "Generate CMD" 버튼. 명령마다 case 를 두는 대신 표 한 줄
// { 명령 번호, 기본 MID, 기본 CC, 위젯 ID 꼬리표 } 로 적고 이 클래스가 그린다.
//
// 입력값은 명령마다 따로 유지된다 (예전에는 case 안의 static 변수였다).
// 패킷 조립과 송신은 ViewModel(CmdGeneratorVM::SendNoArgCommand)이 한다.
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace mim {
namespace ui {

class NoArgCommandForm
{
public:
    // 입력 칸 이름 (GS_nuri5 화면에서 앱마다 달랐던 두 가지를 그대로 둔다)
    enum Labels {
        kShort,     // "msgid" / "fnccode"
        kTitled,    // "Msg ID" (0x%04X 로 표시) / "Command Code(CC)"
    };

    // id      : 명령 번호 (명령 카탈로그의 id)
    // msgid   : 기본 Message ID,  fnccode : 기본 Function Code
    // tag     : 위젯 ID 꼬리표 ("##direct_1801" 등, 화면에 보이지 않음). 없으면 ""
    constexpr NoArgCommandForm(int id, uint16_t msgid, uint8_t fnccode, const char * tag, Labels labels = kShort)
        : id_(id), msgid_(msgid), fnccode_(fnccode), tag_(tag), labels_(labels) {}

    int  Id() const { return id_; }
    void Draw();

private:
    int          id_;
    uint16_t     msgid_;
    uint8_t      fnccode_;
    const char * tag_;
    Labels       labels_;
};

// forms[0..count) 에서 번호가 id 인 폼을 찾아 그린다. 있으면 true
bool DrawNoArgForm(NoArgCommandForm * forms, size_t count, int id);

template <size_t N>
bool DrawNoArgForm(NoArgCommandForm (&forms)[N], int id) { return DrawNoArgForm(forms, N, id); }

} // namespace ui
} // namespace mim
