// View 공용 UI 부품 - 파일 선택 창 구현
#include "file_dialog.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>

#include "YonseiGS_theme.h"
#include "ui_kit.h"

namespace fs = std::filesystem;

namespace mim { namespace ui {

static const ImVec4 FOLDER_COLOR = ImVec4(0.910f, 0.749f, 0.388f, 1.00f); // WARN 과 같은 노랑

static std::string Lower(std::string s)
{
    for (char & c : s)
        c = (char)std::tolower((unsigned char)c);
    return s;
}

static bool IsDir(const std::string & p)
{
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

static std::string Absolute(const std::string & p)
{
    std::error_code ec;
    fs::path a = fs::weakly_canonical(fs::absolute(p, ec), ec);
    return ec ? p : a.string();
}

static std::string Join(const std::string & dir, const std::string & name)
{
    return dir == "/" ? "/" + name : dir + "/" + name;
}

static std::string Extension(const std::string & name)
{
    size_t dot = name.find_last_of('.');
    return (dot == std::string::npos || dot == 0) ? "" : Lower(name.substr(dot));
}

// 폴더 / 파일 아이콘 (글꼴에 아이콘 글리프가 없어 직접 그린다)
static void Icon(bool dir)
{
    float h = ImGui::GetTextLineHeight();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList * dl = ImGui::GetWindowDrawList();
    if (dir)
    {
        ImU32 c = ImGui::GetColorU32(FOLDER_COLOR);
        dl->AddRectFilled(ImVec2(p.x, p.y + h * 0.18f), ImVec2(p.x + h * 0.45f, p.y + h * 0.35f), c, 1.5f);
        dl->AddRectFilled(ImVec2(p.x, p.y + h * 0.30f), ImVec2(p.x + h * 0.95f, p.y + h * 0.85f), c, 2.0f);
    }
    else
    {
        ImU32 c = ImGui::GetColorU32(TEXT_DIM);
        dl->AddRect(ImVec2(p.x + h * 0.15f, p.y + h * 0.12f), ImVec2(p.x + h * 0.80f, p.y + h * 0.90f), c, 1.5f);
        for (int i = 0; i < 3; i++)
        {
            float y = p.y + h * (0.35f + 0.17f * i);
            dl->AddLine(ImVec2(p.x + h * 0.30f, y), ImVec2(p.x + h * 0.65f, y), c);
        }
    }
    ImGui::Dummy(ImVec2(h, h));
}

void FileDialog::Open(const char * start, const char * exts)
{
    open_request_ = true;
    back_.clear();
    forward_.clear();
    search_[0] = '\0';
    name_[0] = '\0';
    error_.clear();

    exts_.clear();
    filter_label_.clear();
    if (exts)
    {
        std::string s(exts);
        size_t pos = 0;
        while (pos <= s.size())
        {
            size_t end = s.find(';', pos);
            if (end == std::string::npos)
                end = s.size();
            if (end > pos)
            {
                exts_.push_back(Lower(s.substr(pos, end - pos)));
                filter_label_ += (filter_label_.empty() ? "*" : ", *") + exts_.back();
            }
            pos = end + 1;
        }
    }
    filter_ = exts_.empty() ? 1 : 0;

    // 왼쪽 바로가기
    places_.clear();
    const char * home = getenv("HOME");
    if (home)
        places_.push_back({"Home", home});
    if (IsDir("../home/TLE"))
        places_.push_back({"TLE", Absolute("../home/TLE")});
    if (IsDir("../data"))
        places_.push_back({"GS Data", Absolute("../data")});
    // WSL: Windows 사용자 폴더의 다운로드 / 바탕화면
    std::error_code ec;
    if (IsDir("/mnt/c/Users"))
    {
        for (const fs::directory_entry & u : fs::directory_iterator("/mnt/c/Users", ec))
        {
            std::string user = u.path().filename().string();
            if (user == "Public" || user == "Default" || user == "Default User" || user == "All Users")
                continue;
            std::string base = u.path().string();
            if (IsDir(base + "/Downloads"))
                places_.push_back({"Downloads (Windows)", base + "/Downloads"});
            if (IsDir(base + "/Desktop"))
                places_.push_back({"Desktop (Windows)", base + "/Desktop"});
        }
    }
    if (IsDir("/mnt/c"))
        places_.push_back({"C: drive", "/mnt/c"});
    places_.push_back({"Root (/)", "/"});

    // 시작 위치: 파일이면 그 폴더를 열고 파일을 선택해 둔다
    std::string first;
    std::string s = start ? start : "";
    if (!s.empty() && IsDir(s))
        first = s;
    else if (!s.empty() && fs::exists(s, ec))
    {
        first = fs::path(s).parent_path().string();
        snprintf(name_, sizeof(name_), "%s", fs::path(s).filename().c_str());
    }
    else if (!s.empty() && IsDir(fs::path(s).parent_path().string()))
        first = fs::path(s).parent_path().string();
    else if (IsDir("../home/TLE"))
        first = "../home/TLE";
    else
        first = ".";

    Navigate(Absolute(first), false);
    for (int i = 0; i < (int)entries_.size(); i++)
        if (!entries_[i].dir && entries_[i].name == name_)
            selected_ = i;
}

void FileDialog::Navigate(const std::string & dir, bool record)
{
    if (!IsDir(dir))
    {
        error_ = "Cannot open folder: " + dir;
        return;
    }
    if (record && dir != cwd_)
    {
        back_.push_back(cwd_);
        forward_.clear();
    }
    cwd_ = dir;
    search_[0] = '\0';
    Refresh();
}

void FileDialog::Refresh()
{
    entries_.clear();
    selected_ = -1;
    error_.clear();

    std::error_code ec;
    fs::directory_iterator it(cwd_, fs::directory_options::skip_permission_denied, ec);
    if (ec)
    {
        error_ = "Cannot read folder: " + ec.message();
        return;
    }
    for (; it != fs::directory_iterator(); it.increment(ec))
    {
        if (ec)
            break;
        std::string name = it->path().filename().string();
        if (name.empty() || name[0] == '.')         // 숨김 파일
            continue;
        struct stat st;
        if (stat(it->path().c_str(), &st) != 0)     // 끊어진 링크 등
            continue;
        entries_.push_back({name, S_ISDIR(st.st_mode), (uintmax_t)st.st_size, st.st_mtime});
    }
    Sort();
}

void FileDialog::Sort()
{
    int col = sort_col_;
    bool asc = sort_asc_;
    std::sort(entries_.begin(), entries_.end(), [col, asc](const Entry & a, const Entry & b) {
        if (a.dir != b.dir)                         // 폴더가 항상 먼저
            return a.dir;
        int r = 0;
        if (col == 1)
            r = a.mtime < b.mtime ? -1 : a.mtime > b.mtime ? 1 : 0;
        else if (col == 2)
            r = Extension(a.name).compare(Extension(b.name));
        else if (col == 3)
            r = a.size < b.size ? -1 : a.size > b.size ? 1 : 0;
        if (r == 0)
            r = Lower(a.name).compare(Lower(b.name));
        return asc ? r < 0 : r > 0;
    });
    selected_ = -1;
}

bool FileDialog::Visible(const Entry & e) const
{
    if (search_[0] && Lower(e.name).find(Lower(search_)) == std::string::npos)
        return false;
    if (e.dir || filter_ == 1)
        return true;
    return std::find(exts_.begin(), exts_.end(), Extension(e.name)) != exts_.end();
}

// "Open" / 더블클릭 / Enter: 폴더면 들어가고, 파일이면 경로를 돌려준다
bool FileDialog::Accept(char * out, size_t out_size)
{
    if (!name_[0])
        return false;
    std::string path = name_[0] == '/' ? std::string(name_) : Join(cwd_, name_);
    if (IsDir(path))
    {
        name_[0] = '\0';
        Navigate(Absolute(path));
        return false;
    }
    std::error_code ec;
    if (!fs::is_regular_file(path, ec))
    {
        error_ = std::string("File not found: ") + name_;
        return false;
    }
    snprintf(out, out_size, "%s", Absolute(path).c_str());
    return true;
}

void FileDialog::DrawToolbar()
{
    const ImGuiStyle & style = ImGui::GetStyle();

    // 뒤로 / 앞으로 / 위로 (비활성은 흐리게만 그리고 클릭을 무시)
    auto NavArrow = [](const char * id, ImGuiDir dir, bool enabled) {
        if (!enabled)
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.35f);
        bool pressed = ImGui::ArrowButton(id, dir) && enabled;
        if (!enabled)
            ImGui::PopStyleVar();
        return pressed;
    };
    if (NavArrow("##fd_back", ImGuiDir_Left, !back_.empty()))
    {
        forward_.push_back(cwd_);
        std::string d = back_.back();
        back_.pop_back();
        Navigate(d, false);
    }
    ImGui::SameLine(0, 4);
    if (NavArrow("##fd_fwd", ImGuiDir_Right, !forward_.empty()))
    {
        back_.push_back(cwd_);
        std::string d = forward_.back();
        forward_.pop_back();
        Navigate(d, false);
    }
    ImGui::SameLine(0, 4);
    if (NavArrow("##fd_up", ImGuiDir_Up, cwd_ != "/"))
        Navigate(fs::path(cwd_).parent_path().string());
    ImGui::SameLine(0, 4);
    if (ImGui::Button("Refresh##fd"))
        Refresh();

    // 경로 표시줄: 폴더 단위 버튼 (넘치면 앞쪽을 "..." 로 줄인다)
    float search_w = ImGui::GetFontSize() * 12.0f;
    ImGui::SameLine();
    float bar_w = ImGui::GetContentRegionAvail().x - search_w - style.ItemSpacing.x;

    std::vector<std::string> parts;
    for (const fs::path & p : fs::path(cwd_))
        if (!p.empty() && p != "/")
            parts.push_back(p.string());
    auto PartWidth = [&](const std::string & s) {
        return ImGui::CalcTextSize(s.c_str()).x + style.FramePadding.x * 2 + ImGui::CalcTextSize(">").x + 4;
    };
    float total = PartWidth("/");
    for (const std::string & s : parts)
        total += PartWidth(s);
    size_t first = 0;
    if (total > bar_w)
    {
        total += ImGui::CalcTextSize("... ").x;
        while (first + 1 < parts.size() && total > bar_w)
            total -= PartWidth(parts[first++]);
    }

    ImVec2 bar_min = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(bar_min,
        ImVec2(bar_min.x + bar_w, bar_min.y + ImGui::GetFrameHeight()),
        ImGui::GetColorU32(ImGuiCol_FrameBg), style.FrameRounding);
    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, style.ItemSpacing.y));
    std::string acc;
    if (ImGui::Button("/##fd_crumb_root"))
        Navigate("/");
    for (size_t i = 0; i < parts.size(); i++)
    {
        acc += "/" + parts[i];
        if (i < first)
            continue;
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::TextDisabled(i == first && first > 0 ? "... >" : ">");
        ImGui::SameLine();
        ImGui::PushID((int)i);
        if (ImGui::Button(parts[i].c_str()))
        {
            ImGui::PopID();
            Navigate(acc);
            break;
        }
        ImGui::PopID();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::EndGroup();

    // 검색칸 (오른쪽 끝)
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - search_w);
    ImGui::SetNextItemWidth(search_w);
    ImGui::InputTextWithHint("##fd_search", "Search", search_, sizeof(search_));
}

