// Beacon / Report 탭 화면 지문 하네스: ImGui 글자 출력 기록
//
// ImGui::Text / TextUnformatted / Separator / NewLine / CollapsingHeader 와 탭 항목 함수를
// 이 파일에서 다시 정의한다 (링크 시 --allow-multiple-definition, 하네스 오브젝트를 먼저).
// 글자는 그려지지 않고 "표 행:열 | 글자" 로 기록된다. 표 행/열은 실제 ImGui 표 상태에서 읽는다.
// BeginTabItem 은 g_tab 과 이름이 같은 탭만 열린 것으로 한다.
#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "imgui.h"
#include "imgui_internal.h"

static std::string g_rec;          // 이번 프레임 기록
static const char * g_tab = "";    // 열린 것으로 할 탭 이름

static void RecLine(const char * kind, const char * text, const char * text_end = nullptr)
{
    ImGuiTable * t = ImGui::GetCurrentTable();
    char pos[48];
    if (t) snprintf(pos, sizeof(pos), "%d:%d", ImGui::TableGetRowIndex(), ImGui::TableGetColumnIndex());
    else snprintf(pos, sizeof(pos), "-");
    g_rec += pos;
    g_rec += ' ';
    g_rec += kind;
    g_rec += " | ";
    g_rec += text_end ? std::string(text, text_end) : std::string(text);
    g_rec += '\n';
}

namespace ImGui {
void TextV(const char* fmt, va_list args)
{
    va_list a2;
    va_copy(a2, args);
    int n = vsnprintf(nullptr, 0, fmt, a2);
    va_end(a2);
    std::string s(n > 0 ? n : 0, '\0');
    if (n > 0) vsnprintf(&s[0], n + 1, fmt, args);
    RecLine("T", s.c_str());
}
void Text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextV(fmt, args);
    va_end(args);
}
void TextUnformatted(const char* text, const char* text_end) { RecLine("T", text, text_end); }   // Text 와 구분하지 않는다
void Separator() { RecLine("S", ""); }
void NewLine() { RecLine("N", ""); }
bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags) { RecLine("H", label); return true; }
bool BeginTabItem(const char* label, bool*, ImGuiTabItemFlags) { return strcmp(label, g_tab) == 0; }
void EndTabItem() {}
}

// 64비트 FNV-1a (사례마다 기록 전체의 지문)
static unsigned long long Fnv(const std::string & s)
{
    unsigned long long h = 1469598103934665603ULL;
    for (unsigned char c : s) { h ^= c; h *= 1099511628211ULL; }
    return h;
}
