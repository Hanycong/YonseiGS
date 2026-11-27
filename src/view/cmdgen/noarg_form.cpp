// View - 명령 생성기: 인자 없는 명령의 공용 입력 폼 (ImGui 호출과 vm 호출만)
#include "noarg_form.h"

#include <stdio.h>

#include "imgui.h"
#include "cmdgen_vm.h"

namespace mim {
namespace ui {

void NoArgCommandForm::Draw()
{
    char label[96];

    if (labels_ == kTitled)
    {
        snprintf(label, sizeof(label), "Msg ID%s", tag_);
        ImGui::InputScalar(label, ImGuiDataType_U16, &msgid_, NULL, NULL, "0x%04X");
        snprintf(label, sizeof(label), "Command Code(CC)%s", tag_);
        ImGui::InputScalar(label, ImGuiDataType_U8, &fnccode_);
    }
    else
    {
        snprintf(label, sizeof(label), "msgid%s", tag_);
        ImGui::InputScalar(label, ImGuiDataType_U16, &msgid_);
        snprintf(label, sizeof(label), "fnccode%s", tag_);
        ImGui::InputScalar(label, ImGuiDataType_U8, &fnccode_);
    }

    snprintf(label, sizeof(label), "Generate CMD%s", tag_);
    if (ImGui::Button(label))
        mim::vm::CmdGeneratorViewModel().SendNoArgCommand(msgid_, fnccode_);
}

bool DrawNoArgForm(NoArgCommandForm * forms, size_t count, int id)
{
    for (size_t i = 0; i < count; i++)
    {
        if (forms[i].Id() == id)
        {
            forms[i].Draw();
            return true;
        }
    }
    return false;
}

} // namespace ui
} // namespace mim
