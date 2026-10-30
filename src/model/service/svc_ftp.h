// Service 계층 - 온보드 FTP 작업 (SLOT_FTP)
//
// 각 함수는 UI 버튼이 하던 "FTP 실행 중 확인 -> (버전 확인) -> 워커 시작" 을
// 원래 순서/로그 그대로 옮긴 것이다.
#pragma once

namespace mim {
namespace svc {

// info: &State.ftplistup[index]
void FtpUpload(void * info);
void FtpDownload(void * info);

// 경로 등 요청 내용은 State.gslistup / gsmove / gsremove / gscopy / gsmkdir / gsrmdir 에 있다
void FtpList();
void FtpMove();
void FtpRemove();
void FtpCopy();
void FtpMkdir();
void FtpRmdir();

} // namespace svc
} // namespace mim
