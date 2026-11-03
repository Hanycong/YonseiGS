// 공통 정의 - 누리호 5차 GS(GS_nuri5) 에서 추가된 위성 명령 정의
//
// BEE1012 / UELYSYS / SD1SAT 의 FSW 앱 MID, CC, 명령·텔레메트리 구조체와 EPS P80 정의.
// GS_nuri5 miman_config.h 에서 YonseiGS_config.h 에 없는 항목만 옮겼다 (순서·내용 그대로).
// 같은 이름인데 값이 다른 항목은 BEE-1000 정의(YonseiGS_config.h)를 유지하고, 5차 쪽은 이름을 바꿨다:
//   EPS_BCN_TLM_ID / EPS_HK_TLM_ID (P80)  -> EPS_P80_BCN_TLM_ID / EPS_P80_HK_TLM_ID
//   TO_LAB_EnableOutputCmd_t (목적지 IP)  -> TO_LAB_EnableOutputIpCmd_t
//   ADCS_GET_RAW_{CUBESENSE_SUN,CSS_SENSOR,GYR_SENSOR}_CC, ADCS_SEQ_{VELPT,KSCPT,LGCPT}_CC (BEE1012 ADCS)
//     -> 쓰는 곳에서 ADCS_GET_RAW_CALIBRATED_*_CC, ADCS_SEQ_{SUN,TGT,NADIR}_CC 를 직접 쓴다
#pragma once

#include "YonseiGS_config.h"

#define EPS_P80_BCN_TLM_ID        0x0875
#define EPS_P80_HK_TLM_ID         0x0876

/* Node & Port Configurations */
// Node Configuration

// Port Configuration

/* Time Settings*/


/* Packet Configurations */
// Packet ID Configuration

// Packet Type Configuration
#define B12_UL_AIOBC                11
#define UEL_UL_PAYYONSEI_OBC        12
#define UEL_UL_SDR_CONTROLLER       13
#define MIM_PT_NODE_TC              14      // GS_nuri5 의 BEE1000: Data 앞 2바이트가 목적 CSP 노드 (포트 13)

#define UELYSYS_PAYYONSEI_OBC_NODE  13
#define UELYSYS_SDR_CONTROLLER_NODE 14
#define UELYSYS_REMOTE_TC_PORT      13

// Packet Parser

// Packet Separator


/* Telecommunicaion Configurations */
// Telecommunication Length
#define BEE1000_LEN_BEACON          sizeof(BEE1000_Beacon_t)
#define BEE1012_LEN_BEACON          sizeof(BEE1012_Beacon_t)
#define UELYSYS_LEN_BEACON          sizeof(UELYSYS_Beacon_t)
#define SD1SAT_LEN_BEACON           sizeof(SD1SAT_Beacon_t)

// Telecommunication Default Settings


/* CCSDS Primary Standard definitions */


/* V1 Command Message IDs must be 0x18xx */
/* V1 Telemetry Message IDs must be 0x08xx */


/*
** TS App command codes
*/


// typedef struct {
//     uint8_t StreamId[2];
//     uint8_t Sequence[2];
//     uint8_t Length[2];
// } CCSDS_PrimaryHeader_t;


/*************************************************************************/
/*
** FTP File Parser
*/
/*************************************************************************/

/*
** FTP File Header
*/


/*
** Ctrlo Type Definitions
*/


/*
** GPS_raw Type Definitions
*/


/* RANGEGPSL1 log measurements. */


/* SATXYZ2 log measurements. */


/* Auxilary measurement package. */


/*
** HK Type Definitions
*/

/*
** SNSR_low Type Definitions
*/


// ADDED FOR BEE-1006 by Jane

// msg id

// Telemetry topics

/*******************************************/
/*                                         */
/*             ADCS  (BEE-1000)            */
/*                                         */
/*******************************************/

// function codes

#define ADCS_SET_INTERFACE_TRANSPORT_CC         91

/* < ADCS Module Command Code (TC, TM) > */
/* Telecommand */

/* Telemetry */
#define ADCS_GET_RAW_CALIBRATED_CUBESENSE_SUN_CC	60 // 170 + 178
#define ADCS_GET_RAW_CALIBRATED_CSS_SENSOR_CC	79 // 203 + 206
#define ADCS_GET_RAW_CALIBRATED_GYR_SENSOR_CC	80 // 204 + 207
#define ADCS_GET_RAW_CALIBRATED_RWL_SENSOR_CC   90 // 205 + 209
#define ADCS_GET_RAW_RWL_SENSOR_CC ADCS_GET_RAW_CALIBRATED_RWL_SENSOR_CC

#define ADCS_SEQ_GNDPT_CC						101
#define ADCS_SEQ_SUN_CC						102
#define ADCS_SEQ_TGT_CC						103
#define ADCS_SEQ_NADIR_CC					104

// #define ADCS_LOOPTEST_CC		106


#define ADCS_GYRO_LOG_CC                        117

typedef struct
{
    uint8 flag_estmode;
    uint16 target_duration;
} __attribute__((packed)) ADCS_SequenceCmdGNDpointing_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    ADCS_SequenceCmdGNDpointing_Payload_t Payload;
} __attribute__((packed)) ADCS_SequenceCmdGNDpointingCmd_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                   << ADCS BCN, HK, AOD Structures >>                      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                     ADCS Set Cmd Payload Structures                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* FORMAT OF STRUCT
typedef struct { // ID

}__attribute__((packed)) ADCS_***Cmd_Payload_t;
*/


// typedef struct { // ID 7 - Noarg

// }


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                      ADCS Get Cmd Payload Structures                      */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* FORMAT OF STRUCT
typedef struct { // ID

}__attribute__((packed)) ADCS_***Tlm_Payload_t;
*/


typedef struct
{ // ID 204
    uint32 TimeSeconds;
    uint32 TimeNanoSeconds;
    float  GYR0RawRateX;
    float  GYR0RawRateY;
    float  GYR0RawRateZ;
    float  GYR1RawRateX;
    float  GYR1RawRateY;
    float  GYR1RawRateZ;
    uint8   GYR0ValidFlag:1; // GYR0, 1
    uint8   GYR1ValidFlag:1; // GYR1, 0
} __attribute__((packed)) ADCS_RawGYRSensorTlm_Paylaod_t;

typedef struct
{ // ID 205
    uint32 TimeSeconds;
    uint32 TimeNanoSeconds;
    float  RWL0MeasuredSpeed;
    float  RWL1MeasuredSpeed;
    float  RWL2MeasuredSpeed;
    float  RWL3MeasuredSpeed;
    uint8  RWL0ValidFlag : 1;
    uint8  RWL1ValidFlag : 1;
    uint8  RWL2ValidFlag : 1;
    uint8  RWL3ValidFlag : 1;
} __attribute__((packed)) ADCS_RawRWLSensorTlm_Payload_t;


/* BEE1012 combines the raw and calibrated sensor replies for CC 60/79/80/90.
 * UELYSYS returns only the raw reply for those raw-sensor command codes. */
typedef struct
{
    uint32 TimeSeconds;
    uint32 TimeNanoSeconds;
    int16  FSS0CalVecX;
    int16  FSS0CalVecY;
    int16  FSS0CalVecZ;
    int16  FSS1CalVecX;
    int16  FSS1CalVecY;
    int16  FSS1CalVecZ;
    int16  FSS2CalVecX;
    int16  FSS2CalVecY;
    int16  FSS2CalVecZ;
    int16  FSS3CalVecX;
    int16  FSS3CalVecY;
    int16  FSS3CalVecZ;
    uint8  FSS0ValidFlag : 1;
    uint8  FSS1ValidFlag : 1;
    uint8  FSS2ValidFlag : 1;
    uint8  FSS3ValidFlag : 1;
    uint8  FSS0BestFlag : 1;
    uint8  FSS1BestFlag : 1;
    uint8  FSS2BestFlag : 1;
    uint8  FSS3BestFlag : 1;
} __attribute__((packed)) ADCS_CalibratedFSSSensorTlm_Payload_t;

typedef struct
{
    uint32 TimeSeconds;
    uint32 TimeNanoSeconds;
    int16  CSSCalUnitVecX;
    int16  CSSCalUnitVecY;
    int16  CSSCalUnitVecZ;
    uint8  CSSValidFlag : 1;
} __attribute__((packed)) ADCS_CalibratedCSSSensorTlm_Payload_t;

typedef struct
{
    uint32 TimeSeconds;
    uint32 TimeNanoSeconds;
    float  WhlSBCTrqX;
    float  WhlSBCTrqY;
    float  WhlSBCTrqZ;
    float  WhlSBCMomX;
    float  WhlSBCMomY;
    float  WhlSBCMomZ;
    uint8  RWLValidFlag : 1;
} __attribute__((packed)) ADCS_CalibratedRWLSensorTlm_Payload_t;

typedef struct
{
    ADCS_RawCubeSenseSunTlm_Payload_t Raw;
    ADCS_CalibratedFSSSensorTlm_Payload_t Calibrated;
} __attribute__((packed)) ADCS_RawCalibratedFSSReport_Payload_t;

typedef struct
{
    ADCS_RawCSSSensorTlm_Payload_t Raw;
    ADCS_CalibratedCSSSensorTlm_Payload_t Calibrated;
} __attribute__((packed)) ADCS_RawCalibratedCSSReport_Payload_t;

typedef struct
{
    ADCS_RawGYRSensorTlm_Paylaod_t Raw;
    ADCS_CalibratedGYRSensorTlm_Payload_t Calibrated;
} __attribute__((packed)) ADCS_RawCalibratedGYRReport_Payload_t;

typedef struct
{
    ADCS_RawRWLSensorTlm_Payload_t Raw;
    ADCS_CalibratedRWLSensorTlm_Payload_t Calibrated;
} __attribute__((packed)) ADCS_RawCalibratedRWLReport_Payload_t;

typedef struct
{
    uint16 DurationSec;
    uint16 SamplesWritten;
    uint16 ReadErrors;
    uint8  GyroSource;
} __attribute__((packed)) ADCS_GyroLogTlm_Payload_t;

static_assert(sizeof(ADCS_RawRWLSensorTlm_Payload_t) == 25,
              "ADCS_RawRWLSensorTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_CalibratedFSSSensorTlm_Payload_t) == 33,
              "ADCS_CalibratedFSSSensorTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_CalibratedCSSSensorTlm_Payload_t) == 15,
              "ADCS_CalibratedCSSSensorTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_CalibratedRWLSensorTlm_Payload_t) == 33,
              "ADCS_CalibratedRWLSensorTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_RawCalibratedFSSReport_Payload_t) == 66,
              "ADCS_RawCalibratedFSSReport_Payload_t size mismatch");
static_assert(sizeof(ADCS_RawCalibratedCSSReport_Payload_t) == 34,
              "ADCS_RawCalibratedCSSReport_Payload_t size mismatch");
static_assert(sizeof(ADCS_RawCalibratedGYRReport_Payload_t) == 90,
              "ADCS_RawCalibratedGYRReport_Payload_t size mismatch");
static_assert(sizeof(ADCS_RawCalibratedRWLReport_Payload_t) == 58,
              "ADCS_RawCalibratedRWLReport_Payload_t size mismatch");
static_assert(sizeof(ADCS_GyroLogTlm_Payload_t) == 7,
              "ADCS_GyroLogTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_WhlConfigTlm_Payload_t) == 62,
              "ADCS_WhlConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_SatelliteConfigTlm_Payload_t) == 42,
              "ADCS_SatelliteConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_ControllerConfigTlm_Payload_t) == 98,
              "ADCS_ControllerConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_DefaultModeConfigTlm_Payload_t) == 4,
              "ADCS_DefaultModeConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_MountingConfigTlm_Payload_t) == 104,
              "ADCS_MountingConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_EstimatorConfigTlm_Payload_t) == 40,
              "ADCS_EstimatorConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_SatOrbitParamConfigTlm_Payload_t) == 64,
              "ADCS_SatOrbitParamConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_NodeSelectionConfigTlm_Payload_t) == 8,
              "ADCS_NodeSelectionConfigTlm_Payload_t size mismatch");
static_assert(sizeof(ADCS_MTQConfigTlm_Payload_t) == 20,
              "ADCS_MTQConfigTlm_Payload_t size mismatch");


/*************************************
 * CubeADCS Event Entry
 *************************************/

/*************************************
 * CubeADCS Frame Something
 *************************************/


typedef struct
{
    uint8 TransportType;
} __attribute__((packed)) ADCS_InterfaceTransportCmd_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
    ADCS_InterfaceTransportCmd_Payload_t Payload;
} __attribute__((packed)) ADCS_InterfaceTransportCmd_t;


/*************************************************************************/
/*
** Type definition (Housekeeping)
*/


/********************************************************
 * 
 * COSMIC Actual Set Command structure
 * Upper functions are just the references
 * 
 ********************************************************/


/********************************************************
 * 
 * COSMIC Actual Get Command structure
 * Everything is No arguments
 * 
 ********************************************************/


// /********************************************************
//  * 
//  * ADCS Telemetry Msg structure
//  * 
//  ********************************************************/
// /* Beacon SB MSG */
// typedef struct
// {
//     CFE_MSG_TelemetryHeader_t  TelemetryHeader; /**< \brief Telemetry header */
//     ADCS_BcnTlm_Payload_t Payload;         /**< \brief Telemetry payload */
//     bool IsSunlight;
// } ADCS_BcnTlm_t;

// /* Housekeeping SB MSG */
// typedef struct
// {
//     CFE_MSG_TelemetryHeader_t  TelemetryHeader; /**< \brief Telemetry header */
//     ADCS_HkTlm_Payload_t Payload;         /**< \brief Telemetry payload */
// } ADCS_HkTlm_t;

// /* Report SB MSG */
// typedef struct {
//     CFE_MSG_TelemetryHeader_t TelemetryHeader;
//     RPT_Report_t Report;
// } ADCS_ReportTlm_t;


/********************************************************
 * 
 * ADCS Additional Msg structure
 * 
 ********************************************************/
/* Detumbling */

/* Velocity Pointing */

/* KissCAM EARTH Pointing */

/* LG CAM EARTH Pointing */

/* GS-based RPY Pointing */


/*******************************************/
/*                                         */
/*                 EPS  (5th)              */
/*                                         */
/*******************************************/
// msg id

#define EPS_RPT_TLM_ID        0x0877


#define EPS_P80_POWER_IF_GET_CC       10
#define EPS_P80_POWER_IF_SET_CC       11
#define EPS_P80_POWER_IF_LIST_CC      12

#define EPS_GET_HK_CC                 20
#define EPS_GET_HK_ALL_CC             21

#define EPS_P80_GND_WDT_CLEAR_CC      30
#define EPS_P80_GND_WDT_CLEAR_ALL_CC  31

#define EPS_RPARAM_GET_CC             40
#define EPS_RPARAM_SET_CC             41
#define EPS_RPARAM_GET_FULL_TABLE_CC  42

#define EPS_RPARAM_SAVE_ALL_CC        50
#define EPS_RPARAM_TABLE_SAVE_CC      51
#define EPS_RPARAM_TABLE_LOAD_CC      52
#define EPS_RPARAM_SAVE_TO_STORE_CC   53
#define EPS_RPARAM_LOAD_FROM_STORE_CC 54

#define EPS_REPORT_BCN_CC             70

#define EPS_CSP_PING_CC               80
#define EPS_CSP_REBOOT_CC             81
#define EPS_CSP_PS_CC                 82
#define EPS_CSP_MEMFREE_CC            83
#define EPS_CSP_BUF_FREE_CC           84
#define EPS_CSP_UPTIME_CC             85

#define EPS_P80_POWER_IF_NAME_LEN 8

#define GPIO_CMD_ID                   0x182B

#define GPIO_DEP1_EN_ON_CC            6
#define GPIO_DEP1_EN_OFF_CC           7
#define GPIO_DEP2_EN_ON_CC            8
#define GPIO_DEP2_EN_OFF_CC           9
#define GPIO_SP_IN_READ_5S_CC         10

#define GPIO_PACK   __attribute__((packed))
#define EPS_BCN_ACU_COUNT 2
#define EPS_BCN_NODE_COUNT 5
#define EPS_BCN_NODE_PMU_INDEX 0u
#define EPS_BCN_NODE_PDU_INDEX 1u
#define EPS_BCN_NODE_ACU1_INDEX 2u
#define EPS_BCN_NODE_ACU2_INDEX 3u
#define EPS_BCN_NODE_BP8_INDEX 4u
#define EPS_BCN_NODE_MASK(index) ((uint8)(1u << (index)))
#define EPS_BCN_INVALID_FILL 0x00u
#define EPS_RPARAM_DATA_MAX_LEN 128
/* Matches libgscsp RPARAM store/slot fields: 25 chars plus NUL. */
#define EPS_RPARAM_STORE_NAME_LEN 26
#define EPS_RPARAM_STORE_SLOT_LEN 26

#define RPT_RET_VALUE_BUF_SIZE  512
#define EPS_QUERY_REPORT_HEADER_SIZE   12
#define EPS_QUERY_REPORT_DATA_MAX_LEN  (RPT_RET_VALUE_BUF_SIZE - EPS_QUERY_REPORT_HEADER_SIZE)

typedef enum
{
    EPS_QUERY_REPORT_P80_POWER_IF_STATUS = 1,
    EPS_QUERY_REPORT_P80_POWER_IF_LIST   = 2,
    EPS_QUERY_REPORT_P80_PMU_HK          = 3,
    EPS_QUERY_REPORT_P80_PDU_HK          = 4,
    EPS_QUERY_REPORT_P80_ACU_HK          = 5,
    EPS_QUERY_REPORT_BP8_HK              = 6,
    EPS_QUERY_REPORT_RPARAM_VALUE        = 7,
    EPS_QUERY_REPORT_RPARAM_TABLE_ROWS   = 8,
    EPS_QUERY_REPORT_RPARAM_TABLE_MEMORY = 9,
    EPS_QUERY_REPORT_CSP_PING_MS         = 10,
    EPS_QUERY_REPORT_CSP_PS_TEXT         = 11,
    EPS_QUERY_REPORT_CSP_MEMFREE         = 12,
    EPS_QUERY_REPORT_CSP_BUF_FREE        = 13,
    EPS_QUERY_REPORT_CSP_UPTIME          = 14,
    EPS_QUERY_REPORT_HK_ALL              = 15
} EPS_Query_Report_DataId_t;

/**********************************************************TC************************************************************/

typedef struct EPS_PACK
{
    uint8  source;      /* CSP node for hardware query reports. */
    uint8  data_id;     /* EPS_Query_Report_DataId_t */
    uint8  arg0;        /* Command-specific: RPARAM table id when applicable. */
    uint8  arg1;        /* Command-specific: RPARAM type when applicable. */
    uint16 sequence;    /* Chunk sequence, starting at 0. */
    uint16 offset;      /* Byte offset into the full command result. */
    uint16 total_size;  /* Full result size in bytes. */
    uint16 chunk_size;  /* Valid bytes in data[]. */
    uint8  data[EPS_QUERY_REPORT_DATA_MAX_LEN];
} EPS_Query_Report_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node; //PMU or PDU
    uint8_t mode;
    uint16_t on_cnt;
    uint16_t off_cnt;
    char    name[EPS_P80_POWER_IF_NAME_LEN];
}EPS_P80_Power_If_Set_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node; //PMU or PDU
    char    name[EPS_P80_POWER_IF_NAME_LEN];
}EPS_P80_Power_If_Get_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node; //PMU or PDU
}EPS_P80_Power_If_List_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node; //PMU or PDU or ACU1 or ACU2
}EPS_P80_Gnd_Watchdog_Clear_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node; //PMU or PDU or ACU1 or ACU2 or BP8
}EPS_Get_HK_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
    uint8_t table_id;
    uint16_t    addr;
    uint8_t     type; 
    uint8_t     data[EPS_RPARAM_DATA_MAX_LEN];
    uint16_t    size;
}EPS_RParam_Set_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
    uint8_t table_id;
    uint16_t    addr;
    uint8_t     type;
    uint16_t    size;
}EPS_RParam_Get_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
    uint8_t table_id;
}EPS_RParam_Get_Full_Table_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_RParam_Save_All_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
    uint8_t table_id;
}EPS_RParam_Table_Save_Cmd_Payload_t;

typedef EPS_RParam_Table_Save_Cmd_Payload_t EPS_RParam_Table_Load_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
    uint8_t table_id;
    char    store[EPS_RPARAM_STORE_NAME_LEN];
    char    slot[EPS_RPARAM_STORE_SLOT_LEN];
}EPS_RParam_Store_Cmd_Payload_t;

typedef EPS_RParam_Store_Cmd_Payload_t EPS_RParam_Save_To_Store_Cmd_Payload_t;
typedef EPS_RParam_Store_Cmd_Payload_t EPS_RParam_Load_From_Store_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t  csp_node;
    uint16_t size;
    uint8_t  opts;
}EPS_CSP_Ping_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_CSP_Reboot_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_CSP_PS_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_CSP_MemFree_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_CSP_BufFree_Cmd_Payload_t;

typedef struct EPS_PACK {
    uint8_t csp_node;
}EPS_CSP_Uptime_Cmd_Payload_t;


typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_P80_Power_If_Get_Cmd_Payload_t Payload;
}EPS_P80_Power_If_Get_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_P80_Power_If_Set_Cmd_Payload_t Payload;
}EPS_P80_Power_If_Set_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_P80_Power_If_List_Cmd_Payload_t Payload;
}EPS_P80_Power_If_List_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_Get_HK_Cmd_Payload_t Payload;
}EPS_Get_HK_Cmd_t;

typedef EPS_NoArgCmd_t EPS_Get_HK_All_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_P80_Gnd_Watchdog_Clear_Cmd_Payload_t Payload;
}EPS_P80_Gnd_Watchdog_Clear_Cmd_t;

typedef EPS_NoArgCmd_t EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Set_Cmd_Payload_t Payload;
}EPS_RParam_Set_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Get_Cmd_Payload_t Payload;
}EPS_RParam_Get_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Get_Full_Table_Cmd_Payload_t Payload;
}EPS_RParam_Get_Full_Table_Cmd_t;


typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Table_Save_Cmd_Payload_t Payload;
}EPS_RParam_Table_Save_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Table_Load_Cmd_Payload_t Payload;
}EPS_RParam_Table_Load_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Save_To_Store_Cmd_Payload_t Payload;
}EPS_RParam_Save_To_Store_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Load_From_Store_Cmd_Payload_t Payload;
}EPS_RParam_Load_From_Store_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_RParam_Save_All_Cmd_Payload_t Payload;
}EPS_RParam_Save_All_Cmd_t;

typedef EPS_NoArgCmd_t  EPS_ReportBcnCmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_PS_Cmd_Payload_t Payload;
}EPS_CSP_PS_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_MemFree_Cmd_Payload_t Payload;
}EPS_CSP_MemFree_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_BufFree_Cmd_Payload_t Payload;
}EPS_CSP_BufFree_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_Uptime_Cmd_Payload_t Payload;
}EPS_CSP_Uptime_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_Ping_Cmd_Payload_t Payload;
}EPS_CSP_Ping_Cmd_t;

typedef struct EPS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    EPS_CSP_Reboot_Cmd_Payload_t Payload;
}EPS_CSP_Reboot_Cmd_t;

typedef struct GPIO_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
} GPIO_NoArgCmd_t;

typedef GPIO_NoArgCmd_t GPIO_Dep1EnOnCmd_t;
typedef GPIO_NoArgCmd_t GPIO_Dep1EnOffCmd_t;
typedef GPIO_NoArgCmd_t GPIO_Dep2EnOnCmd_t;
typedef GPIO_NoArgCmd_t GPIO_Dep2EnOffCmd_t;
typedef GPIO_NoArgCmd_t GPIO_SpInRead5sCmd_t;

