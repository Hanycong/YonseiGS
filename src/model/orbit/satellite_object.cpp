// Model - 위성 객체 (SGP4 위치·관측 방위/고도, 패스(AOS/LOS/최대 고도) 예측, 지도 경로)
//
// YonseiGS_orbital.cpp 에서 SatelliteObject 멤버 함수를 내용 그대로 옮긴 것. 클래스 선언은 YonseiGS_config.h.
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
#include "YonseiGS_orbital.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_config.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_s_control.h"
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

void SatelliteObject::Initializer()
{
    try
    {
        
        this->start_date = DateTime::Now(true);
        this->end_date = DateTime(start_date.AddDays(7.0));
        this->_eci = this->sgp4.FindPosition(DateTime::Now(true));
        this->_eci_1secfuture = this->sgp4.FindPosition(DateTime::Now(true).AddSeconds(this->futuretime));
        this->topo = this->obs.GetLookAngle(this->_eci);
        this->_topo_1secfuture = this->obs.GetLookAngle(this->_eci_1secfuture);
        this->geo = this->_eci.ToGeodetic();
        this->GeneratePassList(180);
        for(int i = 0; i < 64; i++)
        {
            this->AOSeci[i] = this->sgp4.FindPosition(this->_nextaos[i]);
            this->AOStopo[i] = this->obs.GetLookAngle(this->AOSeci[i]);
        }
        
        
    }
    catch(const std::exception& e)
    {
        console.AddLog("[DEBUG]##%s\t%s\t",this->_name, e.what());
    }
}

char * SatelliteObject::Name()
{
    return this->_name;
}

int SatelliteObject::Azimuth()
{
    return static_cast<int>(this->topo.azimuth * RAD_TO_DEG);
}

int SatelliteObject::Elevation()
{
    return static_cast<int>(this->topo.elevation * RAD_TO_DEG);
}

int SatelliteObject::Azimuth_1secfuture()
{
    return static_cast<int>(this->_topo_1secfuture.azimuth * RAD_TO_DEG);
}

int SatelliteObject::Elevation_1secfuture()
{
    return static_cast<int>(this->_topo_1secfuture.elevation * RAD_TO_DEG);
}

int SatelliteObject::AOS_Azimuth(int i)
{
    return static_cast<int>(this->AOStopo[i].azimuth * RAD_TO_DEG);
}

int SatelliteObject::AOS_Elevation(int i)
{
    return static_cast<int>(this->AOStopo[i].elevation * RAD_TO_DEG);
}

double SatelliteObject::MaxElevation(int i)
{
    return this->_max_elevation[i] * RAD_TO_DEG;
}

float SatelliteObject::DisplayPoint_Longitude()
{
    return (float)(this->geo.longitude * RAD_TO_DEG / 180);
}

float SatelliteObject::DisplayPoint_Latitude()
{
    return (float)(this->geo.latitude * RAD_TO_DEG / 90);
}

void SatelliteObject::Update()
{
    this->sgp4.SetTle(tle);
    this->_eci = this->sgp4.FindPosition(DateTime::Now(true));
    this->_eci_1secfuture = this->sgp4.FindPosition(DateTime::Now(true).AddSeconds(this->futuretime));
    this->topo = this->obs.GetLookAngle(this->_eci);
    this->_topo_1secfuture = this->obs.GetLookAngle(this->_eci_1secfuture);
    this->geo = this->_eci.ToGeodetic();
}

void SatelliteObject::GeneratePath()
{
    Eci ECIpath;
    CoordTopocentric TOPOpath;
    DateTime test_date = DateTime(this->_nextaos[0]);
    DateTime fin_date = DateTime(this->_nextlos[0]);
    TimeSpan step = fin_date - test_date;
    step = TimeSpan(step.Ticks() / 1023);
    this->CrossPath = 0;
    for(int i = 0; i < 1024; i++)
    {
        ECIpath = this->sgp4.FindPosition(test_date);
        TOPOpath = this->obs.GetLookAngle(ECIpath);
        path_az[i] = TOPOpath.azimuth;
        path_el[i] = TOPOpath.elevation;
        test_date = test_date + step;
    }
    // printf("Satellite : %s\n", this->Name());
    double cal;
    for(int i = 0; i < 1023; i++)
    {
        
        cal = abs(path_az[i + 1] - path_az[i]) * RAD_TO_DEG;
        // printf("Cal : %lf, az : %lf\n", cal, path_az[i]);
        if(cal > 90.0f)
        {
           
            //Counterclockwise Crossing
            if(this->path_az[0] * RAD_TO_DEG < 90 && this->path_az[1023] * RAD_TO_DEG > 90)
            {
                console.AddLog("Azimuth Crossing Path(Counterclockwise)");
                this->CrossPath = 1;
            }
                
            //Clockwise Crossing
            else if(this->path_az[0] * RAD_TO_DEG > 90 && this->path_az[1023] * RAD_TO_DEG < 90)
            {
                console.AddLog("Azimuth Crossing Path(Clockwise)");
                this->CrossPath = -1;

            }
            //Require Manual Control
            else
            {
                console.AddLog("This Path Requires manual control.");
                this->CrossPath = -2;
            }
            break;
        }
    }

}

