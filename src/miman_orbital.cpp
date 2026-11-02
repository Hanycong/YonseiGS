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


int rotator = -1;
char ControlBuf[32];
char ReadBuf[32];

Observer Yonsei(37.564021, 126.934600, 0.05);

extern StateCheckUnit State;
extern Console console;


Tle TLElistup[32768];
Tle FDSlistup;
int NowSat = -2;

struct Antenna Now;

float AZI_TOLERANCE = 3.0;
float ELE_TOLERANCE = 3.0;

struct termios new_rotator_tio;

extern Setup * setup;
extern int NowTLE;
extern Sband * sgs;

// init_rotator -> model/device/rotator_io.cpp 로 이동

// fin_rotator -> model/device/rotator_io.cpp 로 이동

// SatelliteObject::Initializer -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Name -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Azimuth -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Elevation -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Azimuth_1secfuture -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Elevation_1secfuture -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::AOS_Azimuth -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::AOS_Elevation -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::MaxElevation -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::DisplayPoint_Longitude -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::DisplayPoint_Latitude -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::Update -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::GeneratePath -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::GeneratePassList -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::FindMaxElevation -> model/orbit/satellite_object.cpp 로 이동

// SatelliteObject::FindCrossingPoint -> model/orbit/satellite_object.cpp 로 이동

// SetTLEinfo -> model/orbit/tle_loader.cpp 로 이동

// Initialize_TLESettings -> model/orbit/tle_loader.cpp 로 이동

// Delete_TLESettings -> model/orbit/tle_loader.cpp 로 이동

// write_data -> model/orbit/tle_loader.cpp 로 이동

// DownloadTLE -> model/orbit/tle_loader.cpp 로 이동

// TrackingSatellites -> model/orbit/tle_loader.cpp 로 이동

// ReadTLELines_Errorhandling -> model/orbit/tle_loader.cpp 로 이동

// ReadTLELines -> model/orbit/tle_loader.cpp 로 이동


// SatelliteInitialize -> model/orbit/tle_loader.cpp 로 이동


// RotatorSetTo -> model/device/rotator_io.cpp 로 이동

// RotatorEnter -> model/device/rotator_io.cpp 로 이동

// RotatorReadInfo -> model/device/rotator_io.cpp 로 이동

// Doppler -> model/orbit/doppler.cpp 로 이동
;

// void SetNowTracking(int index)
// {

//     if (index < -1 || index >= SAT_MAX_NUM || (index >= 0 && State.Satellites[index] == NULL)) {
//     console.AddLog("[ERROR]##[Track]SetNowTracking rejected. index=%d ptr=%p",
//                     index, (index >= 0 && index < SAT_MAX_NUM) ? State.Satellites[index] : NULL);
//     NowSat = -1;
//     return;
//     }
//     NowSat = index;
// }
void SetNowTracking_impl(int index, const char* file, int line)
{
    if (index < -1 || index >= SAT_MAX_NUM || (index >= 0 && State.Satellites[index] == NULL)) {
        console.AddLog("[ERROR]##[Track]SetNowTracking rejected. index=%d ptr=%p caller=%s:%d",
                       index, (index >= 0 && index < SAT_MAX_NUM) ? State.Satellites[index] : NULL,
                       file, line);
        NowSat = -1;
        return;
    }
    NowSat = index;
}

#define SetNowTracking(i) SetNowTracking_impl((i), __FILE__, __LINE__)

int GetNowTracking()
{
    return NowSat;
}

// Sattracker -> model/device/rotator_io.cpp 로 이동

// FSWTleConverter -> model/orbit/tle_loader.cpp 로 이동
