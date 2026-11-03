// Service 계층 - 온보드 FTP 작업 (SLOT_FTP)
//
// 각 함수는 UI 버튼이 하던 "FTP 실행 중 확인 -> (버전 확인) -> 워커 시작" 을 원래 로그 그대로 옮긴 것이다.
// 업/다운로드 엔진은 FtpShared.ftp_version 으로 고른다 (1 = V1 force, 2 = V2, 3 = Ryu. ftp_transfer.h).
#pragma once

#include "YonseiGS_config.h"     // ftpinfo

namespace mim {
namespace svc {

// info: 보낼 파일 칸 (보통 &FtpShared.ftplistup[index]). 경로를 복사해 워커에 넘기므로 시작 뒤 칸을 바꿔도 된다.
// 워커를 띄웠으면 true (중복 실행 / 실패 쿨다운이면 로그를 남기고 false)
bool FtpUpload(const ftpinfo * info);
bool FtpDownload(const ftpinfo * info);

// 경로 등 요청 내용은 FtpShared.gslistup / gsmove / gsremove / gscopy / gsmkdir / gsrmdir 에 있다
void FtpList();
void FtpMove();
void FtpRemove();
void FtpCopy();
void FtpMkdir();
void FtpRmdir();

// Stop (누리호 5차 GS): 진행 중인 FTP 를 멈춘다. 멈춘 업/다운로드는 같은 파일로 다시 시작하면 이어받는다.
// 바로 돌아온다 (워커가 끝나는 것은 FtpBusy() 로 본다)
void FtpStop();
bool FtpBusy();         // 워커 시작 요청 ~ 워커 끝
bool FtpStopping();     // Stop 을 눌렀고 워커가 아직 끝나지 않았다

} // namespace svc
} // namespace mim
