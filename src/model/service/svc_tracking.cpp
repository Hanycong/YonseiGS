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
    AutoPilotCfg.Autopilot = true;
    StartThread(SLOT_AUTOPILOT, AutoPilot, NULL);
}

void StopAutoPilot()
{
    AutoPilotCfg.Autopilot = false;
    JoinThread(SLOT_AUTOPILOT);
}

int StartSatelliteInitialize(void * sgp4check)
{
    return StartThread(SLOT_TLE_LOAD, &SatelliteInitialize, sgp4check);
}

} // namespace svc
} // namespace mim
