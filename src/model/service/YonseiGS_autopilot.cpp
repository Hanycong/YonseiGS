#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_model.h"
#include "YonseiGS_ftp.h"
#include "YonseiGS_autopilot.h"
#include "svc_thread.h"
#include "svc_tx.h"
#include "svc_radio.h"
#include "pass_script.h"

extern bool ftp_start_worker(pthread_t *thread, void *(*start_routine)(void *), void *arg, const char *label);

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
// p_thread[] 직접 접근 대신 model/service/svc_thread.h 의 슬롯 이름 사용
extern int NowTLE;
extern int NowFTP;
extern bool sgp4check;
extern int PingCounter;
extern int BeaconCounter;
extern CmdGenerator_GS * SatCMD[256];
static bool wdt_notsent;

// int FindValuablePath()
// {
//     int Valuable = 0;
//     for(int i = 0 ; i < 64; i++)
//     {
//         if(State.Satellites[GetNowTracking()]->MaxElevation(i) > 10.0f)
//         {
//             Valuable = i;
//             break;
//         }
//     }
//     return Valuable;
// }

int FindValuablePath(int satIndex)
{
    if (satIndex < 0 || satIndex >= SAT_MAX_NUM || !OrbitData.Satellites[satIndex]) return 0;
    int Valuable = 0;
    for (int i = 0; i < 64; i++) {
        if (OrbitData.Satellites[satIndex]->MaxElevation(i) > 10.0f) { Valuable = i; break; }
    }
    return Valuable;
}


void * AutoPilot(void *)
{
    console.AddLog("[Auto]Start AutoPilot.");
    int AutoRes = AutoPilot_task();
    if(AutoRes)
        console.AddLog("[ERROR]##[Auto]AutoPilot Stopped with Error Code : %d", AutoRes);
    if(AutoRes != -2)
    {
        LinkFlags.Doppler = false;
        mim::svc::JoinThread(mim::svc::SLOT_DOPPLER);
        console.AddLog("[OK]##[Auto]Stop Doppler Correction.");

        RotatorFlags.Engage = false;
        LinkFlags.Doppler = false;
        mim::svc::JoinThread(mim::svc::SLOT_ROTATOR_CONTROL);
        console.AddLog("[OK]##[Auto]Finish Engaging.");
        
        RotatorSetTo(0, 0);
        console.AddLog("[Auto]Beacon Success : %d, Ping Success : %d CMD Success : %d", BeaconCounter, PingCounter, AutoPilotCfg.CMDCount);
        BeaconCounter = 0;
        PingCounter = 0;
    }
    AutoPilotCfg.Successed = 0;
    return NULL;

}

static bool NowOn(int SatIndex, int NextValuablePath)
{
    if (SatIndex < 0 || SatIndex >= SAT_MAX_NUM) return false;
    if (NextValuablePath < 0 || NextValuablePath >= 64) return false;
    if (!OrbitData.Satellites[SatIndex]) return false;
    return (DateTime::Now(false) - OrbitData.Satellites[SatIndex]->_nextlos[NextValuablePath]).TotalSeconds() < 0;
}



