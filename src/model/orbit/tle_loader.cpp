// Model - TLE 목록·다운로드·파일 읽기, 위성 목록 초기화, FSW 용 TLE 변환
//
// miman_orbital.cpp 에서 TLE 관련 함수를 내용 그대로 옮긴 것. 선언은 miman_orbital.h 그대로.
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

TLEinfo * SetTLEinfo(const char * label)
{
    TLEinfo* res = (TLEinfo *)malloc(sizeof(TLEinfo));
    sprintf(res->remote, "https://celestrak.org/NORAD/elements/gp.php?GROUP=%s&FORMAT=tle", label);
    sprintf(res->local, "../bin/tle/%s.dat", label);
    sprintf(res->label, label);
    return res;
}

void Initialize_TLESettings()
{
    State.tleinfolistup[0] = SetTLEinfo("cubesat");
    State.tleinfolistup[1] = SetTLEinfo("last-30-days");
    State.tleinfolistup[2] = SetTLEinfo("weather");
    State.tleinfolistup[3] = SetTLEinfo("dmc");
    State.tleinfolistup[4] = SetTLEinfo("amateur");
    State.tleinfolistup[5] = SetTLEinfo("active");

    // printf("%s\t%s\t%s\n", State.tleinfolistup[0]->label, State.tleinfolistup[0]->remote, State.tleinfolistup[0]->local);
}

