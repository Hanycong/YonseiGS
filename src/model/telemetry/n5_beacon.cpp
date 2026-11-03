// Model - 누리호 5차 위성 비콘 수신/저장 (BEE1012 포트 17, UELYSYS 포트 12, SD1SAT 포트 22)
//
// GS_nuri5 miman_coms.cpp 의 비콘 저장 함수(BEE1012/UELYSYS/SD1SATBeaconSaver), 포트별 처리
// (Handle*/Process*BeaconPort), FTP 중 비콘 버퍼([FTP-BEACON-BUFFER]/[FTP-BEACON-CLOSE])를 옮긴 것.
// GS_nuri5 와 다른 점:
//  - 파일 이름에 밀리초를 넣고 같은 이름이면 번호를 붙인다 (telemetry_files.h). 초 단위 이름은
//    FTP 후 버퍼를 한꺼번에 저장할 때 서로 덮어썼다.
//  - 저장 폴더는 OpenTelemetryFile 이 만든다 (미션비콘/깨진 비콘 포함). fopen 실패는 콘솔에 남긴다.
//  - 전역 비콘 버퍼(bee1012_beacon 등)는 g_n5_beacon_mtx 아래에서 갱신한다. 화면은 Copy*Beacon 으로 복사해 읽는다.
#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"

#include <map>
#include <mutex>
#include <vector>

#include "n5_beacon.h"
#include "telemetry_files.h"

extern Console console;
extern int BeaconCounter;

// 위성 비콘 길이 (GS_nuri5 빌드와 같은 값). SD1SAT 는 91 바이트 (헤더 + OPS + UTRX + EPS + UANT)
static_assert(sizeof(BEE1012_Beacon_t) == 210, "BEE1012 beacon is 210 bytes");
static_assert(sizeof(UELYSYS_Beacon_t) == 211, "UELYSYS beacon is 211 bytes");
static_assert(sizeof(SD1SAT_Beacon_t) == 91, "SD1SAT beacon is 91 bytes (GS_nuri5)");

std::mutex g_n5_beacon_mtx;
static bool g_have_bee1012_beacon = false;   // 한 번이라도 받았는지 (g_n5_beacon_mtx)
static bool g_have_uelysys_beacon = false;
static bool g_have_sd1sat_beacon = false;
BEE1012_Beacon_t* bee1012_beacon = (BEE1012_Beacon_t *)calloc(1, BEE1012_LEN_BEACON);
UELYSYS_Beacon_t* uelysys_beacon = (UELYSYS_Beacon_t *)calloc(1, UELYSYS_LEN_BEACON);
SD1SAT_Beacon_t* sd1sat_beacon = (SD1SAT_Beacon_t *)calloc(1, SD1SAT_LEN_BEACON);

static void WriteRawBytes(FILE* fp, const char* label, const uint8_t* data, size_t len)
{
    fprintf(fp, "%-24s: ", label);
    for (size_t i = 0; i < len; i++) fprintf(fp, "%02X ", data[i]);
    fprintf(fp, "\n");
}

static void WriteCallSign(FILE* fp, const char* label, const char* call_sign)
{
    char call_sign_buf[8] = {0};
    memcpy(call_sign_buf, call_sign, 7);
    fprintf(fp, "%-24s: %s\n", label, call_sign_buf);
}

static void WriteBinaryDump(FILE* fp, const void* data, size_t len)
{
    const uint8_t* bytes = (const uint8_t*)data;
    fprintf(fp, "\n[BINARY DATA]\n");
    for (size_t i = 0; i < len; i++) {
        fprintf(fp, "%02X ", bytes[i]);
        if ((i + 1) % 16 == 0) fprintf(fp, "\n");
    }
    if (len % 16 != 0) fprintf(fp, "\n");
}