/*******************************************/
/*              UELYSYS TC                 */
/*******************************************/

#define UELYSYS_TTC_CMD_ID              0x1880
#define UTRX_CMD_ID                     0x1850
#define UTRX_RESET_COUNTERS_CC          1
#define UTRX_RESET_APP_CMD_COUNTERS_CC  2
#define UTRX_RESET_DEVICE_CMD_COUNTERS_CC 3
#define UTRX_REBOOT_CC                  7
#define UTRX_SET_DEFAULT_BAUD_CC       10
#define UTRX_RPARAM_SAVE_1_CC          11
#define UTRX_RPARAM_SAVE_5_CC          12
#define UTRX_RPARAM_SAVE_ALL_CC        13
#define UTRX_CHECK_STATE_PING_CC       16
#define UTRX_RXCONF_GET_GUARD_CC       21
#define UTRX_TXCONF_GET_BAUD_CC        22
#define UTRX_TLM_GET_TEMP_BRD_CC       23
#define UTRX_TLM_GET_LAST_RSSI_CC      24
#define UTRX_TLM_GET_LAST_RFERR_CC     25
#define UTRX_TLM_GET_ACTIVE_CONF_CC    26
#define UTRX_TLM_GET_BOOT_COUNT_CC     27
#define UTRX_TLM_GET_BOOT_CAUSE_CC     28
#define UTRX_TLM_GET_LAST_CONTACT_CC   29
#define UTRX_TLM_GET_TOT_TX_BYTES_CC   30
#define UTRX_TLM_GET_TOT_RX_BYTES_CC   31
#define UTRX_RXCONF_GET_FREQ_CC        32
#define UTRX_TXCONF_GET_FREQ_CC        33

#define LTRX_CMD_ID                     0x1896
#define LTRX_NOOP_CC                    0
#define LTRX_RESET_COUNTERS_CC          1
#define LTRX_RESET_APP_CMD_COUNTERS_CC  2
#define LTRX_RESET_DEVICE_CMD_COUNTERS_CC 3
#define LTRX_SET_BUS_BEACON_PERIOD_CC   4
#define LTRX_SESSION_START_DOWNLINK_CC 10
#define LTRX_SESSION_ABORT_CC          11
#define LTRX_SESSION_RESET_STATE_CC    12
#define LTRX_DOWNSTREAM_ENABLE_CC      40
#define LTRX_DOWNSTREAM_DISABLE_CC     41

/*******************************************/
/* BEE1012 cFE core/module telecommands    */
/*******************************************/

/* Final BEE1012 MID values: CFE_CPU1_CMD_MID_BASE (0x1800) | topic ID. */
#define BEE1012_CFE_ES_CMD_ID           0x1800
#define BEE1012_CFE_ES_SEND_HK_ID       0x1801
#define BEE1012_CFE_EVS_CMD_ID          0x1802
#define BEE1012_CFE_EVS_SEND_HK_ID      0x1803
#define BEE1012_CFE_SB_CMD_ID           0x1804
#define BEE1012_CFE_SB_SEND_HK_ID       0x1805
#define BEE1012_CFE_SB_SUB_RPT_CMD_ID   0x1806
#define BEE1012_CFE_TBL_CMD_ID          0x1807
#define BEE1012_CFE_TBL_SEND_HK_ID      0x1808
#define BEE1012_CFE_TIME_CMD_ID         0x1809
#define BEE1012_CFE_TIME_SEND_HK_ID      0x180A
#define BEE1012_CFE_TIME_TONE_CMD_ID     0x180B
#define BEE1012_CFE_TIME_ONEHZ_CMD_ID    0x180C
#define BEE1012_CFE_TIME_DATA_CMD_ID     0x1860
#define BEE1012_CFE_TIME_SEND_CMD_ID     0x1862
#define BEE1012_CFE_SRL_CMD_ID          0x180D
#define BEE1012_CFE_SRL_SEND_HK_ID       0x180E

/* BEE1012 THRUST application (topic ID 0x70). */
#define BEE1012_THRUST_CMD_ID                 0x1870
#define BEE1012_THRUST_NOOP_CC                0
#define BEE1012_THRUST_RESET_COUNTERS_CC      1
#define BEE1012_THRUST_PING_CC                2
#define BEE1012_THRUST_RESET_MODULE_CC        3
#define BEE1012_THRUST_SET_MODE_CC            4
#define BEE1012_THRUST_ARM_CC                5
#define BEE1012_THRUST_DISARM_CC             6
#define BEE1012_THRUST_REQ_STATUS_CC         7
#define BEE1012_THRUST_MAIN_FIRE_CC          8
#define BEE1012_THRUST_MAIN_ABORT_CC         9
#define BEE1012_THRUST_CG_PULSE_CC          10
#define BEE1012_THRUST_CG_ABORT_CC          11
#define BEE1012_THRUST_REQ_FAULT_LOG_CC     12
#define BEE1012_THRUST_CLEAR_FAULT_CC       13
#define BEE1012_THRUST_REQ_HK_CC            14
#define BEE1012_THRUST_SCH_HK_ENABLE_CC     15

#define BEE1012_CFE_ES_NOOP_CC                     0
#define BEE1012_CFE_ES_RESET_COUNTERS_CC           1
#define BEE1012_CFE_ES_RESTART_CC                  2
#define BEE1012_CFE_ES_START_APP_CC                4
#define BEE1012_CFE_ES_STOP_APP_CC                 5
#define BEE1012_CFE_ES_RESTART_APP_CC              6
#define BEE1012_CFE_ES_RELOAD_APP_CC               7
#define BEE1012_CFE_ES_QUERY_ONE_CC                8
#define BEE1012_CFE_ES_QUERY_ALL_CC                9
#define BEE1012_CFE_ES_CLEAR_SYS_LOG_CC           10
#define BEE1012_CFE_ES_WRITE_SYS_LOG_CC           11
#define BEE1012_CFE_ES_CLEAR_ER_LOG_CC            12
#define BEE1012_CFE_ES_WRITE_ER_LOG_CC            13
#define BEE1012_CFE_ES_START_PERF_DATA_CC         14
#define BEE1012_CFE_ES_STOP_PERF_DATA_CC          15
#define BEE1012_CFE_ES_SET_PERF_FILTER_MASK_CC    16
#define BEE1012_CFE_ES_SET_PERF_TRIGGER_MASK_CC   17
#define BEE1012_CFE_ES_OVER_WRITE_SYS_LOG_CC      18
#define BEE1012_CFE_ES_RESET_PR_COUNT_CC          19
#define BEE1012_CFE_ES_SET_MAX_PR_COUNT_CC        20
#define BEE1012_CFE_ES_DELETE_CDS_CC              21
#define BEE1012_CFE_ES_SEND_MEM_POOL_STATS_CC     22
#define BEE1012_CFE_ES_DUMP_CDS_REGISTRY_CC       23
#define BEE1012_CFE_ES_QUERY_ALL_TASKS_CC         24

#define BEE1012_CFE_EVS_NOOP_CC                    0
#define BEE1012_CFE_EVS_RESET_COUNTERS_CC          1
#define BEE1012_CFE_EVS_ENABLE_EVENT_TYPE_CC       2
#define BEE1012_CFE_EVS_DISABLE_EVENT_TYPE_CC      3
#define BEE1012_CFE_EVS_SET_EVENT_FORMAT_MODE_CC   4
#define BEE1012_CFE_EVS_ENABLE_APP_EVENT_TYPE_CC   5
#define BEE1012_CFE_EVS_DISABLE_APP_EVENT_TYPE_CC  6
#define BEE1012_CFE_EVS_ENABLE_APP_EVENTS_CC       7
#define BEE1012_CFE_EVS_DISABLE_APP_EVENTS_CC      8
#define BEE1012_CFE_EVS_RESET_APP_COUNTER_CC       9
#define BEE1012_CFE_EVS_SET_FILTER_CC             10
#define BEE1012_CFE_EVS_ENABLE_PORTS_CC           11
#define BEE1012_CFE_EVS_DISABLE_PORTS_CC          12
#define BEE1012_CFE_EVS_RESET_FILTER_CC           13
#define BEE1012_CFE_EVS_RESET_ALL_FILTERS_CC      14
#define BEE1012_CFE_EVS_ADD_EVENT_FILTER_CC       15
#define BEE1012_CFE_EVS_DELETE_EVENT_FILTER_CC    16
#define BEE1012_CFE_EVS_WRITE_APP_DATA_FILE_CC    17
#define BEE1012_CFE_EVS_WRITE_LOG_DATA_FILE_CC    18
#define BEE1012_CFE_EVS_SET_LOG_MODE_CC           19
#define BEE1012_CFE_EVS_CLEAR_LOG_CC              20

#define BEE1012_CFE_SB_NOOP_CC                     0
#define BEE1012_CFE_SB_RESET_COUNTERS_CC           1
#define BEE1012_CFE_SB_SEND_SB_STATS_CC            2
#define BEE1012_CFE_SB_WRITE_ROUTING_INFO_CC       3
#define BEE1012_CFE_SB_ENABLE_ROUTE_CC             4
#define BEE1012_CFE_SB_DISABLE_ROUTE_CC            5
#define BEE1012_CFE_SB_WRITE_PIPE_INFO_CC          7
#define BEE1012_CFE_SB_WRITE_MAP_INFO_CC           8
#define BEE1012_CFE_SB_ENABLE_SUB_REPORTING_CC     9
#define BEE1012_CFE_SB_DISABLE_SUB_REPORTING_CC   10
#define BEE1012_CFE_SB_SEND_PREV_SUBS_CC          11

#define BEE1012_CFE_TBL_NOOP_CC                    0
#define BEE1012_CFE_TBL_RESET_COUNTERS_CC          1
#define BEE1012_CFE_TBL_LOAD_CC                    2
#define BEE1012_CFE_TBL_DUMP_CC                    3
#define BEE1012_CFE_TBL_VALIDATE_CC                4
#define BEE1012_CFE_TBL_ACTIVATE_CC                5
#define BEE1012_CFE_TBL_DUMP_REGISTRY_CC           6
#define BEE1012_CFE_TBL_SEND_REGISTRY_CC           7
#define BEE1012_CFE_TBL_DELETE_CDS_CC              8
#define BEE1012_CFE_TBL_ABORT_LOAD_CC              9

#define BEE1012_CFE_TIME_NOOP_CC                   0
#define BEE1012_CFE_TIME_RESET_COUNTERS_CC         1
#define BEE1012_CFE_TIME_SEND_DIAGNOSTIC_CC        2
#define BEE1012_CFE_TIME_SET_SOURCE_CC             3
#define BEE1012_CFE_TIME_SET_STATE_CC              4
#define BEE1012_CFE_TIME_ADD_DELAY_CC              5
#define BEE1012_CFE_TIME_SUB_DELAY_CC              6
#define BEE1012_CFE_TIME_SET_TIME_CC               7
#define BEE1012_CFE_TIME_SET_MET_CC                8
#define BEE1012_CFE_TIME_SET_STCF_CC               9
#define BEE1012_CFE_TIME_SET_LEAP_SECONDS_CC      10
#define BEE1012_CFE_TIME_ADD_ADJUST_CC            11
#define BEE1012_CFE_TIME_SUB_ADJUST_CC            12
#define BEE1012_CFE_TIME_ADD_ONE_HZ_ADJUSTMENT_CC 13
#define BEE1012_CFE_TIME_SUB_ONE_HZ_ADJUSTMENT_CC 14
#define BEE1012_CFE_TIME_SET_SIGNAL_CC            15

#define BEE1012_CFE_SRL_NOOP_CC                    0
#define BEE1012_CFE_SRL_RESET_COUNTERS_CC          1
#define BEE1012_CFE_SRL_RESET_HANDLE_COUNTERS_CC   2
#define BEE1012_CFE_SRL_GET_HANDLE_STATUS_CC       3
#define BEE1012_CFE_SRL_INIT_HANDLE_CC             4
#define BEE1012_CFE_SRL_CLOSE_HANDLE_CC            5
#define BEE1012_CFE_SRL_CONFIG_HANDLE_CC           6

/* Mission interface sizes used by the BEE1012 build. */
#define BEE1012_CFE_MAX_API_LEN             20
#define BEE1012_CFE_MAX_PATH_LEN            64
#define BEE1012_CFE_ES_CDS_FULL_NAME_LEN    40
#define BEE1012_CFE_TBL_FULL_NAME_LEN       40
#define BEE1012_CFE_SRL_HANDLE_NAME_LEN     16

typedef struct __attribute__((packed)) {
    uint16_t RestartType;
} BEE1012_CFE_ES_Restart_Payload_t;

typedef struct __attribute__((packed)) {
    char Application[BEE1012_CFE_MAX_API_LEN];
    char AppEntryPoint[BEE1012_CFE_MAX_API_LEN];
    char AppFileName[BEE1012_CFE_MAX_PATH_LEN];
    uint32_t StackSize;
    uint8_t ExceptionAction;
    uint8_t AlignPriority;
    uint16_t Priority;
} BEE1012_CFE_ES_StartApp_Payload_t;

typedef struct __attribute__((packed)) {
    char Application[BEE1012_CFE_MAX_API_LEN];
} BEE1012_CFE_AppName_Payload_t;

typedef struct __attribute__((packed)) {
    char Application[BEE1012_CFE_MAX_API_LEN];
    char AppFileName[BEE1012_CFE_MAX_PATH_LEN];
} BEE1012_CFE_ES_ReloadApp_Payload_t;

typedef struct __attribute__((packed)) {
    char FileName[BEE1012_CFE_MAX_PATH_LEN];
} BEE1012_CFE_Path_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t Value;
} BEE1012_CFE_U32_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t First;
    uint32_t Second;
} BEE1012_CFE_U32Pair_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t Value;
} BEE1012_CFE_U16_Payload_t;

typedef struct __attribute__((packed)) {
    int16_t Value;
} BEE1012_CFE_I16_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t AtToneMETSeconds;
    uint32_t AtToneMETSubseconds;
    uint32_t AtToneSTCFSeconds;
    uint32_t AtToneSTCFSubseconds;
    int16_t AtToneLeapSeconds;
    int16_t AtToneState;
} BEE1012_CFE_TIME_ToneData_Payload_t;

typedef struct __attribute__((packed)) {
    char CdsName[BEE1012_CFE_ES_CDS_FULL_NAME_LEN];
} BEE1012_CFE_ES_DeleteCDS_Payload_t;

typedef struct __attribute__((packed)) {
    char Application[BEE1012_CFE_MAX_API_LEN];
    uint32_t PoolHandle;
} BEE1012_CFE_ES_SendMemPoolStats_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t Value;
    uint8_t Spare;
} BEE1012_CFE_U8Spare_Payload_t;

typedef struct __attribute__((packed)) {
    char AppName[BEE1012_CFE_MAX_API_LEN];
    uint8_t BitMask;
    uint8_t Spare;
} BEE1012_CFE_EVS_AppBitMask_Payload_t;

typedef struct __attribute__((packed)) {
    char AppName[BEE1012_CFE_MAX_API_LEN];
    uint16_t EventID;
} BEE1012_CFE_EVS_AppEvent_Payload_t;

typedef struct __attribute__((packed)) {
    char AppName[BEE1012_CFE_MAX_API_LEN];
    uint16_t EventID;
    uint16_t Mask;
} BEE1012_CFE_EVS_AppEventMask_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t MsgId;
    uint32_t Pipe;
    uint8_t Spare;
    uint8_t Align[3];
} BEE1012_CFE_SB_Route_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t ActiveTableFlag;
    char TableName[BEE1012_CFE_TBL_FULL_NAME_LEN];
    char DumpFilename[BEE1012_CFE_MAX_PATH_LEN];
} BEE1012_CFE_TBL_Dump_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t ActiveTableFlag;
    char TableName[BEE1012_CFE_TBL_FULL_NAME_LEN];
} BEE1012_CFE_TBL_Validate_Payload_t;

typedef struct __attribute__((packed)) {
    char TableName[BEE1012_CFE_TBL_FULL_NAME_LEN];
} BEE1012_CFE_TBL_Name_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t Indexer;
} BEE1012_CFE_SRL_Index_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t Indexer;
    char Name[BEE1012_CFE_SRL_HANDLE_NAME_LEN];
    char DevName[BEE1012_CFE_SRL_HANDLE_NAME_LEN];
    uint8_t DevType;
    uint8_t Padding[3];
} BEE1012_CFE_SRL_InitHandle_Payload_t;

/* The deployed OBC is 32-bit ARM: its packed serial cfg is FD + 12-byte union. */
typedef struct __attribute__((packed)) {
    uint32_t Indexer;
    uint8_t FD;
    uint8_t Config[12];
    uint8_t Align[3];
} BEE1012_CFE_SRL_ConfigHandle_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t Payload[112];
} BEE1012_CFE_CoreCmdBuffer_t;

typedef struct __attribute__((packed)) {
    uint32_t TargetMode;
} BEE1012_THRUST_TargetMode_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t ArmKey;
    uint16_t TimeoutSec;
} BEE1012_THRUST_Arm_Payload_t;

typedef struct __attribute__((packed)) {
    uint32_t FireTime_ms;
    uint16_t IgnitionTime_ms;
    uint16_t FireKey;
} BEE1012_THRUST_MainFire_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t ThrusterId;
    uint32_t PulseWidth_ms;
    uint16_t FireKey;
} BEE1012_THRUST_CGPulse_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t ScheduledHkEnabled;
} BEE1012_THRUST_ScheduledHkEnable_Payload_t;

static_assert(sizeof(BEE1012_CFE_ES_Restart_Payload_t) == 2, "CFE_ES restart payload size mismatch");
static_assert(sizeof(BEE1012_CFE_ES_StartApp_Payload_t) == 112, "CFE_ES start-app payload size mismatch");
static_assert(sizeof(BEE1012_CFE_AppName_Payload_t) == 20, "cFE app-name payload size mismatch");
static_assert(sizeof(BEE1012_CFE_ES_ReloadApp_Payload_t) == 84, "CFE_ES reload-app payload size mismatch");
static_assert(sizeof(BEE1012_CFE_Path_Payload_t) == 64, "cFE pathname payload size mismatch");
static_assert(sizeof(BEE1012_CFE_U32_Payload_t) == 4, "cFE uint32 payload size mismatch");
static_assert(sizeof(BEE1012_CFE_U32Pair_Payload_t) == 8, "cFE uint32-pair payload size mismatch");
static_assert(sizeof(BEE1012_CFE_U16_Payload_t) == 2, "cFE uint16 payload size mismatch");
static_assert(sizeof(BEE1012_CFE_I16_Payload_t) == 2, "cFE int16 payload size mismatch");
static_assert(sizeof(BEE1012_CFE_TIME_ToneData_Payload_t) == 20, "CFE_TIME tone-data payload size mismatch");
static_assert(sizeof(BEE1012_CFE_ES_DeleteCDS_Payload_t) == 40, "CFE_ES delete-CDS payload size mismatch");
static_assert(sizeof(BEE1012_CFE_ES_SendMemPoolStats_Payload_t) == 24, "CFE_ES pool-stats payload size mismatch");
static_assert(sizeof(BEE1012_CFE_U8Spare_Payload_t) == 2, "cFE uint8/spare payload size mismatch");
static_assert(sizeof(BEE1012_CFE_EVS_AppBitMask_Payload_t) == 22, "CFE_EVS app/bitmask payload size mismatch");
static_assert(sizeof(BEE1012_CFE_EVS_AppEvent_Payload_t) == 22, "CFE_EVS app/event payload size mismatch");
static_assert(sizeof(BEE1012_CFE_EVS_AppEventMask_Payload_t) == 24, "CFE_EVS filter payload size mismatch");
static_assert(sizeof(BEE1012_CFE_SB_Route_Payload_t) == 12, "CFE_SB route payload size mismatch");
static_assert(sizeof(BEE1012_CFE_TBL_Dump_Payload_t) == 106, "CFE_TBL dump payload size mismatch");
static_assert(sizeof(BEE1012_CFE_TBL_Validate_Payload_t) == 42, "CFE_TBL validate payload size mismatch");
static_assert(sizeof(BEE1012_CFE_TBL_Name_Payload_t) == 40, "CFE_TBL name payload size mismatch");
static_assert(sizeof(BEE1012_CFE_SRL_Index_Payload_t) == 4, "CFE_SRL index payload size mismatch");
static_assert(sizeof(BEE1012_CFE_SRL_InitHandle_Payload_t) == 40, "CFE_SRL init payload size mismatch");
static_assert(sizeof(BEE1012_CFE_SRL_ConfigHandle_Payload_t) == 20, "CFE_SRL config payload size mismatch");
static_assert(sizeof(BEE1012_CFE_CoreCmdBuffer_t) == 120, "cFE core command buffer size mismatch");
static_assert(sizeof(BEE1012_THRUST_TargetMode_Payload_t) == 4, "THRUST target-mode payload size mismatch");
static_assert(sizeof(BEE1012_THRUST_Arm_Payload_t) == 4, "THRUST arm payload size mismatch");
static_assert(sizeof(BEE1012_THRUST_MainFire_Payload_t) == 8, "THRUST main-fire payload size mismatch");
static_assert(sizeof(BEE1012_THRUST_CGPulse_Payload_t) == 8, "THRUST CG-pulse payload size mismatch");
static_assert(sizeof(BEE1012_THRUST_ScheduledHkEnable_Payload_t) == 1,
              "THRUST scheduled-HK-enable payload size mismatch");

#define BEE1012_CI_LAB_CMD_ID           0x1810
#define BEE1012_CI_LAB_NOOP_CC          0
#define BEE1012_CI_LAB_RESET_COUNTERS_CC 1
#define BEE1012_CI_LAB_CREATE_CHILD_TASK_CC 2
#define BEE1012_DS_CMD_ID               0x1813
#define BEE1012_DS_NOOP_CC              0
#define BEE1012_DS_RESET_COUNTERS_CC    1
#define BEE1012_DS_GET_FILE_INFO_CC    15
#define BEE1012_DS_CLOSE_ALL_CC        17
#define BEE1012_FM_CMD_ID               0x1815
#define BEE1012_FM_NOOP_CC              0
#define BEE1012_FM_RESET_COUNTERS_CC    1
#define BEE1012_FM_GET_OPEN_FILES_CC   11
#define BEE1012_FM_MONITOR_FILESYSTEM_SPACE_CC 16
#define BEE1012_GPS_CMD_ID              0x1894
#define BEE1012_GPS_NOOP_CC             0
#define BEE1012_HK_CMD_ID               0x1817
#define BEE1012_HK_NOOP_CC              0
#define BEE1012_HK_RESET_COUNTERS_CC    1
#define BEE1012_MISSION_CMD_ID          0x1890
#define BEE1012_MISSION_NOOP_CC         0
#define BEE1012_MISSION_RESET_COUNTER_CC 1
#define BEE1012_MISSION_APPS_PERM_OFF_CC 2
#define BEE1012_RPT_CMD_ID              0x1825
#define BEE1012_RPT_NOOP_CC             0
#define BEE1012_RPT_RESET_COUNTER_CC    1
#define BEE1012_RPT_GET_OPS_DATA_CC     4
#define BEE1012_SCH_LAB_CMD_ID          0x18A3
#define BEE1012_SCH_LAB_NOOP_CC         0
#define BEE1012_ADCS2_CMD_ID            0x1865
#define BEE1012_ADCS2_NOOP_CC           0
#define BEE1012_PAY_SLT_CMD_ID                   0x1830
#define BEE1012_PAY_SLT_NOOP_CC                  0
#define BEE1012_PAY_SLT_RESET_COUNTERS_CC        1
#define BEE1012_PAY_SLT_REPORT_BCN_CC            2
#define BEE1012_PAY_SLT_OUTPUT_ENABLED_CC        3
#define BEE1012_PAY_SLT_RS422_PING_CC            4
#define BEE1012_PAY_SLT_PAR_GET_CC               5
#define BEE1012_PAY_SLT_PAR_SET_CC               6
#define BEE1012_PAY_SLT_SCAN_FILES_CC            7
#define BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC     8
#define BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC   9
#define BEE1012_PAY_SLT_GET_FULL_TABLE_CC       10
#define BEE1012_PAY_SLT_PAR_SET_ARRAY_CC        11

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
} __attribute__((packed)) BEE1012_PAY_SLT_NoArgsCmd_t;

