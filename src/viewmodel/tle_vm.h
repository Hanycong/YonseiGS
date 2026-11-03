// ViewModel - TLE Manager 팝업과 TLE 로딩 진행률(Load) 팝업
//
// TLE 목록 선택, 위성 검색/SGP4 필터, 위성 추적 목록 추가/삭제, TLE 다운로드·로컬 파일 로드.
// 행 순회 순서와 필터 조건은 원래 팝업 코드 그대로다.
#pragma once

#include <stddef.h>

namespace mim {
namespace vm {

// "Next AOS" 칸에 쓰는 한국 시각(UTC+9) 성분
struct LocalTime {
    int month, day, hour, minute, second;
};

class TleVM
{
public:
    TleVM();                            // local_path_ 를 절대 경로로 바꿔 둔다

    // ---- TLE 목록 콤보 ----
    int * SelectedList();               // NowTLE (AutoPilot 과 공유)
    const char * SelectedListLabel() const;
    int ListCapacity() const;           // 콤보 순회 상한 (원래 코드는 ftplistup 크기를 썼다)
    const char * ListLabel(int i) const;    // 비어 있으면 nullptr

    // ---- 검색 / 필터 / 로컬 파일 ----
    char * SearchText();
    size_t SearchTextSize() const;
    bool * Sgp4Only();                  // 체크하면 앞으로 AOS 가 없는 위성은 숨긴다
    char * LocalPath();
    size_t LocalPathSize() const;

    void LoadFromLocal();               // "Load from local"
    void DownloadAndLoad();             // "Load TLE"

    // ---- 위성 표 ----
    bool HasSatellite(int i) const;     // 이름이 비면 목록 끝
    bool IsFilteredOut(int i) const;
    const char * SatelliteName(int i) const;
    bool InUse(int i) const;
    void Use(int i);                    // "Select"
    void Unuse(int i);                  // "Delete"
    LocalTime NextAos(int i) const;
    double MaxElevationDeg(int i) const;

    // ---- Load 팝업 ----
    int LoadProgressPercent() const;

private:
    char search_[256] = {0};
    // 기본값: 저장소의 home/TLE/NORAD_TLE.txt (프로그램은 bin/ 에서 실행된다)
    char local_path_[256] = "../home/TLE/NORAD_TLE.txt";
};

TleVM & TleViewModel();

} // namespace vm
} // namespace mim
