// Model - 안테나 로테이터 (Yaesu GS-232B 컨트롤러, /dev/GS232B)
//
// 목표 각도와 현재 각도를 가지고, 시리얼로 이동 명령을 보낸다.
// 현재는 miman_orbital.cpp 의 함수(RotatorSetTo, RotatorEnter)와 각도 전역(Now)을 감싼다.
// - 목표 각도: 수동 제어(Rotator 창)와 자동 추적 스레드(Sattracker)가 함께 쓴다
// - 현재 각도: 로테이터 읽기 스레드(RotatorReadInfo)가 갱신한다
// miman_orbital.cpp 를 분해할 때 시리얼 입출력과 각도 상태가 이 클래스 안으로 들어온다.
#pragma once

namespace mim {
namespace model {

class Gs232bRotator
{
public:
    // 이동 명령 전송 (주의: 기존 구현이 호출한 스레드에서 1초 sleep 한다)
    void MoveTo(int azimuth_deg, int elevation_deg);

    // 시리얼 입출력 버퍼 비우기
    void FlushIo();

    // 목표 각도 [deg] (입력 위젯이 직접 편집할 수 있도록 주소를 돌려준다)
    int * TargetAzimuth();
    int * TargetElevation();

    // 현재 각도 [deg]
    int CurrentAzimuth() const;
    int CurrentElevation() const;
};

Gs232bRotator & Rotator();

} // namespace model
} // namespace mim
