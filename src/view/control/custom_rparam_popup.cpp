// View - Control 창 "Custom RParam Get" 팝업: 임의 노드/테이블/주소의 원격 파라미터 읽기/쓰기
//
// control_view.cpp 에서 내용 그대로 옮긴 것.
#include <inttypes.h>
#include <time.h>
#include <gs/param/rparam.h>
#include <csp/csp.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_csp.h"
#include "svc_tx.h"
#include "svc_radio.h"
#include "control_popups.h"

extern StateCheckUnit State;
extern Setup * setup;
extern Console console;
extern bool p_open;
extern ImGuiWindowFlags mim_winflags;
extern char AmpTimeBuf[32];
extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

void RparamWindow()
{
    static uint8_t node = 0;
    static uint8_t table_id = 0;
    static uint16_t addr = 0;
    static int type_index = 0;

    static uint8_t  value_u8 = 0;
    static uint16_t value_u16 = 0;
    static uint32_t value_u32 = 0;
    static int8_t   value_s8 = 0;
    static int16_t  value_s16 = 0;
    static int32_t  value_s32 = 0;
    static float    value_float = 0;
    // static char     value_char = 0;

    const char* type_names[] = { "uint8", "uint16", "uint32", "int8", "int16", "int32", "float"};

    ImGui::Text("Node");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_node", ImGuiDataType_U8, &node);

    ImGui::Text("Table ID");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_table", ImGuiDataType_U8, &table_id);


    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));
    ImGui::Text("!!!WARNING!!! Your Addr must be decimal, not hex");
    ImGui::PopStyleColor();

    ImGui::Text("Address");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##rparam_addr", ImGuiDataType_U16, &addr);



    ImGui::Text("Type");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::BeginCombo("##rparam_type", type_names[type_index]))
    {
        for (int i = 0; i < IM_ARRAYSIZE(type_names); ++i)
        {
            bool is_selected = (type_index == i);
            if (ImGui::Selectable(type_names[i], is_selected))
                type_index = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (type_index == 0)
    {
        ImGui::Text("Value (uint8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u8", ImGuiDataType_U8, &value_u8, NULL, NULL, "%u");
    }
    else if (type_index == 1)
    {
        ImGui::Text("Value (uint16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u16", ImGuiDataType_U16, &value_u16, NULL, NULL, "%u");
    }
    else if (type_index == 2)
    {
        ImGui::Text("Value (uint32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_u32", ImGuiDataType_U32, &value_u32, NULL, NULL, "%u");
    }
        else if (type_index == 3)
    {
        ImGui::Text("Value (int8)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s8", ImGuiDataType_S8, &value_s8, NULL, NULL, "%u");
    }
        else if (type_index == 4)
    {
        ImGui::Text("Value (int16)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s16", ImGuiDataType_S16, &value_s16, NULL, NULL, "%u");
    }
        else if (type_index == 5)
    {
        ImGui::Text("Value (int32)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_s32", ImGuiDataType_S32, &value_s32, NULL, NULL, "%u");
    }
        else if (type_index == 6)
    {
        ImGui::Text("Value (float)");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##rparam_value_float", ImGuiDataType_Float, &value_float, NULL, NULL, "%u");
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
        if (type_index == 0)
        {
            // eps 안불러와지면 0x0BB0 시도!
            if (gs_rparam_get_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u8) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 1)
        {
            if (gs_rparam_get_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u16) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 2)
        {
            if (gs_rparam_get_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_u32) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }
        else if (type_index == 3)
        {
            if (gs_rparam_get_int8(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s8) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi8, node, table_id, addr, value_s8);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 4)
        {
            if (gs_rparam_get_int16(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s16) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi16, node, table_id, addr, value_s16);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 5)
        {
            if (gs_rparam_get_int32(node, table_id, addr, 0xB00B, setup->default_timeout, &value_s32) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%" PRIi32, node, table_id, addr, value_s32);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }

        else if (type_index == 6)
        {
            if (gs_rparam_get_float(node, table_id, addr, 0xB00B, setup->default_timeout, &value_float) == GS_OK)
                console.AddLog("[OK]##Rparam Load OK. node=%u table=%u addr=0x%04X value=%f", node, table_id, addr, value_float);
            else
                console.AddLog("[ERROR]##Rparam Load Failed.");
        }




    }

    ImGui::SameLine();

    if (ImGui::Button("Save##rparam_save", ImVec2(w * 0.5f, h)))
    {
        if (type_index == 0)
        {
            if (gs_rparam_set_uint8(node, table_id, addr, 0xB00B, setup->default_timeout, value_u8) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu8, node, table_id, addr, value_u8);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
        else if (type_index == 1)
        {
            if (gs_rparam_set_uint16(node, table_id, addr, 0xB00B, setup->default_timeout, value_u16) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu16, node, table_id, addr, value_u16);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
        else if (type_index == 2)
        {
            if (gs_rparam_set_uint32(node, table_id, addr, 0xB00B, setup->default_timeout, value_u32) == GS_OK)
                console.AddLog("[OK]##Rparam Save OK. node=%u table=%u addr=0x%04X value=%" PRIu32, node, table_id, addr, value_u32);
            else
                console.AddLog("[ERROR]##Rparam Save Failed.");
        }
    }
}
