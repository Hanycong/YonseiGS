#include "svc_ftp.h"
#include "svc_thread.h"

#include "miman_config.h"
#include "miman_coms.h"
#include "miman_ftp.h"

extern StateCheckUnit State;
extern Console console;
extern bool ftp_start_worker(pthread_t *thread, void *(*start_routine)(void *), void *arg, const char *label);

namespace mim {
namespace svc {

// FTP 가 이미 돌고 있으면 에러 로그를 남기고 false
static bool FtpIdle()
{
    if(State.ftp_mode)
    {
        console.AddLog("[ERROR]##Still Uplink Thread is Running. Please Retry Later.");
        return false;
    }
    return true;
}

static void StartFtp(ThreadFn fn, void * arg, const char * label)
{
    ftp_start_worker(ThreadHandle(SLOT_FTP), fn, arg, label);
}

void FtpUpload(void * info)
{
    if(!FtpIdle())
        return;
    if(State.ftp_version == 1)
        console.AddLog("[ERROR]##ftp V1 is deprecated. Use V2.");
        //pthread_create(&p_thread[8], NULL, ftp_uplink_force, &State.ftplistup[NowFTP]);
    else
        StartFtp(ftp_uplink_onorbit, info, "upload");
}

void FtpDownload(void * info)
{
    if(!FtpIdle())
        return;
    if(State.ftp_version == 1)
        console.AddLog("[ERROR]##ftp V1 is deprecated. Use V2.");
        // pthread_create(&p_thread[8], NULL, ftp_downlink_force, &State.ftplistup[NowFTP]);
    else
        StartFtp(ftp_downlink_onorbit, info, "download");
}

void FtpList()   { if(FtpIdle()) StartFtp(ftp_list_onorbit,   NULL, "list"); }
void FtpMove()   { if(FtpIdle()) StartFtp(ftp_move_onorbit,   NULL, "move"); }
void FtpRemove() { if(FtpIdle()) StartFtp(ftp_remove_onorbit, NULL, "remove"); }
void FtpCopy()   { if(FtpIdle()) StartFtp(ftp_copy_onorbit,   NULL, "copy"); }
void FtpMkdir()  { if(FtpIdle()) StartFtp(ftp_mkdir_onorbit,  NULL, "mkdir"); }
void FtpRmdir()  { if(FtpIdle()) StartFtp(ftp_rmdir_onorbit,  NULL, "rmdir"); }

} // namespace svc
} // namespace mim
