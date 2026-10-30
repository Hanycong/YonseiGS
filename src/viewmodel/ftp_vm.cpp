#include "ftp_vm.h"

#include <stdlib.h>
#include <string.h>

#include "miman_coms.h"
#include "svc_tx.h"
#include "svc_ftp.h"

extern StateCheckUnit State;
extern dlreqdata * request;
extern int temptarget;
extern int tempstatus;
extern int tempfilenum;
extern int tempoffset;
extern int tempstep;
extern int NowFTP;

namespace mim {
namespace vm {

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
    request->target = (uint16_t) temptarget;
    request->filestatus = (uint16_t) tempstatus;
    request->filenum = (uint32_t) tempfilenum;
    request->offset = (uint32_t) tempoffset;
    request->step = (uint32_t) tempstep;
}

// "Request##ftpwindow"
void FtpVM::SendDownlinkRequest()
{
    packetsign * request_ = (packetsign *)malloc(sizeof(dlreqdata) + 8);
    request_->Identifier = MIM_ID;
    request_->PacketType = MIM_PT_DLREQ;
    request_->Length = sizeof(dlreqdata);
    memcpy(request_->Data, request, sizeof(dlreqdata));
    mim::svc::SendUplink((void *)request_); 
}

int *      FtpVM::Version()   { return &State.ftp_version; }
uint32_t * FtpVM::ChunkSize() { return &State.chunk_sz; }

void FtpVM::ClampChunkSize()
{
    if(State.chunk_sz < 1)
        State.chunk_sz = 1;
}

int * FtpVM::Task() { return &State.ftp_task; }

void FtpVM::RequireV2()
{
    if(State.ftp_version != 2)
        State.ftp_task = FTP_UPLOAD_REQUEST;
}

void FtpVM::ResetTask()
{
    State.ftp_version = 2;
    State.ftp_task = FTP_UPLOAD_REQUEST;
}

int       FtpVM::FileSlotCount() const  { return sizeof(State.ftplistup) / sizeof(State.ftplistup[0]); }
ftpinfo * FtpVM::FileSlot(int i)        { return &State.ftplistup[i]; }
int *     FtpVM::SelectedFileIndex()    { return &NowFTP; }
ftpinfo * FtpVM::SelectedFile()         { return &State.ftplistup[NowFTP]; }

gsftp_listup_t * FtpVM::ListRequest()   { return State.gslistup; }
gsftp_move_t *   FtpVM::MoveRequest()   { return State.gsmove; }
gsftp_remove_t * FtpVM::RemoveRequest() { return State.gsremove; }
gsftp_copy_t *   FtpVM::CopyRequest()   { return State.gscopy; }
gsftp_mkdir_t *  FtpVM::MkdirRequest()  { return State.gsmkdir; }
gsftp_rmdir_t *  FtpVM::RmdirRequest()  { return State.gsrmdir; }
const char *     FtpVM::StatusText() const { return State.FTPWindowBuffer; }

void FtpVM::Upload()   { mim::svc::FtpUpload(&State.ftplistup[NowFTP]); }
void FtpVM::Download() { mim::svc::FtpDownload(&State.ftplistup[NowFTP]); }
void FtpVM::List()     { mim::svc::FtpList(); }
void FtpVM::Move()     { mim::svc::FtpMove(); }
void FtpVM::Remove()   { mim::svc::FtpRemove(); }
void FtpVM::Copy()     { mim::svc::FtpCopy(); }
void FtpVM::Mkdir()    { mim::svc::FtpMkdir(); }
void FtpVM::Rmdir()    { mim::svc::FtpRmdir(); }

FtpVM & FtpViewModel()
{
    static FtpVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
