// View - 명령 생성기 (Control 창의 "Send Telecommand" / "Initial Ops" 팝업 본체)
//
// ImGui 호출과 CmdGeneratorVM 호출만 있다.
// 명령 선택 콤보와 "Write Raw Data" 만 여기 있고, 명령별 입력 폼은 cmdgen_forms.h 의 앱별 파일이 그린다.
#include "imgui.h"
#include "cmdgen_vm.h"
#include "cmdgen_view.h"
#include "cmdgen_forms.h"

void DrawCmdGeneratorBody(bool initial_mode)
{
    mim::vm::CmdGeneratorVM & vm = mim::vm::CmdGeneratorViewModel();

    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::BeginCombo("##Subsystem", vm.CategoryName(*vm.SelectedCategory())))
    {
        for (int n = 0; n < vm.CategoryCount(); n++)
        {
            bool is_selected = (*vm.SelectedCategory() == n);
            if (ImGui::Selectable(vm.CategoryName(n), is_selected))
                *vm.SelectedCategory() = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::BeginCombo("##Select CMD type", vm.SelectedLabel()))
    {
        for (int k = 0; k < vm.PickerSize(initial_mode); k++)
        {
            const mim::tc::CommandDef* cmd = vm.PickerItem(initial_mode, k);
            if (cmd == nullptr)
                continue;

            bool SelectedGen = vm.IsSelected(cmd->id);
            if (ImGui::Selectable(cmd->label, SelectedGen))
                vm.Select(cmd->id);

            if (SelectedGen)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Write Raw Data", vm.RawData());

    // 명령 번호별 입력 폼: FSW 앱별 파일(view/cmdgen/*/forms_*.cpp)에서 처리한다 (번호마다 한 곳)
    DrawRawForms(vm.SelectedCommand())
        || DrawBee1000EpsForms(vm.SelectedCommand())
        || DrawBee1000AdcsForms(vm.SelectedCommand())
        || DrawBee1000ScForms(vm.SelectedCommand())
        || DrawBee1000ToLabForms(vm.SelectedCommand())
        || DrawBee1000UantForms(vm.SelectedCommand())
        || DrawBee1000SpForms(vm.SelectedCommand())
        || DrawBee1000PayuzucForms(vm.SelectedCommand())
        || DrawBee1000FmForms(vm.SelectedCommand())
        || DrawCosmicFtpForms(vm.SelectedCommand())
        || DrawBee1000UnknownForms(vm.SelectedCommand())
        || DrawBee1000UtrxForms(vm.SelectedCommand())
        || DrawCosmicToLabForms(vm.SelectedCommand())
        || DrawCosmicEpsForms(vm.SelectedCommand())
        || DrawCosmicSpForms(vm.SelectedCommand())
        || DrawCosmicUelPiForms(vm.SelectedCommand())
        || DrawBee1000EsForms(vm.SelectedCommand());
}
