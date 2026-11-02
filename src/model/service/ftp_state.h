// Model - 온보드 FTP 공유 상태 (진행 중 작업·청크 상태·목록/이동/복사/삭제 요청 버퍼)
//
// StateCheckUnit(State) 에서 이 모듈이 쓰는 멤버를 선언 그대로 옮긴 것. 인스턴스는 FtpShared (ftp_state.cpp 에 정의).
// YonseiGS_config.h 끝에서 include 하므로 기존처럼 YonseiGS_config.h 만 include 해도 보인다.
#pragma once

#include "YonseiGS_config.h"

struct FtpState {
    bool ftp_mode = false;
    uint32_t chunk_sz = 200;
    int ftp_version = 2;
    int ftp_task = FTP_UPLOAD_REQUEST;
    char FTPWindowBuffer[64];
    chunkstate_t * ChunkState;
    gsftp_listup_t * gslistup;
    gsftp_move_t * gsmove;
    gsftp_copy_t * gscopy;
    gsftp_remove_t * gsremove;
    gsftp_mkdir_t * gsmkdir;
    gsftp_rmdir_t * gsrmdir;
    ftpinfo ftplistup[64];
};

extern FtpState FtpShared;
