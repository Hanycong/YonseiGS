// Service 계층 - TC 로그 (위성별)
//
// log_ptr(main.cpp) 은 송신한 명령과 콘솔 메시지를 적는 TC 로그 파일이다.
// 위성을 바꾸면 그 위성 폴더에 새 파일을 연다 (누리호 5차 GS 와 같은 경로):
//   BEE1000 -> ../log/TC_Log/log--<시각>--      그 외 -> ../data/<위성 키>/tc_log/log--<시각>--
// log_ptr 에 쓰는 곳(콘솔, 업링크, WriteSystemName)은 LockTcLog/UnlockTcLog 로 감싼다 (파일 교체와 겹치지 않게).
//
// 송신한 TC 패킷은 하나마다 파일 하나로도 남긴다: ../data/<위성 키>/tc_log/packet/TCpacketLog--<시각>.txt
#pragma once

namespace mim {
namespace svc {

void LockTcLog();
void UnlockTcLog();

// mode: mim::sat::SatMode. 실패하면 이전 파일을 그대로 두고 false
bool SwitchTcLogFile(int mode);
// 프로그램 끝에서 닫는다
void CloseTcLogFile();

// 송신 직전 패킷 기록. sat_key: 폴더 이름 (mim::sat::CurrentKey())
// packet: packetsign (YonseiGS_coms.h). 헤더 의존을 줄이려고 void 로 받는다
void RecordTcPacket(const void * packet, const char * sat_key);

} // namespace svc
} // namespace mim
