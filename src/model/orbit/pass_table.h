// Model - 위성 패스 표 (다음 AOS / LOS / 최대 앙각)
//
// 주의: 아직 실제 패스 예측(SGP4)이 아니라 Track 창에 원래 들어 있던 고정 예시 데이터 20개다.
// 극좌표에 찍는 방위각/앙각도 위성 번호와 최대 앙각으로 만든 예시 값이다.
// 실제 계산(SatelliteObject::GeneratePassList 등)과 연결하는 것은 기능 변경이므로 별도로 한다.
#pragma once

namespace mim {
namespace model {

struct PassEntry {
    const char* name;
    int aos_h, aos_m, aos_s;
    int los_h, los_m, los_s;
    float max_el;                       // [deg]
};

class ExamplePassTable
{
public:
    int Count() const;
    const PassEntry & At(int index) const;

    // 예시 관측각 [deg] (index 범위 검사 없음: 원래 코드 그대로, CLAUDE.md Known defects)
    float ExampleAzimuth(int index) const;
    float ExampleElevation(int index) const;
};

ExamplePassTable & Passes();

} // namespace model
} // namespace mim
