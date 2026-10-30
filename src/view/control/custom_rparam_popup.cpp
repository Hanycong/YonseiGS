// View - Control 창 "Custom RParam Get" 팝업: 임의 노드/테이블/주소의 원격 파라미터 읽기/쓰기
//
// ImGui 호출과 CustomRparamVM 호출만 있다.
#include "imgui.h"
#include "control_vm.h"
#include "control_popups.h"

void RparamWindow()
{
    mim::vm::CustomRparamVM & vm = mim::vm::CustomRparamViewModel();

    const char* type_names[] = { "uint8", "uint16", "uint32", "int8", "int16", "int32", "float"};

    ImGui::Text("Node");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_node", ImGuiDataType_U8, vm.Node());

    ImGui::Text("Table ID");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_table", ImGuiDataType_U8, vm.TableId());


    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));
    ImGui::Text("!!!WARNING!!! Your Addr must be decimal, not hex");
    ImGui::PopStyleColor();

    ImGui::Text("Address");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_addr", ImGuiDataType_U16, vm.Address());



    ImGui::Text("Type");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::BeginCombo("##rparam_type", type_names[*vm.TypeIndex()]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(type_names); ++i)
        {
            bool is_selected = (*vm.TypeIndex() == i);
            if (ImGui::Selectable(type_names[i], is_selected))
                *vm.TypeIndex() = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (*vm.TypeIndex() == 0)
    {
        ImGui::Text("Value (uint8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u8", ImGuiDataType_U8, vm.ValueU8(), NULL, NULL, "%u");
    }
    else if (*vm.TypeIndex() == 1)
    {
        ImGui::Text("Value (uint16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u16", ImGuiDataType_U16, vm.ValueU16(), NULL, NULL, "%u");
    }
    else if (*vm.TypeIndex() == 2)
    {
        ImGui::Text("Value (uint32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u32", ImGuiDataType_U32, vm.ValueU32(), NULL, NULL, "%u");
    }
        else if (*vm.TypeIndex() == 3)
    {
        ImGui::Text("Value (int8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s8", ImGuiDataType_S8, vm.ValueS8(), NULL, NULL, "%u");
    }
        else if (*vm.TypeIndex() == 4)
    {
        ImGui::Text("Value (int16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s16", ImGuiDataType_S16, vm.ValueS16(), NULL, NULL, "%u");
    }
        else if (*vm.TypeIndex() == 5)
    {
        ImGui::Text("Value (int32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s32", ImGuiDataType_S32, vm.ValueS32(), NULL, NULL, "%u");
    }
        else if (*vm.TypeIndex() == 6)
    {
        ImGui::Text("Value (float)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_float", ImGuiDataType_Float, vm.ValueFloat(), NULL, NULL, "%u");
    }


    float w = ImGui::GetContentRegionAvail().x;
    float h = ImGui::GetFontSize() * 1.5f;
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 215, 0, 255));
    ImGui::Text("Please Try: (get AX100 active_conf)");

    ImGui::Text("node = 5, table_id = 4, address = 24");
    ImGui::Text("Type: uint8");

    ImGui::PopStyleColor();
    if (ImGui::Button("Load##rparam_load", ImVec2(w * 0.5f, h)))
    {
        vm.Load();
    }

    ImGui::SameLine();

    if (ImGui::Button("Save##rparam_save", ImVec2(w * 0.5f, h)))
    {
        vm.Save();
    }
}
