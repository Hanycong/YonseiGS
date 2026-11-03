// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>
// #include <inttypes.h>
// #include <pthread.h>
// #include <signal.h>
// #include <unistd.h>
// #include <time.h>
// #include <sys/stat.h>
// #include <sys/time.h>
// #include <ctime>
// #include <cstdlib>
// #include <iostream>

// /* CSP */
// #include <csp/csp.h>
// #include <csp/csp_error.h>
// #include <csp/interfaces/csp_if_kiss.h>
// #include <csp/drivers/usart.h>
// #include <csp_port.h>
// #include <csp/arch/csp_queue.h>
// #include <csp/arch/csp_semaphore.h>
// #include <csp/arch/csp_malloc.h>
// #include <csp/arch/csp_time.h>
// #include <csp_conn.h>
// #include <csp_io.h>
// #include <csp/csp_endian.h>
// #include <csp/delay.h>

// /* Drivers / Util */
// #include <gs/util/log.h>
// #include <gs/ftp/client.h>
// #include <gs/ftp/types.h>
// #include <gs/util/crc32.h>
// #include <gs/util/string.h>
// #include <gs/util/crc32.h>
// #include <gs/util/clock.h>
// #include <gs/util/vmem.h>

// #include "YonseiGS_config.h"
// #include "YonseiGS_csp.h"
// #include "YonseiGS_coms.h"
// #include "YonseiGS_imgui.h"
// #include "YonseiGS_ftp.h"

// extern Console console;
// extern StateCheckUnit State;
// extern pthread_t p_thread[16];
// extern pthread_mutex_t conn_lock;
// extern Setup * setup;

// static unsigned int ftp_chunk_size = 180;
// static unsigned int ftp_backend = 3; // Use file backend as standard
// static const char * const packet_missing = "-";
// static const char * const packet_ok = "+";

// char flistbuffer[16384];


// static void ftp_info_print_callback(const gs_ftp_info_t * info)
// {

//     switch (info->type) {
//         case GS_FTP_INFO_COMPLETED:
//             {
//                 const uint32_t complete = info->u.completed.completed_chunks;
//                 const uint32_t total    = info->u.completed.total_chunks;
//                 printftp("Transfer Status: %" PRIu32 " of %" PRIu32 " (%.1f%%)\r\n",
//                         complete, total, gs_ftp_percent_completed(complete, total));
//             }
//             break;
//         case GS_FTP_INFO_FILE:
//             {
//                 console.AddLog("[OK]##Received FTP file Spec.");
//                 printftp("Received FTP File Spec. Filesize : %u, CRC : %u.", info->u.file.size, info->u.file.crc);
//             }
//             break;
//         case GS_FTP_INFO_CRC:
//             {
//                 printftp("FTP Transaction Completed.");
//                 console.AddLog("Calculate FTP CRC %s. Remote : %u, Local : %u.", (info->u.crc.remote == info->u.crc.local) ? "OK" : "ERROR", info->u.crc.remote, info->u.crc.local);
//             }
//             break;
//         case GS_FTP_INFO_PROGRESS:
//             printftp("Progress : %u/%u", info->u.progress.current_chunk, info->u.progress.total_chunks);
//             // progress_bar(info->u.progress.current_chunk, info->u.progress.total_chunks, info->u.progress.chunk_size,
//             //              info->user_data);
//             break;
//     }
// }


// int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t * listent, void * data)
// {
//     char pathbuf[256];
//     time_t tmtime = time(0);
//     struct tm * local = localtime(&tmtime);
//     sprintf(pathbuf, "./data/listup/Listup--%04d-%02d-%02d-%02d-", local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour);
//     strcat(pathbuf, State.gslistup->fpathbuf);
//     State.gslistup->fd = fopen(pathbuf, "wb");

//     memset(State.gslistup->flistbuf, 0, sizeof(State.gslistup->flistbuf));
//     if(listent->type == GS_FTP_LIST_FILE)
//         sprintf(State.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "F", listent->size, listent->path);
//     else
//         sprintf(State.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "D", listent->size, listent->path);
//     fprintf(State.gslistup->fd, State.gslistup->flistbuf);
//     strcat(State.gslistup->fdispbuf, State.gslistup->flistbuf);
//     fclose(State.gslistup->fd);
// }

// void * ftp_downlink_onorbit(void * param){
// #define CSP_USE_RDP
//     // ftp_avail();
//     bool dlstate = State.downlink_mode;
//     //This funcion must be on p_thread[4]
//     if((dlstate))
//         State.downlink_mode = false;
//     while(!State.uplink_mode)
//         continue;
//     while(!State.RotatorReadReady)
//         continue;
    
