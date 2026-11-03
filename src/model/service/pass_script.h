// Service 계층 - 패스 TC 스크립트 (AutoPilot 이 한 패스에서 순서대로 보내는 명령 목록)
//
// 한 단계 = 명령 바이트(위성으로 나가는 CCSDS 패킷 그대로) + 반복 횟수 + 송신 후 대기 시간.
// 송신은 기존 수동 송신과 같은 경로(SendUplink → WaitTx, HVD_TEST / MIM_PT_TMTC_TEST)를 쓴다.
//
// 스크립트는 텍스트 파일로 저장한다 (기본 ../data/autopilot/pass_script.txt, bin/ 기준).
//   # 주석 ("#" 다음에 공백. 단계 이름은 "#2 ..." 처럼 '#' 으로 시작할 수 있지만 "# " 로는 못 한다)
//   이름 | 반복 | 대기[s] | 바이트(16진수, 공백 구분)
//   EPS P60 DOCK GET TABLE HK | 2 | 10 | 18 75 C0 00 00 01 14 47
#pragma once

#include <stdint.h>
#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace mim {
namespace svc {

struct TcStep {
    std::string name;
    std::vector<uint8_t> bytes;
    int repeat = 2;                     // 같은 명령을 몇 번 보낼지
    int delay_s = 10;                   // 한 번 보낸 뒤 기다리는 시간 [s]
};

// 한 단계 값의 허용 범위 (파일 읽기와 편집 화면이 같은 기준을 쓴다)
enum {
    TC_MAX_BYTES  = 256,
    TC_MAX_REPEAT = 20,
    TC_MAX_DELAY  = 600,
    TC_MAX_NAME   = 120,                // [byte]
};

// 예전 AutoPilot_task 에 하드코딩돼 있던 명령 순서
// (EPS HK 3종 → AX100 / EPS DOCK GND WDT 리셋 → EPS HK 3종, 각 2회, 10 s 간격)
std::vector<TcStep> DefaultPassScript();

// step 을 repeat 번 보낸다. 보낼 때마다 sent_total 을 1 올린다 (nullptr 이면 무시). 보낸 횟수를 돌려준다.
//   may_send   : 한 번 보낼 때마다 그 직전에 부른다 (송신 조건을 기다려도 된다). false 면 멈춘다.
//   keep_going : 송신 사이 대기 중 0.1 s 마다 부른다. false 면 멈춘다 (Stop, LOS).
int SendTcStep(const TcStep & step, const std::function<bool()> & may_send,
               const std::function<bool()> & keep_going, std::atomic<int> * sent_total = nullptr);

// "18 75 c0 0x00, 01" 처럼 공백/쉼표로 구분한 16진수 바이트를 읽는다
bool ParseHexBytes(const std::string & text, std::vector<uint8_t> & out, std::string & err);
std::string FormatHexBytes(const std::vector<uint8_t> & bytes);

// ---- 현재 스크립트 (편집 화면이 고치고, AutoPilot 스레드가 패스마다 복사해 간다) ----
class PassScript
{
public:
    int    Count() const;
    TcStep At(int i) const;
    std::vector<TcStep> Snapshot() const;

    void Set(int i, const TcStep & step);
    void Insert(int i, const TcStep & step);    // i 앞에 넣는다 (i == Count() 면 끝에)
    void Remove(int i);
    void Swap(int i, int j);
    void ResetToDefault();

    // 실패하면 지금 스크립트를 그대로 두고 err 에 "line N: ..." 를 쓴다
    bool Load(const std::string & path, std::string & err);
    bool Save(const std::string & path, std::string & err);
    std::string Path() const;           // 마지막으로 읽거나 저장한 파일 (처음엔 기본 경로)

    // 프로그램 시작 때 파일 읽기. 파일이 없으면 기본 스크립트를 쓰고,
    // 파일이 있는데 틀렸으면 빈 스크립트로 시작한다 (운용자가 지운 명령이 기본값으로 되살아나 나가지 않도록).
    // 그 오류는 StartupError() 로 남아, Load / Save / ResetToDefault 가 성공할 때까지 AutoPilot 시작을 막는다.
    void LoadAtStartup(const std::string & path);
    std::string StartupError() const;

    static const char * DefaultPath();

private:
    friend PassScript & CurrentPassScript();
    PassScript();

    mutable std::mutex mu_;
    std::vector<TcStep> steps_;
    std::string path_;
    std::string startup_error_;
};

// 처음 부를 때 기본 경로의 파일을 LoadAtStartup 으로 읽는다
PassScript & CurrentPassScript();

// ---- 송신 조건 (앙각·안테나·링크가 모두 맞아야 TC 를 한 번 보낸다) ----
struct PassGate {
    bool   has_elevation = false;       // 위성 위치를 계산할 수 있었는지 (TLE 다시 읽는 중이면 false)
    double elevation_deg = 0;           // 지금 앙각 [deg] (SGP4)
    bool   elevation_ok  = false;       // elevation_deg >= AutoPilotCfg.MinElevation
    bool   rotator_ok    = false;       // 로테이터 연결
    bool   engage_ok     = false;       // 로테이터 자동 추적(Engage) 중
    bool   target_ok     = false;       // 로테이터가 추적하는 위성(Track 대상)이 이 위성
    bool   radio_ok      = false;       // GS100 연결
    bool   link_ok       = false;       // 이번 패스에 위성 응답(핑 또는 비콘)이 있었음 (RequireLink 가 꺼져 있으면 true)

    bool Antenna() const { return rotator_ok && engage_ok && target_ok && radio_ok; }
    bool Ok() const      { return elevation_ok && Antenna() && link_ok; }
};

PassGate EvaluatePassGate(int sat);

} // namespace svc
} // namespace mim