void FileDialog::DrawPlaces()
{
    ImGui::PushStyleColor(ImGuiCol_Text, TEXT_DIM);
    ImGui::TextUnformatted("Quick access");
    ImGui::PopStyleColor();
    ImGui::Spacing();
    for (size_t i = 0; i < places_.size(); i++)
    {
        ImGui::PushID((int)i);
        Icon(true);
        ImGui::SameLine();
        if (ImGui::Selectable(places_[i].label.c_str(), cwd_ == places_[i].path))
            Navigate(places_[i].path);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", places_[i].path.c_str());
        ImGui::PopID();
    }
}

void FileDialog::DrawFiles(char * out, size_t out_size, bool & picked)
{
    ImGuiTableFlags flags = ImGuiTableFlags_Sortable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                            ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_SizingFixedFit;
    if (!ImGui::BeginTable("##fd_files", 4, flags, ImGui::GetContentRegionAvail()))
        return;
    float em = ImGui::GetFontSize();
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Name",          ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
    ImGui::TableSetupColumn("Date modified", ImGuiTableColumnFlags_WidthFixed, em * 9.5f);
    ImGui::TableSetupColumn("Type",          ImGuiTableColumnFlags_WidthFixed, em * 7.0f);
    ImGui::TableSetupColumn("Size",          ImGuiTableColumnFlags_WidthFixed, em * 5.0f);
    ImGui::TableHeadersRow();

    if (ImGuiTableSortSpecs * specs = ImGui::TableGetSortSpecs())
    {
        if (specs->SpecsDirty && specs->SpecsCount > 0)
        {
            sort_col_ = specs->Specs[0].ColumnIndex;
            sort_asc_ = specs->Specs[0].SortDirection == ImGuiSortDirection_Ascending;
            Sort();
            specs->SpecsDirty = false;
        }
    }

    std::vector<int> rows;
    for (int i = 0; i < (int)entries_.size(); i++)
        if (Visible(entries_[i]))
            rows.push_back(i);

    std::string go_into;
    ImGuiListClipper clipper;
    clipper.Begin((int)rows.size());
    while (clipper.Step())
    {
        for (int r = clipper.DisplayStart; r < clipper.DisplayEnd; r++)
        {
            int i = rows[r];
            const Entry & e = entries_[i];
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(i);
            if (ImGui::Selectable("##row", selected_ == i,
                                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick |
                                  ImGuiSelectableFlags_AllowItemOverlap))
            {
                selected_ = i;
                snprintf(name_, sizeof(name_), "%s", e.name.c_str());
                error_.clear();
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    if (e.dir)
                        go_into = Join(cwd_, e.name);
                    else
                        picked = Accept(out, out_size);
                }
            }
            ImGui::SameLine(0, 0);
            Icon(e.dir);
            ImGui::SameLine();
            ImGui::TextUnformatted(e.name.c_str());

            ImGui::TableSetColumnIndex(1);
            char buf[32];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&e.mtime));
            ImGui::TextDisabled("%s", buf);

            ImGui::TableSetColumnIndex(2);
            std::string ext = Extension(e.name);
            if (e.dir)
                ImGui::TextDisabled("File folder");
            else if (ext.empty())
                ImGui::TextDisabled("File");
            else
            {
                std::string t = ext.substr(1);
                for (char & c : t)
                    c = (char)std::toupper((unsigned char)c);
                ImGui::TextDisabled("%s File", t.c_str());
            }

            ImGui::TableSetColumnIndex(3);
            if (!e.dir)
            {
                char sz[32];
                snprintf(sz, sizeof(sz), "%ju KB", (e.size + 1023) / 1024);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(sz).x);
                ImGui::TextDisabled("%s", sz);
            }
            ImGui::PopID();
        }
    }
    if (rows.empty())
        ImGui::TextDisabled(search_[0] ? "  No items match your search." : "  This folder is empty.");
    ImGui::EndTable();

    if (!go_into.empty())
    {
        name_[0] = '\0';
        Navigate(go_into);
    }
}

