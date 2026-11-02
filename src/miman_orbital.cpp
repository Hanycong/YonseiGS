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

int init_rotator() {
	rotator = open(setup->Rotator_devname, O_RDWR | O_NOCTTY | O_NDELAY); // Or O_RDWR | O_NOCTTY? // @@ 로테이터 열기
    // open 함수 https://www.it-note.kr/19
    // O_RDWR은 파일 디스크립터인 fd를 읽기와 쓰기 모드로 열기 위한 지정이며
    // O_NOCCTY와 O_NONBLOCK는 시리얼 통신 장치에 맞추어 추가했습니다.

    if (rotator<0)
    {
		return -1;
	}
    // tcgetattr(rotator, &new_rotator_tio);
    tcflush(rotator, TCIOFLUSH);
    switch(setup->Rotator_baud)
    {
        case 4800 : new_rotator_tio.c_cflag = B4800; break;
        case 9600 : new_rotator_tio.c_cflag = B9600; break;
        default : break;
    }
    

    new_rotator_tio.c_cflag |= (CLOCAL | CREAD); 	// Receiver and local mode
    new_rotator_tio.c_cflag &= ~HUPCL;				// Do not change modem signals
    new_rotator_tio.c_cflag &= ~CSIZE;
    new_rotator_tio.c_cflag |= CS8;					// 8 bit // @@ S대역 포지셔너(로테이터) 컨트롤러이랑 같은 옵션이듯 // @@ 시리얼 통신 설정방법: https://softtone-someday.tistory.com/15, https://blog.naver.com/choi125496/130034222760
    new_rotator_tio.c_cflag &= ~CSTOPB;				// 1 stop bit
    new_rotator_tio.c_cflag &= ~PARENB;				// No parity check
    new_rotator_tio.c_cflag &= ~CRTSCTS;			// Enable hardware / software flow control

    new_rotator_tio.c_iflag &= ~IXON;				// No handshaking

    // Raw output
    new_rotator_tio.c_oflag &= ~OPOST; 				// Prevent special interpretation of output bytes (e.g. newline chars)

    // Canonical input
    new_rotator_tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	// Non-canonical read

    // Initialize control characters
    new_rotator_tio.c_cc[VTIME]    = 5;
    new_rotator_tio.c_cc[VMIN]     = 1;		// Block 0.5(VTIME) sec after read first(VMIN) character

    tcflush(rotator, TCIOFLUSH);
    


    if (tcsetattr(rotator, TCSANOW, &new_rotator_tio) != 0)
    {
        console.AddLog("[ERROR]##Fail to configure rotator serial port\n");
        return -1;
    }
    	
    return 0;
}

void fin_rotator()
{
    if (rotator > 0)
    {
        tcflush(rotator, TCIOFLUSH);
        tcsetattr(rotator, TCSANOW, &new_rotator_tio);
        tcflush(rotator, TCIOFLUSH);
        close(rotator);
    }
}

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


void RotatorSetTo(int azimuth, int elevation)
{   
    State.RotatorReadReady = false;
    sprintf(ControlBuf, "W%03d %03d\r\n", azimuth, elevation);
    tcflush(rotator, TCIOFLUSH);
    int res = write(rotator, ControlBuf, 10);
    if(res > 0)
    {
        console.AddLog("Tracking : Az %d, El %d.", azimuth, elevation);
    }
    else
        console.AddLog("[ERROR]##Write to rotator failed.");
    
    sleep(1);
    State.RotatorReadReady = true;
}

void * RotatorEnter(void *)
{
    
    write(rotator, "\r\n", 2);
    int byte = 1;
    memset(ReadBuf, 0, strlen(ReadBuf));
    byte = read(rotator, ReadBuf, sizeof(ReadBuf));
    console.AddLog("[DEBUG]##Rotator : %s, Read Byte : %d", ReadBuf, byte);
    State.RotatorReading = true;
    tcflush(rotator, TCIOFLUSH);
    return NULL;
}

