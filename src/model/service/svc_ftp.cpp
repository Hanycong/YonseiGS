#include "svc_ftp.h"
#include "svc_thread.h"

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_ftp.h"
#include "ftp_transfer.h"

extern StateCheckUnit State;
extern Console console;

namespace mim {
namespace svc {

// FTP 가 이미 돌고 있으면 에러 로그를 남기고 false
static bool FtpIdle()
{
    if(FtpShared.ftp_mode || miman_ftp_busy())
    {
        console.AddLog("[ERROR]##Still Uplink Thread is Running. Please Retry Later.");
        return false;
    }
    return true;
}

static bool StartFtp(ThreadFn fn, void * arg, const char * label)
{
    return ftp_start_worker(fn, arg, label);
}

// 엔진(버전)별 워커 (누리호 5차 GS start_ftp_transfer)
static ThreadFn TransferWorker(bool upload)
{
    switch (FtpShared.ftp_version)
    {
    case FTP_VERSION_V1_FORCE: return upload ? ftp_uplink_force : ftp_downlink_force;
    case FTP_VERSION_RYU:      return upload ? ftp_ryu_uplink_onorbit : ftp_ryu_downlink_onorbit;
    default:                   return upload ? ftp_uplink_onorbit : ftp_downlink_onorbit;
    }
}

static bool StartTransfer(const ftpinfo * info, bool upload)
{
    if(!FtpIdle())
        return false;
    // 경로를 복사한 인자를 워커가 가진다 (워커가 읽기 전에 GUI 가 칸을 바꿔도 안전)
    miman_ftp_worker_arg_t * arg = miman_ftp_create_worker_arg(info->local_path, info->remote_path);
    if (arg == NULL)
    {
        console.AddLog("[ERROR]##Failed to allocate FTP worker argument.");
        return false;
    }
    if (!StartFtp(TransferWorker(upload), arg, upload ? "upload" : "download"))
    {
        miman_ftp_destroy_worker_arg(arg);
        return false;
    }
    return true;
}

bool FtpUpload(const ftpinfo * info)   { return StartTransfer(info, true); }
bool FtpDownload(const ftpinfo * info) { return StartTransfer(info, false); }

void FtpList()   { if(FtpIdle()) StartFtp(ftp_list_onorbit,   NULL, "list"); }
void FtpMove()   { if(FtpIdle()) StartFtp(ftp_move_onorbit,   NULL, "move"); }
void FtpRemove() { if(FtpIdle()) StartFtp(ftp_remove_onorbit, NULL, "remove"); }
void FtpCopy()   { if(FtpIdle()) StartFtp(ftp_copy_onorbit,   NULL, "copy"); }
void FtpMkdir()  { if(FtpIdle()) StartFtp(ftp_mkdir_onorbit,  NULL, "mkdir"); }
void FtpRmdir()  { if(FtpIdle()) StartFtp(ftp_rmdir_onorbit,  NULL, "rmdir"); }

void FtpStop()
{
    if (!miman_ftp_busy())
        return;
    miman_ftp_process_stop();
    console.AddLog("[STOP]##FTP stop requested by user.");
}

bool FtpBusy()     { return miman_ftp_busy(); }
bool FtpStopping() { return miman_ftp_stopping(); }

} // namespace svc
} // namespace mim