typedef struct
{
    uint8_t HkEnabled;
    uint8_t BcnEnabled;
    uint8_t IfbEnabled;
    uint8_t ExpEnabled;
} __attribute__((packed)) BEE1012_PAY_SLT_OutputEnabled_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_OutputEnabled_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_OutputEnabledCmd_t;

typedef struct
{
    uint8_t  node;
    uint8_t  table;
    uint16_t addr;
    uint8_t  type;
    uint8_t  len;
} __attribute__((packed)) BEE1012_PAY_SLT_ParGet_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_ParGet_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_ParGetCmd_t;

typedef union
{
    uint32_t value;
    char     str[96];
} BEE1012_PAY_SLT_ParSet_Data_t;

typedef struct
{
    uint8_t  node;
    uint8_t  table;
    uint16_t addr;
    uint8_t  type;
    uint8_t  padding[3];
    BEE1012_PAY_SLT_ParSet_Data_t data;
} __attribute__((packed)) BEE1012_PAY_SLT_ParSet_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_ParSet_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_ParSetCmd_t;

#define BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN 8

typedef union
{
    uint8_t  u8[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN];
    uint16_t u16[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN];
    uint32_t u32[BEE1012_PAY_SLT_PAR_SET_ARRAY_MAX_LEN];
} BEE1012_PAY_SLT_ParSetArray_Data_t;

typedef struct
{
    uint8_t  node;
    uint8_t  table;
    uint16_t addr;
    uint8_t  type;
    uint8_t  len;
    uint8_t  padding[2];
    BEE1012_PAY_SLT_ParSetArray_Data_t data;
} __attribute__((packed)) BEE1012_PAY_SLT_ParSetArray_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_ParSetArray_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_ParSetArrayCmd_t;

static_assert(sizeof(BEE1012_PAY_SLT_OutputEnabled_Payload_t) == 4,
              "PAY_SLT output-enabled payload size mismatch");
static_assert(sizeof(BEE1012_PAY_SLT_ParSetArray_Payload_t) == 40,
              "PAY_SLT parameter-array payload size mismatch");
static_assert(sizeof(BEE1012_PAY_SLT_ParSetArrayCmd_t) == 48,
              "PAY_SLT parameter-array command size mismatch");

typedef struct
{
    uint8_t node;
} __attribute__((packed)) BEE1012_PAY_SLT_ScanFiles_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_ScanFiles_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_ScanFilesCmd_t;

typedef struct
{
    uint32_t file_index;
    uint32_t start_chunk;
} __attribute__((packed)) BEE1012_PAY_SLT_DownloadFile_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_DownloadFile_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_DownloadFileCmd_t;

typedef struct
{
    uint8_t node;
    uint8_t table;
} __attribute__((packed)) BEE1012_PAY_SLT_GetFullTable_Payload_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    BEE1012_PAY_SLT_GetFullTable_Payload_t Payload;
} __attribute__((packed)) BEE1012_PAY_SLT_GetFullTableCmd_t;
#define BEE1012_MEOW_CMD_ID             0x1886
#define UELYSYS_MEOW_CMD_ID             0x1886
#define UELYSYS_UTRX_CMD_ID             0x1850
#define UELYSYS_UANT_CMD_ID             0x1840
#define UELYSYS_STX_CMD_ID              0x1854
#define UELYSYS_GPIO_CMD_ID             0x188A
#define UELYSYS_PAYUEL_OBC_CMD_ID       0x18C0
#define UELYSYS_PAYUEL_CAM_CMD_ID       0x18C3
#define UELYSYS_LGBAT_CMD_ID            0x18C6
#define UELYSYS_PAYUEL_AOS_CMD_ID       0x1847

/* PAYYONSEI payload-OBC target applications (CSP node 13). */
#define UELYSYS_PAYYONSEI_TEMP_CMD_ID       0x1892
#define UELYSYS_PAYYONSEI_TEMP_SEND_HK_ID   0x1893
#define UELYSYS_PAYYONSEI_IMU_CMD_ID        0x1894
#define UELYSYS_PAYYONSEI_IMU_SEND_HK_ID    0x1895
#define UELYSYS_PAYYONSEI_IMU_WAKEUP_ID     0x1896
#define UELYSYS_PAYYONSEI_IWSB_CMD_ID       0x189A
#define UELYSYS_PAYYONSEI_IWSB_SEND_HK_ID   0x189B

#define UELYSYS_PAYYONSEI_TEMP_NOOP_CC            0
#define UELYSYS_PAYYONSEI_TEMP_RESET_COUNTERS_CC  1
#define UELYSYS_PAYYONSEI_TEMP_READ_TEMP_CC       2
#define UELYSYS_PAYYONSEI_TEMP_SET_CONFIG_CC      3
#define UELYSYS_PAYYONSEI_TEMP_READ_CONFIG_CC     4
#define UELYSYS_PAYYONSEI_TEMP_SET_LIMITS_CC      5
#define UELYSYS_PAYYONSEI_TEMP_READ_LIMITS_CC     6
#define UELYSYS_PAYYONSEI_TEMP_SET_RESOLUTION_CC  7
#define UELYSYS_PAYYONSEI_TEMP_READ_ID_CC         8
#define UELYSYS_PAYYONSEI_TEMP_START_LOG_CC       10

#define UELYSYS_PAYYONSEI_IMU_NOOP_CC             0
#define UELYSYS_PAYYONSEI_IMU_RESET_COUNTERS_CC   1
#define UELYSYS_PAYYONSEI_IMU_RESET_MODULE_CC     2
#define UELYSYS_PAYYONSEI_IMU_SEND_DATA_CC        3
#define UELYSYS_PAYYONSEI_IMU_RUN_SELFTEST_CC     4
#define UELYSYS_PAYYONSEI_IMU_SAVE_LOG_FILE_CC    5
#define UELYSYS_PAYYONSEI_IMU_START_LOGGING_CC    6
#define UELYSYS_PAYYONSEI_IMU_MEASURE_CC          7
#define UELYSYS_PAYYONSEI_IMU_STOP_LOGGING_CC     8

#define UELYSYS_PAYYONSEI_IWSB_NOOP_CC            0
#define UELYSYS_PAYYONSEI_IWSB_RESET_COUNTERS_CC  1
#define UELYSYS_PAYYONSEI_IWSB_MISSION_START_CC   2
#define UELYSYS_PAYYONSEI_IWSB_ERASE_SLOT_CC      3
#define UELYSYS_PAYYONSEI_IWSB_WRITE_FW_CC        4
#define UELYSYS_PAYYONSEI_IWSB_VERIFY_SLOT_CC     5
#define UELYSYS_PAYYONSEI_IWSB_UPDATE_FW_CC       6
#define UELYSYS_PAYYONSEI_IWSB_GET_CURRENT_FW_CC  7
#define UELYSYS_PAYYONSEI_IWSB_GET_THERMAL_CC     8

/* PAYYONSEI I2C bus-health monitor (topic 0x70 -> MID 0x1870). */
#define UELYSYS_PAYYONSEI_I2C_CMD_ID              0x1870
#define UELYSYS_PAYYONSEI_I2C_NOOP_CC             0
#define UELYSYS_PAYYONSEI_I2C_RESET_COUNTERS_CC   1
#define UELYSYS_PAYYONSEI_I2C_SET_THRESHOLD_CC    2
#define UELYSYS_PAYYONSEI_I2C_CLEAR_LATCH_CC      3

/* SDR controller target application (CSP node 14). */
#define UELYSYS_SDR_CONTROLLER_CMD_ID                 0x1840
#define UELYSYS_SDR_CONTROLLER_NOOP_CC                0
#define UELYSYS_SDR_CONTROLLER_RESET_COUNTERS_CC      1
#define UELYSYS_SDR_CONTROLLER_AMP_ON_CC              2
#define UELYSYS_SDR_CONTROLLER_AMP_OFF_CC             3
#define UELYSYS_SDR_CONTROLLER_PSDR_ON_CC             4
#define UELYSYS_SDR_CONTROLLER_PSDR_OFF_CC            5
#define UELYSYS_SDR_CONTROLLER_SET_PHASE_SHIFT_CC     6
#define UELYSYS_SDR_CONTROLLER_MOVE_DATA_FILE_CC      7
#define UELYSYS_SDR_CONTROLLER_DEL_TEMP_FILE_CC       8
#define UELYSYS_SDR_CONTROLLER_DEL_SAVE_FILE_CC       9
#define UELYSYS_SDR_CONTROLLER_DEL_FW_IMG_CC         10
#define UELYSYS_SDR_CONTROLLER_RF_TESTPATTERN_CC     11
#define UELYSYS_SDR_CONTROLLER_RF_POBC_CC            12
#define UELYSYS_SDR_CONTROLLER_RF_THERMAL_CC         13
#define UELYSYS_SDR_CONTROLLER_PSDR_GET_SLOT_INFO_CC 14
#define UELYSYS_SDR_CONTROLLER_PSDR_ERASE_SLOT_CC    15
#define UELYSYS_SDR_CONTROLLER_PSDR_WRITE_FW_CC      16
#define UELYSYS_SDR_CONTROLLER_PSDR_VERIFY_SLOT_CC   17
#define UELYSYS_SDR_CONTROLLER_PSDR_UPDATE_FW_CC     18
#define UELYSYS_SDR_CONTROLLER_PSDR_GET_CUR_FW_CC    19
#define UELYSYS_SDR_CONTROLLER_PSDR_GET_THERMAL_CC   20

typedef struct __attribute__((packed)) {
    uint16_t ConfigVal;
} UELYSYS_PAYYONSEI_TEMP_SetConfig_Payload_t;

typedef struct __attribute__((packed)) {
    uint16_t UpperLimit;
    uint16_t LowerLimit;
    uint16_t CritLimit;
} UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t ResVal;
    uint8_t Padding;
} UELYSYS_PAYYONSEI_TEMP_SetResolution_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t Slot;
} UELYSYS_PAYYONSEI_IWSB_Slot_Payload_t;

typedef struct __attribute__((packed)) {
    char FilePath[64];
    uint8_t Slot;
} UELYSYS_PAYYONSEI_IWSB_WriteFirmware_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t Target;
} UELYSYS_PAYYONSEI_IWSB_UpdateFirmware_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t Channel;
    bool Mode;
    uint8_t Flag;
} UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t;

typedef struct __attribute__((packed)) {
    char Filename[50];
    uint8_t FilenameLength;
} UELYSYS_SDR_CONTROLLER_Filename_Payload_t;

typedef struct __attribute__((packed)) {
    uint8_t SlotIndex;
} UELYSYS_SDR_CONTROLLER_Slot_Payload_t;

typedef struct __attribute__((packed)) {
    char Filename[50];
    uint8_t FilenameLength;
    uint8_t Slot;
} UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t;

static_assert(sizeof(UELYSYS_PAYYONSEI_TEMP_SetConfig_Payload_t) == 2,
              "PAYYONSEI TEMP SET_CONFIG payload size mismatch");
static_assert(sizeof(UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t) == 6,
              "PAYYONSEI TEMP SET_LIMITS payload size mismatch");
static_assert(sizeof(UELYSYS_PAYYONSEI_TEMP_SetResolution_Payload_t) == 2,
              "PAYYONSEI TEMP SET_RESOLUTION payload size mismatch");
static_assert(sizeof(UELYSYS_PAYYONSEI_IWSB_Slot_Payload_t) == 1,
              "PAYYONSEI IWSB slot payload size mismatch");
static_assert(sizeof(UELYSYS_PAYYONSEI_IWSB_WriteFirmware_Payload_t) == 65,
              "PAYYONSEI IWSB WRITE_FW payload size mismatch");
static_assert(sizeof(UELYSYS_PAYYONSEI_IWSB_UpdateFirmware_Payload_t) == 1,
              "PAYYONSEI IWSB UPDATE_FW payload size mismatch");
static_assert(sizeof(UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t) == 3,
              "SDR_CONTROLLER SET_PHASE_SHIFT payload size mismatch");
static_assert(sizeof(UELYSYS_SDR_CONTROLLER_Filename_Payload_t) == 51,
              "SDR_CONTROLLER filename payload size mismatch");
static_assert(sizeof(UELYSYS_SDR_CONTROLLER_Slot_Payload_t) == 1,
              "SDR_CONTROLLER slot payload size mismatch");
static_assert(sizeof(UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t) == 52,
              "SDR_CONTROLLER WRITE_FW payload size mismatch");

#define UELYSYS_TTC_NOOP_CC             0
#define UELYSYS_TTC_RESET_COUNTERS_CC   1
#define UELYSYS_TTC_REPORT_CC           2
#define UELYSYS_TTC_GET_TIMELINE_HK_CC  10
#define UELYSYS_TTC_RESET_TIMELINE_HK_CC 11
#define UELYSYS_TTC_GET_PENDING_ENTRY_COUNT_CC 12
#define UELYSYS_TTC_GET_NEXT_ENTRY_ID_CC 13
#define UELYSYS_TTC_GET_NEXT_EXEC_TIME_CC 14
#define UELYSYS_TTC_INSERT_ABS_CMD_ENTRY_CC 15
#define UELYSYS_TTC_INSERT_REL_CMD_ENTRY_CC 16
#define UELYSYS_TTC_DELETE_CMD_ENTRY_CC 17
#define UELYSYS_TTC_DELETE_CMD_GROUP_CC 18
#define UELYSYS_TTC_DELETE_ALL_ENTRIES_CC 19
#define UELYSYS_TTC_EXECUTE_CMD_ENTRY_CC 20
#define UELYSYS_TTC_EXECUTE_CMD_GROUP_CC 21
#define UELYSYS_TTC_PAUSE_TIMELINE_PROCESSING_CC 22
#define UELYSYS_TTC_RESUME_TIMELINE_PROCESSING_CC 23
#define UELYSYS_TTC_PLUMB_INIT_ENTRY_CC 24
#define UELYSYS_TTC_PLUMB_WRITE_ENTRY_CC 25
#define UELYSYS_TTC_PLUMB_FINALIZE_ENTRY_CC 26
#define UELYSYS_TTC_PLUMB_PURGE_TIMELINE_CC 27
#define UELYSYS_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC 28
#define UELYSYS_TTC_START_BEACON_BURST_CC 29

#define UELYSYS_MEOW_NOOP_CC            0
#define UELYSYS_MEOW_RESET_COUNTERS_CC  1
#define UELYSYS_MEOW_SHELL_EXEC_SYNC_CC 10
#define UELYSYS_MEOW_SHELL_EXEC_ASYNC_CC 11
#define UELYSYS_MEOW_SHELL_POLL_CC      12
#define UELYSYS_MEOW_SHELL_KILL_CC      13
#define UELYSYS_MEOW_SHELL_EXEC_SYNC_FORCE_CC 14
#define UELYSYS_MEOW_SHELL_EXEC_ASYNC_FORCE_CC 15
#define UELYSYS_MEOW_FILE_READ_CC       20
#define UELYSYS_MEOW_FILE_WRITE_CC      21
#define UELYSYS_MEOW_FILE_REMOVE_CC     22
#define UELYSYS_MEOW_FILE_COPY_CC       23
#define UELYSYS_MEOW_FILE_MOVE_CC       24
#define UELYSYS_MEOW_FILE_STAT_CC       25
#define UELYSYS_MEOW_FILE_TRUNCATE_CC   26
#define UELYSYS_MEOW_FILE_TAIL_CC       27
#define UELYSYS_MEOW_FILE_CHECKSUM_CC   28
#define UELYSYS_MEOW_DISK_STAT_CC       29
#define UELYSYS_MEOW_SYS_SHUTDOWN_CC    40
#define UELYSYS_MEOW_SYS_SYNC_CC        41
#define UELYSYS_MEOW_SYS_INFO_CC        42
#define UELYSYS_MEOW_SYS_TIME_GET_CC    43
#define UELYSYS_MEOW_SYS_TIME_SET_CC    44
#define UELYSYS_MEOW_SYS_FORCE_KILL_CC  45
#define UELYSYS_MEOW_CSP_SERVER_START_CC 50
#define UELYSYS_MEOW_CSP_SERVER_STOP_CC 51
#define UELYSYS_MEOW_CSP_SET_READ_TIMEOUT_CC 52
#define UELYSYS_MEOW_CSP_HANDLER_LOAD_CC 53
#define UELYSYS_MEOW_CSP_HANDLER_CLEAR_CC 54
#define UELYSYS_MEOW_CSP_SEND_CC        55
#define UELYSYS_MEOW_CSP_FTP_UPLOAD_CC  56
#define UELYSYS_MEOW_CSP_FTP_DOWNLOAD_CC 57
#define UELYSYS_MEOW_CSP_IFSTATS_CC     58
#define UELYSYS_MEOW_CSP_ROUTE_SET_CC   59
#define UELYSYS_MEOW_CSP_REROUTE_SET_CC 60
#define UELYSYS_MEOW_CSP_REROUTE_CLEAR_CC 61

#define UELYSYS_UTRX_NOOP_CC            0
#define UELYSYS_UTRX_RESET_COUNTERS_CC  1
#define UELYSYS_UTRX_RESET_APP_COUNTERS_CC 2
#define UELYSYS_UTRX_RESET_DEVICE_COUNTERS_CC 3
#define UELYSYS_UTRX_GNDWDT_CLEAR_CC    6
#define UELYSYS_UTRX_REBOOT_CC          7
#define UELYSYS_UTRX_RXCONF_SET_BAUD_CC 8
#define UELYSYS_UTRX_TXCONF_SET_BAUD_CC 9
#define UELYSYS_UTRX_SET_DEFAULT_BAUD_CC 10
#define UELYSYS_UTRX_RPARAM_SAVE_1_CC   11
#define UELYSYS_UTRX_RPARAM_SAVE_5_CC   12
#define UELYSYS_UTRX_RPARAM_SAVE_ALL_CC 13
#define UELYSYS_UTRX_RXCONF_SET_FREQ_CC 14
#define UELYSYS_UTRX_TXCONF_SET_FREQ_CC 15
#define UELYSYS_UTRX_CHECK_STATE_PING_CC 16
#define UELYSYS_UTRX_RXCONF_GET_BAUD_CC 20
#define UELYSYS_UTRX_RXCONF_GET_GUARD_CC 21
#define UELYSYS_UTRX_TXCONF_GET_BAUD_CC 22
#define UELYSYS_UTRX_TLM_GET_TEMP_BRD_CC 23
#define UELYSYS_UTRX_TLM_GET_LAST_RSSI_CC 24
#define UELYSYS_UTRX_TLM_GET_LAST_RFERR_CC 25
#define UELYSYS_UTRX_TLM_GET_ACTIVE_CONF_CC 26
#define UELYSYS_UTRX_TLM_GET_BOOT_COUNT_CC 27
#define UELYSYS_UTRX_TLM_GET_BOOT_CAUSE_CC 28
#define UELYSYS_UTRX_TLM_GET_LAST_CONTACT_CC 29
#define UELYSYS_UTRX_TLM_GET_TOT_TX_BYTES_CC 30
#define UELYSYS_UTRX_TLM_GET_TOT_RX_BYTES_CC 31
#define UELYSYS_UTRX_RXCONF_GET_FREQ_CC 32
#define UELYSYS_UTRX_TXCONF_GET_FREQ_CC 33
#define UELYSYS_UANT_NOOP_CC                    0
#define UELYSYS_UANT_RESET_COUNTERS_CC          1
#define UELYSYS_UANT_RESET_STATUS_COUNT_CC      2
#define UELYSYS_UANT_BURN_CHANNEL_CC            3
#define UELYSYS_UANT_STOP_BURN_CC               4
#define UELYSYS_UANT_GET_STATUS_CC              5
#define UELYSYS_UANT_GET_BACKUP_STATUS_CC       6
#define UELYSYS_UANT_GET_BOARD_STATUS_CC        7
#define UELYSYS_UANT_GET_TEMPERATURE_CC         8
#define UELYSYS_UANT_GET_SETTINGS_CC            9
#define UELYSYS_UANT_SET_SETTINGS_CC            10
#define UELYSYS_UANT_AUTODEPLOY_CC              11

typedef int32_t esup_ret_t;
#define ESUP_RET_ENGINE_MASK       0xFF000000u /**< Engine / transport byte. */
#define ESUP_RET_FRAME_MASK        0x00F00000u /**< Framer (link) nibble. */
#define ESUP_RET_WIRE_MASK         0x000F0000u /**< Wire nibble. */
#define ESUP_RET_DEVICE_MASK       0x0000FFFFu /**< Complete device-owned region. */
#define ESUP_RET_DEVICE_LOCAL_MASK 0x0000FF00u /**< Device-driver local byte. */
#define ESUP_RET_EXEC_STATUS_MASK  0x000000FFu /**< Module execution-status byte. */

/** @brief Isolate the engine/transport field of a return code. */
#define ESUP_RET_ENGINE(r) ((esup_ret_t)((uint32_t)(r) & ESUP_RET_ENGINE_MASK))
/** @brief Isolate the framer field of a return code. */
#define ESUP_RET_FRAME(r)  ((esup_ret_t)((uint32_t)(r) & ESUP_RET_FRAME_MASK))
/** @brief Isolate the wire field of a return code. */
#define ESUP_RET_WIRE(r)   ((esup_ret_t)((uint32_t)(r) & ESUP_RET_WIRE_MASK))
/** @brief Isolate the device / custom-driver field of a return code. */
#define ESUP_RET_DEVICE(r) ((esup_ret_t)((uint32_t)(r) & ESUP_RET_DEVICE_MASK))
/** @brief Isolate a device driver's local-error field. */
#define ESUP_RET_DEVICE_LOCAL(r) \
    ((esup_ret_t)((uint32_t)(r) & ESUP_RET_DEVICE_LOCAL_MASK))
/** @brief Extract the module's raw command-execution status. */
#define ESUP_RET_EXEC_STATUS(r) \
    ((uint8_t)((uint32_t)(r) & ESUP_RET_EXEC_STATUS_MASK))


#define UELYSYS_STX_NOOP_CC                     0
#define UELYSYS_STX_RESET_COUNTERS_CC           1
#define UELYSYS_STX_INITIALIZE_CC               2
#define UELYSYS_STX_SET_MODULE_ID_CC            3
#define UELYSYS_STX_PARAM_INIT_CC               4
#define UELYSYS_STX_GET_FILEHANDLE_CC           5

