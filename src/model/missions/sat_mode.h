// Model - 운용 대상 위성 (위성 모드)
//
// 지상국이 지금 어느 위성과 교신하는지. "Switch Satellite" 팝업에서 바꾸고,
// 명령 목록 / 비콘 창 / TC 로그 폴더 / GS100 HMAC 키가 이 값을 따른다.
// 누리호 5차 GS(GS_nuri5) 의 SatelliteGuiProfile 을 옮긴 것.
#pragma once

namespace mim {
namespace sat {

enum SatMode {
    SAT_BEE1000 = 0,
    SAT_BEE1012 = 1,
    SAT_UELYSYS = 2,
    SAT_SD1SAT  = 3,
    SAT_MODE_COUNT
};

struct SatProfile {
    const char * name;          // 화면 표시 이름
    const char * key;           // GS100 HMAC 키, 데이터 폴더 이름 (../data/<key>/...)
    bool         hmac;          // TX HMAC 사용 (BEE1000 은 끔)
};

const SatProfile & Profile(int mode);   // 범위 밖이면 BEE1000
int  Current();                          // 지금 위성 모드
void SetCurrent(int mode);               // 범위 밖이면 무시
const char * CurrentKey();               // Profile(Current()).key

} // namespace sat
} // namespace mim
