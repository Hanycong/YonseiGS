// Service 계층 - TX 슬롯(p_thread[4]) 작업
//
// 업링크 명령 패킷 전송, ping, direct shell 처럼 TX 슬롯을 쓰는 작업을 시작/대기한다.
// 각 함수는 기존 pthread_create / pthread_join 한 줄과 1:1 로 대응한다.
// (join + create 를 하나로 합치지 않는다: join 없이 create 만 하던 호출부가 있으므로)
#pragma once

namespace mim {
namespace svc {

// 이전 TX 작업이 끝날 때까지 대기 (pthread_join(p_thread[4]))
int WaitTx();

// task_uplink_onorbit(packet) 을 TX 슬롯에서 시작
int SendUplink(void * packet);

// csp_ping_scan
int StartPingScan();

// csp_ping_console
int StartPingConsole();

// csp_ping_everynode_console(arg)
int StartPingEveryNode(void * arg);

// Direct_Shell(shellcmd)
int StartDirectShell(void * shellcmd);

} // namespace svc
} // namespace mim
