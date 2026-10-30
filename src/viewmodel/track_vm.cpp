#include "track_vm.h"

#include <string.h>
#include <string>

#include "miman_orbital.h"

extern int Requested;
extern int Satellite_row;

namespace mim {
namespace vm {

// ════════════════════════════════════════════
// 임시 예시 데이터 20개
// ════════════════════════════════════════════
static const TrackRow dummies[] = {
    {"BEE-1000",  3,38,42,  3,52,10,  64.f},
    {"ISS",       4,12, 8,  4,23,45,  38.f},
    {"NOAA-19",   5, 1,33,  5,14,20,  21.f},
    {"MIMAN",     6,44,50,  6,55, 5,  12.f},
    {"KOMPSAT-3", 7,10,15,  7,22,30,  45.f},
    {"AQUA",      8, 5,22,  8,18,44,  33.f},
    {"TERRA",     9,30, 0,  9,41,12,  27.f},
    {"SUOMI-NPP",10,15,33, 10,28,55,  52.f},
    {"METOP-B",  11, 2,41, 11,14, 8,  19.f},
    {"FENGYUN-3",12,48,10, 13, 1,22,  41.f},
    {"GOES-16",  13,20,55, 13,33,10,  36.f},
    {"SENTINEL-1",14, 5, 3, 14,17,40,  58.f},
    {"SENTINEL-2",15,33,18, 15,45,55,  22.f},
    {"LANDSAT-8", 16,10,44, 16,23,20,  47.f},
    {"RADARSAT-2",17,22, 5, 17,34,50,  30.f},
    {"SPOT-7",    18, 8,30, 18,21,15,  25.f},
    {"PLEIADES-1",19,45,12, 19,58, 0,  39.f},
    {"WORLDVIEW-3",20,30,22, 20,42,55, 61.f},
    {"GEOEYE-1",  21,15,40, 21,28,10,  18.f},
    {"IKONOS",    22, 5,55, 22,18,30,  43.f},
};
static const int dummyCount = 20;

void TrackVM::BeginFrame()
{
    Satellite_row = 0;
}

char * TrackVM::SearchBuffer()           { return search_; }
size_t TrackVM::SearchBufferSize() const { return sizeof(search_); }

int TrackVM::RowCount() const               { return dummyCount; }
const TrackRow & TrackVM::Row(int i) const  { return dummies[i]; }

bool TrackVM::HiddenBySearch(int i) const
{
    return strlen(search_) != 0 &&
           std::string(dummies[i].name).find(search_) == std::string::npos;
}

bool TrackVM::IsSelected(int i) const { return Requested == i; }
void TrackVM::Select(int i)           { Requested = i; }

bool TrackVM::IsTracking(int i) const { return GetNowTracking() == i; }
void TrackVM::Track(int i)            { SetNowTracking(i); }

bool TrackVM::HasTrackTarget() const { return GetNowTracking() >= 0; }

const char* TrackVM::TrackedName() const
{
    const char* selName = nullptr;
    if (GetNowTracking() >= 0 && GetNowTracking() < dummyCount)
        selName = dummies[GetNowTracking()].name;
    return selName;
}

// 예시 값 (실제 SGP4 관측각이 아님). 원래 코드 그대로: 앙각은 범위 검사 없이 읽는다 (Known defects)
float TrackVM::TrackedAzimuth() const
{
    return (GetNowTracking() >= 0) ? (float)(GetNowTracking() * 18 % 360) : 0.f;
}

float TrackVM::TrackedElevation() const
{
    return (GetNowTracking() >= 0) ? dummies[GetNowTracking()].max_el * 0.7f : 0.f;
}

TrackVM & TrackViewModel()
{
    static TrackVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