int BEE1012BeaconSaver(BEE1012_Beacon_t* bec, const struct timeval* rx_time)  /* [FTP-BEACON-BUFFER] rx_time */
{
if (!bec) return -1;

BeaconCounter++;

FILE* fp = OpenTelemetryFile("../data/BEE1012/beacon/beacon_parsed", "BEE1012Beacon--", "--.txt", rx_time, "w");  /* [FTP-BEACON-BUFFER] rx_time */
if (!fp) {
    console.AddLog("[ERROR] Failed to save BEE1012 beacon in ../data/BEE1012/beacon/beacon_parsed");
    return -2;
}

    fprintf(fp, "================= BEE1012 BEACON SAVE =================\n");
    fprintf(fp, "\n[HEADER]\n");
    WriteCallSign(fp, "Call Sign", bec->header.call_sign);
    WriteRawBytes(fp, "Message ID", bec->header.msg_id, sizeof(bec->header.msg_id));
    WriteRawBytes(fp, "Sequence", bec->header.sequence, sizeof(bec->header.sequence));
    WriteRawBytes(fp, "Length", bec->header.length, sizeof(bec->header.length));
    WriteRawBytes(fp, "Time Code", bec->header.time_code, sizeof(bec->header.time_code));

    fprintf(fp, "\n[RPT]\n");
    fprintf(fp, "boot_count              : %" PRIu16 "\n", bec->rpt.boot_count);
    fprintf(fp, "sequence                : %" PRIu32 "\n", bec->rpt.sequence);
    fprintf(fp, "reset_cause             : %" PRIu8 "\n", bec->rpt.reset_cause);

    fprintf(fp, "\n[COMS]\n");
    fprintf(fp, "AX100 active_conf       : %" PRIu8 "\n", bec->coms.ax100.active_conf);
    fprintf(fp, "AX100 boot_count        : %" PRIu16 "\n", bec->coms.ax100.boot_count);
    fprintf(fp, "AX100 boot_cause        : %" PRIu32 "\n", bec->coms.ax100.boot_cause);
    fprintf(fp, "AX100 temp_board        : %" PRId16 "\n", bec->coms.ax100.temp_board);
    fprintf(fp, "LTRX temp               : %" PRId16 "\n", bec->coms.ltrx.temp);
    fprintf(fp, "LTRX connection_quality : %" PRId8 "\n", bec->coms.ltrx.connection_quality);
    fprintf(fp, "LTRX battery_capacity   : %" PRIu16 "\n", bec->coms.ltrx.battery_capacity);

    fprintf(fp, "\n[EPS - PMU]\n");
    fprintf(fp, "bootcause               : %" PRIu32 "\n", bec->eps.pmu.bootcause);
    fprintf(fp, "resetcause              : %" PRIu16 "\n", bec->eps.pmu.resetcause);
    fprintf(fp, "bootcount               : %" PRIu16 "\n", bec->eps.pmu.bootcount);
    fprintf(fp, "out_en                  : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu8 " ", bec->eps.pmu.out_en[i]);
    fprintf(fp, "\ntemp                    : %" PRId16 " %" PRId16 "\n", bec->eps.pmu.temp[0], bec->eps.pmu.temp[1]);
    fprintf(fp, "batt_mode               : %" PRIu8 "\n", bec->eps.pmu.batt_mode);
    fprintf(fp, "batt_i                  : %" PRId16 "\n", bec->eps.pmu.batt_i);
    fprintf(fp, "batt_v                  : %" PRIu16 "\n", bec->eps.pmu.batt_v);
    fprintf(fp, "sm_en                   : %" PRIu8 "\n", bec->eps.pmu.sm_en);
    fprintf(fp, "gnd_wdt_cnt             : %" PRIu16 "\n", bec->eps.pmu.gnd_wdt_cnt);
    fprintf(fp, "bus_wdt_cnt             : %" PRIu16 "\n", bec->eps.pmu.bus_wdt_cnt);
    fprintf(fp, "gnd_wdt_left            : %" PRIu32 "\n", bec->eps.pmu.gnd_wdt_left);
    fprintf(fp, "bus_wdt_left            : %" PRIu32 "\n", bec->eps.pmu.bus_wdt_left);

    fprintf(fp, "\n[EPS - PDU]\nout_i                   : ");
    for (int i = 0; i < 13; i++) fprintf(fp, "%" PRId16 " ", bec->eps.pdu.out_i[i]);
    fprintf(fp, "\nout_en                  : ");
    for (int i = 0; i < 13; i++) fprintf(fp, "%" PRIu8 " ", bec->eps.pdu.out_en[i]);
    fprintf(fp, "\n");

    fprintf(fp, "\n[EPS - ACU]\n");
    for (int unit = 0; unit < 2; unit++) {
        fprintf(fp, "ACU %d input_i           : ", unit);
        for (int i = 0; i < 6; i++) fprintf(fp, "%" PRId16 " ", bec->eps.acu.acu[unit].input_i[i]);
        fprintf(fp, "\nACU %d input_v           : ", unit);
        for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu16 " ", bec->eps.acu.acu[unit].input_v[i]);
        fprintf(fp, "\nACU %d mppt_mode         : %" PRIu8 "\n", unit, bec->eps.acu.acu[unit].mppt_mode);
    }

    fprintf(fp, "\n[EPS - BP8]\n");
    fprintf(fp, "bootcount               : %" PRIu16 "\n", bec->eps.bp8.bootcount);
    fprintf(fp, "bootcause               : %" PRIu16 "\n", bec->eps.bp8.bootcause);
    fprintf(fp, "resetcause              : %" PRIu16 "\n", bec->eps.bp8.resetcause);
    fprintf(fp, "soc                     : %f\n", bec->eps.bp8.soc);
    fprintf(fp, "bat_avr_temp            : %f\n", bec->eps.bp8.bat_avr_temp);
    fprintf(fp, "vbat                    : %" PRIu16 "\n", bec->eps.bp8.vbat);
    fprintf(fp, "i                       : %f\n", bec->eps.bp8.i);
    fprintf(fp, "heater_i                : %" PRIu16 "\n", bec->eps.bp8.heater_i);
    fprintf(fp, "\n[EPS - GPIO]\ngpio_status             : 0x%04X\n", bec->eps.gpio.gpio_status);
    fprintf(fp, "sp_deploy_status        : %" PRIu8 "\n", bec->eps.gpio.sp_deploy_status);

    fprintf(fp, "\n[ADCS]\n");
    fprintf(fp, "power_state             : %" PRIu8 "\n", bec->adcs.power_state);
    fprintf(fp, "control_mode            : %" PRIu8 "\n", bec->adcs.control_mode);
    fprintf(fp, "gyro0_rate_x            : %f\n", bec->adcs.gyro0_calibrated_rate_x);
    fprintf(fp, "gyro0_rate_y            : %f\n", bec->adcs.gyro0_calibrated_rate_y);
    fprintf(fp, "gyro0_rate_z            : %f\n", bec->adcs.gyro0_calibrated_rate_z);
    fprintf(fp, "css                     : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu8 " ", bec->adcs.css[i]);
    fprintf(fp, "\n");

    WriteBinaryDump(fp, bec, sizeof(*bec));
    fclose(fp);
    return 0;
}

int UELYSYSBeaconSaver(UELYSYS_Beacon_t* bec, const struct timeval* rx_time)  /* [FTP-BEACON-BUFFER] rx_time */
{
if (!bec) return -1;

BeaconCounter++;

FILE* fp = OpenTelemetryFile("../data/UELYSYS/beacon/beacon_parsed", "UELYSYSBeacon--", "--.txt", rx_time, "w");  /* [FTP-BEACON-BUFFER] rx_time */
if (!fp) {
    console.AddLog("[ERROR] Failed to save UELYSYS beacon in ../data/UELYSYS/beacon/beacon_parsed");
    return -2;
}
    fprintf(fp, "================= UELYSYS BEACON SAVE =================\n");
    fprintf(fp, "\n[HEADER]\n");
    WriteCallSign(fp, "Call Sign", bec->header.call_sign);
    WriteRawBytes(fp, "Message ID", bec->header.msg_id, sizeof(bec->header.msg_id));
    WriteRawBytes(fp, "Sequence", bec->header.sequence, sizeof(bec->header.sequence));
    WriteRawBytes(fp, "Length", bec->header.length, sizeof(bec->header.length));
    WriteRawBytes(fp, "Time Code", bec->header.time_code, sizeof(bec->header.time_code));

    fprintf(fp, "\n[FSW]\n");
    fprintf(fp, "boot_count              : %" PRIu16 "\n", bec->fsw.boot_count);
    fprintf(fp, "sequence                : %" PRIu32 "\n", bec->fsw.sequence);
    fprintf(fp, "reset_cause             : %" PRIu8 "\n", bec->fsw.reset_cause);

    fprintf(fp, "\n[COMS]\n");
    fprintf(fp, "i2c1 0x05 ch0 status    : %" PRIu8 "\n", bec->coms.i2c1_0x05_channel_0_status);
    fprintf(fp, "i2c1 0x05 ch1 status    : %" PRIu8 "\n", bec->coms.i2c1_0x05_channel_1_status);
    fprintf(fp, "i2c1 0x06 ch0 status    : %" PRIu8 "\n", bec->coms.i2c1_0x06_channel_0_status);
    fprintf(fp, "i2c1 0x06 ch1 status    : %" PRIu8 "\n", bec->coms.i2c1_0x06_channel_1_status);
    fprintf(fp, "AX100 active_conf       : %" PRIu8 "\n", bec->coms.ax100_active_conf);
    fprintf(fp, "AX100 boot_count        : %" PRIu16 "\n", bec->coms.ax100_boot_count);
    fprintf(fp, "AX100 boot_cause        : %" PRIu32 "\n", bec->coms.ax100_boot_cause);
    fprintf(fp, "AX100 temp_board        : %" PRId16 "\n", bec->coms.ax100_temp_board);

    fprintf(fp, "\n[EPS - PMU]\n");
    fprintf(fp, "bootcause               : %" PRIu32 "\n", bec->eps.pmu.bootcause);
    fprintf(fp, "resetcause              : %" PRIu16 "\n", bec->eps.pmu.resetcause);
    fprintf(fp, "bootcount               : %" PRIu16 "\n", bec->eps.pmu.bootcount);
    fprintf(fp, "out_en                  : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu8 " ", bec->eps.pmu.out_en[i]);
    fprintf(fp, "\ntemp                    : %" PRId16 " %" PRId16 "\n", bec->eps.pmu.temp[0], bec->eps.pmu.temp[1]);
    fprintf(fp, "batt_mode               : %" PRIu8 "\n", bec->eps.pmu.batt_mode);
    fprintf(fp, "batt_i                  : %" PRId16 "\n", bec->eps.pmu.batt_i);
    fprintf(fp, "batt_v                  : %" PRIu16 "\n", bec->eps.pmu.batt_v);
    fprintf(fp, "sm_en                   : %" PRIu8 "\n", bec->eps.pmu.sm_en);
    fprintf(fp, "gnd_wdt_cnt             : %" PRIu16 "\n", bec->eps.pmu.gnd_wdt_cnt);
    fprintf(fp, "bus_wdt_cnt             : %" PRIu16 "\n", bec->eps.pmu.bus_wdt_cnt);
    fprintf(fp, "gnd_wdt_left            : %" PRIu32 "\n", bec->eps.pmu.gnd_wdt_left);
    fprintf(fp, "bus_wdt_left            : %" PRIu32 "\n", bec->eps.pmu.bus_wdt_left);

    fprintf(fp, "\n[EPS - PDU]\nout_i                   : ");
    for (int i = 0; i < 13; i++) fprintf(fp, "%" PRId16 " ", bec->eps.pdu.out_i[i]);
    fprintf(fp, "\nout_en                  : ");
    for (int i = 0; i < 13; i++) fprintf(fp, "%" PRIu8 " ", bec->eps.pdu.out_en[i]);
    fprintf(fp, "\n");

    fprintf(fp, "\n[EPS - ACU]\n");
    for (int unit = 0; unit < 2; unit++) {
        fprintf(fp, "ACU %d input_i           : ", unit);
        for (int i = 0; i < 6; i++) fprintf(fp, "%" PRId16 " ", bec->eps.acu.acu[unit].input_i[i]);
        fprintf(fp, "\nACU %d input_v           : ", unit);
        for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu16 " ", bec->eps.acu.acu[unit].input_v[i]);
        fprintf(fp, "\nACU %d mppt_mode         : %" PRIu8 "\n", unit, bec->eps.acu.acu[unit].mppt_mode);
    }

    fprintf(fp, "\n[EPS - BP8]\n");
    fprintf(fp, "bootcount               : %" PRIu16 "\n", bec->eps.bp8.bootcount);
    fprintf(fp, "bootcause               : %" PRIu16 "\n", bec->eps.bp8.bootcause);
    fprintf(fp, "resetcause              : %" PRIu16 "\n", bec->eps.bp8.resetcause);
    fprintf(fp, "soc                     : %f\n", bec->eps.bp8.soc);
    fprintf(fp, "bat_avr_temp            : %f\n", bec->eps.bp8.bat_avr_temp);
    fprintf(fp, "vbat                    : %" PRIu16 "\n", bec->eps.bp8.vbat);
    fprintf(fp, "i                       : %f\n", bec->eps.bp8.i);
    fprintf(fp, "heater_i                : %" PRIu16 "\n", bec->eps.bp8.heater_i);
    fprintf(fp, "\n[EPS - DSP]\n");
    fprintf(fp, "dsp_i2c1_0x07_status    : %" PRIu8 "\n", bec->eps.dsp.dsp_i2c1_0x07_status);
    fprintf(fp, "dsp_i2c1_0x08_status    : %" PRIu8 "\n", bec->eps.dsp.dsp_i2c1_0x08_status);
    fprintf(fp, "dsp_i2c1_0x09_status    : %" PRIu8 "\n", bec->eps.dsp.dsp_i2c1_0x09_status);
    fprintf(fp, "dsp_i2c1_0x10_status    : %" PRIu8 "\n", bec->eps.dsp.dsp_i2c1_0x10_status);
    fprintf(fp, "dsp_GPIO_status    : %" PRIu8 "\n", bec->eps.dsp.dsp_GPIO_status);

    fprintf(fp, "\n[ADCS]\n");
    fprintf(fp, "power_state             : %" PRIu8 "\n", bec->adcs.power_state);
    fprintf(fp, "control_mode            : %" PRIu8 "\n", bec->adcs.control_mode);
    fprintf(fp, "gyro0_rate_x            : %f\n", bec->adcs.gyro0_calibrated_rate_x);
    fprintf(fp, "gyro0_rate_y            : %f\n", bec->adcs.gyro0_calibrated_rate_y);
    fprintf(fp, "gyro0_rate_z            : %f\n", bec->adcs.gyro0_calibrated_rate_z);
    fprintf(fp, "css                     : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu8 " ", bec->adcs.css[i]);
    fprintf(fp, "\n");

    WriteBinaryDump(fp, bec, sizeof(*bec));
    fclose(fp);
    return 0;
}


int SD1SATBeaconSaver(SD1SAT_Beacon_t* bec, const struct timeval* rx_time)  /* [FTP-BEACON-BUFFER] rx_time */
{
    if (!bec) return -1;

    BeaconCounter++;

    FILE* fp = OpenTelemetryFile("../data/SD1SAT/beacon/beacon_parsed", "SD1SATBeacon--", "--.txt", rx_time, "w");  /* [FTP-BEACON-BUFFER] rx_time */
if (!fp) {
    console.AddLog("[ERROR] Failed to save SD1SAT beacon in ../data/SD1SAT/beacon/beacon_parsed");
    return -2;
}

    fprintf(fp, "================= SD1SAT BEACON SAVE =================\n");

    fprintf(fp, "\n[HEADER]\n");
    {
        char call_sign_buf[5] = {0};
        memcpy(call_sign_buf, bec->header.call_sign, sizeof(bec->header.call_sign));
        fprintf(fp, "%-24s: %s\n", "Call Sign", call_sign_buf);
    }
    fprintf(fp, "\n[OPS]\n");
    fprintf(fp, "RestoredTimestamp       : %" PRIu32 "\n", bec->OPS.RestoredTimestamp);
    fprintf(fp, "SecondsSinceFirstBoot   : %" PRIu32 "\n", bec->OPS.SecondsSinceFirstBoot);
    fprintf(fp, "BootCount               : %" PRIu16 "\n", bec->OPS.BootCount);
    fprintf(fp, "WarningFlags            : 0x%04X\n", bec->OPS.WarningFlags);

    fprintf(fp, "\n[UTRX]\n");
    fprintf(fp, "BootCause               : 0x%08X\n", bec->UTRX.BootCause);
    fprintf(fp, "BootCount               : %" PRIu16 "\n", bec->UTRX.BootCount);
    fprintf(fp, "BoardTemp               : %" PRId16 "\n", bec->UTRX.BoardTemp);
    fprintf(fp, "ActiveConf              : %" PRIu8 "\n", bec->UTRX.ActiveConf);

    fprintf(fp, "\n[EPS - PCDU P60 Dock]\n");
    fprintf(fp, "DockOutEnMask           : 0x%04X\n", bec->EPS.DockOutEnMask);
    fprintf(fp, "PduOutEnMask            : 0x%04X\n", bec->EPS.PduOutEnMask);
    fprintf(fp, "PduConvEnMask           : 0x%04X\n", bec->EPS.PduConvEnMask);
    fprintf(fp, "AcuCin                  : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRId16 " ", bec->EPS.AcuCin[i]);
    fprintf(fp, "\nAcuVin                  : ");
    for (int i = 0; i < 6; i++) fprintf(fp, "%" PRIu16 " ", bec->EPS.AcuVin[i]);
    fprintf(fp, "\nVbatt                   : %" PRIu16 "\n", bec->EPS.Vbatt);
    fprintf(fp, "Vcc                     : %" PRId16 "\n", bec->EPS.Vcc);
    fprintf(fp, "BattV                   : %" PRIu16 "\n", bec->EPS.BattV);
    fprintf(fp, "BattC                   : %" PRId16 "\n", bec->EPS.BattC);
    fprintf(fp, "BattChrg                : %" PRId16 "\n", bec->EPS.BattChrg);
    fprintf(fp, "BattDischrg             : %" PRId16 "\n", bec->EPS.BattDischrg);
    fprintf(fp, "BattTemp                : %" PRId16 "\n", bec->EPS.BattTemp);
    fprintf(fp, "DockTemp                : %" PRId16 "\n", bec->EPS.DockTemp);
    fprintf(fp, "DockBootCount           : %" PRIu16 "\n", bec->EPS.DockBootCount);
    fprintf(fp, "GndWdtCnt               : %" PRIu16 "\n", bec->EPS.GndWdtCnt);
    fprintf(fp, "CanWdtCnt               : %" PRIu16 "\n", bec->EPS.CanWdtCnt);
    fprintf(fp, "GndWdtLeft              : %" PRIu32 "\n", bec->EPS.GndWdtLeft);
    fprintf(fp, "CanWdtLeft              : %" PRIu32 "\n", bec->EPS.CanWdtLeft);
    fprintf(fp, "DockResetCause          : 0x%08X\n", bec->EPS.DockResetCause);

    fprintf(fp, "\n[UANT]\n");
    fprintf(fp, "DoorStates              : 0x%02X\n", bec->UANT.DoorStates);
    fprintf(fp, "FeedbackSwitches        : 0x%02X\n", bec->UANT.FeedbackSwitches);

    WriteBinaryDump(fp, bec, sizeof(*bec));
    fclose(fp);
    return 0;
}


static void DumpBeaconPacketToFile(FILE* fp, const csp_packet_t* packet)
{
    for (int i = 0; i < packet->length; i++) {
        if (!(i % 10) && i != 0) {
            printf("\n");
            if (fp) fprintf(fp, "\n");
        }
        printf("0x%x ", packet->data[i]);
        if (fp) fprintf(fp, "%02hhx\t", packet->data[i]);
    }
}
static void HandleBEE1012BeaconPacket(const csp_packet_t* packet, int dport,
                                       const struct timeval* rx_time = NULL)  /* [FTP-BEACON-BUFFER] rx_time */
{
    console.AddLog("Received BEE1012 Beacon from port : %d.", dport);

    std::string bcnpktfilename;
    FILE* bcn_fp = OpenTelemetryFile("../data/BEE1012/beacon/beacon_raw", "BEE1012BeaconPkt--", "--", rx_time, "wb", &bcnpktfilename);

    if (!bcn_fp) {
        console.AddLog("[ERROR] Failed to open BEE1012 beacon file: %s",
                       bcnpktfilename.c_str());
    }

    printf("\nBEE1012 Beacon Length: %u", packet->length);

    if (bcn_fp)
        DumpBeaconPacketToFile(bcn_fp, packet);

    if (packet->length == BEE1012_LEN_BEACON) {
        BEE1012_Beacon_t bcn;
        memcpy(&bcn, packet->data, BEE1012_LEN_BEACON);
        {
            std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);   // 화면 스레드가 읽는 버퍼
            *bee1012_beacon = bcn;
            g_have_bee1012_beacon = true;
        }

        BEE1012BeaconSaver(&bcn, rx_time);  /* [FTP-BEACON-BUFFER] */
    }
    else {
        console.AddLog(
            "Received BEE1012 Beacon length mismatch. expected=%zu actual=%u",
            (size_t)BEE1012_LEN_BEACON,
            packet->length);
    }

    if (bcn_fp)
        fclose(bcn_fp);
}


static void HandleUELYSYSBeaconPacket(const csp_packet_t* packet, int dport,
                                       const struct timeval* rx_time = NULL)  /* [FTP-BEACON-BUFFER] rx_time */
{
    console.AddLog("Received UELYSYS Beacon from port : %d.", dport);

    std::string bcnpktfilename;
    FILE* bcn_fp = OpenTelemetryFile("../data/UELYSYS/beacon/beacon_raw", "UELYSYSBeaconPkt--", "--", rx_time, "wb", &bcnpktfilename);

    if (!bcn_fp) {
        console.AddLog("[ERROR] Failed to open UELYSYS beacon file: %s",
                       bcnpktfilename.c_str());
    }

    printf("\nUELYSYS Beacon Length: %u", packet->length);

    if (bcn_fp)
        DumpBeaconPacketToFile(bcn_fp, packet);

    if (packet->length == UELYSYS_LEN_BEACON) {
        UELYSYS_Beacon_t bcn;
        memcpy(&bcn, packet->data, UELYSYS_LEN_BEACON);
        {
            std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);   // 화면 스레드가 읽는 버퍼
            *uelysys_beacon = bcn;
            g_have_uelysys_beacon = true;
        }

        UELYSYSBeaconSaver(&bcn, rx_time);  /* [FTP-BEACON-BUFFER] */
    }
    else {
        console.AddLog(
            "Received UELYSYS Beacon length mismatch. expected=%zu actual=%u",
            (size_t)UELYSYS_LEN_BEACON,
            packet->length);
    }

    if (bcn_fp)
        fclose(bcn_fp);
}


static void HandleSD1SATBeaconPacket(const csp_packet_t* packet, int dport,
                                       const struct timeval* rx_time = NULL)  /* [FTP-BEACON-BUFFER] rx_time */
{
    console.AddLog("Received SD1SAT Beacon from port : %d.", dport);

    std::string bcnpktfilename;
    FILE* bcn_fp = OpenTelemetryFile("../data/SD1SAT/beacon/beacon_raw", "SD1SATBeaconPkt--", "--", rx_time, "wb", &bcnpktfilename);

    if (!bcn_fp) {
        console.AddLog("[ERROR] Failed to open SD1SAT beacon file: %s",
                       bcnpktfilename.c_str());
    }

    printf("\nSD1SAT Beacon Length: %u", packet->length);

    if (bcn_fp)
        DumpBeaconPacketToFile(bcn_fp, packet);

    if (packet->length == SD1SAT_LEN_BEACON) {
        SD1SAT_Beacon_t bcn;
        memcpy(&bcn, packet->data, SD1SAT_LEN_BEACON);
        {
            std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);   // 화면 스레드가 읽는 버퍼
            *sd1sat_beacon = bcn;
            g_have_sd1sat_beacon = true;
        }

        SD1SATBeaconSaver(&bcn, rx_time);  /* [FTP-BEACON-BUFFER] */
    }
    else {
        console.AddLog(
            "Received SD1SAT Beacon length mismatch. expected=%zu actual=%u",
            (size_t)SD1SAT_LEN_BEACON,
            packet->length);
    }

    if (bcn_fp)
        fclose(bcn_fp);
}


