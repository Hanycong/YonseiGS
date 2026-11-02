// Model - 텔레메트리 기록 (model/telemetry/telemetry_logger.cpp)
//
// 수신 스레드(task_downlink_onorbit)가 패킷 종류별로 부른다.
#pragma once

#include "miman_coms.h"     // Beacon, MissionBeacon, Report, Event

int BeaconSaver(Beacon* bec);
int MissionBeaconSaver(MissionBeacon* misnbec);
int ReportSaver(Report* rpt);
int EventSaver(Event *event);