bool SatelliteObject::GeneratePassList(const int time_step)
{
    bool found_aos = false;
    DateTime previous_date = DateTime(this->start_date);
    DateTime current_date = DateTime(this->start_date);

    for(int i = 0; i < 64; i++)
    {
        while (current_date < this->end_date)
        {
            bool end_of_pass = false;

            /*
            * calculate satellite position
            */
            Eci eci = this->sgp4.FindPosition(current_date);
            CoordTopocentric topo = this->obs.GetLookAngle(eci);

            if (!found_aos && topo.elevation > 0.0)
            {
                /*
                * aos hasnt occured yet, but the satellite is now above horizon
                * this must have occured within the last time_step
                */
                if (this->start_date == current_date)
                {
                    /*
                    * satellite was already above the horizon at the start,
                    * so use the start time
                    */
                    this->_nextaos[i] = this->start_date;
                }
                else
                {
                    /*
                    * find the point at which the satellite crossed the horizon
                    */
                    this->_nextaos[i] = this->FindCrossingPoint(this->obs, this->sgp4, previous_date, current_date, true);
                }
                found_aos = true;
            }
            else if (found_aos && topo.elevation < 0.0)
            {
                found_aos = false;
                /*
                * end of pass, so move along more than time_step
                */
                end_of_pass = true;
                /*
                * already have the aos, but now the satellite is below the horizon,
                * so find the los
                */
                this->_nextlos[i] = this->FindCrossingPoint(this->obs, this->sgp4, previous_date, current_date, false);

                this->_max_elevation[i] = this->FindMaxElevation(this->obs, this->sgp4, this->_nextaos[i], this->_nextlos[i]);
            }

            /*
            * save current time
            */
            previous_date = current_date;

            if (end_of_pass)
            {
                /*
                * at the end of the pass move the time along by 30mins
                */
                current_date = current_date + TimeSpan(0, 30, 0);
                found_aos = false;
                end_of_pass = false;
                break;
            }
            else
            {
                /*
                * move the time along by the time step value
                */
                current_date = current_date + TimeSpan(0, 0, time_step);
            }

            if (current_date > end_date)
            {
                /*
                * dont go past end time
                */
                current_date = this->end_date;
            }
        };

        if (found_aos)
        {
            /*
            * satellite still above horizon at end of search period, so use end
            * time as los
            */
            this->_nextlos[i] = this->end_date;
            this->_max_elevation[i] = this->FindMaxElevation(this->obs, this->sgp4, this->_nextaos[i], this->_nextlos[i]);
            return false;
        }
    }

    
    return true;
}

double SatelliteObject::FindMaxElevation(Observer obs, SGP4& sgp4, const DateTime& aos, const DateTime& los)
{
    //Observer obs(user_geo);
    bool running;

    double time_step = (los - aos).TotalSeconds() / 9.0;
    DateTime current_time(aos); //! current time
    DateTime time1(aos); //! start time of search period
    DateTime time2(los); //! end time of search period
    double max_elevation; //! max elevation

    running = true;

    do
    {
        running = true;
        max_elevation = -99999999999999.0;
        while (running && current_time < time2)
        {
            /*
            * find position
            */
            Eci eci = sgp4.FindPosition(current_time);
            CoordTopocentric topo = obs.GetLookAngle(eci);

            if (topo.elevation > max_elevation)
            {
                /*
                * still going up
                */
                max_elevation = topo.elevation;
                /*
                * move time along
                */
                current_time = current_time.AddSeconds(time_step);
                if (current_time > time2)
                {
                    /*
                    * dont go past end time
                    */
                    current_time = time2;
                }
            }
            else
            {
                /*
                * stop
                */
                running = false;
            }
        }

        /*
        * make start time to 2 time steps back
        */
        time1 = current_time.AddSeconds(-2.0 * time_step);
        /*
        * make end time to current time
        */
        time2 = current_time;
        /*
        * current time to start time
        */
        current_time = time1;
        /*
        * recalculate time step
        */
        time_step = (time2 - time1).TotalSeconds() / 9.0;
    }
    while (time_step > 1.0);
        return max_elevation;
}

DateTime SatelliteObject::FindCrossingPoint(Observer obs, SGP4& sgp4, const DateTime& initial_time1, const DateTime& initial_time2, bool finding_aos)
{
    //Observer obs(user_geo);
    bool running;
    int cnt;

    DateTime time1(initial_time1);
    DateTime time2(initial_time2);
    DateTime middle_time;

    running = true;
    cnt = 0;
    while (running && cnt++ < 16)
    {
        middle_time = time1.AddSeconds((time2 - time1).TotalSeconds() / 2.0);
        /*
        * calculate satellite position
        */
        Eci eci = sgp4.FindPosition(middle_time);
        CoordTopocentric topo = obs.GetLookAngle(eci);

        if (topo.elevation > 0.0)
        {
            /*
            * satellite above horizon
            */
            if (finding_aos)
            {
                time2 = middle_time;
            }
            else
            {
                time1 = middle_time;
            }
        }
        else
        {
            if (finding_aos)
            {
                time1 = middle_time;
            }
            else
            {
                time2 = middle_time;
            }
        }

        if ((time2 - time1).TotalSeconds() < 1.0)
        {
            /*
            * two times are within a second, stop
            */
            running = false;
            /*
            * remove microseconds
            */
            int us = middle_time.Microsecond();
            middle_time = middle_time.AddMicroseconds(-us);
            /*
            * step back into the pass by 1 second
            */
            middle_time = middle_time.AddSeconds(finding_aos ? 1 : -1);
        }
    }

    /*
    * go back/forward 1second until below the horizon
    */
    running = true;
    cnt = 0;
    while (running && cnt++ < 6)
    {
        Eci eci = sgp4.FindPosition(middle_time);
        CoordTopocentric topo = obs.GetLookAngle(eci);
        if (topo.elevation > 0)
        {
            middle_time = middle_time.AddSeconds(finding_aos ? -1 : 1);
        }
        else
        {
            running = false;
        }
    }

    return middle_time;
}
