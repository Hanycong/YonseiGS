// ViewModel - Control 창 (Pass Operation 탭 왼쪽 열)과 그 팝업 3개 (노드 핑, 위성 전환, 임의 RParam)
//
// 버튼 하나 = 함수 하나. 순서/조건/로그는 원래 버튼 블록 그대로 여기서 정한다.
// 팝업 창의 열림 여부는 화면 상태라 View(control_view.cpp)가 가진다.
#pragma once

#include <stdint.h>

namespace mim {
namespace vm {

// AMP 버튼 한 프레임 분 상태 (AMP 타이머 스레드가 바꾸는 값을 한 번만 읽는다)
struct AmpButton {
    bool on;                            // State.AMPON
    bool timer;                         // State.AmpTime > 0
    const char * label;                 // "AMP ON" / "Disabled  MM:SS" / "AMP OFF" / "AMP OFF MM:SS"
                                        // (+ "###AmpButton": 남은 시간이 바뀌어도 ImGui ID 가 같아 클릭이 빠지지 않게)
};

class ControlVM
{
public:
    // ---- 핑 / 진단 ----
    void PingAx100();
    void PingObc();                     // 추적 대상 위성의 TLE 를 실은 핑 패킷 업링크
    void PrintRoutingTable();           // "Send Telecommand" 를 열 때 CSP 라우팅 표를 표준출력에
    void StartBaudCalibration();
    void StartFreqCalibration();

    // ---- AMP ----
    AmpButton Amp();                    // 라벨 문자열을 만들고(공유 버퍼 AmpTimeBuf) 상태를 돌려준다
    void TurnAmpOn();
    void TurnAmpOff();
    void PressAmp(const AmpButton & amp);   // AMP 버튼 클릭: on 이면 Off, 아니면 On ("Disabled" 포함)

    // ---- 명령 큐 ----
    void OpenCommandManager();          // CMD Manager 팝업
    void ClearCommandQueue();           // 쌓인 명령(SatCMD) 모두 삭제

    // ---- "Ping each node" 팝업 ----
    uint8_t * PingTargetNode();
    void PingNode();

    // ---- "Switch Satellite" 팝업 ----
    int  SatelliteCount() const;
    const char * SatelliteName(int i) const;
    int * SelectedSatellite();          // RadioButton 이 직접 편집
    void SwitchToSelectedSatellite();   // 선택이 바뀐 프레임에 호출

private:
    uint8_t ping_node_ = 0;
    int selected_sat_ = 0;              // mim::sat::SatMode (라디오 버튼 편집용 사본)
};

// "Custom RParam Get" 팝업: 임의 노드/테이블/주소의 원격 파라미터 읽기/쓰기
class CustomRparamVM
{
public:
    uint8_t *  Node();
    uint8_t *  TableId();
    uint16_t * Address();               // 10진수 입력
    int *      TypeIndex();             // 0 uint8, 1 uint16, 2 uint32, 3 int8, 4 int16, 5 int32, 6 float

    uint8_t *  ValueU8();
    uint16_t * ValueU16();
    uint32_t * ValueU32();
    int8_t *   ValueS8();
    int16_t *  ValueS16();
    int32_t *  ValueS32();
    float *    ValueFloat();

    void Load();                        // 선택한 형식으로 읽어 해당 값 칸에 채운다
    void Save();                        // uint8/16/32 만 (원래 동작 그대로)

private:
    uint8_t node = 0;
    uint8_t table_id = 0;
    uint16_t addr = 0;
    int type_index = 0;
    uint8_t  value_u8 = 0;
    uint16_t value_u16 = 0;
    uint32_t value_u32 = 0;
    int8_t   value_s8 = 0;
    int16_t  value_s16 = 0;
    int32_t  value_s32 = 0;
    float    value_float = 0;
};

ControlVM & ControlViewModel();
CustomRparamVM & CustomRparamViewModel();

} // namespace vm
} // namespace mim
