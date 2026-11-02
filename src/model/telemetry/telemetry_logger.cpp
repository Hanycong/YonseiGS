// Model - 텔레메트리 기록 (비콘·미션 비콘·리포트·이벤트를 받을 때마다 전역 구조체와 파일에 남긴다)
//
// miman_coms.cpp 에서 *Saver 함수와 리포트 덤프 도우미를 내용 그대로 옮긴 것.
#include "miman_config.h"
#include "miman_csp.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_radial.h"
#include <netinet/in.h>

#include <mutex>

#include "report_parser.h"
#include "telemetry_logger.h"

extern FILE *log_ptr;
extern bool raw_data;
extern Console console;
extern StateCheckUnit State;
extern Setup * setup;
extern int BeaconCounter;
extern int MissionBeaconCounter;
extern int ReportCounter;

int BeaconSaver(Beacon* bec)
{
    if (!bec) return -1;

    BeaconCounter++;

    char filename[128];
    time_t tmtime = time(0);
    struct tm* local = localtime(&tmtime);

    sprintf(filename,
            "../data/beacon/Beacon--%04d-%02d-%02d-%02d-%02d-%02d--.txt",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    FILE* fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "================= BEACON SAVE =================\n");

    // ---------------------------------------------------
    //  CCSDS HEADER
    // ---------------------------------------------------
    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MID        : %02X %02X\n", bec->CCSDS_MID[0], bec->CCSDS_MID[1]);
    fprintf(fp, "SEQ        : %02X %02X\n", bec->CCSDS_Seq[0], bec->CCSDS_Seq[1]);
    fprintf(fp, "LEN        : %02X %02X\n", bec->CCSDS_Len[0], bec->CCSDS_Len[1]);
    fprintf(fp, "TimeCode   : %02X %02X %02X %02X %02X %02X\n",
            bec->CCSDS_TimeCode[0], bec->CCSDS_TimeCode[1], bec->CCSDS_TimeCode[2],
            bec->CCSDS_TimeCode[3], bec->CCSDS_TimeCode[4], bec->CCSDS_TimeCode[5]);

    // // ---------------------------------------------------
    // //  RPT (FSW) - Should be deprecated
    // // ---------------------------------------------------
    // fprintf(fp, "\n[FSW - RPT]\n");
    // fprintf(fp, "BootCount     : %" PRIu16 "\n", bec->RPT_BootCount);
    // fprintf(fp, "SC Time Sec   : %" PRIu32 "\n", bec->RPT_ScTimeSec);
    // fprintf(fp, "SC Time Sub   : %" PRIu32 "\n", bec->RPT_ScTimeSubsec);
    // fprintf(fp, "Sequence      : %" PRIu32 "\n", bec->RPT_Sequence);
    // fprintf(fp, "Reset Cause   : %" PRIu8  "\n", bec->RPT_ResetCause);
    /********************************************************************/
    /*               BEE RPT Revision (Kweon Hyeokjin)                  */
    /********************************************************************/
    fprintf(fp, "\n[FSW - RPT]\n");
    fprintf(fp, "Cmd Counter   : %" PRIu8 "\n", bec->RPT_CmdCounter);
    fprintf(fp, "Err Counter   : %" PRIu8 "\n", bec->RPT_ErrCounter);
    fprintf(fp, "Report Q Cnt  : %" PRIu8 "\n", bec->RPT_ReportCnt);
    fprintf(fp, "Critical Q Cnt: %" PRIu8 "\n", bec->RPT_CriticalCnt);
    fprintf(fp, "Boot Count    : %" PRIu16 "\n", bec->RPT_BootCount);
    fprintf(fp, "SC Time Sec   : %" PRIu32 "\n", bec->RPT_ScTimeSec);
    fprintf(fp, "SC Time Sub   : %" PRIu32 "\n", bec->RPT_ScTimeSubsec);
    fprintf(fp, "Sequence(LSB) : %" PRIu8  "\n", bec->RPT_Sequence_LSB);
    /*--------------End of BEE RPT Revision (Kweon Hyeokjin)--------------*/

    // ---------------------------------------------------
    // STX (S-band)
    // ---------------------------------------------------
    fprintf(fp, "\n[COMS - STX]\n");
    fprintf(fp, "Symbol Rate        : %" PRIu8 "\n", bec->STX_symbol_rate);
    fprintf(fp, "Tx Power           : %" PRIu8 "\n", bec->STX_transmit_power);
    fprintf(fp, "MODCOD             : %" PRIu8 "\n", bec->STX_modcod);
    fprintf(fp, "Roll-off           : %" PRIu8 "\n", bec->STX_roll_off);
    fprintf(fp, "Pilot Signal       : %" PRIu8 "\n", bec->STX_pilot_signal);
    fprintf(fp, "FEC Frame Size     : %" PRIu8 "\n", bec->STX_fec_frame_size);
    fprintf(fp, "Pre-Tx Delay       : %" PRIu16 "\n", bec->STX_pretransmission_delay);
    fprintf(fp, "Center Frequency   : %f\n", bec->STX_center_frequency);
    fprintf(fp, "Mod Interface Type : %" PRIu8 "\n", bec->STX_modulator_interface_type);
    fprintf(fp, "LVDS IO Type       : %" PRIu8 "\n", bec->STX_lvds_io_type);
    fprintf(fp, "System State       : %" PRIu8 "\n", bec->STX_SystemState);
    fprintf(fp, "Status Flag        : %" PRIu8 "\n", bec->STX_StatusFlag);
    // fprintf(fp, "CPU Temp           : %f\n", bec->STX_CpuTemp);

    // ---------------------------------------------------
    // UANT (UHF Antenna)
    // ---------------------------------------------------
    fprintf(fp, "\n[COMS - UANT]\n");
    fprintf(fp, "UANT1 (0/1/BK) : %" PRIu8 " %" PRIu8 " (BK:%" PRIu8 ")\n",
            bec->UANT1_Chan0, bec->UANT1_Chan1, bec->UANT1_BackupActive);
    fprintf(fp, "UANT2 (0/1/BK) : %" PRIu8 " %" PRIu8 " (BK:%" PRIu8 ")\n",
            bec->UANT2_Chan0, bec->UANT2_Chan1, bec->UANT2_BackupActive);

    // ---------------------------------------------------
    // UTRX
    // ---------------------------------------------------
    fprintf(fp, "\n[COMS - UTRX]\n");
    fprintf(fp, "UTRX ActiveConf : %" PRIu8 "\n", bec->UTRX_ActiveConf);
    fprintf(fp, "UTRX BootCount  : %" PRIu16 "\n", bec->UTRX_BootCount);
    fprintf(fp, "UTRX BootCause  : %" PRIu32 "\n", bec->UTRX_BootCause);
    fprintf(fp, "UTRX Temp       : %" PRId16 "\n", bec->UTRX_BoardTemp);

    // ---------------------------------------------------
    // P60 Dock
    // ---------------------------------------------------
    fprintf(fp, "\n[PCDU - P60 DOCK]\n");
    fprintf(fp, "Cout[0..8]    : ");
    for (int i = 0; i < 9; i++) fprintf(fp, "%d ", bec->P60D_Cout[i]);
    fprintf(fp, "\n");

    fprintf(fp, "Vout[0..8]    : ");
    for (int i = 0; i < 9; i++) fprintf(fp, "%u ", bec->P60D_Vout[i]);
    fprintf(fp, "\n");

    fprintf(fp, "OutEn         : 0x%04X\n", bec->P60D_OutEn);
    fprintf(fp, "BootCause     : %" PRIu32 "\n", bec->P60D_BootCause);
    fprintf(fp, "BootCount     : %" PRIu32 "\n", bec->P60D_BootCount);
    fprintf(fp, "BattMode      : %" PRIu8  "\n", bec->P60D_BattMode);
    fprintf(fp, "HeaterOn      : %" PRIu8  "\n", bec->P60D_HeaterOn);
    fprintf(fp, "VBAT          : %" PRIu16 "\n", bec->P60D_VbatV);
    fprintf(fp, "VCC Current   : %" PRIi16 "\n", bec->P60D_VccC);
    fprintf(fp, "BattV         : %" PRIu16 "\n", bec->P60D_BattV);
    fprintf(fp, "BattTemp      : %d %d\n", bec->P60D_BattTemp[0], bec->P60D_BattTemp[1]);
    fprintf(fp, "WDT CAN Left  : %" PRIu32 "\n", bec->P60D_WdtCanLeft);
    fprintf(fp, "Batt Chg Curr : %" PRId16 "\n", bec->P60D_BattChrg);
    fprintf(fp, "Batt Dis Curr : %" PRId16 "\n", bec->P60D_BattDischrg);

    // ---------------------------------------------------
    // P60 PDU
    // ---------------------------------------------------
    fprintf(fp, "\n[PCDU - P60 PDU]\n");
    fprintf(fp, "Cout : ");
    for (int i = 0; i < 9; i++) fprintf(fp, "%d ", bec->P60P_Cout[i]);
    fprintf(fp, "\n");

    fprintf(fp, "Vout : ");
    for (int i = 0; i < 9; i++) fprintf(fp, "%u ", bec->P60P_Vout[i]);
    fprintf(fp, "\n");

    fprintf(fp, "Vcc     : %d\n", bec->P60P_Vcc);
    fprintf(fp, "ConvEn  : %" PRIu8 "\n", bec->P60P_ConvEn);
    fprintf(fp, "OutEn   : 0x%04X\n", bec->P60P_OutEn);

    // ---------------------------------------------------
    // P60 ACU
    // ---------------------------------------------------
    fprintf(fp, "\n[PCDU - P60 ACU]\n");
    fprintf(fp, "Cin : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%d ", bec->P60A_Cin[i]);
    fprintf(fp, "\n");

    fprintf(fp, "Vin : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%u ", bec->P60A_Vin[i]);
    fprintf(fp, "\n");

    // ---------------------------------------------------
    // ADCS
    // ---------------------------------------------------
    fprintf(fp, "\n[ADCS]\n");
    fprintf(fp, "PowerState       : 0x%02X\n", bec->ADCS_PowerState);
    fprintf(fp, "ControlMode      : %" PRIu8 "\n", bec->ADCS_ControlMode);
    fprintf(fp, "GYR0 Calib X     : %f\n", bec->ADCS_GYR0CalibratedRateXComponent);
    fprintf(fp, "GYR0 Calib Y     : %f\n", bec->ADCS_GYR0CalibratedRateYComponent);
    fprintf(fp, "GYR0 Calib Z     : %f\n", bec->ADCS_GYR0CalibratedRateZComponent);

    // ---------------------------------------------------
    // BINARY DUMP
    // ---------------------------------------------------
    fprintf(fp, "\n[BINARY DATA]\n");
    for (size_t i = 0; i < sizeof(*bec); i++)
        fprintf(fp, "%02X ", ((unsigned char*)bec)[i]);
    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}