#define UELYSYS_STX_SET_SYMBOL_RATE_CC          10
#define UELYSYS_STX_SET_TX_POWER_CC             11
#define UELYSYS_STX_SET_CENTER_FREQ_CC          12
#define UELYSYS_STX_SET_MODCOD_CC               13
#define UELYSYS_STX_SET_ROLL_OFF_CC             14
#define UELYSYS_STX_SET_PILOT_SIGNAL_CC         15
#define UELYSYS_STX_SET_FEC_FRAME_SIZE_CC       16
#define UELYSYS_STX_SET_PRETX_DELAY_CC          17
#define UELYSYS_STX_SET_ALL_PARAMETERS_CC       18
#define UELYSYS_STX_SET_RS485_BAUD_CC           19
#define UELYSYS_STX_SET_MOD_DATA_IFACE_CC       20
#define UELYSYS_STX_GET_SYMBOL_RATE_CC          30
#define UELYSYS_STX_GET_TX_POWER_CC             31
#define UELYSYS_STX_GET_CENTER_FREQ_CC          32
#define UELYSYS_STX_GET_MODCOD_CC               33
#define UELYSYS_STX_GET_ROLL_OFF_CC             34
#define UELYSYS_STX_GET_PILOT_SIGNAL_CC         35
#define UELYSYS_STX_GET_FEC_FRAME_SIZE_CC       36
#define UELYSYS_STX_GET_PRETX_DELAY_CC          37
#define UELYSYS_STX_GET_ALL_PRAMETERS_CC        38
#define UELYSYS_STX_GET_REPORT_CC               39
#define UELYSYS_STX_GET_MOD_DATA_IFACE_CC       40
#define UELYSYS_STX_FILESYS_CC_DIRNEXT          51
#define UELYSYS_STX_FILESYS_CC_DELFILE          52
#define UELYSYS_STX_FILESYS_CC_DELALLFILE       53
#define UELYSYS_STX_FILESYS_CC_DIR              50
#define UELYSYS_STX_FILESYS_CC_CREATEFILE       54
#define UELYSYS_STX_FILESYS_CC_WRITEFILE        55
#define UELYSYS_STX_FILESYS_CC_OPENFILE         56
#define UELYSYS_STX_FILESYS_CC_READFILE         57
#define UELYSYS_STX_FILESYS_CC_SENDFILE         58
#define UELYSYS_STX_FILESYS_CC_SENDFILERTI      59
#define UELYSYS_STX_SYSCONF_CC_TRANSMITMODE     62
#define UELYSYS_STX_SYSCONF_CC_IDLEMODE         63
#define UELYSYS_STX_SYSCONF_CC_UPDATEFW         64
#define UELYSYS_STX_SYSCONF_CC_SAFESHUTDOWN     65

#define UELYSYS_GPIO_NOOP_CC                    0
#define UELYSYS_GPIO_RESET_COUNTERS_CC          1
#define UELYSYS_GPIO_PROCESS_CC                 2
#define UELYSYS_GPIO_DIGPIOLAY_PARAM_CC         3
#define UELYSYS_GPIO_STX_EN1_HIGH_CC            4
#define UELYSYS_GPIO_STX_EN1_LOW_CC             5
#define UELYSYS_GPIO_LGE_MCU_RESET_HIGH_CC      6
#define UELYSYS_GPIO_LGE_MCU_RESET_LOW_CC       7
#define UELYSYS_GPIO_STX_EN2_HIGH_CC            8
#define UELYSYS_GPIO_STX_EN2_LOW_CC             9
#define UELYSYS_GPIO_ADCS_EN_HIGH_CC            10
#define UELYSYS_GPIO_ADCS_EN_LOW_CC             11
#define UELYSYS_GPIO_ADCS_BOOT_HIGH_CC          12
#define UELYSYS_GPIO_ADCS_BOOT_LOW_CC           13

#define UELYSYS_PAYUEL_OBC_NOOP_CC              0
#define UELYSYS_PAYUEL_OBC_RESET_COUNTERS_CC    1
#define UELYSYS_PAYUEL_OBC_SEND_OBC_BCN_CC      2
#define UELYSYS_PAYUEL_OBC_MOTOR_MODE_CC        3
#define UELYSYS_PAYUEL_OBC_CAM_SHOT_CC          4
#define UELYSYS_PAYUEL_OBC_DOWNLOAD_META_CC     5
#define UELYSYS_PAYUEL_OBC_CHUNK_DOWNLOAD_CC    6
#define UELYSYS_PAYUEL_OBC_SENSOR_META_CC       7
#define UELYSYS_PAYUEL_OBC_SENSOR_CHUNK_CC      8
#define UELYSYS_PAYUEL_OBC_DOWNLOAD_IMAGE_CC    9
#define UELYSYS_PAYUEL_OBC_DOWNLOAD_SENSOR_CC   10
#define UELYSYS_PAYUEL_OBC_REPORT_BCN_CC        11
#define UELYSYS_PAYUEL_OBC_DOWNLOAD_RANGE_CC    12

typedef struct __attribute__((packed)) PAYUEL_OBC_MotorMode_Report_Payload
{
    int8   LeftSpeed;
    int8   RightSpeed;
    uint8  Duration;
    uint8  ResponseValid;
    uint8  ErrorDetail[4];
    uint16 CurrentMilliamp[6];
} PAYUEL_OBC_MotorMode_Report_Payload_t;

#define UELYSYS_PAYUEL_CAM_NOOP_CC              0
#define UELYSYS_PAYUEL_CAM_RESET_COUNTERS_CC    1
#define UELYSYS_PAYUEL_CAM_SHOT_CC              2
#define UELYSYS_PAYUEL_CAM_HEALTH_CHECK_CC      3
#define UELYSYS_PAYUEL_CAM_PROCESS_BINNING_CC   4
#define UELYSYS_PAYUEL_CAM_DOWNLOAD_META_CC     5
#define UELYSYS_PAYUEL_CAM_CHUNK_DOWNLOAD_CC    6
#define UELYSYS_PAYUEL_CAM_SEND_BCN_CC          7
#define UELYSYS_PAYUEL_CAM_DOWNLOAD_IMAGE_CC    8
#define UELYSYS_PAYUEL_CAM_DOWNLOAD_RANGE_CC    9
#define UELYSYS_PAYUEL_CAM_REPORT_BCN_CC        10

#define UELYSYS_LGBAT_NOOP_CC                   0
#define UELYSYS_LGBAT_RESET_COUNTERS_CC         1
#define UELYSYS_LGBAT_SEND_BCN_CC               2
#define UELYSYS_LGBAT_REQUEST_DATA_CC           3
#define UELYSYS_LGBAT_REQUEST_ALL_DATA_CC       4
#define UELYSYS_LGBAT_CRITICAL_ALERT_CC         0xFE

#define UELYSYS_PAYUEL_AOS_NOOP_CC              0
#define UELYSYS_PAYUEL_AOS_RESET_COUNTERS_CC    1
#define UELYSYS_PAYUEL_AOS_RESET_CC             2
#define UELYSYS_PAYUEL_AOS_WRITE_REGISTER_CC    3
#define UELYSYS_PAYUEL_AOS_READ_REGISTER_CC     4
#define UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC 5

/* PAYUEL_YONSEI command/report interface (topic 0xCC -> MID 0x18CC). */
#define UELYSYS_PAYUEL_YONSEI_CMD_ID             0x18CC

/* Remaining UELYSYS applications which publish RPT_Report_t command results. */
#define UELYSYS_CI_LAB_CMD_ID                     0x1810
#define UELYSYS_EO_CMD_ID                         0x1890
#define UELYSYS_GPS_CMD_ID                        0x1894

#define UELYSYS_GPS_NOOP_CC                         0
#define UELYSYS_GPS_RESET_COUNTERS_CC               1
#define UELYSYS_GPS_GET_COUNTERS_CC                 2
#define UELYSYS_GPS_GET_APPDATA_CC                  3
#define UELYSYS_GPS_DRIVER_REPORT_HK_CC             4
#define UELYSYS_GPS_DRIVER_CLEAR_HK_CC              5
#define UELYSYS_GPS_OEM_CMD_LOG_CC                  12
#define UELYSYS_GPS_OEM_CMD_LOG_ONCE_CC             13
#define UELYSYS_GPS_OEM_CMD_LOG_ONTIME_CC           14
#define UELYSYS_GPS_OEM_CMD_LOG_ONCHANGED_CC        15
#define UELYSYS_GPS_OEM_CMD_LOG_ONNEW_CC            16
#define UELYSYS_GPS_OEM_CMD_UNLOG_CC                17
#define UELYSYS_GPS_OEM_CMD_UNLOGALL_CC             18
#define UELYSYS_GPS_OEM_CMD_ELEVATION_CUTOFF_CC     19
#define UELYSYS_GPS_OEM_CMD_INTERFACE_MODE_CC       20
#define UELYSYS_GPS_OEM_CMD_SERIAL_CONFIG_CC        21
#define UELYSYS_GPS_OEM_CMD_PUBLISH_CC              22
#define UELYSYS_GPS_OEM_LOG_GET_HANDLER_HK_CC       30
#define UELYSYS_GPS_OEM_LOG_GET_STAT_CC             31
#define UELYSYS_GPS_OEM_LOG_HANDLER_SET_STATUS_CC   32
#define UELYSYS_GPS_OEM_LOG_HANDLER_GET_STATUS_CC   33
#define UELYSYS_GPS_OEM_LOG_HANDLER_ACTIVATE_CC     34
#define UELYSYS_GPS_OEM_LOG_HANDLER_DEACTIVATE_CC   35
#define UELYSYS_GPS_OEM_LOG_HANDLER_GO_DORMANT_CC   36
#define UELYSYS_GPS_OEM_LOG_HANDLER_WAKEUP_CC       37
#define UELYSYS_GPS_OEM_LOG_HANDLER_ACTIVATE_ALL_CC 38
#define UELYSYS_GPS_OEM_LOG_HANDLER_DEACTIVATE_ALL_CC 39
#define UELYSYS_GPS_OEM_LOG_HANDLER_REGISTER_CC     50
#define UELYSYS_GPS_OEM_LOG_HANDLER_UNREGISTER_CC   51
#define UELYSYS_GPS_OEM_LOG_ADD_CALLBACK_CC         52
#define UELYSYS_GPS_OEM_LOG_CLEAR_CALLBACKS_CC      53
#define UELYSYS_GPS_OEM_LOG_HANDLER_MARK_BROKEN_CC  54
#define UELYSYS_GPS_OEM_LOG_GET_MESSAGE_LENGTH_CC   55
#define UELYSYS_GPS_OEM_LOG_GET_HANDLER_NAME_CC     56
#define UELYSYS_GPS_OEM_LOG_RESET_STAT_CC           57
#define UELYSYS_GPS_OEM_LOG_GET_RECENT_MESSAGE_CC   58
#define UELYSYS_GPS_OEM_LOG_REJECT_MISSING_CRC_CC   59
#define UELYSYS_GPS_OEM_LOG_IGNORE_MISSING_CRC_CC   60
#define UELYSYS_GPS_OEM_LOG_LOCK_HANDLERS_CC        100
#define UELYSYS_GPS_OEM_LOG_UNLOCK_HANDLERS_CC      101

#define UELYSYS_SCH_LAB_CMD_ID                    0x18A3
#define UELYSYS_SP_CMD_ID                         0x1858
#define UELYSYS_TO_LAB_CMD_ID                     0x1823

#define UELYSYS_CI_LAB_NOOP_CC                    0
#define UELYSYS_CI_LAB_RESET_COUNTERS_CC          1
#define UELYSYS_CI_LAB_CREATE_CHILD_TASK_CC       2

#define UELYSYS_EO_NOOP_CC                        0
#define UELYSYS_EO_RESET_COUNTER_CC               1
#define UELYSYS_EO_RESET_PHASE_CC                 2
#define UELYSYS_EO_SEND_STATE_CC                  3
#define UELYSYS_EO_FINISH_PHASE_CC                4
#define UELYSYS_EO_START_CHILD_TASK_CC            6

#define UELYSYS_SCH_LAB_NOOP_CC                   0
#define UELYSYS_SCH_LAB_RESET_COUNTERS_CC         1
#define UELYSYS_SCH_LAB_SET_ENTRY_STATE_CC        2
#define UELYSYS_SCH_LAB_SET_ENTRY_CONFIG_CC       3

#define UELYSYS_SP_NOOP_CC                        0
#define UELYSYS_SP_RESET_COUNTERS_CC              1
#define UELYSYS_SP_GET_AR6_STATUS_CC              2
#define UELYSYS_SP_DEPLOY_CC                      3
#define UELYSYS_SP_STOP_BURN_CC                   4
#define UELYSYS_SP_AUTO_DEPLOY_CC                 5
#define UELYSYS_SP_SET_AR6_ADDR_CC                6
#define UELYSYS_SP_SCAN_AR6_CC                    7
#define UELYSYS_SP_ALL_STOP_BURN_CC               8
#define UELYSYS_SP_RESET_AR6_COUNTERS_CC          9

#define UELYSYS_TO_LAB_NOOP_CC                    0
#define UELYSYS_TO_LAB_RESET_STATUS_CC            1
#define UELYSYS_TO_LAB_ADD_PKT_CC                 2
#define UELYSYS_TO_LAB_SEND_DATA_TYPES_CC         3
#define UELYSYS_TO_LAB_REMOVE_PKT_CC              4
#define UELYSYS_TO_LAB_REMOVE_ALL_PKT_CC          5
#define UELYSYS_TO_LAB_OUTPUT_ENABLE_CC           6
#define UELYSYS_TO_LAB_OUTPUT_DISABLE_CC          7
#define UELYSYS_PAYUEL_YONSEI_NOOP_CC            0
#define UELYSYS_PAYUEL_YONSEI_RESET_COUNTERS_CC  1
#define UELYSYS_PAYUEL_YONSEI_FORWARD_SB_CC      7

/*****************************************************************************************/
/*              SD1-SAT (FSW_SD1SAT/apps, excluding gps and temp)          */
/*                                                                                       */
/* Command MIDs below were derived from each app's default_<app>_topicids.h            */
/* (CFE_MISSION_<APP>_CMD_TOPICID), OR'd onto the mission's 0x1800 command base.        */
/* ds / hk / sch_lab are unmodified upstream NASA apps (git submodules, no local        */
/* topicid override in this tree); their MIDs are placeholders in an unused slot and    */
/* should be confirmed once the mission topicid table is generated.                     */
/*****************************************************************************************/
#define SD1SAT_PACK __attribute__((packed))

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
} SD1SAT_NoArgCmd_t;

/* -------------------- CI -------------------- */
#define SD1SAT_CI_CMD_ID                 0x1900
#define SD1SAT_CI_GATE_CMD_ID            0x1901

#define SD1SAT_CI_NOOP_CC                0
#define SD1SAT_CI_RESET_COUNTERS_CC      1

#define SD1SAT_CI_GATE_NOOP_CC           0
#define SD1SAT_CI_GATE_RESET_INGEST_CC   1
#define SD1SAT_CI_GATE_SET_UPLINK_CC     2
#define SD1SAT_CI_GATE_SET_CHECKSUM_CC   3
#define SD1SAT_CI_GATE_SET_FRAMING_CC   4

typedef struct SD1SAT_PACK {
    uint8_t Enable; /* 0 = off, non-zero = on */
} SD1SAT_CI_Enable_Payload_t;

typedef struct SD1SAT_PACK {
    uint32 Magic;
} SD1SAT_CI_Framing_Payload_t;

/* -------------------- CR -------------------- */
#define SD1SAT_CR_CMD_ID                 0x1940

#define SD1SAT_CR_NOOP_CC                0
#define SD1SAT_CR_RESET_COUNTERS_CC      1
#define SD1SAT_CR_RETRANSMIT_LATEST_CC   2
#define SD1SAT_CR_RETRANSMIT_APP_CC      3
#define SD1SAT_CR_RETRANSMIT_APPCMD_CC   4
#define SD1SAT_CR_DUMP_QUEUE_CC          5
#define SD1SAT_CR_SET_DOWNLINK_MODE_CC   6
#define SD1SAT_CR_GET_FULL_STATES_CC     7

typedef struct SD1SAT_PACK {
    uint16_t CmdMsgId;
} SD1SAT_CR_App_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t CmdMsgId;
    uint8_t  CmdCode;
    uint8_t  Spare;
} SD1SAT_CR_AppCmd_Payload_t;

typedef struct SD1SAT_PACK {
    char Path[64];
} SD1SAT_CR_Dump_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Mode; /* 0=TO, 1=CSP, 2=BOTH */
} SD1SAT_CR_Mode_Payload_t;

typedef struct SD1SAT_PACK {
    int32_t OsError; /* 0=TO, 1=CSP, 2=BOTH */
} SD1SAT_CR_DumpQueue_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Mode;
} SD1SAT_CR_SetDownlinkMode_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    /* Command handling */
    uint32_t CommandCounter;
    uint32_t CommandErrorCounter;

    /* Collection */
    uint32_t ReportsReceived;
    uint32_t ReportsSuccess;
    uint32_t ReportsError;
    uint32_t ReportsMalformed;

    /* Retention */
    uint32_t QueueOverwrites;
  
    /* Retransmission */
    uint32_t RetransmitCount;
    uint32_t RetransmitNotFound;

    /* Downlink */
    uint32_t DownlinkSent;
    uint32_t DownlinkErrors;

    /* File dump */
    uint32_t DumpCount;
    uint32_t DumpErrors;

    /* Downlink path */
    uint8_t DownlinkMode; /* See CR_DownlinkMode_t */
} SD1SAT_CR_GetFullReportState_tlm_Payload_t;

/* -------------------- DS / HK / SCH_LAB (reused BEE1012 case bodies) -------------------- */
#define SD1SAT_DS_CMD_ID      0x1930
#define SD1SAT_HK_CMD_ID      0x1950
#define SD1SAT_SCH_LAB_CMD_ID 0x1960
#define SD1SAT_FM_CMD_ID      0x188C

/* -------------------- EPS (SD1_EPS_ prefix: "EPS_" is already used by the shared
 * BEE1012/UELYSYS P80 power-interface app; SD1-SAT's EPS is an unrelated P60 board app) */
#define SD1_EPS_CMD_ID                    0x1980

#define SD1_EPS_NOOP_CC                   0
#define SD1_EPS_RESET_COUNTERS_CC         1
#define SD1_EPS_PING_CC                   2
#define SD1_EPS_IDENT_CC                  3
#define SD1_EPS_REBOOT_CC                 4
#define SD1_EPS_MEMFREE_CC                5
#define SD1_EPS_BUFFREE_CC                6
#define SD1_EPS_GNDWDT_CLEAR_CC           7
#define SD1_EPS_GET_CC                    8
#define SD1_EPS_SET_CC                    9
#define SD1_EPS_GET_RAW_CC                10
#define SD1_EPS_SET_RAW_CC                11
#define SD1_EPS_CHANNEL_SET_CC            12
#define SD1_EPS_CHANNEL_GET_CC            13
#define SD1_EPS_CHANNELS_GET_CC           14
#define SD1_EPS_TABLE_SAVE_CC             15
#define SD1_EPS_TABLE_LOAD_CC             16
#define SD1_EPS_HK_SET_MODE_CC            17
#define SD1_EPS_HK_GET_MODE_CC            18
#define SD1_EPS_TABLE_ACQUIRE_CC          19
#define SD1_EPS_TABLE_READBACK_CC         20
#define SD1_EPS_TABLE_TO_FILE_CC          21
#define SD1_EPS_TABLE_GET_CC              22
#define SD1_EPS_WDT_GET_CC                23

#define SD1_EPS_VAL_MAX  180
#define SD1_EPS_PATH_MAX 64
#define SD1_EPS_REPORT_DATA_MAX 512

typedef struct SD1SAT_PACK {
    uint8_t Board; /* P60_KIND_DOCK/ACU/PDU */
} SD1_EPS_Board_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Board;
    uint8_t Table; /* P60_TBL_CONF/CAL/HK */
} SD1_EPS_Table_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Board;
    uint8_t  Table;
    uint16_t Addr;
    uint16_t OutSize;
} SD1_EPS_Get_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Board;
    uint8_t  Table;
    uint16_t Addr;
    uint16_t ValSize;
    uint8_t  Val[SD1_EPS_VAL_MAX];
} SD1_EPS_Set_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Board;
    uint8_t  Table;
    uint8_t  Type;
    uint8_t  Spare;
    uint16_t Addr;
    uint16_t ElemSize;
    uint16_t Count;
} SD1_EPS_GetRaw_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Board;
    uint8_t  Table;
    uint8_t  Type;
    uint8_t  Spare;
    uint16_t Addr;
    uint16_t ValSize;
    uint8_t  Val[SD1_EPS_VAL_MAX];
} SD1_EPS_SetRaw_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Board;
    uint8_t Channel;
    uint8_t On;
} SD1_EPS_ChannelSet_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Board;
    uint8_t Channel;
} SD1_EPS_ChannelGet_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Mode; /* 0 = CELL, 1 = SINGLE */
} SD1_EPS_SetMode_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t Offset;
    uint16_t Length;
} SD1_EPS_Readback_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Board;
    uint8_t Table;
    char    Path[SD1_EPS_PATH_MAX];
} SD1_EPS_ToFile_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t ReturnMs;
} SD1_EPS_PING_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    char hostname[24];   /* e.g. "p60-dock".           */
    char model[32];      /* e.g. "NanoPower P60 Dock". */
    char revision[24];   /* Firmware revision.         */
    char date[16];       /* Build date.                */
    char time[12];       /* Build time.                */
} SD1_EPS_IDENT_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t FreeBytes;
} SD1_EPS_MEMFREE_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t FreeBuffers;
} SD1_EPS_BUFFREE_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t mode;
} SD1_EPS_HkSetMode_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t mode;
} SD1_EPS_HkGetMode_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    bool IsOn;
} SD1_EPS_CHANNEL_GET_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    bool ChannelStates[13];
} SD1_EPS_CHANNELS_GET_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t ReportSize;
} SD1_EPS_TABLE_ACQUIRE_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    uint32 WdtGndLeft;
    uint32 WdtCanLeft;
    uint32 CntWdtGnd;
    uint32 CntWdtCan;
} SD1_EPS_WDT_GET_tlm_Payload_t;

typedef struct SD1SAT_PACK {
    size_t ImageSize;
} SD1_EPS_TABLE_TO_FILE_tlm_Payload_t;

/* -------------------- GPS (report parsing only; no TC UI yet) -------------------- */
#define SD1SAT_GPS_CMD_ID                        0x1990
#define SD1SAT_GPS_NOOP_CC                       0
#define SD1SAT_GPS_RESET_COUNTERS_CC             1
#define SD1SAT_GPS_GET_COUNTERS_CC               2
#define SD1SAT_GPS_GET_APPDATA_CC                3
#define SD1SAT_GPS_DRIVER_REPORT_HK_CC           4
#define SD1SAT_GPS_DRIVER_CLEAR_HK_CC            5
#define SD1SAT_GPS_OEM_CMD_LOG_CC                10
#define SD1SAT_GPS_OEM_CMD_LOG_ONCE_CC           11
#define SD1SAT_GPS_OEM_CMD_LOG_ONTIME_CC         12
#define SD1SAT_GPS_OEM_CMD_LOG_ONCHANGED_CC      13
#define SD1SAT_GPS_OEM_CMD_LOG_ONNEW_CC          14
#define SD1SAT_GPS_OEM_CMD_UNLOG_CC              15
#define SD1SAT_GPS_OEM_CMD_UNLOGALL_CC           16
#define SD1SAT_GPS_OEM_CMD_ELEVATION_CUTOFF_CC   17
#define SD1SAT_GPS_OEM_CMD_INTERFACE_MODE_CC     18
#define SD1SAT_GPS_OEM_CMD_SERIAL_CONFIG_CC      19
#define SD1SAT_GPS_OEM_CMD_PUBLISH_CC            29
#define SD1SAT_GPS_OEM_LOG_GET_HANDLER_HK_CC     30
#define SD1SAT_GPS_OEM_LOG_GET_STAT_CC           31
#define SD1SAT_GPS_OEM_LOG_RESET_STAT_CC         32
#define SD1SAT_GPS_OEM_LOG_GET_RECENT_MESSAGE_CC 33
#define SD1SAT_GPS_OEM_LOG_HANDLER_GET_STATUS_CC 34
#define SD1SAT_GPS_OEM_LOG_HANDLER_ACTIVATE_CC   35
#define SD1SAT_GPS_OEM_LOG_HANDLER_DEACTIVATE_CC 36
#define SD1SAT_GPS_OEM_LOG_HANDLER_GO_DORMANT_CC 37
#define SD1SAT_GPS_OEM_LOG_HANDLER_WAKEUP_CC     38
#define SD1SAT_GPS_OEM_LOG_HANDLER_ACTIVATE_ALL_CC   39
#define SD1SAT_GPS_OEM_LOG_HANDLER_DEACTIVATE_ALL_CC 40
#define SD1SAT_GPS_OEM_LOG_HANDLER_REGISTER_CC   50
#define SD1SAT_GPS_OEM_LOG_HANDLER_UNREGISTER_CC 51
#define SD1SAT_GPS_OEM_LOG_ADD_CALLBACK_CC       52
#define SD1SAT_GPS_OEM_LOG_CLEAR_CALLBACKS_CC    53
#define SD1SAT_GPS_OEM_LOG_HANDLER_MARK_BROKEN_CC 54
#define SD1SAT_GPS_OEM_LOG_GET_MESSAGE_LENGTH_CC 55
#define SD1SAT_GPS_OEM_LOG_GET_HANDLER_NAME_CC   56
#define SD1SAT_GPS_OEM_LOG_HANDLER_SET_STATUS_CC 57
#define SD1SAT_GPS_OEM_LOG_IGNORE_MISSING_CRC_CC 58
#define SD1SAT_GPS_OEM_LOG_REJECT_MISSING_CRC_CC 59
#define SD1SAT_GPS_OEM_LOG_LOCK_HANDLERS_CC      100
#define SD1SAT_GPS_OEM_LOG_UNLOCK_HANDLERS_CC    101