//     //host : OBC adress
//     //Port : 9
//     ftpinfo * FTP = (ftpinfo *) param;
//     // gs_ftp_info_callback_t = ftp_callback;
//     printftp("Start FTP Downlink.");
//     State.downlink_mode = false;
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = 30000; //default timeout value
//     ftp_config.chunk_size = State.chunk_sz; //default chunk size value
//     State.ftp_mode = true;
//     int status = (int)gs_ftp_download(&ftp_config, FTP->local_path, FTP->remote_path, ftp_info_print_callback, NULL);
//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to complete ftp_download. Retcode : %d", status);\
// 	}
//     else
//     {
//         console.AddLog("[OK]##End FTP download. Retcode : %d", status);\
//     }
//     // ftp_abort();
//     State.ftp_mode = false;
//     if(dlstate)
//     {
//         State.downlink_mode = dlstate;
//     }
// #undef CSP_USE_RDP
// 	return NULL;
// }

// void * ftp_uplink_onorbit(void * param){
// #define CSP_USE_RDP
//     // ftp_avail();
//     bool dlstate = State.downlink_mode;
//     //This funcion must be on p_thread[4]
//     if((dlstate))
//         State.downlink_mode = false;
//     while(!State.uplink_mode)
//         continue;
//     while(!State.RotatorReadReady)
//         continue;
//     //host : OBC adress
//     //Port : 15
//     ftpinfo * FTP = (ftpinfo *) param;
//     State.downlink_mode = false;
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = 30000; //default timeout value
//     ftp_config.chunk_size = State.chunk_sz; //default chunk size value
//     State.ftp_mode = true;
//     int status = 0;
//     status = (int)gs_ftp_upload(&ftp_config, FTP->local_path, FTP->remote_path, ftp_info_print_callback, NULL);
//     printftp("FTP task DONE.");
    
//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to complete ftp_upload. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("[OK]##End FTP upload. Retcode : %d", status);
//     }
//     // ftp_abort();
//     State.ftp_mode = false;
//     if(dlstate)
//     {
//         State.downlink_mode = dlstate;
//     }
// #undef CSP_USE_RDP
// 	return NULL;
// }


// void * ftp_list_onorbit(void *){
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_list(&ftp_config, State.gslistup->fpathbuf, ftp_list_callback, NULL);
    
//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to call FTP list. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("Call FTP list. Retcode : %d", status);
//         printf("%s", flistbuffer);
//     }
// }

// void * ftp_move_onorbit(void *){
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_move(&ftp_config, State.gsmove->from, State.gsmove->to);

//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to FTP move. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("[OK]##FTP move done. Retcode : %d", status);
//     }
// }

// void * ftp_remove_onorbit(void *){
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_remove(&ftp_config, State.gsremove->path);

//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to FTP remove. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("[OK]##FTP remove done. Retcode : %d", status);
//     }
// }

// void * ftp_copy_onorbit(void *){
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_copy(&ftp_config, State.gscopy->from, State.gscopy->to);

//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to FTP copy. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("FTP copy done. Retcode : %d", status);
//     }
// }

// void * ftp_mkdir_onorbit(void *){
//     uint32_t mode = 0;
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_mkdir(&ftp_config, State.gsmkdir->path, mode);

//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to FTP make directory. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("[OK]##FTP make directory done. Retcode : %d", status);
//     }
// }

// void * ftp_rmdir_onorbit(void *){
//     gs_ftp_settings_t ftp_config;
//     ftp_config.mode = GS_FTP_MODE_STANDARD;
//     ftp_config.host = setup->obc_node;
//     ftp_config.port = FTPRDP_PORT;
//     ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
//     ftp_config.chunk_size = 200; //default chunk size value
//     int status = (int)gs_ftp_rmdir(&ftp_config, State.gsrmdir->path);

//     if (status != 0) {
// 		console.AddLog("[ERROR]##Fail to FTP remove directory. Retcode : %d", status);
// 	}
//     else
//     {
//         console.AddLog("[OK]##FTP remove directory done. Retcode : %d", status);
//     }
// }




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <atomic>

/* CSP */
#include <csp/csp.h>
#include <csp/csp_error.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/drivers/usart.h>
#include <csp_port.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_time.h>
#include <csp_conn.h>
#include <csp_io.h>
#include <csp/csp_endian.h>
#include <csp/delay.h>

