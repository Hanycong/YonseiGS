// View - TLE 팝업 (TLE 목록/불러오기)
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
#include "ui_kit.h"
#include "tle_vm.h"

using namespace mim;

bool popup_tle()
{
    mim::vm::TleVM & vm = mim::vm::TleViewModel();
    ImGui::OpenPopup("TLE Manager");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1080, 720), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("TLE Manager", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        if (ImGui::BeginCombo("##TLECombobox", vm.SelectedListLabel()))
        {
            for (int TLEindex = 0; TLEindex < vm.ListCapacity(); TLEindex++)
            {
                if (!vm.ListLabel(TLEindex))
                    break;
                bool SelectedTLE = (*vm.SelectedList() == TLEindex);
                char Selectedlabel[64];
                strcpy(Selectedlabel, vm.ListLabel(TLEindex));
                if (ImGui::Selectable(Selectedlabel, SelectedTLE))
                {
                    *vm.SelectedList() = TLEindex;
                }
                if (SelectedTLE)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        ImGui::InputText("##", vm.SearchText(), vm.SearchTextSize());
        ImGui::SameLine();
        ImGui::Text("SGP4");
        ImGui::SameLine();
        ImGui::Checkbox("##sgp4checkbox", vm.Sgp4Only());

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6);
        ImGui::InputText("##localtleimport", vm.LocalPath(), vm.LocalPathSize(), 0, NULL, NULL);
        ImGui::SameLine();
        if(ImGui::Button("Load from local", ImVec2(ImGui::GetContentRegionAvail().x, mim::ui::ButtonHeight())))
        {
            vm.LoadFromLocal();
        }
        
        if (ImGui::BeginTable("##TLESatelliteListup", 4, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.9)))
        {
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableSetupColumn("Name##tle",                ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Use##tle",                 ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Next AOS##tle",            ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Max Elevation##tle",       ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableHeadersRow();

            char ButtonLabel[32];
            for (int i = 0; vm.HasSatellite(i); i++)
            {
                if (vm.IsFilteredOut(i))
                    continue;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", vm.SatelliteName(i));
                ImGui::TableSetColumnIndex(1);
                if(vm.InUse(i) == false)
                {
                    sprintf(ButtonLabel, "Select##TLE%d", i);
                    if(ImGui::Button(ButtonLabel, ImVec2(ImGui::GetContentRegionAvail().x, mim::ui::ButtonHeight())))
                    {
                        vm.Use(i);
                    }
                }
                else
                {
                    sprintf(ButtonLabel, "Delete##TLE%d", i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
                    if(ImGui::Button(ButtonLabel, ImVec2(ImGui::GetContentRegionAvail().x, mim::ui::ButtonHeight())))
                    {
                        vm.Unuse(i);
                    }
                    ImGui::PopStyleColor();
                }
                ImGui::TableSetColumnIndex(2);
                mim::vm::LocalTime aos = vm.NextAos(i);
                ImGui::Text("%d/%d %d:%d:%d", aos.month, aos.day, aos.hour, aos.minute, aos.second);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.3lf°", vm.MaxElevationDeg(i));
            }

            
            ImGui::EndTable();
        }
        if(ImGui::Button("Load TLE", ImVec2((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, mim::ui::ButtonHeight())))
        {
            vm.DownloadAndLoad();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, mim::ui::ButtonHeight())))
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
