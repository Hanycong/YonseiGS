// View - Beacon 창 "Beacon" 탭
//
// ImGui 호출과 BeaconVM 호출만 있다. 표시 값은 viewmodel/beacon_vm.h 참고.
#include <inttypes.h>

#include "imgui.h"
#include "beacon_vm.h"
#include "beacon_tabs.h"

// 표 머리글 고정 행/열 수 (YonseiGS_imgui.cpp 의 같은 이름 static 과 같은 값)
static int freeze_cols = 1;
static int freeze_rows = 1;

void DrawBeaconTab(float fontscale)
{
    mim::vm::BeaconVM & vm = mim::vm::BeaconViewModel();
    Beacon *beacon = vm.LastBeacon();
    if(ImGui::BeginTabItem("Beacon"))
    {

        if (ImGui::BeginTable("##BeaconTables", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))


    ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
    ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Data",      ImGuiTableColumnFlags_NoHide, 0.0f);
   
    ImGui::TableHeadersRow();


    if (!beacon)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Beacon");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("No data");
    }
    else
    {
        char buf[128];


        // =========================
        //  CCSDS HEADER
        // =========================
        BeaconSectionHeader("CCSDS Header");
        StateWindowColumnManager("CCSDS MsgID");
        ImGui::Text("0x%02X 0x%02X", beacon->CCSDS_MID[0], beacon->CCSDS_MID[1]);

        StateWindowColumnManager("CCSDS Sequence");
        ImGui::Text("0x%02X 0x%02X", beacon->CCSDS_Seq[0], beacon->CCSDS_Seq[1]);

        StateWindowColumnManager("CCSDS Length");
        ImGui::Text("0x%02X 0x%02X", beacon->CCSDS_Len[0], beacon->CCSDS_Len[1]);

        StateWindowColumnManager("CCSDS Time Code");
        ImGui::Text("0x%02X %02X %02X %02X %02X %02X",
                    beacon->CCSDS_TimeCode[0], beacon->CCSDS_TimeCode[1],
                    beacon->CCSDS_TimeCode[2], beacon->CCSDS_TimeCode[3],
                    beacon->CCSDS_TimeCode[4], beacon->CCSDS_TimeCode[5]);

        // =========================
        //  FSW - RPT
        // =========================


        BeaconSectionHeader("FSW - RPT");

        StateWindowColumnManager("RPT Command Counter");
        ImGui::Text("%" PRIu8, beacon->RPT_CmdCounter);

        StateWindowColumnManager("RPT Error Counter");
        ImGui::Text("%" PRIu8, beacon->RPT_ErrCounter);

        StateWindowColumnManager("RPT Report Count");
        ImGui::Text("%" PRIu8, beacon->RPT_ReportCnt);

        StateWindowColumnManager("RPT Critical Count");
        ImGui::Text("%" PRIu8, beacon->RPT_CriticalCnt);

        StateWindowColumnManager("RPT Boot Count");
        ImGui::Text("%" PRIu16, beacon->RPT_BootCount);

        StateWindowColumnManager("RPT Sc Time (sec)");
        ImGui::Text("%" PRIu32, beacon->RPT_ScTimeSec);

        StateWindowColumnManager("RPT Sc Time (subsec)");
        ImGui::Text("%" PRIu32, beacon->RPT_ScTimeSubsec);

        StateWindowColumnManager("RPT Sequence LSB");
        ImGui::Text("%" PRIu8, beacon->RPT_Sequence_LSB);

        // =========================
        //  COMS - STX (S-band Tx)
        // =========================
        BeaconSectionHeader("COMS - STX");
        StateWindowColumnManager("STX Symbol Rate");
        ImGui::Text("%" PRIu8, beacon->STX_symbol_rate);

        StateWindowColumnManager("STX Transmit Power");
        ImGui::Text("%" PRIu8, beacon->STX_transmit_power);

        StateWindowColumnManager("STX MODCOD");
        ImGui::Text("%" PRIu8, beacon->STX_modcod);

        StateWindowColumnManager("STX Roll-off");
        ImGui::Text("%" PRIu8, beacon->STX_roll_off);

        StateWindowColumnManager("STX Pilot Signal");
        ImGui::Text("%" PRIu8, beacon->STX_pilot_signal);

        StateWindowColumnManager("STX FEC Frame Size");
        ImGui::Text("%" PRIu8, beacon->STX_fec_frame_size);

        StateWindowColumnManager("STX Pre-Transmission Delay");
        ImGui::Text("%" PRIu16, beacon->STX_pretransmission_delay);

        StateWindowColumnManager("STX Center Frequency");
        ImGui::Text("%.3f", beacon->STX_center_frequency);

        StateWindowColumnManager("STX Modulator IF Type");
        ImGui::Text("%" PRIu8, beacon->STX_modulator_interface_type);

        StateWindowColumnManager("STX LVDS IO Type");
        ImGui::Text("%" PRIu8, beacon->STX_lvds_io_type);

        StateWindowColumnManager("STX System State");
        ImGui::Text("%" PRIu8, beacon->STX_SystemState);

        StateWindowColumnManager("STX Status Flag");
        ImGui::Text("%" PRIu8, beacon->STX_StatusFlag);

        // StateWindowColumnManager("STX CPU Temperature");
        // ImGui::Text("%.2f", beacon->STX_CpuTemp);

        // =========================
        //  COMS - UANT
        // =========================
        BeaconSectionHeader("COMS - UANT");
        StateWindowColumnManager("UANT1 Channels (0,1,Backup)");
        ImGui::Text("%" PRIu8 " %" PRIu8 " (Backup:%" PRIu8 ")",
                    beacon->UANT1_Chan0, beacon->UANT1_Chan1, beacon->UANT1_BackupActive);

        StateWindowColumnManager("UANT2 Channels (0,1,Backup)");
        ImGui::Text("%" PRIu8 " %" PRIu8 " (Backup:%" PRIu8 ")",
                    beacon->UANT2_Chan0, beacon->UANT2_Chan1, beacon->UANT2_BackupActive);

        // =========================
        //  COMS - UTRX
        // =========================
        BeaconSectionHeader("COMS - UTRX");
        StateWindowColumnManager("UTRX Active Config");
        ImGui::Text("%" PRIu8, beacon->UTRX_ActiveConf);

        StateWindowColumnManager("UTRX Boot Count");
        ImGui::Text("%" PRIu16, beacon->UTRX_BootCount);

        StateWindowColumnManager("UTRX Boot Cause");
        ImGui::Text("%" PRIu32, beacon->UTRX_BootCause);

        StateWindowColumnManager("UTRX Board Temperature");
        ImGui::Text("%" PRId16, beacon->UTRX_BoardTemp);

        // =========================
        //  PCDU - P60 Dock
        // =========================
        BeaconSectionHeader("EPS - P60 Dock");
        StateWindowColumnManager("P60 Dock Cout [0..2]");
        ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16,
                    beacon->P60D_Cout[0], beacon->P60D_Cout[1], beacon->P60D_Cout[2]);

        StateWindowColumnManager("P60 Dock Vout [0..2]");
        ImGui::Text("%" PRIu16 " %" PRIu16 " %" PRIu16,
                    beacon->P60D_Vout[0], beacon->P60D_Vout[1], beacon->P60D_Vout[2]);

        StateWindowColumnManager("P60 Dock OutEn (bitmask)");
        ImGui::Text("0x%04X", beacon->P60D_OutEn);

        StateWindowColumnManager("P60 Dock Boot Cause");
        ImGui::Text("%" PRIu32, beacon->P60D_BootCause);

        StateWindowColumnManager("P60 Dock Boot Count");
        ImGui::Text("%" PRIu32, beacon->P60D_BootCount);

        StateWindowColumnManager("P60 Dock Batt Mode");
        ImGui::Text("%" PRIu8, beacon->P60D_BattMode);

        StateWindowColumnManager("P60 Dock Heater On");
        ImGui::Text("%" PRIu8, beacon->P60D_HeaterOn);

        StateWindowColumnManager("P60 Dock VBAT Voltage");
        ImGui::Text("%" PRIu16, beacon->P60D_VbatV);

        StateWindowColumnManager("P60 Dock VCC Current");
        ImGui::Text("%" PRId16, beacon->P60D_VccC);

        StateWindowColumnManager("P60 Dock Batt Voltage");
        ImGui::Text("%" PRIu16, beacon->P60D_BattV);

        StateWindowColumnManager("P60 Dock Batt Temp[0..1]");
        ImGui::Text("%" PRId16 " %" PRId16,
                    beacon->P60D_BattTemp[0], beacon->P60D_BattTemp[1]);

        StateWindowColumnManager("P60 Dock WDT CAN Left");
        ImGui::Text("%" PRIu32, beacon->P60D_WdtCanLeft);

        StateWindowColumnManager("P60 Dock Batt Charge Current");
        ImGui::Text("%" PRId16, beacon->P60D_BattChrg);

        StateWindowColumnManager("P60 Dock Batt Discharge Current");
        ImGui::Text("%" PRId16, beacon->P60D_BattDischrg);

        // =========================
        //  PCDU - P60 PDU
        // =========================
        BeaconSectionHeader("EPS - P60 PDU");
        StateWindowColumnManager("P60 PDU Cout[0..8]");
        ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16,
                    beacon->P60P_Cout[0], beacon->P60P_Cout[1], beacon->P60P_Cout[2], beacon->P60P_Cout[3], beacon->P60P_Cout[4], beacon->P60P_Cout[5],
                beacon->P60P_Cout[6], beacon->P60P_Cout[7], beacon->P60P_Cout[8]);

        StateWindowColumnManager("P60 PDU Vout[0..8]");
        ImGui::Text("%" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16,
                    beacon->P60P_Vout[0], beacon->P60P_Vout[1], beacon->P60P_Vout[2], beacon->P60P_Vout[3], beacon->P60P_Vout[4], beacon->P60P_Vout[5], 
                beacon->P60P_Vout[6], beacon->P60P_Vout[7], beacon->P60P_Vout[8]);

        StateWindowColumnManager("P60 PDU Vcc");
        ImGui::Text("%" PRId16, beacon->P60P_Vcc);

        StateWindowColumnManager("P60 PDU ConvEn");
        ImGui::Text("%" PRIu8, beacon->P60P_ConvEn);

        StateWindowColumnManager("P60 PDU OutEn (bitmask)");
        ImGui::Text("0x%04X", beacon->P60P_OutEn);

        // =========================
        //  PCDU - P60 ACU
        // =========================
        BeaconSectionHeader("EPS - P60 ACU");
        StateWindowColumnManager("P60 ACU Cin[0..5]");
        ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16,
                    beacon->P60A_Cin[0], beacon->P60A_Cin[1],
                    beacon->P60A_Cin[2], beacon->P60A_Cin[3], 
                    beacon->P60A_Cin[4], beacon->P60A_Cin[5]);

        StateWindowColumnManager("P60 ACU Vin[0..5]");
        ImGui::Text("%" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16,
                    beacon->P60A_Vin[0], beacon->P60A_Vin[1],
                    beacon->P60A_Vin[2], beacon->P60A_Vin[3], 
                    beacon->P60A_Vin[4], beacon->P60A_Vin[5]);

        // =========================
        //  ADCS
        // =========================
        BeaconSectionHeader("ADCS");
        StateWindowColumnManager("ADCS Power State (bitfield)");
        ImGui::Text("0x%02X", beacon->ADCS_PowerState);

        StateWindowColumnManager("ADCS Control Mode");
        ImGui::Text("%" PRIu8, beacon->ADCS_ControlMode);

        StateWindowColumnManager("ADCS GYR0 Calib Rate X");
        ImGui::Text("%.6f", beacon->ADCS_GYR0CalibratedRateXComponent);

        StateWindowColumnManager("ADCS GYR0 Calib Rate Y");
        ImGui::Text("%.6f", beacon->ADCS_GYR0CalibratedRateYComponent);

        StateWindowColumnManager("ADCS GYR0 Calib Rate Z");
        ImGui::Text("%.6f", beacon->ADCS_GYR0CalibratedRateZComponent);
    }


        ImGui::EndTable();
        ImGui::EndTabItem();
        ImGui::SetWindowFontScale(1.0 * fontscale);
    }
}
