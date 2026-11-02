// View - Beacon 창 "Report" 탭: ADCS 로그 마스크·센서 텔레메트리·구성값 리포트
//
// report_tab.cpp 의 리포트 종류 switch 에서 이 종류의 case 를 내용 그대로 옮긴 것.
// 처리한 종류면 true. 행은 report_tab.cpp 의 표(##ReportTable) 안에 그려진다.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"
#include "report_sections.h"

bool DrawAdcsReport(ReportView_t & view)
{
                auto ReportRow = [](const char* label, const char* fmt, auto value)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(label);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(fmt, value);
                };

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
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("UART Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.UARTTlmReturnInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("UART2 Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.UART2TlmReturnInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("CAN Return Interval");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.CANTlmRetrunInterval);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Reserved");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.Reserved);

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

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Seconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeSeconds);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Nanoseconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR0 Raw Rate]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 X");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateX);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Y");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateY);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Z");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR0RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[GYR1 Raw Rate]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 X");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateX);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Y");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateY);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Z");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.GYR1RawRateZ);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR0 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.GYR0ValidFlag,
                                    pl.GYR0ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("GYR1 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.GYR1ValidFlag,
                                    pl.GYR1ValidFlag ? "Valid" : "Invalid");

                        break;
                    }


                    case REPORT_KIND_ADCS_RAW_RW_SENSOR_TLM:
                    {
                        auto &pl = view.u.adcs_raw_rw_sensor;

                        ImGui::TextUnformatted("ADCS2 Raw RW Sensor Telemetry");

                        ImGui::Separator();
                        ImGui::Text("[Time]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Seconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeSeconds);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("Time Nanoseconds");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u", pl.TimeNanoSeconds);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Reaction Wheel Speed]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL0 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL0MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL1 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL1MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL2 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL2MeasSpeed);

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL3 Speed");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.6f", pl.RWL3MeasSpeed);

                        // ========================
                        ImGui::Separator();
                        ImGui::Text("[Valid Flags]");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL0 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL0ValidFlag,
                                    pl.RWL0ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL1 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL1ValidFlag,
                                    pl.RWL1ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL2 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL2ValidFlag,
                                    pl.RWL2ValidFlag ? "Valid" : "Invalid");

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted("RWL3 Valid");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%u (%s)", pl.RWL3ValidFlag,
                                    pl.RWL3ValidFlag ? "Valid" : "Invalid");

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

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl0 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl0MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl1]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl1 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl1MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl2]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl2 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl2MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::Text("[RWl3]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Inertia");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3Inertia);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Max Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3MaxMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Rwl3 Max Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Rwl3MaxToque);

                    //============================================================
                    ImGui::Separator();
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Ramp Torque");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.WheelRampTorque);
                    
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Scheme");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.WheelScheme);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Failed Wheel ID");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.FailedWheelID);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid Nominal Momentum");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.PyramidNominalMomentum);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid Tilt Angle");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.PyramidTiltAngle);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SATELLITE_CONFIG :
                {
                    auto &pl = view.u.adcs_satelliteconfig;

                    ImGui::TextUnformatted("ADCS Satellite Config Telemetry");
                    
                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Inertia]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixx");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixx);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Iyy");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Iyy);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Izz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Izz);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixy");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixy);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ixz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Ixz);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Iyz");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.Iyz);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sun Pointing Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Pointing Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SunPointingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Target Tracking Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Target Tracking Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.TargetTrackingBodyVectorZ);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Sat Tracking Body Vector]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector X");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorX);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector Y");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorY);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sat Tracking Body Vector Z");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.SatTrackingBodyVectorZ);

                    break;
                }

                case REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG :
                {
                    auto &pl = view.u.adcs_controllerconfig;

                    ImGui::TextUnformatted("ADCS Controller Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Detumbling Damping Gain (Kd)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.DetumblingDampingGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Spin Gain Sunlit (KDSun)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunSpinGain_Sunlit);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Spin Gain Eclipse (KDecel)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunSpinGain_Eclipse);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Detumbling Spin Gain (Ks)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.DetumblingSpinGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Fast BD Gain (Kdf)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.FastBDotGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Nutation Damping Gain (Kn)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomNutationDampingGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Nutation Damping Quat Gain (Kq)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomNutationDampingQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Quat Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("XGG Quat Gain (Kqx)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.XGGQuatGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("YGG Quat Gain (Kqy)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YGGQuatGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("ZGG Quat Gain (Kqz)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ZGGQuatGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Control Gains]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel Desat Control Gain (Kh)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.WheelDesatControlGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Proportional Gain (Kp1)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Mom Derivative Gain (Kd1)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YMomDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("R Wheel Proportional Gain (Kp2)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RWheelProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("R Wheel Derivative Gain (Kd2)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RWheelDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Proportional Gain (Kp3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingProportionalGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Derivative Gain (Kd3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingDerivativeGain);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Tracking Integral Gain (Ki3)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.TrackingIntegralGain);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Reference]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reference Spin Rate (deg/s)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ReferenceSpinRate_degps);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reference Wheel Momentum (Nms)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.ReferenceWheelMomentum_Nms);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Y Wheel Bias Momentum (Nms)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.YWheelBiasMomentum_Nms);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ref Spin Rate RW (deg/s)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RefSpinRate_RW_degps);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Angle]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Sun Keep Out Angle (deg)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.SunKeepOutAngle_deg);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Roll Limit Angle (deg)");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.RollLimitAngle_deg);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Yaw Compensation For Earth Rotation");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.YawCompensationForEarthRotation);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Enable Sun Tracking In Eclipse");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.EnableSunTrackingInEclipse);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Enable Sun Avoidance");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.EnableSunAvoidance);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Reserved");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.flags.Reserved);

                    break;

                }

                case REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG :
                {
                    auto &pl = view.u.adcs_defaultmodeconfig;

                    ImGui::TextUnformatted("ADCS Default Mode Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Default Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Run Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultRunMode);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Operational State");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultOperationalState);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode In Op State Safe");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlModeInOpStateSafe);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Control Mode In Op State Auto");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultControlModeInOpStateAuto);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MOUNTING_CONFIG :
                {
                    auto &pl = view.u.adcs_mountingconfig;

                    ImGui::TextUnformatted("ADCS Mounting Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Stack Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack X Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackX_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack Y Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackY_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Stack Z Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.StackZ_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MTQ2_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Wheel Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Wheel3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Wheel3_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Pyramid RWL]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Pyramid RWL Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.PyramidRWL_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[CSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS0 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS0_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS3_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS4 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS4_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS5 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS5_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS6 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS6_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS7 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS7_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS8 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS8_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS9 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.CSS9_mounting);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[FSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS1_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS2 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS2_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS3 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.FSS3_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[HSS Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.HSS1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MAG Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MAG1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[STR Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.STR1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Sensor Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor0 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor0_gamma);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Alpha");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_alpha);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Beta");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_beta);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor1 Gamma");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.ExtSensor1_gamma);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Ext Gyro Mounting]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro0 Axis3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro0_axis3_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis1 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis1_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis2 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis2_mounting);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Gyro1 Axis3 Mounting");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtGyro1_axis3_mounting);

                    break;
                }

            case REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG :
                {
                    auto &pl = view.u.adcs_estimatorconfig;

                    ImGui::TextUnformatted("ADCS Estimator Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Estimator Mode]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Main Estimator Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultMainEstimatorMode);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Default Backup Estimator Mode");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.DefaultBackupEstimatorMode);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Measurement Noise]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MAGMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("CSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.CSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.FSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.HSSMeasurementNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR Measurement Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.STRMeasurementNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[System Noise]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MMTRKF System Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MMTRKFSystemNoise);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("EKF System Noise");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.EKFSystemNoise);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Nutation Correction]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Nutation Epsilon Correction");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.NutationEpsilonCorrection);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Nutation Psi Correction");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.NutationPsiCorrection);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use FSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseFSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use CSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseCSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use HSS in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseHSSinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Use STR in EKF");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.UseSTRinEKF);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Triad Vector1");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.TriadVector1);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Triad Vector2");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.TriadVector2);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Spare");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG :
                {
                    auto &pl = view.u.adcs_nodeselectionconfig;

                    ImGui::TextUnformatted("ADCS Node Selection Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Selection Flags]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("RWL Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.RWLSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MAG Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.MAGSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("FSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.FSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("HSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.HSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("GYR Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.GYRSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("STR Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.STRSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("GNSS Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.GNSSSelectionFlags);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Ext Sensor Selection Flags");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.ExtSensorSelectionFlags);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MTQ_CONFIG :
                {
                    auto &pl = view.u.adcs_mtqconfig;

                    ImGui::TextUnformatted("ADCS MTQ Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ Max Dipole Moment]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ0 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ0MaxDipoleMoment);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ1 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ1MaxDipoleMoment);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("MTQ2 Max Dipole Moment");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MTQ2MaxDipoleMoment);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[MTQ On Time]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Max MTQ On Time");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MaxMTQOnTime);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Min MTQ On Time");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", pl.MinMTQOnTime);

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Filter]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Magnetic Control Filter Factor");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%f", pl.MagneticControlFilterFactor);

                    break;
                }

                case REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG :
                {
                    auto &pl = view.u.adcs_magsensingelmconfig;

                    ImGui::TextUnformatted("ADCS Mag Sensing Element Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Mag Sensing Element]");

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

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Spare");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%u", pl.Spare);

                    break;
                }

                case REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG :
                {
                    auto &pl = view.u.adcs_satorbitparamconfig;

                    ImGui::TextUnformatted("ADCS Sat Orbit Param Config Telemetry");

                    //=========================================================
                    ImGui::Separator();
                    ImGui::Text("[Orbit Parameters]");

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Epoch");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Epoch);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Inclination");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Inclination);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("RAAN");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.RAAN);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Eccentricity");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.Eccentricity);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("AOP");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.AOP);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mean Anomaly");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.MeanAnomaly);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("Mean Motion");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.MeanMotion);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted("B Star Drag");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", pl.B_StarDrag);

                    break;
                }

                /******************************************************************************************************************************************** */


                    default:
                        return false;
                    }
                    return true;
}