int AutoPilot_task()
{
    bool BeaconAvailable = false;
    bool PingAvailable = false;
    char NowTrackingNameBuffer[32];
    int NowTracking = GetNowTracking();
    if (NowTracking < 0 || NowTracking >= SAT_MAX_NUM || !OrbitData.Satellites[NowTracking]) {
        console.AddLog("[ERROR]##[Auto]Invalid tracking at start. trk=%d", NowTracking);
        AutoPilotCfg.Autopilot = false;
        return -2;
    }
    snprintf(NowTrackingNameBuffer, sizeof(NowTrackingNameBuffer), "%s",
            OrbitData.Satellites[NowTracking]->Name());
    int SatIndex = GetNowTracking();
    if (SatIndex < 0 || SatIndex >= SAT_MAX_NUM || !OrbitData.Satellites[SatIndex]) return -2;

    while(AutoPilotCfg.Autopilot)
    {

        bool BeaconAvailable = false;
        bool PingAvailable = false;
        bool CalibrationSuccess = false; // Add
        printf("Start AutoPilot.\n");
        int NextValuablePath = FindValuablePath(SatIndex);
        console.AddLog("[Auto]Start Autopilot. Next Valuable AOS : %d/%d %d:%d:%d",
                                                        OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath].AddHours(9).Month(), 
                                                        OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath].AddHours(9).Day(),
                                                        OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath].AddHours(9).Hour(),
                                                        OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath].AddHours(9).Minute(),
                                                        OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath].AddHours(9).Second());
        console.AddLog("[Auto]Max Elevation : %.2lf° Please Keep Devices On.", OrbitData.Satellites[SatIndex]->_max_elevation[NextValuablePath] * RAD_TO_DEG);
        // printf("%f\n", (State.Satellites[SatIndex]->_nextaos[NextValuablePath] -  DateTime::Now(true)).TotalMinutes());
        while(AutoPilotCfg.Autopilot)
        {
            if((OrbitData.Satellites[SatIndex]->_nextaos[NextValuablePath] -  DateTime::Now(true)).TotalMinutes() < 5.0f)
                break;
            usleep(20000); // 20 ms
        }
        PathGenerator(SatIndex);

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        RotatorFlags.Engage = true;
        mim::svc::StartThread(mim::svc::SLOT_ROTATOR_CONTROL, Sattracker, NULL);
        console.AddLog("[OK]##[Auto]Start Engaging.");

        LinkFlags.Doppler = true;
        mim::svc::StartThread(mim::svc::SLOT_DOPPLER, Doppler, NULL);
        console.AddLog("[OK]##[Auto]Start Doppler Correction.");

        //Task
        //Check Beacon Received
        while(AutoPilotCfg.Autopilot && AutoPilotCfg.DoBeaconCheck)
        {
            if(BeaconCounter >= AutoPilotCfg.iteration)
            {
                BeaconAvailable = true;
                break;
            }
                
            if(!NowOn(SatIndex, NextValuablePath))
            {
                console.AddLog("[ERROR]##[Auto]Beacon Success is too hard. Beacon Success : %d", BeaconCounter);
                break;
            }
        }

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }


        if(AutoPilotCfg.AMPmode)
        {
    // --- DEBUG: show computed minutes-to-LOS ---
    double min_to_los =
        (OrbitData.Satellites[SatIndex]->_nextlos[NextValuablePath] - DateTime::Now(true)).TotalMinutes();

    // console.AddLog("[DBG][AMP] enter: SatIndex=%d Path=%d min_to_los=%.3f Autopilot=%d",
    //                SatIndex, NextValuablePath, min_to_los, (int)State.Autopilot);

    // throttle log inside loop (e.g., once per 1 second)
    DateTime last_log = DateTime::Now(false);

    while (AutoPilotCfg.Autopilot)
    {
        min_to_los =
            (OrbitData.Satellites[SatIndex]->_nextlos[NextValuablePath] - DateTime::Now(false)).TotalMinutes();

        if (min_to_los <= 15.0)
            break;

        // print at most once per 1 sec
        if ((DateTime::Now(false) - last_log).TotalSeconds() >= 1.0)
        {
            // console.AddLog("[DBG][AMP] waiting: min_to_los=%.3f (cond: %.3f > 15) now=%d/%d %02d:%02d:%02d",
            //                min_to_los, min_to_los,
            //                DateTime::Now(false).Month(), DateTime::Now(false).Day(),
            //                DateTime::Now(false).Hour(), DateTime::Now(false).Minute(), DateTime::Now(false).Second());
            last_log = DateTime::Now(false);
        }

        usleep(50000); // 50 ms
    }

    // console.AddLog("[DBG][AMP] exit-wait: min_to_los=%.3f Autopilot=%d",
                //    min_to_los, (int)State.Autopilot);

    if (!AutoPilotCfg.Autopilot)
        return 0;



            // while((State.Satellites[SatIndex]->_nextlos[NextValuablePath] - DateTime::Now(false)).TotalMinutes() > 15.0f)
            //     {
            //                     console.AddLog("[AUTO]##AMP Process running");
            //     if (!State.Autopilot) break;
            //     usleep(50000); // 50 ms
            //     }
            if(LinkFlags.AmpTime > 0)
            {
                console.AddLog("[ERRPR]##Cannot Turn on AMP Now. Wati until the timer goes on.");
                console.AddLog("[ERROR]##If you don't want it, restart the software.");
                console.AddLog("[ERROR]##Remind the AMP would be turned off after the timer goes on");
                console.AddLog("[ERROR]##even if you restart the software.");
            while (LinkFlags.AmpTime > 0 && AutoPilotCfg.Autopilot)
            {
                usleep(20000); // 20 ms
            }
            }
                
            pid_t pid = mim::svc::AmpReady("ampcontrol") ? fork() : -2; // -2: 앰프 미연결 (로그는 AmpReady 가 남김)
            if (pid == -2) {
            }
            else if (pid == -1) {
                console.AddLog("[ERROR]##Failed to fork process");
            }
            else if (pid == 0) {
                // Child process
                execl("../amp/ampcontrol", "ampcontrol", NULL);
                // exec 실패 시 자식이 GS 복제본으로 계속 돌면 X 연결이 깨져 본체까지 죽는다
                perror("execl ../amp/ampcontrol");
                _exit(127);
            }
            else
            {
                console.AddLog("[OK]##Forked Safe Amplifier Process.(ON, Timer)");
                mim::svc::StartThread(mim::svc::SLOT_AMP_TIMER, AmpTimer, NULL);
            }
        }
        
        /*-----------------Calibration Code-------------------*/
        if (AutoPilotCfg.DoBaudCalibration) {
            mim::svc::StartThread(mim::svc::SLOT_CALIBRATION, csp_baud_calibration, NULL);
        }
        if (AutoPilotCfg.DoFreqCalibration) {
            mim::svc::StartThread(mim::svc::SLOT_CALIBRATION, csp_freq_calibration, NULL);
        }
        /*-----------------------------------------------------*/

        //Ping Try
        while(AutoPilotCfg.Autopilot && AutoPilotCfg.DoPing && NowOn(SatIndex, NextValuablePath))
        {
            sleep(AutoPilotCfg.SpanTime);
            
            mim::svc::StartPingConsole();
            if(PingCounter >= 5)
            {
                PingAvailable = true;
                break;
            }
                
            if(!NowOn(SatIndex, NextValuablePath))
            {
                console.AddLog("[ERROR]##[Auto]Ping Success is too hard. Ping Success : %d", PingCounter);
                break;
            }
            mim::svc::WaitTx();
        }


        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        uint32_t now_total_rx_bytes = get_rx_bytes();
        wdt_notsent = true;   
        //Send Watchdog

