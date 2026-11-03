#include "svc_tracking.h"
#include "svc_thread.h"

#include "YonseiGS_config.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_autopilot.h"

extern StateCheckUnit State;
extern Console console;
extern int BeaconCounter;
extern int PingCounter;

namespace mim {
namespace svc {

void StartEngage()
{
    RotatorFlags.Engage = true;
    StartThread(SLOT_ROTATOR_CONTROL, Sattracker, NULL);
    console.AddLog("[OK]##Start Engaging.");
}

void StopEngage()
{
    RotatorFlags.Engage = false;
    LinkFlags.Doppler = false;
    JoinThread(SLOT_ROTATOR_CONTROL);
    console.AddLog("[OK]##Finish Engaging.");
    console.AddLog("Beacon Success : %d, Ping Success : %d", BeaconCounter, PingCounter);
    BeaconCounter = 0;
    PingCounter = 0;
}

void RotatorOn()
{
    init_rotator();
    StartThread(SLOT_ROTATOR_READ, RotatorReadInfo, NULL);
}

void RotatorOff()
{
    RotatorFlags.RotatorReading = false;
    JoinThread(SLOT_ROTATOR_READ);
    // fin_rotator();
}

void StartDoppler()
{
    if(LinkFlags.GS100_Connection)
    {
        LinkFlags.Doppler = true;
        StartThread(SLOT_DOPPLER, Doppler, NULL);
        console.AddLog("[OK]##Start Doppler Correction.");
    }
    else
    {
        console.AddLog("[ERROR]##GS100 Connection Error - Cannot Start Doppler Correction.");
    }
}

void StopDoppler()
{
    LinkFlags.Doppler = false;
    JoinThread(SLOT_DOPPLER);
}

void StartAutoPilot()
{
    JoinThread(SLOT_AUTOPILOT);         // 이전 실행의 스레드 (이미 끝났으면 바로 돌아온다)
    AutoPilotCfg.Autopilot = true;
    StartThread(SLOT_AUTOPILOT, AutoPilot, NULL);
}

// 멈추라고만 알린다. 스레드는 추적 해제 등 정리를 마치고 스스로 끝나며 (Stage 가 AP_IDLE 로 돌아온다),
// join 은 다음 StartAutoPilot 또는 종료 시 StopAllThreads 가 한다 (UI 스레드를 막지 않도록).
void StopAutoPilot()
{
    AutoPilotCfg.Autopilot = false;
}

int StartSatelliteInitialize(void * sgp4check)
{
    return StartThread(SLOT_TLE_LOAD, &SatelliteInitialize, sgp4check);
}

} // namespace svc
} // namespace mim
