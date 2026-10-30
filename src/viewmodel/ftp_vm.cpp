#include "ftp_vm.h"

#include "ftp_client.h"
#include "downlink_request.h"

extern StateCheckUnit State;
extern int temptarget;
extern int tempstatus;
extern int tempfilenum;
extern int tempoffset;
extern int tempstep;

namespace mim {
namespace vm {

using mim::model::Ftp;

int * FtpVM::DownlinkTarget() { return &temptarget; }
int * FtpVM::DownlinkStatus() { return &tempstatus; }
int * FtpVM::FileNumber()     { return &tempfilenum; }
int * FtpVM::Step()           { return &tempstep; }
int * FtpVM::Offset()         { return &tempoffset; }

void FtpVM::ClampDownlinkRequest()
{
    if(tempfilenum < 1)
        tempfilenum = 1;
    if(tempstep < 0)
        tempstep = 0;
    if(tempoffset < 0)
        tempoffset = 0;
}

// "Save##ftpwindow"
void FtpVM::SaveDownlinkRequest()
{
    mim::model::Downlink().Store((uint16_t) temptarget, (uint16_t) tempstatus, (uint32_t) tempfilenum,
                                 (uint32_t) tempoffset, (uint32_t) tempstep);
}

// "Request##ftpwindow"
void FtpVM::SendDownlinkRequest()
{
    mim::model::Downlink().Send();
}

int *      FtpVM::Version()   { return Ftp().Version(); }
uint32_t * FtpVM::ChunkSize() { return Ftp().ChunkSize(); }

void FtpVM::ClampChunkSize()
{
    if(*Ftp().ChunkSize() < 1)
        *Ftp().ChunkSize() = 1;
}

int * FtpVM::Task() { return &State.ftp_task; }

void FtpVM::RequireV2()
{
    if(*Ftp().Version() != 2)
        State.ftp_task = FTP_UPLOAD_REQUEST;
}

void FtpVM::ResetTask()
{
    *Ftp().Version() = 2;
    State.ftp_task = FTP_UPLOAD_REQUEST;
}

int       FtpVM::FileSlotCount() const  { return Ftp().FileSlotCount(); }
ftpinfo * FtpVM::FileSlot(int i)        { return Ftp().FileSlot(i); }
int *     FtpVM::SelectedFileIndex()    { return Ftp().SelectedIndex(); }
ftpinfo * FtpVM::SelectedFile()         { return Ftp().Selected(); }

gsftp_listup_t * FtpVM::ListRequest()   { return Ftp().ListRequest(); }
gsftp_move_t *   FtpVM::MoveRequest()   { return Ftp().MoveRequest(); }
gsftp_remove_t * FtpVM::RemoveRequest() { return Ftp().RemoveRequest(); }
gsftp_copy_t *   FtpVM::CopyRequest()   { return Ftp().CopyRequest(); }
gsftp_mkdir_t *  FtpVM::MkdirRequest()  { return Ftp().MkdirRequest(); }
gsftp_rmdir_t *  FtpVM::RmdirRequest()  { return Ftp().RmdirRequest(); }
const char *     FtpVM::StatusText() const { return Ftp().StatusText(); }

void FtpVM::Upload()   { Ftp().UploadSelected(); }
void FtpVM::Download() { Ftp().DownloadSelected(); }
void FtpVM::List()     { Ftp().List(); }
void FtpVM::Move()     { Ftp().Move(); }
void FtpVM::Remove()   { Ftp().Remove(); }
void FtpVM::Copy()     { Ftp().Copy(); }
void FtpVM::Mkdir()    { Ftp().Mkdir(); }
void FtpVM::Rmdir()    { Ftp().Rmdir(); }

FtpVM & FtpViewModel()
{
    static FtpVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
