// View - Beacon 창 "Report" 탭: ADCS 로그 마스크·센서 텔레메트리·구성값 리포트
//
// report_tab.cpp 의 리포트 종류 switch 에서 이 종류의 case 를 내용 그대로 옮긴 것.
// 처리한 종류면 true. 행은 report_tab.cpp 의 표(##ReportTable) 안에 그려진다.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"
#include "report_rows.h"

bool DrawAdcsReport(ReportView_t & view)
{
                    switch (view.kind)
                    {
                    case REPORT_KIND_ADCS_LOG_MASK:
                    {
                        ImGui::TextUnformatted("ADCS Log Inclusion Mask");

                        auto &pl = view.u.adcs_logmask;

                        ImGui::Separator();
                        ImGui::Text("Fast Inclusion Bitmask:");
                        for (int i = 0; i < 5; i++){
                            ImGui::Text("  [%d] : 0x%02X (byte)", i, pl.FastInclusionBitmask[i]);
                            uint8_t b = pl.FastInclusionBitmask[i];
                            ImGui::Text("[%d] 0x%02X  |  b7 b6 b5 b4 b3 b2 b1 b0 = %d%d%d%d%d%d%d%d",
                                i, b,
                                (b>>7)&1, (b>>6)&1, (b>>5)&1, (b>>4)&1,
                                (b>>3)&1, (b>>2)&1, (b>>1)&1, (b>>0)&1 );
                        }


                        ImGui::Separator();
                        ImGui::Text("Slow Inclusion Bitmask:");
                        for (int i = 0; i < 5; i++){
                            ImGui::Text("  [%d] : 0x%02X (byte)", i, pl.SlowInclusionBitmask[i]);
                            uint8_t b = pl.SlowInclusionBitmask[i];
                            ImGui::Text("[%d] 0x%02X  |  b7 b6 b5 b4 b3 b2 b1 b0 = %d%d%d%d%d%d%d%d",
                                i, b,
                                (b>>7)&1, (b>>6)&1, (b>>5)&1, (b>>4)&1,
                                (b>>3)&1, (b>>2)&1, (b>>1)&1, (b>>0)&1 );
                        }


                        break;
                    }


                    case REPORT_KIND_ADCS_UNSOLICIT_TLM_SETUP_TLM:
                    {
                        auto &pl = view.u.adcs_unsolicited_tlm_tlm;

                        ImGui::TextUnformatted("ADCS Unsolicited TLM Message Setup (TLM)");

                        ImGui::Separator();
                        ImGui::Text("[Return Interval Settings]");
                        rpt5::RowFmt("UART Return Interval", "%u", pl.UARTTlmReturnInterval);

                        rpt5::RowFmt("UART2 Return Interval", "%u", pl.UART2TlmReturnInterval);

                        rpt5::RowFmt("CAN Return Interval", "%u", pl.CANTlmRetrunInterval);

                        rpt5::RowFmt("Reserved", "%u", pl.Reserved);

                        ImGui::Separator();


                        
                        auto PrintBitmask = [&](const char *title, const uint8_t mask[5])
                        {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted(title);
                            ImGui::TableSetColumnIndex(1);

                            float line_h = ImGui::GetTextLineHeightWithSpacing();
                            float h = line_h * (5 + 1); // 5줄 + 여유

                            ImGui::BeginChild(title, ImVec2(0, h), true, ImGuiWindowFlags_NoScrollbar);

                            for (int i = 0; i < 5; i++)
                            {
                                uint8_t b = mask[i];

                                char bits[9];
                                for (int k = 0; k < 8; k++)
                                    bits[k] = ((b >> (7 - k)) & 1) ? '1' : '0';
                                bits[8] = 0;

                                ImGui::Text("idx %d: 0x%02X | %s", i, b, bits);
                            }

                            ImGui::EndChild();
                        };


                        PrintBitmask("UART ID Inclusion Bitmask",  pl.UARTTlmIDInclusionBitmask);
                        PrintBitmask("UART2 ID Inclusion Bitmask", pl.UART2TlmIDInclusionBitmask);
                        PrintBitmask("CAN ID Inclusion Bitmask",   pl.CANTlmIDInclusionBitmask);

                        break;
                    }

                    case REPORT_KIND_ADCS_RAW_GYR_SENSOR_TLM:
                    {
                        auto &pl = view.u.adcs_raw_gyr_sensor;

                        ImGui::TextUnformatted("ADCS2 Raw GYR Sensor Telemetry");

                        ImGui::Separator();
                        ImGui::Text("[Time]");

                        rpt5::RowFmt("Time Seconds", "%u", pl.TimeSeconds);

                        rpt5::RowFmt("Time Nanoseconds", "%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR0 Raw Rate]");

                        rpt5::RowFmt("GYR0 X", "%.6f", pl.GYR0RawRateX);

                        rpt5::RowFmt("GYR0 Y", "%.6f", pl.GYR0RawRateY);

                        rpt5::RowFmt("GYR0 Z", "%.6f", pl.GYR0RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR1 Raw Rate]");

                        rpt5::RowFmt("GYR1 X", "%.6f", pl.GYR1RawRateX);

                        rpt5::RowFmt("GYR1 Y", "%.6f", pl.GYR1RawRateY);

                        rpt5::RowFmt("GYR1 Z", "%.6f", pl.GYR1RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        rpt5::RowFmt("GYR0 Valid", "%u (%s)", pl.GYR0ValidFlag, pl.GYR0ValidFlag ? "Valid" : "Invalid");

                        rpt5::RowFmt("GYR1 Valid", "%u (%s)", pl.GYR1ValidFlag, pl.GYR1ValidFlag ? "Valid" : "Invalid");

                        break;
                    }


                    case REPORT_KIND_ADCS_RAW_RW_SENSOR_TLM:
                    {
                        auto &pl = view.u.adcs_raw_rw_sensor;

                        ImGui::TextUnformatted("ADCS2 Raw RW Sensor Telemetry");

                        ImGui::Separator();
                        ImGui::Text("[Time]");

                        rpt5::RowFmt("Time Seconds", "%u", pl.TimeSeconds);

                        rpt5::RowFmt("Time Nanoseconds", "%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Reaction Wheel Speed]");

                        rpt5::RowFmt("RWL0 Speed", "%.6f", pl.RWL0MeasSpeed);

                        rpt5::RowFmt("RWL1 Speed", "%.6f", pl.RWL1MeasSpeed);

                        rpt5::RowFmt("RWL2 Speed", "%.6f", pl.RWL2MeasSpeed);

                        rpt5::RowFmt("RWL3 Speed", "%.6f", pl.RWL3MeasSpeed);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        rpt5::RowFmt("RWL0 Valid", "%u (%s)", pl.RWL0ValidFlag, pl.RWL0ValidFlag ? "Valid" : "Invalid");

                        rpt5::RowFmt("RWL1 Valid", "%u (%s)", pl.RWL1ValidFlag, pl.RWL1ValidFlag ? "Valid" : "Invalid");

                        rpt5::RowFmt("RWL2 Valid", "%u (%s)", pl.RWL2ValidFlag, pl.RWL2ValidFlag ? "Valid" : "Invalid");

                        rpt5::RowFmt("RWL3 Valid", "%u (%s)", pl.RWL3ValidFlag, pl.RWL3ValidFlag ? "Valid" : "Invalid");

                        break;
                    }

                /******************************************************************************************************************************************* */

                case REPORT_KIND_ADCS_GET_WHL_CONFIG :
                {
                    auto &pl = view.u.adcs_whlconfig;

                    ImGui::TextUnformatted("ADCS Whl Config Telemetry");
                    
                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Rwl0]");

                    rpt5::RowFmt("Rwl0 Inertia", "%f", pl.Rwl0Inertia);

                    rpt5::RowFmt("Rwl0 Max Momentum", "%f", pl.Rwl0MaxMomentum);

                    rpt5::RowFmt("Rwl0 Max Torque", "%f", pl.Rwl0MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl1]");

                    rpt5::RowFmt("Rwl1 Inertia", "%f", pl.Rwl1Inertia);

                    rpt5::RowFmt("Rwl1 Max Momentum", "%f", pl.Rwl1MaxMomentum);

                    rpt5::RowFmt("Rwl1 Max Torque", "%f", pl.Rwl1MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl2]");

                    rpt5::RowFmt("Rwl2 Inertia", "%f", pl.Rwl2Inertia);

                    rpt5::RowFmt("Rwl2 Max Momentum", "%f", pl.Rwl2MaxMomentum);

                    rpt5::RowFmt("Rwl2 Max Torque", "%f", pl.Rwl2MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl3]");

                    rpt5::RowFmt("Rwl3 Inertia", "%f", pl.Rwl3Inertia);

                    rpt5::RowFmt("Rwl3 Max Momentum", "%f", pl.Rwl3MaxMomentum);

                    rpt5::RowFmt("Rwl3 Max Torque", "%f", pl.Rwl3MaxToque);

                    //============================================================
                    ImGui::Separator();
                    rpt5::RowFmt("Wheel Ramp Torque", "%f", pl.WheelRampTorque);
                    
                    rpt5::RowFmt("Wheel Scheme", "%u", pl.WheelScheme);

                    rpt5::RowFmt("Failed Wheel ID", "%u", pl.FailedWheelID);

                    rpt5::RowFmt("Pyramid Nominal Momentum", "%f", pl.PyramidNominalMomentum);

                    rpt5::RowFmt("Pyramid Tilt Angle", "%f", pl.PyramidTiltAngle);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SATELLITE_CONFIG :
                {
                    auto &pl = view.u.adcs_satelliteconfig;

                    ImGui::TextUnformatted("ADCS Satellite Config Telemetry");
                    
                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Inertia]");

                    rpt5::RowFmt("Ixx", "%f", pl.Ixx);

                    rpt5::RowFmt("Iyy", "%f", pl.Iyy);

                    rpt5::RowFmt("Izz", "%f", pl.Izz);

                    rpt5::RowFmt("Ixy", "%f", pl.Ixy);

                    rpt5::RowFmt("Ixz", "%f", pl.Ixz);

                    rpt5::RowFmt("Iyz", "%f", pl.Iyz);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sun Pointing Body Vector]");

                    rpt5::RowFmt("Sun Pointing Body Vector X", "%d", pl.SunPointingBodyVectorX);

                    rpt5::RowFmt("Sun Pointing Body Vector Y", "%d", pl.SunPointingBodyVectorY);

                    rpt5::RowFmt("Sun Pointing Body Vector Z", "%d", pl.SunPointingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Target Tracking Body Vector]");

                    rpt5::RowFmt("Target Tracking Body Vector X", "%d", pl.TargetTrackingBodyVectorX);

                    rpt5::RowFmt("Target Tracking Body Vector Y", "%d", pl.TargetTrackingBodyVectorY);

                    rpt5::RowFmt("Target Tracking Body Vector Z", "%d", pl.TargetTrackingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sat Tracking Body Vector]");

                    rpt5::RowFmt("Sat Tracking Body Vector X", "%d", pl.SatTrackingBodyVectorX);

                    rpt5::RowFmt("Sat Tracking Body Vector Y", "%d", pl.SatTrackingBodyVectorY);

                    rpt5::RowFmt("Sat Tracking Body Vector Z", "%d", pl.SatTrackingBodyVectorZ);

                    break;
                }

                case REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG :
                {
                    auto &pl = view.u.adcs_controllerconfig;

                    ImGui::TextUnformatted("ADCS Controller Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Mode]");

                    rpt5::RowFmt("Default Control Mode", "%u", pl.DefaultControlMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Gains]");

                    rpt5::RowFmt("Detumbling Damping Gain (Kd)", "%f", pl.DetumblingDampingGain);

                    rpt5::RowFmt("Sun Spin Gain Sunlit (KDSun)", "%f", pl.SunSpinGain_Sunlit);

                    rpt5::RowFmt("Sun Spin Gain Eclipse (KDecel)", "%f", pl.SunSpinGain_Eclipse);

                    rpt5::RowFmt("Detumbling Spin Gain (Ks)", "%f", pl.DetumblingSpinGain);

                    rpt5::RowFmt("Fast BD Gain (Kdf)", "%f", pl.FastBDotGain);

                    rpt5::RowFmt("Y Mom Nutation Damping Gain (Kn)", "%f", pl.YMomNutationDampingGain);

                    rpt5::RowFmt("Y Mom Nutation Damping Quat Gain (Kq)", "%f", pl.YMomNutationDampingQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Quat Gains]");

                    rpt5::RowFmt("XGG Quat Gain (Kqx)", "%f", pl.XGGQuatGain);

                    rpt5::RowFmt("YGG Quat Gain (Kqy)", "%f", pl.YGGQuatGain);

                    rpt5::RowFmt("ZGG Quat Gain (Kqz)", "%f", pl.ZGGQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Gains]");

                    rpt5::RowFmt("Wheel Desat Control Gain (Kh)", "%f", pl.WheelDesatControlGain);

                    rpt5::RowFmt("Y Mom Proportional Gain (Kp1)", "%f", pl.YMomProportionalGain);

                    rpt5::RowFmt("Y Mom Derivative Gain (Kd1)", "%f", pl.YMomDerivativeGain);

                    rpt5::RowFmt("R Wheel Proportional Gain (Kp2)", "%f", pl.RWheelProportionalGain);

                    rpt5::RowFmt("R Wheel Derivative Gain (Kd2)", "%f", pl.RWheelDerivativeGain);

                    rpt5::RowFmt("Tracking Proportional Gain (Kp3)", "%f", pl.TrackingProportionalGain);

                    rpt5::RowFmt("Tracking Derivative Gain (Kd3)", "%f", pl.TrackingDerivativeGain);

                    rpt5::RowFmt("Tracking Integral Gain (Ki3)", "%f", pl.TrackingIntegralGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Reference]");

                    rpt5::RowFmt("Reference Spin Rate (deg/s)", "%f", pl.ReferenceSpinRate_degps);

                    rpt5::RowFmt("Reference Wheel Momentum (Nms)", "%f", pl.ReferenceWheelMomentum_Nms);

                    rpt5::RowFmt("Y Wheel Bias Momentum (Nms)", "%f", pl.YWheelBiasMomentum_Nms);

                    rpt5::RowFmt("Ref Spin Rate RW (deg/s)", "%f", pl.RefSpinRate_RW_degps);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Angle]");

                    rpt5::RowFmt("Sun Keep Out Angle (deg)", "%f", pl.SunKeepOutAngle_deg);

                    rpt5::RowFmt("Roll Limit Angle (deg)", "%f", pl.RollLimitAngle_deg);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    rpt5::RowFmt("Yaw Compensation For Earth Rotation", "%u", pl.flags.YawCompensationForEarthRotation);

                    rpt5::RowFmt("Enable Sun Tracking In Eclipse", "%u", pl.flags.EnableSunTrackingInEclipse);

                    rpt5::RowFmt("Enable Sun Avoidance", "%u", pl.flags.EnableSunAvoidance);

                    rpt5::RowFmt("Reserved", "%u", pl.flags.Reserved);

                    break;

                }

                case REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG :
                {
                    auto &pl = view.u.adcs_defaultmodeconfig;

                    ImGui::TextUnformatted("ADCS Default Mode Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Default Mode]");

                    rpt5::RowFmt("Default Run Mode", "%u", pl.DefaultRunMode);

                    rpt5::RowFmt("Default Operational State", "%u", pl.DefaultOperationalState);

                    rpt5::RowFmt("Default Control Mode In Op State Safe", "%u", pl.DefaultControlModeInOpStateSafe);

                    rpt5::RowFmt("Default Control Mode In Op State Auto", "%u", pl.DefaultControlModeInOpStateAuto);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MOUNTING_CONFIG :
                {
                    auto &pl = view.u.adcs_mountingconfig;

                    ImGui::TextUnformatted("ADCS Mounting Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Stack Mounting]");

                    rpt5::RowFmt("Stack X Mounting", "%u", pl.StackX_mounting);

                    rpt5::RowFmt("Stack Y Mounting", "%u", pl.StackY_mounting);

                    rpt5::RowFmt("Stack Z Mounting", "%u", pl.StackZ_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Mounting]");

                    rpt5::RowFmt("MTQ0 Mounting", "%u", pl.MTQ0_mounting);

                    rpt5::RowFmt("MTQ1 Mounting", "%u", pl.MTQ1_mounting);

                    rpt5::RowFmt("MTQ2 Mounting", "%u", pl.MTQ2_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Wheel Mounting]");

                    rpt5::RowFmt("Wheel0 Mounting", "%u", pl.Wheel0_mounting);

                    rpt5::RowFmt("Wheel1 Mounting", "%u", pl.Wheel1_mounting);

                    rpt5::RowFmt("Wheel2 Mounting", "%u", pl.Wheel2_mounting);

                    rpt5::RowFmt("Wheel3 Mounting", "%u", pl.Wheel3_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Pyramid RWL]");

                    rpt5::RowFmt("Pyramid RWL Alpha", "%d", pl.PyramidRWL_alpha);

                    rpt5::RowFmt("Pyramid RWL Beta", "%d", pl.PyramidRWL_beta);

                    rpt5::RowFmt("Pyramid RWL Gamma", "%d", pl.PyramidRWL_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[CSS Mounting]");

                    rpt5::RowFmt("CSS0 Mounting", "%u", pl.CSS0_mounting);

                    rpt5::RowFmt("CSS1 Mounting", "%u", pl.CSS1_mounting);

                    rpt5::RowFmt("CSS2 Mounting", "%u", pl.CSS2_mounting);

                    rpt5::RowFmt("CSS3 Mounting", "%u", pl.CSS3_mounting);

                    rpt5::RowFmt("CSS4 Mounting", "%u", pl.CSS4_mounting);

                    rpt5::RowFmt("CSS5 Mounting", "%u", pl.CSS5_mounting);

                    rpt5::RowFmt("CSS6 Mounting", "%u", pl.CSS6_mounting);

                    rpt5::RowFmt("CSS7 Mounting", "%u", pl.CSS7_mounting);

                    rpt5::RowFmt("CSS8 Mounting", "%u", pl.CSS8_mounting);

                    rpt5::RowFmt("CSS9 Mounting", "%u", pl.CSS9_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[FSS Mounting]");

                    rpt5::RowFmt("FSS0 Alpha", "%d", pl.FSS0_alpha);

                    rpt5::RowFmt("FSS0 Beta", "%d", pl.FSS0_beta);

                    rpt5::RowFmt("FSS0 Gamma", "%d", pl.FSS0_gamma);

                    rpt5::RowFmt("FSS1 Alpha", "%d", pl.FSS1_alpha);

                    rpt5::RowFmt("FSS1 Beta", "%d", pl.FSS1_beta);

                    rpt5::RowFmt("FSS1 Gamma", "%d", pl.FSS1_gamma);

                    rpt5::RowFmt("FSS2 Alpha", "%d", pl.FSS2_alpha);

                    rpt5::RowFmt("FSS2 Beta", "%d", pl.FSS2_beta);

                    rpt5::RowFmt("FSS2 Gamma", "%d", pl.FSS2_gamma);

                    rpt5::RowFmt("FSS3 Alpha", "%d", pl.FSS3_alpha);

                    rpt5::RowFmt("FSS3 Beta", "%d", pl.FSS3_beta);

                    rpt5::RowFmt("FSS3 Gamma", "%d", pl.FSS3_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[HSS Mounting]");

                    rpt5::RowFmt("HSS0 Alpha", "%d", pl.HSS0_alpha);

                    rpt5::RowFmt("HSS0 Beta", "%d", pl.HSS0_beta);

                    rpt5::RowFmt("HSS0 Gamma", "%d", pl.HSS0_gamma);

                    rpt5::RowFmt("HSS1 Alpha", "%d", pl.HSS1_alpha);

                    rpt5::RowFmt("HSS1 Beta", "%d", pl.HSS1_beta);

                    rpt5::RowFmt("HSS1 Gamma", "%d", pl.HSS1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MAG Mounting]");

                    rpt5::RowFmt("MAG0 Alpha", "%d", pl.MAG0_alpha);

                    rpt5::RowFmt("MAG0 Beta", "%d", pl.MAG0_beta);

                    rpt5::RowFmt("MAG0 Gamma", "%d", pl.MAG0_gamma);

                    rpt5::RowFmt("MAG1 Alpha", "%d", pl.MAG1_alpha);

                    rpt5::RowFmt("MAG1 Beta", "%d", pl.MAG1_beta);

                    rpt5::RowFmt("MAG1 Gamma", "%d", pl.MAG1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[STR Mounting]");

                    rpt5::RowFmt("STR0 Alpha", "%d", pl.STR0_alpha);

                    rpt5::RowFmt("STR0 Beta", "%d", pl.STR0_beta);

                    rpt5::RowFmt("STR0 Gamma", "%d", pl.STR0_gamma);

                    rpt5::RowFmt("STR1 Alpha", "%d", pl.STR1_alpha);

                    rpt5::RowFmt("STR1 Beta", "%d", pl.STR1_beta);

                    rpt5::RowFmt("STR1 Gamma", "%d", pl.STR1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Sensor Mounting]");

                    rpt5::RowFmt("Ext Sensor0 Alpha", "%d", pl.ExtSensor0_alpha);

                    rpt5::RowFmt("Ext Sensor0 Beta", "%d", pl.ExtSensor0_beta);

                    rpt5::RowFmt("Ext Sensor0 Gamma", "%d", pl.ExtSensor0_gamma);

                    rpt5::RowFmt("Ext Sensor1 Alpha", "%d", pl.ExtSensor1_alpha);

                    rpt5::RowFmt("Ext Sensor1 Beta", "%d", pl.ExtSensor1_beta);

                    rpt5::RowFmt("Ext Sensor1 Gamma", "%d", pl.ExtSensor1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Gyro Mounting]");

                    rpt5::RowFmt("Ext Gyro0 Axis1 Mounting", "%u", pl.ExtGyro0_axis1_mounting);

                    rpt5::RowFmt("Ext Gyro0 Axis2 Mounting", "%u", pl.ExtGyro0_axis2_mounting);

                    rpt5::RowFmt("Ext Gyro0 Axis3 Mounting", "%u", pl.ExtGyro0_axis3_mounting);

                    rpt5::RowFmt("Ext Gyro1 Axis1 Mounting", "%u", pl.ExtGyro1_axis1_mounting);

                    rpt5::RowFmt("Ext Gyro1 Axis2 Mounting", "%u", pl.ExtGyro1_axis2_mounting);

                    rpt5::RowFmt("Ext Gyro1 Axis3 Mounting", "%u", pl.ExtGyro1_axis3_mounting);

                    break;
                }

            case REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG :
                {
                    auto &pl = view.u.adcs_estimatorconfig;

                    ImGui::TextUnformatted("ADCS Estimator Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Estimator Mode]");

                    rpt5::RowFmt("Default Main Estimator Mode", "%u", pl.DefaultMainEstimatorMode);

                    rpt5::RowFmt("Default Backup Estimator Mode", "%u", pl.DefaultBackupEstimatorMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Measurement Noise]");

                    rpt5::RowFmt("MAG Measurement Noise", "%f", pl.MAGMeasurementNoise);

                    rpt5::RowFmt("CSS Measurement Noise", "%f", pl.CSSMeasurementNoise);

                    rpt5::RowFmt("FSS Measurement Noise", "%f", pl.FSSMeasurementNoise);

                    rpt5::RowFmt("HSS Measurement Noise", "%f", pl.HSSMeasurementNoise);

                    rpt5::RowFmt("STR Measurement Noise", "%f", pl.STRMeasurementNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[System Noise]");

                    rpt5::RowFmt("MMTRKF System Noise", "%f", pl.MMTRKFSystemNoise);

                    rpt5::RowFmt("EKF System Noise", "%f", pl.EKFSystemNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Nutation Correction]");

                    rpt5::RowFmt("Nutation Epsilon Correction", "%f", pl.NutationEpsilonCorrection);

                    rpt5::RowFmt("Nutation Psi Correction", "%f", pl.NutationPsiCorrection);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    rpt5::RowFmt("Use FSS in EKF", "%u", pl.UseFSSinEKF);

                    rpt5::RowFmt("Use CSS in EKF", "%u", pl.UseCSSinEKF);

                    rpt5::RowFmt("Use HSS in EKF", "%u", pl.UseHSSinEKF);

                    rpt5::RowFmt("Use STR in EKF", "%u", pl.UseSTRinEKF);

                    rpt5::RowFmt("Triad Vector1", "%u", pl.TriadVector1);

                    rpt5::RowFmt("Triad Vector2", "%u", pl.TriadVector2);

                    rpt5::RowFmt("Spare", "%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG :
                {
                    auto &pl = view.u.adcs_nodeselectionconfig;

                    ImGui::TextUnformatted("ADCS Node Selection Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Selection Flags]");

                    rpt5::RowFmt("RWL Selection Flags", "%u", pl.RWLSelectionFlags);

                    rpt5::RowFmt("MAG Selection Flags", "%u", pl.MAGSelectionFlags);

                    rpt5::RowFmt("FSS Selection Flags", "%u", pl.FSSSelectionFlags);

                    rpt5::RowFmt("HSS Selection Flags", "%u", pl.HSSSelectionFlags);

                    rpt5::RowFmt("GYR Selection Flags", "%u", pl.GYRSelectionFlags);

                    rpt5::RowFmt("STR Selection Flags", "%u", pl.STRSelectionFlags);

                    rpt5::RowFmt("GNSS Selection Flags", "%u", pl.GNSSSelectionFlags);

                    rpt5::RowFmt("Ext Sensor Selection Flags", "%u", pl.ExtSensorSelectionFlags);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MTQ_CONFIG :
                {
                    auto &pl = view.u.adcs_mtqconfig;

                    ImGui::TextUnformatted("ADCS MTQ Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Max Dipole Moment]");

                    rpt5::RowFmt("MTQ0 Max Dipole Moment", "%f", pl.MTQ0MaxDipoleMoment);

                    rpt5::RowFmt("MTQ1 Max Dipole Moment", "%f", pl.MTQ1MaxDipoleMoment);

                    rpt5::RowFmt("MTQ2 Max Dipole Moment", "%f", pl.MTQ2MaxDipoleMoment);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ On Time]");

                    rpt5::RowFmt("Max MTQ On Time", "%d", pl.MaxMTQOnTime);

                    rpt5::RowFmt("Min MTQ On Time", "%d", pl.MinMTQOnTime);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Filter]");

                    rpt5::RowFmt("Magnetic Control Filter Factor", "%f", pl.MagneticControlFilterFactor);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG :
                {
                    auto &pl = view.u.adcs_magsensingelmconfig;

                    ImGui::TextUnformatted("ADCS Mag Sensing Element Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Mag Sensing Element]");

                    rpt5::RowFmt("Mag0 Sensing Element", "%u", pl.Mag0SensingElement);

                    rpt5::RowFmt("Mag1 Sensing Element", "%u", pl.Mag1SensingElement);

                    rpt5::RowFmt("Spare", "%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG :
                {
                    auto &pl = view.u.adcs_satorbitparamconfig;

                    ImGui::TextUnformatted("ADCS Sat Orbit Param Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Orbit Parameters]");

                    rpt5::RowFmt("Epoch", "%lf", pl.Epoch);

                    rpt5::RowFmt("Inclination", "%lf", pl.Inclination);

                    rpt5::RowFmt("RAAN", "%lf", pl.RAAN);

                    rpt5::RowFmt("Eccentricity", "%lf", pl.Eccentricity);

                    rpt5::RowFmt("AOP", "%lf", pl.AOP);

                    rpt5::RowFmt("Mean Anomaly", "%lf", pl.MeanAnomaly);

                    rpt5::RowFmt("Mean Motion", "%lf", pl.MeanMotion);

                    rpt5::RowFmt("B Star Drag", "%lf", pl.B_StarDrag);

                    break;
                }

                /******************************************************************************************************************************************** */


                    default:
                        return false;
                    }
                    return true;
}