int MissionBeaconSaver(MissionBeacon* misnbec)
{
    if (!misnbec) return -1;

    MissionBeaconCounter++;

    char filename[128];
    time_t tmtime = time(0);
    struct tm* local = localtime(&tmtime);

    sprintf(filename,
            "../data/missionbeacon/MissionBeacon--%04d-%02d-%02d-%02d-%02d-%02d--.txt",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    FILE* fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "================= MISSION BEACON SAVE =================\n");

    /* ===================================================
    *  CCSDS HEADER
    * =================================================== */
    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MID        : %02X %02X\n",
            misnbec->CCSDS_MID[0], misnbec->CCSDS_MID[1]);
    fprintf(fp, "SEQ        : %02X %02X\n",
            misnbec->CCSDS_Seq[0], misnbec->CCSDS_Seq[1]);
    fprintf(fp, "LEN        : %02X %02X\n",
            misnbec->CCSDS_Len[0], misnbec->CCSDS_Len[1]);
    fprintf(fp, "TimeCode   : %02X %02X %02X %02X %02X %02X\n",
            misnbec->CCSDS_TimeCode[0], misnbec->CCSDS_TimeCode[1],
            misnbec->CCSDS_TimeCode[2], misnbec->CCSDS_TimeCode[3],
            misnbec->CCSDS_TimeCode[4], misnbec->CCSDS_TimeCode[5]);

    /* ===================================================
    *  SRL HOUSEKEEPING
    * =================================================== */
    fprintf(fp, "\n[SRL HOUSEKEEPING]\n");
    fprintf(fp, "SRL Command Counter        : %" PRIu8 "\n",
            misnbec->srlpayload.CommandCounter);
    fprintf(fp, "SRL Command Error Counter  : %" PRIu8 "\n",
            misnbec->srlpayload.CommandErrorCounter);

    for (int i = 0; i < 4; i++) {
        fprintf(fp, "IOHandleStatus[%d]         : %" PRIu8 "\n",
                i, misnbec->srlpayload.IOHandleStatus[i]);
        fprintf(fp, "IOHandleTxCount[%d]        : %" PRIu16 "\n",
                i, misnbec->srlpayload.IOHandleTxCount[i]);
    }

    /* ===================================================
    *  RPT PAYLOAD SUMMARY
    * =================================================== */
    fprintf(fp, "\n[RPT PAYLOAD SUMMARY]\n");
    fprintf(fp, "CmdCounter                : %" PRIu8 "\n",
            misnbec->rptpayload.CmdCounter);
    fprintf(fp, "CmdErrCounter             : %" PRIu8 "\n",
            misnbec->rptpayload.CmdErrCounter);

    /* ===================================================
    *  RPT QUEUE INFO
    * =================================================== */
    fprintf(fp, "\n[RPT QUEUE INFO]\n");
    fprintf(fp, "ReportQueueCnt            : %" PRIu8 "\n",
            misnbec->rptpayload.ReportQueueCnt);
    fprintf(fp, "CriticalQueueCnt          : %" PRIu8 "\n",
            misnbec->rptpayload.CriticalQueueCnt);

    /* ===================================================
    *  OPERATION DATA
    * =================================================== */
    fprintf(fp, "\n[OPERATION DATA]\n");
    fprintf(fp, "BootCount                 : %" PRIu16 "\n",
            misnbec->rptpayload.BootCount);
    fprintf(fp, "TimeSec                   : %" PRIu32 "\n",
            misnbec->rptpayload.TimeSec);
    fprintf(fp, "TimeSubsec                : %" PRIu32 "\n",
            misnbec->rptpayload.TimeSubsec);
    fprintf(fp, "Sequence                  : %" PRIu32 "\n",
            misnbec->rptpayload.Sequence);
    fprintf(fp, "ResetCause                : 0x%02X\n",
            misnbec->rptpayload.ResetCause);

    /* ===================================================
    *  MISSION BEACON PAYLOAD
    * =================================================== */
    fprintf(fp, "\n[MISSION BEACON PAYLOAD]\n");
    fprintf(fp, "CommandCounter             : %" PRIu8 "\n",
            misnbec->paybcnpayload.CommandCounter);
    fprintf(fp, "CommandErrorCounter        : %" PRIu8 "\n",
            misnbec->paybcnpayload.CommandErrorCounter);
    fprintf(fp, "System Status              : %" PRIi8 "\n",
            misnbec->paybcnpayload.sys_status);

    fprintf(fp, "NTC Temp 0                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_0);
    fprintf(fp, "NTC Temp 1                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_1);
    fprintf(fp, "NTC Temp 2                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_2);
    fprintf(fp, "NTC Temp 3                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_3);
    fprintf(fp, "NTC Temp 4                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_4);
    fprintf(fp, "NTC Temp 5                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_5);
    fprintf(fp, "NTC Temp 6                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_6);
    fprintf(fp, "NTC Temp 7                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_7);
    fprintf(fp, "NTC Temp 8                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_8);
    fprintf(fp, "NTC Temp 9                 : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_9);
    fprintf(fp, "NTC Temp 10                : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_10);
    fprintf(fp, "NTC Temp 11                : %" PRIi16 "\n", misnbec->paybcnpayload.temp_ntc_11);

    /* ===================================================
    *  MISSION HOUSEKEEPING PAYLOAD
    * =================================================== */
    fprintf(fp, "\n[MISSION HOUSEKEEPING]\n");
    fprintf(fp, "HK CommandCounter          : %" PRIu8 "\n",
            misnbec->payhkpayload.CommandCounter);
    fprintf(fp, "HK CommandErrorCounter     : %" PRIu8 "\n",
            misnbec->payhkpayload.CommandErrorCounter);
    fprintf(fp, "HK System Status           : %" PRIi8 "\n",
            misnbec->payhkpayload.sys_status);

    fprintf(fp, "HK NTC Temp 0              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_0);
    fprintf(fp, "HK NTC Temp 1              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_1);
    fprintf(fp, "HK NTC Temp 2              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_2);
    fprintf(fp, "HK NTC Temp 3              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_3);
    fprintf(fp, "HK NTC Temp 4              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_4);
    fprintf(fp, "HK NTC Temp 5              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_5);
    fprintf(fp, "HK NTC Temp 6              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_6);
    fprintf(fp, "HK NTC Temp 7              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_7);
    fprintf(fp, "HK NTC Temp 8              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_8);
    fprintf(fp, "HK NTC Temp 9              : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_9);
    fprintf(fp, "HK NTC Temp 10             : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_10);
    fprintf(fp, "HK NTC Temp 11             : %" PRIi16 "\n", misnbec->payhkpayload.temp_ntc_11);

    fprintf(fp, "HK Sensor1 Data 0           : %" PRIu32 "\n",
            misnbec->payhkpayload.sen1_data_0);
    fprintf(fp, "HK Sensor1 Data 1           : %" PRIu32 "\n",
            misnbec->payhkpayload.sen1_data_1);





    // ---------------------------------------------------
    // BINARY DUMP
    // ---------------------------------------------------
    fprintf(fp, "\n[BINARY DATA]\n");
    for (size_t i = 0; i < sizeof(*misnbec); i++)
        fprintf(fp, "%02X ", ((unsigned char*)misnbec)[i]);
    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}

static void DumpHex(FILE *fp, const uint8_t *p, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        fprintf(fp, "%02X ", p[i]);
        if ((i + 1) % 16 == 0) fprintf(fp, "\n");
    }
    if (n % 16 != 0) fprintf(fp, "\n");
}

static void DumpArr_i16(FILE *fp, const char *label, const int16_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIi16 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u16(FILE *fp, const char *label, const uint16_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu16 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u8(FILE *fp, const char *label, const uint8_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu8 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpArr_u32(FILE *fp, const char *label, const uint32_t *aa, int n)
{
    fprintf(fp, "%s: ", label);
    for (int i = 0; i < n; i++) fprintf(fp, "%" PRIu32 "%s", aa[i], (i == n - 1) ? "" : " ");
    fprintf(fp, "\n");
}

static void DumpReportPayloadParsed_ByMidCc(FILE *fp, const Report *rpt)
{
    uint16_t payload_len = rpt->RetValSize;
    if (payload_len > sizeof(rpt->RetVal)) payload_len = (uint16_t)sizeof(rpt->RetVal);

    fprintf(fp, "\n================= PARSED BY MID/CC =================\n");
    fprintf(fp, "ReflectedMID : 0x%04X\n", (unsigned)rpt->ReflectedMID);
    fprintf(fp, "ReflectedCC  : 0x%02X\n", (unsigned)rpt->ReflectedCC);
    fprintf(fp, "PayloadLen   : %" PRIu16 "\n", payload_len);


    const uint8_t *p = rpt->RetVal;

    if ((rpt->ReflectedMID == EPS_CMD_ID) ||(rpt->ReflectedMID == 0x7518))
    {
        switch (rpt->ReflectedCC)
        {
            case EPS_P60_DOCK_GET_TABLE_HK_CC:
            {
                fprintf(fp, "\n[EPS P60 DOCK GET TABLE HK]\n");
                if (payload_len < sizeof(EPS_P60_DOCK_GET_TABLE_HK)) {
                    fprintf(fp, "WARN: payload too small. need=%zu got=%" PRIu16 "\n",
                            sizeof(EPS_P60_DOCK_GET_TABLE_HK), payload_len);
                    break;
                }

                EPS_P60_DOCK_GET_TABLE_HK pl;
                memcpy(&pl, p, sizeof(pl));

                fprintf(fp, "\n[Report Data]\n");
                DumpArr_i16(fp, "c_out[0..12]",  pl.c_out, 13);
                DumpArr_u16(fp, "v_out[0..12]",  pl.v_out, 13);
                DumpArr_u8 (fp, "out_en[0..12]", pl.out_en, 13);

                fprintf(fp, "\n[Temps]\n");
                DumpArr_i16(fp, "temp[0..1]", pl.temp, 2);

                fprintf(fp, "\n[Boot/Time]\n");
                fprintf(fp, "bootcause              : %" PRIu32 "\n", pl.bootcause);
                fprintf(fp, "bootcnt                : %" PRIu32 "\n", pl.bootcnt);
                fprintf(fp, "uptime                 : %" PRIu32 "\n", pl.uptime);
                fprintf(fp, "resetcause             : 0x%04X\n", (unsigned int)pl.resetcause);

                fprintf(fp, "\n[Modes/Flags]\n");
                fprintf(fp, "batt_mode              : %" PRIu8 "\n", pl.batt_mode);
                fprintf(fp, "heater_on              : %" PRIu8 "\n", pl.heater_on);
                fprintf(fp, "conv_5v_en              : %" PRIu8 "\n", pl.conv_5v_en);

                fprintf(fp, "\n[Latchups]\n");
                DumpArr_u16(fp, "latchup[0..12]", pl.latchup, 13);

                fprintf(fp, "\n[Battery/Power]\n");
                fprintf(fp, "vbat_v                 : %" PRIu16 "\n", pl.vbat_v);
                fprintf(fp, "vcc_c                  : %" PRIi16 "\n", pl.vcc_c);
                fprintf(fp, "batt_c                 : %" PRIi16 "\n", pl.batt_c);
                fprintf(fp, "batt_v                 : %" PRIu16 "\n", pl.batt_v);
                DumpArr_i16(fp, "batt_temp[0..1]", pl.batt_temp, 2);

                fprintf(fp, "\n[Device]\n");
                DumpArr_u8(fp, "device_type[0..7]",   pl.device_type, 8);
                DumpArr_u8(fp, "device_status[0..7]", pl.device_status, 8);
                fprintf(fp, "dearm_status           : %" PRIu8 "\n", pl.dearm_status);

                fprintf(fp, "\n[WDT Counters]\n");
                fprintf(fp, "wdt_cnt_gnd            : %" PRIu32 "\n", pl.wdt_cnt_gnd);
                fprintf(fp, "wdt_cnt_i2c            : %" PRIu32 "\n", pl.wdt_cnt_i2c);
                fprintf(fp, "wdt_cnt_can            : %" PRIu32 "\n", pl.wdt_cnt_can);
                DumpArr_u32(fp, "wdt_cnt_csp[0..1]", pl.wdt_cnt_csp, 2);

                fprintf(fp, "\n[WDT Left]\n");
                fprintf(fp, "wdt_gnd_left           : %" PRIu32 "\n", pl.wdt_gnd_left);
                fprintf(fp, "wdt_i2c_left           : %" PRIu32 "\n", pl.wdt_i2c_left);
                fprintf(fp, "wdt_can_left           : %" PRIu32 "\n", pl.wdt_can_left);
                DumpArr_u8(fp, "wdt_csp_left[0..1]", pl.wdt_csp_left, 2);

                fprintf(fp, "\n[Battery Currents]\n");
                fprintf(fp, "batt_chrg              : %" PRIi16 "\n", pl.batt_chrg);
                fprintf(fp, "batt_dischrg           : %" PRIi16 "\n", pl.batt_dischrg);

                fprintf(fp, "\n[Deploy]\n");
                fprintf(fp, "ant6_depl              : %" PRIi8 "\n", pl.ant6_depl);
                fprintf(fp, "ar6_depl               : %" PRIi8 "\n", pl.ar6_depl);
                break;
            }

            case EPS_P60_PDU_GET_TABLE_HK_CC:
            {
                fprintf(fp, "\n[EPS P60 PDU GET TABLE HK]\n");
                if (payload_len < sizeof(EPS_P60_PDU_GET_TABLE_HK)) {
                    fprintf(fp, "WARN: payload too small. need=%zu got=%" PRIu16 "\n",
                            sizeof(EPS_P60_PDU_GET_TABLE_HK), payload_len);
                    break;
                }

                EPS_P60_PDU_GET_TABLE_HK pl;
                memcpy(&pl, p, sizeof(pl));

                fprintf(fp, "\n[Report Data]\n");
                DumpArr_i16(fp, "c_out[0..8]",  pl.c_out, 9);
                DumpArr_u16(fp, "v_out[0..8]",  pl.v_out, 9);
                DumpArr_u8 (fp, "out_en[0..8]", pl.out_en, 9);
                DumpArr_u8 (fp, "conv_en[0..2]", pl.conv_en, 3);

                fprintf(fp, "\n[Power]\n");
                fprintf(fp, "vcc                    : %" PRIu16 "\n", pl.vcc);
                fprintf(fp, "vbat                   : %" PRIu16 "\n", pl.vbat);
                fprintf(fp, "temp                   : %" PRIi16 "\n", pl.temp);

                fprintf(fp, "\n[Boot/Time]\n");
                fprintf(fp, "bootcause              : %" PRIu32 "\n", pl.bootcause);
                fprintf(fp, "bootcnt                : %" PRIu32 "\n", pl.bootcnt);
                fprintf(fp, "uptime                 : %" PRIu32 "\n", pl.uptime);
                fprintf(fp, "resetcause             : 0x%04X\n", (unsigned int)pl.resetcause);

                fprintf(fp, "\n[Modes]\n");
                fprintf(fp, "batt_mode              : %" PRIu8 "\n", pl.batt_mode);

                fprintf(fp, "\n[Latchups]\n");
                DumpArr_u16(fp, "latchup[0..8]", pl.latchup, 9);

                fprintf(fp, "\n[Device]\n");
                DumpArr_u8(fp, "device_type[0..7]",   pl.device_type, 8);
                DumpArr_u8(fp, "device_status[0..7]", pl.device_status, 8);

                fprintf(fp, "\n[WDT Counters]\n");
                fprintf(fp, "wdt_cnt_gnd            : %" PRIu32 "\n", pl.wdt_cnt_gnd);
                fprintf(fp, "wdt_cnt_i2c            : %" PRIu32 "\n", pl.wdt_cnt_i2c);
                fprintf(fp, "wdt_cnt_can            : %" PRIu32 "\n", pl.wdt_cnt_can);
                DumpArr_u32(fp, "wdt_cnt_csp[0..1]", pl.wdt_cnt_csp, 2);

                fprintf(fp, "\n[WDT Left]\n");
                fprintf(fp, "wdt_gnd_left           : %" PRIu32 "\n", pl.wdt_gnd_left);
                fprintf(fp, "wdt_i2c_left           : %" PRIu32 "\n", pl.wdt_i2c_left);
                fprintf(fp, "wdt_can_left           : %" PRIu32 "\n", pl.wdt_can_left);
                DumpArr_u8(fp, "wdt_csp_left[0..1]", pl.wdt_csp_left, 2);

                break;
            }

            case EPS_P60_ACU_GET_TABLE_HK_CC:
            {
                fprintf(fp, "\n[EPS P60 ACU GET TABLE HK]\n");
                if (payload_len < sizeof(EPS_P60_ACU_GET_TABLE_HK)) {
                    fprintf(fp, "WARN: payload too small. need=%zu got=%" PRIu16 "\n",
                            sizeof(EPS_P60_ACU_GET_TABLE_HK), payload_len);
                    break;
                }

                EPS_P60_ACU_GET_TABLE_HK pl;
                memcpy(&pl, p, sizeof(pl));

                fprintf(fp, "\n[Inputs]\n");
                DumpArr_i16(fp, "c_in[0..5]", pl.c_in, 6);
                DumpArr_u16(fp, "v_in[0..5]", pl.v_in, 6);

                fprintf(fp, "\n[Bus/Temp]\n");
                fprintf(fp, "vbat: %" PRIu16 "\n", pl.vbat);
                fprintf(fp, "vcc : %" PRIu16 "\n", pl.vcc);
                DumpArr_i16(fp, "temp[0..2]", pl.temp, 3);

                fprintf(fp, "\n[MPPT]\n");
                fprintf(fp, "mppt_mode: %" PRIu8 "\n", pl.mppt_mode);
                DumpArr_u16(fp, "vboost[0..5]", pl.vboost, 6);
                DumpArr_u16(fp, "power[0..5]",  pl.power,  6);

                fprintf(fp, "\n[DAC]\n");
                DumpArr_u8 (fp, "dac_en[0..2]",  pl.dac_en,  3);
                DumpArr_u16(fp, "dac_val[0..5]", pl.dac_val, 6);

                fprintf(fp, "\n[Boot/Time]\n");
                fprintf(fp, "bootcause : %" PRIu32 "\n", pl.bootcause);
                fprintf(fp, "bootcnt   : %" PRIu32 "\n", pl.bootcnt);
                fprintf(fp, "uptime    : %" PRIu32 "\n", pl.uptime);
                fprintf(fp, "resetcause: 0x%04X\n", (unsigned)pl.resetcause);

                fprintf(fp, "\n[MPPT Timing]\n");
                fprintf(fp, "mppt_time  : %" PRIu16 "\n", pl.mppt_time);
                fprintf(fp, "mppt_period: %" PRIu16 "\n", pl.mppt_period);

                fprintf(fp, "\n[Device]\n");
                DumpArr_u8(fp, "device_type[0..7]", pl.device_type, 8);
                DumpArr_u8(fp, "device_status[0..7]", pl.device_status, 8);

                fprintf(fp, "\n[WDT]\n");
                fprintf(fp, "wdt_cnt_gnd : %" PRIu32 "\n", pl.wdt_cnt_gnd);
                fprintf(fp, "wdt_gnd_left: %" PRIu32 "\n", pl.wdt_gnd_left);

                break;
            }

            default:
                fprintf(fp, "\n[EPS P60] Unknown CC: 0x%02X\n", (unsigned)rpt->ReflectedCC);
                break;
        }
    }
    else
    {
        fprintf(fp, "\n[UNKNOWN MID] No parser for ReflectedMID=0x%04X\n", (unsigned)rpt->ReflectedMID);
    }

    fprintf(fp, "\n[PAYLOAD HEX]\n");
    DumpHex(fp, rpt->RetVal, payload_len);
}

int ReportSaver(Report* rpt)
{
    if (!rpt) return -1;

    ReportCounter++;

    char filename[128];
    time_t tmtime = time(0);
    struct tm* local = localtime(&tmtime);

    sprintf(filename,
            "../data/report_parsed/Report--%04d-%02d-%02d-%02d-%02d-%02d--.txt",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    FILE* fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "================= REPORT SAVE =================\n");

    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MsgId      : 0x%04X\n", (unsigned int)rpt->CCSDS_MsgId);
    fprintf(fp, "Seq        : 0x%04X\n", (unsigned int)rpt->CCSDS_Seq);
    fprintf(fp, "Len        : 0x%04X\n", (unsigned int)rpt->CCSDS_Len);
    fprintf(fp, "TimeCode   : %02X %02X %02X %02X %02X %02X\n",
            rpt->CCSDS_TimeCode[0], rpt->CCSDS_TimeCode[1],
            rpt->CCSDS_TimeCode[2], rpt->CCSDS_TimeCode[3],
            rpt->CCSDS_TimeCode[4], rpt->CCSDS_TimeCode[5]);
    fprintf(fp, "Padding    : 0x%08X\n", (unsigned int)rpt->CCSDS_Padding);

    fprintf(fp, "\n[REPORT BODY]\n");
    fprintf(fp, "Reflected MID  : 0x%04X\n", (unsigned int)rpt->ReflectedMID);
    fprintf(fp, "Reflected CC   : 0x%02X\n", (unsigned int)rpt->ReflectedCC);
    fprintf(fp, "RetType        : 0x%02X\n", (unsigned int)rpt->RetType);
    fprintf(fp, "RetCode        : %" PRId32 "\n", (int32_t)rpt->RetCode);
    fprintf(fp, "RetValSize     : %" PRIu16 "\n", (uint16_t)rpt->RetValSize);
    DumpReportPayloadParsed_ByMidCc(fp, rpt);
    fprintf(fp, "\n[RETURN VALUE]\n");
    uint16_t dump_size = rpt->RetValSize;
    if (dump_size > sizeof(rpt->RetVal))
        dump_size = sizeof(rpt->RetVal);

    for (uint16_t i = 0; i < dump_size; i++) {
        fprintf(fp, "%02X ", rpt->RetVal[i]);
        if ((i + 1) % 16 == 0)
            fprintf(fp, "\n");
    }
    if (dump_size % 16 != 0)
        fprintf(fp, "\n");

    fprintf(fp, "\n[BINARY DATA]\n");
    for (size_t i = 0; i < sizeof(*rpt); i++)
        fprintf(fp, "%02X ", ((unsigned char*)rpt)[i]);
    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}

int EventSaver(Event *event)
{
    if (!event) return -1;

    char filename[128];
    time_t tmtime = time(0);
    struct tm *local = localtime(&tmtime);

    sprintf(filename,
            "../data/event_parsed/Event--%04d-%02d-%02d-%02d-%02d-%02d--.txt",
            local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);

    FILE *fp = fopen(filename, "w");
    if (!fp) return -2;

    fprintf(fp, "===================== EVENT SAVE =====================\n");

    fprintf(fp, "\n[CCSDS HEADER]\n");
    fprintf(fp, "MsgId      : 0x%04" PRIX16 "\n", (uint16_t)event->CCSDS_MsgId);
    fprintf(fp, "Seq        : 0x%04" PRIX16 "\n", (uint16_t)event->CCSDS_Seq);
    fprintf(fp, "Len        : 0x%04" PRIX16 "\n", (uint16_t)event->CCSDS_Len);
    fprintf(fp, "TimeCode   : %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 "\n",
            (uint8_t)event->CCSDS_TimeCode[0], (uint8_t)event->CCSDS_TimeCode[1],
            (uint8_t)event->CCSDS_TimeCode[2], (uint8_t)event->CCSDS_TimeCode[3],
            (uint8_t)event->CCSDS_TimeCode[4], (uint8_t)event->CCSDS_TimeCode[5]);
    fprintf(fp, "Padding    : 0x%08" PRIX32 "\n", (uint32_t)event->CCSDS_Padding);

    fprintf(fp, "\n[EVENT PAYLOAD]\n");
    fprintf(fp, "AppName    : %.*s\n", (int)sizeof(event->AppName), event->AppName);
    fprintf(fp, "EventID    : %" PRIu16 " (0x%04" PRIX16 ")\n", (uint16_t)event->EventID, (uint16_t)event->EventID);
    fprintf(fp, "EventType  : %" PRIu16 " (0x%04" PRIX16 ")\n", (uint16_t)event->EventType, (uint16_t)event->EventType);
    fprintf(fp, "SCID       : %" PRIu32 " (0x%08" PRIX32 ")\n", (uint32_t)event->SpacecraftID, (uint32_t)event->SpacecraftID);
    fprintf(fp, "ProcessorID: %" PRIu32 " (0x%08" PRIX32 ")\n", (uint32_t)event->ProcessorID, (uint32_t)event->ProcessorID);
    fprintf(fp, "Message    : %.*s\n", (int)sizeof(event->Message), event->Message);
    fprintf(fp, "Spare1     : %" PRIu8 " (0x%02" PRIX8 ")\n", (uint8_t)event->Spare1, (uint8_t)event->Spare1);
    fprintf(fp, "Spare2     : %" PRIu8 " (0x%02" PRIX8 ")\n", (uint8_t)event->Spare2, (uint8_t)event->Spare2);

    fprintf(fp, "\n[BINARY DATA]\n");
    for (size_t i = 0; i < sizeof(*event); i++)
        fprintf(fp, "%02X ", ((const unsigned char *)event)[i]);
    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}
