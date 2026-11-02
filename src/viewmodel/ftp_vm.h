// ViewModel - FTP 창 (Pass Operation 탭 가운데 열)
//
// 위: 다운링크 파일 요청(종류/상태/번호/스텝/오프셋 -> Save, Request)
// 아래: FTP 작업(버전, 청크 크기, 작업 종류별 입력 칸과 버튼). AMP 버튼은 ControlVM::Amp() 를 같이 쓴다.
// 입력 칸은 ImGui 위젯이 직접 편집하도록 주소/구조체를 돌려준다 (구조체 형식은 YonseiGS_config.h).
#pragma once

#include <stdint.h>
#include "YonseiGS_config.h"     // ftpinfo, gsftp_*_t, FTP_*_REQUEST

namespace mim {
namespace vm {

class FtpVM
{
public:
    // ---- 다운링크 파일 요청 ----
    int * DownlinkTarget();             // MIM_DLTYPE_*
    int * DownlinkStatus();             // MIM_DLSTAT_*
    int * FileNumber();
    int * Step();
    int * Offset();
    void ClampDownlinkRequest();        // 번호 >= 1, 스텝/오프셋 >= 0 (입력 칸 그리기 전 매 프레임)
    void SaveDownlinkRequest();         // 입력 값을 요청 버퍼에 저장
    void SendDownlinkRequest();         // 요청 버퍼를 업링크

    // ---- FTP 작업 ----
    int *      Version();               // 1, 2
    uint32_t * ChunkSize();
    void       ClampChunkSize();        // >= 1 (입력 칸 다음 매 프레임)
    int *      Task();                  // FTP_*_REQUEST
    void       RequireV2();             // v2 전용 작업인데 v1 이면 Upload 로 되돌린다
    void       ResetTask();             // 알 수 없는 작업이면 v2 / Upload 로

    int       FileSlotCount() const;    // 업/다운로드 파일 목록 칸 수 (이름이 빈 칸은 건너뜀)
    ftpinfo * FileSlot(int i);
    int *     SelectedFileIndex();
    ftpinfo * SelectedFile();

    gsftp_listup_t * ListRequest();
    gsftp_move_t *   MoveRequest();
    gsftp_remove_t * RemoveRequest();
    gsftp_copy_t *   CopyRequest();
    gsftp_mkdir_t *  MkdirRequest();
    gsftp_rmdir_t *  RmdirRequest();
    const char *     StatusText() const;   // FTP 스레드가 쓰는 진행 문구

    void Upload();
    void Download();
    void List();
    void Move();
    void Remove();
    void Copy();
    void Mkdir();
    void Rmdir();
};

FtpVM & FtpViewModel();

} // namespace vm
} // namespace mim