/* Drivers / Util */
#include <gs/util/log.h>
#include <gs/ftp/client.h>
#include <gs/ftp/types.h>
#include <gs/util/crc32.h>
#include <gs/util/string.h>
#include <gs/util/crc32.h>
#include <gs/util/clock.h>
#include <gs/util/vmem.h>
#include <ryu_ftp_client_adapter.h>     // Ryu 엔진 (lib/libftp_client_ryu), gs_ftp_process

#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_ftp.h"
#include "ftp_transfer.h"
#include "svc_thread.h"

extern Console console;
extern StateCheckUnit State;
extern pthread_mutex_t conn_lock;
extern Setup * setup;

void miman_begin_ftp_rdp_profile(uint32_t ftp_timeout_ms);
void miman_end_ftp_rdp_profile(void);

static unsigned int ftp_chunk_size = 200;
static unsigned int ftp_backend = 3; // Use file backend as standard
static const char * const packet_missing = "-";
static const char * const packet_ok = "+";
static const unsigned int ftp_chunk_size_limit = 200;
static const unsigned int ftp_transfer_timeout_ms = 120000;
static const unsigned int ftp_download_timeout_ms = 30000;
static const unsigned int ftp_force_timeout_ms = 5000;      // V1 force (누리호 5차 GS miman_ftpfcd.cpp)
static const time_t ftp_failure_cooldown_sec = 30;

/*
 * 엔진별 FTP 서비스 포트 (누리호 5차 GS)
 *   V1 force : CSP 포트 15 (FTPFCD_PORT)
 *   V2       : CSP 포트 9  (FTPRDP_PORT)
 *   Ryu      : CSP 포트 19
 */
static const uint8_t FTP_RDP_PORT_RYU = 19;

static pthread_mutex_t ftp_launch_lock = PTHREAD_MUTEX_INITIALIZER;
static bool ftp_launch_pending = false;
static time_t ftp_block_until = 0;
static int ftp_last_status = 0;

// 시작 요청 ~ 워커 끝 (누리호 5차 GS gs_ftp_busy). gs_ftp_process 는 Ryu 라이브러리(ftpnew_client.c)에 있다
static std::atomic<bool> ftp_busy(false);
// SLOT_FTP_STOP 의 abort 유지 스레드가 돌고 있는지
static std::atomic<bool> ftp_stop_keeper_running(false);

char flistbuffer[16384];

// ---------------------------------------------------------------- Start / Stop

static void miman_ftp_process_begin(void)
{
    ftp_busy = true;
    gs_ftp_process = true;
}

static void miman_ftp_process_end(void)
{
    gs_ftp_process = false;
    ftp_busy = false;
}

bool miman_ftp_busy()
{
    return ftp_busy;
}

bool miman_ftp_stopping()
{
    return ftp_busy && !gs_ftp_process;
}

// V2 libftp_client 의 ftp_data() 는 시작할 때 ftpavailable 을 true 로 되돌린다.
// Stop 뒤 워커가 끝날 때까지 abort 플래그를 다시 세운다 (누리호 5차 GS 는 GUI 프레임마다 했다:
// 여기서는 FTP 창이 안 보이거나 AutoPilot/종료에서 멈출 때도 먹도록 스레드로 한다)
static void * ftp_stop_keeper(void *)
{
    while (ftp_busy && !gs_ftp_process)
    {
        ftp_abort();
        usleep(50000);
    }
    ftp_stop_keeper_running = false;
    return NULL;
}

void miman_ftp_process_stop(void)
{
    if (!ftp_busy)
        return;
    gs_ftp_process = false;
    ftp_abort();    /* V1/V2 libftp_client checks ftpavailable per chunk */
    if (!ftp_stop_keeper_running.exchange(true))
    {
        mim::svc::JoinThread(mim::svc::SLOT_FTP_STOP);     // 앞 keeper 는 이미 끝났다 (running 이 false 였음)
        if (mim::svc::StartThread(mim::svc::SLOT_FTP_STOP, ftp_stop_keeper, NULL) != 0)
            ftp_stop_keeper_running = false;
    }
}

static unsigned int ftp_chunk_size_clamped(unsigned int requested)
{
    if (requested == 0) {
        return ftp_chunk_size_limit;
    }
    if (requested > ftp_chunk_size_limit) {
        return ftp_chunk_size_limit;
    }
    return requested;
}

static unsigned int ftp_cooldown_remaining_locked(time_t now)
{
    if (ftp_block_until <= now) {
        return 0;
    }
    return (unsigned int) (ftp_block_until - now);
}

