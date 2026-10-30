// View - Control 창 "Ping each node" 팝업: 지정한 CSP 노드에 핑
//
// ImGui 호출과 ControlVM 호출만 있다.
#include "imgui.h"
#include "control_vm.h"
#include "control_popups.h"

void PingWindow() {
    mim::vm::ControlVM & vm = mim::vm::ControlViewModel();

    ImGui::InputScalar("node", ImGuiDataType_U8, vm.PingTargetNode());

    if (ImGui::Button("Send Ping")) {
        vm.PingNode();
    }
}
