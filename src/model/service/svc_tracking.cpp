#include "svc_tracking.h"
#include "svc_thread.h"

#include "miman_config.h"
#include "miman_orbital.h"
#include "miman_autopilot.h"

extern StateCheckUnit State;
extern Console console;
extern int BeaconCounter;
extern int PingCounter;

namespace mim {
namespace svc {

void StartEngage()
{
    State.Engage = true;
    StartThread(SLOT_ROTATOR_CONTROL, Sattracker, NULL);
    console.AddLog("[OK]##Start Engaging.");
}

void StopEngage()
{
    State.Engage = false;
    State.Doppler = false;
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
    State.RotatorReading = false;
    JoinThread(SLOT_ROTATOR_READ);
    // fin_rotator();
}

void StartDoppler()
{
    if(State.GS100_Connection)
    {
        State.Doppler = true;
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
    State.Doppler = false;
    JoinThread(SLOT_DOPPLER);
}

void StartAutoPilot()
{
    State.Autopilot = true;
    StartThread(SLOT_AUTOPILOT, AutoPilot, NULL);
}

void StopAutoPilot()
{
    State.Autopilot = false;
    JoinThread(SLOT_AUTOPILOT);
}

int StartSatelliteInitialize(void * sgp4check)
{
    return StartThread(SLOT_TLE_LOAD, &SatelliteInitialize, sgp4check);
}

} // namespace svc
} // namespace mim
