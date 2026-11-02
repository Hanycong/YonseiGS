// ViewModel - GS100 파라미터 테이블 팝업 3개 (테이블 0, 1, 5)
//
// 입력 칸이 바로 편집하는 테이블 값은 참조로 내주고, 버튼(Update / Save / Save to FRAM)은 테이블 번호를 받는다.
#pragma once

#include <stdint.h>

#include "YonseiGS_config.h"
#include "YonseiGS_radial.h"   // Ptable_0, Ptable_1, Ptable_5

namespace mim {
namespace vm {

class Gs100ParamVM
{
public:
    Ptable_0 & Table0();
    Ptable_1 & Table1();
    Ptable_5 & Table5();

    void Load(int table);               // "Update": GS100 에서 읽어 온다
    void Save(int table);               // "Save"
    void SaveToFram(int8_t table);      // "Save to FRAM" (Known defects: 스레드에 지역 변수 주소를 넘기는 것까지 원래 그대로)
};

Gs100ParamVM & Gs100ParamViewModel();

} // namespace vm
} // namespace mim