if (wdt_notsent &&
    AutoPilotCfg.Autopilot &&
    AutoPilotCfg.DoCMD &&
    NowOn(SatIndex, NextValuablePath))
{
    // 이 패스에서 보낼 TC 목록 (model/service/pass_script)
    for (const mim::svc::TcStep & step : mim::svc::DefaultPassScript())
        mim::svc::SendTcStep(step, [] { return AutoPilotCfg.Autopilot; });

    console.AddLog("[Auto]AutoPilot sequence Ended");
    wdt_notsent = false;
}

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        bool ulsigdone = false;
        //UL SIGNAL
        if((AutoPilotCfg.DoFTPDL || AutoPilotCfg.DoFTPUL) && NowOn(SatIndex, NextValuablePath))
        {
            int NowBecCount = BeaconCounter;
        while (NowBecCount == BeaconCounter &&
            AutoPilotCfg.Autopilot &&
            NowOn(SatIndex, NextValuablePath))
        {
            usleep(5000); 
        }

            uint32_t initial = get_rx_bytes();
            DateTime inittime = DateTime::Now(false);
            
            while(AutoPilotCfg.Autopilot && !ulsigdone)
            {
                if((inittime - DateTime::Now()).TotalSeconds() > 30)
                    console.AddLog("[Auto]No Beacon for 30 seconds. Try Next Task.");
                console.AddLog("[Auto]Try to Send UL Signal");
                
                DateTime BeaconStandard = DateTime::Now(false);
                for(int i = 0; i < 10; i++)
                {
                    sleep(AutoPilotCfg.SpanTime);
                    if((DateTime::Now(false) - OrbitData.Satellites[SatIndex]->_nextlos[NextValuablePath]).TotalSeconds() > 0)
                    {
                        // console.AddLog("[ERROR]##[Auto]Failed to Send UL Signal.");
                        break;
                    }
                    
                    // MIM_TLEinfo = FSWTleConverter(TrackingTLE[NowSat]);
                    packetsign* TestPacket = (packetsign *)malloc(9);
                    TestPacket->Identifier = MIM_ID;
                    TestPacket->PacketType = MIM_PT_SIGNAL;
                    TestPacket->Length = 1;
                    memset(TestPacket->Data, 0, TestPacket->Length);
                    mim::svc::SendUplink((void *)TestPacket);
                    mim::svc::WaitTx();
                }

                console.AddLog("Try to config total rx bytes.");
                int trying = 0;
                while(AutoPilotCfg.Autopilot && NowOn(SatIndex, NextValuablePath))
                {
                    if(gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK)
                    {
                        if((get_rx_bytes() - initial) % 16 != 0)
                        {
                            console.AddLog("[OK]##[Auto]UL Signal Send Done. Init : %u, Now : %u", initial, get_rx_bytes());
                            ulsigdone = true;
                            break;
                        }
                    }
                    else if (trying == 5)
                        break;
                    else
                    {
                        trying ++;
                        continue;
                    }
                        

                }
                
                if(!NowOn(SatIndex, NextValuablePath))
                {
                    console.AddLog("[ERROR]##[Auto]Failed to Send UL Signal.");
                    break;
                }
            }
        }

        

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        // //CMD
        // if(State.Autopilot && State.DoCMD && NowOn(SatIndex, NextValuablePath))
        // {
        //     for(int i = 0; i < 256; i++)
        //     {
        //         if(SatCMD[i] == NULL)
        //             continue;
        //         if(State.Successed > State.CMDCount)
        //         {
        //             State.CMDCount++;
        //             continue;
        //         }
        //         while(State.Autopilot)
        //         {
        //             sleep(State.SpanTime);
                    
        //             if(SatCMD[i]->Checksum)
        //                 SatCMD[i]->GenerateChecksum();
        //             packetsign * SendingCMD = SatCMD[i]->GenerateCMDPacket();
        //             pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void *)SendingCMD);
        //             pthread_join(p_thread[4], NULL);
        //             if(State.Successed < State.CMDCount)
        //             {
        //                 State.Successed = State.CMDCount;
        //                 break;
        //             }
        //             if(!NowOn(SatIndex, NextValuablePath))
        //             {
        //                 break;
        //             }
        //         }
        //         if(!NowOn(SatIndex, NextValuablePath))
        //         {
        //             break;
        //         }

        //     }
        //     if(!NowOn(SatIndex, NextValuablePath))
        //     {
        //         console.AddLog("[ERROR]##[Auto]Stop CMD after LOS. Success : %d", State.CMDCount);
        //         State.CMDCount = 0;
        //         break;
        //     }
        // }

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        //FTP DL
        //Temporary condition : max_el > 30.0deg
        while(AutoPilotCfg.Autopilot && AutoPilotCfg.DoFTPDL && NowOn(SatIndex, NextValuablePath))
        {
            if(FtpShared.ftp_mode)
                {usleep(10000); 
                    continue;
                }
            sleep(AutoPilotCfg.SpanTime);
            if(!NowOn(SatIndex, NextValuablePath))
            {
                console.AddLog("[Auto]Stop FTP Download after LOS.");
                break;
            }
            if(FtpShared.ftp_version == 1)

                console.AddLog("[ERROR]##ftp V1 is deprecated. Use V2.");   
                // pthread_create(&p_thread[8], NULL, ftp_downlink_force, &State.ftplistup[NowFTP]);
            else
            {
                if (!ftp_start_worker(mim::svc::ThreadHandle(mim::svc::SLOT_FTP), ftp_downlink_onorbit, &FtpShared.ftplistup[NowFTP], "download")) {
                    console.AddLog("[Auto]FTP download launch blocked. Stop automatic FTP retries for this pass.");
                    break;
                }
            }
            
        }

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        //FTP UL
        //Temporary condition : max_el > 30.0deg
        while(AutoPilotCfg.Autopilot && AutoPilotCfg.DoFTPUL && NowOn(SatIndex, NextValuablePath))
        {
            if(FtpShared.ftp_mode)
                continue;
            sleep(AutoPilotCfg.SpanTime);
            if(!NowOn(SatIndex, NextValuablePath))
            {
                console.AddLog("[Auto]Stop FTP Upload after LOS.");
                break;
            }
            if(FtpShared.ftp_version == 1)
                console.AddLog("[ERROR]##ftp V1 is deprecated. Use V2.");   
                // pthread_create(&p_thread[8], NULL, ftp_uplink_force, &State.ftplistup[NowFTP]);
            else
            {
                if (!ftp_start_worker(mim::svc::ThreadHandle(mim::svc::SLOT_FTP), ftp_uplink_onorbit, &FtpShared.ftplistup[NowFTP], "upload")) {
                    console.AddLog("[Auto]FTP upload launch blocked. Stop automatic FTP retries for this pass.");
                    break;
                }
            }
            
            
        }
        

        if(!AutoPilotCfg.Autopilot)
        {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User.");
            return 0;
        }

        //Wait Until LOS and return multhreads.
        mim::svc::JoinThread(mim::svc::SLOT_AMP_TIMER);
        mim::svc::JoinThread(mim::svc::SLOT_FTP);
        mim::svc::WaitTx();

        while(AutoPilotCfg.Autopilot && NowOn(SatIndex, NextValuablePath))
            usleep(20000);
        LinkFlags.Doppler = false;
        mim::svc::JoinThread(mim::svc::SLOT_DOPPLER);
        console.AddLog("[OK]##[Auto]Stop Doppler Correction.");

        RotatorFlags.Engage = false;
        LinkFlags.Doppler = false;
        mim::svc::JoinThread(mim::svc::SLOT_ROTATOR_CONTROL);
        console.AddLog("[OK]##[Auto]Finish Engaging.");
        
        RotatorSetTo(0, 0);
        console.AddLog("[Auto]Beacon Success : %d, Ping Success : %d CMD Success : %d", BeaconCounter, PingCounter, AutoPilotCfg.CMDCount);
        BeaconCounter = 0;
        PingCounter = 0;


        //TLE update시 주석 해제 필수
        console.AddLog("[Auto]Update TLE...");
        for(int i = 0; i < SAT_MAX_NUM; i++)
        {
            if (OrbitData.Satellites[i]) OrbitData.Satellites[i]->use = false;
        }
        if(DownloadTLE(OrbitData.tleinfolistup[NowTLE]->remote, OrbitData.tleinfolistup[NowTLE]->local))
        {
            OrbitData.NowTracking = false;
            
            console.AddLog("[OK]##[Auto]TLE \"%s\" Download Completed.", OrbitData.tleinfolistup[NowTLE]->label);

            // pthread_create(&p_thread[15], NULL, &SatelliteInitialize,  &sgp4check);
            // SatelliteInitialize(NULL);
            
        }
        else
        {
            console.AddLog("[ERROR]##[Auto]TLE Download failed. Please check again.");
            return -1;
        }
        // pthread_join(p_thread[15], NULL);
        printf("Updated TLE.\n");
        sleep(3);

        //AMP OFF
        pid_t pid_off = mim::svc::AmpReady("ampoff") ? fork() : -2; // -2: 앰프 미연결 (로그는 AmpReady 가 남김)

        if (pid_off == -2) {
        }
        else if (pid_off == -1) {
            console.AddLog("[ERROR]##Failed to fork process");
        }
        else if (pid_off == 0) {
            // Child process
            execl("../amp/ampoff", "ampoff", NULL);
            // exec 실패 시 자식이 GS 복제본으로 계속 돌면 X 연결이 깨져 본체까지 죽는다
            perror("execl ../amp/ampoff");
            _exit(127);
        }
        else
        {
            console.AddLog("[OK]##Forked Safe Amplifier Process. (OFF)");
        }
        LinkFlags.AMPON = false;



        // TLE  업데이트시 주석 해ㅈ[ ㅍ;ㄹ수]
        csp_transaction(CSP_PRIO_HIGH, setup->gs100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0;
        console.AddLog("Reset GS100 watchdog.");
        printf("Reset GS100 Watchdog.\n");
        sleep(3);
        int tlepopupindex = 0;
        
        printf("Search Samename Satellite.\n");

        OrbitData.tleallindex = SAT_MAX_NUM;
        if((OrbitData.tleallindex = ReadTLELines_Errorhandling(OrbitData.tleinfolistup[NowTLE]->local, NowTrackingNameBuffer,false, false)) <= 0)
            console.AddLog("[ERROR]##Invalid type of TLE File. Please check again.");
        AppUi.Display_TLE = true;
       
        sleep(3);
        if (NowTracking < 0 || NowTracking >= SAT_MAX_NUM || !OrbitData.Satellites[NowTracking]) {
            console.AddLog("[ERROR]##[Auto]NowTracking invalid after TLE reload. idx=%d", NowTracking);
            return -2;
        }



        OrbitData.Satellites[NowTracking]->use = true;
        if(OrbitData.Satellites[NowTracking]->cal == false)
            OrbitData.Satellites[NowTracking]->Refresh(OrbitData.Satellites[NowTracking]->tle, OrbitData.Satellites[NowTracking]->obs, true, true);
        printf("Updated Propagator.\n");
        sleep(3);
        OrbitData.NowTracking = true;
        OrbitData.ModelRefreshRequired = true;
        printf("Model Refresh.\n");
        sleep(3);

        SetNowTracking(NowTracking);
        SatIndex = GetNowTracking();
        if (SatIndex < 0 || SatIndex >= SAT_MAX_NUM || !OrbitData.Satellites[SatIndex]) return -2;
        // BEFORE calling PathGenerator()
        if (!AutoPilotCfg.Autopilot) {
            console.AddLog("[OK]##[Auto]Autopilot Stopped by User (before PathGenerator).");
            return 0;
        }

        int trk = GetNowTracking();
        if (trk < 0 || trk >= SAT_MAX_NUM || OrbitData.Satellites[trk] == NULL) {
            console.AddLog("[ERROR]##[Auto]Invalid tracking index before PathGenerator. trk=%d ptr=%p",
                        trk, (trk >= 0 && trk < SAT_MAX_NUM) ? OrbitData.Satellites[trk] : NULL);
            return -2;
        }

        PathGenerator(trk);




        // PathGenerator(GetNowTracking());
        AppUi.Display_load = false;
        AppUi.Display_TLE = false;
        printf("Finish Autopilot.\n");
    }
    return 0;
}