/*
 * [FTP-BEACON-BUFFER] task_downlink_onorbit 의 case 17 / 12 / 22 블록을 옮긴 것.
 * 일반 비콘이면 Handle*BeaconPacket(), 길이가 다르면 미션비콘/깨진 패킷으로 저장한다.
 * rx_time 이 NULL 이면 현재 시각(평소 수신), 아니면 그 시각으로 파일명을 만든다
 * (FTP 중 버퍼에 쌓아 둔 비콘을 FTP 후 저장할 때).
 */
void ProcessBEE1012BeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time)
{
    if (packet->length == BEE1012_LEN_BEACON) {
        HandleBEE1012BeaconPacket(packet, dport, rx_time);
    }
    else {
        console.AddLog("Received BEE1012 Mission Beacon from port: %d.", dport);

        FILE *unk_fp = OpenTelemetryFile("../data/BEE1012/missionbeacon", "msnbcn--", "--", rx_time, "wb");

        printf("Unknown Packet Length: %u\n", packet->length);
        printf("===== UNKNOWN PACKET DUMP =====\n");

        for (int i = 0; i < packet->length; i++) {
            if (!(i % 10) && i != 0)
                printf("\n");

            printf("0x%02X ", packet->data[i]);

            if (unk_fp) {
                if (!(i % 10) && i != 0)
                    fprintf(unk_fp, "\n");

                fprintf(unk_fp, "%02hhx\t", packet->data[i]);
            }
        }

        printf("\n===============================\n");

        if (unk_fp)
            fclose(unk_fp);
    }
}

void ProcessUELYSYSBeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time)
{
    if (packet->length == UELYSYS_LEN_BEACON) {
        HandleUELYSYSBeaconPacket(packet, dport, rx_time);
    }
    else {
        console.AddLog("Received UELYSYS Mission Beacon from port: %d.", dport);

        FILE *unk_fp = OpenTelemetryFile("../data/UELYSYS/missionbeacon", "msnbcn--", "--", rx_time, "wb");

        printf("Unknown Packet Length: %u\n", packet->length);
        printf("===== UNKNOWN PACKET DUMP =====\n");

        for (int i = 0; i < packet->length; i++) {
            if (!(i % 10) && i != 0)
                printf("\n");

            printf("0x%02X ", packet->data[i]);

            if (unk_fp) {
                if (!(i % 10) && i != 0)
                    fprintf(unk_fp, "\n");

                fprintf(unk_fp, "%02hhx\t", packet->data[i]);
            }
        }

        printf("\n===============================\n");

        if (unk_fp)
            fclose(unk_fp);
    }
}

void ProcessSD1SATBeaconPort(const csp_packet_t* packet, int dport, const struct timeval* rx_time)
{
    if (packet->length == SD1SAT_LEN_BEACON) {
        HandleSD1SATBeaconPacket(packet, dport, rx_time);
    }
    else {
        console.AddLog("Received SD1SAT Beacon but broken. expected=%zu actual=%u",
                        (size_t)SD1SAT_LEN_BEACON, packet->length);

        FILE *unk_fp = OpenTelemetryFile("../data/SD1SAT/beacon/beacon_unknown", "bcnpkt--", "--", rx_time, "wb");

        printf("Unknown Packet Length: %u\n", packet->length);
        printf("===== UNKNOWN PACKET DUMP =====\n");

        for (int i = 0; i < packet->length; i++) {
            if (!(i % 10) && i != 0)
                printf("\n");

            printf("0x%02X ", packet->data[i]);

            if (unk_fp) {
                if (!(i % 10) && i != 0)
                    fprintf(unk_fp, "\n");

                fprintf(unk_fp, "%02hhx\t", packet->data[i]);
            }
        }

        printf("\n===============================\n");

        if (unk_fp)
            fclose(unk_fp);
    }
}


