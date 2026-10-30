// ViewModel - Frequency 창
//
// 화면(view/frequency_view.cpp)이 매 프레임 읽어 가는 상태와, 버튼이 부르는 동작을 가진다.
// View 는 이 클래스만 알고 setup / State / 장비 함수는 모른다.
//  - 입력 값: ImGui 위젯이 직접 편집할 수 있도록 값의 주소를 돌려준다 (즉시 모드 GUI 방식)
//  - 표시 값: 매 프레임 호출되는 getter
//  - 동작:   버튼 하나 = 함수 하나. 순서/조건/로그는 여기서 정한다
#pragma once

#include <stdint.h>

namespace mim {
namespace vm {

class FrequencyVM
{
public:
    // ---- 입력 (지상국 설정값) ----
    double *   CenterFrequencyHz();     // 중심 주파수 [Hz]
    uint16_t * PingSize();
    uint32_t * DefaultDelay();
    uint32_t * GuardDelay();
    uint32_t * QueueDelay();

    // ---- 표시 (GS100 송수신기 상태) ----
    float    RxFrequencyMHz() const;
    float    TxFrequencyMHz() const;
    uint32_t RxBaud() const;            // [bps]
    uint32_t TxBaud() const;            // [bps]
    int16_t  LastRssi() const;          // [dBm]
    bool     DopplerRunning() const;

    // ---- 동작 ----
    void ApplyCenterFrequency();        // RX/TX 주파수와 보드레이트(4800) 설정 후 다시 읽기
    void SwitchToRx();
    void SwitchToTx();
    void StartDoppler();
    void StopDoppler();
    void ApplyDelaySpec();              // KISS 링크 지연(Default/Guard/Queue) 적용 + 로그
};

// Frequency 창의 ViewModel (창 하나에 하나)
FrequencyVM & FrequencyViewModel();

} // namespace vm
} // namespace mim