static void ftp_worker_begin(void)
{
    pthread_mutex_lock(&ftp_launch_lock);
    ftp_launch_pending = false;
    FtpShared.ftp_mode = true;
    pthread_mutex_unlock(&ftp_launch_lock);
}

// 워커의 마지막 동작 (이 뒤로 공유 상태를 건드리지 않는다: busy 가 내려가면 다음 워커가 시작할 수 있다)
static void ftp_worker_finish(int status, bool user_stopped = false)
{
    pthread_mutex_lock(&ftp_launch_lock);
    ftp_last_status = status;
    ftp_launch_pending = false;
    FtpShared.ftp_mode = false;
    if (status != 0 && !user_stopped) {
        ftp_block_until = time(NULL) + ftp_failure_cooldown_sec;
    } else {
        ftp_block_until = 0;    // 성공 또는 사용자가 멈춤: 바로 다시(이어서) 시작할 수 있다
    }
    pthread_mutex_unlock(&ftp_launch_lock);
    miman_ftp_process_end();
}

bool ftp_start_worker(void *(*start_routine)(void *), void *arg, const char *label)
{
    time_t now = time(NULL);

    pthread_mutex_lock(&ftp_launch_lock);
    if (FtpShared.ftp_mode || ftp_launch_pending || ftp_busy) {
        pthread_mutex_unlock(&ftp_launch_lock);
        console.AddLog("[ERROR]##FTP %s blocked: another FTP worker is active.", label);
        return false;
    }
    if (ftp_cooldown_remaining_locked(now) > 0) {
        unsigned int remaining = ftp_cooldown_remaining_locked(now);
        pthread_mutex_unlock(&ftp_launch_lock);
        console.AddLog("[ERROR]##FTP %s blocked: cooldown active for %u sec after failure.", label, remaining);
        return false;
    }
    ftp_launch_pending = true;
    miman_ftp_process_begin();
    pthread_mutex_unlock(&ftp_launch_lock);

    // 앞 워커는 끝났다 (busy 가 내려간 뒤라 join 이 바로 돌아온다). JoinThread 는 한 번만 join 한다
    mim::svc::JoinThread(mim::svc::SLOT_FTP);
    int create_status = mim::svc::StartThread(mim::svc::SLOT_FTP, start_routine, arg);
    if (create_status != 0) {
        pthread_mutex_lock(&ftp_launch_lock);
        ftp_launch_pending = false;
        pthread_mutex_unlock(&ftp_launch_lock);
        miman_ftp_process_end();
        console.AddLog("[ERROR]##Failed to launch FTP %s worker. pthread_create ret=%d", label, create_status);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------- 진행 표시 (누리호 5차 GS miman_ftpfcd.cpp ftp_callback)

static void ftp_callback(const gs_ftp_info_t * info)
{
    if (info == NULL) {
        console.AddLog("[ERROR]##FTP callback invoked with NULL info.");
        return;
    }
    switch(info->type)
    {
        case GS_FTP_INFO_DL_FILE : {
            console.AddLog("[OK]##Received FTP Download Spec.");
            printftp("Received FTP Download Spec. Filesize : %u, CRC : %u.", info->u.file.size, info->u.file.crc);
            break;
        }
        case GS_FTP_INFO_UL_FILE : {
            console.AddLog("[OK]##Received FTP Upload Spec.");
            printftp("Received FTP Upload File Spec. Filesize : %u, CRC : %u.", info->u.file.size, info->u.file.crc);
            break;
        }
        case GS_FTP_INFO_CRC : {
            printftp("FTP Transaction Completed.");
            console.AddLog("Calculate FTP CRC %s. Remote : %u, Local : %u.", (info->u.crc.remote == info->u.crc.local) ? "OK" : "ERROR", info->u.crc.remote, info->u.crc.local);
            break;
        }
        case GS_FTP_INFO_DL_COMPLETED : {
            console.AddLog("[OK]##Received FTP Status Request.");
            // 0 이 아니면 이전에 멈춘 전송을 이어받는다 (resume)
            if(info->u.completed.completed_chunks != 0)
                printftp("Restart FTP Downlink from chunk : %u, to total : %u", info->u.completed.completed_chunks, info->u.completed.total_chunks);
            else
                printftp("Start FTP Downlink. Total Chunk Size : %u", info->u.completed.total_chunks);
            break;
        }
        case GS_FTP_INFO_UL_COMPLETED : {
            console.AddLog("[OK]##Received FTP Status Reply.");
            if(info->u.completed.completed_chunks != 0)
                printftp("Restart FTP Uplink from chunk : %u, to total : %u", info->u.completed.completed_chunks, info->u.completed.total_chunks);
            else
                printftp("Start FTP Uplink. Total Chunk Size : %u", info->u.completed.total_chunks);
            break;
        }
        case GS_FTP_INFO_DL_PROGRESS : {
            printftp("Download : %u/%u", info->u.progress.current_chunk, info->u.progress.total_chunks);
            break;
        }
        case GS_FTP_INFO_UL_PROGRESS : {
            printftp("Upload : %u/%u", info->u.progress.current_chunk, info->u.progress.total_chunks);
            break;
        }
        default : {
            console.AddLog("[ERROR]##Invalid Call to FTP Callback Function.");
        }
    }
}

// ---------------------------------------------------------------- 엔진 선택 + 워커 (누리호 5차 GS miman_ftprdp.cpp)

enum miman_ftp_engine_t {
    MIMAN_FTP_ENGINE_FORCE = 0,     // V1
    MIMAN_FTP_ENGINE_OLD   = 1,     // V2
    MIMAN_FTP_ENGINE_RYU   = 2,     // V3
};

enum miman_ftp_direction_t {
    MIMAN_FTP_DIRECTION_DOWNLOAD = 0,
    MIMAN_FTP_DIRECTION_UPLOAD = 1,
};

static const uint32_t MIMAN_FTP_WORKER_ARG_MAGIC = 0x46545052u;     // "FTPR"

struct miman_ftp_worker_arg {
    uint32_t magic;
    uint32_t magic_inverse;
    ftpinfo paths;
};

struct miman_ftp_worker_copy_t {
    ftpinfo paths;
    gs_ftp_settings_t settings;
    miman_ftp_engine_t engine;
    miman_ftp_direction_t direction;
    int iteration;                  // V1 force 반복 횟수
    uint32_t transfer_id;
};

static pthread_mutex_t ftp_transfer_status_lock = PTHREAD_MUTEX_INITIALIZER;
static uint32_t ftp_transfer_id_next = 1;
static miman_ftp_transfer_status_t ftp_transfer_last_status;

static const char * ftp_engine_name(miman_ftp_engine_t engine)
{
    switch (engine) {
        case MIMAN_FTP_ENGINE_FORCE: return "V1";
        case MIMAN_FTP_ENGINE_RYU:   return "RYU";
        case MIMAN_FTP_ENGINE_OLD:
        default:                     return "V2";
    }
}

static const char * ftp_direction_name(miman_ftp_direction_t direction)
{
    return (direction == MIMAN_FTP_DIRECTION_UPLOAD) ? "UPLOAD" : "DOWNLOAD";
}

static uint8_t ftp_port_for_engine(miman_ftp_engine_t engine)
{
    switch (engine) {
        case MIMAN_FTP_ENGINE_FORCE: return FTPFCD_PORT;
        case MIMAN_FTP_ENGINE_RYU:   return FTP_RDP_PORT_RYU;
        case MIMAN_FTP_ENGINE_OLD:
        default:                     return FTPRDP_PORT;
    }
}

static uint32_t ftp_timeout_for(miman_ftp_engine_t engine, miman_ftp_direction_t direction)
{
    if (engine == MIMAN_FTP_ENGINE_FORCE)
        return ftp_force_timeout_ms;
    return (direction == MIMAN_FTP_DIRECTION_UPLOAD) ? ftp_transfer_timeout_ms : ftp_download_timeout_ms;
}

static uint32_t ftp_next_transfer_id(void)
{
    uint32_t transfer_id;

    pthread_mutex_lock(&ftp_transfer_status_lock);
    transfer_id = ftp_transfer_id_next++;
    if (ftp_transfer_id_next == 0) {
        ftp_transfer_id_next = 1;
    }
    pthread_mutex_unlock(&ftp_transfer_status_lock);

    return transfer_id;
}

static void ftp_copy_paths(ftpinfo * destination, const ftpinfo * source)
{
    memcpy(destination, source, sizeof(*destination));
    destination->name[sizeof(destination->name) - 1] = '\0';
    destination->local_path[sizeof(destination->local_path) - 1] = '\0';
    destination->remote_path[sizeof(destination->remote_path) - 1] = '\0';
}

miman_ftp_worker_arg_t * miman_ftp_create_worker_arg(const char * local_path,
                                                     const char * remote_path)
{
    miman_ftp_worker_arg_t * arg =
        (miman_ftp_worker_arg_t *) calloc(1, sizeof(*arg));
    if (arg == NULL) {
        return NULL;
    }

    arg->magic = MIMAN_FTP_WORKER_ARG_MAGIC;
    arg->magic_inverse = ~MIMAN_FTP_WORKER_ARG_MAGIC;
    if (local_path != NULL) {
        snprintf(arg->paths.local_path, sizeof(arg->paths.local_path), "%s",
                 local_path);
    }
    if (remote_path != NULL) {
        snprintf(arg->paths.remote_path, sizeof(arg->paths.remote_path), "%s",
                 remote_path);
    }

    return arg;
}

void miman_ftp_destroy_worker_arg(miman_ftp_worker_arg_t * arg)
{
    if (arg == NULL) {
        return;
    }

    arg->magic = 0;
    arg->magic_inverse = 0;
    free(arg);
}

static bool ftp_is_owned_worker_arg(const void * param)
{
    const miman_ftp_worker_arg_t * arg =
        (const miman_ftp_worker_arg_t *) param;

    return (arg->magic == MIMAN_FTP_WORKER_ARG_MAGIC) &&
           (arg->magic_inverse == ~MIMAN_FTP_WORKER_ARG_MAGIC);
}

static void ftp_store_transfer_status(const miman_ftp_worker_copy_t * job,
                                      int return_code,
                                      bool cleanup_done,
                                      bool user_stopped)
{
    pthread_mutex_lock(&ftp_transfer_status_lock);
    ftp_transfer_last_status.transfer_id = job->transfer_id;
    ftp_transfer_last_status.return_code = return_code;
    ftp_transfer_last_status.cleanup_done = cleanup_done ? 1 : 0;
    ftp_transfer_last_status.user_stopped = user_stopped ? 1 : 0;
    snprintf(ftp_transfer_last_status.engine,
             sizeof(ftp_transfer_last_status.engine), "%s",
             ftp_engine_name(job->engine));
    snprintf(ftp_transfer_last_status.direction,
             sizeof(ftp_transfer_last_status.direction), "%s",
             ftp_direction_name(job->direction));
    snprintf(ftp_transfer_last_status.local_path,
             sizeof(ftp_transfer_last_status.local_path), "%s",
             job->paths.local_path);
    snprintf(ftp_transfer_last_status.remote_path,
             sizeof(ftp_transfer_last_status.remote_path), "%s",
             job->paths.remote_path);
    pthread_mutex_unlock(&ftp_transfer_status_lock);
}

int miman_ftp_get_last_transfer_status(miman_ftp_transfer_status_t * status)
{
    if (status == NULL) {
        return -1;
    }

    pthread_mutex_lock(&ftp_transfer_status_lock);
    *status = ftp_transfer_last_status;
    pthread_mutex_unlock(&ftp_transfer_status_lock);

    return 0;
}

// 워커 인자를 job 으로 복사한다 (소유 인자면 여기서 해제). 설정(목적지/포트/청크)도 이 순간 값으로 고정
static bool ftp_copy_worker_param(void * param, miman_ftp_worker_copy_t * job,
                                  miman_ftp_engine_t engine,
                                  miman_ftp_direction_t direction)
{
    if ((param == NULL) || (job == NULL)) {
        console.AddLog("[ERROR]##FTP %s %s missing worker argument.",
                       ftp_engine_name(engine), ftp_direction_name(direction));
        return false;
    }

    const ftpinfo * source = NULL;
    miman_ftp_worker_arg_t * owned_arg = NULL;

    if (ftp_is_owned_worker_arg(param)) {
        owned_arg = (miman_ftp_worker_arg_t *) param;
        source = &owned_arg->paths;
    }
    else {
        source = (const ftpinfo *) param;
        console.AddLog("[WARN]##FTP %s %s received legacy shared ftpinfo argument; "
                       "use miman_ftp_create_worker_arg().",
                       ftp_engine_name(engine), ftp_direction_name(direction));
    }

    memset(job, 0, sizeof(*job));
    ftp_copy_paths(&job->paths, source);
    if (owned_arg != NULL) {
        miman_ftp_destroy_worker_arg(owned_arg);
    }
    job->settings.mode = GS_FTP_MODE_STANDARD;
    job->settings.host = setup->obc_node;
    job->settings.port = ftp_port_for_engine(engine);
    job->settings.timeout = ftp_timeout_for(engine, direction);
    job->settings.chunk_size = ftp_chunk_size_clamped(FtpShared.chunk_sz);
    job->engine = engine;
    job->direction = direction;
    job->iteration = FtpShared.force_iteration;
    job->transfer_id = ftp_next_transfer_id();
    ftp_store_transfer_status(job, 0, false, false);

    return true;
}

static int ftp_run_transfer(const miman_ftp_worker_copy_t * job)
{
    const bool upload = (job->direction == MIMAN_FTP_DIRECTION_UPLOAD);
    switch (job->engine) {
        case MIMAN_FTP_ENGINE_RYU:
            if (upload)
                return (int) ryu_ftp_upload(&job->settings, job->paths.local_path, job->paths.remote_path,
                                            ftp_callback, NULL, true, 0);
            return (int) ryu_ftp_download(&job->settings, job->paths.local_path, job->paths.remote_path,
                                          ftp_callback, NULL, true);
        case MIMAN_FTP_ENGINE_FORCE:
            if (upload)
                return (int) gs_ftp_upload_force(&job->settings, job->paths.local_path, job->paths.remote_path,
                                                 ftp_callback, NULL, job->iteration);
            return (int) gs_ftp_download_force(&job->settings, job->paths.local_path, job->paths.remote_path,
                                               ftp_callback, NULL, job->iteration);
        case MIMAN_FTP_ENGINE_OLD:
        default:
            if (upload)
                return (int) gs_ftp_upload(&job->settings, job->paths.local_path, job->paths.remote_path,
                                           ftp_callback, NULL);
            return (int) gs_ftp_download(&job->settings, job->paths.local_path, job->paths.remote_path,
                                         ftp_callback, NULL);
    }
}

static void * ftp_transfer_onorbit(void * param,
                                   miman_ftp_engine_t engine,
                                   miman_ftp_direction_t direction)
{
    ftp_worker_begin();
    const bool libftp = (engine != MIMAN_FTP_ENGINE_RYU);   // V1/V2: libftp_client 의 ftpavailable 플래그
    if (libftp) {
        ftp_avail();
        /* Stop pressed before this thread got here: keep the abort flag. */
        if (!gs_ftp_process)
            ftp_abort();
    }

    miman_ftp_worker_copy_t job;
    if (!ftp_copy_worker_param(param, &job, engine, direction)) {
        ftp_worker_finish(GS_ERROR_ARG);
        return NULL;
    }

    bool dlstate = LinkFlags.downlink_mode;
    if((dlstate))
        LinkFlags.downlink_mode = false;
    // TX 모드가 될 때까지 (Stop 으로 빠져나올 수 있다). 누리호 5차 GS 와 같이 로테이터 읽기는 기다리지 않는다
    while(!LinkFlags.uplink_mode && gs_ftp_process)
        usleep(10000);

    if (!gs_ftp_process) {
        console.AddLog("[STOP]##FTP %s %s id=%u stopped by user before start.",
                       ftp_engine_name(engine), ftp_direction_name(direction), job.transfer_id);
        ftp_store_transfer_status(&job, GS_ERROR_BREAK, true, true);
        if(dlstate)
            LinkFlags.downlink_mode = dlstate;
        ftp_worker_finish(GS_ERROR_BREAK, true);
        return NULL;
    }

    printftp("Start FTP %s %s id=%u.", ftp_engine_name(engine), ftp_direction_name(direction), job.transfer_id);
    LinkFlags.downlink_mode = false;
    console.AddLog("[FTP]##%s %s id=%u config: host=%u port=%u ftp_timeout=%u ms chunk=%u",
                   ftp_engine_name(engine), ftp_direction_name(direction), job.transfer_id,
                   job.settings.host, job.settings.port, job.settings.timeout, job.settings.chunk_size);
    console.AddLog("[FTP]##id=%u local=%s remote=%s", job.transfer_id, job.paths.local_path, job.paths.remote_path);

    pthread_mutex_lock(&conn_lock);
    miman_begin_ftp_rdp_profile(job.settings.timeout);
    int status = ftp_run_transfer(&job);
    miman_end_ftp_rdp_profile();
    pthread_mutex_unlock(&conn_lock);
    printftp("FTP task DONE.");

    const bool user_stopped = !gs_ftp_process;
    const char * what = (direction == MIMAN_FTP_DIRECTION_UPLOAD) ? "upload" : "download";
    if (user_stopped)
        console.AddLog("[STOP]##FTP %s stopped by user. id=%u Retcode : %d", what, job.transfer_id, status);
    else if (status != 0)
        console.AddLog("[ERROR]##Fail to complete ftp_%s. id=%u Retcode : %d", what, job.transfer_id, status);
    else
        console.AddLog("[OK]##End FTP %s. id=%u Retcode : %d", what, job.transfer_id, status);

    if (libftp)
        ftp_abort();
    ftp_store_transfer_status(&job, status, true, user_stopped);
    if(dlstate)
    {
        LinkFlags.downlink_mode = dlstate;
    }
    ftp_worker_finish(status, user_stopped);
	return NULL;
}

void * ftp_downlink_onorbit(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_OLD, MIMAN_FTP_DIRECTION_DOWNLOAD);
}

void * ftp_uplink_onorbit(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_OLD, MIMAN_FTP_DIRECTION_UPLOAD);
}