/*
 * [FTP-BEACON-CLOSE] FTP 중에는 task_downlink_onorbit 가 멈춰 있어 DLsocket 을
 * accept 하는 곳이 없다. 그대로 두면 비콘 연결이 CSP 연결 슬롯(CSP_CONN_MAX=10)을
 * 하나씩 차지해 "No more free connections" 로 이어진다. 그래서 FTP 중에는
 * 들어온 연결을 저장/처리 없이 패킷만 해제하고 바로 닫는다.
 * 비콘 포트는 수신 사실만 콘솔에 남긴다 (파일 저장 없음).
 */
static const char * FtpBeaconPortName(int dport)
{
    switch (dport) {
        case 31: return "BEE1000";
        case 17: return "BEE1012";
        case 12: return "UELYSYS";
        case 22: return "SD1SAT";
        default: return NULL;
    }
}

/*
 * [FTP-BEACON-BUFFER] FTP 중 들어온 BEE1012 / UELYSYS / SD1SAT 비콘(포트 17/12/22)은
 * 버리지 않고 위성별 map 에 [수신 시각(us) -> 원본 바이트] 로 쌓아 두었다가,
 * FTP 가 끝나면 FlushFtpBeaconBuffers() 가 평소와 같은 저장 함수로 파일을 만들고 비운다.
 * 쌓기와 저장 모두 TRxController 스레드에서만 일어나므로 lock 이 필요 없다.
 * BEE1000(포트 31)과 그 외 포트는 지금처럼 버린다.
 */

