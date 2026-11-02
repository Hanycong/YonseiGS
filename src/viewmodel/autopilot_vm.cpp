#include "autopilot_vm.h"

#include <string.h>

#include "miman_config.h"
#include "miman_coms.h"
#include "svc_tracking.h"

extern StateCheckUnit State;
extern Console console;
extern int BeaconCounter;
extern int PingCounter;
extern CmdGenerator_GS * SatCMD[256];
extern int NowCMD;

// 옛 CMDDataGenerator 의 입력 상태 (miman_imgui.cpp 전역에서 옮김)
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

AutoPilotVM & AutoPilotViewModel()
{
    static AutoPilotVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
