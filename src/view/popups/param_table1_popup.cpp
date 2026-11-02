// View - GS100 파라미터 테이블 1
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

bool popup_param_table1()
{
    mim::vm::Gs100ParamVM & vm = mim::vm::Gs100ParamViewModel();
    Ptable_1 & tbl = vm.Table1();
    ImGui::OpenPopup("GS100 Parameter Table 1");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(720, 540), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("GS100 Parameter Table 1", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::BeginChild("##Ptable1", ImVec2(ImGui::GetContentRegionAvail().x, 650), true, mim_winflags);
        ImGui::PushItemWidth(300);
        mim::ui::RowLabel("Rx Frequency", 11.0f);
        ImGui::InputScalar("##freq", ImGuiDataType_U32, &tbl.freq, NULL, NULL, "%u");
        mim::ui::RowLabel("BaudRate", 11.0f);
        ImGui::InputScalar("##baud", ImGuiDataType_U32, &tbl.baud, NULL, NULL, "%u");
        mim::ui::RowLabel("modindex", 11.0f);
        ImGui::InputScalar("##modeindex", ImGuiDataType_Float, &tbl.modindex, NULL, NULL, "%f");
        mim::ui::RowLabel("Guard", 11.0f);
        ImGui::InputScalar("##guard", ImGuiDataType_U16, &tbl.guard, NULL, NULL, "%u");
        mim::ui::RowLabel("pllrang", 11.0f);
        ImGui::InputScalar("##pllrang", ImGuiDataType_U8, &tbl.pllrang, NULL, NULL, "%u");
        mim::ui::RowLabel("mode", 11.0f);
        ImGui::InputScalar("##mode", ImGuiDataType_U8, &tbl.mode, NULL, NULL, "%u");
        mim::ui::RowLabel("csp_hmac", 11.0f);
        ImGui::Checkbox("##csp_hmac", &tbl.csp_hmac);
        mim::ui::RowLabel("csp_rs", 11.0f);
        ImGui::Checkbox("##csp_rs", &tbl.csp_rs);
        mim::ui::RowLabel("csp_crc", 11.0f);
        ImGui::Checkbox("##csp_crc", &tbl.csp_crc);
        mim::ui::RowLabel("csp_rand", 11.0f);
        ImGui::Checkbox("##csp_rand", &tbl.csp_rand);
        mim::ui::RowLabel("AX25 Call", 11.0f);
        ImGui::InputText("##ax25_call", tbl.ax25_call, sizeof(tbl.ax25_call));
        mim::ui::RowLabel("Bandwidth", 11.0f);
        ImGui::InputScalar("##bw", ImGuiDataType_U32, &tbl.bw, NULL, NULL, "%u");
        mim::ui::RowLabel("AFC Range", 11.0f);
        ImGui::InputScalar("##afcrange", ImGuiDataType_S32, &tbl.afcrange, NULL, NULL, "%d");
        ImGui::PopItemWidth();
        ImGui::EndChild();
        

        if (ImGui::Button("Update", ImVec2(ImGui::GetContentRegionAvail().x * 0.25, 40)))
            vm.Load(1);
        ImGui::SameLine();
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, 40)))
            vm.Save(1);
        ImGui::SameLine();
        if (ImGui::Button("Save to FRAM", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 40)))
        {
            vm.SaveToFram(1);
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