typedef std::map<int64_t, std::vector<uint8_t>> FtpBeaconBuffer;
static FtpBeaconBuffer g_ftp_bcn_bee1012;   /* port 17 */
static FtpBeaconBuffer g_ftp_bcn_uelysys;   /* port 12 */
static FtpBeaconBuffer g_ftp_bcn_sd1sat;    /* port 22 */
static const size_t FTP_BEACON_BUFFER_MAX = 5000;   /* 위성당 최대 개수, 넘으면 오래된 것부터 버림 */

static FtpBeaconBuffer * FtpBeaconBufferForPort(int dport)
{
    switch (dport) {
        case 17: return &g_ftp_bcn_bee1012;
        case 12: return &g_ftp_bcn_uelysys;
        case 22: return &g_ftp_bcn_sd1sat;
        default: return NULL;
    }
}

static void BufferFtpBeacon(FtpBeaconBuffer * buf, const csp_packet_t * packet)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t key = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
    while (buf->count(key))   /* 같은 us 에 두 개가 오면 1us 씩 밀어서 둘 다 보관 */
        key++;

    if (buf->size() >= FTP_BEACON_BUFFER_MAX)
        buf->erase(buf->begin());
    (*buf)[key] = std::vector<uint8_t>(packet->data, packet->data + packet->length);
}