void Delete_TLESettings()
{
    for(int i = 0; i < sizeof(State.tleinfolistup); i++)
    {
        if(State.tleinfolistup[i] != NULL)
            free(State.tleinfolistup[i]);
        memset(State.tleinfolistup[i], 0, sizeof(State.tleinfolistup[i]));
    }
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool DownloadTLE(char *TLEURL, char *Local)
{
    //TLE Propagator Testing Part
    //TLE Download
    CURL *TLE_dcurl;
    FILE *TLE_fp;
    CURLcode TLEres;
    if (TLE_dcurl = curl_easy_init())
    {
        TLE_fp = fopen(Local, "wb");
        curl_easy_setopt(TLE_dcurl, CURLOPT_URL, TLEURL);
        curl_easy_setopt(TLE_dcurl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(TLE_dcurl, CURLOPT_WRITEDATA, TLE_fp);
        TLEres = curl_easy_perform(TLE_dcurl);

        curl_easy_cleanup(TLE_dcurl);
        fclose(TLE_fp);
        return true;
    }
    else
    {
        return false;
    }
}

void * TrackingSatellites(void *)
{
    while(State.TrackThread)
    {
        if(!State.NowTracking)
            continue;
        if((State.Fatellites != NULL) && (State.Fatellites->use != false))
        {
            try
            {
                State.Fatellites->Update();
            }
            catch(const std::exception& e)
            {
                console.AddLog("[ERROR]##Unable to Track Satellite : %s", State.Fatellites->Name());
                console.AddLog(e.what());
            }
        }
        for(int i = 0; i < sizeof(State.Satellites) / sizeof(SatelliteObject *); i++)
        {
            if((State.Satellites[i] == NULL) || (State.Satellites[i]->use == false))
                continue;
            else
            {
                if(State.Satellites[i]->cal = false)
                {
                    State.Satellites[State.loadindex]->Refresh(State.Satellites[i]->tle, Yonsei, false, true);
                    State.loadindex++;
                }
                try
                {
                    State.Satellites[i]->Update();
                }
                catch(const std::exception& e)
                {
                    console.AddLog("[ERROR]##Unable to Track Satellite : %s", State.Satellites[i]->Name());
                    console.AddLog(e.what());
                }
            }
        }
        usleep(10000);
    }
    return NULL;
}

int ReadTLELines_Errorhandling(char * TLEFilename, char * trackingTLEname, bool fds, bool sgp4on)
{
    std::string TLEResultName;
    std::string TLEResultLine1;
    std::string TLEResultLine2;
    std::ifstream TLEDATA;
    TLEDATA.open(TLEFilename);
    State.loadindex = GetNowTracking();

    DateTime start_date = DateTime::Now(true);
    DateTime end_date(start_date.AddDays(7.0));

    while (getline(TLEDATA, TLEResultName) && getline(TLEDATA, TLEResultLine1) && getline(TLEDATA, TLEResultLine2))
    {
        
        TLEResultName.erase(std::find_if(TLEResultName.rbegin(), TLEResultName.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultName.end());
        TLEResultLine1.erase(std::find_if(TLEResultLine1.rbegin(), TLEResultLine1.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine1.end());
        TLEResultLine2.erase(std::find_if(TLEResultLine2.rbegin(), TLEResultLine2.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine2.end());
        if(!strstr(TLEResultName.c_str(), trackingTLEname))
        {
            continue;
        }
        try
        {
            if(fds)
                FDSlistup = Tle(TLEResultName, TLEResultLine1, TLEResultLine2);
            else
            {
                TLElistup[State.loadindex] = Tle(TLEResultName, TLEResultLine1, TLEResultLine2);
                
            }
                
        }
        catch(const std::exception& e)
        {
            console.AddLog("[DEBUG]##TLE Load Error. Index : %d, Error : %s", State.loadindex, e.what());
        }
        try
        {
            if(fds)
            {
                if(strlen(State.Fatellites->Name()) != 0)
                    State.Fatellites->~SatelliteObject();
                State.Fatellites->Refresh(FDSlistup, Yonsei);
                break;
            }
            else
            {
                if(strlen(State.Satellites[State.loadindex]->Name()) != 0)
                    State.Satellites[State.loadindex]->~SatelliteObject();
                State.Satellites[State.loadindex]->Refresh(TLElistup[State.loadindex], Yonsei, false, sgp4on);
                State.loadindex++;
                // printf("Now : %d\n", State.loadindex);
            }
            
        }
        catch(const std::exception& e)
        {
            console.AddLog("[DEBUG]##SGP4 Error during importing. Skip this satellite and continue. Error : %s", e.what());
        }
        
    }
    TLEDATA.close();
    return State.loadindex;
}

int ReadTLELines(char *TLEFilename, bool fds, bool sgp4on)
{
    std::string TLEResultName;
    std::string TLEResultLine1;
    std::string TLEResultLine2;
    std::ifstream TLEDATA;
    TLEDATA.open(TLEFilename);
    State.loadindex = 0;

    DateTime start_date = DateTime::Now(true);
    DateTime end_date(start_date.AddDays(7.0));

    while (getline(TLEDATA, TLEResultName) && getline(TLEDATA, TLEResultLine1) && getline(TLEDATA, TLEResultLine2))
    {
        TLEResultName.erase(std::find_if(TLEResultName.rbegin(), TLEResultName.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultName.end());
        TLEResultLine1.erase(std::find_if(TLEResultLine1.rbegin(), TLEResultLine1.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine1.end());
        TLEResultLine2.erase(std::find_if(TLEResultLine2.rbegin(), TLEResultLine2.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), TLEResultLine2.end());
        try
        {
            if(fds)
                FDSlistup = Tle(TLEResultName, TLEResultLine1, TLEResultLine2);
            else
            {
                TLElistup[State.loadindex] = Tle(TLEResultName, TLEResultLine1, TLEResultLine2);
                
            }
                
        }
        catch(const std::exception& e)
        {
            console.AddLog("[DEBUG]##TLE Load Error. Index : %d, Error : %s", State.loadindex, e.what());
        }
        try
        {
            if(fds)
            {
                if(strlen(State.Fatellites->Name()) != 0)
                    State.Fatellites->~SatelliteObject();
                State.Fatellites->Refresh(FDSlistup, Yonsei);
                break;
            }
            else
            {
                if(strlen(State.Satellites[State.loadindex]->Name()) != 0)
                    State.Satellites[State.loadindex]->~SatelliteObject();
                State.Satellites[State.loadindex]->Refresh(TLElistup[State.loadindex], Yonsei, false, sgp4on);
                State.loadindex++;
                // printf("Now : %d\n", State.loadindex);
            }
            
        }
        catch(const std::exception& e)
        {
            console.AddLog("[DEBUG]##SGP4 Error during importing. Skip this satellite and continue. Error : %s", e.what());
        }
        
    }
    TLEDATA.close();
    return State.loadindex;
}

void * SatelliteInitialize(void * sgp4on)
{
    
    State.tleallindex = 32768;
    AppUi.Display_TLE = false;
    AppUi.Display_load = true;
    for(State.loadindex = 0; State.loadindex < sizeof(State.Satellites) / sizeof(SatelliteObject *); State.loadindex++)
    {
        
        if(State.Satellites[State.loadindex] != NULL)
        {
            if(strlen(State.Satellites[State.loadindex]->Name()) == 0)
                break;
            State.Satellites[State.loadindex]->~SatelliteObject();
        }
    }
    if((State.tleallindex = ReadTLELines(State.tleinfolistup[NowTLE]->local, false, *((bool *)sgp4on))) <= 0)
        console.AddLog("[ERROR]##Invalid type of TLE File. Please check again.");
    
    
    AppUi.Display_load = false;
    AppUi.Display_TLE = true;
    State.NowTracking = true;
    State.loadindex = 0;
    return NULL;
}

void FSWTleConverter(Tle& TleInfo, FSWTle * fswtle)
{

    bool in_degree = true;

    DateTime UTCtime = DateTime::Now(true);
    DateTime EpochInfo = TleInfo.Epoch();

    fswtle->fingerprint[0] = 1;
    fswtle->fingerprint[1] = 2;
    fswtle->fingerprint[2] = 0;
    fswtle->fingerprint[3] = 8;

    time_t now, standard;
    struct tm epochtime;
    epochtime.tm_year = 1980 - 1990;
    epochtime.tm_mon = 0;
    epochtime.tm_mday = 1;
    epochtime.tm_hour = 0;
    epochtime.tm_min = 0;
    epochtime.tm_sec = 0;
    epochtime.tm_isdst = 0;
    now = time(NULL);
    standard = mktime(&epochtime);
    fswtle->sec = static_cast<unsigned int>(difftime(standard, now));


    // fswtle->sec =
    //  (uint32_t)(UTCtime.Second() + 60 * UTCtime.Minute() + 60 * 60 * UTCtime.Hour() + 60 * 60 * 24 * UTCtime.DayOfYear(UTCtime.Year(), UTCtime.Month(), UTCtime.Day()));
    //fswtle->subsec = (uint32_t)(UTCtime.Microsecond()); //to b
    fswtle->micsec = (uint32_t)(UTCtime.Microsecond());
    //***************************TBD

    fswtle->epochYear = (uint32_t)(EpochInfo.Year());
    fswtle->epochDay = (uint32_t)(EpochInfo.DayOfYear(EpochInfo.Year(), EpochInfo.Month(), EpochInfo.Day()))-1;

    fswtle->INC = (double)(TleInfo.Inclination(in_degree));
    fswtle->RAAN = (double)(TleInfo.RightAscendingNode(in_degree));
    fswtle->ECC = (double)(TleInfo.Eccentricity());
    fswtle->AOP = (double)(TleInfo.ArgumentPerigee(in_degree));
    fswtle->MA = (double)(TleInfo.MeanAnomaly(in_degree));
    fswtle->MM = (double)(TleInfo.MeanMotion());
    fswtle->padding[0] = 0;
    fswtle->padding[1] = 0;
    fswtle->padding[2] = 0;
    fswtle->padding[3] = 0;
    fswtle->Checksum = gs_crc32(((uint8_t *)(&fswtle->sec)),72);

    console.AddLog("[DEBUG]##Ping Format - Time\nFingerprint : %u%u%u%u\tSecond : %u\tMicroSecond : %u\t EpochYear : %u\t, EpochDay : %u\n",
                    fswtle->fingerprint[0], fswtle->fingerprint[1], fswtle->fingerprint[2], fswtle->fingerprint[3],
                    fswtle->sec, fswtle->micsec, fswtle->epochYear, fswtle->epochDay);
    console.AddLog("[DEBUG]##Ping Format - TLE\nInclination : %f\t RAAN : %f\t Eccentricity :%f\t AOP : %f\t MA : %f\t MM : %f\t Checksum : %u\n",
                    fswtle->INC, fswtle->RAAN, fswtle->ECC, fswtle->AOP, fswtle->MA, fswtle->MM, fswtle->Checksum);
}