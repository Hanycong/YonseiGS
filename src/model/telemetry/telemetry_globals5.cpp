// Model - 누리호 5차 위성 텔레메트리 전역 (임시 정의)
//
// 이 전역들은 YonseiGS_coms.h 에 extern 으로 선언돼 있고, 수신 스레드(5차 downlink 수신/해석)가 채운다.
// n5-view 브랜치에서는 화면(Beacon / Report 탭)이 읽기만 하므로 링크만 되도록 여기서 정의한다.
//   definitions — downlink branch may move these
// 5차 downlink 브랜치가 같은 이름을 정의하면 병합할 때 이 파일을 지운다
// (또는 그 브랜치에서 YONSEIGS_DL5_GLOBALS_DEFINED 를 정의하면 이 파일은 비게 된다).
//
// 초기값은 GS_nuri5 miman_coms.cpp 와 같다. 단 비콘 버퍼는 malloc 대신 calloc (받기 전에는 0).
#ifndef YONSEIGS_DL5_GLOBALS_DEFINED

#include <stdlib.h>
#include "YonseiGS_coms.h"

RPT_OpsData_t g_rpt_ops_data;
bool g_rpt_ops_data_valid = false;
EPS_Report_State_t g_eps_report_state;
EPS_RParam_Reassembly_State_t g_eps_rparam_reassembly;

BEE1012_Beacon_t* bee1012_beacon = (BEE1012_Beacon_t *)calloc(1, BEE1012_LEN_BEACON);
UELYSYS_Beacon_t* uelysys_beacon = (UELYSYS_Beacon_t *)calloc(1, UELYSYS_LEN_BEACON);
SD1SAT_Beacon_t* sd1sat_beacon = (SD1SAT_Beacon_t *)calloc(1, SD1SAT_LEN_BEACON);

#endif // YONSEIGS_DL5_GLOBALS_DEFINED