static size_t FlushOneFtpBeaconBuffer(FtpBeaconBuffer * buf,
                                      void (*process)(const csp_packet_t*, int, const struct timeval*),
                                      int dport)
{
    size_t saved = 0;
    for (const auto & entry : *buf) {
        const std::vector<uint8_t> & bytes = entry.second;
        csp_packet_t * packet = (csp_packet_t *)csp_buffer_get(bytes.size());
        if (packet == NULL) {
            console.AddLog("[ERROR]##FTP beacon buffer: no CSP buffer, %u beacon(s) of port %d dropped.",
                           (unsigned)(buf->size() - saved), dport);
            break;
        }
        memcpy(packet->data, bytes.data(), bytes.size());
        packet->length = (uint16_t)bytes.size();

        struct timeval rx_time;
        rx_time.tv_sec  = (time_t)(entry.first / 1000000);
        rx_time.tv_usec = (suseconds_t)(entry.first % 1000000);
        process(packet, dport, &rx_time);

        csp_buffer_free(packet);
        saved++;
    }
    buf->clear();
    return saved;
}

void FlushFtpBeaconBuffers(void)
{
    if (g_ftp_bcn_bee1012.empty() && g_ftp_bcn_uelysys.empty() && g_ftp_bcn_sd1sat.empty())
        return;

    size_t n_bee1012 = FlushOneFtpBeaconBuffer(&g_ftp_bcn_bee1012, ProcessBEE1012BeaconPort, 17);
    size_t n_uelysys = FlushOneFtpBeaconBuffer(&g_ftp_bcn_uelysys, ProcessUELYSYSBeaconPort, 12);
    size_t n_sd1sat  = FlushOneFtpBeaconBuffer(&g_ftp_bcn_sd1sat,  ProcessSD1SATBeaconPort,  22);
    console.AddLog("[OK]##Saved beacons buffered during FTP. BEE1012=%u UELYSYS=%u SD1SAT=%u",
                   (unsigned)n_bee1012, (unsigned)n_uelysys, (unsigned)n_sd1sat);
}

