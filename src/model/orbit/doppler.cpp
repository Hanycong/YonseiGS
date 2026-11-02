// Model - 도플러 보정 스레드 (추적 위성의 거리 변화율로 GS100 RX/TX 주파수 갱신)
//
// miman_orbital.cpp 에서 Doppler 스레드 함수를 내용 그대로 옮긴 것. 선언은 miman_orbital.h 그대로.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <curl/curl.h>
#include <curl/easy.h>
#include <vector>
#include <list>
#include <pthread.h>
#include <Util.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

//For SGP4(Testing)
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <SGP4.h>

//Custom Headers
#include "miman_orbital.h"
#include "miman_radial.h"
#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_s_control.h"
#include <gs/util/crc32.h>


extern int rotator;
extern char ControlBuf[32];
extern char ReadBuf[32];
extern Observer Yonsei;
extern StateCheckUnit State;
extern Console console;
extern Tle TLElistup[32768];
extern Tle FDSlistup;
extern int NowSat;
extern struct Antenna Now;
extern float AZI_TOLERANCE;
extern float ELE_TOLERANCE;
extern struct termios new_rotator_tio;
extern Setup * setup;
extern int NowTLE;
extern Sband * sgs;

void * Doppler(void *) {
    int index = -2;
    uint32_t rxfreq, txfreq;
    double sol = 299792458;  // [m/s]
    double vel;
    double updated_freq;
    while(State.Doppler)
    {
        while(!State.RotatorReadReady)
            continue;
        index = GetNowTracking();
        // printf("%d\n", index);
        if (!State.Engage) {
            console.AddLog("[ERROR]##You tried doppler correction with no engaging");
            State.Doppler = false;
            break;
        }
        if(index < -1)
        {
            console.AddLog("[ERROR]##Please load TLE or FDS. Stop Doppler Correction.");
            State.Doppler = false;
            break;
        }
        else if (index == -1)
        {
            if(State.Fatellites == NULL)
            {
                State.Doppler = false;
                break;
            }
            vel= 1 - (State.Fatellites->topo.range_rate * 1000)/sol;
            updated_freq = vel * (setup->default_freq);
            
            rxfreq = uint32_t(static_cast<unsigned int>(updated_freq));
            txfreq = uint32_t(static_cast<unsigned int>((setup->default_freq) * 2 - updated_freq));
            console.AddLog("[DEBUG]##TX : %u, RX : %u", txfreq, rxfreq);

            //Set freq
            SetRxFreq(rxfreq);
            SetTxFreq(txfreq);
            int8_t rx_table_id = 1;
            int8_t tx_table_id = 5;
            // Save2FRAM((void *)&rx_table_id);
            // Save2FRAM((void *)&tx_table_id);
            usleep(DOPPLER_TIMESTEP);
            
            UpdateFreq();
        }
        else
        {
            if(State.Satellites[index] == NULL)
            {
                State.Doppler = false;
                break;
            }
            vel= 1 - (State.Satellites[index]->topo.range_rate * 1000)/sol;
            updated_freq = vel * (setup->default_freq);
            
            rxfreq = uint32_t(static_cast<unsigned int>(updated_freq));
            txfreq = uint32_t(static_cast<unsigned int>((setup->default_freq) * 2 - updated_freq));
            console.AddLog("[DEBUG]##TX : %u, RX : %u", txfreq, rxfreq);
            //Set freq
            SetRxFreq(rxfreq);
            SetTxFreq(txfreq);
            int8_t rx_table_id = 1;
            int8_t tx_table_id = 5;
            // Save2FRAM((void *)&rx_table_id);
            // Save2FRAM((void *)&tx_table_id);
            usleep(DOPPLER_TIMESTEP);
            UpdateFreq();
        }
        
    }
    
    return NULL;
}