// View - Setup 팝업 (시작 시 장비/지상국 설정, Start/Quit)
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
#include "setup_vm.h"
#include "ui_kit.h"

using namespace mim;

bool popup_setup(Setup * setup)
{
    mim::vm::SetupVM & vm = mim::vm::SetupViewModel();
    Setup & cfg = vm.Config();
    ImGui::OpenPopup("YonseiGS Setup");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if ( ImGui::BeginPopupModal("YonseiGS Setup", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        using namespace mim::ui;
        const float lw = 7.5f;   // 라벨 열 너비 (글자 수)
        auto Field = [](const char * label, float w) {
            RowLabel(label, w);
            ImGui::SetNextItemWidth(-FLT_MIN);
        };

        // 왼쪽: 장비 연결 / 오른쪽: CSP 라우팅, S-band
        if (ImGui::BeginTable("##setup_cols", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_PadOuterX,
                              ImVec2(ImGui::GetFontSize() * 46.0f, 0)))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            mim::ui::SectionHeader("TRANSCEIVER (GS100)");
            Field("Device", lw);
            ImGui::InputText("##GS100_name", &cfg.Transciever_devname[0], sizeof(cfg.Transciever_devname));
            Field("Frequency (Hz)", lw);
            ImGui::InputDouble("##GS100_freq", &cfg.default_freq);
            Field("Baudrate", lw);
            ImGui::InputScalar("##GS100_baud", ImGuiDataType_U32, &cfg.Transceiver_baud, NULL, NULL, "%u");
            Field("Delay (ms)", lw);
            ImGui::InputScalar("##GS100_delay", ImGuiDataType_U32, &cfg.default_timeout, NULL, NULL, "%u");

            mim::ui::SectionHeader("SWITCH BOX (KTA223)");
            Field("Device", lw);
            ImGui::InputText("##KTA223_name", &cfg.Switch_devname[0], sizeof(cfg.Switch_devname));
            Field("Baudrate", lw);
            ImGui::InputScalar("##KTA223_baud", ImGuiDataType_U32, &cfg.Switch_baud, NULL, NULL, "%u");

            mim::ui::SectionHeader("ROTATOR (GS232B)");
            Field("Device", lw);
            ImGui::InputText("##GS232B_name", &cfg.Rotator_devname[0], sizeof(cfg.Rotator_devname));
            Field("Baudrate", lw);
            ImGui::InputScalar("##GS232B_baud", ImGuiDataType_U32, &cfg.Rotator_baud, NULL, NULL, "%u");

            ImGui::TableSetColumnIndex(1);

            mim::ui::SectionHeader("ROUTE TABLE (CSP NODE)");
            Field("GS100", lw);
            ImGui::InputScalar("##gs100_node", ImGuiDataType_U8, &cfg.gs100_node, NULL, NULL, "%u");
            Field("GS PC", lw);
            ImGui::InputScalar("##gspc_node", ImGuiDataType_U8, &cfg.kiss_node, NULL, NULL, "%u");
            Field("AX100", lw);
            ImGui::InputScalar("##ax100_node", ImGuiDataType_U8, &cfg.ax100_node, NULL, NULL, "%u");
            Field("OBC", lw);
            ImGui::InputScalar("##obc_node", ImGuiDataType_U8, &cfg.obc_node, NULL, NULL, "%u");

            mim::ui::SectionHeader("S-BAND GROUND STATION");
            RowLabel("Use S-band", lw);
            ImGui::Checkbox("##s_band_use_state_check", vm.UseSband());
            if(*vm.UseSband())
            {
                Field("Username", lw);
                ImGui::InputText("##S-BandUsername", &cfg.S_Username[0], sizeof(cfg.S_Username));
                Field("IP Address", lw);
                ImGui::InputText("##S-BandIP", &cfg.S_Address[0], sizeof(cfg.S_Address));
                Field("Password", lw);
                ImGui::InputText("##S-BandPW", &cfg.S_passwd[0], sizeof(cfg.S_passwd));
            }
            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Start", ImVec2((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f, mim::ui::ButtonHeight())))
        {
            vm.Start();


            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset Watchdog", ImVec2((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, mim::ui::ButtonHeight())))
        {
            vm.ResetWatchdog();
        }
        ImGui::SameLine();
        if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, mim::ui::ButtonHeight())))
        {
            vm.Quit();
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        
        ImGui::EndPopup();
        return true;
    }
    return true;
}