void CloseDownlinkConnDuringFtp(csp_socket_t * sock)
{
    csp_conn_t * conn = csp_accept(sock, 100);
    if (conn == NULL)
        return;

    const int dport = csp_conn_dport(conn);
    const char * beacon_name = FtpBeaconPortName(dport);
    FtpBeaconBuffer * buf = FtpBeaconBufferForPort(dport);   /* [FTP-BEACON-BUFFER] */
    csp_packet_t * packet;
    while ((packet = csp_read(conn, 0)) != NULL) {
        if (buf) {   /* [FTP-BEACON-BUFFER] */
            BufferFtpBeacon(buf, packet);
            console.AddLog("Received %s Beacon from port : %d. (FTP in progress, buffered)",
                           beacon_name, dport);
        }
        else if (beacon_name)
            console.AddLog("Received %s Beacon from port : %d. (FTP in progress, not saved)",
                           beacon_name, dport);
        csp_buffer_free(packet);
    }
    csp_close(conn);
}


bool CopyBEE1012Beacon(BEE1012_Beacon_t & out)
{
    std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);
    out = *bee1012_beacon;
    return g_have_bee1012_beacon;
}

bool CopyUELYSYSBeacon(UELYSYS_Beacon_t & out)
{
    std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);
    out = *uelysys_beacon;
    return g_have_uelysys_beacon;
}

bool CopySD1SATBeacon(SD1SAT_Beacon_t & out)
{
    std::lock_guard<std::mutex> lk(g_n5_beacon_mtx);
    out = *sd1sat_beacon;
    return g_have_sd1sat_beacon;
}