void * ftp_downlink_force(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_FORCE, MIMAN_FTP_DIRECTION_DOWNLOAD);
}

void * ftp_uplink_force(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_FORCE, MIMAN_FTP_DIRECTION_UPLOAD);
}

void * ftp_ryu_downlink_onorbit(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_RYU, MIMAN_FTP_DIRECTION_DOWNLOAD);
}

void * ftp_ryu_uplink_onorbit(void * param){
    return ftp_transfer_onorbit(param, MIMAN_FTP_ENGINE_RYU, MIMAN_FTP_DIRECTION_UPLOAD);
}


int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t * listent, void * data)
{
    char pathbuf[256];
    time_t tmtime = time(0);
    struct tm * local = localtime(&tmtime);
    sprintf(pathbuf, "./data/listup/Listup--%04d-%02d-%02d-%02d-", local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour);
    strcat(pathbuf, FtpShared.gslistup->fpathbuf);
    FtpShared.gslistup->fd = fopen(pathbuf, "wb");

    memset(FtpShared.gslistup->flistbuf, 0, sizeof(FtpShared.gslistup->flistbuf));
    if(listent->type == GS_FTP_LIST_FILE)
        sprintf(FtpShared.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "F", listent->size, listent->path);
    else
        sprintf(FtpShared.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "D", listent->size, listent->path);
    fprintf(FtpShared.gslistup->fd, FtpShared.gslistup->flistbuf);
    strcat(FtpShared.gslistup->fdispbuf, FtpShared.gslistup->flistbuf);
    fclose(FtpShared.gslistup->fd);
    return 0;
}