/**
 * Handler-MsgId-only payload template.
 */
typedef struct __attribute__((packed)) {
    uint16 msgId;
} GPS_OEM_HandlerNoArgCmd_Payload_t;


/*
** OEM receiver command types.
*/
typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint8  type;
    uint32 port;
    uint32 trigger;
    uint32 hold;
    double period;
    double offset;
} GPS_OEM_Cmd_Log_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint32 port;
} GPS_OEM_Cmd_LogOnce_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint32 port;
    double period;
    double offset;
} GPS_OEM_Cmd_LogOnTime_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint32 port;
} GPS_OEM_Cmd_LogOnChanged_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint32 port;
} GPS_OEM_Cmd_LogOnNew_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint8  type;
    uint32 port;
} GPS_OEM_Cmd_Unlog_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint32 port;
    bool held;
} GPS_OEM_Cmd_UnlogAll_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint32 constellation;
    float cutoff;
} GPS_OEM_Cmd_ElevationCutoff_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint32 port;
    uint32 rxType;
    uint32 txType;
    uint32 responses;
} GPS_OEM_Cmd_InterfaceMode_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint32 port;
    uint32 baud;
    uint32 parity;
    uint32 databits;
    uint32 stopbits;
    uint32 handshake;
    uint32 _break;
} GPS_OEM_Cmd_SerialConfig_Payload_t;

typedef struct __attribute__((packed)) {
    int32  interfaceIndex;

    uint16 msgId;
    uint16 bodylength;
    uint8 body[180];
} GPS_OEM_Cmd_Publish_Payload_t;


/*
** OEM log handler command types.
*/
typedef struct __attribute__((packed)) {
    char name[16];
    uint16 msgId;
    uint16 msgLength;
} GPS_OEM_Log_HandlerRegister_Payload_t;

typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerUnregister_Payload_t;

typedef struct __attribute__((packed)) {
    uint16 msgId;
    int32  options;
    char   libpath[64];
    char   funcName[32];
} GPS_OEM_Log_AddCallback_Payload_t;

typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_ClearCallbacks_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_GetHandlerHk_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_GetStat_Payload_t;

typedef struct __attribute__((packed)) {
    uint16 msgId;
    uint8_t status;
    bool override;
} GPS_OEM_Log_HandlerSetStatus_Payload_t;

typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerActivate_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerDeactivate_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerGoDormant_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerGetStatus_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerWakeup_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerActivateAll_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerDeactivateAll_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_HandlerMarkBroken_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_GetMessageLength_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_GetHandlerName_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_ResetStat_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_RejectMissingCrc_Payload_t;
typedef GPS_OEM_HandlerNoArgCmd_Payload_t GPS_OEM_Log_IgnoreMissingCrc_Payload_t;

/**
 * oem_log_get_recent_message() takes an offset, allowing a large message to be
 * retrieved in windows across multiple commands.
 */
typedef struct __attribute__((packed)) {
    uint16 msgId;
    uint32 offset;
} GPS_OEM_Log_GetRecentMessage_Payload_t;

/**
 * These two commands temper with the global handler lock from OUTSIDE the
 * driver task - which may invoke undefined behavior if the sync is off.
 * To prevent an unintentional execution, a magic key is first validated.
 */
typedef struct __attribute__((packed)) {
    uint32  Magic; //  C01DCAFE
} GPS_OEM_Log_LockHandlers_Payload_t;

typedef struct __attribute__((packed)) {
    uint32  Magic; //  A7EDECAF
} GPS_OEM_Log_UnlockHandlers_Payload_t;

/* -------------------- MEOW (report parsing only; no TC UI yet) -------------------- */
#define SD1SAT_MEOW_CMD_ID                  0x1950
#define SD1SAT_MEOW_NOOP_CC                 0
#define SD1SAT_MEOW_RESET_COUNTERS_CC       1
#define SD1SAT_MEOW_GET_COUNTERS_CC         2
#define SD1SAT_MEOW_SHELL_EXEC_SYNC_CC      10
#define SD1SAT_MEOW_SHELL_EXEC_ASYNC_CC     11
#define SD1SAT_MEOW_SHELL_POLL_CC           12
#define SD1SAT_MEOW_SHELL_KILL_CC           13
#define SD1SAT_MEOW_SHELL_EXEC_SYNC_FORCE_CC  14
#define SD1SAT_MEOW_SHELL_EXEC_ASYNC_FORCE_CC 15
#define SD1SAT_MEOW_FILE_READ_CC            20
#define SD1SAT_MEOW_FILE_WRITE_CC           21
#define SD1SAT_MEOW_FILE_REMOVE_CC          22
#define SD1SAT_MEOW_FILE_COPY_CC            23
#define SD1SAT_MEOW_FILE_MOVE_CC            24
#define SD1SAT_MEOW_FILE_STAT_CC            25
#define SD1SAT_MEOW_FILE_TRUNCATE_CC        26
#define SD1SAT_MEOW_FILE_TAIL_CC            27
#define SD1SAT_MEOW_FILE_CHECKSUM_CC        28
#define SD1SAT_MEOW_DISK_STAT_CC            29
#define SD1SAT_MEOW_SYS_SHUTDOWN_CC         40
#define SD1SAT_MEOW_SYS_SYNC_CC             41
#define SD1SAT_MEOW_SYS_INFO_CC             42
#define SD1SAT_MEOW_SYS_TIME_GET_CC         43
#define SD1SAT_MEOW_SYS_TIME_SET_CC         44
#define SD1SAT_MEOW_SYS_FORCE_KILL_CC       45

/* -------------------- SNBP (report parsing only; no TC UI yet) -------------------- */
#define SD1SAT_SNBP_CMD_ID               0x19C0
#define SD1SAT_SNBP_NOOP_CC              0
#define SD1SAT_SNBP_RESET_COUNTERS_CC    1
#define SD1SAT_SNBP_RAMM_GET_DPSD_CC     4
#define SD1SAT_SNBP_RAMM_GET_ADSD_CC     5
#define SD1SAT_SNBP_RAMM_GET_HKSD_CC     6
#define SD1SAT_SNBP_RAMM_GET_BESD_CC     7
#define SD1SAT_SNBP_REMM_GET_POSD_CC     8
#define SD1SAT_SNBP_REMM_GET_BESD_CC     9
#define SD1SAT_SNBP_REMM_GET_ADSD_CC     10
#define SD1SAT_SNBP_RAMM_STORE_CC        11
#define SD1SAT_SNBP_REMM_STORE_CC        12
#define SD1SAT_SNBP_RAMM_SET_PERIOD_CC   13
#define SD1SAT_SNBP_REMM_SET_PERIOD_CC   14

#define SD1SAT_SNBP_SENTENCE_MAX         128
#define SD1SAT_SNBP_READING_OVERHEAD     5

typedef struct SNBP_Store_Payload {
    uint16 Count;
} SNBP_Store_Payload_t;

/**
 * @brief SET_PERIOD: seconds between sends. Zero is rejected.
 */
typedef struct SNBP_Period_Payload {
    uint16 Seconds;
} SNBP_Period_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t Seconds;                         /* spacecraft time (CFE_TIME_GetTime)   */
    uint8_t  Length;                          /* characters in Text, terminator apart */
    char     Text[SD1SAT_SNBP_SENTENCE_MAX];  /* sentence body, NUL terminated        */
} SD1SAT_SNBP_Reading_t;

/* -------------------- TTC (report parsing only; no TC UI yet) -------------------- */
#define SD1SAT_TTC_CMD_ID                          0x1930
#define SD1SAT_TTC_NOOP_CC                         0
#define SD1SAT_TTC_RESET_COUNTERS_CC               1
#define SD1SAT_TTC_REPORT_CC                       2
#define SD1SAT_TTC_GET_TIMELINE_HK_CC              10
#define SD1SAT_TTC_RESET_TIMELINE_HK_CC            11
#define SD1SAT_TTC_GET_PENDING_ENTRY_COUNT_CC      12
#define SD1SAT_TTC_GET_NEXT_ENTRY_ID_CC            13
#define SD1SAT_TTC_GET_NEXT_EXEC_TIME_CC           14
#define SD1SAT_TTC_INSERT_ABS_CMD_ENTRY_CC         15
#define SD1SAT_TTC_INSERT_REL_CMD_ENTRY_CC         16
#define SD1SAT_TTC_DELETE_CMD_ENTRY_CC             17
#define SD1SAT_TTC_DELETE_CMD_GROUP_CC             18
#define SD1SAT_TTC_DELETE_ALL_ENTRIES_CC           19
#define SD1SAT_TTC_EXECUTE_CMD_ENTRY_CC            20
#define SD1SAT_TTC_EXECUTE_CMD_GROUP_CC            21
#define SD1SAT_TTC_PAUSE_TIMELINE_PROCESSING_CC    22
#define SD1SAT_TTC_RESUME_TIMELINE_PROCESSING_CC   23
#define SD1SAT_TTC_PLUMB_INIT_ENTRY_CC              24
#define SD1SAT_TTC_PLUMB_WRITE_ENTRY_CC             25
#define SD1SAT_TTC_PLUMB_FINALIZE_ENTRY_CC          26
#define SD1SAT_TTC_PLUMB_PURGE_TIMELINE_CC          27
#define SD1SAT_TTC_PLUMB_DELETE_RESERVED_ENTRY_CC   28

/* -------------------- OPS (all commands are no-arg) -------------------- */
#define SD1SAT_OPS_CMD_ID                0x1920

#define SD1SAT_OPS_NOOP_CC               0
#define SD1SAT_OPS_RESET_COUNTERS_CC     1
#define SD1SAT_OPS_GET_RECORD_CC         2
#define SD1SAT_OPS_DEPLOY_INHIBIT_CC     3
#define SD1SAT_OPS_DEPLOY_ENABLE_CC      4
#define SD1SAT_OPS_SET_SECONDS_SINCE_FIRST_BOOT_CC 5

typedef struct {
    uint32 SecondsSinceFirstBoot;
} OPS_SetSecondsSinceFirstBootCmd_Payload_t;

/* -------------------- TO -------------------- */
#define SD1SAT_TO_CMD_ID                  0x1910

#define SD1SAT_TO_NOOP_CC                 0
#define SD1SAT_TO_RESET_CC                1
#define SD1SAT_TO_ENABLE_OUTPUT_CC        2
#define SD1SAT_TO_DISABLE_OUTPUT_CC       3
#define SD1SAT_TO_ADD_TBL_ENTRY_CC        6
#define SD1SAT_TO_REMOVE_TBL_ENTRY_CC     7
#define SD1SAT_TO_ENABLE_MSG_CC           8
#define SD1SAT_TO_DISABLE_MSG_CC          9
#define SD1SAT_TO_ENABLE_GROUP_CC         10
#define SD1SAT_TO_DISABLE_GROUP_CC        11
#define SD1SAT_TO_ENABLE_ALL_CC           12
#define SD1SAT_TO_DISABLE_ALL_CC          13
#define SD1SAT_TO_SET_ROUTE_BY_MID_CC     14
#define SD1SAT_TO_SET_ROUTE_BY_GROUP_CC   15
#define SD1SAT_TO_MANAGE_TABLE_CC         16
#define SD1SAT_TO_ACTIVATE_ROUTES_CC      17
#define SD1SAT_TO_DEACTIVATE_ROUTES_CC    18
#define SD1SAT_TO_SET_ROUTE_PERIOD_CC     19
#define SD1SAT_TO_SET_WAKEUP_TIMEOUT_CC   20
#define SD1SAT_TO_OPEN_WINDOW_CC          30
#define SD1SAT_TO_CLOSE_WINDOW_CC         31
#define SD1SAT_TO_SET_BEACON_PATTERN_CC   32
#define SD1SAT_TO_BEACON_MUZZLE_CC        33
#define SD1SAT_TO_SET_FRAMING_CC          34

#define SD1SAT_TO_BEACON_PATTERN_MAX 5

typedef struct SD1SAT_PACK {
    uint16_t MsgId;
    uint16_t MsgLimit;
    uint16_t RouteMask;
    uint32_t GroupData;
    uint16_t Flag;
    uint16_t State;
} SD1SAT_TO_AddTblEntry_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t MsgId;
} SD1SAT_TO_MsgId_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t GroupData;
} SD1SAT_TO_Group_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t MsgId;
    uint16_t RouteMask;
} SD1SAT_TO_RouteByMid_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t GroupData;
    uint16_t RouteMask;
} SD1SAT_TO_RouteByGroup_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t RouteMask;
} SD1SAT_TO_RouteMask_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t RouteMask;
    uint16_t WakePeriod;
} SD1SAT_TO_RoutePeriod_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t WakeupTimeout;
} SD1SAT_TO_WakeupTimeout_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t Bytes;
    uint8_t  Profile;
} SD1SAT_TO_Window_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t Interval[SD1SAT_TO_BEACON_PATTERN_MAX];
} SD1SAT_TO_BeaconPattern_Payload_t;

typedef struct SD1SAT_PACK {
    uint16_t Ticks;
} SD1SAT_TO_BeaconMuzzle_Payload_t;

typedef struct SD1SAT_PACK {
    uint32_t Magic;
} SD1SAT_TO_Framing_Payload_t;

/* -------------------- UANT -------------------- */
#define SD1SAT_UANT_CMD_ID               0x19B0

#define SD1SAT_UANT_NOOP_CC              0
#define SD1SAT_UANT_RESET_COUNTERS_CC    1
#define SD1SAT_UANT_DEPLOY_CC            2
#define SD1SAT_UANT_ABORT_CC             3
#define SD1SAT_UANT_DEPLOY_NORMAL_CC     4
#define SD1SAT_UANT_DEPLOY_HARD_CC       5
#define SD1SAT_UANT_READ_RAW_CC          6
#define SD1SAT_UANT_WRITE_RAW_CC         7
#define SD1SAT_UANT_DEPLOY_AUTO_CC       8

typedef struct SD1SAT_PACK {
    uint8_t Algorithm;
    uint8_t Antennas;
} SD1SAT_UANT_Deploy_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Command;
} SD1SAT_UANT_WriteRaw_Payload_t;

/* -------------------- UTRX -------------------- */
#define SD1SAT_UTRX_CMD_ID                    0x19A0

#define SD1SAT_UTRX_NOOP_CC                   0
#define SD1SAT_UTRX_RESET_COUNTERS_CC         1
#define SD1SAT_UTRX_AX100_PING_CC             10
#define SD1SAT_UTRX_AX100_GNDWDT_CLEAR_CC     11
#define SD1SAT_UTRX_AX100_GNDWDT_GET_CC       12
#define SD1SAT_UTRX_AX100_GET_DIAG_CC         13
#define SD1SAT_UTRX_AX100_TABLE_GET_CC        14
#define SD1SAT_UTRX_AX100_SET_TX_BAUD_CC      15
#define SD1SAT_UTRX_AX100_SET_RX_BAUD_CC      16
#define SD1SAT_UTRX_AX100_REBOOT_CC           17
#define SD1SAT_UTRX_AX100_IDENT_CC            20
#define SD1SAT_UTRX_AX100_MEMFREE_CC          21
#define SD1SAT_UTRX_AX100_BUFFREE_CC          22
#define SD1SAT_UTRX_AX100_TABLE_SAVE_CC       23
#define SD1SAT_UTRX_AX100_TABLE_LOAD_CC       24
#define SD1SAT_UTRX_AX100_GET_CC              25
#define SD1SAT_UTRX_AX100_SET_CC              26
#define SD1SAT_UTRX_AX100_GET_RAW_CC          27
#define SD1SAT_UTRX_AX100_SET_RAW_CC          28
#define SD1SAT_UTRX_AX100_HK_SET_MODE_CC      30
#define SD1SAT_UTRX_AX100_HK_GET_MODE_CC      31

#define SD1SAT_UTRX_AX100_VAL_MAX 160

#define SD1SAT_AX100_TBL_SYS_IMAGE_SIZE 0x98u  /* 152 -- AX100_PARAM_SIZE    */
#define SD1SAT_AX100_TBL_RX_IMAGE_SIZE  0x50u  /*  80 -- AX100_RX_PARAM_SIZE */
#define SD1SAT_AX100_TBL_TX_IMAGE_SIZE  0x60u  /*  96 -- AX100_TX_PARAM_SIZE */
#define SD1SAT_AX100_TBL_TM_IMAGE_SIZE  0x40u  /*  64 -- AX100_TELEM_SIZE    */

typedef struct UTRX_AX100_SetTxBaud_Payload {
    uint32 Baud;
} UTRX_AX100_SetTxBaud_Payload_t;

typedef struct UTRX_AX100_SetRxBaud_Payload {
    uint32 Baud;
} UTRX_AX100_SetRxBaud_Payload_t;

/* ax100_hk_set_mode */
typedef struct UTRX_AX100_SetMode_Payload {
    uint8 Mode; /* ax100_hk_get_mode_t: 0 = CELL, 1 = SINGLE */
} UTRX_AX100_SetMode_Payload_t;

/* ax100_hk_get / ax100_table_dump / ax100_table_save / ax100_table_load */
typedef struct UTRX_AX100_Table_Payload {
    uint8 Table; /* AX100_TBL_SYS/RX/TM/TX */
} UTRX_AX100_Table_Payload_t;

/* ax100_get */
typedef struct UTRX_AX100_Get_Payload {
    uint8  Table;
    uint8  Spare;
    uint16 Addr;
    uint16 OutSize; /* requested read length in bytes */
} UTRX_AX100_Get_Payload_t;

/* ax100_set */
typedef struct UTRX_AX100_Set_Payload {
    uint8  Table;
    uint8  Spare;
    uint16 Addr;
    uint16 ValSize;
    uint8  Val[SD1SAT_UTRX_AX100_VAL_MAX];
} UTRX_AX100_Set_Payload_t;

/* ax100_get_raw */
typedef struct UTRX_AX100_GetRaw_Payload {
    uint8  Table;
    uint8  Type;
    uint16 Addr;
    uint16 ElemSize;
    uint16 Count;
} UTRX_AX100_GetRaw_Payload_t;

/* ax100_set_raw */
typedef struct UTRX_AX100_SetRaw_Payload {
    uint8  Table;
    uint8  Type;
    uint16 Addr;
    uint16 ValSize;
    uint8  Val[SD1SAT_UTRX_AX100_VAL_MAX];
} UTRX_AX100_SetRaw_Payload_t;

/* --- Telemetry --- */

typedef struct __attribute__((packed)) {
    uint32 TxCount;
    uint32 RxCount;
    uint32 TxBytes;
    uint32 RxBytes;
    uint32 TotalTxBytes;
    uint32 TotalRxBytes;
    uint32 TotalTxCount;
    uint32 TotalRxCount;
    uint32 BootCause;
    uint16 BootCount;
    int16  LastRSSI;
    uint8  ActiveConf;
} UTRX_AX100_Diagnosis_t;

typedef struct SD1SAT_PACK {
    uint8_t Mode; /* 0 = CELL, 1 = SINGLE */
} SD1SAT_UTRX_SetMode_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t Table; /* AX100_TBL_SYS/RX/TM/TX */
} SD1SAT_UTRX_Table_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Table;
    uint8_t  Spare;
    uint16_t Addr;
    uint16_t OutSize;
} SD1SAT_UTRX_Get_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Table;
    uint8_t  Spare;
    uint16_t Addr;
    uint16_t ValSize;
    uint8_t  Val[SD1SAT_UTRX_AX100_VAL_MAX];
} SD1SAT_UTRX_Set_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Table;
    uint8_t  Type;
    uint16_t Addr;
    uint16_t ElemSize;
    uint16_t Count;
} SD1SAT_UTRX_GetRaw_Payload_t;

typedef struct SD1SAT_PACK {
    uint8_t  Table;
    uint8_t  Type;
    uint16_t Addr;
    uint16_t ValSize;
    uint8_t  Val[SD1SAT_UTRX_AX100_VAL_MAX];
} SD1SAT_UTRX_SetRaw_Payload_t;

// CC 10
typedef struct SD1SAT_PACK {
    uint32_t return_ms;
} SD1SAT_UTRX_PingPayload_t;

// CC 12
typedef struct SD1SAT_PACK {
    uint32_t gnd_wdt_ms;
} SD1SAT_UTRX_GndWdtGetPayload_t;

// CC 13
typedef struct SD1SAT_PACK {
    uint32_t TxCount;
    uint32_t RxCount;
    uint32_t TxBytes;
    uint32_t RxBytes;
    uint32_t TotalTxBytes;
    uint32_t TotalRxBytes;
    uint32_t TotalTxCount;
    uint32_t TotalRxCount;
    uint32_t BootCause;
    uint16_t BootCount;
    int16_t  LastRSSI;
    uint8_t  ActiveConf;
} SD1SAT_UTRX_Diagnosis_t;

// CC 14
typedef struct SD1SAT_PACK {
    uint8_t buf[SD1SAT_UTRX_AX100_VAL_MAX];
} SD1SAT_UTRX_TableGetPayload_t;

// CC 20
typedef struct SD1SAT_PACK {
    char hostname[24];
    char model[32];
    char revision[24]; 
    char date[16];
    char time[12];
} SD1SAT_UTRX_ident_t;

// CC 21
typedef struct SD1SAT_PACK {
    uint32_t bytes;
} SD1SAT_UTRX_MemFreePayload_t;

// CC 22
typedef struct SD1SAT_PACK {
    uint32_t count;
} SD1SAT_UTRX_BufFreePayload_t;

// CC 25
typedef struct SD1SAT_PACK {
    uint8_t buf[SD1SAT_UTRX_AX100_VAL_MAX];
} SD1SAT_UTRX_GetPayload_t;

// CC 27
typedef struct SD1SAT_PACK {
    uint8_t buf[SD1SAT_UTRX_AX100_VAL_MAX];
} SD1SAT_UTRX_GetRawPayload_t;

// CC 30
typedef struct SD1SAT_PACK {
    uint8_t mode;
} SD1SAT_UTRX_HkSetModePayload_t;

// CC 31
typedef struct SD1SAT_PACK {
    uint8_t mode;
} SD1SAT_UTRX_HkGetModePayload_t;


/* -------------------- PAYSD1_LOTHAN ------------------ */
#define SD1SAT_PAYSD1_LOTHAN_CMD_ID                    0x19D0
#define SD1SAT_PAYSD1_LOTHAN_CMD_REPORT_DATA_MAX       166

