// View - GS100 파라미터 테이블 0
//
// YonseiGS_imgui.cpp 에서 내용 그대로 옮긴 것 (팝업 View 추출).
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "YonseiGS_config.h"
#include "ui_kit.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_radial.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_model.h"
#include "YonseiGS_s_control.h"
#include "YonseiGS_ftp.h"
#include "YonseiGS_autopilot.h"

#include "YonseiGS_theme.h"
#include "YonseiGS_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "popups_common.h"
#include "gs100_param_vm.h"

using namespace mim;

bool popup_param_table0()
{
    mim::vm::Gs100ParamVM & vm = mim::vm::Gs100ParamViewModel();
    Ptable_0 & tbl = vm.Table0();
    ImGui::OpenPopup("GS100 Parameter Table 0");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("GS100 Parameter Table 0", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::BeginChild("##Ptable0", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::PushItemWidth(300);
        mim::ui::RowLabel("RSSI Offset", 11.0f);
        ImGui::InputScalar("##rssi_offset", ImGuiDataType_S8, &tbl.rssi_offset, NULL, NULL, "%d");
        mim::ui::RowLabel("Max Temperature", 11.0f);
        ImGui::InputScalar("°C##max_temp", ImGuiDataType_S16, &tbl.max_temp, NULL, NULL, "%d");
        mim::ui::RowLabel("bgndrssl_ema", 11.0f);
        ImGui::InputScalar("##bgndrssl_ema", ImGuiDataType_Float, &tbl.bgndrssl_ema);
        mim::ui::RowLabel("CSP Node", 11.0f);
        ImGui::InputScalar("##csp_node", ImGuiDataType_U8, &tbl.csp_node, NULL, NULL, "%u");
        mim::ui::RowLabel("I2C", 11.0f);
        ImGui::Checkbox("##i2c_enable", &tbl.i2c_en);
        mim::ui::RowLabel("CAN", 11.0f);
        ImGui::Checkbox("##can_enable", &tbl.can_en);
        mim::ui::RowLabel("EXTPTT", 11.0f);
        ImGui::Checkbox("##extptt_enable", &tbl.extptt_en);
        ImGui::SameLine();
        ImGui::InputScalar("##max_temp", ImGuiDataType_S8, &tbl.max_temp, NULL, NULL, "%d");
        mim::ui::RowLabel("I2C Address", 11.0f);
        ImGui::InputScalar("##i2c_addr", ImGuiDataType_U8, &tbl.i2c_addr, NULL, NULL, "%u");
        mim::ui::RowLabel("I2C Frequency", 11.0f);
        ImGui::InputScalar("kHz##i2c_khz", ImGuiDataType_U16, &tbl.i2c_khz, NULL, NULL, "%u");
        mim::ui::RowLabel("CAN Frequency", 11.0f);
        ImGui::InputScalar("kHz##can_khz", ImGuiDataType_U16, &tbl.can_khz, NULL, NULL, "%u");
        mim::ui::RowLabel("Reboot in", 11.0f);
        ImGui::InputScalar("##reboot_in", ImGuiDataType_U16, &tbl.reboot_in, NULL, NULL, "%u");
        mim::ui::RowLabel("TX Inhibit", 11.0f);
        ImGui::InputScalar("##tx_inhibit", ImGuiDataType_U32, &tbl.tx_inhibit, NULL, NULL, "%u");
        mim::ui::RowLabel("Store Log", 11.0f);
        ImGui::InputScalar("##log_store", ImGuiDataType_U8, &tbl.log_store, NULL, NULL, "%u");
        mim::ui::RowLabel("TX Power", 11.0f);
        ImGui::InputScalar("##tx_power", ImGuiDataType_U8, &tbl.tx_pwr, NULL, NULL, "%d");
        mim::ui::RowLabel("Maximum TX Time", 11.0f);
        ImGui::InputScalar("second##max_tx_time", ImGuiDataType_U16, &tbl.max_tx_time, NULL, NULL, "%u");
        mim::ui::RowLabel("Maximum IDLE Time", 11.0f);
        ImGui::InputScalar("second##max_idle_time", ImGuiDataType_U16, &tbl.max_idle_time, NULL, NULL, "%u");
        mim::ui::RowLabel("CSP Routing Table", 11.0f);
        ImGui::InputText("##csp_rtable", tbl.csp_rtable, sizeof(tbl.csp_rtable));
        ImGui::PopItemWidth();
        ImGui::EndChild();

        if (ImGui::Button("Update", ImVec2(ImGui::GetContentRegionAvail().x * 0.25, 40)))
            vm.Load(0);
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, 40)))
            vm.Save(0);
        ImGui::SameLine();
        if (ImGui::Button("Save to FRAM", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40)))
        {
            vm.SaveToFram(0);
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        
        ImGui::EndPopup();
        return true;
    }
    return true;
}
