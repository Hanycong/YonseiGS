// View - 명령 생성기: BEE1012/UELYSYS 표 기반 명령 (cFE ES/EVS/SB/TBL/TIME/SRL, THRUST, PAYYONSEI, SDR, LGPM)
//
// 누리호 5차 GS(GS_nuri5) 의 Bee1012CfeCommands 표와 범용 편집기를 그대로 옮긴 것.
// 명령마다 필드(오프셋/크기/종류)가 표로 정의돼 있고 편집기 하나가 모두 그린다.
#include <pthread.h>
#include <inttypes.h>
#include <string.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "imgui.h"
#include "YonseiGS_config.h"
#include "mission_defs.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"
#include "sat_mode.h"
#include "cmdgen5_common.h"

extern Console console;
extern uint16_t gen_fnccode;

enum class Bee1012CfeFieldKind
{
    U8,
    Bool,
    U16,
    I16,
    U32,
    String
};

struct Bee1012CfeField
{
    const char *Label;
    Bee1012CfeFieldKind Kind;
    size_t Offset;
    size_t Size;
};

struct Bee1012CfeCommand
{
    int GuiIndex;
    const char *Module;
    const char *Name;
    uint16_t Mid;
    uint8_t Cc;
    size_t PayloadSize;
    const Bee1012CfeField *Fields;
    size_t FieldCount;
    const char *Validation;
    uint16_t PacketType;
};

#define BEE_CFE_FIELD(type, member, kind, label) \
    {label, Bee1012CfeFieldKind::kind, offsetof(type, member), sizeof(((type *)0)->member)}

static const Bee1012CfeField CfeEsRestartFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_ES_Restart_Payload_t, RestartType, U16, "Restart type (1=processor, 2=power-on)")};
static const Bee1012CfeField CfeEsStartAppFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, Application, String, "Application name"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, AppEntryPoint, String, "Entry point"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, AppFileName, String, "Application file"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, StackSize, U32, "Stack size (0=platform default)"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, ExceptionAction, U8, "Exception action (0=restart app, 1=processor reset)"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_StartApp_Payload_t, Priority, U16, "Task priority")};
static const Bee1012CfeField CfeAppNameFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_AppName_Payload_t, Application, String, "Application name")};
static const Bee1012CfeField CfeEsReloadFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_ES_ReloadApp_Payload_t, Application, String, "Application name"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_ReloadApp_Payload_t, AppFileName, String, "Application file")};
static const Bee1012CfeField CfePathFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_Path_Payload_t, FileName, String, "Path / filename")};
static const Bee1012CfeField CfeU32Fields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_U32_Payload_t, Value, U32, "Value")};
static const Bee1012CfeField CfeU16Fields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_U16_Payload_t, Value, U16, "Value")};
static const Bee1012CfeField CfeI16Fields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_I16_Payload_t, Value, I16, "Value")};
static const Bee1012CfeField CfeMaskPairFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_U32Pair_Payload_t, First, U32, "Mask word index"),
    BEE_CFE_FIELD(BEE1012_CFE_U32Pair_Payload_t, Second, U32, "Mask value")};
static const Bee1012CfeField CfeTimePairFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_U32Pair_Payload_t, First, U32, "Seconds"),
    BEE_CFE_FIELD(BEE1012_CFE_U32Pair_Payload_t, Second, U32, "Microseconds / subseconds")};
static const Bee1012CfeField CfeTimeToneDataFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneMETSeconds, U32, "At-tone MET seconds"),
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneMETSubseconds, U32, "At-tone MET binary subseconds"),
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneSTCFSeconds, U32, "At-tone STCF seconds"),
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneSTCFSubseconds, U32, "At-tone STCF binary subseconds"),
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneLeapSeconds, I16, "At-tone leap seconds"),
    BEE_CFE_FIELD(BEE1012_CFE_TIME_ToneData_Payload_t, AtToneState, I16, "At-tone state (-1=invalid, 0=valid, 1=flywheel)")};
static const Bee1012CfeField CfeEsDeleteCdsFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_ES_DeleteCDS_Payload_t, CdsName, String, "CDS full name")};
static const Bee1012CfeField CfeEsPoolFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_ES_SendMemPoolStats_Payload_t, Application, String, "Reserved application (normally empty)"),
    BEE_CFE_FIELD(BEE1012_CFE_ES_SendMemPoolStats_Payload_t, PoolHandle, U32, "Memory-pool handle")};
static const Bee1012CfeField CfeU8SpareFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_U8Spare_Payload_t, Value, U8, "Value / bit mask")};
static const Bee1012CfeField CfeEvsAppBitMaskFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppBitMask_Payload_t, AppName, String, "Application name"),
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppBitMask_Payload_t, BitMask, U8, "Event-type bit mask")};
static const Bee1012CfeField CfeEvsAppEventFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppEvent_Payload_t, AppName, String, "Application name"),
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppEvent_Payload_t, EventID, U16, "Event ID")};
static const Bee1012CfeField CfeEvsFilterFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppEventMask_Payload_t, AppName, String, "Application name"),
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppEventMask_Payload_t, EventID, U16, "Event ID"),
    BEE_CFE_FIELD(BEE1012_CFE_EVS_AppEventMask_Payload_t, Mask, U16, "Filter mask")};
static const Bee1012CfeField CfeSbRouteFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_SB_Route_Payload_t, MsgId, U32, "Route message ID"),
    BEE_CFE_FIELD(BEE1012_CFE_SB_Route_Payload_t, Pipe, U32, "Pipe resource ID")};
static const Bee1012CfeField CfeTblDumpFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Dump_Payload_t, ActiveTableFlag, U16, "Buffer (0=inactive, 1=active)"),
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Dump_Payload_t, TableName, String, "Table full name"),
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Dump_Payload_t, DumpFilename, String, "Dump filename")};
static const Bee1012CfeField CfeTblValidateFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Validate_Payload_t, ActiveTableFlag, U16, "Buffer (0=inactive, 1=active)"),
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Validate_Payload_t, TableName, String, "Table full name")};
static const Bee1012CfeField CfeTblNameFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_TBL_Name_Payload_t, TableName, String, "Table full name")};
static const Bee1012CfeField CfeSrlIndexFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_SRL_Index_Payload_t, Indexer, U32, "Handle index (0..5)")};
static const Bee1012CfeField CfeSrlInitFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_SRL_InitHandle_Payload_t, Indexer, U32, "Handle index (0..5)"),
    BEE_CFE_FIELD(BEE1012_CFE_SRL_InitHandle_Payload_t, Name, String, "Handle name"),
    BEE_CFE_FIELD(BEE1012_CFE_SRL_InitHandle_Payload_t, DevName, String, "Device path"),
    BEE_CFE_FIELD(BEE1012_CFE_SRL_InitHandle_Payload_t, DevType, U8, "Device type (1=I2C,2=SPI,3=CAN,4=UART,5=RS422)")};
static const Bee1012CfeField CfeSrlConfigBaseFields[] = {
    BEE_CFE_FIELD(BEE1012_CFE_SRL_ConfigHandle_Payload_t, Indexer, U32, "Handle index (0..5)"),
    BEE_CFE_FIELD(BEE1012_CFE_SRL_ConfigHandle_Payload_t, FD, U8, "File descriptor")};

typedef struct __attribute__((packed))
{
    uint8_t reg_addr;
    uint16_t data;
} PayyonseiPayuelAosWriteRegisterPayload;

typedef struct __attribute__((packed))
{
    uint8_t reg_addr;
} PayyonseiPayuelAosReadRegisterPayload;

typedef struct __attribute__((packed))
{
    uint8_t DataID;
    uint8_t Spare[3];
} PayyonseiPayuelLgbatRequestDataPayload;

static_assert(sizeof(PayyonseiPayuelAosWriteRegisterPayload) == 3,
              "PAYYONSEI payuel_aos WRITE_REGISTER payload size mismatch");