#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_NOOP_CC      0
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_RESET_COUNTERS          1
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_TRIGGER_CAPTURE         2
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REGISTER_LOCATION       3
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_ROLLBACK_BASELINE       4
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_MARK_DOWNLINKED         5
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_POLICY              6
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_STATUS          7
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_DIGEST          8
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_THUMBNAIL       9
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_GET_POLICY              10
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_DOWNLINK_IMAGE  11
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPLINK_FILE             12
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPDATE_APPLY            13
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_UPDATE_ABORT            14
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_UPDATE_STATUS   15
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_TIME                16
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_SET_DIAGNOSTIC          17
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_RUN_GEOFENCE            18
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_SYNC_TIME_FROM_CFS      19
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_REQUEST_ORIGINAL_SIZE   20
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_SPLIT_ORIGINAL          21
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_DELETE_SPLIT            22
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_COUNT_SPLIT_FILES       23
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_START           24
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_START_RELATIVE  25
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_CAPTURE_STOP            26
#define SD1SAT_PAYSD1_LOTHAN_FunctionCode_OTA_PATCH_TEST          27

typedef struct SD1SAT_PACK
{
    uint16 LocId;
    uint32 AltitudeM;
    uint8  Flags;
} SD1SAT_PAYSD1_LOTHAN_TriggerCapture_Payload_t;

typedef struct SD1SAT_PACK
{
    uint16 LocId;
    int16  LatCentideg;
    int16  LonCentideg;
} SD1SAT_PAYSD1_LOTHAN_RegisterLocation_Payload_t;

typedef struct SD1SAT_PACK
{
    uint16 LocId;
} SD1SAT_PAYSD1_LOTHAN_LocId_Payload_t;

typedef struct SD1SAT_PACK
{
    uint16 CaptureId;
} SD1SAT_PAYSD1_LOTHAN_CaptureId_Payload_t;

typedef struct SD1SAT_PACK
{
    uint16 CaptureId;
    uint32 PartSize;
} SD1SAT_PAYSD1_LOTHAN_SplitOriginal_Payload_t;

typedef struct SD1SAT_PACK
{
    uint8 PolicyId;
    uint8 Value;
} SD1SAT_PAYSD1_LOTHAN_SetPolicy_Payload_t;

typedef struct SD1SAT_PACK
{
    uint8 PolicyId;
} SD1SAT_PAYSD1_LOTHAN_GetPolicy_Payload_t;

typedef struct SD1SAT_PACK
{
    uint16 TransferId;
} SD1SAT_PAYSD1_LOTHAN_TransferId_Payload_t;

typedef struct SD1SAT_PACK
{
    uint64 EpochMs;
} SD1SAT_PAYSD1_LOTHAN_SetTime_Payload_t;

typedef struct SD1SAT_PACK
{
    uint8 Enabled;
} SD1SAT_PAYSD1_LOTHAN_SetDiagnostic_Payload_t;

typedef struct SD1SAT_PACK
{
    uint64 StartTimeMs;
    uint32 RepeatEveryMs;
    uint16 RepeatCount;
    uint16 LocId;
    uint32 AltitudeM;
    uint8 Flags;
} SD1SAT_PAYSD1_LOTHAN_CaptureStart_Payload_t;

typedef struct SD1SAT_PACK
{
    uint64 DelayMs;
    uint32 RepeatEveryMs;
    uint16 RepeatCount;
    uint16 LocId;
    uint32 AltitudeM;
    uint8  Flags;
} PAYSD1_LOTHAN_CaptureStartRelative_Payload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_AckPayload
{
    uint8 OrigMsg;
    uint8 Status;
    uint16 RefId;
} SD1SAT_PAYSD1_LOTHAN_AckPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_StatusPayload
{
    uint16 NumLocations;
    uint32 DiskUsedKb;
    uint16 PendingDlKb;
} SD1SAT_PAYSD1_LOTHAN_StatusPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_CaptureCompletePayload
{
    uint16 CaptureId;
    uint16 LocId;
    uint16 ChangeScore;
    uint8 Flags;
    uint8 Confidence;
} SD1SAT_PAYSD1_LOTHAN_CaptureCompletePayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_DigestHeaderPayload
{
    uint32 Timestamp;
    uint8 TotalLocations;
    uint8 NoChange;
    uint8 Skipped;
    uint8 NumChanges;
} SD1SAT_PAYSD1_LOTHAN_DigestHeaderPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_DigestChangePayload
{
    uint16 LocId;
    uint16 ChangeScore;
    uint16 NdviCaptureId;
    uint8 Seq;
} SD1SAT_PAYSD1_LOTHAN_DigestChangePayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_DigestChangeBandsPayload
{
    uint16 NdviCaptureId;
    uint16 RedCaptureId;
    uint16 NirCaptureId;
    uint8 Seq;
} SD1SAT_PAYSD1_LOTHAN_DigestChangeBandsPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_DigestForeignObjectPayload
{
    uint16 LocId;
    uint16 CaptureId;
    uint8 DetectionCount;
    uint8 MaxPeakIntensity;
    uint8 Flags;
    uint8 Seq;
} SD1SAT_PAYSD1_LOTHAN_DigestForeignObjectPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_DigestEndPayload
{
    uint16 NumChanges;
    uint16 NumForeignObjects;
} SD1SAT_PAYSD1_LOTHAN_DigestEndPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_PolicyValuePayload
{
    uint8 PolicyId;
    uint8 Value;
} SD1SAT_PAYSD1_LOTHAN_PolicyValuePayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_ImageHeaderPayload
{
    uint16 CaptureId;
    uint16 TotalBytes;
    uint16 TotalChunks;
} SD1SAT_PAYSD1_LOTHAN_ImageHeaderPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_ImageDataPayload
{
    uint16 ChunkIndex;
    uint8 Data[6];
} SD1SAT_PAYSD1_LOTHAN_ImageDataPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_ImageEndPayload
{
    uint16 CaptureId;
    uint32 Crc32;
} SD1SAT_PAYSD1_LOTHAN_ImageEndPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_UplinkStatusPayload
{
    uint16 TransferId;
    uint8 Status;
} SD1SAT_PAYSD1_LOTHAN_UplinkStatusPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_UpdateStatusPayload
{
    uint8 State;
    uint8 TrialBoots;
    uint32 ShaPrefix;
} SD1SAT_PAYSD1_LOTHAN_UpdateStatusPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_OriginalSizePayload
{
    uint16 CaptureId;
    uint32 SizeBytes;
} SD1SAT_PAYSD1_LOTHAN_OriginalSizePayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_SplitResultPayload
{
    uint16 CaptureId;
    uint32 PartCount;
} SD1SAT_PAYSD1_LOTHAN_SplitResultPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_SplitFileCountPayload
{
    uint16 CaptureId;
    uint32 FileCount;
} SD1SAT_PAYSD1_LOTHAN_SplitFileCountPayload_t; /* CC 23 result; CAN RSP 0x27 SPLIT_FILE_COUNT */

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_OtaPatchTestPayload
{
    uint8 Value;
} SD1SAT_PAYSD1_LOTHAN_OtaPatchTestPayload_t; /* CC 27 result; CAN RSP 0x28 OTA_PATCH_TEST */

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_GeofenceResultPayload
{
    uint16 CaptureId;
    uint16 LocId;
    int32  LatitudeMicroDeg;
    int32  LongitudeMicroDeg;
    uint8  Inside;
} SD1SAT_PAYSD1_LOTHAN_GeofenceResultPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_TimeSyncResultPayload
{
    uint32 ObcSeconds;
    uint64 EpochMs;
} SD1SAT_PAYSD1_LOTHAN_TimeSyncResultPayload_t;

/*
 * Local copy of GPS_PosTlm_Payload_t. Keep this byte-for-byte compatible with
 * the GPS_POS_TLM_MID payload; do not include GPS application headers here.
 */
typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_GpsFixPayload
{
    double Latitude;
    double Longitude;
    uint32 Altitude;
    float  LatStd;
    float  LonStd;
    float  AltStd;
    uint32 SolStatus;
} SD1SAT_PAYSD1_LOTHAN_GpsFixPayload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_HkTlm_Payload
{
    uint8 CommandCounter;
    uint8 CommandErrorCounter;
    SD1SAT_PAYSD1_LOTHAN_AckPayload_t LastAck;
    SD1SAT_PAYSD1_LOTHAN_StatusPayload_t LastStatus;
    SD1SAT_PAYSD1_LOTHAN_CaptureCompletePayload_t LastCaptureComplete;
    SD1SAT_PAYSD1_LOTHAN_DigestHeaderPayload_t LastDigestHeader;
    SD1SAT_PAYSD1_LOTHAN_DigestChangePayload_t LastDigestChange;
    SD1SAT_PAYSD1_LOTHAN_DigestChangeBandsPayload_t LastDigestChangeBands;
    SD1SAT_PAYSD1_LOTHAN_DigestForeignObjectPayload_t LastDigestForeignObject;
    SD1SAT_PAYSD1_LOTHAN_DigestEndPayload_t LastDigestEnd;
    SD1SAT_PAYSD1_LOTHAN_PolicyValuePayload_t LastPolicyValue;
    SD1SAT_PAYSD1_LOTHAN_ImageHeaderPayload_t LastImageHeader;
    SD1SAT_PAYSD1_LOTHAN_ImageEndPayload_t LastImageEnd;
    SD1SAT_PAYSD1_LOTHAN_UplinkStatusPayload_t LastUplinkStatus;
    SD1SAT_PAYSD1_LOTHAN_UpdateStatusPayload_t LastUpdateStatus;
    SD1SAT_PAYSD1_LOTHAN_OriginalSizePayload_t LastOriginalSize;
    SD1SAT_PAYSD1_LOTHAN_SplitResultPayload_t LastSplitResult;
    SD1SAT_PAYSD1_LOTHAN_SplitFileCountPayload_t LastSplitFileCount;
    SD1SAT_PAYSD1_LOTHAN_OtaPatchTestPayload_t LastOtaPatchTest;
    SD1SAT_PAYSD1_LOTHAN_GeofenceResultPayload_t LastGeofenceResult;
    SD1SAT_PAYSD1_LOTHAN_TimeSyncResultPayload_t LastTimeSyncResult;
} SD1SAT_PAYSD1_LOTHAN_HkTlm_Payload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_BcnTlm_Payload
{
    uint8 empty;
} SD1SAT_PAYSD1_LOTHAN_BcnTlm_Payload_t;

typedef struct SD1SAT_PACK SD1SAT_PAYSD1_LOTHAN_CmdReport_Payload
{
    uint16 CmdMsgId;
    uint16 CmdSeq;
    int32  RetVal;
    uint8  CmdCode;
    uint8  RetType;
    uint16 DataLength;
    uint8  Data[SD1SAT_PAYSD1_LOTHAN_CMD_REPORT_DATA_MAX];
} SD1SAT_PAYSD1_LOTHAN_CmdReport_Payload_t;
/* -------------------- SD1SAT_PAYSD1_ROMA -------------------- */
#define SD1SAT_PAYSD1_ROMA_CMD_ID                    0x19E0

#define SD1SAT_PAYSD1_ROMA_NOOP_CC                   0
#define SD1SAT_PAYSD1_ROMA_RESET_COUNTERS_CC         1
#define SD1SAT_PAYSD1_ROMA_PING_CC                   2
#define SD1SAT_PAYSD1_ROMA_CLOCK_SYNC_CC             3
#define SD1SAT_PAYSD1_ROMA_GET_SPECIFIC_LINE_CC      4
#define SD1SAT_PAYSD1_ROMA_GET_MULTIPLE_LINES_CC     5
#define SD1SAT_PAYSD1_ROMA_GET_LATEST_LINE_CC        6
#define SD1SAT_PAYSD1_ROMA_GET_LATEST_N_LINES_CC     7
#define SD1SAT_PAYSD1_ROMA_CLEAR_ALL_LINES_CC        8
#define SD1SAT_PAYSD1_ROMA_GET_SINGLE_ENTRY_CC       9
#define SD1SAT_PAYSD1_ROMA_GET_MULTIPLE_ENTRIES_CC   10
#define SD1SAT_PAYSD1_ROMA_ADD_ENTRY_CC              11
#define SD1SAT_PAYSD1_ROMA_REMOVE_ENTRY_CC           12
#define SD1SAT_PAYSD1_ROMA_GET_USED_SLOTS_CC         13
#define SD1SAT_PAYSD1_ROMA_ADD_RELATIVE_ENTRY_CC     14
#define SD1SAT_PAYSD1_ROMA_SET_ROUTE_DEFAULT_CC      15
#define SD1SAT_PAYSD1_ROMA_RESET_ROUTE_CC            16
#define SD1SAT_PAYSD1_ROMA_LOAD_ROUTE_CC             17
#define SD1SAT_PAYSD1_ROMA_SAVE_ROUTE_CC             18
#define SD1SAT_PAYSD1_ROMA_SEND_ROUTE_CC             19
#define SD1SAT_PAYSD1_ROMA_SET_ROUTE_CC              20
#define SD1SAT_PAYSD1_ROMA_PAR_GET_CC                21
#define SD1SAT_PAYSD1_ROMA_PAR_SET_CC                22
#define SD1SAT_PAYSD1_ROMA_PAR_DEFAULTS_CC           23
#define SD1SAT_PAYSD1_ROMA_PAR_SAVE_CC               24
#define SD1SAT_PAYSD1_ROMA_PAR_RESTORE_CC            25
#define SD1SAT_PAYSD1_ROMA_PAR_LOAD_CC                26
#define SD1SAT_PAYSD1_ROMA_PAR_SET_OOB_CC            27
#define SD1SAT_PAYSD1_ROMA_SEND_COMMAND_CC           28
#define SD1SAT_PAYSD1_ROMA_SEND_RF_BYTES_CC          29
#define SD1SAT_PAYSD1_ROMA_SYNC_RX_CC                30
#define SD1SAT_PAYSD1_ROMA_SYNC_TX_CC                31
#define SD1SAT_PAYSD1_ROMA_PAY_INIT_CC               32
#define SD1SAT_PAYSD1_ROMA_LISTEN_START_CC           33
#define SD1SAT_PAYSD1_ROMA_LISTEN_STOP_CC            34
#define SD1SAT_PAYSD1_ROMA_SHOUT_START_CC            35
#define SD1SAT_PAYSD1_ROMA_SHOUT_START_RELATIVE_CC   36
#define SD1SAT_PAYSD1_ROMA_SHOUT_STOP_CC             37
#define SD1SAT_PAYSD1_ROMA_ISL_TEST_COUNT_UP_CC      38
#define SD1SAT_PAYSD1_ROMA_ISL_TEST_COUNT_LOAD_CC    39
#define SD1SAT_PAYSD1_ROMA_SET_DIAGNOSTIC_CC         40

#define SD1SAT_PAYSD1_ROMA_MAX_SCHEDULE_COMMAND_BYTES 64
#define SD1SAT_PAYSD1_ROMA_MAX_ROUTE_CHARS            127
#define SD1SAT_PAYSD1_ROMA_MAX_SHELL_COMMAND_CHARS    127
#define SD1SAT_PAYSD1_ROMA_RF_MAX_PAYLOAD_BYTES       252
#define SD1SAT_PAYSD1_ROMA_ISL_MAX_COMMAND_BYTES      235

typedef struct SD1SAT_PACK { uint16_t line_no; } SD1SAT_PAYSD1_ROMA_GetSpecificLine_Payload_t;
typedef struct SD1SAT_PACK { uint16_t line_start; uint16_t line_stop; } SD1SAT_PAYSD1_ROMA_GetMultipleLines_Payload_t;
typedef struct SD1SAT_PACK { uint16_t n_lines; } SD1SAT_PAYSD1_ROMA_GetLatestNLines_Payload_t;
typedef struct SD1SAT_PACK { uint16_t confirmation; } SD1SAT_PAYSD1_ROMA_ClearAllLines_Payload_t;

typedef struct SD1SAT_PACK { uint16_t entry_index; } SD1SAT_PAYSD1_ROMA_GetSingleEntry_Payload_t;
typedef struct SD1SAT_PACK { uint16_t start_index; uint16_t end_index; } SD1SAT_PAYSD1_ROMA_GetMultipleEntries_Payload_t;
typedef struct SD1SAT_PACK {
    uint64_t time_ms;
    uint32_t repeat_every_ms;
    int8_t   repeat_max;
    uint8_t  command[SD1SAT_PAYSD1_ROMA_MAX_SCHEDULE_COMMAND_BYTES];
} SD1SAT_PAYSD1_ROMA_AddEntry_Payload_t;
typedef struct SD1SAT_PACK {
    uint64_t delay_ms;
    uint32_t repeat_every_ms;
    int8_t   repeat_max;
    uint8_t  command[SD1SAT_PAYSD1_ROMA_MAX_SCHEDULE_COMMAND_BYTES];
} SD1SAT_PAYSD1_ROMA_AddRelativeEntry_Payload_t;
typedef struct SD1SAT_PACK { uint8_t entry_index; } SD1SAT_PAYSD1_ROMA_RemoveEntry_Payload_t;

typedef struct SD1SAT_PACK { char route[SD1SAT_PAYSD1_ROMA_MAX_ROUTE_CHARS]; } SD1SAT_PAYSD1_ROMA_SetRoute_Payload_t;

typedef struct SD1SAT_PACK { uint8_t table; uint8_t param; } SD1SAT_PAYSD1_ROMA_ParGet_Payload_t;
typedef struct SD1SAT_PACK { uint8_t table; uint8_t param; int32_t value; } SD1SAT_PAYSD1_ROMA_ParSet_Payload_t;
typedef struct SD1SAT_PACK { uint8_t table; } SD1SAT_PAYSD1_ROMA_ParTable_Payload_t;
typedef struct SD1SAT_PACK { uint8_t table; uint8_t enable; } SD1SAT_PAYSD1_ROMA_ParSetOob_Payload_t;

typedef struct SD1SAT_PACK { char cmd[SD1SAT_PAYSD1_ROMA_MAX_SHELL_COMMAND_CHARS]; } SD1SAT_PAYSD1_ROMA_SendCommand_Payload_t;
typedef struct SD1SAT_PACK {
    uint8_t Length;
    uint8_t Bytes[SD1SAT_PAYSD1_ROMA_RF_MAX_PAYLOAD_BYTES];
} SD1SAT_PAYSD1_ROMA_SendRfBytes_Payload_t;

typedef struct SD1SAT_PACK {
    uint64_t StartTimeMs;
    uint32_t RepeatEveryMs;
    uint16_t RepeatCount;
    uint8_t  Length;
    uint8_t  Bytes[SD1SAT_PAYSD1_ROMA_ISL_MAX_COMMAND_BYTES];
} SD1SAT_PAYSD1_ROMA_ShoutStart_Payload_t;

typedef struct SD1SAT_PACK {
    uint64_t DelayMs;
    uint32_t RepeatEveryMs;
    uint16_t RepeatCount;
    uint8_t  Length;
    uint8_t  Bytes[SD1SAT_PAYSD1_ROMA_ISL_MAX_COMMAND_BYTES];
} SD1SAT_PAYSD1_ROMA_ShoutStartRelative_Payload_t;

typedef struct SD1SAT_PACK { uint8_t Enabled; } SD1SAT_PAYSD1_ROMA_SetDiagnostic_Payload_t;

/*****************************************************************************************/

#define UELYSYS_PACK __attribute__((packed))

#define UELYSYS_TTC_PLATFORM_MAX_COMMAND_SIZE 128
#define UELYSYS_TTC_PLATFORM_MAX_CHUNK_SIZE   128
#define UELYSYS_MEOW_MISSION_MAX_CMD_LEN      80
#define UELYSYS_MEOW_MISSION_MAX_PATH_LEN     80
#define UELYSYS_MEOW_MISSION_MAX_WRITE_LEN    80
#define UELYSYS_MEOW_MISSION_MAX_READ_LEN     4096
#define UELYSYS_MEOW_MISSION_MAX_IFACE_NAME_LEN 16
#define UELYSYS_MEOW_MISSION_MAX_SYMBOL_LEN   64

#define MEOW_MISSION_MAX_CMD_LEN 128
#define MEOW_MISSION_MAX_PATH_LEN 48
#define MEOW_MISSION_MAX_WRITE_LEN 128
#define MEOW_MISSION_MAX_OFFSET 0x07FFFFFF
#define MEOW_MISSION_MAX_READ_LEN 512
#define MEOW_MISSION_SYS_SHUTDOWN_MAGIC 0x900DB007u
#define MEOW_MISSION_SHELL_FORCE_MAGIC 0xDEADC0DEu
#define MEOW_MISSION_SYS_FORCE_KILL_MAGIC 0xDEADFACEu

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
} UELYSYS_NoArgCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t arg;
} UELYSYS_U32ArgCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t ExecutionTimeAbsolute;
    uint16_t StalenessThreshold;
    uint16_t EntryId;
    uint16_t GroupId;
    uint8_t ExecutionType;
    uint16_t CommandSize;
    uint8_t Command[UELYSYS_TTC_PLATFORM_MAX_COMMAND_SIZE];
} UELYSYS_TTC_InsertAbsCmdEntryCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t ExecutionTimeRelative;
    uint16_t StalenessThreshold;
    uint16_t EntryId;
    uint16_t GroupId;
    uint8_t ExecutionType;
    uint16_t CommandSize;
    uint8_t Command[UELYSYS_TTC_PLATFORM_MAX_COMMAND_SIZE];
} UELYSYS_TTC_InsertRelCmdEntryCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EntryId;
    uint16_t GroupId;
} UELYSYS_TTC_DeleteEntryCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t GroupId;
} UELYSYS_TTC_DeleteGroupCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t GroupId;
    uint16_t EntryId;
    bool PersistentExecution;
} UELYSYS_TTC_ExecuteEntryCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t GroupId;
    bool PersistentExecution;
} UELYSYS_TTC_ExecuteGroupCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EntryId;
    uint16_t GroupId;
} UELYSYS_TTC_PlumbEntryInitCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EntryId;
    uint16_t GroupId;
    uint16_t Offset;
    uint16_t ChunkSize;
    uint8_t Data[UELYSYS_TTC_PLATFORM_MAX_CHUNK_SIZE];
} UELYSYS_TTC_PlumbEntryWriteCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EntryId;
    uint16_t GroupId;
    uint32_t TimeTag;
    uint16_t StalenessThreshold;
    uint16_t CmdSize;
    uint8_t TimeTagType;
    uint8_t ExecutionType;
} UELYSYS_TTC_PlumbEntryFinalizeCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[UELYSYS_MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t timeout_ms;
    uint8_t spare[4];
} UELYSYS_MEOW_ShellExecSyncCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t timeout_ms;
} SD1SAT_MEOW_ShellExecSyncCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[UELYSYS_MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
} UELYSYS_MEOW_ShellExecAsyncCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[MEOW_MISSION_MAX_PATH_LEN];
} SD1SAT_MEOW_ShellExecAsyncCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t pid;
} UELYSYS_MEOW_ShellPollCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t pid;
} SD1SAT_MEOW_ShellPollCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t pid;
    int32_t sig;
} UELYSYS_MEOW_ShellKillCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t pid;
    int32_t sig;
} SD1SAT_MEOW_ShellKillCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[UELYSYS_MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t timeout_ms;
    uint32_t magic;
} UELYSYS_MEOW_ShellExecSyncForceCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t timeout_ms;
    uint32_t magic;
} SD1SAT_MEOW_ShellExecSyncForceCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[UELYSYS_MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t magic;
} UELYSYS_MEOW_ShellExecAsyncForceCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char cmd[MEOW_MISSION_MAX_CMD_LEN];
    char redir_path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t magic;
} SD1SAT_MEOW_ShellExecAsyncForceCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t offset;
    uint32_t size;
} UELYSYS_MEOW_FileReadCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t offset;
    uint32_t size;
} SD1SAT_MEOW_FileReadCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t offset;
    uint16_t data_len;
    uint8_t flags;
    uint8_t spare;
    uint8_t data[UELYSYS_MEOW_MISSION_MAX_WRITE_LEN];
} UELYSYS_MEOW_FileWriteCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t offset;
    uint16_t data_len;
    uint8_t flags;
    uint8_t data[MEOW_MISSION_MAX_WRITE_LEN];
} SD1SAT_MEOW_FileWriteCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
} UELYSYS_MEOW_FilePathCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
} SD1SAT_MEOW_FilePathCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char src[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    char dst[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
} UELYSYS_MEOW_FileSrcDstCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char src[MEOW_MISSION_MAX_PATH_LEN];
    char dst[MEOW_MISSION_MAX_PATH_LEN];
} SD1SAT_MEOW_FileSrcDstCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    uint32_t n_bytes;
    uint32_t size;
} UELYSYS_MEOW_FileTailCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char path[MEOW_MISSION_MAX_PATH_LEN];
    uint32_t n_bytes;
    uint32_t size;
} SD1SAT_MEOW_FileTailCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t type;
    uint32_t magic;
} UELYSYS_MEOW_SysShutdownCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32_t type;
    uint32_t magic;
} SD1SAT_MEOW_SysShutdownCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t magic;
    int32_t mode;
} UELYSYS_MEOW_SysForceKillCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t magic;
    int32_t mode;
} SD1SAT_MEOW_SysForceKillCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int64_t sec;
    uint32_t nsec;
    uint8_t spare[4];
} UELYSYS_MEOW_SysTimeSetCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    int64_t sec;
    uint32_t nsec;
} SD1SAT_MEOW_SysTimeSetCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t timeout_ms;
} UELYSYS_MEOW_CspSetReadTimeoutCmd_t;

