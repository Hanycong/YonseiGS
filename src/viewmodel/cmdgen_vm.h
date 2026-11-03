// ViewModel - 명령 생성기 (Send Telecommand / Initial Ops 팝업)의 명령 선택 부분
//
// 분류 콤보, 명령 콤보(초기 운용 모드면 초기 운용 세트만), Write Raw Data, 선택된 명령 번호.
// 목록은 명령 카탈로그(model/telecommand/command_catalog.h, model/missions/*)에서 읽는다.
// 명령별 입력 폼과 패킷 조립은 아직 view/cmdgen/*/forms_*.cpp 에 있다 (CLAUDE.md 다음 단계).
#pragma once

#include <stdint.h>
#include "command_catalog.h"

namespace mim {
namespace vm {

class CmdGeneratorVM
{
public:
    // ---- 분류 콤보 (두 팝업이 같은 값을 쓴다: 원래 함수 static 하나). 항목은 위성 모드에 따라 바뀐다 ----
    int * SelectedCategory();
    int CategoryCount() const;
    const char * CategoryName(int i) const;

    // ---- 명령 콤보 ----
    const char * SelectedLabel() const;               // 선택된 명령 이름, 없으면 "Select CMD type"
    int PickerSize(bool initial_mode) const;          // 후보 수 (초기 운용 세트 또는 전체 카탈로그)
    const mim::tc::CommandDef * PickerItem(bool initial_mode, int k) const;  // 분류에 안 맞으면 nullptr
    bool IsSelected(int command_id) const;
    void Select(int command_id);
    uint16_t SelectedCommand() const;                 // gen_fnccode

    bool * RawData();                                 // "Write Raw Data"

private:
    void SyncSatellite();               // 위성 모드가 바뀌었으면 콤보/선택 초기화
    int selected_category_ = 0;
    int last_sat_ = -1;
};

CmdGeneratorVM & CmdGeneratorViewModel();

} // namespace vm
} // namespace mim
