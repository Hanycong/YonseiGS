// View - Beacon 창 탭 공통 표 도우미
#include <inttypes.h>
#include <stdio.h>
#include <string>

#include "imgui.h"
#include "beacon_tabs.h"

void StateWindowColumnManager(const char * info)
{
    ImGui::TableNextColumn();
    ImGui::Text(info);
    ImGui::TableNextColumn();
}

// 비콘 가독성 업!!
// static void BeaconSectionHeader(const char* title)
// {
//     ImGui::TableNextRow();
//     ImGui::TableSetColumnIndex(0);

//     ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1,
//                        ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.35f, 0.4f)));


//     ImGui::PushFont(ImGui::GetFont());        
//     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
//     ImGui::TextUnformatted(title);
//     ImGui::PopStyleColor();
//     ImGui::PopFont();

//     ImGui::TableSetColumnIndex(1);
//     ImGui::TextUnformatted("");
// }
void BeaconSectionHeader(const char* title)
{
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Row background
    ImGui::TableSetBgColor(
        ImGuiTableBgTarget_RowBg0,
        ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.35f, 0.6f))
    );

    ImGui::TableSetColumnIndex(0);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.6f, 1.0f));
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();

    ImGui::TableSetColumnIndex(1);
    ImGui::TextUnformatted("");
}

// 배열 한 줄 (GS_nuri5 ImGui_Beacon*Array 와 같은 형식: 값마다 뒤에 공백 하나).
// 5차 GS 는 고정 크기 버퍼(64/256)에 써서 긴 배열이 잘릴 수 있었다. 여기서는 std::string.
template <typename T>
static void BeaconArrayRow(const char* label, const T* data, size_t len, const char* fmt)
{
    std::string s;
    char tmp[16];
    for (size_t i = 0; i < len; i++)
    {
        snprintf(tmp, sizeof(tmp), fmt, data[i]);
        s += tmp;
    }
    StateWindowColumnManager(label);
    ImGui::TextUnformatted(s.c_str());
}

void BeaconRawBytesRow(const char* label, const uint8_t* data, size_t len) { BeaconArrayRow(label, data, len, "%02X "); }
void BeaconU8ArrayRow(const char* label, const uint8_t* data, size_t len)  { BeaconArrayRow(label, data, len, "%" PRIu8 " "); }
void BeaconI16ArrayRow(const char* label, const int16_t* data, size_t len) { BeaconArrayRow(label, data, len, "%" PRId16 " "); }
void BeaconU16ArrayRow(const char* label, const uint16_t* data, size_t len){ BeaconArrayRow(label, data, len, "%" PRIu16 " "); }