typedef struct SD1SAT_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t timeout_ms;
} SD1SAT_MEOW_CspSetReadTimeoutCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t port;
    char path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    char symbol[UELYSYS_MEOW_MISSION_MAX_SYMBOL_LEN];
} UELYSYS_MEOW_CspHandlerLoadCmd_t;


typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t port;
} UELYSYS_MEOW_CspHandlerClearCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t dst;
    uint8_t dst_port;
    uint8_t src_port;
    uint8_t prio;
    uint32_t timeout_ms;
    uint16_t len;
    uint8_t data[UELYSYS_MEOW_MISSION_MAX_WRITE_LEN];
} UELYSYS_MEOW_CspSendCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t host;
    uint8_t port;
    uint16_t timeout_ms;
    uint16_t chunk_size;
    char local_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
    char remote_path[UELYSYS_MEOW_MISSION_MAX_PATH_LEN];
} UELYSYS_MEOW_CspFtpCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char iface_name[UELYSYS_MEOW_MISSION_MAX_IFACE_NAME_LEN];
} UELYSYS_MEOW_CspIfstatsCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t dst;
    uint8_t mask;
    uint8_t via;
    uint8_t spare;
    char iface_name[UELYSYS_MEOW_MISSION_MAX_IFACE_NAME_LEN];
} UELYSYS_MEOW_CspRouteSetCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t idx;
    uint8_t dst_port;
    uint8_t src_node;
    uint8_t fwd_dst;
    uint8_t fwd_dst_port;
    uint8_t fwd_src_port;
    uint8_t active;
    uint8_t spare;
    uint16_t timeout_ms;
} UELYSYS_MEOW_CspRerouteSetCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t idx;
} UELYSYS_MEOW_CspRerouteClearCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t Addr;
} UELYSYS_UANT_AddrCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t Addr;
    uint8_t Channel;
    uint8_t Duration;
} UELYSYS_UANT_BurnChannelCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t ModuleId;
} UELYSYS_STX_SetModuleIdCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char filename_max[30];
    int8_t filename_len;
    uint32_t file_size;
} UELYSYS_STX_CREATEFILE_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char filename[64];
    uint32_t size;
    uint32_t offset;
    uint8_t interpacket_delay;
} UELYSYS_STX_WRITEFILE_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    char filename_max[30];
    int8_t filename_len;
} UELYSYS_STX_SENDFILE_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t CameraID;
    uint8_t ImageNumber;
} UELYSYS_PAYUEL_OBC_CamImageCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t CameraID;
    uint8_t ImageNumber;
    uint16_t ChunkNumber;
} UELYSYS_PAYUEL_OBC_ChunkDownloadCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t ImageSlot;
    uint8_t CameraNumber;
} UELYSYS_PAYUEL_CAM_ShotCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t ImageSlot;
    uint8_t ImageNumber;
} UELYSYS_PAYUEL_CAM_ImageCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t ImageSlot;
    uint8_t ImageNumber;
    uint16_t ChunkNumber;
} UELYSYS_PAYUEL_CAM_ChunkDownloadCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t DataID;
    uint8_t Spare[3];
} UELYSYS_LGBAT_RequestDataCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t reg_addr;
    uint16_t data;
} UELYSYS_PAYUEL_AOS_WriteRegisterCmd_t;

typedef struct UELYSYS_PACK {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t reg_addr;
} UELYSYS_PAYUEL_AOS_ReadRegisterCmd_t;

static_assert(sizeof(UELYSYS_STX_SetModuleIdCmd_t) == 10, "UELYSYS_STX_SetModuleIdCmd_t size mismatch");
static_assert(sizeof(UELYSYS_STX_CREATEFILE_t) == 43, "UELYSYS_STX_CREATEFILE_t size mismatch");
static_assert(sizeof(UELYSYS_STX_WRITEFILE_t) == 81, "UELYSYS_STX_WRITEFILE_t size mismatch");
static_assert(sizeof(UELYSYS_STX_SENDFILE_t) == 39, "UELYSYS_STX_SENDFILE_t size mismatch");

/*******************************************************************************************************************/


/*******************************************/
/*                                         */
/*                  TO APP                 */
/*                                         */
/*******************************************/


// typedef struct
// {
//     CFE_MSG_TelemetryHeader_t TelemetryHeader; /**< \brief Telemetry header */
//     TO_LAB_HkTlm_Payload_t    Payload;         /**< \brief Telemetry payload */
// } TO_LAB_HkTlm_t;

// /******************************************************************************/

// typedef struct
// {
//     CFE_MSG_TelemetryHeader_t  TelemetryHeader; /**< \brief Telemetry header */
//     TO_LAB_DataTypes_Payload_t Payload;         /**< \brief Telemetry payload */
// } TO_LAB_DataTypesTlm_t;

/******************************************************************************/

/*
 * The following commands do not have any payload,
 * but should still "reserve" a unique structure type to
 * employ a consistent handler pattern.
 *
 * This matches the pattern in CFE core and other modules.
 */


/*******************************************/
/*                                         */
/*              COMS Commands              */
/*                                         */
/*******************************************/


/*******************************************/
/*                                         */
/*                  UANT                   */
/*                                         */
/*******************************************/


    /* Burn control */

    /* Telemetry */


/*******************************************/
/*                                         */
/*               KISSCAM APP               */
/*                                         */
/*******************************************/
#define PAYBEE_KISSCAM_CMD_ID             0x1833

#define PAYBEE_KISSCAM_NOOP_CC             0
#define PAYBEE_KISSCAM_RESET_COUNTERS_CC   1

#define PAYBEE_KISSCAM_PING_CC             2
#define PAYBEE_KISSCAM_SET_MODE_CC         3
#define PAYBEE_KISSCAM_MEMORY_STATUS_CC    4
#define PAYBEE_KISSCAM_SET_EXPOSURE_CC     5
#define PAYBEE_KISSCAM_CAPTURE_CC          6
#define PAYBEE_KISSCAM_DOWNLOAD_CC         7
#define PAYBEE_KISSCAM_READ_REGISTER_CC    8
#define PAYBEE_KISSCAM_WRITE_REGISTER_CC   9
#define PAYBEE_KISSCAM_DOWNLOAD_ALL_CC     10
#define PAYBEE_KISSCAM_IMAGE_COMPRESS_CC   11


typedef struct PAYBEE_KISSCAM_Ping_Payload {
    /**
     * `0x00` for MCU only
     * `0x01` for MCU & Img sensor
     */
    uint8 PN;
} __attribute__((packed)) PAYBEE_KISSCAM_Ping_Payload_t;


/**
 * Set Mode Command
 */
typedef struct PAYBEE_KISSCAM_SetMode_Payload {
    /**
     * `0` for sleep
     * `1` for SD
     * `2` for HD
     */
    uint8 MD;
} __attribute__((packed)) PAYBEE_KISSCAM_SetMode_Payload_t;


/**
 * Memory Status Command
 * 
 * No Arg Command
 */


/**
 * Set Exposure Command
 */
typedef struct PAYBEE_KISSCAM_SetExposure_Payload {
    uint8 EX1;
    uint8 EX2;
} __attribute__((packed)) PAYBEE_KISSCAM_SetExposure_Payload_t;


/**
 * Capture Command
 */
typedef struct PAYBEE_KISSCAM_Capture_Payload {
    uint8 MEM; /* Memory slot number. In HD mode, this paramter is ignored */
    uint8 TST;
} __attribute__((packed)) PAYBEE_KISSCAM_Capture_Payload_t;


/**
 * Download Command
 * @param MEM Memory Slot: Can be `0` ~ `5`
 * @param PRE Preview Flag : `0` or `1`
 * @param LN1 MSB line number
 * @param LN2 LSB line number
 */
typedef struct PAYBEE_KISSCAM_Download_Payload {
    uint8 MEM;
    uint8 PRE;
    uint8 LN1;
    uint8 LN2;
} __attribute__((packed)) PAYBEE_KISSCAM_Download_Payload_t;


/**
 * Read Register Command
 */
typedef struct PAYBEE_KISSCAM_ReadRegister_Payload {
    uint8 AD1;
    uint8 AD2;
} __attribute__((packed)) PAYBEE_KISSCAM_ReadRegister_Payload_t;


/**
 * Write Register Command
 */
typedef struct PAYBEE_KISSCAM_WriteRegister_Payload {
    uint8 AD1;
    uint8 AD2;
    uint8 RG1;
    uint8 RG2;
} __attribute__((packed)) PAYBEE_KISSCAM_WriteRegister_Payload_t;


/**
 * Download All Command
 * @param MEM Memory Slot: Can be `0` ~ `5`
 * @param PRE Preview Flag : `0` or `1`
 * @param StartLine Starting line number: from `0` to `479`
 * @param LineNum Total number to download : `0` for one line download
 */
typedef struct PAYBEE_KISSCAM_DownloadAll_Payload {
    uint8 MEM;
    uint8 PRE;

    uint16_t StartLine;
    uint16_t LineNum;
} __attribute__((packed)) PAYBEE_KISSCAM_DownloadAll_Payload_t;


/**
 * Image Compress Command
 */
typedef struct PAYBEE_KISSCAM_ImageCompress_Payload {
    uint8_t MEM;
    uint8_t TargetIdx;
} __attribute__((packed)) PAYBEE_KISSCAM_ImageCompress_Payload_t;


typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
} __attribute__((packed)) PAYBEE_KISSCAM_NoopCmd_t;

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
} __attribute__((packed)) PAYBEE_KISSCAM_ResetCountersCmd_t;


/**
 * Ping Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_Ping_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_PingCmd_t;


/**
 * Set Mode Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_SetMode_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_SetModeCmd_t;


/**
 * Memory Status Command
 * 
 * No Arg Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
} __attribute__((packed)) PAYBEE_KISSCAM_MemoryStatusCmd_t;


/**
 * Set Exposure Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_SetExposure_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_SetExposureCmd_t;


/**
 * Capture Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_Capture_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_CaptureCmd_t;


/**
 * Download Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_Download_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_DownloadCmd_t;


/**
 * Read Register Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_ReadRegister_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_ReadRegisterCmd_t;


/**
 * Write Register Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_WriteRegister_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_WriteRegisterCmd_t;


/**
 * Download All Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_DownloadAll_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_DownloadAllCmd_t;

/**
 * Image Compress Command
 */
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    PAYBEE_KISSCAM_ImageCompress_Payload_t Payload;
} __attribute__((packed)) PAYBEE_KISSCAM_ImageCompressCmd_t;


// rename to lab


// delete to lab


/*************************************************5차 추가 부분************************************************************/

#define RPT_RET_VALUE_BUF_SIZE                      512  

typedef struct RPT_Report {

    uint16 MsgID;
    uint8 CommandCode;

    /**
     * `enum RPT_ReturnType_t`
     */
    uint8 ReturnType;
    int32 ReturnCode;

    /**
     * If cmd acquires **any kind of data**, use this member.
     * Ex 1) Get CMD -> earned values 
     *      (In this case, `ReturnType` must be `RPT_RETTYPE_SUCCESS`)
     * 
     * Ex 2) HW's Error packet -> that error values
     *      (In this case, `ReturnType` must be `RPT_RETTYPE_SUCCESS`)
     * 
     * Ex 3) Partially readed packet -> segmented packet
     *      (In this case, `ReturnType` must be `RPT_RETTYPE_CFE`
     *       and `ReturnCode` must be `CFE_SRL_PARTIAL_READ_ERR`)
     */
    uint16 ReturnDataSize;
    uint8 ReturnValue[RPT_RET_VALUE_BUF_SIZE];

} RPT_Report_t;

/***********************************************/
/*              PAYUEL_ROMA                    */
/***********************************************/
#define PAYUEL_ROMA_CMD_MID                             0x1830
#define PAYUEL_ROMA_SEND_HK_MID                         0x1831
#define PAYUEL_ROMA_SEND_BCN_MID                        0x1832

#define PAYUEL_ROMA_HK_TLM_MID                          0x0830
#define PAYUEL_ROMA_BCN_TLM_MID                         0x0831
#define PAYUEL_ROMA_REPORT_TLM_MID                      0x0832

#define PAYUEL_ROMA_NOOP_CC                         0
#define PAYUEL_ROMA_RESET_COUNTERS_CC               1
#define PAYUEL_ROMA_COMM_TEST_CC                    2
#define PAYUEL_ROMA_CLOCK_SYNC_CC                   3
#define PAYUEL_ROMA_LOG_TEST_CC                     4
#define PAYUEL_ROMA_TRANS_TEST_CC                   5

#define PAYUEL_ROMA_GET_SPECIFIC_LINE_CC            6
#define PAYUEL_ROMA_GET_MULTIPLE_LINES_CC           7
#define PAYUEL_ROMA_GET_LATEST_LINE_CC              8
#define PAYUEL_ROMA_GET_LATEST_N_LINES_CC           9
#define PAYUEL_ROMA_CLEAR_ALL_LINES_CC              10
#define PAYUEL_ROMA_GET_SINGLE_ENTRY_CC             11
#define PAYUEL_ROMA_GET_MULTIPLE_ENTRIES_CC          12
#define PAYUEL_ROMA_ADD_ENTRY_CC                    13
#define PAYUEL_ROMA_REMOVE_ENTRY_CC                 14
#define PAYUEL_ROMA_GET_USED_SLOTS_CC               15

#define PAYUEL_ROMA_SET_ROUTE_DEFAULT_CC            16
#define PAYUEL_ROMA_RESET_ROUTE_CC                  17
#define PAYUEL_ROMA_LOAD_ROUTE_CC                   18
#define PAYUEL_ROMA_SAVE_ROUTE_CC                   19
#define PAYUEL_ROMA_SEND_ROUTE_CC                   20
#define PAYUEL_ROMA_SET_ROUTE_CC                    21
#define PAYUEL_ROMA_PAR_GET_CC                      22
#define PAYUEL_ROMA_PAR_SET_CC                      23
#define PAYUEL_ROMA_PAR_DEFAULTS_CC                 24
#define PAYUEL_ROMA_PAR_SAVE_CC                     25
#define PAYUEL_ROMA_PAR_RESTORE_CC                  26
#define PAYUEL_ROMA_PAR_LOAD_CC                     27
#define PAYUEL_ROMA_PAR_SET_OOB_CC                  28
#define PAYUEL_ROMA_SEND_COMMAND_CC                 29
#define PAYUEL_ROMA_SEND_MSG_CC                     30
#define PAYUEL_ROMA_SYNC_RX_CC                      31
#define PAYUEL_ROMA_SYNC_TX_CC                      32
#define PAYUEL_ROMA_PAY_INIT_CC                     33
#define PAYUEL_ROMA_LISTEN_START_CC                 34
#define PAYUEL_ROMA_LISTEN_STOP_CC                  35
#define PAYUEL_ROMA_SHOUT_START_CC                  36
#define PAYUEL_ROMA_SHOUT_START_RELATIVE_CC         37
#define PAYUEL_ROMA_SHOUT_STOP_CC                   38
#define PAYUEL_ROMA_ISL_TEST_COUNT_UP_CC            39
#define PAYUEL_ROMA_ISL_TEST_COUNT_LOAD_CC          40

#define PAYUEL_ROMA_ISL_MAX_COMMAND_BYTES           235

/****************************TM**************************************/
typedef struct __attribute__((packed)) {
    uint8_t CmdCounter;
    uint8_t ErrCounter;
} payuel_roma_Noop_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t CmdCounter;
    uint8_t ErrCounter;
} payuel_roma_ResetCounters_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t RxBuf[16];
} payuel_roma_CommTest_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint16_t line_no;
    uint8_t  info;
    uint8_t  len;
    uint32_t time_s;
    uint16_t time_ms;
    uint16_t incr;
    char     line[256]; // FSW 상에 설정된 최대 값으로 크기 설정
} payuel_roma_GetSpecificLine_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t buffer[512];
    uint16_t offset;
} payuel_roma_GetMultipleLines_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint16_t line_no;
    uint8_t  info;
    uint8_t  len;
    uint32_t time_s;
    uint16_t time_ms;
    uint16_t incr;
    char     line[256]; // FSW 상에 설정된 최대 값으로 크기 설정
} payuel_roma_GetLatestLine_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t buffer[512];
    uint16_t offset;
} payuel_roma_GetLatest_N_Lines_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_SetRouteDefault_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ResetRoute_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_LoadRoute_tlm_payload_t;

typedef struct __attribute__((packed)) {
    char route[128]; // FSW 상에 설정된 최대 값으로 크기 설정
} payuel_roma_SaveRoute_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_SendRoute_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_SetRoute_tlm_payload_t;

typedef struct __attribute__((packed)) {
    int32_t value;
} payuel_roma_ParGet_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParSet_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParDefaults_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParSave_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParRestore_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParLoad_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_ParSetOOB_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} payuel_roma_SendCommand_tlm_payload_t;

/******************************TC***********************************/
// UELYSYS FSW declares this command family under a global pack(1).
#pragma pack(push, 1)
typedef struct // cc 0
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_ROMA_NoopCmd_t;

typedef struct // cc 1
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_ROMA_ResetCountersCmd_t;

