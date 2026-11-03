#include "ftp_client.h"

#include "svc_ftp.h"

extern StateCheckUnit State;
extern int NowFTP;

namespace mim {
namespace model {

int *      FtpClient::Version()   { return &FtpShared.ftp_version; }
uint32_t * FtpClient::ChunkSize() { return &FtpShared.chunk_sz; }
int *      FtpClient::ForceIteration() { return &FtpShared.force_iteration; }

int       FtpClient::FileSlotCount() const { return sizeof(FtpShared.ftplistup) / sizeof(FtpShared.ftplistup[0]); }
ftpinfo * FtpClient::FileSlot(int i)       { return &FtpShared.ftplistup[i]; }
int *     FtpClient::SelectedIndex()       { return &NowFTP; }
ftpinfo * FtpClient::Selected()            { return &FtpShared.ftplistup[NowFTP]; }

gsftp_listup_t * FtpClient::ListRequest()   { return FtpShared.gslistup; }
gsftp_move_t *   FtpClient::MoveRequest()   { return FtpShared.gsmove; }
gsftp_remove_t * FtpClient::RemoveRequest() { return FtpShared.gsremove; }
gsftp_copy_t *   FtpClient::CopyRequest()   { return FtpShared.gscopy; }
gsftp_mkdir_t *  FtpClient::MkdirRequest()  { return FtpShared.gsmkdir; }
gsftp_rmdir_t *  FtpClient::RmdirRequest()  { return FtpShared.gsrmdir; }
const char *     FtpClient::StatusText() const { return FtpShared.FTPWindowBuffer; }

void FtpClient::UploadSelected()   { mim::svc::FtpUpload(&FtpShared.ftplistup[NowFTP]); }
void FtpClient::DownloadSelected() { mim::svc::FtpDownload(&FtpShared.ftplistup[NowFTP]); }
void FtpClient::List()   { mim::svc::FtpList(); }
void FtpClient::Move()   { mim::svc::FtpMove(); }
void FtpClient::Remove() { mim::svc::FtpRemove(); }
void FtpClient::Copy()   { mim::svc::FtpCopy(); }
void FtpClient::Mkdir()  { mim::svc::FtpMkdir(); }
void FtpClient::Rmdir()  { mim::svc::FtpRmdir(); }

void FtpClient::Stop()           { mim::svc::FtpStop(); }
bool FtpClient::Busy() const     { return mim::svc::FtpBusy(); }
bool FtpClient::Stopping() const { return mim::svc::FtpStopping(); }

FtpClient & Ftp()
{
    static FtpClient ftp;
    return ftp;
}

} // namespace model
} // namespace mim
