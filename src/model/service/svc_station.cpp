#include "svc_station.h"
#include "svc_thread.h"

#include <signal.h>
#include <stdio.h>
#include <csp/switch.h>

#include "miman_config.h"
#include "miman_orbital.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_csp.h"
#include "miman_s_control.h"

extern StateCheckUnit State;
extern Console console;
extern Setup * setup;

namespace mim {
namespace svc {

void InitializeHardware(void (*sigint_handler)(int))
{
    //Switch Box
    if(init_switch(setup->Switch_devname, setup->Switch_baud) < 0)
    {
        console.AddLog("[ERROR]##Connect to Switch Box..............Failed.");
        State.SwitchConnection = false;
    }
    else
    {
        console.AddLog("[OK]##Connect to Switch Box.............Success.");
        State.SwitchConnection = true;
    }


    //Rotator
    if(init_rotator() < 0)
    {
        console.AddLog("[ERROR]##Connect to Rotator.................Failed.");
    }
    else
    {
        State.RotatorConnection = true;
        State.RotatorInitialize = true;
        console.AddLog("[OK]##Connect to Rotator................Success.");
    }
    if(init_transceiver() < 0)
    {
        console.AddLog("[ERROR]##Connect to GS100...................Failed.");
        State.GS100_Connection = false;
    }
    else
    {
        // pthread_join(p_thread[11], NULL);
        // pthread_join(p_thread[12], NULL);
        // pthread_join(p_thread[13], NULL);
        console.AddLog("[OK]##Connect to GS100..................Success.");
        State.GS100_Connection = true;

        StartThread(SLOT_RADIO_INIT, RadInitialize, (void *)setup);
        State.TRx_mode = true;
        State.downlink_mode = true;
        StartThread(SLOT_TRX_CONTROLLER, TRxController, NULL);


        signal(SIGINT, sigint_handler);

    }
    if(State.SbandUse)
    {
        if(init_sband(setup) < 0)
        {
            console.AddLog("[ERROR]##Connect to S-band GS...............Failed.");
        }
        else
        {
            console.AddLog("[OK]##Connect to S-band GS...............Success.");
        }
    }
    State.InitializeRequired = false;
    StartThread(SLOT_TLE_TRACKING, TrackingSatellites, NULL);
    StartThread(SLOT_ROTATOR_READ, RotatorReadInfo, NULL);
}

void FinishHardware()
{
    fin_rotator();
    fin_transceiver();
}

void StopAllThreads()
{
    printf("[MAIN-DIAG] entering shutdown sequence\n");
    printf("[MAIN-DIAG] TRx=%d downlink=%d uplink=%d ftp=%d\n",
            State.TRx_mode,
            State.downlink_mode,
            State.uplink_mode,
            State.ftp_mode);
    printf("Ending procedure...\n");
    State.TRx_mode = false;
    State.downlink_mode = false;
    State.uplink_mode = false;
    State.ftp_mode = false;
    State.Engage = false;
    State.Doppler = false;
    State.GS100_Connection =false;
    State.NowTracking = false;
    State.TrackThread = false;
    State.RotatorReading = false;

    State.InitializeRequired = true;
    State.Autopilot = false;
    printf("Joined thread : \n");
    for(int i = 0; i < SLOT_COUNT; i++)
    {
        JoinThread((Slot)i);
        printf("%d\t", i);
    }
    printf("\n");
}

} // namespace svc
} // namespace mim
