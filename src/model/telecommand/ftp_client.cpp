#include "ftp_client.h"

#include "svc_ftp.h"

extern StateCheckUnit State;
extern int NowFTP;

namespace mim {
namespace model {

int *      FtpClient::Version()   { return &State.ftp_version; }
uint32_t * FtpClient::ChunkSize() { return &State.chunk_sz; }

int       FtpClient::FileSlotCount() const { return sizeof(State.ftplistup) / sizeof(State.ftplistup[0]); }
ftpinfo * FtpClient::FileSlot(int i)       { return &State.ftplistup[i]; }
int *     FtpClient::SelectedIndex()       { return &NowFTP; }
ftpinfo * FtpClient::Selected()            { return &State.ftplistup[NowFTP]; }

gsftp_listup_t * FtpClient::ListRequest()   { return State.gslistup; }
gsftp_move_t *   FtpClient::MoveRequest()   { return State.gsmove; }
gsftp_remove_t * FtpClient::RemoveRequest() { return State.gsremove; }
gsftp_copy_t *   FtpClient::CopyRequest()   { return State.gscopy; }
gsftp_mkdir_t *  FtpClient::MkdirRequest()  { return State.gsmkdir; }
gsftp_rmdir_t *  FtpClient::RmdirRequest()  { return State.gsrmdir; }
const char *     FtpClient::StatusText() const { return State.FTPWindowBuffer; }

void FtpClient::UploadSelected()   { mim::svc::FtpUpload(&State.ftplistup[NowFTP]); }
void FtpClient::DownloadSelected() { mim::svc::FtpDownload(&State.ftplistup[NowFTP]); }
void FtpClient::List()   { mim::svc::FtpList(); }
void FtpClient::Move()   { mim::svc::FtpMove(); }
void FtpClient::Remove() { mim::svc::FtpRemove(); }
void FtpClient::Copy()   { mim::svc::FtpCopy(); }
void FtpClient::Mkdir()  { mim::svc::FtpMkdir(); }
void FtpClient::Rmdir()  { mim::svc::FtpRmdir(); }

FtpClient & Ftp()
{
    static FtpClient ftp;
    return ftp;
}

} // namespace model
} // namespace mim
