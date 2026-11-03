// View - 명령 생성기 (Control 창의 "Send Telecommand" / "Initial Ops" 팝업 본체)
//
// ImGui 호출과 CmdGeneratorVM 호출만 있다.
// 명령 선택 콤보와 "Write Raw Data" 만 여기 있고, 명령별 입력 폼은 cmdgen_forms.h 의 앱별 파일이 그린다.
#include "imgui.h"
#include "cmdgen_vm.h"
#include "cmdgen_view.h"
#include "cmdgen_forms.h"
#include "cmdgen5_common.h"
#include "sat_mode.h"

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

    const uint16_t id = vm.SelectedCommand();
    if (mim::sat::Current() != mim::sat::SAT_BEE1000)
    {
        // 누리호 5차 위성: 표 기반 명령(cFE 코어 등)을 먼저, 그다음 앱별 폼
        DrawBee1012CfeCommandEditor(id)
        || DrawBee1012AiobcForms(id)
        || DrawGeneric5Forms(id)
        || DrawBee1012CiLabForms(id)
        || DrawBee1012DsForms(id)
        || DrawBee1012FmForms(id)
        || DrawBee1012HkForms(id)
        || DrawBee1012ToLabForms(id)
        || DrawBee1012RptForms(id)
        || DrawBee1012GpioForms(id)
        || DrawBee1012PaySltForms(id)
        || DrawBee1012PaybeeKisscamForms(id)
        || DrawBee1012UtrxForms(id)
        || DrawBee1012StxForms(id)
        || DrawBee1012AdcsForms(id)
        || DrawBee1012EpsForms(id)
        || DrawBee1012TtcForms(id)
        || DrawBee1012MeowForms(id)
        || DrawBee1012MissionForms(id)
        || DrawBee1012GpsForms(id)
        || DrawBee1012LtrxForms(id)
        || DrawBee1012SchLabForms(id)
        || DrawUelysysEoForms(id)
        || DrawUelysysPayuelAosForms(id)
        || DrawUelysysPayuelLgbatForms(id)
        || DrawUelysysPayuelYonseiForms(id)
        || DrawUelysysPayuelRomaForms(id)
        || DrawUelysysUantForms(id)
        || DrawUelysysGpioForms(id)
        || DrawUelysysSchLabForms(id)
        || DrawUelysysSpForms(id)
        || DrawUelysysStxForms(id)
        || DrawUelysysTtcForms(id)
        || DrawUelysysPayuelObcForms(id)
        || DrawUelysysPayuelCamForms(id)
        || DrawUelysysPayuelLgpmForms(id)
        || DrawSd1SatCiForms(id)
        || DrawSd1SatCrForms(id)
        || DrawSd1SatOpsForms(id)
        || DrawUelysysMeowForms(id)
        || DrawSd1SatUantForms(id)
        || DrawSd1SatUtrxForms(id)
        || DrawSd1SatToForms(id)
        || DrawSd1SatEpsForms(id)
        || DrawSd1SatPaysd1RomaForms(id)
        || DrawSd1SatPaysd1LothanForms(id)
        || DrawSd1SatSnbpForms(id)
        || DrawSd1SatGpsForms(id)
        || DrawSd1SatFmForms(id);
        return;
    }

    // 명령 번호별 입력 폼: FSW 앱별 파일(view/cmdgen/*/forms_*.cpp)에서 처리한다 (번호마다 한 곳)
    DrawRawForms(id)
        || DrawBee1000EpsForms(id)
        || DrawBee1000AdcsForms(id)
        || DrawBee1000ScForms(id)
        || DrawBee1000ToLabForms(id)
        || DrawBee1000UantForms(id)
        || DrawBee1000SpForms(id)
        || DrawBee1000PayuzucForms(id)
        || DrawBee1000FmForms(id)
        || DrawCosmicFtpForms(id)
        || DrawBee1000UnknownForms(id)
        || DrawBee1000UtrxForms(id)
        || DrawCosmicToLabForms(id)
        || DrawCosmicEpsForms(id)
        || DrawCosmicSpForms(id)
        || DrawCosmicUelPiForms(id)
        || DrawBee1000EsForms(id);
}
