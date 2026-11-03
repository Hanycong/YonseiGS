// Model - 누리호 5차 위성 비콘 수신/저장 (model/telemetry/n5_beacon.cpp)
//
// 수신 스레드(task_downlink_onorbit)가 포트 17(BEE1012) / 12(UELYSYS) / 22(SD1SAT) 패킷을 넘긴다.
// FTP 중에는 TRxController 가 CloseDownlinkConnDuringFtp 로 연결을 받아 비콘만 쌓아 두고,
// FTP 가 끝나면 FlushFtpBeaconBuffers 로 수신 시각 그대로 저장한다.
#pragma once

#include <sys/time.h>
#include <mutex>

#include "YonseiGS_coms.h"     // BEE1012_Beacon_t, UELYSYS_Beacon_t, SD1SAT_Beacon_t, csp

// 전역 비콘 버퍼(bee1012_beacon, uelysys_beacon, sd1sat_beacon) 보호
extern std::mutex g_n5_beacon_mtx;

// 포트별 처리: 비콘 길이면 원본/해석 파일 저장 + 전역 버퍼 갱신, 아니면 미션비콘/깨진 패킷으로 원본 저장.
// rx_time 이 NULL 이면 현재 시각, 아니면 그 시각으로 파일 이름을 만든다 (FTP 후 저장).
void ProcessBEE1012BeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time = NULL);
void ProcessUELYSYSBeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time = NULL);
void ProcessSD1SATBeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time = NULL);

// FTP 중 (FtpShared.ftp_mode, 수신 스레드 정지) 들어온 연결 하나를 받아 닫는다. 5차 비콘은 버퍼에 쌓는다
void CloseDownlinkConnDuringFtp(csp_socket_t * sock);
// FTP 가 끝난 뒤 쌓아 둔 비콘을 저장하고 비운다 (비어 있으면 아무것도 안 함)
void FlushFtpBeaconBuffers(void);

// 마지막으로 받은 비콘을 잠금 아래에서 복사. 받은 적 없으면 false (out 은 0)
bool CopyBEE1012Beacon(BEE1012_Beacon_t & out);
bool CopyUELYSYSBeacon(UELYSYS_Beacon_t & out);
bool CopySD1SATBeacon(SD1SAT_Beacon_t & out);
