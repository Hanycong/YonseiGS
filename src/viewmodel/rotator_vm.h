// ViewModel - Rotator 창
//
// 화면(view/rotator_view.cpp)이 매 프레임 읽어 가는 상태와, 버튼이 부르는 동작을 가진다.
// View 는 이 클래스만 알고 State / Now / 로테이터 함수는 모른다. (Frequency 창과 같은 구조)
#pragma once

namespace mim {
namespace vm {

class RotatorVM
{
public:
    // ---- 입력 (수동 제어 목표 각도) ----
    int * TargetAzimuth();              // [deg]. 자동 추적(Engage) 스레드도 같은 값을 쓴다
    int * TargetElevation();            // [deg]

    // ---- 표시 ----
    bool Engaging() const;              // 자동 추적 중인지
    int  CurrentAzimuth() const;        // [deg] 로테이터 읽기 스레드가 갱신
    int  CurrentElevation() const;      // [deg]

    // ---- 동작 ----
    void StartEngage();
    void StopEngage();
    void MoveToTarget();                // 목표 AZ/EL 로 이동 (Set)
    void MoveToHome();                  // 0, 0 으로 이동 (Init)
    void RotatorOn();
    void RotatorOff();
    void FlushIo();                     // 로테이터 시리얼 입출력 버퍼 비우기 (IO Buffer Free)
};

// Rotator 창의 ViewModel (창 하나에 하나)
RotatorVM & RotatorViewModel();

} // namespace vm
} // namespace mim
