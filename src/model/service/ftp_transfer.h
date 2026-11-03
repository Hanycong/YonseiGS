// Model/Service - FTP 전송 엔진 선택, 워커 인자, Stop, 전송 상태 (누리호 5차 GS miman_ftprdp_integration.h)
//
// 엔진 (FtpShared.ftp_version):
//   1 = V1 force  (libftp_client gs_ftp_*_force, CSP 포트 15, 반복 FtpShared.force_iteration)
//   2 = V2        (libftp_client gs_ftp_upload/download, CSP 포트 9)  <- 기본
//   3 = Ryu       (lib/libftp_client_ryu ftpnew_*, CSP 포트 19). 누리호 5차 GS 의 Ryu 이전 보고서가 NO-GO 라
//                 기본값이 아니고 V3 라디오로만 고른다
// 목적지 노드는 FtpShared.dest_node (모든 FTP 작업 공통, 누리호 5차 GS "FTP with SLT").
//
// Start/Stop (모든 엔진 공통):
//   ftp_start_worker 가 busy 를 세우고 (gs_ftp_process = true) 워커를 SLOT_FTP 에 띄운다.
//   miman_ftp_process_stop() 은 gs_ftp_process = false + ftp_abort(). V2 의 ftp_data() 가 시작할 때
//   ftpavailable 을 true 로 되돌리므로 워커가 끝날 때까지 SLOT_FTP_STOP 스레드가 ftp_abort() 를 계속 부른다.
//   워커가 끝나면 busy 가 내려간다. 사용자가 멈춘 전송은 실패 쿨다운(30 초)을 걸지 않는다 (바로 이어받기).
#pragma once

#include <stdint.h>

enum FtpVersion
{
    FTP_VERSION_V1_FORCE = 1,
    FTP_VERSION_V2       = 2,
    FTP_VERSION_RYU      = 3,
};

// 마지막 전송의 상태 (워커가 시작할 때와 끝날 때 갱신)
typedef struct {
    uint32_t transfer_id;
    int return_code;
    int cleanup_done;           // 1 = 워커가 정리까지 끝냈다
    int user_stopped;           // 1 = Stop 으로 끝났다
    char engine[8];             // "V1" / "V2" / "RYU"
    char direction[16];         // "UPLOAD" / "DOWNLOAD"
    char local_path[256];
    char remote_path[256];
} miman_ftp_transfer_status_t;

// 업/다운로드 워커 인자: 경로를 복사해 둔 힙 객체. 워커가 받아서 해제한다
// (FtpShared.ftplistup 칸을 그대로 넘기면 워커가 읽기 전에 GUI 가 칸을 바꿀 수 있다)
typedef struct miman_ftp_worker_arg miman_ftp_worker_arg_t;
miman_ftp_worker_arg_t * miman_ftp_create_worker_arg(const char * local_path, const char * remote_path);
void miman_ftp_destroy_worker_arg(miman_ftp_worker_arg_t * arg);

int miman_ftp_get_last_transfer_status(miman_ftp_transfer_status_t * status);

// 워커 스레드 함수 (인자: miman_ftp_worker_arg_t*, 옛 호출처럼 ftpinfo* 도 받지만 경고 로그)
void * ftp_uplink_onorbit(void * param);        // V2
void * ftp_downlink_onorbit(void * param);
void * ftp_uplink_force(void * param);          // V1
void * ftp_downlink_force(void * param);
void * ftp_ryu_uplink_onorbit(void * param);    // Ryu
void * ftp_ryu_downlink_onorbit(void * param);

// FTP 워커를 SLOT_FTP 에 띄운다. 다른 워커가 돌고 있거나(중복 실행), 실패 후 30 초 쿨다운이면 로그만 남기고 false.
// GUI 스레드에서 불러도 된다: 앞 워커는 이미 끝났으므로 join 이 바로 돌아온다.
bool ftp_start_worker(void *(*start_routine)(void *), void * arg, const char * label);

bool miman_ftp_busy();          // 워커 시작 요청 ~ 워커 끝
bool miman_ftp_stopping();      // busy 이고 Stop 이 눌렸다 ("Stopping...")
void miman_ftp_process_stop();  // Stop 버튼 / 종료 (busy 가 아니면 아무것도 안 한다)
