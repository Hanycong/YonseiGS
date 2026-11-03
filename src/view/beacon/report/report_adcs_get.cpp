// View - Beacon 창 "Report" 탭: ADCS GET 계열 리포트 (5차 위성 형식)
//
// GS_nuri5 miman_imgui.cpp Report 탭의 ADCS_GET_ERROR_LOG_SETTING ~ ADCS_GET_PORTMAP, ADCS_HK_REPORT ~ ADCS_GET_RAW_CALIBRATED_RWL case 를 옮긴 것 (행 이름/형식 그대로).
// BEE-1000 모드는 쓰지 않는다 (BEE-1000 의 ADCS 설정 리포트는 report_adcs.cpp 형식 그대로).
// LOG_MASK / UNSOLICIT_TLM_SETUP / RAW GYR·RW 센서는 report_adcs.cpp 를 같이 쓴다.
// 행 도우미(RowFmt, HeaderRow, ...)는 report_rows.h.
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"
#include "report_rows.h"

using namespace rpt5;

bool DrawAdcsGetReport(ReportCtx & c)
{
    ReportView_t & view = c.view;
    const EPS_Report_State_t & eps_state = c.eps_state;
    const EPS_RParam_Reassembly_State_t & eps_rparam = c.eps_rparam;
    bool & show_hexdump = c.show_hexdump;
    (void)eps_state; (void)eps_rparam; (void)show_hexdump;
    auto RowEPSHKAllSummary = [&]() { rpt5::RowEPSHKAllSummary(eps_state); };
    (void)RowEPSHKAllSummary;

    switch (view.kind)
    {
    case REPORT_KIND_ADCS_GET_ERROR_LOG_SETTING:
    {
        auto &pl = view.u.adcs_errorlogsetting;

        ImGui::TextUnformatted("ADCS Error Log Setting");

        ImGui::Separator();
        ImGui::Text("[Settings]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Active State");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.ActiveState);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Buffer Full Action");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.BufferFullAction);   // GS_nuri5 는 "%u (%s)" 에 문자열 인자가 없어 죽었다

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Reserved");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.Reserved);

        break;
    }

    case REPORT_KIND_ADCS_GET_CURRENT_UNIX_TIME:
    {
        auto &pl = view.u.adcs_currentunixtime;

        ImGui::TextUnformatted("ADCS Current Unix Time");

        ImGui::Separator();
        ImGui::Text("[Time]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Current Unix Seconds [s]");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CurrentUnixseconds);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Current Unix Nanoseconds [ns]");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CurrentUnixNanoseconds);

        // UTC 변환 표시
        time_t t = static_cast<time_t>(pl.CurrentUnixseconds);
        struct tm tm_utc;
        gmtime_r(&t, &tm_utc);
        char utc_str[64];
        strftime(utc_str, sizeof(utc_str), "%Y-%m-%d %H:%M:%S UTC", &tm_utc);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UTC Time");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s.%09u", utc_str, pl.CurrentUnixNanoseconds);

        break;
    }

    case REPORT_KIND_ADCS_GET_PERSIST_CONFIG_DIAGNOSTIC:
    {
        auto &pl = view.u.adcs_persistconfigdiagnostic;

        ImGui::TextUnformatted("ADCS Persist Config Diagnostic");

        ImGui::Separator();
        ImGui::Text("[Diagnostic]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("State");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.State);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Last Result");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.LastResult);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Timestamp [s]");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.Timestamp);

        break;
    }

    case REPORT_KIND_ADCS_GET_COMMUNICATION_STATUS:
    {
        auto &pl = view.u.adcs_communicationstatus;

        ImGui::TextUnformatted("ADCS Communication Status");

        ImGui::Separator();
        ImGui::Text("[UART]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART TC Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART_TcCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART TLM Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART_TlmCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART SW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART_ErrSW);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART HW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART_ErrHW);

        ImGui::Separator();
        ImGui::Text("[UART2]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART2 TC Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART2_TcCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART2 TLM Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART2_TlmCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART2 SW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART2_ErrSW);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART2 HW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART2_ErrHW);

        ImGui::Separator();
        ImGui::Text("[CAN]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("CAN TC Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CAN_TcCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("CAN TLM Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CAN_TlmCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("CAN SW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CAN_ErrSW);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("CAN HW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CAN_ErrHW);

        ImGui::Separator();
        ImGui::Text("[I2C]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("I2C TC Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.I2C_TcCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("I2C TLM Count");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.I2C_TlmCnt);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("I2C SW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.I2C_ErrSW);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("I2C HW Error");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.I2C_ErrHW);

        break;
    }

    case REPORT_KIND_ADCS_GET_CONTROL_ESTIMATION_MODE:
    {
        auto &pl = view.u.adcs_controlestimationmode;

        ImGui::TextUnformatted("ADCS Control & Estimation Mode");

        ImGui::Separator();
        ImGui::Text("[Mode]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Control Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.ControlMode);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Main Estimator Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.MainEstimatorMode);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Backup Estimator Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.BackupEstimatorMode);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Control Timeout [s]");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.ControlTimeout);

        break;
    }

    case REPORT_KIND_ADCS_GET_REFERENCE_IRC_VECTOR:
    {
        auto &pl = view.u.adcs_referenceircvector;

        ImGui::TextUnformatted("ADCS Reference IRC Vector (ECI)");

        ImGui::Separator();
        ImGui::Text("[ECI Pointing Vector]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("ECI Pointing Vector X");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.ECIPointingVectorX);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("ECI Pointing Vector Y");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.ECIPointingVectorY);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("ECI Pointing Vector Z");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.ECIPointingVectorZ);

        break;
    }

    case REPORT_KIND_ADCS_GET_REFERENCE_LLH_TARGET:
    {
        auto &pl = view.u.adcs_referencellhtarget;

        ImGui::TextUnformatted("ADCS Reference LLH Target");

        ImGui::Separator();
        ImGui::Text("[LLH Target]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Latitude");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.Latitude);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Longitude");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.Longitude);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Altitude");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", pl.Altitude);

        break;
    }

    case REPORT_KIND_ADCS_GET_ORBIT_MODE :
    {
        auto &pl = view.u.adcs_orbitmode;

        ImGui::TextUnformatted("ADCS Orbit Mode");

        ImGui::Separator();
        ImGui::Text("[Otbit Mode]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("OrbitMode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.OrbitMode);

        break;
    }

    case REPORT_KIND_ADCS_GET_HEALTH_TLM_MMT:
    {
        auto &pl = view.u.adcs_healthtlmmmt;

        ImGui::TextUnformatted("ADCS Health TLM MMT");

        // 일반 행 헬퍼
        auto row_i16 = [](const char* label, int16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", v);
        };
        auto row_u16 = [](const char* label, uint16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        auto row_flag = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[Mag0 MCU]");
        row_i16("Mag0 MCU Temperature", pl.Mag0MCUTemperature);
        row_u16("Mag0 MCU Current",       pl.Mag0MCUCurrent);
        row_u16("Mag0 MCU Voltage",       pl.Mag0MCUVoltage);

        ImGui::Separator();
        ImGui::Text("[Mag0 Sensor]");
        row_i16("Mag0 Primary Temperature",   pl.Mag0PrimaryTemperature);
        row_i16("Mag0 Redundant Temperature", pl.Mag0RedundantTemperature);

        ImGui::Separator();
        ImGui::Text("[Mag0 Burn]");
        row_u32 ("Mag0 Burn Current",  pl.Mag0BurnCurrent);
        row_flag("Mag0 Deploy Pin State",   pl.Mag0DeployPinState);
        row_flag("Mag0 Burn Pin State",     pl.Mag0BurnPinState);
        row_flag("Mag0 Burn Under Current", pl.Mag0BurnUnderCurrent);
        row_flag("Mag0 Burn Over Current",  pl.Mag0BurnOverCurrent);
        row_flag("Mag0 Deploy Timeout",     pl.Mag0DeployTimeout);

        ImGui::Separator();
        ImGui::Text("[Mag1 MCU]");
        row_i16("Mag1 MCU Temperature", pl.Mag1MCUTemperature);
        row_u16("Mag1 MCU Current",       pl.Mag1MCUCurrent);
        row_u16("Mag1 MCU Voltage",       pl.Mag1MCUVoltage);

        ImGui::Separator();
        ImGui::Text("[Mag1 Sensor]");
        row_i16("Mag1 Primary Temperature",   pl.Mag1PrimaryTemperature);
        row_i16("Mag1 Redundant Temperature", pl.Mag1RedundantTemperature);

        ImGui::Separator();
        ImGui::Text("[Mag1 Burn]");
        row_u32 ("Mag1 Burn Current",  pl.Mag1BurnCurrent);
        row_flag("Mag1 Deploy Pin State",   pl.Mag1DeployPinState);
        row_flag("Mag1 Burn Pin State",     pl.Mag1BurnPinState);
        row_flag("Mag1 Burn Under Current", pl.Mag1BurnUnderCurrent);
        row_flag("Mag1 Burn Over Current",  pl.Mag1BurnOverCurrent);
        row_flag("Mag1 Deploy Timeout",     pl.Mag1DeployTimeout);

        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CUBESENSE_SUN:
    {
        auto &pl = view.u.adcs_rawcubesensesun;

        ImGui::TextUnformatted("ADCS Raw CubeSense Sun");

        // 헬퍼 람다들
        auto row_i16 = [](const char* label, int16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", v);
        };
        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[Timestamp]");
        row_u32("Time Second [s]",      pl.TimeSecond);
        row_u32("Time NanoSecond [ns]", pl.TimeNanoSecond);

        ImGui::Separator();
        ImGui::Text("[FSS0]");
        row_i16("FSS0 Alpha Angle",       pl.FSS0AlphaAngle);
        row_i16("FSS0 Beta Angle",        pl.FSS0BetaAngle);
        row_u8 ("FSS0 Capture Result",    pl.FSS0CaptureResult);
        row_u8 ("FSS0 Detection Result",  pl.FSS0DetectionResult);

        ImGui::Separator();
        ImGui::Text("[FSS1]");
        row_i16("FSS1 Alpha Angle",       pl.FSS1AlphaAngle);
        row_i16("FSS1 Beta Angle",        pl.FSS1BetaAngle);
        row_u8 ("FSS1 Capture Result",    pl.FSS1CaptureResult);
        row_u8 ("FSS1 Detection Result",  pl.FSS1DetectionResult);

        ImGui::Separator();
        ImGui::Text("[FSS2]");
        row_i16("FSS2 Alpha Angle",       pl.FSS2AlphaAngle);
        row_i16("FSS2 Beta Angle",        pl.FSS2BetaAngle);
        row_u8 ("FSS2 Capture Result",    pl.FSS2CaptureResult);
        row_u8 ("FSS2 Detection Result",  pl.FSS2DetectionResult);

        ImGui::Separator();
        ImGui::Text("[FSS3]");
        row_i16("FSS3 Alpha Angle",       pl.FSS3AlphaAngle);
        row_i16("FSS3 Beta Angle",        pl.FSS3BetaAngle);
        row_u8 ("FSS3 Capture Result",    pl.FSS3CaptureResult);
        row_u8 ("FSS3 Detection Result",  pl.FSS3DetectionResult);

        ImGui::Separator();
        ImGui::Text("[Valid]");
        row_u8("Valid Result", pl.ValidResult);

        break;
    }

    case REPORT_KIND_ADCS_GET_REFERENCE_RPY_VALUES:
    {
        auto &pl = view.u.adcs_referencerpyvalues;

        ImGui::TextUnformatted("ADCS Reference RPY Values");

        ImGui::Separator();
        ImGui::Text("[Reference RPY]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Roll");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.Roll);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Pitch");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.Pitch);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Yaw");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.6f", pl.Yaw);

        break;
    }

    case REPORT_KIND_ADCS_GET_OPENLOOPCMD_MTQ:
    {
        auto &pl = view.u.adcs_openloopcmdmtq;

        ImGui::TextUnformatted("ADCS Open Loop Command MTQ");

        ImGui::Separator();
        ImGui::Text("[MTQ Command]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("MTQ0 Open Loop Cmd");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", pl.MTQ0_OpenLoopCmd);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("MTQ1 Open Loop Cmd");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", pl.MTQ1_OpenLoopCmd);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("MTQ2 Open Loop Cmd");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", pl.MTQ2_OpenLoopCmd);

        break;
    }

    case REPORT_KIND_ADCS_GET_POWER_STATE:
    {
        auto &pl = view.u.adcs_powerstate;

        ImGui::TextUnformatted("ADCS Power State");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[Reaction Wheels]");
        row_u8("RWL0", pl.RWL0);
        row_u8("RWL1", pl.RWL1);
        row_u8("RWL2", pl.RWL2);
        row_u8("RWL3", pl.RWL3);

        ImGui::Separator();
        ImGui::Text("[Magnetometers]");
        row_u8("MAG0", pl.MAG0);
        row_u8("MAG1", pl.MAG1);

        ImGui::Separator();
        ImGui::Text("[Gyroscopes]");
        row_u8("GYR0", pl.GYR0);
        row_u8("GYR1", pl.GYR1);

        ImGui::Separator();
        ImGui::Text("[Fine Sun Sensors]");
        row_u8("FSS0", pl.FSS0);
        row_u8("FSS1", pl.FSS1);
        row_u8("FSS2", pl.FSS2);
        row_u8("FSS3", pl.FSS3);

        ImGui::Separator();
        ImGui::Text("[Horizon Sun Sensors]");
        row_u8("HSS0", pl.HSS0);
        row_u8("HSS1", pl.HSS1);

        ImGui::Separator();
        ImGui::Text("[Star Trackers]");
        row_u8("STR0", pl.STR0);
        row_u8("STR1", pl.STR1);

        ImGui::Separator();
        ImGui::Text("[External Sensors]");
        row_u8("Ext Sensor 0", pl.ExtSensor0);
        row_u8("Ext Sensor 1", pl.ExtSensor1);
        row_u8("Ext GYR 0",    pl.ExtGYR0);
        row_u8("Ext GYR 1",    pl.ExtGYR1);

        break;
    }

    case REPORT_KIND_ADCS_GET_RUN_MODE:
    {
        auto &pl = view.u.adcs_runmode;

        ImGui::TextUnformatted("ADCS Run Mode");

        ImGui::Separator();
        ImGui::Text("[Run Mode]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Run Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.RunMode);

        break;
    }

    case REPORT_KIND_ADCS_GET_CONTROL_MODE:
    {
        auto &pl = view.u.adcs_controlmode;

        ImGui::TextUnformatted("ADCS Control Mode");

        ImGui::Separator();
        ImGui::Text("[Control Mode]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Control Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.ControlMode);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Control Timeout");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.ControlTimeout);

        break;
    }

    case REPORT_KIND_ADCS_GET_MAG0_MMT_CALIB_CONFIG:
    {
        auto &pl = view.u.adcs_mag0mmtcalibconfig;

        ImGui::TextUnformatted("ADCS Mag0 MMT Calibration Config");

        auto row_i16 = [](const char* label, int16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", v);
        };

        ImGui::Separator();
        ImGui::Text("[Channel Offsets]");
        row_i16("MMT Ch1 Offset", pl.MMT_Ch1Offset);
        row_i16("MMT Ch2 Offset", pl.MMT_Ch2Offset);
        row_i16("MMT Ch3 Offset", pl.MMT_Ch3Offset);

        ImGui::Separator();
        ImGui::Text("[Sensitivity Matrix - Diagonal]");
        row_i16("MMT Sensitivity S11", pl.MMT_SensitivityMAT_S11);
        row_i16("MMT Sensitivity S22", pl.MMT_SensitivityMAT_S22);
        row_i16("MMT Sensitivity S33", pl.MMT_SensitivityMAT_S33);

        ImGui::Separator();
        ImGui::Text("[Sensitivity Matrix - Off-Diagonal]");
        row_i16("MMT Sensitivity S12", pl.MMT_SensitivityMAT_S12);
        row_i16("MMT Sensitivity S13", pl.MMT_SensitivityMAT_S13);
        row_i16("MMT Sensitivity S21", pl.MMT_SensitivityMAT_S21);
        row_i16("MMT Sensitivity S23", pl.MMT_SensitivityMAT_S23);
        row_i16("MMT Sensitivity S31", pl.MMT_SensitivityMAT_S31);
        row_i16("MMT Sensitivity S32", pl.MMT_SensitivityMAT_S32);

        break;
    }

    case REPORT_KIND_ADCS_GET_MAG1_MMT_CALIB_CONFIG:
    {
        auto &pl = view.u.adcs_mag1mmtcalibconfig;

        ImGui::TextUnformatted("ADCS Mag1 MMT Calibration Config");

        auto row_i16 = [](const char* label, int16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", v);
        };

        ImGui::Separator();
        ImGui::Text("[Channel Offsets]");
        row_i16("MMT Ch1 Offset", pl.MMT_Ch1Offset);
        row_i16("MMT Ch2 Offset", pl.MMT_Ch2Offset);
        row_i16("MMT Ch3 Offset", pl.MMT_Ch3Offset);

        ImGui::Separator();
        ImGui::Text("[Sensitivity Matrix - Diagonal]");
        row_i16("MMT Sensitivity S11", pl.MMT_SensitivityMAT_S11);
        row_i16("MMT Sensitivity S22", pl.MMT_SensitivityMAT_S22);
        row_i16("MMT Sensitivity S33", pl.MMT_SensitivityMAT_S33);

        ImGui::Separator();
        ImGui::Text("[Sensitivity Matrix - Off-Diagonal]");
        row_i16("MMT Sensitivity S12", pl.MMT_SensitivityMAT_S12);
        row_i16("MMT Sensitivity S13", pl.MMT_SensitivityMAT_S13);
        row_i16("MMT Sensitivity S21", pl.MMT_SensitivityMAT_S21);
        row_i16("MMT Sensitivity S23", pl.MMT_SensitivityMAT_S23);
        row_i16("MMT Sensitivity S31", pl.MMT_SensitivityMAT_S31);
        row_i16("MMT Sensitivity S32", pl.MMT_SensitivityMAT_S32);

        break;
    }

    case REPORT_KIND_ADCS_GET_ESTIMATION_MODE:
    {
        auto &pl = view.u.adcs_estimationmode;

        ImGui::TextUnformatted("ADCS Estimation Mode");

        ImGui::Separator();
        ImGui::Text("[Mode]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Main Estimator Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.MainEstimatorMode);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Backup Estimator Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.BackupEstimatorMode);

        break;
    }

    case REPORT_KIND_ADCS_GET_OPERATIONAL_STATE:
    {
        auto &pl = view.u.adcs_operationalstate;

        ImGui::TextUnformatted("ADCS Operational State");

        ImGui::Separator();
        ImGui::Text("[State]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Operational Mode");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.OperationalMode);

        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CSS_SENSOR:
    {
        auto &pl = view.u.adcs_rawcsssensor;

        ImGui::TextUnformatted("ADCS Raw CSS Sensor");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[Timestamp]");
        row_u32("Time Seconds",     pl.TimeSeconds);
        row_u32("Time NanoSeconds", pl.TimeNanoSeconds);

        ImGui::Separator();
        ImGui::Text("[CSS Raw Values]");
        row_u8("CSS0", pl.CSS0);
        row_u8("CSS1", pl.CSS1);
        row_u8("CSS2", pl.CSS2);
        row_u8("CSS3", pl.CSS3);
        row_u8("CSS4", pl.CSS4);
        row_u8("CSS5", pl.CSS5);
        row_u8("CSS6", pl.CSS6);
        row_u8("CSS7", pl.CSS7);
        row_u8("CSS8", pl.CSS8);
        row_u8("CSS9", pl.CSS9);

        ImGui::Separator();
        ImGui::Text("[Valid]");
        row_u8("CSS Valid Flag", pl.CSSValidFlag);

        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_GYR_SENSOR:
    {
        auto &pl = view.u.adcs_rawgyrsensor;

        ImGui::TextUnformatted("ADCS Raw GYR Sensor");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_f32 = [](const char* label, float v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.6f", v);
        };

        ImGui::Separator();
        ImGui::Text("[Timestamp]");
        row_u32("Time Seconds",     pl.TimeSeconds);
        row_u32("Time NanoSeconds", pl.TimeNanoSeconds);

        ImGui::Separator();
        ImGui::Text("[GYR0 Raw Rate]");
        row_f32("GYR0 Raw Rate X", pl.GYR0RawRateX);
        row_f32("GYR0 Raw Rate Y", pl.GYR0RawRateY);
        row_f32("GYR0 Raw Rate Z", pl.GYR0RawRateZ);

        ImGui::Separator();
        ImGui::Text("[GYR1 Raw Rate]");
        row_f32("GYR1 Raw Rate X", pl.GYR1RawRateX);
        row_f32("GYR1 Raw Rate Y", pl.GYR1RawRateY);
        row_f32("GYR1 Raw Rate Z", pl.GYR1RawRateZ);

        ImGui::Separator();
        ImGui::Text("[Valid]");
        row_u8("GYR0 Valid Flag", pl.GYR0ValidFlag);
        row_u8("GYR1 Valid Flag", pl.GYR1ValidFlag);

        break;
    }

    case REPORT_KIND_ADCS_GET_CALIBRATED_GYR_SENSOR:
    {
        auto &pl = view.u.adcs_calibratedgyrsensor;

        ImGui::TextUnformatted("ADCS Calibrated GYR Sensor");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_f32 = [](const char* label, float v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.6f", v);
        };

        ImGui::Separator();
        ImGui::Text("[Timestamp]");
        row_u32("Time Seconds",     pl.TimeSeconds);
        row_u32("Time NanoSeconds", pl.TimeNanoSeconds);

        ImGui::Separator();
        ImGui::Text("[GYR0 Calibrated Rate]");
        row_f32("GYR0 Calibrated Rate X", pl.GYR0CalibratedRateX);
        row_f32("GYR0 Calibrated Rate Y", pl.GYR0CalibratedRateY);
        row_f32("GYR0 Calibrated Rate Z", pl.GYR0CalibratedRateZ);

        ImGui::Separator();
        ImGui::Text("[GYR1 Calibrated Rate]");
        row_f32("GYR1 Calibrated Rate X", pl.GYR1CalibratedRateX);
        row_f32("GYR1 Calibrated Rate Y", pl.GYR1CalibratedRateY);
        row_f32("GYR1 Calibrated Rate Z", pl.GYR1CalibratedRateZ);

        ImGui::Separator();
        ImGui::Text("[External GYR0 Calibrated Rate]");
        row_f32("Ext GYR0 Calibrated Rate X", pl.ExtGYR0CalibratedRateX);
        row_f32("Ext GYR0 Calibrated Rate Y", pl.ExtGYR0CalibratedRateY);
        row_f32("Ext GYR0 Calibrated Rate Z", pl.ExtGYR0CalibratedRateZ);

        ImGui::Separator();
        ImGui::Text("[External GYR1 Calibrated Rate]");
        row_f32("Ext GYR1 Calibrated Rate X", pl.ExtGYR1CalibratedRateX);
        row_f32("Ext GYR1 Calibrated Rate Y", pl.ExtGYR1CalibratedRateY);
        row_f32("Ext GYR1 Calibrated Rate Z", pl.ExtGYR1CalibratedRateZ);

        ImGui::Separator();
        ImGui::Text("[Valid]");
        row_u8("GYR0 Valid Flag",     pl.GYR0ValidFlag);
        row_u8("GYR1 Valid Flag",     pl.GYR1ValidFlag);
        row_u8("Ext GYR0 Valid Flag", pl.EXTGYR0ValidFlag);
        row_u8("Ext GYR1 Valid Flag", pl.EXTGYR1ValidFlag);

        break;
    }

    case REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG:
    {
        auto &pl = view.u.adcs_magsensingelmconfig;

        ImGui::TextUnformatted("ADCS Mag Sensing Element Config");

        ImGui::Separator();
        ImGui::Text("[Sensing Element]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Mag0 Sensing Element");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.Mag0SensingElement);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("Mag1 Sensing Element");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.Mag1SensingElement);

        break;
    }

    case REPORT_KIND_ADCS_GET_TLM_LOG_INCLMASK:
    {
        auto &pl = view.u.adcs_tlmloginclmask;

        ImGui::TextUnformatted("ADCS TLM Log Inclusion Mask");

        ImGui::Separator();
        ImGui::Text("[Fast Inclusion Bitmask]");
        for (int i = 0; i < 5; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fast Inclusion Bitmask[%d]", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%02X", pl.FastInclusionBitmask[i]);
        }

        ImGui::Separator();
        ImGui::Text("[Slow Inclusion Bitmask]");
        for (int i = 0; i < 5; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Slow Inclusion Bitmask[%d]", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%02X", pl.SlowInclusionBitmask[i]);
        }

        break;
    }

    case REPORT_KIND_ADCS_GET_UNSOLICIT_TLM_MSG_SETUP:
    {
        auto &pl = view.u.adcs_unsolicittlmmsgsetup;

        ImGui::TextUnformatted("ADCS Unsolicited TLM Msg Setup");

        ImGui::Separator();
        ImGui::Text("[Return Intervals]");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART TLM Return Interval");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UARTTlmReturnInterval);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("UART2 TLM Return Interval");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.UART2TlmReturnInterval);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("CAN TLM Return Interval");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u", pl.CANTlmRetrunInterval);

        ImGui::Separator();
        ImGui::Text("[UART TLM ID Inclusion Bitmask]");
        for (int i = 0; i < 5; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("UART TLM ID Bitmask[%d]", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%02X", pl.UARTTlmIDInclusionBitmask[i]);
        }

        ImGui::Separator();
        ImGui::Text("[UART2 TLM ID Inclusion Bitmask]");
        for (int i = 0; i < 5; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("UART2 TLM ID Bitmask[%d]", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%02X", pl.UART2TlmIDInclusionBitmask[i]);
        }

        ImGui::Separator();
        ImGui::Text("[CAN TLM ID Inclusion Bitmask]");
        for (int i = 0; i < 5; ++i) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("CAN TLM ID Bitmask[%d]", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%02X", pl.CANTlmIDInclusionBitmask[i]);
        }

        break;
    }

    case REPORT_KIND_ADCS_GET_UNSOLICIT_EVENT_MSG_SETUP:
    {
        auto &pl = view.u.adcs_unsoliciteventmsgsetup;

        ImGui::TextUnformatted("ADCS Unsolicited Event Msg Setup");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[UART Event Filter]");
        row_u8("Info UART",     pl.InfoUART);
        row_u8("Minor UART",    pl.MinorUART);
        row_u8("Major UART",    pl.MajorUART);
        row_u8("Critical UART", pl.CriticalUART);

        ImGui::Separator();
        ImGui::Text("[UART2 Event Filter]");
        row_u8("Info UART2",     pl.InfoUART2);
        row_u8("Minor UART2",    pl.MinorUART2);
        row_u8("Major UART2",    pl.MajorUART2);
        row_u8("Critical UART2", pl.CriticalUART2);

        ImGui::Separator();
        ImGui::Text("[CAN Event Filter]");
        row_u8("Info CAN",     pl.InfoCAN);
        row_u8("Minor CAN",    pl.MinorCAN);
        row_u8("Major CAN",    pl.MajorCAN);
        row_u8("Critical CAN", pl.CriticalCAN);

        break;
    }

    case REPORT_KIND_ADCS_GET_EVENT_LOG_STATUS_RESPONSE:
    {
        auto &pl = view.u.adcs_eventlogstatusresponse;

        ImGui::TextUnformatted("ADCS Event Log Status Response");

        auto row_u8 = [](const char* label, uint8_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u16 = [](const char* label, uint16_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };
        auto row_u32 = [](const char* label, uint32_t v) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(label);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", v);
        };

        ImGui::Separator();
        ImGui::Text("[Entry Counts]");
        row_u16("Num Queued Entry",   pl.NumQueuedEntry);
        row_u16("Num Buffered Entry", pl.NumBufferedEntry);
        row_u32("Num Entry",          pl.NumEntry);
        row_u32("Num Empty Entry",    pl.NumEmptyEntry);

        ImGui::Separator();
        ImGui::Text("[Entry Timestamps]");
        row_u32("Old Entry Unix Time",  pl.OldEntryUnixTime);
        row_u32("Last Entry Unix Time", pl.LastEntryUnixTime);

        ImGui::Separator();
        ImGui::Text("[Event Severity Counts]");
        row_u32("Num Critical EVS",      pl.NumCriticalEVS);
        row_u32("Num Major Warning EVS", pl.NumMajorWarningEVS);
        row_u32("Num Minor Warning EVS", pl.NumMinorWarningEVS);
        row_u32("Num Info EVS",          pl.NumInfoEVS);

        ImGui::Separator();
        ImGui::Text("[State]");
        row_u32("Write Count",     pl.WriteCnt);
        row_u8 ("Read Que State", pl.ReadQueState);

        break;
    }

    case REPORT_KIND_ADCS_GET_PORTMAP:
    {
        auto &pl = view.u.adcs_portmap;

        ImGui::TextUnformatted("ADCS Port Map");

        // 노드 정보 표시 헬퍼 (4개 필드를 한 그룹으로)
        auto node_group = [&](const char* group_label,
                              uint8_t node_type, uint8_t abst_node_type,
                              uint32_t serial_num, uint32_t address) {
            ImGui::Separator();
            ImGui::Text("[%s]", group_label);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Node Type");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", node_type);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Abstract Node Type");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", abst_node_type);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Serial Number");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%u", serial_num);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Address");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("0x%08X", address);
        };

        node_group("Sensor1", pl.NodeType_Sensor1, pl.AbstNodeType_Sensor1, pl.SerialNum_Sensor1, pl.Address_Sensor1);
        node_group("Sensor2", pl.NodeType_Sensor2, pl.AbstNodeType_Sensor2, pl.SerialNum_Sensor2, pl.Address_Sensor2);
        node_group("Sensor3", pl.NodeType_Sensor3, pl.AbstNodeType_Sensor3, pl.SerialNum_Sensor3, pl.Address_Sensor3);
        node_group("Sensor4", pl.NodeType_Sensor4, pl.AbstNodeType_Sensor4, pl.SerialNum_Sensor4, pl.Address_Sensor4);
        node_group("Sensor5", pl.NodeType_Sensor5, pl.AbstNodeType_Sensor5, pl.SerialNum_Sensor5, pl.Address_Sensor5);
        node_group("Sensor6", pl.NodeType_Sensor6, pl.AbstNodeType_Sensor6, pl.SerialNum_Sensor6, pl.Address_Sensor6);
        node_group("Sensor7", pl.NodeType_Sensor7, pl.AbstNodeType_Sensor7, pl.SerialNum_Sensor7, pl.Address_Sensor7);
        node_group("Sensor8", pl.NodeType_Sensor8, pl.AbstNodeType_Sensor8, pl.SerialNum_Sensor8, pl.Address_Sensor8);

        node_group("Wheel1", pl.NodeType_Wheel1, pl.AbstNodeType_Wheel1, pl.SerialNum_Wheel1, pl.Address_Wheel1);
        node_group("Wheel2", pl.NodeType_Wheel2, pl.AbstNodeType_Wheel2, pl.SerialNum_Wheel2, pl.Address_Wheel2);
        node_group("Wheel3", pl.NodeType_Wheel3, pl.AbstNodeType_Wheel3, pl.SerialNum_Wheel3, pl.Address_Wheel3);
        node_group("Wheel4", pl.NodeType_Wheel4, pl.AbstNodeType_Wheel4, pl.SerialNum_Wheel4, pl.Address_Wheel4);

        break;
    }

    case REPORT_KIND_ADCS_HK_REPORT:
    {
        const auto &p = view.u.adcs_hk;
        RowText("Type", "ADCS housekeeping report");
        RowFmt("MAG0 MCU Current", "%u", (unsigned)p.MAG0MCUCurrent);
        RowFmt("FSS0 MCU Temperature", "%d", (int)p.FSS0MCUTemperature);
        RowFmt("FSS0 MCU Current", "%u", (unsigned)p.FSS0MCUCurrent);
        RowFmt("FSS0 MCU Voltage", "%u", (unsigned)p.FSS0MCUVoltage);
        RowFmt("FSS0 CAM/SRAM Overcurrent", "%u", (unsigned)p.FSS0CAMSRAMOvercurrent);
        RowFmt("MTQ Current +/-", "%f, %f, %f, %f, %f, %f",
               p.MTQ1PositiveCurrentAverage, p.MTQ1NegativeCurrentAverage,
               p.MTQ2PositiveCurrentAverage, p.MTQ2NegativeCurrentAverage,
               p.MTQ3PositiveCurrentAverage, p.MTQ3NegativeCurrentAverage);
        RowFmt("MTQ Polarity", "0x%02X", (unsigned)p.MTQPolarity);
        RowFmt("Bytes Received", "%u", (unsigned)p.bytesReceived);
        RowFmt("MTQ Max Dipole", "%f, %f, %f", p.mtq0Mmax, p.mtq1Mmax, p.mtq2Mmax);
        RowFmt("MTQ On Time Max", "%u", (unsigned)p.onTimeMax);
        RowFmt("MTQ Filter Factor", "%f", p.mtqFfac);
        RowFmt("CSS Raw", "%u, %u, %u, %u, %u, %u, %u, %u, %u, %u",
               (unsigned)p.css0Raw, (unsigned)p.css1Raw, (unsigned)p.css2Raw,
               (unsigned)p.css3Raw, (unsigned)p.css4Raw, (unsigned)p.css5Raw,
               (unsigned)p.css6Raw, (unsigned)p.css7Raw, (unsigned)p.css8Raw,
               (unsigned)p.css9Raw);
        RowFmt("CSS Raw Valid", "%u", (unsigned)p.rawCssIsValid);
        RowFmt("CSS Cal Vector", "%d, %d, %d",
               (int)p.cssCalVecX, (int)p.cssCalVecY, (int)p.cssCalVecZ);
        RowFmt("CSS Cal Valid", "%u", (unsigned)p.calCssIsValid);
        break;
    }

    case REPORT_KIND_ADCS_NOOP_REPORT:
        RowText("Type", view.raw_size == 14 ? "ADCS NOOP text (BEE1012)" : "ADCS NOOP counters (UELYSYS)");
        if (view.raw_size == 14)
            RowFmt("Message", "%.*s", (int)view.raw_size, (const char *)view.raw_bytes);
        else if (view.raw_size == 2) {
            RowFmt("Command Counter", "%u", (unsigned)view.raw_bytes[0]);
            RowFmt("Command Error Counter", "%u", (unsigned)view.raw_bytes[1]);
        }
        break;

    case REPORT_KIND_ADCS_RESET_COUNTERS_REPORT:
        RowText("Type", "ADCS reset counters report");
        RowFmt("Command Counter", "%u", (unsigned)view.raw_bytes[0]);
        RowFmt("Command Error Counter", "%u", (unsigned)view.raw_bytes[1]);
        break;

    case REPORT_KIND_ADCS_INTERFACE_TRANSPORT_REPORT:
        RowText("Type", "ADCS interface transport report");
        RowFmt("Transport Type", "%u", (unsigned)view.u.adcs_interface_transport.TransportType);
        break;

    case REPORT_KIND_ADCS_GYRO_LOG_REPORT:
        RowText("Type", "ADCS gyro log report");
        RowFmt("Duration [s]", "%u", (unsigned)view.u.adcs_gyro_log.DurationSec);
        RowFmt("Samples Written", "%u", (unsigned)view.u.adcs_gyro_log.SamplesWritten);
        RowFmt("Read Errors", "%u", (unsigned)view.u.adcs_gyro_log.ReadErrors);
        RowFmt("Gyro Source", "%u", (unsigned)view.u.adcs_gyro_log.GyroSource);
        break;

    case REPORT_KIND_ADCS_COMMAND_REPORT:
        RowText("Type", "ADCS command status / sequence stage");
        RowFmt("Command Code", "%u", (unsigned)view.reflected_cc);
        RowFmt("Return Code", "%d", (int)view.ret_code);
        RowFmt("Payload Size", "%u", (unsigned)view.raw_size);
        if (view.raw_size > 0)
            RowArr_Hex("Payload", view.raw_bytes, view.raw_size, 16);
        break;

    case REPORT_KIND_ADCS_GET_WHL_CONFIG:
    {
        const auto &p = view.u.adcs_whlconfig;
        RowText("Type", "ADCS wheel configuration");
        RowFmt("RWL0 (inertia, momentum, torque)", "%f, %f, %f", p.Rwl0Inertia, p.Rwl0MaxMomentum, p.Rwl0MaxToque);
        RowFmt("RWL1 (inertia, momentum, torque)", "%f, %f, %f", p.Rwl1Inertia, p.Rwl1MaxMomentum, p.Rwl1MaxToque);
        RowFmt("RWL2 (inertia, momentum, torque)", "%f, %f, %f", p.Rwl2Inertia, p.Rwl2MaxMomentum, p.Rwl2MaxToque);
        RowFmt("RWL3 (inertia, momentum, torque)", "%f, %f, %f", p.Rwl3Inertia, p.Rwl3MaxMomentum, p.Rwl3MaxToque);
        RowFmt("Wheel Ramp Torque", "%f", p.WheelRampTorque);
        RowFmt("Wheel Scheme", "%u", (unsigned)p.WheelScheme);
        RowFmt("Failed Wheel ID", "%u", (unsigned)p.FailedWheelID);
        RowFmt("Pyramid Nominal Momentum", "%f", p.PyramidNominalMomentum);
        RowFmt("Pyramid Tilt Angle", "%f", p.PyramidTiltAngle);
        break;
    }

    case REPORT_KIND_ADCS_GET_SATELLITE_CONFIG:
    {
        const auto &p = view.u.adcs_satelliteconfig;
        RowText("Type", "ADCS satellite configuration");
        RowFmt("Inertia Ixx/Iyy/Izz", "%f, %f, %f", p.Ixx, p.Iyy, p.Izz);
        RowFmt("Inertia Ixy/Ixz/Iyz", "%f, %f, %f", p.Ixy, p.Ixz, p.Iyz);
        RowFmt("Sun Pointing Body Vector", "%d, %d, %d", (int)p.SunPointingBodyVectorX, (int)p.SunPointingBodyVectorY, (int)p.SunPointingBodyVectorZ);
        RowFmt("Target Tracking Body Vector", "%d, %d, %d", (int)p.TargetTrackingBodyVectorX, (int)p.TargetTrackingBodyVectorY, (int)p.TargetTrackingBodyVectorZ);
        RowFmt("Satellite Tracking Body Vector", "%d, %d, %d", (int)p.SatTrackingBodyVectorX, (int)p.SatTrackingBodyVectorY, (int)p.SatTrackingBodyVectorZ);
        break;
    }

    case REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG:
    {
        const auto &p = view.u.adcs_controllerconfig;
        RowText("Type", "ADCS controller configuration");
        RowFmt("Default Control Mode", "%u", (unsigned)p.DefaultControlMode);
        RowFmt("Detumbling Gains", "%f, %f", p.DetumblingDampingGain, p.DetumblingSpinGain);
        RowFmt("Sun Spin Gains (sunlit, eclipse)", "%f, %f", p.SunSpinGain_Sunlit, p.SunSpinGain_Eclipse);
        RowFmt("Fast B-Dot Gain", "%f", p.FastBDotGain);
        RowFmt("Y Momentum Nutation Gains", "%f, %f", p.YMomNutationDampingGain, p.YMomNutationDampingQuatGain);
        RowFmt("GG Quaternion Gains", "%f, %f, %f", p.XGGQuatGain, p.YGGQuatGain, p.ZGGQuatGain);
        RowFmt("Wheel Desaturation Gain", "%f", p.WheelDesatControlGain);
        RowFmt("Y Momentum P/D", "%f, %f", p.YMomProportionalGain, p.YMomDerivativeGain);
        RowFmt("Reaction Wheel P/D", "%f, %f", p.RWheelProportionalGain, p.RWheelDerivativeGain);
        RowFmt("Tracking P/D/I", "%f, %f, %f", p.TrackingProportionalGain, p.TrackingDerivativeGain, p.TrackingIntegralGain);
        RowFmt("Reference Spin Rate [deg/s]", "%f", p.ReferenceSpinRate_degps);
        RowFmt("Reference Wheel Momentum [Nms]", "%f", p.ReferenceWheelMomentum_Nms);
        RowFmt("Y Wheel Bias Momentum [Nms]", "%f", p.YWheelBiasMomentum_Nms);
        RowFmt("RW Reference Spin Rate [deg/s]", "%f", p.RefSpinRate_RW_degps);
        RowFmt("Sun Keep-Out Angle [deg]", "%f", p.SunKeepOutAngle_deg);
        RowFmt("Roll Limit Angle [deg]", "%f", p.RollLimitAngle_deg);
        RowFmt("Flags (yaw, eclipse, avoidance)", "%u, %u, %u",
               (unsigned)p.flags.YawCompensationForEarthRotation,
               (unsigned)p.flags.EnableSunTrackingInEclipse,
               (unsigned)p.flags.EnableSunAvoidance);
        break;
    }

    case REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG:
    {
        const auto &p = view.u.adcs_defaultmodeconfig;
        RowText("Type", "ADCS default mode configuration");
        RowFmt("Default Run Mode", "%u", (unsigned)p.DefaultRunMode);
        RowFmt("Default Operational State", "%u", (unsigned)p.DefaultOperationalState);
        RowFmt("Safe-State Control Mode", "%u", (unsigned)p.DefaultControlModeInOpStateSafe);
        RowFmt("Auto-State Control Mode", "%u", (unsigned)p.DefaultControlModeInOpStateAuto);
        break;
    }

    case REPORT_KIND_ADCS_GET_MOUNTING_CONFIG:
    {
        const auto &p = view.u.adcs_mountingconfig;
        RowText("Type", "ADCS mounting configuration");
        RowFmt("Stack X/Y/Z", "%u, %u, %u", (unsigned)p.StackX_mounting, (unsigned)p.StackY_mounting, (unsigned)p.StackZ_mounting);
        RowFmt("MTQ0/1/2", "%u, %u, %u", (unsigned)p.MTQ0_mounting, (unsigned)p.MTQ1_mounting, (unsigned)p.MTQ2_mounting);
        RowFmt("Wheel0/1/2/3", "%u, %u, %u, %u", (unsigned)p.Wheel0_mounting, (unsigned)p.Wheel1_mounting, (unsigned)p.Wheel2_mounting, (unsigned)p.Wheel3_mounting);
        RowFmt("Pyramid RWL alpha/beta/gamma", "%d, %d, %d", (int)p.PyramidRWL_alpha, (int)p.PyramidRWL_beta, (int)p.PyramidRWL_gamma);
        RowFmt("CSS0..4", "%u, %u, %u, %u, %u", (unsigned)p.CSS0_mounting, (unsigned)p.CSS1_mounting, (unsigned)p.CSS2_mounting, (unsigned)p.CSS3_mounting, (unsigned)p.CSS4_mounting);
        RowFmt("CSS5..9", "%u, %u, %u, %u, %u", (unsigned)p.CSS5_mounting, (unsigned)p.CSS6_mounting, (unsigned)p.CSS7_mounting, (unsigned)p.CSS8_mounting, (unsigned)p.CSS9_mounting);
        RowFmt("FSS0 alpha/beta/gamma", "%d, %d, %d", (int)p.FSS0_alpha, (int)p.FSS0_beta, (int)p.FSS0_gamma);
        RowFmt("FSS1 alpha/beta/gamma", "%d, %d, %d", (int)p.FSS1_alpha, (int)p.FSS1_beta, (int)p.FSS1_gamma);
        RowFmt("FSS2 alpha/beta/gamma", "%d, %d, %d", (int)p.FSS2_alpha, (int)p.FSS2_beta, (int)p.FSS2_gamma);
        RowFmt("FSS3 alpha/beta/gamma", "%d, %d, %d", (int)p.FSS3_alpha, (int)p.FSS3_beta, (int)p.FSS3_gamma);
        RowFmt("HSS0 alpha/beta/gamma", "%d, %d, %d", (int)p.HSS0_alpha, (int)p.HSS0_beta, (int)p.HSS0_gamma);
        RowFmt("HSS1 alpha/beta/gamma", "%d, %d, %d", (int)p.HSS1_alpha, (int)p.HSS1_beta, (int)p.HSS1_gamma);
        RowFmt("MAG0 alpha/beta/gamma", "%d, %d, %d", (int)p.MAG0_alpha, (int)p.MAG0_beta, (int)p.MAG0_gamma);
        RowFmt("MAG1 alpha/beta/gamma", "%d, %d, %d", (int)p.MAG1_alpha, (int)p.MAG1_beta, (int)p.MAG1_gamma);
        RowFmt("STR0 alpha/beta/gamma", "%d, %d, %d", (int)p.STR0_alpha, (int)p.STR0_beta, (int)p.STR0_gamma);
        RowFmt("STR1 alpha/beta/gamma", "%d, %d, %d", (int)p.STR1_alpha, (int)p.STR1_beta, (int)p.STR1_gamma);
        RowFmt("External Sensor0 alpha/beta/gamma", "%d, %d, %d", (int)p.ExtSensor0_alpha, (int)p.ExtSensor0_beta, (int)p.ExtSensor0_gamma);
        RowFmt("External Sensor1 alpha/beta/gamma", "%d, %d, %d", (int)p.ExtSensor1_alpha, (int)p.ExtSensor1_beta, (int)p.ExtSensor1_gamma);
        RowFmt("External Gyro0 axes", "%u, %u, %u", (unsigned)p.ExtGyro0_axis1_mounting, (unsigned)p.ExtGyro0_axis2_mounting, (unsigned)p.ExtGyro0_axis3_mounting);
        RowFmt("External Gyro1 axes", "%u, %u, %u", (unsigned)p.ExtGyro1_axis1_mounting, (unsigned)p.ExtGyro1_axis2_mounting, (unsigned)p.ExtGyro1_axis3_mounting);
        break;
    }

    case REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG:
    {
        const auto &p = view.u.adcs_estimatorconfig;
        RowText("Type", "ADCS estimator configuration");
        RowFmt("Default Estimators", "%u, %u", (unsigned)p.DefaultMainEstimatorMode, (unsigned)p.DefaultBackupEstimatorMode);
        RowFmt("Measurement Noise MAG/CSS/FSS", "%f, %f, %f", p.MAGMeasurementNoise, p.CSSMeasurementNoise, p.FSSMeasurementNoise);
        RowFmt("Measurement Noise HSS/STR", "%f, %f", p.HSSMeasurementNoise, p.STRMeasurementNoise);
        RowFmt("System Noise MMTRKF/EKF", "%f, %f", p.MMTRKFSystemNoise, p.EKFSystemNoise);
        RowFmt("Nutation Correction epsilon/psi", "%f, %f", p.NutationEpsilonCorrection, p.NutationPsiCorrection);
        RowFmt("Use FSS/CSS/HSS/STR", "%u, %u, %u, %u", (unsigned)p.UseFSSinEKF, (unsigned)p.UseCSSinEKF, (unsigned)p.UseHSSinEKF, (unsigned)p.UseSTRinEKF);
        RowFmt("Triad Vector 1/2", "%u, %u", (unsigned)p.TriadVector1, (unsigned)p.TriadVector2);
        break;
    }

    case REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG:
    {
        const auto &p = view.u.adcs_satorbitparamconfig;
        RowText("Type", "ADCS satellite orbit parameter configuration");
        RowFmt("Epoch", "%.12lf", p.Epoch);
        RowFmt("Inclination", "%.12lf", p.Inclination);
        RowFmt("RAAN", "%.12lf", p.RAAN);
        RowFmt("Eccentricity", "%.12lf", p.Eccentricity);
        RowFmt("Argument of Perigee", "%.12lf", p.AOP);
        RowFmt("Mean Anomaly", "%.12lf", p.MeanAnomaly);
        RowFmt("Mean Motion", "%.12lf", p.MeanMotion);
        RowFmt("B-Star Drag", "%.12lf", p.B_StarDrag);
        break;
    }

    case REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG:
    {
        const auto &p = view.u.adcs_nodeselectionconfig;
        RowText("Type", "ADCS node selection configuration");
        RowFmt("RWL Selection Flags", "0x%02X", (unsigned)p.RWLSelectionFlags);
        RowFmt("MAG Selection Flags", "0x%02X", (unsigned)p.MAGSelectionFlags);
        RowFmt("FSS Selection Flags", "0x%02X", (unsigned)p.FSSSelectionFlags);
        RowFmt("HSS Selection Flags", "0x%02X", (unsigned)p.HSSSelectionFlags);
        RowFmt("GYR Selection Flags", "0x%02X", (unsigned)p.GYRSelectionFlags);
        RowFmt("STR Selection Flags", "0x%02X", (unsigned)p.STRSelectionFlags);
        RowFmt("GNSS Selection Flags", "0x%02X", (unsigned)p.GNSSSelectionFlags);
        RowFmt("External Sensor Selection Flags", "0x%02X", (unsigned)p.ExtSensorSelectionFlags);
        break;
    }

    case REPORT_KIND_ADCS_GET_MTQ_CONFIG:
    {
        const auto &p = view.u.adcs_mtqconfig;
        RowText("Type", "ADCS MTQ configuration");
        RowFmt("Maximum Dipole Moment 0/1/2", "%f, %f, %f", p.MTQ0MaxDipoleMoment, p.MTQ1MaxDipoleMoment, p.MTQ2MaxDipoleMoment);
        RowFmt("Maximum MTQ On Time", "%u", (unsigned)p.MaxMTQOnTime);
        RowFmt("Minimum MTQ On Time", "%u", (unsigned)p.MinMTQOnTime);
        RowFmt("Magnetic Control Filter Factor", "%f", p.MagneticControlFilterFactor);
        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_RWL_SENSOR:
    {
        const auto &p = view.u.adcs_rawrwlsensor;
        RowText("Type", "ADCS raw reaction-wheel sensor");
        RowFmt("Time", "%u.%09u", (unsigned)p.TimeSeconds, (unsigned)p.TimeNanoSeconds);
        RowFmt("RWL0/1/2/3 Speed", "%f, %f, %f, %f", p.RWL0MeasuredSpeed, p.RWL1MeasuredSpeed, p.RWL2MeasuredSpeed, p.RWL3MeasuredSpeed);
        RowFmt("RWL0/1/2/3 Valid", "%u, %u, %u, %u", (unsigned)p.RWL0ValidFlag, (unsigned)p.RWL1ValidFlag, (unsigned)p.RWL2ValidFlag, (unsigned)p.RWL3ValidFlag);
        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_FSS:
    {
        const auto &p = view.u.adcs_rawcalibratedfss;
        RowText("Type", "BEE1012 raw + calibrated FSS report");
        RowFmt("Raw Time", "%u.%09u", (unsigned)p.Raw.TimeSecond, (unsigned)p.Raw.TimeNanoSecond);
        RowFmt("Raw FSS0 angles/result", "%d, %d, %u, %u", (int)p.Raw.FSS0AlphaAngle, (int)p.Raw.FSS0BetaAngle, (unsigned)p.Raw.FSS0CaptureResult, (unsigned)p.Raw.FSS0DetectionResult);
        RowFmt("Raw FSS1 angles/result", "%d, %d, %u, %u", (int)p.Raw.FSS1AlphaAngle, (int)p.Raw.FSS1BetaAngle, (unsigned)p.Raw.FSS1CaptureResult, (unsigned)p.Raw.FSS1DetectionResult);
        RowFmt("Raw FSS2 angles/result", "%d, %d, %u, %u", (int)p.Raw.FSS2AlphaAngle, (int)p.Raw.FSS2BetaAngle, (unsigned)p.Raw.FSS2CaptureResult, (unsigned)p.Raw.FSS2DetectionResult);
        RowFmt("Raw FSS3 angles/result", "%d, %d, %u, %u", (int)p.Raw.FSS3AlphaAngle, (int)p.Raw.FSS3BetaAngle, (unsigned)p.Raw.FSS3CaptureResult, (unsigned)p.Raw.FSS3DetectionResult);
        RowFmt("Raw Valid", "%u", (unsigned)p.Raw.ValidResult);
        RowFmt("Calibrated Time", "%u.%09u", (unsigned)p.Calibrated.TimeSeconds, (unsigned)p.Calibrated.TimeNanoSeconds);
        RowFmt("Calibrated FSS0", "%d, %d, %d", (int)p.Calibrated.FSS0CalVecX, (int)p.Calibrated.FSS0CalVecY, (int)p.Calibrated.FSS0CalVecZ);
        RowFmt("Calibrated FSS1", "%d, %d, %d", (int)p.Calibrated.FSS1CalVecX, (int)p.Calibrated.FSS1CalVecY, (int)p.Calibrated.FSS1CalVecZ);
        RowFmt("Calibrated FSS2", "%d, %d, %d", (int)p.Calibrated.FSS2CalVecX, (int)p.Calibrated.FSS2CalVecY, (int)p.Calibrated.FSS2CalVecZ);
        RowFmt("Calibrated FSS3", "%d, %d, %d", (int)p.Calibrated.FSS3CalVecX, (int)p.Calibrated.FSS3CalVecY, (int)p.Calibrated.FSS3CalVecZ);
        RowFmt("Calibrated Valid", "%u, %u, %u, %u", (unsigned)p.Calibrated.FSS0ValidFlag, (unsigned)p.Calibrated.FSS1ValidFlag, (unsigned)p.Calibrated.FSS2ValidFlag, (unsigned)p.Calibrated.FSS3ValidFlag);
        RowFmt("Calibrated Best", "%u, %u, %u, %u", (unsigned)p.Calibrated.FSS0BestFlag, (unsigned)p.Calibrated.FSS1BestFlag, (unsigned)p.Calibrated.FSS2BestFlag, (unsigned)p.Calibrated.FSS3BestFlag);
        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_CSS:
    {
        const auto &p = view.u.adcs_rawcalibratedcss;
        RowText("Type", "BEE1012 raw + calibrated CSS report");
        RowFmt("Raw Time", "%u.%09u", (unsigned)p.Raw.TimeSeconds, (unsigned)p.Raw.TimeNanoSeconds);
        RowFmt("Raw CSS0..4", "%u, %u, %u, %u, %u", (unsigned)p.Raw.CSS0, (unsigned)p.Raw.CSS1, (unsigned)p.Raw.CSS2, (unsigned)p.Raw.CSS3, (unsigned)p.Raw.CSS4);
        RowFmt("Raw CSS5..9", "%u, %u, %u, %u, %u", (unsigned)p.Raw.CSS5, (unsigned)p.Raw.CSS6, (unsigned)p.Raw.CSS7, (unsigned)p.Raw.CSS8, (unsigned)p.Raw.CSS9);
        RowFmt("Raw Valid", "%u", (unsigned)p.Raw.CSSValidFlag);
        RowFmt("Calibrated Time", "%u.%09u", (unsigned)p.Calibrated.TimeSeconds, (unsigned)p.Calibrated.TimeNanoSeconds);
        RowFmt("Calibrated CSS Vector", "%d, %d, %d", (int)p.Calibrated.CSSCalUnitVecX, (int)p.Calibrated.CSSCalUnitVecY, (int)p.Calibrated.CSSCalUnitVecZ);
        RowFmt("Calibrated Valid", "%u", (unsigned)p.Calibrated.CSSValidFlag);
        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_GYR:
    {
        const auto &p = view.u.adcs_rawcalibratedgyr;
        RowText("Type", "BEE1012 raw + calibrated gyro report");
        RowFmt("Raw Time", "%u.%09u", (unsigned)p.Raw.TimeSeconds, (unsigned)p.Raw.TimeNanoSeconds);
        RowFmt("Raw GYR0", "%f, %f, %f", p.Raw.GYR0RawRateX, p.Raw.GYR0RawRateY, p.Raw.GYR0RawRateZ);
        RowFmt("Raw GYR1", "%f, %f, %f", p.Raw.GYR1RawRateX, p.Raw.GYR1RawRateY, p.Raw.GYR1RawRateZ);
        RowFmt("Raw Valid", "%u, %u", (unsigned)p.Raw.GYR0ValidFlag, (unsigned)p.Raw.GYR1ValidFlag);
        RowFmt("Calibrated Time", "%u.%09u", (unsigned)p.Calibrated.TimeSeconds, (unsigned)p.Calibrated.TimeNanoSeconds);
        RowFmt("Calibrated GYR0", "%f, %f, %f", p.Calibrated.GYR0CalibratedRateX, p.Calibrated.GYR0CalibratedRateY, p.Calibrated.GYR0CalibratedRateZ);
        RowFmt("Calibrated GYR1", "%f, %f, %f", p.Calibrated.GYR1CalibratedRateX, p.Calibrated.GYR1CalibratedRateY, p.Calibrated.GYR1CalibratedRateZ);
        RowFmt("Calibrated External GYR0", "%f, %f, %f", p.Calibrated.ExtGYR0CalibratedRateX, p.Calibrated.ExtGYR0CalibratedRateY, p.Calibrated.ExtGYR0CalibratedRateZ);
        RowFmt("Calibrated External GYR1", "%f, %f, %f", p.Calibrated.ExtGYR1CalibratedRateX, p.Calibrated.ExtGYR1CalibratedRateY, p.Calibrated.ExtGYR1CalibratedRateZ);
        RowFmt("Calibrated Valid", "%u, %u, %u, %u", (unsigned)p.Calibrated.GYR0ValidFlag, (unsigned)p.Calibrated.GYR1ValidFlag, (unsigned)p.Calibrated.EXTGYR0ValidFlag, (unsigned)p.Calibrated.EXTGYR1ValidFlag);
        break;
    }

    case REPORT_KIND_ADCS_GET_RAW_CALIBRATED_RWL:
    {
        const auto &p = view.u.adcs_rawcalibratedrwl;
        RowText("Type", "BEE1012 raw + calibrated reaction-wheel report");
        RowFmt("Raw Time", "%u.%09u", (unsigned)p.Raw.TimeSeconds, (unsigned)p.Raw.TimeNanoSeconds);
        RowFmt("Raw RWL0/1/2/3 Speed", "%f, %f, %f, %f", p.Raw.RWL0MeasuredSpeed, p.Raw.RWL1MeasuredSpeed, p.Raw.RWL2MeasuredSpeed, p.Raw.RWL3MeasuredSpeed);
        RowFmt("Raw Valid", "%u, %u, %u, %u", (unsigned)p.Raw.RWL0ValidFlag, (unsigned)p.Raw.RWL1ValidFlag, (unsigned)p.Raw.RWL2ValidFlag, (unsigned)p.Raw.RWL3ValidFlag);
        RowFmt("Calibrated Time", "%u.%09u", (unsigned)p.Calibrated.TimeSeconds, (unsigned)p.Calibrated.TimeNanoSeconds);
        RowFmt("Wheel Torque X/Y/Z", "%f, %f, %f", p.Calibrated.WhlSBCTrqX, p.Calibrated.WhlSBCTrqY, p.Calibrated.WhlSBCTrqZ);
        RowFmt("Wheel Momentum X/Y/Z", "%f, %f, %f", p.Calibrated.WhlSBCMomX, p.Calibrated.WhlSBCMomY, p.Calibrated.WhlSBCMomZ);
        RowFmt("Calibrated Valid", "%u", (unsigned)p.Calibrated.RWLValidFlag);
        break;
    }


    default:
        return false;
    }
    return true;
}