void * ftp_list_onorbit(void *){
    ftp_worker_begin();
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_list(&ftp_config, FtpShared.gslistup->fpathbuf, ftp_list_callback, NULL);
    pthread_mutex_unlock(&conn_lock);
    
    if (status != 0) {
		console.AddLog("[ERROR]##Fail to call FTP list. Retcode : %d", status);
	}
    else
    {
        console.AddLog("Call FTP list. Retcode : %d", status);
        printf("%s", flistbuffer);
    }
    ftp_worker_finish(status);
    return NULL;
}

void * ftp_move_onorbit(void *){
    ftp_worker_begin();
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_move(&ftp_config, FtpShared.gsmove->from, FtpShared.gsmove->to);
    pthread_mutex_unlock(&conn_lock);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP move. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP move done. Retcode : %d", status);
    }
    ftp_worker_finish(status);
    return NULL;
}

void * ftp_remove_onorbit(void *){
    ftp_worker_begin();
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_remove(&ftp_config, FtpShared.gsremove->path);
    pthread_mutex_unlock(&conn_lock);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP remove. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP remove done. Retcode : %d", status);
    }
    ftp_worker_finish(status);
    return NULL;
}

void * ftp_copy_onorbit(void *){
    ftp_worker_begin();
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_copy(&ftp_config, FtpShared.gscopy->from, FtpShared.gscopy->to);
    pthread_mutex_unlock(&conn_lock);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP copy. Retcode : %d", status);
	}
    else
    {
        console.AddLog("FTP copy done. Retcode : %d", status);
    }
    ftp_worker_finish(status);
    return NULL;
}

void * ftp_mkdir_onorbit(void *){
    ftp_worker_begin();
    uint32_t mode = 0;
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_mkdir(&ftp_config, FtpShared.gsmkdir->path, mode);
    pthread_mutex_unlock(&conn_lock);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP make directory. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP make directory done. Retcode : %d", status);
    }
    ftp_worker_finish(status);
    return NULL;
}

void * ftp_rmdir_onorbit(void *){
    ftp_worker_begin();
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = ftp_chunk_size_clamped(200);
    pthread_mutex_lock(&conn_lock);
    int status = (int)gs_ftp_rmdir(&ftp_config, FtpShared.gsrmdir->path);
    pthread_mutex_unlock(&conn_lock);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP remove directory. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP remove directory done. Retcode : %d", status);
    }
    ftp_worker_finish(status);
    return NULL;
}
