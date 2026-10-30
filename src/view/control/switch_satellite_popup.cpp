// View - Control 창 "Switch Satellite" 팝업: 대상 위성(HMAC 키) 전환
//
// ImGui 호출과 ControlVM 호출만 있다.
#include "imgui.h"
#include "control_vm.h"
#include "control_popups.h"

void HMACWindow()
{
    mim::vm::ControlVM & vm = mim::vm::ControlViewModel();

    ImGui::Text("Select Satellite");

    for (int i = 0; i < vm.SatelliteCount(); i++)
    {
        if (ImGui::RadioButton(vm.SatelliteName(i), vm.SelectedSatellite(), i))
        {
            vm.SwitchToSelectedSatellite();
        }
    }
}
