// View 공용 UI 부품 구현
#include "ui_kit.h"
#include "YonseiGS_theme.h"

namespace mim { namespace ui {

void SectionHeader(const char * title)
{
    ImGui::Spacing();
    if (mim::font_section)
        ImGui::PushFont(mim::font_section);
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();
    if (mim::font_section)
        ImGui::PopFont();
    ImGui::PushStyleColor(ImGuiCol_Separator, BORDER);
    ImGui::Separator();
    ImGui::PopStyleColor();
}

void RowLabel(const char * label, float label_em)
{
    ImGui::AlignTextToFramePadding();
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    ImGui::SameLine(ImGui::GetFontSize() * label_em);
}

float ButtonHeight()
{
    return ImGui::GetFrameHeight() * 1.3f;
}

bool GridButton(const char * label, int col)
{
    float w;
    if (col == 0)
        w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
    else
    {
        ImGui::SameLine();
        w = ImGui::GetContentRegionAvail().x;
    }
    return ImGui::Button(label, ImVec2(w, ButtonHeight()));
}

bool WideButton(const char * label)
{
    return ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x, ButtonHeight()));
}

bool ToggleWideButton(const char * label, bool active)
{
    if (active)
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        DANGER);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, DANGER_HOV);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  DANGER_HOV);
    }
    bool clicked = WideButton(label);
    if (active)
        ImGui::PopStyleColor(3);
    return clicked;
}

void StatusLamp(const char * label, bool ok)
{
    ImVec4 col = ok ? OK : ERR;
    float r = ImGui::GetFontSize() * 0.22f;
    ImVec2 p = ImGui::GetCursorScreenPos();
    float cy = p.y + ImGui::GetTextLineHeight() * 0.5f;
    ImDrawList * dl = ImGui::GetWindowDrawList();
    dl->AddCircleFilled(ImVec2(p.x + r, cy), r * 2.0f, U32(Alpha(col, 0.18f)));
    dl->AddCircleFilled(ImVec2(p.x + r, cy), r, U32(col));
    ImGui::Dummy(ImVec2(r * 2.0f, ImGui::GetTextLineHeight()));
    ImGui::SameLine(0, r * 1.6f);
    ImGui::PushStyleColor(ImGuiCol_Text, ok ? TEXT : TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
}

void ValueRow(const char * label, const char * value)
{
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    float vw = ImGui::CalcTextSize(value).x;
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - vw);
    ImGui::TextUnformatted(value);
}

}} // namespace mim::ui