typedef struct
{
    uint8_t           CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_ClockSyncCmd_t;

typedef struct // cc 2
{
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_CommTestCmd_t;

typedef struct
{
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_LogTestCmd_t;

typedef struct
{
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_TransTestCmd_t;

typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_SendBcnCmd_t;

/* LOG (PORT 16) */

typedef struct PAYUEL_ROMA_GetSpecificLine_Payload
{
    uint8 type;
    uint16 line_no;
} PAYUEL_ROMA_GetSpecificLine_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_GetMultipleLines_Payload
{
    uint8 type;
    uint16 line_start;
    uint16 line_stop;
} PAYUEL_ROMA_GetMultipleLines_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_GetLatestLine_Payload
{
    uint8 type;
} PAYUEL_ROMA_GetLatestLine_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_GetLatestNLines_Payload
{
    uint8 type;
    uint16 n_lines;
} PAYUEL_ROMA_GetLatestNLines_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ClearAllLines_Payload
{
    uint8 type;
    uint16 code;
} PAYUEL_ROMA_ClearAllLines_Cmd_Payload_t;

typedef struct __attribute__((packed)) { // cc 6
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];   /* Command header*/
    PAYUEL_ROMA_GetSpecificLine_Cmd_Payload_t    Payload;
} PAYUEL_ROMA_GetSpecificLineCmd_t;

typedef struct __attribute__((packed)) { // cc 7
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_GetMultipleLines_Cmd_Payload_t   Payload;
} PAYUEL_ROMA_GetMultipleLinesCmd_t;

typedef struct __attribute__((packed)) { // cc 8
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_GetLatestLine_Cmd_Payload_t      Payload;
} PAYUEL_ROMA_GetLatestLineCmd_t;

typedef struct __attribute__((packed)) { // cc 9
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_GetLatestNLines_Cmd_Payload_t    Payload;
} PAYUEL_ROMA_GetLatestNLinesCmd_t;

typedef struct __attribute__((packed)) { // cc 10
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ClearAllLines_Cmd_Payload_t      Payload;
} PAYUEL_ROMA_ClearAllLinesCmd_t;


/* ROUTING (PORT 18) */

typedef struct PAYUEL_ROMA_SetRouteDefault_Payload
{
    uint8 type;
} PAYUEL_ROMA_SetRouteDefault_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ResetRoute_Payload
{
    uint8 type;
} PAYUEL_ROMA_ResetRoute_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_LoadRoute_Payload
{
    uint8 type;
} PAYUEL_ROMA_LoadRoute_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_SaveRoute_Payload
{
    uint8 type;
} PAYUEL_ROMA_SaveRoute_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_SendRoute_Payload
{
    uint8 type;
} PAYUEL_ROMA_SendRoute_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_SetRoute_Payload
{
    uint8 type;
    char route[127];      // route 문자열 최대 길이 127, 전체 128 byte
} PAYUEL_ROMA_SetRoute_Cmd_Payload_t;

typedef struct __attribute__((packed)) { // cc 16
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SetRouteDefault_Cmd_Payload_t    Payload;
} PAYUEL_ROMA_SetRouteDefaultCmd_t;

typedef struct __attribute__((packed)) { // cc 17
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ResetRoute_Cmd_Payload_t         Payload;
} PAYUEL_ROMA_ResetRouteCmd_t;

typedef struct __attribute__((packed)) { // cc 18
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_LoadRoute_Cmd_Payload_t          Payload;
} PAYUEL_ROMA_LoadRouteCmd_t;

typedef struct __attribute__((packed)) { // cc 19
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SaveRoute_Cmd_Payload_t          Payload;
} PAYUEL_ROMA_SaveRouteCmd_t;

typedef struct __attribute__((packed)) { // cc 20
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SendRoute_Cmd_Payload_t          Payload;
} PAYUEL_ROMA_SendRouteCmd_t;

typedef struct __attribute__((packed)) { // cc 21
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SetRoute_Cmd_Payload_t           Payload;
} PAYUEL_ROMA_SetRouteCmd_t;


/* PARAMETERS (PORT 19) */

typedef struct PAYUEL_ROMA_ParGet_Payload
{
    uint8 type;
    uint8 table;
    uint8 param;
} PAYUEL_ROMA_ParGet_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParSet_Payload
{
    uint8 type;
    uint8 table;
    uint8 param;
    int32 value;
} PAYUEL_ROMA_ParSet_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParDefaults_Payload
{
    uint8 type;
    uint8 table;
} PAYUEL_ROMA_ParDefaults_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParSave_Payload
{
    uint8 type;
    uint8 table;
} PAYUEL_ROMA_ParSave_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParRestore_Payload
{
    uint8 type;
    uint8 table;
} PAYUEL_ROMA_ParRestore_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParLoad_Payload
{
    uint8 type;
    uint8 table;
} PAYUEL_ROMA_ParLoad_Cmd_Payload_t;

typedef struct PAYUEL_ROMA_ParSetOob_Payload
{
    uint8 type;
    uint8 table;
    uint8 enable;
} PAYUEL_ROMA_ParSetOOB_Cmd_Payload_t;

typedef struct __attribute__((packed)) { // cc 22
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParGet_Cmd_Payload_t             Payload;
} PAYUEL_ROMA_ParGetCmd_t;

typedef struct __attribute__((packed)) { // cc 23
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParSet_Cmd_Payload_t             Payload;
} PAYUEL_ROMA_ParSetCmd_t;

typedef struct __attribute__((packed)) { // cc 24
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParDefaults_Cmd_Payload_t        Payload;
} PAYUEL_ROMA_ParDefaultsCmd_t;

typedef struct __attribute__((packed)) { // cc 25
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParSave_Cmd_Payload_t            Payload;
} PAYUEL_ROMA_ParSaveCmd_t;

typedef struct __attribute__((packed)) { // cc 26
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParRestore_Cmd_Payload_t         Payload;
} PAYUEL_ROMA_ParRestoreCmd_t;

typedef struct __attribute__((packed)) { // cc 27
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParLoad_Cmd_Payload_t            Payload;
} PAYUEL_ROMA_ParLoadCmd_t;

typedef struct __attribute__((packed)) { // cc 28
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_ParSetOOB_Cmd_Payload_t          Payload;
} PAYUEL_ROMA_ParSetOOBCmd_t;


/* REMOTE TERMINAL (PORT 20) */

typedef struct PAYUEL_ROMA_SendCommand_Payload
{
    char cmd[128];                 // NOTE: must be NULL terminated
} PAYUEL_ROMA_SendCommand_Cmd_Payload_t;


typedef struct __attribute__((packed)) { // cc 29
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SendCommand_Cmd_Payload_t        Payload;
} PAYUEL_ROMA_SendCommandCmd_t;


/* PAYLOAD OPERATIONS (PORT 8) */

typedef struct PAYUEL_ROMA_SendMsg_Payload
{
    char msg[128];                 // NOTE: no need for null termination
} PAYUEL_ROMA_SendMsg_Cmd_Payload_t;

typedef struct __attribute__((packed)) { // cc 30
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PAYUEL_ROMA_SendMsg_Cmd_Payload_t        Payload;
} PAYUEL_ROMA_SendMsgCmd_t;

typedef struct __attribute__((packed)) { // cc 31
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_SyncRxCmd_t;

typedef struct __attribute__((packed)) { // cc 32
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_SyncTxCmd_t;

typedef struct __attribute__((packed)) { // cc 33
    uint8_t            CmdHeader[CFE_SB_CMD_HDR_SIZE];
} PAYUEL_ROMA_PayInitCmd_t;

/* CC 36/37 are variable-length commands.  Only the 15-byte prefix and the
 * number of Bytes selected by Length are placed on the wire. */
typedef struct PAYUEL_ROMA_ShoutStart_WirePayload
{
    uint64_t StartTimeMs;
    uint32_t RepeatEveryMs;
    uint16_t RepeatCount;
    uint8_t  Length;
    uint8_t  Bytes[PAYUEL_ROMA_ISL_MAX_COMMAND_BYTES];
} PAYUEL_ROMA_ShoutStart_WirePayload_t;

typedef struct PAYUEL_ROMA_ShoutStartRelative_WirePayload
{
    uint64_t DelayMs;
    uint32_t RepeatEveryMs;
    uint16_t RepeatCount;
    uint8_t  Length;
    uint8_t  Bytes[PAYUEL_ROMA_ISL_MAX_COMMAND_BYTES];
} PAYUEL_ROMA_ShoutStartRelative_WirePayload_t;

static_assert(offsetof(PAYUEL_ROMA_ShoutStart_WirePayload_t, Bytes) == 15,
              "PAYUEL_ROMA SHOUT_START wire prefix size mismatch");
static_assert(offsetof(PAYUEL_ROMA_ShoutStartRelative_WirePayload_t, Bytes) == 15,
              "PAYUEL_ROMA SHOUT_START_RELATIVE wire prefix size mismatch");
static_assert(sizeof(PAYUEL_ROMA_ShoutStart_WirePayload_t) == 250,
              "PAYUEL_ROMA SHOUT_START maximum wire payload size mismatch");
static_assert(sizeof(PAYUEL_ROMA_ShoutStartRelative_WirePayload_t) == 250,
              "PAYUEL_ROMA SHOUT_START_RELATIVE maximum wire payload size mismatch");

#pragma pack(pop)


/***********************************************/
/*              PAYUEL_LGPM                    */
/***********************************************/
#define PAYUEL_LGPM_CMD_ID                          0x1835
#define PAYUEL_LGPM_SEND_HK_ID                      0x1836
#define PAYUEL_LGPM_SEND_BCN_ID                     0x1837

#define PAYUEL_LGPM_HK_TLM_ID                       0x0835
#define PAYUEL_LGPM_REPORT_TLM_ID                   0x0836
#define PAYUEL_LGPM_BCN_TLM_ID                      0x0837

#define PAYUEL_LGPM_NOOP_CC                         0  
#define PAYUEL_LGPM_RESET_COUNTERS_CC               1  
#define PAYUEL_LGPM_MCU_ALIVE_CHECK_CC              2
#define PAYUEL_LGPM_3V3_PWR_ON_CC                   3
#define PAYUEL_LGPM_3V3_PWR_OFF_CC                  4
#define PAYUEL_LGPM_MAIN_BOOST_SW_ON_CC             5
#define PAYUEL_LGPM_MAIN_BOOST_SW_OFF_CC            6
#define PAYUEL_LGPM_SUB_BOOST_SW_ON_CC              7
#define PAYUEL_LGPM_SUB_BOOST_SW_OFF_CC             8
#define PAYUEL_LGPM_V28_MAIN_ON_CC                  9
#define PAYUEL_LGPM_V28_MAIN_OFF_CC                 10
#define PAYUEL_LGPM_V28_SUB_ON_CC                   11
#define PAYUEL_LGPM_V28_SUB_OFF_CC                  12
#define PAYUEL_LGPM_V12_MAIN_ON_CC                  13
#define PAYUEL_LGPM_V12_MAIN_OFF_CC                 14
#define PAYUEL_LGPM_PWR_SENSE_INFO_CC               15
#define PAYUEL_LGPM_PWR_SEQ_ON_CC                   16
#define PAYUEL_LGPM_PWR_SEQ_OFF_CC                  17
#define PAYUEL_LGPM_RWA_CONTROL_idx1_CC             18
#define PAYUEL_LGPM_RWA_CONTROL_idx2_CC             19
#define PAYUEL_LGPM_RWA_CONTROL_idx3_CC             20
#define PAYUEL_LGPM_RWA_PWR_ON_CC                   21
#define PAYUEL_LGPM_RWA_PWR_OFF_CC                  22
#define PAYUEL_LGPM_RWA_SENSE_INFO_CC               23
#define PAYUEL_LGPM_INTERVAL_REQUEST_PWR_SENSE_INFO_CC 24

/***********************TM*******************************/
typedef struct __attribute__((packed))
{
    uint8_t cmd_code;
    uint8_t sync;
    uint8_t length;
} PWR_CommonHeader_t;

typedef struct __attribute__((packed)){
    uint8 cmd_code;
    uint8 sync;      
    uint8 length;    
} RWA_CommonHeader_t;

// User Setting Structure : Used CC 0, 1 
typedef struct __attribute__((packed)) {
    uint8_t CmdCounter;
    uint8_t ErrCounter;
} PAYUEL_LGPM_Noop_tlm_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t CmdCounter;
    uint8_t ErrCounter;
} PAYUEL_LGPM_ResetCounters_tlm_payload_t;

// TM Packet Structure : Used CC 2
typedef struct PAYUEL_LGPM_MCU_ALIVE_CHECK_Tlm_Payload
{
    PWR_CommonHeader_t header;
    uint8 MCU_ALIVE_CHECK;
    uint32 Execution_Timestamp;    
    char Reply_Message[12];                        
    uint16 CRC16; 
} PAYUEL_LGPM_McuAliveCheck_tlm_payload_t;

// TM Packet Structure : Used CC 3
typedef struct PAYUEL_LGPM_Payload2OBC_3V3_PWR_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 PWR_ON_3V3;
    uint32 Execution_Timestamp;    
    char Reply_Message[30];                        
    uint16 CRC16; 
} PAYUEL_LGPM_3V3PwrOn_tlm_payload_t;

// TM Packet Structure : Used CC 4
typedef struct PAYUEL_LGPM_Payload2OBC_3V3_PWR_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 PWR_OFF_3V3;
    uint32 Execution_Timestamp;    
    char Reply_Message[30];                        
    uint16 CRC16; 
} PAYUEL_LGPM_3V3PwrOff_tlm_payload_t;

// TM Packet Structure : Used CC 5
typedef struct PAYUEL_LGPM_Payload2OBC_MAIN_BOOST_SW_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 MAIN_BOOST_SW_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[23];                       
    uint16 CRC16; 
} PAYUEL_LGPM_MainBoostSwOn_tlm_payload_t;

// TM Packet Structure : Used CC 6
typedef struct PAYUEL_LGPM_Payload2OBC_MAIN_BOOST_SW_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 MAIN_BOOST_SW_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[24];                       
    uint16 CRC16; 
} PAYUEL_LGPM_MainBoostSwOff_tlm_payload_t;

// TM Packet Structure : Used CC 7
typedef struct PAYUEL_LGPM_Payload2OBC_SUB_BOOST_SW_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 SUB_BOOST_SW_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[22];                   
    uint16 CRC16; 
} PAYUEL_LGPM_SubBoostSwOn_tlm_payload_t;

// TM Packet Structure : Used CC 8
typedef struct PAYUEL_LGPM_Payload2OBC_SUB_BOOST_SW_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 SUB_BOOST_SW_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[23];                       
    uint16 CRC16; 
} PAYUEL_LGPM_SubBoostSwOff_tlm_payload_t;

// TM Packet Structure : Used CC 9
typedef struct PAYUEL_LGPM_Payload2OBC_V28_MAIN_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 V28_MAIN_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[14];                      
    uint16 CRC16; 
} PAYUEL_LGPM_V28MainOn_tlm_payload_t;

// TM Packet Structure : Used CC 10
typedef struct PAYUEL_LGPM_Payload2OBC_V28_MAIN_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 V28_MAIN_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[15];                         
    uint16 CRC16; 
} PAYUEL_LGPM_V28MainOff_tlm_payload_t;

// TM Packet Structure : Used CC 11
typedef struct PAYUEL_LGPM_Payload2OBC_V28_SUB_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 V28_SUB_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[13];                        
    uint16 CRC16; 
} PAYUEL_LGPM_V28SubOn_tlm_payload_t;

// TM Packet Structure : Used CC 12
typedef struct PAYUEL_LGPM_Payload2OBC_V28_SUB_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 V28_SUB_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[14];                        
    uint16 CRC16; 
} PAYUEL_LGPM_V28SubOff_tlm_payload_t;

// TM Packet Structure : Used CC 13
typedef struct PAYUEL_LGPM_Payload2OBC_V12_MAIN_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 V12_MAIN_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[14];                          
    uint16 CRC16; 
} PAYUEL_LGPM_V12MainOn_tlm_payload_t;

// TM Packet Structure : Used CC 14
typedef struct PAYUEL_LGPM_Payload2OBC_V12_MAIN_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 V12_MAIN_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[15];                      
    uint16 CRC16; 
} PAYUEL_LGPM_V12MainOff_tlm_payload_t;

// TM Packet Structure : Used CC 15
typedef struct PAYUEL_LGPM_Payload2OBC_PWR_SENSE_INFO_Payload
{
    PWR_CommonHeader_t header;
    uint8 PWR_SENSE_INFO;
    uint32 Execution_Timestamp;    
    char Reply_Message[185];  
    uint16 CRC16;                           
} PAYUEL_LGPM_PwrSenseInfo_tlm_payload_t;

// TM Packet Structure : Used CC 16
typedef struct PAYUEL_LGPM_Payload2OBC_PWR_SEQ_ON_Payload
{
    PWR_CommonHeader_t header;
    uint8 PWR_SEQ_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[79];  
    uint16 CRC16;                           
} PAYUEL_LGPM_PwrSeqOn_tlm_payload_t;

// TM Packet Structure : Used CC 17
typedef struct PAYUEL_LGPM_Payload2OBC_PWR_SEQ_OFF_Payload
{
    PWR_CommonHeader_t header;
    uint8 PWR_SEQ_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[83]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_PwrSeqOff_tlm_payload_t;

// TM Packet Structure : Used CC 18
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_CONTROL_idx1_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_CONTROL;
    uint32 Execution_Timestamp;    
    char Reply_Message[14]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaControlIdx1_tlm_payload_t;

// TM Packet Structure : Used CC 19
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_CONTROL_idx2_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_CONTROL;
    uint32 Execution_Timestamp;    
    char Reply_Message[14]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaControlIdx2_tlm_payload_t;

// TM Packet Structure : Used CC 20
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_CONTROL_idx3_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_CONTROL;
    uint32 Execution_Timestamp;    
    char Reply_Message[14]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaControlIdx3_tlm_payload_t;

// TM Packet Structure : Used CC 21
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_PWR_ON_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_PWR_ON;
    uint32 Execution_Timestamp;    
    char Reply_Message[42]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaPwrOn_tlm_payload_t;

// TM Packet Structure : Used CC 22
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_PWR_OFF_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_PWR_OFF;
    uint32 Execution_Timestamp;    
    char Reply_Message[46]; 
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaPwrOff_tlm_payload_t;

// TM Packet Structure : Used CC 23
typedef struct PAYUEL_LGPM_Payload2OBC_RWA_SENSE_INFO_Payload
{
    RWA_CommonHeader_t header;
    uint8 RWA_SENSE_INFO;
    uint32 Execution_Timestamp;    
    char Reply_Message[258];   
    uint16 CRC16;                           
} PAYUEL_LGPM_RwaSenseInfo_tlm_payload_t;

/************************************TC***********************************/

// 0. CC 0 : NOOP (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_NoopCmd_t;

// 1. CC 1 : RESET_COUNTERS (No Args) 
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_ResetCountersCmd_t;

// 2. CC 2 : MCU_ALIVE_CHECK (With Payload)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32 Ground_UTC_Time;
} PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t;

// 3. CC 3 : 3V3_PWR_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_3V3_PWR_ON_Cmd_t;

// 4. CC 4 : 3V3_PWR_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t;

// 5. CC 5 : MAIN_BOOST_SW_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t;

// 6. CC 6 : MAIN_BOOST_SW_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t;

// 7. CC 7 : SUB_BOOST_SW_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t;

// 8. CC 8 : SUB_BOOST_SW_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t;

// 9. CC 9 : V28_MAIN_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V28_MAIN_ON_Cmd_t;

// 10. CC 10 : V28_MAIN_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t;

// 11. CC 11 : V28_SUB_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V28_SUB_ON_Cmd_t;

// 12. CC 12 : V28_SUB_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V28_SUB_OFF_Cmd_t;

// 13. CC 13 : V12_MAIN_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V12_MAIN_ON_Cmd_t;

// 14. CC 14 : V12_MAIN_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t;

// 15. CC 15 : PWR_SENSE_INFO (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t;

// 16. CC 16 : PWR_SEQ_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t;

// 17. CC 17 : PWR_SEQ_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t;

typedef struct PAYUEL_LGPM_RWA_CONTROL_idx1_CMD_Payload {     
    int16 TargetSpeed_RPM;    
    int16 TargetAcc_RPM;
    int16 Operating_Time;
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx1_CMD_Payload_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
    PAYUEL_LGPM_RWA_CONTROL_idx1_CMD_Payload_t  payload;
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t;


// 19. CC 19 : RWA_CONTROL_idx2
typedef struct PAYUEL_LGPM_RWA_CONTROL_idx2_CMD_Payload {     
    uint8 Motor_power;
    uint8 reseved[5]; // 5바이트 패딩
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx2_CMD_Payload_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
    PAYUEL_LGPM_RWA_CONTROL_idx2_CMD_Payload_t  payload;
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t;


// 20. CC 20 : RWA_CONTROL_idx3
typedef struct PAYUEL_LGPM_RWA_CONTROL_idx3_CMD_Payload {     
    uint8 Motor_driver_EN;
    uint8 reseved[5]; // 5바이트 패딩
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx3_CMD_Payload_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
    PAYUEL_LGPM_RWA_CONTROL_idx3_CMD_Payload_t  payload;
} __attribute__((packed)) PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t;

// 21. CC 21 : RWA_PWR_ON (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_RWA_PWR_ON_Cmd_t;

// 22. CC 22 : RWA_PWR_OFF (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t;

// 23. CC 23 : RWA_SENSE_INFO (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t;

// 24. CC 24 : INTERVAL_REQUEST_PWR_SENSE_INFO (No Args)
typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_INTERVAL_PWR_SENSE_INFO_Cmd_t;

static_assert(sizeof(PAYUEL_LGPM_INTERVAL_PWR_SENSE_INFO_Cmd_t) == CFE_SB_CMD_HDR_SIZE,
              "LGPM interval power-sense command must contain only the cFE command header");

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_ProcessCmd_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_DisplayParamCmd_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_NativeCANCmd_t;

typedef struct {
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; /**< \brief Command header */
} PAYUEL_LGPM_SendHkCmd_t;


/***********************************************/
/*              PAYUEL_LGBAT                   */
/***********************************************/
#define PAYUEL_LGBAT_CMD_ID                                0x18C6
#define PAYUEL_LGBAT_SEND_HK_ID                            0x18C7
#define PAYUEL_LGBAT_SEND_BCN_ID                           0x18C8
#define PAYUEL_LGBAT_HK_TLM_ID                             0x08C6
#define PAYUEL_LGBAT_REPORT_TLM_ID                         0x08C7
#define PAYUEL_LGBAT_REPORT_TLM_ID                         0x08C8

#define LGBAT_NOOP_CC                               0  
#define LGBAT_RESET_COUNTER_CC                      1  
#define LGBAT_SEND_BCN_CC                           2  
#define LGBAT_REQUEST_DATA_CC                       3  
#define LGBAT_REQUEST_ALL_DATA_CC                   4  
#define LGBAT_SET_POWER_CC                          5  
#define LGBAT_RESET_BMS_CC                          6 

/* HK Telemetry Payload (MID 0x08C6) */
typedef struct {
    uint8_t  CmdCounter;
    uint8_t  CmdErrCounter;
    uint8_t  PowerApplied;
    uint8_t  MissionActive;
    uint8_t  FirstCommDone;
    uint8_t  Spare[3];
    uint16_t Pack_Voltage_mV;
    int16_t  Pack_Current_mA;
    uint16_t SOC_x100;
    uint8_t  SOH_pct;
    uint8_t  Power_Supply_Status;
    uint8_t  Failure_Level;
    uint8_t  FailStatus2_Raw;
    uint8_t  FailStatus3_Raw;
} __attribute__((packed)) LGBAT_HkTlm_Payload_t;

/* Beacon Telemetry Payload (MID 0x08C8) */
typedef struct {
    uint8_t  CmdCounter;
    uint8_t  CmdErrCounter;
    uint8_t  PowerApplied;
    uint8_t  Spare;

    uint16_t Pack_Voltage_mV;
    int16_t  Pack_Current_mA;
    uint16_t Avg_Time_To_Empty_sec;
    uint16_t Avg_Time_To_Full_sec;

    uint16_t SOC_x100;
    uint8_t  SOH_pct;
    uint8_t  Power_Supply_Status;
    uint16_t RC_Remaining_Cap;
    uint16_t AE_Available_Energy;

    uint16_t System_Max_Voltage;
    uint8_t  Power_Supply_Health;
    uint8_t  Spare2;
    uint16_t FETTestRequiredVoltage;

    uint16_t Charge_Percentage;
    uint16_t Design_Capacity;
    uint16_t Capacity;
    uint16_t Charge;

    uint16_t Cell_Voltage_Max_mV;
    uint16_t Cell_Voltage_Min_mV;
    int16_t  Cell_Temp_Max_x10;
    int16_t  Cell_Temp_Min_x10;

    uint16_t Cell_Voltage_01_mV;
    uint16_t Cell_Voltage_02_mV;

    /* ICD 0x07: byte[1-2]=T02 stored first, byte[3-4]=T01 stored second */
    int16_t  Cell_Temp_02_x10;
    int16_t  Cell_Temp_01_x10;
    int16_t  Balancing_R_Temp_x10;
    int16_t  PreCharge_R_Temp_x10;

    int16_t  FET_Down_Temp_x10;
    int16_t  FET_Up_Temp_x10;
    uint16_t CtrlCBStatus;
    uint8_t  SoftVersion;
    uint8_t  Spare3;

    uint8_t  Failure_Level;
    uint8_t  DFET_Status;
    uint8_t  CFET_Status;
    uint8_t  Voltage_Diag;
    uint8_t  Current_Diag;
    uint8_t  Temp_Diag;

    uint8_t  FailStatus2_Raw;
    uint8_t  FailStatus3_Raw;

    uint16_t BOOST_Out_Voltage_mV;
    uint16_t MCU_B_Plus_Volt_mV;
    uint16_t MCU_P_Plus_Volt_mV;
    uint16_t MCU_BMIC_REG_Out_Volt_mV;

    int16_t  BMS_Current_mA;
    int16_t  PCB_Temperature_x10;
    int16_t  BOOST_Temperature_x10;
    uint16_t MCU_SBC_AMUX_Voltage_mV;

} __attribute__((packed)) LGBAT_BcnTlm_Payload_t;
/*************************************************************************************************************************** */

/* BEE1012 AIOBC commands (restored from 6847e6aac^). */
#define AIOBC_PADDING_LENGTH 180

typedef struct AIOBC_HealthChckCmd
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t padding[AIOBC_PADDING_LENGTH];
} __attribute__((packed)) AIOBC_HealthChckCmd_t;

typedef struct AIOBC_DataChunkCmd
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EnvelopeMagic;
    uint16_t flags;
    uint16_t chunk_seq;
    uint16_t total_chunks;
    uint16_t RawDataLength;
    uint32_t ChunkCRC32;
    float Data[34];
} __attribute__((packed)) AIOBC_DataChunkCmd_t;

typedef struct AIOBC_AIModeCmd
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t EnvelopeMagic;
    uint16_t flags;
    uint16_t chunk_seq;
    uint16_t total_chunks;
    uint16_t RawDataLength;
    uint32_t ChunkCRC32;
    uint32_t scenario_id;
} __attribute__((packed)) AIOBC_AIModeCmd_t;

typedef struct AIOBC_DataEndCmd
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16_t Argument;
    uint8_t reserved[178];
} __attribute__((packed)) AIOBC_DataEndCmd_t;

typedef struct AIOBC_FDIRCmd
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t scenarioid;
    uint8_t commandid;
    uint32_t flags;
} __attribute__((packed)) AIOBC_FDIRCmd_t;

static_assert(sizeof(AIOBC_HealthChckCmd_t) == 188, "AIOBC health-check command size mismatch");
static_assert(sizeof(AIOBC_DataChunkCmd_t) == 158, "AIOBC data-chunk command size mismatch");
static_assert(sizeof(AIOBC_AIModeCmd_t) == 26, "AIOBC AI-mode command size mismatch");
static_assert(sizeof(AIOBC_DataEndCmd_t) == 188, "AIOBC data-end command size mismatch");
static_assert(sizeof(AIOBC_FDIRCmd_t) == 17, "AIOBC FDIR command size mismatch");


static_assert(sizeof(AIOBC_FDIRCmd_t) == 17, "AIOBC FDIR command size mismatch");

// TO_LAB Enable Output (목적지 IP 포함, BEE1012/UELYSYS). BEE-1000 은 YonseiGS_config.h 의 TO_LAB_EnableOutputCmd_t (payload 없음)
typedef struct
{
    uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE];
    TO_LAB_EnableOutput_Payload_t Payload;
} TO_LAB_EnableOutputIpCmd_t;