int TLE_Autoupdate_Test()
{
    int NowTracking = GetNowTracking();
    char NowTrackingNameBuffer[32];
    printf("[Auto]Update TLE...\n");
    sprintf(NowTrackingNameBuffer, OrbitData.Satellites[NowTracking]->Name());
    for(int i = 0; i < SAT_MAX_NUM; i++)
    {
        if (OrbitData.Satellites[i]) OrbitData.Satellites[i]->use = false;
    }
    if(DownloadTLE(OrbitData.tleinfolistup[NowTLE]->remote, OrbitData.tleinfolistup[NowTLE]->local))
    {
        OrbitData.NowTracking = false;
        
        printf("[OK]##[Auto]TLE \"%s\" Download Completed.\n", OrbitData.tleinfolistup[NowTLE]->label);

        // pthread_create(&p_thread[15], NULL, &SatelliteInitialize,  &sgp4check);
        // SatelliteInitialize(NULL);
        
    }
    else
    {
        printf("[ERROR]##[Auto]TLE Download failed. Please check again.\n");
        return -1;
    }
    // pthread_join(p_thread[15], NULL);
    printf("Updated TLE.\n");
    sleep(3);

    printf("Skip GS100 Watchdog");
    // csp_transaction(CSP_PRIO_HIGH, setup->gs100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0;
    // console.AddLog("Reset GS100 watchdog.");
    // printf("Reset GS100 Watchdog.\n");
    // sleep(3);
    int tlepopupindex = 0;
    
    printf("Search Samename Satellite.\n");

    OrbitData.tleallindex = 32768;
    if((OrbitData.tleallindex = ReadTLELines_Errorhandling(OrbitData.tleinfolistup[NowTLE]->local, NowTrackingNameBuffer,false, false)) <= 0)
        printf("[ERROR]##Invalid type of TLE File. Please check again.\n");
    AppUi.Display_TLE = true;
    
    sleep(3);

    if (NowTracking < 0 || NowTracking >= SAT_MAX_NUM || !OrbitData.Satellites[NowTracking]) {
    console.AddLog("[ERROR]##[Auto]NowTracking invalid after TLE reload. idx=%d", NowTracking);
    return -2;
    }   

    OrbitData.Satellites[NowTracking]->use = true;
    if(OrbitData.Satellites[NowTracking]->cal == false)
        OrbitData.Satellites[NowTracking]->Refresh(OrbitData.Satellites[NowTracking]->tle, OrbitData.Satellites[NowTracking]->obs, true, true);
    printf("Updated Propagator.\n");
    sleep(3);
    OrbitData.NowTracking = true;
    OrbitData.ModelRefreshRequired = true;
    printf("Model Refresh.\n");
    sleep(3);
    SetNowTracking(NowTracking);
    PathGenerator(GetNowTracking());
    AppUi.Display_load = false;
    AppUi.Display_TLE = false;
    printf("Finish Autopilot.\n");
    return 0;
}



