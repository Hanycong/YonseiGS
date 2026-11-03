#include "autopilot_vm.h"

#include <stdio.h>
#include <string.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_tracking.h"
#include "pass_script.h"
#include "track_target.h"
#include "YonseiGS_autopilot.h"

extern StateCheckUnit State;
extern Console console;
extern int BeaconCounter;
extern int PingCounter;
extern CmdGenerator_GS * SatCMD[256];
extern int NowCMD;

// 옛 CMDDataGenerator 의 입력 상태 (YonseiGS_imgui.cpp 전역에서 옮김)
bool SchedulerState = false;
bool ChecksumState = true;
uint32_t ExecutionTimeBuf = 0;
uint32_t ExecutionWindowBuf = 0;
uint16_t EntryIDBuf = 0;
uint16_t GroupIDBuf = 0;

// AutoPilot 창의 간이 생성기 입력 (창 함수 안 static 에서 옮김)
static uint32_t test_msgid;
static uint16_t test_fnccode;
static CFE_MSG_CommandHeader dummy_hdr;

namespace mim {
namespace vm {

bool * AutoPilotVM::DoBeaconCheck() { return &AutoPilotCfg.DoBeaconCheck; }
bool * AutoPilotVM::DoPing()        { return &AutoPilotCfg.DoPing; }
bool * AutoPilotVM::DoCmd()         { return &AutoPilotCfg.DoCMD; }
bool * AutoPilotVM::AmpMode()       { return &AutoPilotCfg.AMPmode; }
int *  AutoPilotVM::Iteration()     { return &AutoPilotCfg.iteration; }
int *  AutoPilotVM::SpanTime()      { return &AutoPilotCfg.SpanTime; }

bool AutoPilotVM::Running() const { return AutoPilotCfg.Autopilot; }
void AutoPilotVM::Start() { mim::svc::StartAutoPilot(); }
void AutoPilotVM::Stop()  { mim::svc::StopAutoPilot(); }

int AutoPilotVM::BeaconCount() const { return BeaconCounter; }
int AutoPilotVM::PingCount() const   { return PingCounter; }
int AutoPilotVM::CmdCount() const    { return AutoPilotCfg.CMDCount; }
bool AutoPilotVM::QueuedCommandReady(int i) const { return SatCMD[i] != NULL; }

uint32_t * AutoPilotVM::TestMsgId()   { return &test_msgid; }
uint16_t * AutoPilotVM::TestFncCode() { return &test_fnccode; }
void * AutoPilotVM::TestHeader()      { return &dummy_hdr; }
size_t AutoPilotVM::TestHeaderSize() const { return sizeof(dummy_hdr); }

bool * AutoPilotVM::ChecksumOn()        { return &ChecksumState; }
bool * AutoPilotVM::SchedulerOn()       { return &SchedulerState; }
uint32_t * AutoPilotVM::ExecutionTime()   { return &ExecutionTimeBuf; }
uint32_t * AutoPilotVM::ExecutionWindow() { return &ExecutionWindowBuf; }
uint16_t * AutoPilotVM::EntryId()         { return &EntryIDBuf; }
uint16_t * AutoPilotVM::GroupId()         { return &GroupIDBuf; }

// "Generate": 원래 버튼 본문 그대로 (끝의 지역 변수 4개는 전역을 가리는 선언이라 아무 효과 없음 — 원래 그대로)
void AutoPilotVM::Generate(uint32_t msgid, uint16_t fnccode, void * Requested, size_t RequestedSize)
{
		SatCMD[NowCMD] = new CmdGenerator_GS();
        console.AddLog("[DEBUG]##Generate : %u, %u, %d\n", msgid, fnccode, RequestedSize);
        if(RequestedSize == sizeof(CFE_MSG_CommandHeader))
            SatCMD[NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, NULL);
        else
		    SatCMD[NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, Requested + 8);
		if(SchedulerState)
        {
            SatCMD[NowCMD]->Scheduled = true;
            SatCMD[NowCMD]->Scheduling(ExecutionTimeBuf, ExecutionWindowBuf, EntryIDBuf, GroupIDBuf);
        }
        else if(!ChecksumState)
        {
            SatCMD[NowCMD]->Checksum = false;
        }
			
		NowCMD ++;

		console.AddLog("Generated Command Message.");
        memset(Requested, 0, RequestedSize);
		uint32_t ExecutionTimeBuf = 0;
		uint32_t ExecutionWindowBuf = 0;
		uint16_t EntryIDBuf = 0;
		uint16_t GroupIDBuf = 0;
        ChecksumState = true;
        SchedulerState = false;
	}

// "Close"
void AutoPilotVM::ClearRequest(void * Requested, size_t RequestedSize)
{
    memset(Requested, 0, RequestedSize);
}

// ---------------------------------------------------------------- 3. Autopilot 탭

using mim::svc::CurrentPassScript;
using mim::svc::TcStep;

float * AutoPilotVM::MinElevation() { return &AutoPilotCfg.MinElevation; }
bool *  AutoPilotVM::RequireLink()  { return &AutoPilotCfg.RequireLink; }

// 실행 중이면 AutoPilot 이 잡은 위성, 아니면 Track 버튼으로 고른 위성
static int TargetSat()
{
    int sat = AutoPilotCfg.Autopilot ? AutoPilotCfg.PassSat.load() : mim::model::Tracking().Index();
    if (sat < 0 && AutoPilotCfg.Autopilot)
        sat = mim::model::Tracking().Index();
    if (!OrbitData.NowTracking || sat < 0 || sat >= SAT_MAX_NUM || OrbitData.Satellites[sat] == NULL ||
        !OrbitData.Satellites[sat]->Name()[0])
        return -1;
    return sat;
}

static int TargetPass(int sat)
{
    int idx = AutoPilotCfg.Autopilot ? AutoPilotCfg.PassIndex.load() : -1;
    if (idx < 0 || idx >= 64)
        idx = FindValuablePath(sat);
    return idx;
}

bool AutoPilotVM::HasTarget() const { return TargetSat() >= 0; }

const char * AutoPilotVM::TargetName() const
{
    int sat = TargetSat();
    return sat >= 0 ? OrbitData.Satellites[sat]->Name() : "-";
}

bool AutoPilotVM::HasPass() const
{
    int sat = TargetSat();
    return sat >= 0 && OrbitData.Satellites[sat]->_max_elevation[TargetPass(sat)] > 0;
}

void AutoPilotVM::PassTimes(char * aos, char * los, size_t size) const
{
    int sat = TargetSat();
    if (sat < 0)
    {
        snprintf(aos, size, "-");
        snprintf(los, size, "-");
        return;
    }
    int idx = TargetPass(sat);
    DateTime a = OrbitData.Satellites[sat]->_nextaos[idx].AddHours(9);
    DateTime l = OrbitData.Satellites[sat]->_nextlos[idx].AddHours(9);
    snprintf(aos, size, "%d/%d %02d:%02d:%02d", a.Month(), a.Day(), a.Hour(), a.Minute(), a.Second());
    snprintf(los, size, "%d/%d %02d:%02d:%02d", l.Month(), l.Day(), l.Hour(), l.Minute(), l.Second());
}

float AutoPilotVM::PassMaxElevation() const
{
    int sat = TargetSat();
    return sat >= 0 ? (float)OrbitData.Satellites[sat]->MaxElevation(TargetPass(sat)) : 0.f;
}

// 이전 실행이 정리를 마칠 때까지(Stage 가 AP_IDLE) 다시 시작하지 않는다
bool AutoPilotVM::CanStart() const { return !Running() && Stage() == AP_IDLE && HasTarget(); }

// Stop 을 누른 뒤 스레드가 정리하는 동안은 "Stopping..."
int AutoPilotVM::Stage() const
{
    int st = AutoPilotCfg.Stage;
    return (!Running() && st != AP_IDLE) ? AP_STOPPING : st;
}

const char * AutoPilotVM::StageName(int stage)
{
    switch (stage)
    {
    case AP_WAIT_PASS:  return "Waiting for pass (starts 5 min before AOS)";
    case AP_ENGAGE:     return "Starting rotator tracking / Doppler";
    case AP_BEACON:     return "Checking beacon";
    case AP_PING:       return "Checking link (ping)";
    case AP_WAIT_GATE:  return "Waiting for TC conditions";
    case AP_TC_SCRIPT:  return "Sending TC script";
    case AP_FTP:        return "UL signal / FTP";
    case AP_WAIT_LOS:   return "Waiting for LOS";
    case AP_POST_PASS:  return "Pass finished - updating TLE";
    case AP_STOPPING:   return "Stopping...";
    default:            return "Stopped";
    }
}

AutoPilotVM::Gate AutoPilotVM::CurrentGate() const
{
    mim::svc::PassGate g = mim::svc::EvaluatePassGate(TargetSat());
    Gate r;
    r.has_elevation = g.has_elevation;
    r.elevation     = (float)g.elevation_deg;
    r.elevation_ok  = g.elevation_ok;
    r.rotator_ok    = g.rotator_ok;
    r.engage_ok     = g.engage_ok;
    r.radio_ok      = g.radio_ok;
    r.link_ok       = g.link_ok;
    r.antenna_ok    = g.Antenna();
    r.ok            = g.Ok();
    return r;
}

int AutoPilotVM::ScriptStep() const { return AutoPilotCfg.ScriptStep; }
int AutoPilotVM::ScriptSent() const { return AutoPilotCfg.ScriptSent; }

bool AutoPilotVM::Editable() const { return !Running(); }
int  AutoPilotVM::StepCount() const { return CurrentPassScript().Count(); }

AutoPilotVM::StepRow AutoPilotVM::Row(int i) const
{
    TcStep st = CurrentPassScript().At(i);
    StepRow r;
    snprintf(r.name, sizeof(r.name), "%s", st.name.c_str());
    std::string hex = mim::svc::FormatHexBytes(st.bytes);
    if (hex.size() >= sizeof(r.bytes))
        hex = hex.substr(0, sizeof(r.bytes) - 4) + "...";
    snprintf(r.bytes, sizeof(r.bytes), "%s", hex.c_str());
    r.repeat  = st.repeat;
    r.delay_s = st.delay_s;
    return r;
}

int AutoPilotVM::SelectedStep() const { return selected_ < StepCount() ? selected_ : -1; }

void AutoPilotVM::SelectStep(int i)
{
    if (i < 0 || i >= StepCount())
        return;
    selected_ = i;
    TcStep st = CurrentPassScript().At(i);
    snprintf(edit_name_, sizeof(edit_name_), "%s", st.name.c_str());
    snprintf(edit_bytes_, sizeof(edit_bytes_), "%s", mim::svc::FormatHexBytes(st.bytes).c_str());
    edit_repeat_ = st.repeat;
    edit_delay_  = st.delay_s;
    message_.clear();
}

char * AutoPilotVM::EditName()             { return edit_name_; }
size_t AutoPilotVM::EditNameSize() const   { return sizeof(edit_name_); }
char * AutoPilotVM::EditBytes()            { return edit_bytes_; }
size_t AutoPilotVM::EditBytesSize() const  { return sizeof(edit_bytes_); }
int *  AutoPilotVM::EditRepeat()           { return &edit_repeat_; }
int *  AutoPilotVM::EditDelay()            { return &edit_delay_; }

void AutoPilotVM::SetMessage(const std::string & msg, bool error)
{
    message_ = msg;
    message_error_ = error;
}

bool AutoPilotVM::ReadEdit(TcStep & out)
{
    std::string name = edit_name_;
    for (char & c : name)
        if (c == '|')
            c = '/';
    size_t b = name.find_first_not_of(' ');
    if (b == std::string::npos)
    {
        SetMessage("Enter a name for the step.", true);
        return false;
    }
    out.name = name.substr(b);
    std::string err;
    if (!mim::svc::ParseHexBytes(edit_bytes_, out.bytes, err))
    {
        SetMessage("Bytes: " + err, true);
        return false;
    }
    if (edit_repeat_ < 1 || edit_repeat_ > mim::svc::TC_MAX_REPEAT)
    {
        SetMessage("Repeat must be 1-" + std::to_string(mim::svc::TC_MAX_REPEAT) + ".", true);
        return false;
    }
    if (edit_delay_ < 0 || edit_delay_ > mim::svc::TC_MAX_DELAY)
    {
        SetMessage("Delay must be 0-" + std::to_string(mim::svc::TC_MAX_DELAY) + " s.", true);
        return false;
    }
    out.repeat  = edit_repeat_;
    out.delay_s = edit_delay_;
    return true;
}

void AutoPilotVM::ApplyEdit()
{
    TcStep st;
    if (!Editable() || SelectedStep() < 0 || !ReadEdit(st))
        return;
    CurrentPassScript().Set(selected_, st);
    SetMessage("Step " + std::to_string(selected_ + 1) + " updated (not saved).", false);
}

void AutoPilotVM::AddStep()
{
    TcStep st;
    if (!Editable() || !ReadEdit(st))
        return;
    int at = SelectedStep() < 0 ? StepCount() : selected_ + 1;
    CurrentPassScript().Insert(at, st);
    selected_ = at;
    SetMessage("Step " + std::to_string(at + 1) + " added (not saved).", false);
}

void AutoPilotVM::RemoveStep()
{
    if (!Editable() || SelectedStep() < 0)
        return;
    CurrentPassScript().Remove(selected_);
    SetMessage("Step " + std::to_string(selected_ + 1) + " removed (not saved).", false);
    if (selected_ >= StepCount())
        selected_ = StepCount() - 1;
    if (selected_ >= 0)
    {
        std::string keep = message_;
        SelectStep(selected_);
        SetMessage(keep, false);
    }
}

void AutoPilotVM::MoveStep(int dir)
{
    int to = selected_ + dir;
    if (!Editable() || SelectedStep() < 0 || to < 0 || to >= StepCount())
        return;
    CurrentPassScript().Swap(selected_, to);
    selected_ = to;
}

void AutoPilotVM::ResetScript()
{
    if (!Editable())
        return;
    CurrentPassScript().ResetToDefault();
    selected_ = -1;
    SetMessage("Built-in script restored (not saved).", false);
}

void AutoPilotVM::SaveScript()
{
    if (!Editable())
        return;
    std::string err;
    std::string path = CurrentPassScript().Path();
    if (CurrentPassScript().Save(path, err))
    {
        SetMessage("Saved to " + path, false);
        console.AddLog("[OK]##[Auto]Pass TC script saved: %s (%d steps)", path.c_str(), StepCount());
    }
    else
        SetMessage(err, true);
}

void AutoPilotVM::LoadScript(const char * path)
{
    if (!Editable())
        return;
    std::string err;
    if (CurrentPassScript().Load(path, err))
    {
        selected_ = -1;
        SetMessage("Loaded " + std::string(path), false);
        console.AddLog("[OK]##[Auto]Pass TC script loaded: %s (%d steps)", path, StepCount());
    }
    else
        SetMessage(err, true);
}

const char * AutoPilotVM::ScriptPath() const
{
    // 매 프레임 c_str 를 넘기므로 복사본을 멤버에 둔다
    script_path_ = CurrentPassScript().Path();
    return script_path_.c_str();
}

char * AutoPilotVM::DialogPath()           { return dialog_path_; }
size_t AutoPilotVM::DialogPathSize() const { return sizeof(dialog_path_); }

const char * AutoPilotVM::Message() const { return message_.c_str(); }
bool AutoPilotVM::MessageIsError() const  { return message_error_; }

AutoPilotVM & AutoPilotViewModel()
{
    static AutoPilotVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
