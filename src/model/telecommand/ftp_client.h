// Model - 위성 FTP (파일 업로드/다운로드/목록/이동/삭제/복사/디렉토리)
//
// FTP 설정(버전, 청크 크기), 업/다운로드 파일 칸(State.ftplistup, 선택 칸 NowFTP), 작업별 요청
// 구조체(State.gslistup 등), 진행 문구(State.FTPWindowBuffer)와 작업 시작(svc_ftp)을 묶는다.
// 이 값들은 AutoPilot / 수신 스레드(파일 목록 응답) / FTP 스레드도 같이 쓴다.
// 현재는 StateCheckUnit 전역을 감싼다 (StateCheckUnit 분해 때 이 클래스가 값을 가진다).
#pragma once

#include <stdint.h>
#include "YonseiGS_config.h"     // ftpinfo, gsftp_*_t

namespace mim {
namespace model {

class FtpClient
{
public:
    int *      Version();               // 1, 2
    uint32_t * ChunkSize();             // [byte]

    int       FileSlotCount() const;
    ftpinfo * FileSlot(int i);
    int *     SelectedIndex();          // NowFTP
    ftpinfo * Selected();

    gsftp_listup_t * ListRequest();
    gsftp_move_t *   MoveRequest();
    gsftp_remove_t * RemoveRequest();
    gsftp_copy_t *   CopyRequest();
    gsftp_mkdir_t *  MkdirRequest();
    gsftp_rmdir_t *  RmdirRequest();
    const char *     StatusText() const;

    // 작업 시작 (FTP 스레드 슬롯)
    void UploadSelected();
    void DownloadSelected();
    void List();
    void Move();
    void Remove();
    void Copy();
    void Mkdir();
    void Rmdir();
};

FtpClient & Ftp();

} // namespace model
} // namespace mim
