// Model - 수신 데이터 저장 파일 이름 (model/telemetry/telemetry_files.cpp)
//
// 누리호 5차 위성(BEE1012 / UELYSYS / SD1SAT) 수신 데이터를 ../data/<위성>/... 아래에 저장할 때 쓴다.
// GS_nuri5 는 초 단위 파일 이름이라 같은 초에 받은 패킷(FTP 후 한꺼번에 저장하는 비콘, 여러 개 묶인 리포트)이
// 서로 덮어썼다. 여기서는 밀리초까지 넣고, 그래도 같은 이름이 있으면 "_1", "_2" ... 를 붙인다.
// BEE-1000 저장 경로/이름은 바꾸지 않는다 (YonseiGS_coms.cpp, telemetry_logger.cpp 그대로).
#pragma once

#include <stdio.h>
#include <sys/time.h>
#include <string>

// dir 을 만들고 (없으면) "<dir>/<prefix>YYYY-MM-DD-HH-MM-SS-mmm<suffix>" 를 mode 로 연다.
// rx_time 이 NULL 이면 현재 시각. 실패하면 NULL (path 에는 시도한 이름).
FILE * OpenTelemetryFile(const char * dir, const char * prefix, const char * suffix,
                         const struct timeval * rx_time, const char * mode, std::string * path = NULL);
