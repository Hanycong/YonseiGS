// View - 명령 생성기 (Control 창의 "Send Telecommand" / "Initial Ops" 팝업 본체)
//
// miman_imgui.cpp 에서 내용 그대로 옮긴 것 (MVVM 1단계: View 추출).
// 명령 선택 콤보와 "Write Raw Data" 만 여기 있고, 명령별 입력 폼은 cmdgen_forms.h 의 앱별 파일이 그린다.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include <mutex>


#include "cmdgen_view.h"
#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

void DrawCmdGeneratorBody(bool initial_mode)
{

    // 명령 선택 목록(라벨, 필터, 초기 운용 세트)은 명령 카탈로그에서 읽는다
    // (model/telecommand/command_catalog.h, model/missions/*)
    static int selected_index = 0;

    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::BeginCombo("##Subsystem", mim::tc::CategoryName(selected_index)))
    {
        for (int n = 0; n < mim::tc::CategoryCount(); n++)
        {
            bool is_selected = (selected_index == n);
            if (ImGui::Selectable(mim::tc::CategoryName(n), is_selected))
                selected_index = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    const mim::tc::CommandDef* current = mim::tc::FindCommand(gen_fnccode);
    const char* current_label = current ? current->label : "Select CMD type";

    if (ImGui::BeginCombo("##Select CMD type", current_label))
    {
        if (initial_mode)
        {

            const int* initial_ops = mim::tc::InitialOpsCommands();
            for (int k = 0; initial_ops[k] >= 0; ++k)
            {
                const mim::tc::CommandDef* cmd = mim::tc::FindCommand(initial_ops[k]);
                if (cmd == nullptr || !mim::tc::InCategory(*cmd, selected_index))
                    continue;

                int i = cmd->id;
                bool SelectedGen = (gen_fnccode == i);
                if (ImGui::Selectable(cmd->label, SelectedGen))
                    gen_fnccode = i;

                if (SelectedGen)
                    ImGui::SetItemDefaultFocus();
            }
        }
        else
        {
            for (int k = 0; k < mim::tc::CommandCount(); k++)
            {
                const mim::tc::CommandDef* cmd = &mim::tc::CommandAt(k);
                if (!mim::tc::InCategory(*cmd, selected_index))
                    continue;

                int i = cmd->id;
                bool SelectedGen = (gen_fnccode == i);
                if (ImGui::Selectable(cmd->label, SelectedGen))
                {
                    if (gen_fnccode != i)
                        gen_fnccode = i;
                }
                if (SelectedGen)
                    ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    ImGui::Checkbox("Write Raw Data", &raw_data);

    // 명령 번호별 입력 폼: FSW 앱별 파일(view/cmdgen/*/forms_*.cpp)에서 처리한다 (번호마다 한 곳)
    DrawRawForms(gen_fnccode)
        || DrawBee1000EpsForms(gen_fnccode)
        || DrawBee1000AdcsForms(gen_fnccode)
        || DrawBee1000ScForms(gen_fnccode)
        || DrawBee1000ToLabForms(gen_fnccode)
        || DrawBee1000UantForms(gen_fnccode)
        || DrawBee1000SpForms(gen_fnccode)
        || DrawBee1000PayuzucForms(gen_fnccode)
        || DrawBee1000FmForms(gen_fnccode)
        || DrawCosmicFtpForms(gen_fnccode)
        || DrawBee1000UnknownForms(gen_fnccode)
        || DrawBee1000UtrxForms(gen_fnccode)
        || DrawCosmicToLabForms(gen_fnccode)
        || DrawCosmicEpsForms(gen_fnccode)
        || DrawCosmicSpForms(gen_fnccode)
        || DrawCosmicUelPiForms(gen_fnccode)
        || DrawBee1000EsForms(gen_fnccode);
}
