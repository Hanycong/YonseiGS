// BEE-1000 미션 명령 정의
//
// MID / CC 는 명령 생성기의 기본값과 같은 식(매크로)을 쓴다. 매크로는 miman_config.h 에 있다.
// 결함 표시(Dn)는 docs/CMDGEN_ANALYSIS.md §4 참고.
#include "command_catalog.h"
#include "miman_config.h"

namespace mim {
namespace tc {

static const CommandDef kCommands[] = {
    // id   label                                      mission    app        category          mid                          cc                                   kind              criticality            implemented
    {  10, "EPS NoOp",                                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_NOOP_CC,                         CmdKind::Single,  Criticality::Query,       true },
    {  11, "EPS Reset Counters",                      "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_RESET_COUNTERS_CC,               CmdKind::Single,  Criticality::Config,      true },
    {  12, "EPS Get Counters",                        "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_GET_COUNTERS_CC,                 CmdKind::Single,  Criticality::Query,       true },
    {  13, "EPS Get App Data",                        "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_GET_APPDATA_CC,                  CmdKind::Single,  Criticality::Query,       true },
    {  14, "EPS Report App Data",                     "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_REPORT_APPDATA_CC,               CmdKind::Single,  Criticality::Query,       true },
    {  15, "EPS_P60_Dock Set Channel Single",         "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_SET_CHANNEL_SINGLE_CC,  CmdKind::Single,  Criticality::Hazardous,   true },
    {  16, "EPS_P60_Dock Get Channel Single",         "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_GET_CHANNEL_SINGLE_CC,  CmdKind::Single,  Criticality::Query,       true },
    {  17, "EPS_P60_Dock Set Channels",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_SET_CHANNELS_CC,        CmdKind::Single,  Criticality::Hazardous,   true },
    {  18, "EPS_P60_Dock Get Channels",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_GET_CHANNELS_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  19, "EPS_P60_PDU Set Channel Single",          "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_SET_CHANNEL_SINGLE_CC,   CmdKind::Single,  Criticality::Hazardous,   true },
    {  20, "EPS_P60_PDU Get Channel Single",          "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_GET_CHANNEL_SINGLE_CC,   CmdKind::Single,  Criticality::Query,       true },
    {  21, "EPS_P60_PDU Set Channels",                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_SET_CHANNELS_CC,         CmdKind::Single,  Criticality::Hazardous,   true },
    {  22, "EPS_P60_PDU Get Channels",                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_GET_CHANNELS_CC,         CmdKind::Single,  Criticality::Query,       true },
    {  23, "EPS_P60_ACU Set MPPT Mode",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_SET_MPPT_MODE_CC,        CmdKind::Single,  Criticality::Config,      true },
    {  24, "EPS_P60_ACU Get MPPT Mode",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_GET_MPPT_MODE_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  25, "EPS_P60_Dock Get Table HK",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_GET_TABLE_HK_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  26, "EPS_P60_Dock Get Table Conf",             "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_GET_TABLE_CONF_CC,      CmdKind::Single,  Criticality::Query,       true },
    {  27, "EPS_P60_Dock Get Table Cal",              "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_GET_TABLE_CAL_CC,       CmdKind::Single,  Criticality::Query,       true },
    {  28, "EPS_P60_PDU Get Table HK",                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_GET_TABLE_HK_CC,         CmdKind::Single,  Criticality::Query,       true },
    {  29, "EPS_P60_PDU Get Table Conf",              "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_GET_TABLE_CONF_CC,       CmdKind::Single,  Criticality::Query,       true },
    {  30, "EPS_P60_PDU Get Table Cal",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_GET_TABLE_CAL_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  31, "EPS_P60_ACU Get Table HK",                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_GET_TABLE_HK_CC,         CmdKind::Single,  Criticality::Query,       true },
    {  32, "EPS_P60_ACU Get Table Conf",              "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_GET_TABLE_CONF_CC,       CmdKind::Single,  Criticality::Query,       true },
    {  33, "EPS_P60_ACU Get Table Cal",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_GET_TABLE_CAL_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  34, "EPS_P60_Dock Reset GND WDT",              "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_DOCK_RESET_GND_WDT_CC,       CmdKind::Single,  Criticality::Config,      true },
    {  35, "EPS_P60_PDU Reset GND WDT",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_PDU_RESET_GND_WDT_CC,        CmdKind::Single,  Criticality::Config,      true },
    {  36, "EPS_P60_ACU Reset GND WDT",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_ACU_RESET_GND_WDT_CC,        CmdKind::Single,  Criticality::Config,      true },
    {  37, "EPS_P60 Get Param",                       "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_GET_PARAM_CC,                CmdKind::Single,  Criticality::Query,       true },
    {  38, "EPS_P60 Get Param Array",                 "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_GET_PARAM_ARRAY_CC,          CmdKind::Single,  Criticality::Query,       true },
    {  39, "EPS_P60 Set Param",                       "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_SET_PARAM_CC,                CmdKind::Single,  Criticality::Config,      true },
    {  40, "EPS_P60 Get Table",                       "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_GET_TABLE_CC,                CmdKind::Single,  Criticality::Query,       true },
    {  41, "EPS_P60 Load Table",                      "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_LOAD_TABLE_CC,               CmdKind::Single,  Criticality::Config,      true },
    {  42, "EPS_P60 Save Table",                      "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_SAVE_TABLE_CC,               CmdKind::Single,  Criticality::Config,      true },
    {  43, "EPS_P60 Table Get Static",                "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_TABLE_GET_STATIC_CC,         CmdKind::Single,  Criticality::Query,       true },
    {  44, "EPS_P60 Table Dump Static",               "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_TABLE_DUMP_STATIC_CC,        CmdKind::Single,  Criticality::Query,       true },
    {  45, "EPS_P60 Get Dock Info",                   "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_GET_DOCK_INFO_CC,            CmdKind::Single,  Criticality::Query,       true },
    {  46, "EPS_P60 Reset",                           "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_RESET_CC,                    CmdKind::Single,  Criticality::Hazardous,   true },
    {  47, "ADCS NoOp",                               "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_NOOP_CC,                        CmdKind::Single,  Criticality::Query,       true },
    {  48, "ADCS Reset Counters",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_RESET_COUNTERS_CC,              CmdKind::Single,  Criticality::Config,      true },
    {  49, "ADCS Reset App Cmd Counters",             "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_RESET_APP_CMD_COUNTERS_CC,      CmdKind::Single,  Criticality::Config,      true },
    {  50, "ADCS Reset Device Cmd Counters",          "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_RESET_DEVICE_CMD_COUNTERS_CC,   CmdKind::Single,  Criticality::Config,      true },
    {  51, "ADCS Set Communication Mode (CAN)",       "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_COMMUNICATION_MODE_AS_CAN_CC, CmdKind::Single,  Criticality::Hazardous,   true },
    {  52, "ADCS GPIO Enable High",                   "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_GPIO_ENABLE_HIGH_CC,            CmdKind::Single,  Criticality::Hazardous,   true },
    {  53, "ADCS GPIO Enable Low",                    "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_GPIO_ENABLE_LOW_CC,             CmdKind::Single,  Criticality::Hazardous,   true },
    {  54, "ADCS GPIO Boot High",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_GPIO_BOOT_HIGH_CC,              CmdKind::Single,  Criticality::Hazardous,   true },
    {  55, "ADCS GPIO Boot Low",                      "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_GPIO_BOOT_LOW_CC,               CmdKind::Single,  Criticality::Hazardous,   true },
    {  56, "ADCS Exit Bootloader",                    "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_EXIT_BOOTLOADER_CC,             CmdKind::Single,  Criticality::Hazardous,   true },
    {  57, "ADCS Reset",                              "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_RESET_CC,                   CmdKind::Single,  Criticality::Hazardous,   true },
    {  58, "ADCS Set Current Unix Time",              "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_CURRENT_UNIX_TIME_CC,       CmdKind::Single,  Criticality::Config,      true },
    {  59, "ADCS Set Error Log Setting",              "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_ERROR_LOG_SETTING_CC,       CmdKind::Single,  Criticality::Config,      true },
    {  60, "ADCS Persist Config",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_PERSIST_CONFIG_CC,          CmdKind::Single,  Criticality::Config,      true },
    {  61, "ADCS Set Control & Estimation Mode",      "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_CONTROL_ESTIMATION_MODE_CC, CmdKind::Single,  Criticality::Config,      true },
    {  62, "ADCS Disable Mag/RWL Maintenance",        "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_DISABLE_MAG_RWL_MNT_MNG_CC, CmdKind::Single,  Criticality::Config,      true },
    {  63, "ADCS Set Reference IRC Vector",           "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_REFERENCE_IRC_VECTOR_CC,    CmdKind::Single,  Criticality::Config,      true },
    {  64, "ADCS Set Reference LLH Target",           "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_REFERENCE_LLH_TARGET_CC,    CmdKind::Single,  Criticality::Config,      true },
    {  65, "ADCS Set Orbit Mode",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_ORBIT_MODE_CC,              CmdKind::Single,  Criticality::Config,      true },
    {  66, "ADCS Set Mag Deploy",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_MAG_DEPLOY_CMD_CC,          CmdKind::Single,  Criticality::Hazardous,   true },
    {  67, "ADCS Set Reference RPY",                  "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_REFERENCE_RPY_VALUES_CC,    CmdKind::Single,  Criticality::Config,      true },
    {  68, "ADCS Set Open-Loop MTQ Command",          "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_OPENLOOPCMD_MTQ_CC,         CmdKind::Single,  Criticality::Config,      true },
    {  69, "ADCS Set Power State",                    "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_POWER_STATE_CC,             CmdKind::Single,  Criticality::Hazardous,   true },
    {  70, "ADCS Set Run Mode",                       "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_RUN_MODE_CC,                CmdKind::Single,  Criticality::Config,      true },
    {  71, "ADCS Set Control Mode",                   "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_CONTROL_MODE_CC,            CmdKind::Single,  Criticality::Config,      true },
    {  72, "ADCS Set Wheel Config",                   "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_WHL_CONFIG_CC,              CmdKind::Single,  Criticality::Config,      true },
    {  73, "ADCS Set Satellite Config",               "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_SATELLITE_CONFIG_CC,        CmdKind::Single,  Criticality::Config,      true },
    {  74, "ADCS Set Controller Config",              "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_CONTROLLER_CONFIG_CC,       CmdKind::Single,  Criticality::Config,      true },
    {  75, "ADCS Set MAG0 MMT Calib Config",          "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_MAG0_MMT_CALIB_CONFIG_CC,   CmdKind::Single,  Criticality::Config,      true },
    {  76, "ADCS Set Default Mode Config",            "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_DEFAULT_MODE_CONFIG_CC,     CmdKind::Single,  Criticality::Config,      true },
    {  77, "ADCS Set Mounting Config",                "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_MOUNTING_CONFIG_CC,         CmdKind::Single,  Criticality::Config,      true },
    {  78, "ADCS Set MAG1 MMT Calib Config",          "BEE1000", "ADCS",    "ADCS",           0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    {  79, "ADCS Set Estimator Config",               "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_ESTIMATOR_CONFIG_CC,        CmdKind::Single,  Criticality::Config,      true },
    {  80, "ADCS Set Sat Orbit Param Config",         "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_SAT_ORBIT_PARAMS_CONFIG_CC, CmdKind::Single,  Criticality::Config,      true },
    {  81, "ADCS Set Node Selection Config",          "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_NODE_SELECTION_CONFIG_CC,   CmdKind::Single,  Criticality::Config,      true },
    {  82, "ADCS Set MTQ Config",                     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_MTQ_CONFIG_CC,              CmdKind::Single,  Criticality::Config,      true },
    {  83, "ADCS Set Estimation Mode",                "BEE1000", "ADCS",    "ADCS",           0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    {  84, "ADCS Set Operational State",              "BEE1000", "ADCS",    "ADCS",           0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    {  85, "ADCS Set Mag Sensing Element Config",     "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_MAG_SENSING_ELM_CONFIG_CC,  CmdKind::Single,  Criticality::Config,      true },
    {  86, "ADCS Set Unsolicited TLM Msg Setup",      "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 ADCS_SET_UNSOLICIT_TLM_MSG_SETUP_CC, CmdKind::Single,  Criticality::Config,      true },
    {  87, "ADCS Set Unsolicited Event Msg Setup",    "BEE1000", "ADCS",    "ADCS",           0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    {  88, "ADCS CC 101",                             "BEE1000", "ADCS",    "ADCS",           ADCS_CMD_ID,                 101,                                 CmdKind::Single,  Criticality::Query,       true },   // ADCS2_COMM_01 (CC 101)
    {  89, "ADCS Open-Loop Reaction Wheel Command",   "BEE1000", "ADCS",    nullptr,          ADCS_CMD_ID,                 43,                                  CmdKind::Single,  Criticality::Config,      true },   // D5: ADCS 필터 범위(47-88) 밖이라 category 없음
    { 126, "ADCS Sequence: Detumbling",               "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 127, "ADCS Sequence: Sun Pointing",             "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 128, "ADCS Sequence: Velocity Pointing",        "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 129, "ADCS Sequence: KSC Earth Pointing",       "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 130, "ADCS Sequence: LGC Earth Pointing",       "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 131, "ADCS Sequence: RPY Pointing",             "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 132, "ADCS Error Log Clear",                    "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Config,      false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 133, "ADCS Get Current Unix Time (Internal)",   "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Query,       false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 134, "ADCS Send HK",                            "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Query,       false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 135, "ADCS Send Beacon",                        "BEE1000", "ADCS",    nullptr,          0,                           0,                                   CmdKind::Single,  Criticality::Query,       false },   // D6: 라벨만 있고 생성기에 구현 없음
    { 136, "SC NoOp",                                 "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_NOOP_CC,                          CmdKind::Single,  Criticality::Query,       true },
    { 137, "SC Reset Counters",                       "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_RESET_COUNTERS_CC,                CmdKind::Single,  Criticality::Config,      true },
    { 138, "SC Start ATS",                            "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_START_ATS_CC,                     CmdKind::Single,  Criticality::Config,      true },
    { 139, "SC Stop ATS",                             "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_STOP_ATS_CC,                      CmdKind::Single,  Criticality::Config,      true },
    { 140, "SC Start RTS",                            "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_START_RTS_CC,                     CmdKind::Single,  Criticality::Config,      true },
    { 141, "SC Stop RTS",                             "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_STOP_RTS_CC,                      CmdKind::Single,  Criticality::Config,      true },
    { 142, "SC Disable RTS",                          "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_DISABLE_RTS_CC,                   CmdKind::Single,  Criticality::Config,      true },
    { 143, "SC Enable RTS",                           "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_ENABLE_RTS_CC,                    CmdKind::Single,  Criticality::Config,      true },
    { 144, "SC Switch ATS",                           "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_SWITCH_ATS_CC,                    CmdKind::Single,  Criticality::Config,      true },
    { 145, "SC Jump ATS Time",                        "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_JUMP_ATS_CC,                      CmdKind::Single,  Criticality::Config,      true },
    { 146, "SC Set Continue-ATS-On-Failure",          "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_CONTINUE_ATS_ON_FAILURE_CC,       CmdKind::Single,  Criticality::Config,      true },
    { 147, "SC Append ATS",                           "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_APPEND_ATS_CC,                    CmdKind::Single,  Criticality::Config,      true },
    { 148, "SC Manage Table",                         "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_MANAGE_TABLE_CC,                  CmdKind::Single,  Criticality::Config,      true },
    { 149, "SC Start RTS Group",                      "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_START_RTS_GRP_CC,                 CmdKind::Single,  Criticality::Config,      true },
    { 150, "SC Stop RTS Group",                       "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_STOP_RTS_GRP_CC,                  CmdKind::Single,  Criticality::Config,      true },
    { 151, "SC Disable RTS Group",                    "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_DISABLE_RTS_GRP_CC,               CmdKind::Single,  Criticality::Config,      true },
    { 152, "SC Enable RTS Group",                     "BEE1000", "SC",      "SC APP",         SC_CMD_MID,                  SC_ENABLE_RTS_GRP_CC,                CmdKind::Single,  Criticality::Config,      true },
    { 153, "TO LAB NoOp",                             "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_NOOP_CC,                      CmdKind::Single,  Criticality::Query,       true },
    { 154, "TO LAB Reset Status",                     "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_RESET_STATUS_CC,              CmdKind::Single,  Criticality::Config,      true },
    { 155, "TO LAB Add Packet(Enable Beacon RN)",     "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_ADD_PKT_CC,                   CmdKind::Single,  Criticality::Config,      true },
    { 156, "TO LAB Send Data Types",                  "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_SEND_DATA_TYPES_CC,           CmdKind::Single,  Criticality::Query,       true },
    { 157, "TO LAB Remove Packet(Stop Beacon)",       "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_REMOVE_PKT_CC,                CmdKind::Single,  Criticality::Config,      true },
    { 158, "TO LAB Remove All Packet",                "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_REMOVE_ALL_PKT_CC,            CmdKind::Single,  Criticality::Config,      true },
    { 159, "TO LAB Output Enable",                    "BEE1000", "TO_LAB",  "cFE",            CFE_MISSION_TO_LAB_CMD_ID,   TO_LAB_OUTPUT_ENABLE_CC,             CmdKind::Single,  Criticality::Config,      true },
    { 160, "UANT Get Status",                         "BEE1000", "UANT",    "UANT",           UANT_APP_CMD_ID,             UANT_APP_GET_STATUS_CC,              CmdKind::Single,  Criticality::Query,       true },
    { 161, "UANT Burn Channel Command",               "BEE1000", "UANT",    "UANT",           UANT_APP_CMD_ID,             UANT_APP_BURN_CHANNEL_CC,            CmdKind::Single,  Criticality::Hazardous,   true },
    { 162, "UANT Set Settings Command",               "BEE1000", "UANT",    "UANT",           UANT_APP_CMD_ID,             UANT_APP_SET_SETTINGS_CC,            CmdKind::Single,  Criticality::Config,      true },
    { 163, "UANT Auto Deploy Command",                "BEE1000", "UANT",    "UANT",           UANT_APP_CMD_ID,             UANT_APP_AUTODEPLOY_CC,              CmdKind::Single,  Criticality::Hazardous,   true },
    { 164, "Deploy Solar Panel 1",                    "BEE1000", "SP",      "SP",             SP_CMD_ID,                   SP_Deploy1_CC,                       CmdKind::Single,  Criticality::Hazardous,   true },
    { 165, "Deploy Solar Panel 2",                    "BEE1000", "SP",      "SP",             SP_CMD_ID,                   SP_Deploy2_CC,                       CmdKind::Single,  Criticality::Hazardous,   true },
    { 166, "Deploy Solar Panel 3",                    "BEE1000", "SP",      "SP",             SP_CMD_ID,                   SP_Deploy3_CC,                       CmdKind::Single,  Criticality::Hazardous,   true },
    { 167, "Deploy Solar Panel 4",                    "BEE1000", "SP",      "SP",             SP_CMD_ID,                   SP_Deploy4_CC,                       CmdKind::Single,  Criticality::Hazardous,   true },
    { 168, "PAYUZUC NoOp",                            "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_NOOP_CC,                     CmdKind::Single,  Criticality::Query,       true },
    { 169, "PAYUZUC Reset Counters",                  "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_RESET_COUNTERS_CC,           CmdKind::Single,  Criticality::Config,      true },
    { 170, "PAYUZUC Ping",                            "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_PING_CC,                     CmdKind::Single,  Criticality::Query,       true },
    { 171, "PAYUZUC Set Mode",                        "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_SET_MODE_CC,                 CmdKind::Single,  Criticality::Config,      true },
    { 172, "PAYUZUC Memory Status",                   "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_MEMORY_STATUS_CC,            CmdKind::Single,  Criticality::Query,       true },
    { 173, "PAYUZUC Set Exposure",                    "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_SET_EXPOSURE_CC,             CmdKind::Single,  Criticality::Config,      true },
    { 174, "PAYUZUC Capture",                         "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_CAPTURE_CC,                  CmdKind::Single,  Criticality::Config,      true },
    { 175, "PAYUZUC Download",                        "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_DOWNLOAD_CC,                 CmdKind::Single,  Criticality::Query,       true },
    { 176, "PAYUZUC Read Register",                   "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_READ_REGISTER_CC,            CmdKind::Single,  Criticality::Query,       true },
    { 177, "PAYUZUC Write Register",                  "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_WRITE_REGISTER_CC,           CmdKind::Single,  Criticality::Config,      true },
    { 178, "PAYUZUC Download All",                    "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_DOWNLOAD_ALL_CC,             CmdKind::Single,  Criticality::Query,       true },
    { 179, "PAYUZUC Mosaic",                          "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_MOSAIC_CC,                   CmdKind::Single,  Criticality::Config,      true },
    { 180, "PAYUZUC Download All Child",              "BEE1000", "PAYUZUC", "PAYUZUC",        PAYUZUC_CMD_ID,              PAYUZUC_DOWNLOAD_ALL_CHILD_CC,       CmdKind::Single,  Criticality::Query,       true },
    { 201, "TO Delete",                               "BEE1000", "FM",      "FTP",            0x1815,                      5,                                   CmdKind::Single,  Criticality::Hazardous,   true },   // 라벨은 TO 지만 MID 0x1815 는 FM 앱
    { 202, "TO Rename",                               "BEE1000", "FM",      "FTP",            0x1815,                      4,                                   CmdKind::Single,  Criticality::Config,      true },   // 라벨은 TO 지만 MID 0x1815 는 FM 앱
    { 204, "FM Get File Info",                        "BEE1000", "FM",      "FTP",            0x1815,                      10,                                  CmdKind::Single,  Criticality::Query,       true },
    { 205, "Beacon Mukbang (Useless but may try)",    "BEE1000", "UNKNOWN", nullptr,          0x1819,                      0,                                   CmdKind::Single,  Criticality::Unclassified, true },   // 용도 불명 (MID 0x1819)
    { 206, "UTRX GNDWDT Clear",                       "BEE1000", "UTRX",    "UTRX GNDWDT",    0x1850,                      6,                                   CmdKind::Single,  Criticality::Config,      true },
    { 213, "FM Get Dir List Pkt",                     "BEE1000", "FM",      "FTP",            0x1815,                      15,                                  CmdKind::Single,  Criticality::Query,       true },
    { 216, "ES NoOp",                                 "BEE1000", "EPS",     "EPS",            0x1875,                      EPS_P60_GET_DOCK_INFO_CC,            CmdKind::Single,  Criticality::Query,       true },   // D4: 라벨은 ES NoOp 이지만 구현은 EPS Get Dock Info(45) 복사
    { 217, "ES Start App",                            "BEE1000", "ES",      "cFE",            CFE_ES_CMD_MID,              4,                                   CmdKind::Single,  Criticality::Config,      true },
    { 218, "ES Stop App",                             "BEE1000", "ES",      "cFE",            CFE_ES_CMD_MID,              CFE_ES_STOP_APP_CC,                  CmdKind::Single,  Criticality::Hazardous,   true },
    { 219, "ES Reload App",                           "BEE1000", "ES",      "cFE",            CFE_ES_CMD_MID,              CFE_ES_RELOAD_APP_CC,                CmdKind::Single,  Criticality::Hazardous,   true },
    { 220, "ES Query One",                            "BEE1000", "ES",      "cFE",            CFE_ES_CMD_MID,              CFE_ES_QUERY_ONE_CC,                 CmdKind::Single,  Criticality::Query,       true },
    { 221, "ES Query All",                            "BEE1000", "ES",      "cFE",            CFE_ES_CMD_MID,              CFE_ES_QUERY_ALL_CC,                 CmdKind::Single,  Criticality::Query,       true },
};

// 초기 운용(LEOP) 명령 세트. 배열 순서가 Initial Operations 창의 표시 순서다.
// (기존 miman_imgui.cpp INITIAL_CMD_INDICES 와 동일)
static const int kInitialOps[] = {
    0, 1, 2, 3, 4,   // no arg, u8, u16, u32, u64
    72, 73, 74, 76, 77, 79, 81, 82, 85, 86, 58,80, // ADCS sunpointing용
    155,    // Enable Beacon
    200,    // D14: 라벨·구현 없는 번호 (표시되지 않음)
    205,    // D14: 원래 주석은 "UANT burn channel" 이지만 205 는 Beacon Mukbang. UANT Burn Channel 은 161
    160,    // UANT Get Status
    25,     // EPS Dock Get HK
    164, 165, 166, 167,   // Deploy SP
    204,       // FM Get File Info
    206,       // UTRX GNDWDT clear
    -1               // 끝 표시
};

extern const MissionCommands kBee1000Commands = {
    "BEE1000", kCommands, (int)(sizeof(kCommands) / sizeof(kCommands[0])), kInitialOps
};

} // namespace tc
} // namespace mim
