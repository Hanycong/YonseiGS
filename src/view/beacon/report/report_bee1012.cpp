// View - Beacon 창 "Report" 탭: BEE1012 앱 리포트의 ReturnValue 해석
//
// GS_nuri5 miman_imgui.cpp Report 탭의 BEE1012 LTRX/KISSCAM/PAY_SLT/THRUST, PAYYONSEI, SDR_CONTROLLER 부분 를 옮긴 것 (행 이름/형식 그대로).
// report_app.cpp 의 DrawAppReport 가 머리(Type, Parsed payload size)를 그린 뒤 부른다. 처리한 종류면 true.
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawBee1012AppBody(ReportCtx & c)
{
    ReportView_t & view = c.view;
    const EPS_Report_State_t & eps_state = c.eps_state;
    const EPS_RParam_Reassembly_State_t & eps_rparam = c.eps_rparam;
    bool & show_hexdump = c.show_hexdump;
    (void)eps_state; (void)eps_rparam; (void)show_hexdump;
    auto RowEPSHKAllSummary = [&]() { rpt5::RowEPSHKAllSummary(eps_state); };
    (void)RowEPSHKAllSummary;
    const uint8_t *p = view.raw_bytes;
    const uint16_t n = view.raw_size;
    auto CopyLocal = [&](void *dst, size_t size) {
        if (n < size) return false;
        memcpy(dst, p, size);
        return true;
    };
    auto CopyExactLocal = [&](void *dst, size_t size) {
        if (n != size) return false;
        memcpy(dst, p, size);
        return true;
    };
    (void)CopyLocal; (void)CopyExactLocal;

    if (view.kind == REPORT_KIND_BEE1012_LTRX)
    {
        if ((view.reflected_cc == LTRX_NOOP_CC) && n == sizeof(LTRX_HkSnapshot_Report_t)) {
            LTRX_HkSnapshot_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("cmd_counter", "%" PRIu8, x.cmd_counter);
            RowFmt("cmd_err_counter", "%" PRIu8, x.cmd_err_counter);
            RowFmt("have_gnss", "%" PRIu8, x.have_gnss);
            if (x.have_gnss) {
                RowFmt("gnss.utc_time_ms", "%" PRIu32, x.last_gnss.utc_time_ms);
                RowFmt("gnss.latitude_1e7deg", "%" PRId32, x.last_gnss.latitude);
                RowFmt("gnss.longitude_1e7deg", "%" PRId32, x.last_gnss.longitude);
                RowFmt("gnss.altitude_0p1m", "%" PRIu32, x.last_gnss.altitude);
                RowFmt("gnss.fix_quality", "%" PRIu8, x.last_gnss.fix_quality);
                RowFmt("gnss.num_satellites", "%" PRIu8, x.last_gnss.num_satellites);
            }
            RowFmt("have_beacon_status", "%" PRIu8, x.have_beacon_status);
            if (x.have_beacon_status) {
                RowFmt("beacon.temperature_0p1c", "%" PRId16, x.last_beacon_status.temperature);
                RowFmt("beacon.connection_quality", "%" PRIu8, x.last_beacon_status.connection_quality);
                RowFmt("beacon.battery_capacity_0p1pct", "%" PRIu16, x.last_beacon_status.battery_capacity);
            }
            RowFmt("downstream_enabled", "%" PRIu8, x.downstream_enabled);
            RowFmt("bus_beacon_period", "%" PRIu16, x.bus_beacon_period);
            RowFmt("bus_beacon_count", "%" PRIu16, x.bus_beacon_count);
        } else if (view.reflected_cc == LTRX_NOOP_CC && n == 14) {
            char text[15]{}; memcpy(text, p, 14);
            RowText("noop_echo", text);
        } else if (view.reflected_cc == LTRX_SET_BUS_BEACON_PERIOD_CC && n == sizeof(uint16_t)) {
            uint16_t period = 0; memcpy(&period, p, sizeof(period));
            RowFmt("period_sec", "%" PRIu16, period);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_BEE1012_KISSCAM)
    {
        if ((view.reflected_cc == PAYBEE_KISSCAM_NOOP_CC ||
             view.reflected_cc == PAYBEE_KISSCAM_RESET_COUNTERS_CC) && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if ((view.reflected_cc == PAYBEE_KISSCAM_DOWNLOAD_CC ||
                    view.reflected_cc == PAYBEE_KISSCAM_DOWNLOAD_ALL_CC) && n >= 1) {
            RowFmt("phase", "%" PRIu8 " (0=STARTED, 1=FINISHED)", p[0]);
            if (n > 1) RowArr_Hex("trailing_bytes", p + 1, n - 1, 16);
        } else {
            RowText("Payload", n ? "Raw camera-protocol response; see full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_BEE1012_PAY_SLT)
    {
        if (view.reflected_cc == BEE1012_PAY_SLT_NOOP_CC && n == 32) {
            RowFmt("message", "%.*s", 31, (const char *)p);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_REPORT_BCN_CC && n == 1) {
            RowFmt("bcn_enabled", "%" PRIu8, p[0]);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_OUTPUT_ENABLED_CC && n == sizeof(PAY_SLT_OutputEnabled_Report_t)) {
            PAY_SLT_OutputEnabled_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("hk_enabled", "%" PRIu8, x.hk_enabled);
            RowFmt("bcn_enabled", "%" PRIu8, x.bcn_enabled);
            RowFmt("ifb_enabled", "%" PRIu8, x.ifb_enabled);
            RowFmt("exp_enabled", "%" PRIu8, x.exp_enabled);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_PAR_GET_CC && n >= sizeof(PAY_SLT_ParamHeader_Report_t)) {
            PAY_SLT_ParamHeader_Report_t h{}; memcpy(&h, p, sizeof(h));
            RowFmt("node", "%" PRIu8, h.node);
            RowFmt("table", "%" PRIu8, h.table);
            RowFmt("addr", "0x%04" PRIX16, h.addr);
            RowFmt("type", "%" PRIu8, h.type);
            RowFmt("count", "%" PRIu8, h.len);
            const uint8_t *values = p + sizeof(h);
            size_t element_size = 0;
            if (h.type == 0 || h.type == 4 || h.type == 14) element_size = 1;
            else if (h.type == 1 || h.type == 5) element_size = 2;
            else if (h.type == 2 || h.type == 13) element_size = 4;
            const uint8_t value_count = element_size == 0 ? 0 :
                (uint8_t)std::min<size_t>(h.len, (n - sizeof(h)) / element_size);
            for (uint8_t i = 0; i < value_count; ++i) {
                char label[32]; snprintf(label, sizeof(label), "value[%u]", (unsigned)i);
                if (h.type == 0) RowFmt(label, "%" PRIu8, values[i]);
                else if (h.type == 4) RowFmt(label, "%" PRId8, (int8_t)values[i]);
                else if (h.type == 1 || h.type == 5) {
                    uint16_t value = 0; memcpy(&value, values + 2U * i, sizeof(value));
                    if (h.type == 1) RowFmt(label, "%" PRIu16, value);
                    else RowFmt(label, "%" PRId16, (int16_t)value);
                } else if (h.type == 2 || h.type == 13) {
                    uint32_t bits = 0; memcpy(&bits, values + 4U * i, sizeof(bits));
                    if (h.type == 2) RowFmt(label, "%" PRIu32, bits);
                    else { float value = 0.0f; memcpy(&value, &bits, sizeof(value)); RowFmt(label, "%g", (double)value); }
                }
            }
            if (h.type == 14) RowFmt("string", "%.*s", (int)value_count, (const char *)values);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_PAR_SET_CC && n == 104) {
            uint16_t addr = 0; memcpy(&addr, p + 2, sizeof(addr));
            RowFmt("rejected_node", "%" PRIu8, p[0]);
            RowFmt("rejected_table", "%" PRIu8, p[1]);
            RowFmt("addr", "0x%04" PRIX16, addr);
            RowFmt("type", "%" PRIu8, p[4]);
            if (p[4] == 14) RowFmt("string", "%.*s", 96, (const char *)(p + 8));
            else { uint32_t value = 0; memcpy(&value, p + 8, sizeof(value)); RowFmt("value", "%" PRIu32, value); }
        } else if (view.reflected_cc == BEE1012_PAY_SLT_SCAN_FILES_CC && n == sizeof(PAY_SLT_ScanFiles_Report_t)) {
            PAY_SLT_ScanFiles_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("snap_id", "%" PRIu32, x.snap_id);
            RowFmt("file_count", "%" PRIu32, x.file_count);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_SCAN_FILES_CC && n == 1) {
            RowFmt("rejected_node", "%" PRIu8, p[0]);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_RS422_PING_CC && n == 10) {
            RowArr_Hex("rs422_rx", p, n, 16);
        } else if ((view.reflected_cc == BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC ||
                    view.reflected_cc == BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC) &&
                   n == sizeof(PAY_SLT_DownloadRequest_Report_t)) {
            PAY_SLT_DownloadRequest_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("file_index", "%" PRIu32, x.file_index);
            RowFmt("start_chunk", "%" PRIu32, x.start_chunk);
        } else if ((view.reflected_cc == BEE1012_PAY_SLT_DOWNLOAD_FILE_I2C_CC ||
                    view.reflected_cc == BEE1012_PAY_SLT_DOWNLOAD_FILE_RS422_CC) &&
                   n == sizeof(PAY_SLT_DownloadResult_Report_t)) {
            PAY_SLT_DownloadResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            x.file_path[sizeof(x.file_path) - 1] = '\0';
            RowFmt("file_path", "%.*s", (int)sizeof(x.file_path), x.file_path);
            RowFmt("expected_file_size", "%" PRIu32, x.expected_file_size);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_GET_FULL_TABLE_CC && view.ret_type == 0) {
            RowFmt("table_data_size", "%" PRIu16, n);
            if (n) RowArr_Hex("table_data", p, n, 16);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_GET_FULL_TABLE_CC && n == 2) {
            RowFmt("rejected_node", "%" PRIu8, p[0]);
            RowFmt("rejected_table", "%" PRIu8, p[1]);
        } else if (view.reflected_cc == BEE1012_PAY_SLT_GET_FULL_TABLE_CC && n == 4) {
            int32_t error = 0; memcpy(&error, p, sizeof(error));
            RowFmt("error_or_required_size", "%" PRId32, error);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_BEE1012_THRUST)
    {
        if (n == sizeof(THRUST_DefaultResponse_Report_t) &&
            view.reflected_cc != BEE1012_THRUST_REQ_STATUS_CC) {
            THRUST_DefaultResponse_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("result_code", "%" PRIu16 " (0=ACK, 1=NACK)", x.result_code);
            RowFmt("current_status", "0x%04" PRIX16, x.current_status);
            RowFmt("fault_flags", "0x%04" PRIX16, x.fault_flags);
            RowFmt("interlock_flags", "0x%04" PRIX16, x.interlock_flags);
        } else if (view.reflected_cc == BEE1012_THRUST_REQ_STATUS_CC && n == sizeof(THRUST_StatusData_Report_t)) {
            THRUST_StatusData_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("current_mode", "%" PRIu32, x.current_mode);
            RowFmt("last_msg_id", "%" PRIu8, x.last_msg_id);
            RowFmt("last_result", "%" PRIu8 " (0=ACK, 1=NACK)", x.last_result);
            RowFmt("result_code", "%" PRIu16, x.result_code);
            RowFmt("current_status", "0x%04" PRIX16, x.current_status);
            RowFmt("fault_flags", "0x%04" PRIX16, x.fault_flags);
            RowFmt("interlock_flags", "0x%04" PRIX16, x.interlock_flags);
        } else if (view.reflected_cc == BEE1012_THRUST_REQ_HK_CC && n == sizeof(THRUST_HkData_Report_t)) {
            THRUST_HkData_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("timestamp_ms", "%" PRIu32, x.timestamp_ms);
            RowFmt("pressure_ch0_bar", "%f", x.pressure_ch0);
            RowFmt("pressure_ch1_bar", "%f", x.pressure_ch1);
            RowFmt("pressure_ch2_bar", "%f", x.pressure_ch2);
            RowFmt("pressure_ch3_bar", "%f", x.pressure_ch3);
            RowFmt("temp_ch0_c", "%f", x.temp_ch0);
            RowFmt("temp_ch1_c", "%f", x.temp_ch1);
            RowFmt("temp_ch2_c", "%f", x.temp_ch2);
            RowFmt("temp_ch3_c", "%f", x.temp_ch3);
            RowFmt("temp_ch4_c", "%f", x.temp_ch4);
            RowFmt("temp_ch5_c", "%f", x.temp_ch5);
            RowFmt("status", "0x%08" PRIX32, x.status);
        } else if (view.reflected_cc == BEE1012_THRUST_REQ_FAULT_LOG_CC && n == 40) {
            RowArr_Hex("fault_log", p, n, 16);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_PAYYONSEI_TEMP)
    {
        auto DualTempRow = [&](const char *tag, const TEMP_SensorTemp_Report_t &s) {
            RowFmt(tag, "addr=0x%02X status=%" PRId32 " temp_c=%f alert=0x%02X",
                   s.addr, s.status, s.temperature_c, s.alert_flags);
        };
        auto DualConfigRow = [&](const char *tag, const TEMP_SensorConfig_Report_t &s) {
            RowFmt(tag, "addr=0x%02X status=%" PRId32 " config=0x%04X", s.addr, s.status, s.config_val);
        };
        auto DualLimitsRow = [&](const char *tag, const TEMP_SensorLimits_Report_t &s) {
            RowFmt(tag, "addr=0x%02X status=%" PRId32 " upper=0x%04X lower=0x%04X crit=0x%04X",
                   s.addr, s.status, s.upper_limit, s.lower_limit, s.crit_limit);
        };
        auto DualIdRow = [&](const char *tag, const TEMP_SensorId_Report_t &s) {
            RowFmt(tag, "addr=0x%02X status=%" PRId32 " mfg=0x%04X dev=0x%04X", s.addr, s.status, s.mfg_id, s.dev_id);
        };
        if ((view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_NOOP_CC ||
             view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_RESET_COUNTERS_CC) && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_READ_TEMP_CC && n == sizeof(TEMP_DualTemp_Report_t)) {
            TEMP_DualTemp_Report_t x{}; memcpy(&x, p, sizeof(x));
            DualTempRow("sensor_pobc", x.sensor[0]); DualTempRow("sensor_sdrc", x.sensor[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_READ_CONFIG_CC && n == sizeof(TEMP_DualConfig_Report_t)) {
            TEMP_DualConfig_Report_t x{}; memcpy(&x, p, sizeof(x));
            DualConfigRow("sensor_pobc", x.sensor[0]); DualConfigRow("sensor_sdrc", x.sensor[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_READ_LIMITS_CC && n == sizeof(TEMP_DualLimits_Report_t)) {
            TEMP_DualLimits_Report_t x{}; memcpy(&x, p, sizeof(x));
            DualLimitsRow("sensor_pobc", x.sensor[0]); DualLimitsRow("sensor_sdrc", x.sensor[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_READ_ID_CC && n == sizeof(TEMP_DualId_Report_t)) {
            TEMP_DualId_Report_t x{}; memcpy(&x, p, sizeof(x));
            DualIdRow("sensor_pobc", x.sensor[0]); DualIdRow("sensor_sdrc", x.sensor[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_TEMP_START_LOG_CC && n == sizeof(TEMP_LogStart_Report_t)) {
            TEMP_LogStart_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("started", "%" PRIu8, x.started);
            RowFmt("active", "%" PRIu8, x.active);
            RowFmt("interval_ms", "%" PRIu16, x.interval_ms);
            RowFmt("duration_ms", "%" PRIu32, x.duration_ms);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_PAYYONSEI_IMU)
    {
        if (view.reflected_cc == UELYSYS_PAYYONSEI_IMU_RUN_SELFTEST_CC && n == sizeof(IMU_SelftestResult_Report_t)) {
            IMU_SelftestResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("return_code", "%" PRId32, x.return_code);
            RowFmt("raw_bitmask", "0x%04" PRIX16, x.raw_bitmask);
            RowFmt("acc_pass", "x=%" PRIu8 " y=%" PRIu8 " z=%" PRIu8, x.acc_x_pass, x.acc_y_pass, x.acc_z_pass);
            RowFmt("gyr_pass", "x=%" PRIu8 " y=%" PRIu8 " z=%" PRIu8, x.gyr_x_pass, x.gyr_y_pass, x.gyr_z_pass);
            RowFmt("mag_pass", "x=%" PRIu8 " y=%" PRIu8 " z=%" PRIu8, x.mag_x_pass, x.mag_y_pass, x.mag_z_pass);
            RowFmt("cmd_counter", "%" PRIu8, x.cmd_counter);
            RowFmt("err_counter", "%" PRIu8, x.err_counter);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IMU_SAVE_LOG_FILE_CC && n == sizeof(IMU_CommandResult_Report_t)) {
            IMU_CommandResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("return_code", "%" PRId32, x.return_code);
            RowFmt("imu_state", "%" PRIu8, x.imu_state);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IMU_SAVE_LOG_FILE_CC && n > 0) {
            char text[65]{}; uint16_t cap = (n > 64) ? 64 : n; memcpy(text, p, cap);
            RowText("saved_file_path", text);
        } else if (n == sizeof(IMU_CommandResult_Report_t)) {
            IMU_CommandResult_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("return_code", "%" PRId32, x.return_code);
            RowFmt("last_measurement_status", "%" PRId32, x.last_measurement_status);
            RowFmt("status_word", "0x%08" PRIX32, x.status_word);
            RowFmt("cmd_counter", "%" PRIu8, x.cmd_counter);
            RowFmt("err_counter", "%" PRIu8, x.err_counter);
            RowFmt("imu_state", "%" PRIu8, x.imu_state);
            RowFmt("operating_mode", "%" PRIu8, x.operating_mode);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_PAYYONSEI_IWSB)
    {
        if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_NOOP_CC && n == 4) {
            uint16_t counters[2]{}; memcpy(counters, p, sizeof(counters));
            RowFmt("cmd_counter", "%" PRIu16, counters[0]);
            RowFmt("err_counter", "%" PRIu16, counters[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_MISSION_START_CC && n == 1) {
            RowFmt("state", "%" PRIu8, p[0]);
        } else if ((view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_ERASE_SLOT_CC ||
                    view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_VERIFY_SLOT_CC) && n == 1) {
            RowFmt("slot", "%" PRIu8, p[0]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_UPDATE_FW_CC && n == 1) {
            RowFmt("target", "%" PRIu8, p[0]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_WRITE_FW_CC && n == sizeof(IWSB_WriteFirmware_Report_t)) {
            IWSB_WriteFirmware_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("bytes_sent", "%" PRIu32, x.bytes_sent);
            RowFmt("packets_sent", "%" PRIu32, x.packets_sent);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_GET_CURRENT_FW_CC && n == 1) {
            RowFmt("firmware_version", "%" PRIu8, p[0]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_IWSB_GET_THERMAL_CC && n == sizeof(IWSB_Thermal_Report_t)) {
            IWSB_Thermal_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("temperature_c", "%f", x.temperature_c);
            RowFmt("voltage_mv", "%" PRIu16, x.voltage_mv);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_PAYYONSEI_I2C)
    {
        if ((view.reflected_cc == UELYSYS_PAYYONSEI_I2C_NOOP_CC ||
             view.reflected_cc == UELYSYS_PAYYONSEI_I2C_RESET_COUNTERS_CC) && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_I2C_SET_THRESHOLD_CC && n == sizeof(I2C_ThresholdReport_Report_t)) {
            I2C_ThresholdReport_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("requested_latency_threshold_ns", "%" PRIu32, x.requested_latency_threshold_ns);
            RowFmt("active_latency_threshold_ns", "%" PRIu32, x.active_latency_threshold_ns);
            RowFmt("requested_over_limit", "%" PRIu8, x.requested_over_limit);
            RowFmt("requested_fail_limit", "%" PRIu8, x.requested_fail_limit);
            RowFmt("active_over_limit", "%" PRIu8, x.active_over_limit);
            RowFmt("active_fail_limit", "%" PRIu8, x.active_fail_limit);
        } else if (view.reflected_cc == UELYSYS_PAYYONSEI_I2C_CLEAR_LATCH_CC && n == sizeof(I2C_ClearLatchReport_Report_t)) {
            I2C_ClearLatchReport_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("previous_first_fault_time_sec", "%" PRIu32, x.previous_first_fault_time_sec);
            RowFmt("previous_worst_health", "%" PRIu8, x.previous_worst_health);
            RowFmt("health", "%" PRIu8, x.health);
            RowFmt("worst_health", "%" PRIu8, x.worst_health);
            RowFmt("fault_latched", "%" PRIu8, x.fault_latched);
        } else {
            RowText("Payload", n ? "See full raw bytes below" : "No return payload");
        }
    }
    else if (view.kind == REPORT_KIND_SDR_CONTROLLER)
    {
        if (view.reflected_cc == UELYSYS_SDR_CONTROLLER_RESET_COUNTERS_CC && n == 2) {
            RowFmt("cmd_counter", "%" PRIu8, p[0]);
            RowFmt("err_counter", "%" PRIu8, p[1]);
        } else if (view.reflected_cc == UELYSYS_SDR_CONTROLLER_SET_PHASE_SHIFT_CC && n == sizeof(SDRC_SetPhaseShift_Report_t)) {
            SDRC_SetPhaseShift_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("channel", "%" PRIu8, x.channel);
            RowFmt("mode", "%" PRIu8, x.mode);
            RowFmt("flag", "0x%02" PRIX8, x.flag);
        } else if (view.reflected_cc == UELYSYS_SDR_CONTROLLER_PSDR_GET_SLOT_INFO_CC && n == sizeof(SDRC_SlotInfo_Report_t)) {
            SDRC_SlotInfo_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("ack", "0x%02" PRIX8, x.ack);
            RowFmt("slot_fw_version", "%" PRIu32, x.info);
        } else if (view.reflected_cc == UELYSYS_SDR_CONTROLLER_PSDR_GET_CUR_FW_CC && n == sizeof(SDRC_FwVersion_Report_t)) {
            SDRC_FwVersion_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("ack", "0x%02" PRIX8, x.ack);
            RowFmt("fw_version", "%" PRIu8, x.version);
        } else if (view.reflected_cc == UELYSYS_SDR_CONTROLLER_PSDR_GET_THERMAL_CC && n == sizeof(SDRC_Thermal_Report_t)) {
            SDRC_Thermal_Report_t x{}; memcpy(&x, p, sizeof(x));
            RowFmt("ack", "0x%02" PRIX8, x.ack);
            RowFmt("thermal_c", "%f", x.thermal);
            RowFmt("voltage_mv", "%" PRIu16, x.voltage);
        } else if (n == 1) {
            RowFmt("ack_or_status_byte", "0x%02" PRIX8, p[0]);
        } else if (n == 4) {
            int32_t value = 0; memcpy(&value, p, sizeof(value));
            RowFmt("result", "%" PRId32, value);
        } else {
            RowText("Payload", n ? "Version string / raw device echo; see full raw bytes below" : "No return payload");
        }
    }
    else
        return false;
    return true;
}
