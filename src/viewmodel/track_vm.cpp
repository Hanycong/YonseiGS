#include "track_vm.h"

#include <ctype.h>
#include <string.h>

#include "YonseiGS_config.h"
#include "YonseiGS_orbital.h"
#include "track_target.h"
#include "ui_state.h"

extern int Requested;
extern int Satellite_row;

namespace mim {
namespace vm {

using mim::model::Tracking;

// 대소문자 구분 없는 부분 문자열 검색
static bool ContainsNoCase(const char * text, const char * pattern)
{
    if (!pattern[0])
        return true;
    for (const char * t = text; *t; t++)
    {
        const char * a = t;
        const char * b = pattern;
        while (*a && *b && tolower((unsigned char)*a) == tolower((unsigned char)*b))
        {
            a++;
            b++;
        }
        if (!*b)
            return true;
    }
    return false;
}

// TLE Manager 에서 고른(use) 위성인지
static bool InUse(int sat)
{
    return sat >= 0 && sat < SAT_MAX_NUM && OrbitData.Satellites[sat] != NULL &&
           OrbitData.Satellites[sat]->use && OrbitData.Satellites[sat]->Name()[0];
}

void TrackVM::BeginFrame()
{
    Satellite_row = 0;
    rows_.clear();
    selected_count_ = 0;
    if (Loading())
        return;

    // TLE 목록은 이름이 빈 슬롯에서 끝난다 (TleVM::HasSatellite 와 같은 조건)
    for (int i = 0; i < SAT_MAX_NUM && OrbitData.Satellites[i] != NULL && OrbitData.Satellites[i]->Name()[0]; i++)
    {
        SatelliteObject * s = OrbitData.Satellites[i];
        if (!s->use)
            continue;
        selected_count_++;
        if (!ContainsNoCase(s->Name(), search_))
            continue;

        TrackRow r = {};
        r.sat = i;
        r.name = s->Name();
        r.has_pass = s->_max_elevation[0] > 0;
        if (r.has_pass)
        {
            DateTime aos = s->_nextaos[0].AddHours(9);
            DateTime los = s->_nextlos[0].AddHours(9);
            r.aos_mon = aos.Month(); r.aos_day = aos.Day();
            r.aos_h = aos.Hour(); r.aos_m = aos.Minute(); r.aos_s = aos.Second();
            r.los_mon = los.Month(); r.los_day = los.Day();
            r.los_h = los.Hour(); r.los_m = los.Minute(); r.los_s = los.Second();
            r.max_el = (float)(s->_max_elevation[0] * RAD_TO_DEG);
        }
        rows_.push_back(r);
    }
}

char * TrackVM::SearchBuffer()           { return search_; }
size_t TrackVM::SearchBufferSize() const { return sizeof(search_); }

// LoadFromLocal / Load TLE 가 위성 객체를 다시 만드는 동안 NowTracking 이 false 다
bool TrackVM::Loading() const               { return !OrbitData.NowTracking; }
int  TrackVM::RowCount() const              { return (int)rows_.size(); }
const TrackRow & TrackVM::Row(int r) const  { return rows_[r]; }
int  TrackVM::SelectedCount() const         { return selected_count_; }

bool TrackVM::IsSelected(int r) const { return Requested == rows_[r].sat; }
void TrackVM::Select(int r)           { Requested = rows_[r].sat; }

bool TrackVM::IsTracking(int r) const { return Tracking().Index() == rows_[r].sat; }
void TrackVM::Track(int r)            { Tracking().Set(rows_[r].sat); }

void TrackVM::OpenTleManager() { AppUi.Display_TLE = true; }

bool TrackVM::HasTrackTarget() const { return !Loading() && InUse(Tracking().Index()); }

const char* TrackVM::TrackedName() const
{
    return HasTrackTarget() ? OrbitData.Satellites[Tracking().Index()]->Name() : nullptr;
}

// topo 는 추적 스레드(TrackingSatellites)가 10 ms 마다 갱신한다 [rad]
float TrackVM::TrackedAzimuth() const
{
    return HasTrackTarget() ? (float)(OrbitData.Satellites[Tracking().Index()]->topo.azimuth * RAD_TO_DEG) : 0.f;
}

float TrackVM::TrackedElevation() const
{
    return HasTrackTarget() ? (float)(OrbitData.Satellites[Tracking().Index()]->topo.elevation * RAD_TO_DEG) : 0.f;
}

TrackVM & TrackViewModel()
{
    static TrackVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
