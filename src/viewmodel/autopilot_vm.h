// ViewModel - AutoPilot 창 (자동 운용 설정, 시작/정지, 카운터, 간이 명령 생성기)
//
// 체크박스·입력 칸이 바로 편집하는 값은 포인터로 내준다 (State 의 운용 플래그, 생성기 입력 버퍼).
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>

namespace mim {
namespace svc { struct TcStep; }
namespace vm {

class AutoPilotVM
{
public:
    // ---- 자동 운용 설정 ----
    bool * DoBeaconCheck();
    bool * DoPing();
    bool * DoCmd();
    bool * AmpMode();
    int *  Iteration();
    int *  SpanTime();

    bool Running() const;               // State.Autopilot
    void Start();
    void Stop();

    // ---- 3. Autopilot 탭: 송신 조건 설정 ----
    float * MinElevation();             // [deg]
    bool *  RequireLink();

    // ---- 3. Autopilot 탭: 대상 위성과 패스 ----
    // 실행 중이면 AutoPilot 이 잡은 패스, 아니면 지금 추적 대상(Track 버튼)의 다음 패스
    bool HasTarget() const;
    const char * TargetName() const;    // 없으면 "-"
    bool HasPass() const;
    void PassTimes(char * aos, char * los, size_t size) const;  // "11/3 23:05:12" (한국 시각)
    float PassMaxElevation() const;     // [deg]
    bool CanStart() const;              // 추적 대상이 있고, 스크립트 파일 오류가 없어야 시작할 수 있다
    const char * ScriptError() const;   // 시작 때 스크립트 파일을 못 읽은 이유 ("" 이면 없음)

    // ---- 3. Autopilot 탭: 진행 상태 ----
    int  Stage() const;                 // AutoPilotStage
    static const char * StageName(int stage);
    struct Gate {
        bool has_elevation;
        float elevation;                // [deg]
        bool elevation_ok, rotator_ok, engage_ok, target_ok, radio_ok, link_ok, antenna_ok, ok;
    };
    Gate CurrentGate() const;
    int  ScriptStep() const;            // 보내는 중인 단계, 없으면 -1
    int  ScriptSent() const;

    // ---- 3. Autopilot 탭: TC 스크립트 편집 (실행 중에는 바꿀 수 없다) ----
    bool Editable() const;
    int  StepCount() const;
    struct StepRow {
        char name[128];
        char bytes[128];                // 16진수 (길면 잘라서 "..." )
        int  repeat;
        int  delay_s;
    };
    StepRow Row(int i) const;
    int  SelectedStep() const;
    void SelectStep(int i);             // 편집 칸에 그 단계 값을 채운다
    char * EditName();
    size_t EditNameSize() const;
    char * EditBytes();
    size_t EditBytesSize() const;
    int *  EditRepeat();
    int *  EditDelay();
    void ApplyEdit();                   // 고른 단계를 편집 칸 값으로 바꾼다
    void AddStep();                     // 편집 칸 값으로 새 단계 (고른 단계 뒤)
    void RemoveStep();
    void MoveStep(int dir);             // -1 위로, +1 아래로
    void ResetScript();
    void SaveScript();
    void LoadScript(const char * path);
    const char * ScriptPath() const;
    char * DialogPath();                // 파일 선택 창이 고른 경로를 쓰는 칸
    size_t DialogPathSize() const;
    const char * Message() const;       // 마지막 편집/파일 작업 결과 ("" 이면 없음)
    bool MessageIsError() const;

    // ---- 카운터 ----
    int BeaconCount() const;
    int PingCount() const;
    int CmdCount() const;
    bool QueuedCommandReady(int i) const;   // SatCMD[i] 가 있으면

    // ---- 간이 명령 생성기 (옛 CMDDataGenerator) ----
    uint32_t * TestMsgId();
    uint16_t * TestFncCode();
    void * TestHeader();                // 명령 헤더 버퍼 (CFE_MSG_CommandHeader)
    size_t TestHeaderSize() const;

    bool * ChecksumOn();
    bool * SchedulerOn();
    uint32_t * ExecutionTime();
    uint32_t * ExecutionWindow();
    uint16_t * EntryId();
    uint16_t * GroupId();
    void Generate(uint32_t msgid, uint16_t fnccode, void * Requested, size_t RequestedSize);   // "Generate"
    void ClearRequest(void * Requested, size_t RequestedSize);                                  // "Close"

private:
    void SetMessage(const std::string & msg, bool error);
    bool ReadEdit(mim::svc::TcStep & out);   // 편집 칸 값 검사 (틀리면 Message 에 이유)

    int  selected_ = -1;
    char edit_name_[128] = "";
    char edit_bytes_[1024] = "";
    int  edit_repeat_ = 1;
    int  edit_delay_ = 10;
    char dialog_path_[512] = "";
    mutable std::string script_path_;   // ScriptPath() 가 돌려줄 문자열
    mutable std::string script_error_;  // ScriptError() 가 돌려줄 문자열
    std::string message_;
    bool message_error_ = false;
};

AutoPilotVM & AutoPilotViewModel();

} // namespace vm
} // namespace mim