static_assert(sizeof(PayyonseiPayuelAosReadRegisterPayload) == 1,
              "PAYYONSEI payuel_aos READ_REGISTER payload size mismatch");
static_assert(sizeof(PayyonseiPayuelLgbatRequestDataPayload) == 4,
              "PAYYONSEI payuel_lgbat REQUEST_DATA payload size mismatch");

static const Bee1012CfeField PayyonseiPayuelAosWriteRegisterFields[] = {
    BEE_CFE_FIELD(PayyonseiPayuelAosWriteRegisterPayload, reg_addr, U8, "reg_addr"),
    BEE_CFE_FIELD(PayyonseiPayuelAosWriteRegisterPayload, data, U16, "data")};
static const Bee1012CfeField PayyonseiPayuelAosReadRegisterFields[] = {
    BEE_CFE_FIELD(PayyonseiPayuelAosReadRegisterPayload, reg_addr, U8, "reg_addr")};
static const Bee1012CfeField PayyonseiPayuelLgbatRequestDataFields[] = {
    BEE_CFE_FIELD(PayyonseiPayuelLgbatRequestDataPayload, DataID, U8, "DataID")};

static const Bee1012CfeField PayyonseiTempSetConfigFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_TEMP_SetConfig_Payload_t, ConfigVal, U16, "Raw MCP9808 config register")};
static const Bee1012CfeField PayyonseiTempSetLimitsFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t, UpperLimit, U16, "Raw upper-limit register"),
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t, LowerLimit, U16, "Raw lower-limit register"),
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t, CritLimit, U16, "Raw critical-limit register")};
static const Bee1012CfeField PayyonseiTempSetResolutionFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_TEMP_SetResolution_Payload_t, ResVal, U8,
                  "Resolution (0=0.5C, 1=0.25C, 2=0.125C, 3=0.0625C)")};
static const Bee1012CfeField PayyonseiIwsbSlotFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_IWSB_Slot_Payload_t, Slot, U8, "Firmware slot (must be 0)")};
static const Bee1012CfeField PayyonseiIwsbWriteFirmwareFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_IWSB_WriteFirmware_Payload_t, FilePath, String,
                  "OBC firmware-image path"),
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_IWSB_WriteFirmware_Payload_t, Slot, U8,
                  "Firmware slot (must be 0)")};
static const Bee1012CfeField PayyonseiIwsbUpdateFirmwareFields[] = {
    BEE_CFE_FIELD(UELYSYS_PAYYONSEI_IWSB_UpdateFirmware_Payload_t, Target, U8,
                  "Update target (0=self, 1..4=RX node)")};
static const Bee1012CfeField SdrControllerSetPhaseShiftFields[] = {
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t, Channel, U8,
                  "Channel (FSW handles 1 or 2)"),
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t, Mode, Bool,
                  "GPIO mode"),
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t, Flag, U8,
                  "Phase-bit mask (bits 0..5)")};
static const Bee1012CfeField SdrControllerFilenameFields[] = {
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_Filename_Payload_t, Filename, String,
                  "Filename (50-byte FSW field; length is generated automatically)")};
static const Bee1012CfeField SdrControllerSlotFields[] = {
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_Slot_Payload_t, SlotIndex, U8,
                  "Slot index (FSW applies no range check)")};
static const Bee1012CfeField SdrControllerWriteFirmwareFields[] = {
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t, Filename, String,
                  "Firmware filename (50-byte FSW field; length is generated automatically)"),
    BEE_CFE_FIELD(UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t, Slot, U8,
                  "Destination slot (FSW applies no range check)")};
static const Bee1012CfeField ThrustTargetModeFields[] = {
    BEE_CFE_FIELD(BEE1012_THRUST_TargetMode_Payload_t, TargetMode, U32, "Target mode")};
static const Bee1012CfeField ThrustArmFields[] = {
    BEE_CFE_FIELD(BEE1012_THRUST_Arm_Payload_t, ArmKey, U16, "Arm key"),
    BEE_CFE_FIELD(BEE1012_THRUST_Arm_Payload_t, TimeoutSec, U16, "Timeout (sec)")};
static const Bee1012CfeField ThrustMainFireFields[] = {
    BEE_CFE_FIELD(BEE1012_THRUST_MainFire_Payload_t, FireTime_ms, U32, "Fire time (ms)"),
    BEE_CFE_FIELD(BEE1012_THRUST_MainFire_Payload_t, IgnitionTime_ms, U16, "Ignition time (ms)"),
    BEE_CFE_FIELD(BEE1012_THRUST_MainFire_Payload_t, FireKey, U16, "Fire key")};
static const Bee1012CfeField ThrustCgpulseFields[] = {
    BEE_CFE_FIELD(BEE1012_THRUST_CGPulse_Payload_t, ThrusterId, U16, "Thruster ID"),
    BEE_CFE_FIELD(BEE1012_THRUST_CGPulse_Payload_t, PulseWidth_ms, U32, "Pulse width (ms)"),
    BEE_CFE_FIELD(BEE1012_THRUST_CGPulse_Payload_t, FireKey, U16, "Fire key")};
static const Bee1012CfeField ThrustScheduledHkEnableFields[] = {
    BEE_CFE_FIELD(BEE1012_THRUST_ScheduledHkEnable_Payload_t, ScheduledHkEnabled, Bool,
                  "Scheduled HK enable")};

#define BEE_CFE_CMD0(index, module, name, mid, cc, validation) \
    {index, module, name, mid, cc, 0, nullptr, 0, validation, MIM_PT_TMTC_TEST}
#define BEE_CFE_CMD(index, module, name, mid, cc, payload_type, fields, validation) \
    {index, module, name, mid, cc, sizeof(payload_type), fields, IM_ARRAYSIZE(fields), validation, MIM_PT_TMTC_TEST}
#define UEL_REMOTE_CMD0(index, module, name, mid, cc, packet_type, validation) \
    {index, module, name, mid, cc, 0, nullptr, 0, validation, packet_type}
#define UEL_REMOTE_CMD(index, module, name, mid, cc, payload_type, fields, packet_type, validation) \
    {index, module, name, mid, cc, sizeof(payload_type), fields, IM_ARRAYSIZE(fields), validation, packet_type}

