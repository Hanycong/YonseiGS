// View - Load 팝업
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
#include "tle_vm.h"

using namespace mim;

bool popup_load()
{
    ImGui::OpenPopup("Load");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::Text("Progress : %d%c", mim::vm::TleViewModel().LoadProgressPercent(), '%');
        ImGui::EndPopup();
    }
    return true;
}