bool FileDialog::Draw(const char * title, char * out, size_t out_size)
{
    if (open_request_)
    {
        ImGui::OpenPopup(title);
        open_request_ = false;
    }
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1000, 620), ImGuiCond_Appearing);
    if (!ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoSavedSettings))
        return false;

    bool picked = false;
    const ImGuiStyle & style = ImGui::GetStyle();
    float em = ImGui::GetFontSize();

    DrawToolbar();
    ImGui::Spacing();

    // 아래 바(파일 이름 + 필터 / 상태 + 버튼) 높이를 남기고 본문
    float footer_h = ImGui::GetFrameHeightWithSpacing() + ButtonHeight() + style.ItemSpacing.y * 2;
    float body_h = ImGui::GetContentRegionAvail().y - footer_h;

    ImGui::BeginChild("##fd_places", ImVec2(em * 12.0f, body_h), true);
    DrawPlaces();
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##fd_list", ImVec2(0, body_h), true);
    DrawFiles(out, out_size, picked);
    ImGui::EndChild();

    // 파일 이름 + 형식 필터
    float filter_w = em * 11.0f;
    RowLabel("File name", 4.5f);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - filter_w - style.ItemSpacing.x);
    if (ImGui::InputText("##fd_name", name_, sizeof(name_), ImGuiInputTextFlags_EnterReturnsTrue))
        picked = picked || Accept(out, out_size);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(filter_w);
    std::string filter_items[2] = {"Files (" + filter_label_ + ")", "All files (*.*)"};
    if (ImGui::BeginCombo("##fd_filter", filter_items[filter_].c_str()))
    {
        for (int f = exts_.empty() ? 1 : 0; f < 2; f++)
            if (ImGui::Selectable(filter_items[f].c_str(), filter_ == f))
                filter_ = f;
        ImGui::EndCombo();
    }

    // 상태 (항목 수 또는 오류) + Open / Cancel
    float btn_w = em * 7.0f;
    ImGui::AlignTextToFramePadding();
    if (!error_.empty())
        ImGui::TextColored(ERR, "%s", error_.c_str());
    else
    {
        int n = 0;
        for (const Entry & e : entries_)
            n += Visible(e) ? 1 : 0;
        ImGui::TextDisabled("%d items", n);
    }
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - btn_w * 2 - style.ItemSpacing.x);
    // Enter: 이름칸 밖(목록 등)에 포커스가 있어도 Open 과 같다
    bool enter = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() &&
                 ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter));
    if (ImGui::Button("Open##fd", ImVec2(btn_w, ButtonHeight())) || enter)
        picked = picked || Accept(out, out_size);
    ImGui::SameLine();
    bool cancel = ImGui::Button("Cancel##fd", ImVec2(btn_w, ButtonHeight())) ||
                  (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)));

    if (picked || cancel)
        ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
    return picked;
}

}} // namespace mim::ui