static const Bee1012CfeCommand Bee1012CfeCommands[] = {
    BEE_CFE_CMD0(3600, "CFE_ES", "NOOP", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3601, "CFE_ES", "RESET_COUNTERS", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD(3602, "CFE_ES", "RESTART", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_RESTART_CC, BEE1012_CFE_ES_Restart_Payload_t, CfeEsRestartFields, "RestartType must be 1 or 2."),
    BEE_CFE_CMD(3604, "CFE_ES", "START_APP", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_START_APP_CC, BEE1012_CFE_ES_StartApp_Payload_t, CfeEsStartAppFields, "Filename valid; app and entry names nonempty; priority <= OS_MAX_PRIORITY; exception action 0 or 1."),
    BEE_CFE_CMD(3605, "CFE_ES", "STOP_APP", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_STOP_APP_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must exist and be deletable."),
    BEE_CFE_CMD(3606, "CFE_ES", "RESTART_APP", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_RESTART_APP_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must exist and be restartable."),
    BEE_CFE_CMD(3607, "CFE_ES", "RELOAD_APP", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_RELOAD_APP_CC, BEE1012_CFE_ES_ReloadApp_Payload_t, CfeEsReloadFields, "Application must exist and filename must parse as a dynamic module."),
    BEE_CFE_CMD(3608, "CFE_ES", "QUERY_ONE", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_QUERY_ONE_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application or library name must exist."),
    BEE_CFE_CMD(3609, "CFE_ES", "QUERY_ALL", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_QUERY_ALL_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD0(3610, "CFE_ES", "CLEAR_SYS_LOG", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_CLEAR_SYS_LOG_CC, "Exact packet length only."),
    BEE_CFE_CMD(3611, "CFE_ES", "WRITE_SYS_LOG", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_WRITE_SYS_LOG_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD0(3612, "CFE_ES", "CLEAR_ER_LOG", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_CLEAR_ER_LOG_CC, "Exact packet length only."),
    BEE_CFE_CMD(3613, "CFE_ES", "WRITE_ER_LOG", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_WRITE_ER_LOG_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD(3614, "CFE_ES", "START_PERF_DATA", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_START_PERF_DATA_CC, BEE1012_CFE_U32_Payload_t, CfeU32Fields, "Trigger mode must be 0=start, 1=center, or 2=end."),
    BEE_CFE_CMD(3615, "CFE_ES", "STOP_PERF_DATA", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_STOP_PERF_DATA_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Output filename must parse."),
    BEE_CFE_CMD(3616, "CFE_ES", "SET_PERF_FILTER_MASK", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_SET_PERF_FILTER_MASK_CC, BEE1012_CFE_U32Pair_Payload_t, CfeMaskPairFields, "Mask word index must be below CFE_ES_PERF_32BIT_WORDS_IN_MASK."),
    BEE_CFE_CMD(3617, "CFE_ES", "SET_PERF_TRIGGER_MASK", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_SET_PERF_TRIGGER_MASK_CC, BEE1012_CFE_U32Pair_Payload_t, CfeMaskPairFields, "Mask word index must be below CFE_ES_PERF_32BIT_WORDS_IN_MASK."),
    BEE_CFE_CMD(3618, "CFE_ES", "OVER_WRITE_SYS_LOG", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_OVER_WRITE_SYS_LOG_CC, BEE1012_CFE_U32_Payload_t, CfeU32Fields, "Mode must be 0=overwrite or 1=discard."),
    BEE_CFE_CMD0(3619, "CFE_ES", "RESET_PR_COUNT", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_RESET_PR_COUNT_CC, "Exact packet length only."),
    BEE_CFE_CMD(3620, "CFE_ES", "SET_MAX_PR_COUNT", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_SET_MAX_PR_COUNT_CC, BEE1012_CFE_U16_Payload_t, CfeU16Fields, "Exact packet length; uint16 maximum-reset count."),
    BEE_CFE_CMD(3621, "CFE_ES", "DELETE_CDS", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_DELETE_CDS_CC, BEE1012_CFE_ES_DeleteCDS_Payload_t, CfeEsDeleteCdsFields, "CDS name must resolve, and it must not be in use."),
    BEE_CFE_CMD(3622, "CFE_ES", "SEND_MEM_POOL_STATS", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_SEND_MEM_POOL_STATS_CC, BEE1012_CFE_ES_SendMemPoolStats_Payload_t, CfeEsPoolFields, "Pool handle must identify a registered memory pool; Application is reserved/zero."),
    BEE_CFE_CMD(3623, "CFE_ES", "DUMP_CDS_REGISTRY", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_DUMP_CDS_REGISTRY_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD(3624, "CFE_ES", "QUERY_ALL_TASKS", BEE1012_CFE_ES_CMD_ID, BEE1012_CFE_ES_QUERY_ALL_TASKS_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD0(3690, "CFE_ES", "SEND_HK", BEE1012_CFE_ES_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),

    BEE_CFE_CMD0(3700, "CFE_EVS", "NOOP", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3701, "CFE_EVS", "RESET_COUNTERS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD(3702, "CFE_EVS", "ENABLE_EVENT_TYPE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_ENABLE_EVENT_TYPE_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3703, "CFE_EVS", "DISABLE_EVENT_TYPE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_DISABLE_EVENT_TYPE_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3704, "CFE_EVS", "SET_EVENT_FORMAT_MODE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_SET_EVENT_FORMAT_MODE_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Format must be 0=short or 1=long."),
    BEE_CFE_CMD(3705, "CFE_EVS", "ENABLE_APP_EVENT_TYPE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_ENABLE_APP_EVENT_TYPE_CC, BEE1012_CFE_EVS_AppBitMask_Payload_t, CfeEvsAppBitMaskFields, "Application must be registered; bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3706, "CFE_EVS", "DISABLE_APP_EVENT_TYPE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_DISABLE_APP_EVENT_TYPE_CC, BEE1012_CFE_EVS_AppBitMask_Payload_t, CfeEvsAppBitMaskFields, "Application must be registered; bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3707, "CFE_EVS", "ENABLE_APP_EVENTS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_ENABLE_APP_EVENTS_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must be registered with EVS."),
    BEE_CFE_CMD(3708, "CFE_EVS", "DISABLE_APP_EVENTS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_DISABLE_APP_EVENTS_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must be registered with EVS."),
    BEE_CFE_CMD(3709, "CFE_EVS", "RESET_APP_COUNTER", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_RESET_APP_COUNTER_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must be registered with EVS."),
    BEE_CFE_CMD(3710, "CFE_EVS", "SET_FILTER", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_SET_FILTER_CC, BEE1012_CFE_EVS_AppEventMask_Payload_t, CfeEvsFilterFields, "Application and event filter must already be registered."),
    BEE_CFE_CMD(3711, "CFE_EVS", "ENABLE_PORTS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_ENABLE_PORTS_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Port bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3712, "CFE_EVS", "DISABLE_PORTS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_DISABLE_PORTS_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Port bit mask must be 0x01..0x0F."),
    BEE_CFE_CMD(3713, "CFE_EVS", "RESET_FILTER", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_RESET_FILTER_CC, BEE1012_CFE_EVS_AppEvent_Payload_t, CfeEvsAppEventFields, "Application and event filter must be registered."),
    BEE_CFE_CMD(3714, "CFE_EVS", "RESET_ALL_FILTERS", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_RESET_ALL_FILTERS_CC, BEE1012_CFE_AppName_Payload_t, CfeAppNameFields, "Application must be registered with EVS."),
    BEE_CFE_CMD(3715, "CFE_EVS", "ADD_EVENT_FILTER", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_ADD_EVENT_FILTER_CC, BEE1012_CFE_EVS_AppEventMask_Payload_t, CfeEvsFilterFields, "Application registered; event not already filtered; free filter slot required."),
    BEE_CFE_CMD(3716, "CFE_EVS", "DELETE_EVENT_FILTER", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_DELETE_EVENT_FILTER_CC, BEE1012_CFE_EVS_AppEvent_Payload_t, CfeEvsAppEventFields, "Application and event filter must be registered."),
    BEE_CFE_CMD(3717, "CFE_EVS", "WRITE_APP_DATA_FILE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_WRITE_APP_DATA_FILE_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD(3718, "CFE_EVS", "WRITE_LOG_DATA_FILE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_WRITE_LOG_DATA_FILE_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects the platform default; otherwise path must parse."),
    BEE_CFE_CMD(3719, "CFE_EVS", "SET_LOG_MODE", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_SET_LOG_MODE_CC, BEE1012_CFE_U8Spare_Payload_t, CfeU8SpareFields, "Log mode must be 0=overwrite or 1=discard."),
    BEE_CFE_CMD0(3720, "CFE_EVS", "CLEAR_LOG", BEE1012_CFE_EVS_CMD_ID, BEE1012_CFE_EVS_CLEAR_LOG_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3790, "CFE_EVS", "SEND_HK", BEE1012_CFE_EVS_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),

    BEE_CFE_CMD0(3800, "CFE_SB", "NOOP", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3801, "CFE_SB", "RESET_COUNTERS", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3802, "CFE_SB", "SEND_SB_STATS", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_SEND_SB_STATS_CC, "Exact packet length only."),
    BEE_CFE_CMD(3803, "CFE_SB", "WRITE_ROUTING_INFO", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_WRITE_ROUTING_INFO_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects platform default; otherwise path must parse."),
    BEE_CFE_CMD(3804, "CFE_SB", "ENABLE_ROUTE", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_ENABLE_ROUTE_CC, BEE1012_CFE_SB_Route_Payload_t, CfeSbRouteFields, "Message ID and pipe ID must identify an existing disabled route."),
    BEE_CFE_CMD(3805, "CFE_SB", "DISABLE_ROUTE", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_DISABLE_ROUTE_CC, BEE1012_CFE_SB_Route_Payload_t, CfeSbRouteFields, "Message ID and pipe ID must identify an existing enabled route."),
    BEE_CFE_CMD(3807, "CFE_SB", "WRITE_PIPE_INFO", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_WRITE_PIPE_INFO_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects platform default; otherwise path must parse."),
    BEE_CFE_CMD(3808, "CFE_SB", "WRITE_MAP_INFO", BEE1012_CFE_SB_CMD_ID, BEE1012_CFE_SB_WRITE_MAP_INFO_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects platform default; otherwise path must parse."),
    BEE_CFE_CMD0(3850, "CFE_SB", "ENABLE_SUB_REPORTING", BEE1012_CFE_SB_SUB_RPT_CMD_ID, BEE1012_CFE_SB_ENABLE_SUB_REPORTING_CC, "Exact packet length; handled only on MID 0x1806."),
    BEE_CFE_CMD0(3851, "CFE_SB", "DISABLE_SUB_REPORTING", BEE1012_CFE_SB_SUB_RPT_CMD_ID, BEE1012_CFE_SB_DISABLE_SUB_REPORTING_CC, "Exact packet length; handled only on MID 0x1806."),
    BEE_CFE_CMD0(3852, "CFE_SB", "SEND_PREV_SUBS", BEE1012_CFE_SB_SUB_RPT_CMD_ID, BEE1012_CFE_SB_SEND_PREV_SUBS_CC, "Exact packet length; handled only on MID 0x1806."),
    BEE_CFE_CMD0(3890, "CFE_SB", "SEND_HK", BEE1012_CFE_SB_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),

    BEE_CFE_CMD0(3900, "CFE_TBL", "NOOP", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(3901, "CFE_TBL", "RESET_COUNTERS", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD(3902, "CFE_TBL", "LOAD", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_LOAD_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Load file must be a valid cFE table image and target a registered table."),
    BEE_CFE_CMD(3903, "CFE_TBL", "DUMP", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_DUMP_CC, BEE1012_CFE_TBL_Dump_Payload_t, CfeTblDumpFields, "Buffer selector 0/1; table registered; output path valid."),
    BEE_CFE_CMD(3904, "CFE_TBL", "VALIDATE", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_VALIDATE_CC, BEE1012_CFE_TBL_Validate_Payload_t, CfeTblValidateFields, "Buffer selector 0/1 and table must be registered/available."),
    BEE_CFE_CMD(3905, "CFE_TBL", "ACTIVATE", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_ACTIVATE_CC, BEE1012_CFE_TBL_Name_Payload_t, CfeTblNameFields, "Table must be registered with a validated pending load."),
    BEE_CFE_CMD(3906, "CFE_TBL", "DUMP_REGISTRY", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_DUMP_REGISTRY_CC, BEE1012_CFE_Path_Payload_t, CfePathFields, "Empty filename selects platform default; otherwise path must parse."),
    BEE_CFE_CMD(3907, "CFE_TBL", "SEND_REGISTRY", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_SEND_REGISTRY_CC, BEE1012_CFE_TBL_Name_Payload_t, CfeTblNameFields, "Table must be registered."),
    BEE_CFE_CMD(3908, "CFE_TBL", "DELETE_CDS", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_DELETE_CDS_CC, BEE1012_CFE_TBL_Name_Payload_t, CfeTblNameFields, "Critical table must exist and its CDS must be deletable."),
    BEE_CFE_CMD(3909, "CFE_TBL", "ABORT_LOAD", BEE1012_CFE_TBL_CMD_ID, BEE1012_CFE_TBL_ABORT_LOAD_CC, BEE1012_CFE_TBL_Name_Payload_t, CfeTblNameFields, "Table must have a load in progress."),
    BEE_CFE_CMD0(3990, "CFE_TBL", "SEND_HK", BEE1012_CFE_TBL_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),

    BEE_CFE_CMD0(9400, "CFE_TIME", "NOOP", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(9401, "CFE_TIME", "RESET_COUNTERS", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD0(9402, "CFE_TIME", "SEND_DIAGNOSTIC", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SEND_DIAGNOSTIC_CC, "Exact packet length only."),
    BEE_CFE_CMD(9403, "CFE_TIME", "SET_SOURCE", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_SOURCE_CC, BEE1012_CFE_I16_Payload_t, CfeI16Fields, "Time source must be 1=internal or 2=external."),
    BEE_CFE_CMD(9404, "CFE_TIME", "SET_STATE", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_STATE_CC, BEE1012_CFE_I16_Payload_t, CfeI16Fields, "Clock state must be -1=invalid, 0=valid, or 1=flywheel."),
    BEE_CFE_CMD(9405, "CFE_TIME", "ADD_DELAY", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_ADD_DELAY_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000."),
    BEE_CFE_CMD(9406, "CFE_TIME", "SUB_DELAY", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SUB_DELAY_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000."),
    BEE_CFE_CMD(9407, "CFE_TIME", "SET_TIME", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_TIME_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000; processed by a TIME server."),
    BEE_CFE_CMD(9408, "CFE_TIME", "SET_MET", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_MET_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000; processed by a TIME server."),
    BEE_CFE_CMD(9409, "CFE_TIME", "SET_STCF", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_STCF_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000; processed by a TIME server."),
    BEE_CFE_CMD(9410, "CFE_TIME", "SET_LEAP_SECONDS", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_LEAP_SECONDS_CC, BEE1012_CFE_I16_Payload_t, CfeI16Fields, "Signed 16-bit leap-second value; processed by a TIME server."),
    BEE_CFE_CMD(9411, "CFE_TIME", "ADD_ADJUST", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_ADD_ADJUST_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000; processed by a TIME server."),
    BEE_CFE_CMD(9412, "CFE_TIME", "SUB_ADJUST", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SUB_ADJUST_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Microseconds must be below 1,000,000; processed by a TIME server."),
    BEE_CFE_CMD(9413, "CFE_TIME", "ADD_ONE_HZ_ADJUSTMENT", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_ADD_ONE_HZ_ADJUSTMENT_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Second field is binary subseconds; processed by a TIME server."),
    BEE_CFE_CMD(9414, "CFE_TIME", "SUB_ONE_HZ_ADJUSTMENT", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SUB_ONE_HZ_ADJUSTMENT_CC, BEE1012_CFE_U32Pair_Payload_t, CfeTimePairFields, "Second field is binary subseconds; processed by a TIME server."),
    BEE_CFE_CMD(9415, "CFE_TIME", "SET_SIGNAL", BEE1012_CFE_TIME_CMD_ID, BEE1012_CFE_TIME_SET_SIGNAL_CC, BEE1012_CFE_I16_Payload_t, CfeI16Fields, "Tone source must be 1=primary or 2=redundant."),
    BEE_CFE_CMD0(9490, "CFE_TIME", "SEND_HK", BEE1012_CFE_TIME_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),
    BEE_CFE_CMD0(9491, "CFE_TIME", "TONE_SIGNAL", BEE1012_CFE_TIME_TONE_CMD_ID, 0, "Internal TIME trigger exposed for ground synchronization. It latches a tone signal and may complete an at-tone pair."),
    BEE_CFE_CMD(9492, "CFE_TIME", "TONE_DATA", BEE1012_CFE_TIME_DATA_CMD_ID, 0, BEE1012_CFE_TIME_ToneData_Payload_t, CfeTimeToneDataFields, "Internal TIME data exposed for ground synchronization. BEE1012 uses native little-endian fields; subseconds are 2^-32 units, not microseconds."),
    BEE_CFE_CMD0(9493, "CFE_TIME", "ONE_HZ", BEE1012_CFE_TIME_ONEHZ_CMD_ID, 0, "Runs the local 1-Hz state machine. With BEE1012 fake-tone enabled, it also invokes the tone ISR path."),
    BEE_CFE_CMD0(9494, "CFE_TIME", "TONE_SEND", BEE1012_CFE_TIME_SEND_CMD_ID, 0, "TIME-server request to generate and transmit current at-tone data; BEE1012 is configured as a server."),

    BEE_CFE_CMD0(9500, "CFE_SRL", "NOOP", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(9501, "CFE_SRL", "RESET_COUNTERS", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD(9502, "CFE_SRL", "RESET_HANDLE_COUNTERS", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_RESET_HANDLE_COUNTERS_CC, BEE1012_CFE_SRL_Index_Payload_t, CfeSrlIndexFields, "Handle index must identify one of the six BEE1012 serial handles."),
    BEE_CFE_CMD(9503, "CFE_SRL", "GET_HANDLE_STATUS", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_GET_HANDLE_STATUS_CC, BEE1012_CFE_SRL_Index_Payload_t, CfeSrlIndexFields, "Deprecated handler; index is still length-checked and decoded."),
    BEE_CFE_CMD(9504, "CFE_SRL", "INIT_HANDLE", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_INIT_HANDLE_CC, BEE1012_CFE_SRL_InitHandle_Payload_t, CfeSrlInitFields, "Index 0..5; device type 1..5; names fit 16-byte fields."),
    BEE_CFE_CMD(9505, "CFE_SRL", "CLOSE_HANDLE", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_CLOSE_HANDLE_CC, BEE1012_CFE_SRL_Index_Payload_t, CfeSrlIndexFields, "Handle index must identify an initialized handle."),
    BEE_CFE_CMD(9506, "CFE_SRL", "CONFIG_HANDLE", BEE1012_CFE_SRL_CMD_ID, BEE1012_CFE_SRL_CONFIG_HANDLE_CC, BEE1012_CFE_SRL_ConfigHandle_Payload_t, CfeSrlConfigBaseFields, "Index must be initialized and FD must match; config interpretation must match the handle device type."),
    BEE_CFE_CMD0(9590, "CFE_SRL", "SEND_HK", BEE1012_CFE_SRL_SEND_HK_ID, 0, "Housekeeping request MID; function code is ignored."),

    BEE_CFE_CMD0(10900, "THRUST", "NOOP", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_NOOP_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10901, "THRUST", "RESET_COUNTERS", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_RESET_COUNTERS_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10902, "THRUST", "PING", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_PING_CC, "Exact packet length only."),
    BEE_CFE_CMD(10903, "THRUST", "RESET_MODULE", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_RESET_MODULE_CC, BEE1012_THRUST_TargetMode_Payload_t, ThrustTargetModeFields, "Target mode is validated by FSW."),
    BEE_CFE_CMD(10904, "THRUST", "SET_MODE", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_SET_MODE_CC, BEE1012_THRUST_TargetMode_Payload_t, ThrustTargetModeFields, "Target mode is validated by FSW."),
    BEE_CFE_CMD(10905, "THRUST", "ARM", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_ARM_CC, BEE1012_THRUST_Arm_Payload_t, ThrustArmFields, "Arm key and timeout are validated by FSW."),
    BEE_CFE_CMD0(10906, "THRUST", "DISARM", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_DISARM_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10907, "THRUST", "REQ_STATUS", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_REQ_STATUS_CC, "Exact packet length only."),
    BEE_CFE_CMD(10908, "THRUST", "MAIN_FIRE", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_MAIN_FIRE_CC, BEE1012_THRUST_MainFire_Payload_t, ThrustMainFireFields, "Fire timing and key are validated by FSW."),
    BEE_CFE_CMD0(10909, "THRUST", "MAIN_ABORT", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_MAIN_ABORT_CC, "Exact packet length only."),
    BEE_CFE_CMD(10910, "THRUST", "CG_PULSE", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_CG_PULSE_CC, BEE1012_THRUST_CGPulse_Payload_t, ThrustCgpulseFields, "Thruster, pulse width and key are validated by FSW."),
    BEE_CFE_CMD0(10911, "THRUST", "CG_ABORT", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_CG_ABORT_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10912, "THRUST", "REQ_FAULT_LOG", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_REQ_FAULT_LOG_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10913, "THRUST", "CLEAR_FAULT", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_CLEAR_FAULT_CC, "Exact packet length only."),
    BEE_CFE_CMD0(10914, "THRUST", "REQ_HK", BEE1012_THRUST_CMD_ID, BEE1012_THRUST_REQ_HK_CC, "Exact packet length only."),
    BEE_CFE_CMD(10915, "THRUST", "SCH_HK_ENABLE", BEE1012_THRUST_CMD_ID,
                BEE1012_THRUST_SCH_HK_ENABLE_CC, BEE1012_THRUST_ScheduledHkEnable_Payload_t,
                ThrustScheduledHkEnableFields, "ScheduledHkEnabled must be 0 or 1."),

    BEE_CFE_CMD0(7024, "PAYUEL_LGPM", "INTERVAL_PWR_SENSE_INFO", PAYUEL_LGPM_CMD_ID,
                 PAYUEL_LGPM_INTERVAL_REQUEST_PWR_SENSE_INFO_CC,
                 "Exact 8-byte packet; handler performs the fixed delayed power-on/sense sequence."),

    UEL_REMOTE_CMD0(9600, "PAYYONSEI/TEMP", "NOOP", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_NOOP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9601, "PAYYONSEI/TEMP", "RESET_COUNTERS", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_RESET_COUNTERS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9602, "PAYYONSEI/TEMP", "READ_TEMP", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_READ_TEMP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD(9603, "PAYYONSEI/TEMP", "SET_CONFIG", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                   UELYSYS_PAYYONSEI_TEMP_SET_CONFIG_CC, UELYSYS_PAYYONSEI_TEMP_SetConfig_Payload_t,
                   PayyonseiTempSetConfigFields, UEL_UL_PAYYONSEI_OBC,
                   "Exact packet length; FSW forwards the raw 16-bit register value to both sensors."),
    UEL_REMOTE_CMD0(9604, "PAYYONSEI/TEMP", "READ_CONFIG", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_READ_CONFIG_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD(9605, "PAYYONSEI/TEMP", "SET_LIMITS", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                   UELYSYS_PAYYONSEI_TEMP_SET_LIMITS_CC, UELYSYS_PAYYONSEI_TEMP_SetLimits_Payload_t,
                   PayyonseiTempSetLimitsFields, UEL_UL_PAYYONSEI_OBC,
                   "Exact packet length; values are raw MCP9808 upper/lower/critical registers."),
    UEL_REMOTE_CMD0(9606, "PAYYONSEI/TEMP", "READ_LIMITS", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_READ_LIMITS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD(9607, "PAYYONSEI/TEMP", "SET_RESOLUTION", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                   UELYSYS_PAYYONSEI_TEMP_SET_RESOLUTION_CC, UELYSYS_PAYYONSEI_TEMP_SetResolution_Payload_t,
                   PayyonseiTempSetResolutionFields, UEL_UL_PAYYONSEI_OBC,
                   "Exact packet length; FSW does not range-check ResVal; reserved padding byte is forced to zero."),
    UEL_REMOTE_CMD0(9608, "PAYYONSEI/TEMP", "READ_ID", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_READ_ID_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9610, "PAYYONSEI/TEMP", "START_LOG", UELYSYS_PAYYONSEI_TEMP_CMD_ID,
                    UELYSYS_PAYYONSEI_TEMP_START_LOG_CC, UEL_UL_PAYYONSEI_OBC,
                    "Rejects the command while the fixed 20-minute logging task is already active."),

    UEL_REMOTE_CMD0(9620, "PAYYONSEI/AOS", "NOOP", UELYSYS_PAYUEL_AOS_CMD_ID,
                    UELYSYS_PAYUEL_AOS_NOOP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9621, "PAYYONSEI/AOS", "RESET_COUNTERS", UELYSYS_PAYUEL_AOS_CMD_ID,
                    UELYSYS_PAYUEL_AOS_RESET_COUNTERS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9622, "PAYYONSEI/AOS", "RESET", UELYSYS_PAYUEL_AOS_CMD_ID,
                    UELYSYS_PAYUEL_AOS_RESET_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD(9623, "PAYYONSEI/AOS", "WRITE_REGISTER", UELYSYS_PAYUEL_AOS_CMD_ID,
                   UELYSYS_PAYUEL_AOS_WRITE_REGISTER_CC, PayyonseiPayuelAosWriteRegisterPayload,
                   PayyonseiPayuelAosWriteRegisterFields, UEL_UL_PAYYONSEI_OBC,
                   "Exact packed 3-byte payload."),
    UEL_REMOTE_CMD(9624, "PAYYONSEI/AOS", "READ_REGISTER", UELYSYS_PAYUEL_AOS_CMD_ID,
                   UELYSYS_PAYUEL_AOS_READ_REGISTER_CC, PayyonseiPayuelAosReadRegisterPayload,
                   PayyonseiPayuelAosReadRegisterFields, UEL_UL_PAYYONSEI_OBC,
                   "Exact packed 1-byte payload."),
    UEL_REMOTE_CMD0(9625, "PAYYONSEI/AOS", "READ_ALL_CHANNELS", UELYSYS_PAYUEL_AOS_CMD_ID,
                    UELYSYS_PAYUEL_AOS_READ_ALL_CHANNELS_CC, UEL_UL_PAYYONSEI_OBC,
                    "Exact packet length only."),

    UEL_REMOTE_CMD0(9630, "PAYYONSEI/LGBAT", "NOOP", UELYSYS_LGBAT_CMD_ID,
                    UELYSYS_LGBAT_NOOP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9631, "PAYYONSEI/LGBAT", "RESET_COUNTER", UELYSYS_LGBAT_CMD_ID,
                    UELYSYS_LGBAT_RESET_COUNTERS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9632, "PAYYONSEI/LGBAT", "SEND_BCN", UELYSYS_LGBAT_CMD_ID,
                    UELYSYS_LGBAT_SEND_BCN_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD(9633, "PAYYONSEI/LGBAT", "REQUEST_DATA", UELYSYS_LGBAT_CMD_ID,
                   UELYSYS_LGBAT_REQUEST_DATA_CC, PayyonseiPayuelLgbatRequestDataPayload,
                   PayyonseiPayuelLgbatRequestDataFields, UEL_UL_PAYYONSEI_OBC,
                   "DataID plus three zero-initialized spare bytes."),
    UEL_REMOTE_CMD0(9634, "PAYYONSEI/LGBAT", "REQUEST_ALL_DATA", UELYSYS_LGBAT_CMD_ID,
                    UELYSYS_LGBAT_REQUEST_ALL_DATA_CC, UEL_UL_PAYYONSEI_OBC,
                    "Exact packet length only."),

    UEL_REMOTE_CMD0(9690, "PAYYONSEI/TEMP", "SEND_HK", UELYSYS_PAYYONSEI_TEMP_SEND_HK_ID,
                    0, UEL_UL_PAYYONSEI_OBC, "Housekeeping request MID; function code is ignored."),

    UEL_REMOTE_CMD0(9700, "PAYYONSEI/IMU", "NOOP", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_NOOP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9701, "PAYYONSEI/IMU", "RESET_COUNTERS", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_RESET_COUNTERS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9702, "PAYYONSEI/IMU", "RESET_MODULE", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_RESET_MODULE_CC, UEL_UL_PAYYONSEI_OBC,
                    "Resets the sensor and restores its 100 Hz measurement setup."),
    UEL_REMOTE_CMD0(9703, "PAYYONSEI/IMU", "SEND_DATA", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_SEND_DATA_CC, UEL_UL_PAYYONSEI_OBC,
                    "Publishes the most recently saved measurement."),
    UEL_REMOTE_CMD0(9704, "PAYYONSEI/IMU", "RUN_SELFTEST", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_RUN_SELFTEST_CC, UEL_UL_PAYYONSEI_OBC,
                    "Sensor must be ready and in measurement mode."),
    UEL_REMOTE_CMD0(9705, "PAYYONSEI/IMU", "SAVE_LOG_FILE", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_SAVE_LOG_FILE_CC, UEL_UL_PAYYONSEI_OBC,
                    "Writes the saved in-memory records to the next /tmp/imu_log_NNN.txt file."),
    UEL_REMOTE_CMD0(9706, "PAYYONSEI/IMU", "START_LOGGING", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_START_LOGGING_CC, UEL_UL_PAYYONSEI_OBC,
                    "Rejected while a CSV logging session is already active."),
    UEL_REMOTE_CMD0(9707, "PAYYONSEI/IMU", "MEASURE", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_MEASURE_CC, UEL_UL_PAYYONSEI_OBC,
                    "Sensor must be ready and in measurement mode."),
    UEL_REMOTE_CMD0(9708, "PAYYONSEI/IMU", "STOP_LOGGING", UELYSYS_PAYYONSEI_IMU_CMD_ID,
                    UELYSYS_PAYYONSEI_IMU_STOP_LOGGING_CC, UEL_UL_PAYYONSEI_OBC,
                    "Rejected when no CSV logging session is active."),
    UEL_REMOTE_CMD0(9790, "PAYYONSEI/IMU", "SEND_HK", UELYSYS_PAYYONSEI_IMU_SEND_HK_ID,
                    0, UEL_UL_PAYYONSEI_OBC, "Housekeeping request MID; function code is ignored."),
    UEL_REMOTE_CMD0(9791, "PAYYONSEI/IMU", "WAKEUP", UELYSYS_PAYYONSEI_IMU_WAKEUP_ID,
                    0, UEL_UL_PAYYONSEI_OBC,
                    "Internal scheduler trigger exposed for controlled ground operation; exact header length only."),

    UEL_REMOTE_CMD0(9800, "PAYYONSEI/IWSB", "NOOP", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                    UELYSYS_PAYYONSEI_IWSB_NOOP_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9801, "PAYYONSEI/IWSB", "RESET_COUNTERS", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                    UELYSYS_PAYYONSEI_IWSB_RESET_COUNTERS_CC, UEL_UL_PAYYONSEI_OBC, "Exact packet length only."),
    UEL_REMOTE_CMD0(9802, "PAYYONSEI/IWSB", "MISSION_START", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                    UELYSYS_PAYYONSEI_IWSB_MISSION_START_CC, UEL_UL_PAYYONSEI_OBC,
                    "Starts one 100-sweep acquisition mission."),
    UEL_REMOTE_CMD(9803, "PAYYONSEI/IWSB", "ERASE_SLOT", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                   UELYSYS_PAYYONSEI_IWSB_ERASE_SLOT_CC, UELYSYS_PAYYONSEI_IWSB_Slot_Payload_t,
                   PayyonseiIwsbSlotFields, UEL_UL_PAYYONSEI_OBC,
                   "Management commands are rejected while a mission is pending/running; slot must be 0."),
    UEL_REMOTE_CMD(9804, "PAYYONSEI/IWSB", "WRITE_FW", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                   UELYSYS_PAYYONSEI_IWSB_WRITE_FW_CC, UELYSYS_PAYYONSEI_IWSB_WriteFirmware_Payload_t,
                   PayyonseiIwsbWriteFirmwareFields, UEL_UL_PAYYONSEI_OBC,
                   "Mission must be inactive; nonempty regular file in slot 0, nonzero size, 4-byte aligned."),
    UEL_REMOTE_CMD(9805, "PAYYONSEI/IWSB", "VERIFY_SLOT", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                   UELYSYS_PAYYONSEI_IWSB_VERIFY_SLOT_CC, UELYSYS_PAYYONSEI_IWSB_Slot_Payload_t,
                   PayyonseiIwsbSlotFields, UEL_UL_PAYYONSEI_OBC,
                   "Management commands are rejected while a mission is pending/running; slot must be 0."),
    UEL_REMOTE_CMD(9806, "PAYYONSEI/IWSB", "UPDATE_FW", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                   UELYSYS_PAYYONSEI_IWSB_UPDATE_FW_CC, UELYSYS_PAYYONSEI_IWSB_UpdateFirmware_Payload_t,
                   PayyonseiIwsbUpdateFirmwareFields, UEL_UL_PAYYONSEI_OBC,
                   "Mission must be inactive; target must be 0..4."),
    UEL_REMOTE_CMD0(9807, "PAYYONSEI/IWSB", "GET_CURRENT_FW", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                    UELYSYS_PAYYONSEI_IWSB_GET_CURRENT_FW_CC, UEL_UL_PAYYONSEI_OBC,
                    "Management command is rejected while a mission is pending/running."),
    UEL_REMOTE_CMD0(9808, "PAYYONSEI/IWSB", "GET_THERMAL", UELYSYS_PAYYONSEI_IWSB_CMD_ID,
                    UELYSYS_PAYYONSEI_IWSB_GET_THERMAL_CC, UEL_UL_PAYYONSEI_OBC,
                    "Mission must be inactive; response is validated as -100..200 C and 500..5500 mV."),
    UEL_REMOTE_CMD0(9890, "PAYYONSEI/IWSB", "SEND_HK", UELYSYS_PAYYONSEI_IWSB_SEND_HK_ID,
                    0, UEL_UL_PAYYONSEI_OBC, "Housekeeping request MID; function code is ignored."),

    UEL_REMOTE_CMD0(9900, "SDR_CONTROLLER", "NOOP", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_NOOP_CC, UEL_UL_SDR_CONTROLLER, "Exact packet length only."),
    UEL_REMOTE_CMD0(9901, "SDR_CONTROLLER", "RESET_COUNTERS", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_RESET_COUNTERS_CC, UEL_UL_SDR_CONTROLLER, "Exact packet length only."),
    UEL_REMOTE_CMD0(9902, "SDR_CONTROLLER", "AMP_ON", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_AMP_ON_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; enables the AMP GPIO and starts a 10-minute thermal logger child task."),
    UEL_REMOTE_CMD0(9903, "SDR_CONTROLLER", "AMP_OFF", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_AMP_OFF_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; disables the AMP GPIO."),
    UEL_REMOTE_CMD0(9904, "SDR_CONTROLLER", "PSDR_ON", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_PSDR_ON_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; invokes the fixed FSW i2cset power command."),
    UEL_REMOTE_CMD0(9905, "SDR_CONTROLLER", "PSDR_OFF", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_PSDR_OFF_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; invokes the fixed FSW i2cset power command."),
    UEL_REMOTE_CMD(9906, "SDR_CONTROLLER", "SET_PHASE_SHIFT", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_SET_PHASE_SHIFT_CC,
                   UELYSYS_SDR_CONTROLLER_SetPhaseShift_Payload_t, SdrControllerSetPhaseShiftFields,
                   UEL_UL_SDR_CONTROLLER,
                   "Exact packet length; channel 1/2 is handled, mode is boolean, flag bits 0..5 select phase GPIOs."),
    UEL_REMOTE_CMD0(9907, "SDR_CONTROLLER", "MOVE_DATA_FILE", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_MOVE_DATA_FILE_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; moves /root/temp/* to /root/save/."),
    UEL_REMOTE_CMD0(9908, "SDR_CONTROLLER", "DEL_TEMP_FILE", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_DEL_TEMP_FILE_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; removes /root/temp/*."),
    UEL_REMOTE_CMD0(9909, "SDR_CONTROLLER", "DEL_SAVE_FILE", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_DEL_SAVE_FILE_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; removes /root/save/*."),
    UEL_REMOTE_CMD(9910, "SDR_CONTROLLER", "DEL_FW_IMG", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_DEL_FW_IMG_CC,
                   UELYSYS_SDR_CONTROLLER_Filename_Payload_t, SdrControllerFilenameFields,
                   UEL_UL_SDR_CONTROLLER,
                   "Exact packet length; filename_len is not range-checked by FSW; deletes under /root/FW/."),
    UEL_REMOTE_CMD0(9911, "SDR_CONTROLLER", "RF_TESTPATTERN", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_RF_TESTPATTERN_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; sends the SDR test-pattern request over I2C."),
    UEL_REMOTE_CMD(9912, "SDR_CONTROLLER", "RF_POBC", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_RF_POBC_CC,
                   UELYSYS_SDR_CONTROLLER_Filename_Payload_t, SdrControllerFilenameFields,
                   UEL_UL_SDR_CONTROLLER,
                   "Exact packet length; opens the named /root/POBC/ file and streams it to SDR over SPI."),
    UEL_REMOTE_CMD(9913, "SDR_CONTROLLER", "RF_THERMAL", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_RF_THERMAL_CC,
                   UELYSYS_SDR_CONTROLLER_Filename_Payload_t, SdrControllerFilenameFields,
                   UEL_UL_SDR_CONTROLLER,
                   "Exact packet length; opens the named /root/save/ file and streams it to SDR over SPI."),
    UEL_REMOTE_CMD(9914, "SDR_CONTROLLER", "PSDR_GET_SLOT_INFO", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_PSDR_GET_SLOT_INFO_CC,
                   UELYSYS_SDR_CONTROLLER_Slot_Payload_t, SdrControllerSlotFields,
                   UEL_UL_SDR_CONTROLLER, "Exact packet length; FSW applies no slot range check."),
    UEL_REMOTE_CMD(9915, "SDR_CONTROLLER", "PSDR_ERASE_SLOT", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_PSDR_ERASE_SLOT_CC,
                   UELYSYS_SDR_CONTROLLER_Slot_Payload_t, SdrControllerSlotFields,
                   UEL_UL_SDR_CONTROLLER, "Exact packet length; FSW applies no slot range check."),
    UEL_REMOTE_CMD(9916, "SDR_CONTROLLER", "PSDR_WRITE_FW", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_PSDR_WRITE_FW_CC,
                   UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t, SdrControllerWriteFirmwareFields,
                   UEL_UL_SDR_CONTROLLER,
                   "Exact packet length; opens the named /root/FW/ file; filename length and slot are not range-checked."),
    UEL_REMOTE_CMD(9917, "SDR_CONTROLLER", "PSDR_VERIFYSLOTIMG", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_PSDR_VERIFY_SLOT_CC,
                   UELYSYS_SDR_CONTROLLER_Slot_Payload_t, SdrControllerSlotFields,
                   UEL_UL_SDR_CONTROLLER, "Exact packet length; FSW applies no slot range check."),
    UEL_REMOTE_CMD(9918, "SDR_CONTROLLER", "PSDR_UPDATE_FW", UELYSYS_SDR_CONTROLLER_CMD_ID,
                   UELYSYS_SDR_CONTROLLER_PSDR_UPDATE_FW_CC,
                   UELYSYS_SDR_CONTROLLER_Slot_Payload_t, SdrControllerSlotFields,
                   UEL_UL_SDR_CONTROLLER, "Exact packet length; FSW applies no slot range check."),
    UEL_REMOTE_CMD0(9919, "SDR_CONTROLLER", "PSDR_GET_CUR_FW", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_PSDR_GET_CUR_FW_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; reads the current SDR firmware version over I2C."),
    UEL_REMOTE_CMD0(9920, "SDR_CONTROLLER", "PSDR_GET_THERMAL", UELYSYS_SDR_CONTROLLER_CMD_ID,
                    UELYSYS_SDR_CONTROLLER_PSDR_GET_THERMAL_CC, UEL_UL_SDR_CONTROLLER,
                    "Exact packet length; reads SDR thermal and voltage information over I2C.")};

#undef BEE_CFE_CMD
#undef BEE_CFE_CMD0
#undef UEL_REMOTE_CMD
#undef UEL_REMOTE_CMD0
#undef BEE_CFE_FIELD

static const Bee1012CfeCommand *FindBee1012CfeCommand(int gui_index)
{
    for (const auto &cmd : Bee1012CfeCommands)
    {
        if (cmd.GuiIndex == gui_index)
            return &cmd;
    }
    return nullptr;
}

static void SendBee1012CfeCommand(const Bee1012CfeCommand &definition, uint16_t msgid,
                                  uint8_t fnccode, const uint8_t *payload)
{
    constexpr size_t MaxPayloadSize = sizeof(BEE1012_CFE_ES_StartApp_Payload_t);
    const size_t total_size = CFE_SB_CMD_HDR_SIZE + definition.PayloadSize;
    if (definition.PayloadSize > MaxPayloadSize)
    {
        console.AddLog("[ERROR]##cFE command buffer overflow blocked (%zu bytes).", total_size);
        return;
    }

    uint8_t wire[CFE_SB_CMD_HDR_SIZE + MaxPayloadSize] = {0};
    wire[0] = (uint8_t)(msgid >> 8);
    wire[1] = (uint8_t)(msgid & 0xFF);
    wire[2] = 0xC0;
    wire[3] = 0x00;
    const uint16_t ccsds_length = (uint16_t)(total_size - 7);
    wire[4] = (uint8_t)(ccsds_length >> 8);
    wire[5] = (uint8_t)(ccsds_length & 0xFF);
    wire[6] = fnccode;
    if (definition.PayloadSize != 0)
        memcpy(wire + CFE_SB_CMD_HDR_SIZE, payload, definition.PayloadSize);

    wire[7] = 0;
    uint8_t checksum = 0xFF;
    for (size_t i = 0; i < total_size; ++i)
        checksum ^= wire[i];
    wire[7] = checksum;

    packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + total_size);
    if (pkt == nullptr)
    {
        console.AddLog("[ERROR]##Unable to allocate cFE command packet.");
        return;
    }
    pkt->Identifier = HVD_TEST;
    pkt->PacketType = definition.PacketType;
    pkt->Length = (uint16_t)total_size;
    memcpy(pkt->Data, wire, total_size);

    WriteSystemName(msgid);
    mim::svc::WaitTx();
    mim::svc::SendUplink((void *)pkt);
    console.AddLog("Generated %s %s: MID=0x%04X CC=%u size=%zu packet-type=%u.",
                   definition.Module, definition.Name, msgid, fnccode,
                   total_size, definition.PacketType);
}

bool DrawBee1012CfeCommandEditor(int gui_index)
{
    const Bee1012CfeCommand *definition = FindBee1012CfeCommand(gui_index);
    if (definition == nullptr)
        return false;

    static int last_gui_index = -1;
    static uint16_t msgid = 0;
    static uint8_t fnccode = 0;
    static uint8_t payload[sizeof(BEE1012_CFE_ES_StartApp_Payload_t)] = {0};
    static int srl_config_kind = 0;
    if (last_gui_index != gui_index)
    {
        msgid = definition->Mid;
        fnccode = definition->Cc;
        memset(payload, 0, sizeof(payload));
        srl_config_kind = 0;
        last_gui_index = gui_index;
    }

    ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL, "0x%04X");
    ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

    for (size_t i = 0; i < definition->FieldCount; ++i)
    {
        const Bee1012CfeField &field = definition->Fields[i];
        char label[160];
        snprintf(label, sizeof(label), "%s##bee_cfe_%d_%zu", field.Label, gui_index, i);
        uint8_t *address = payload + field.Offset;
        switch (field.Kind)
        {
            case Bee1012CfeFieldKind::U8:
            {
                uint8_t value;
                memcpy(&value, address, sizeof(value));
                if (ImGui::InputScalar(label, ImGuiDataType_U8, &value))
                    memcpy(address, &value, sizeof(value));
                break;
            }
            case Bee1012CfeFieldKind::Bool:
            {
                bool value;
                memcpy(&value, address, sizeof(value));
                if (ImGui::Checkbox(label, &value))
                    memcpy(address, &value, sizeof(value));
                break;
            }
            case Bee1012CfeFieldKind::U16:
            {
                uint16_t value;
                memcpy(&value, address, sizeof(value));
                if (ImGui::InputScalar(label, ImGuiDataType_U16, &value))
                    memcpy(address, &value, sizeof(value));
                break;
            }
            case Bee1012CfeFieldKind::I16:
            {
                int16_t value;
                memcpy(&value, address, sizeof(value));
                if (ImGui::InputScalar(label, ImGuiDataType_S16, &value))
                    memcpy(address, &value, sizeof(value));
                break;
            }
            case Bee1012CfeFieldKind::U32:
            {
                uint32_t value;
                memcpy(&value, address, sizeof(value));
                if (ImGui::InputScalar(label, ImGuiDataType_U32, &value))
                    memcpy(address, &value, sizeof(value));
                break;
            }
            case Bee1012CfeFieldKind::String:
                ImGui::InputText(label, (char *)address, field.Size);
                break;
        }
    }

    if (gui_index == 9506)
    {
        static const char *config_names[] = {"I2C", "SPI", "CAN", "UART / RS422"};
        const int previous_config_kind = srl_config_kind;
        if (ImGui::Combo("Config interpretation##bee_cfe_srl_config", &srl_config_kind, config_names,
                         IM_ARRAYSIZE(config_names)) && previous_config_kind != srl_config_kind)
        {
            memset(payload + 5, 0, 12);
        }
        auto input_u8 = [&](const char *name, size_t offset) {
            uint8_t value;
            memcpy(&value, payload + offset, sizeof(value));
            if (ImGui::InputScalar(name, ImGuiDataType_U8, &value))
                memcpy(payload + offset, &value, sizeof(value));
        };
        auto input_u32 = [&](const char *name, size_t offset) {
            uint32_t value;
            memcpy(&value, payload + offset, sizeof(value));
            if (ImGui::InputScalar(name, ImGuiDataType_U32, &value))
                memcpy(payload + offset, &value, sizeof(value));
        };

        /* Payload offsets 5..16 are the 32-bit OBC's protocol-config union. */
        switch (srl_config_kind)
        {
            case 0:
                input_u8("Ten-bit address##bee_cfe_srl_i2c", 5);
                input_u8("PEC enable##bee_cfe_srl_i2c", 6);
                input_u8("Retries##bee_cfe_srl_i2c", 7);
                break;
            case 1:
                input_u8("SPI mode##bee_cfe_srl_spi", 5);
                input_u8("Bits per word##bee_cfe_srl_spi", 6);
                input_u32("SPI speed##bee_cfe_srl_spi", 9);
                break;
            case 2:
                memset(payload + 5, 0, 4); /* Never uplink a ground-process pointer. */
                input_u8("CAN filter count (pointer forced null)##bee_cfe_srl_can", 9);
                input_u8("CAN loopback enable##bee_cfe_srl_can", 10);
                input_u8("CAN receive own messages##bee_cfe_srl_can", 11);
                input_u8("CAN receive error frames##bee_cfe_srl_can", 12);
                input_u8("CAN timestamp enable##bee_cfe_srl_can", 13);
                break;
            default:
                input_u32("Baud##bee_cfe_srl_uart", 5);
                input_u8("Data bits##bee_cfe_srl_uart", 9);
                input_u8("Parity (0=none,1=even,2=odd)##bee_cfe_srl_uart", 10);
                input_u8("Stop bits##bee_cfe_srl_uart", 11);
                break;
        }
    }

    if (ImGui::Button("Generate CMD##bee_cfe_core"))
    {
        if (gui_index == 9910 || gui_index == 9912 || gui_index == 9913)
        {
            const uint8_t filename_length = (uint8_t)strnlen((const char *)payload, 50);
            memcpy(payload + offsetof(UELYSYS_SDR_CONTROLLER_Filename_Payload_t, FilenameLength),
                   &filename_length, sizeof(filename_length));
        }
        else if (gui_index == 9916)
        {
            const uint8_t filename_length = (uint8_t)strnlen((const char *)payload, 50);
            memcpy(payload + offsetof(UELYSYS_SDR_CONTROLLER_WriteFirmware_Payload_t, FilenameLength),
                   &filename_length, sizeof(filename_length));
        }
        SendBee1012CfeCommand(*definition, msgid, fnccode, payload);
    }
    return true;
}
