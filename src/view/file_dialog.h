// View 공용 UI 부품 - 파일 선택 창 (Windows 탐색기 형태의 ImGui 모달)
//
// 화면 상태만 가진다 (현재 폴더, 방문 기록, 선택 항목). 파일을 읽지는 않고 고른 경로만 돌려준다.
// 다른 팝업 모달 안에서 Draw 를 부르면 그 위에 겹쳐 열린다.
#pragma once
#include <ctime>
#include <string>
#include <vector>
#include "imgui.h"

namespace mim { namespace ui {

class FileDialog
{
public:
    // 다음 Draw 에서 창을 연다.
    // start: 시작 파일 또는 폴더 (없으면 ../home/TLE, 그것도 없으면 현재 폴더)
    // exts : ".txt;.tle" 처럼 ';' 로 구분한 확장자. nullptr 이면 모든 파일
    void Open(const char * start, const char * exts = nullptr);

    // 매 프레임 호출. 사용자가 파일을 고르면 true 를 돌려주고 out 에 절대 경로를 쓴다.
    bool Draw(const char * title, char * out, size_t out_size);

private:
    struct Entry
    {
        std::string name;
        bool        dir;
        uintmax_t   size;
        std::time_t mtime;
    };
    struct Place
    {
        std::string label;
        std::string path;
    };

    void Navigate(const std::string & dir, bool record = true);
    void Refresh();
    void Sort();
    bool Visible(const Entry & e) const;
    bool Accept(char * out, size_t out_size);
    void DrawToolbar();
    void DrawPlaces();
    void DrawFiles(char * out, size_t out_size, bool & picked);

    bool open_request_ = false;
    std::string cwd_;
    std::vector<std::string> back_, forward_;
    std::vector<Entry> entries_;
    std::vector<Place> places_;
    std::vector<std::string> exts_;
    std::string filter_label_;
    int  filter_ = 0;                   // 0: exts_ 만, 1: 모든 파일
    int  selected_ = -1;
    bool text_enter_ = false;           // 이번 프레임 Enter 로 검색칸/이름칸을 빠져나왔는지
    int  sort_col_ = 0;
    bool sort_asc_ = true;
    char name_[256] = "";
    char search_[64] = "";
    std::string error_;
};

}} // namespace mim::ui