void * RotatorReadInfo(void *)
{
    State.RotatorReading = true;
    int byte = 0;
    int read_count = 0;
    while(State.RotatorReading)
    {
        tcflush(rotator, TCIOFLUSH);
        memset(ReadBuf, 0, strlen(ReadBuf));

        if(write(rotator, "C2\r\n", 4) > 0)
        {
            State.RotatorReadReady = true;
        }
        else
        {
            console.AddLog("[ERROR]##Rotator Read Failed. Please Check Again.");
            State.RotatorReading = false;
            break;
        }

        State.RotatorReading = true;
        
        sleep(1);
        while(State.RotatorReading && State.RotatorReadReady)
        {
            byte = read(rotator, &ReadBuf[read_count], 1);
            if (byte < 1 && read_count > 3 && ReadBuf[read_count - 1] == '\n')
            {
                char azi[5] = {0}, ele[5] = {0};

                if (ReadBuf[3] == '-')	// AZ=-XXX
                {
                    // Sometime azi / ele can be negative value
                    // in that case, we need calibration

                    strncpy(azi, &ReadBuf[3], 4);
                    Now.az_now= atoi(azi);

                    fprintf(stderr, "Rotator azimuth calibration needed \n");

                    if (ReadBuf[12] == '-')		// AZ=-XXX  EL=-XXX
                    {
                        strncpy(ele, &ReadBuf[12], 4);
                        Now.el_now = atoi(ele);

                        fprintf(stderr, "Rotator elevation calibration needed \n");
                    }
                    else								// AZ=-XXX  EL=XXX
                    {
                        strncpy(ele, &ReadBuf[12], 3);
                        Now.el_now = atoi(ele);
                    }
                }
                else
                {
                    strncpy(azi, &ReadBuf[3], 3);
                    Now.az_now = atoi(azi);

                    if (ReadBuf[11] == '-')		// AZ=XXX  EL=-XXX
                    {
                        strncpy(ele, &ReadBuf[11], 4);
                        Now.el_now = atoi(ele);

                        fprintf(stderr, "Rotator elevation calibration needed \n");
                    }
                    else								// AZ=XXX  EL=XXX
                    {
                        strncpy(ele, &ReadBuf[11], 3);
                        Now.el_now = atoi(ele);
                    }
                }
                read_count = 0;
                sleep(1);
                State.RotatorReadReady = false;
            }
            else if (byte > 0)
            {
                read_count++;
            }
            else if (byte < 1)
            {
                console.AddLog("[ERROR]##Rotator Read Failed.");
                sleep(30);
                break;
            }
        }
    tcflush(rotator,TCIOFLUSH);
    }
    
    if(State.RotatorReading)
        State.RotatorReading = false;
    return NULL;
}

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
};

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

void * Sattracker(void *)
{
    State.Engage = true;

    while(State.Engage)
    {
        if(NowSat == -1)
        {
            if(State.Fatellites == NULL)
                continue;
            if(State.Fatellites->Elevation() < 0)
            {
                console.AddLog("[DEBUG]##Change to AOS.");
                Now.az_tag = State.Fatellites->AOS_Azimuth();
                Now.el_tag = State.Fatellites->AOS_Elevation();
            }
            else
            {
                Now.az_tag = State.Fatellites->Azimuth_1secfuture();
                Now.el_tag = State.Fatellites->Elevation_1secfuture();
            }
            if(Now.el_now > 80)
            {
                Now.el_tag = 80;
                State.Fatellites->futuretime = 6.0f;
            }
            else
                State.Fatellites->futuretime = 3.0f;
            
            if(State.Fatellites->CrossPath)
            {
                switch(State.Fatellites->CrossPath)
                {
                    case(1): {
                        
                        if(Now.az_tag < 90)
                            Now.az_tag += 360;
                        break;
                    }
                    case(-1): {
                        if(Now.az_tag < 90)
                            Now.az_tag += 360;
                        break;
                    }
                }
            }
            if ((abs(Now.az_tag - Now.az_now) >= AZI_TOLERANCE || abs(Now.el_tag - Now.el_now) >= ELE_TOLERANCE))
            {
                

                console.AddLog("[DEBUG]##Azimuth : %d, Elevation : %d\n", Now.az_tag, Now.el_tag);
                RotatorSetTo(Now.az_tag, Now.el_tag);
            }
            else
                console.AddLog("[DEBUG]##Tolerance Filtering...");
            sleep(1);
        }
        else
        {
            if(State.Satellites[NowSat] == NULL)
            {
                // console.AddLog("[ERROR]##Cannot Detect Tracking State.Satellites.");
                continue;
            }
        
            if(State.Satellites[NowSat]->Elevation() < 0)
            {
                console.AddLog("[DEBUG]##Change to AOS.");
                Now.az_tag = State.Satellites[NowSat]->AOS_Azimuth();
                Now.el_tag = State.Satellites[NowSat]->AOS_Elevation();
            }
            else
            {
                Now.az_tag = State.Satellites[NowSat]->Azimuth_1secfuture();
                Now.el_tag = State.Satellites[NowSat]->Elevation_1secfuture();
            }
            if(Now.el_now > 80)
            {
                Now.el_tag = 80;
                State.Satellites[NowSat]->futuretime = 6.0f;
            }
            else
                State.Satellites[NowSat]->futuretime = 3.0f;
            
            if(State.Satellites[NowSat]->CrossPath)
            {
                switch(State.Satellites[NowSat]->CrossPath)
                {
                    case(1): {
                        if(Now.az_tag < 90)
                            Now.az_tag += 360;
                        break;
                    }
                    case(-1): {
                        if(Now.az_tag < 90)
                            Now.az_tag += 360;
                        break;
                    }
                }
            }
            if ((abs(Now.az_tag - Now.az_now) >= AZI_TOLERANCE || abs(Now.el_tag - Now.el_now) >= ELE_TOLERANCE))
            {
                

                console.AddLog("[DEBUG]##Azimuth : %d, Elevation : %d\n", Now.az_tag, Now.el_tag);
                RotatorSetTo(Now.az_tag, Now.el_tag);
            }
            else
                console.AddLog("[DEBUG]##Tolerance Filtering...");
            sleep(1);
        }
        
    }
    return NULL;
}

// FSWTleConverter -> model/orbit/tle_loader.cpp 로 이동
