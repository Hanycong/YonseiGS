// Model - 수신 데이터 저장 파일 이름 (model/telemetry/telemetry_files.cpp)
//
// 누리호 5차 위성(BEE1012 / UELYSYS / SD1SAT) 수신 데이터를 ../data/<위성>/... 아래에 저장할 때 쓴다.
// GS_nuri5 는 초 단위 파일 이름이라 같은 초에 받은 패킷(FTP 후 한꺼번에 저장하는 비콘, 여러 개 묶인 리포트)이
// 서로 덮어썼다. 여기서는 밀리초까지 넣고, 그래도 같은 이름이 있으면 "_1", "_2" ... 를 붙인다.
// BEE-1000 저장 경로/이름은 바꾸지 않는다 (YonseiGS_coms.cpp, telemetry_logger.cpp 그대로).
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <string>

// dir 을 만들고 (없으면) "<dir>/<prefix>YYYY-MM-DD-HH-MM-SS-mmm<suffix>" 를 mode 로 연다.
// rx_time 이 NULL 이면 현재 시각. 실패하면 NULL (path 에는 시도한 이름).
FILE * OpenTelemetryFile(const char * dir, const char * prefix, const char * suffix,
                         const struct timeval * rx_time, const char * mode, std::string * path = NULL);

// BEE-1000 저장 파일 이름 (예전 형식 그대로): "<prefix>YYYY-MM-DD-HH-MM-SS<suffix>" 를 out 에 쓴다.
// 지금 시각, 초 단위. 폴더를 만들지 않고 겹치는 이름도 확인하지 않는다 (같은 초에 받으면 덮어쓴다: 원래 동작).
void SecondStampedPath(char * out, size_t size, const char * prefix, const char * suffix);

// 수신 패킷 바이트 덤프 (10바이트마다 줄바꿈).
//   표준출력: upper_stdout 이면 "0x%02X ", 아니면 "0x%x " (원래 자리마다 달랐던 형식 그대로)
//   fp      : NULL 이 아니면 "%02hhx\t" 로 같은 줄바꿈 (원본 패킷 저장 파일 형식)
void DumpPacketBytes(FILE * fp, const uint8_t * data, int length, bool upper_stdout);
