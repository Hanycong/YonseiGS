#include "track_vm.h"

#include <string.h>
#include <string>

#include "pass_table.h"
#include "track_target.h"

extern int Requested;
extern int Satellite_row;

namespace mim {
namespace vm {

using mim::model::Passes;
using mim::model::Tracking;

void TrackVM::BeginFrame()
{
    Satellite_row = 0;
}

char * TrackVM::SearchBuffer()           { return search_; }
size_t TrackVM::SearchBufferSize() const { return sizeof(search_); }

int TrackVM::RowCount() const               { return Passes().Count(); }
const TrackRow & TrackVM::Row(int i) const  { return Passes().At(i); }

bool TrackVM::HiddenBySearch(int i) const
{
    return strlen(search_) != 0 &&
           std::string(Passes().At(i).name).find(search_) == std::string::npos;
}

bool TrackVM::IsSelected(int i) const { return Requested == i; }
void TrackVM::Select(int i)           { Requested = i; }

bool TrackVM::IsTracking(int i) const { return Tracking().Index() == i; }
void TrackVM::Track(int i)            { Tracking().Set(i); }

bool TrackVM::HasTrackTarget() const { return Tracking().Index() >= 0; }

const char* TrackVM::TrackedName() const
{
    const char* selName = nullptr;
    if (Tracking().Index() >= 0 && Tracking().Index() < Passes().Count())
        selName = Passes().At(Tracking().Index()).name;
    return selName;
}

float TrackVM::TrackedAzimuth() const
{
    return (Tracking().Index() >= 0) ? Passes().ExampleAzimuth(Tracking().Index()) : 0.f;
}

float TrackVM::TrackedElevation() const
{
    return (Tracking().Index() >= 0) ? Passes().ExampleElevation(Tracking().Index()) : 0.f;
}

TrackVM & TrackViewModel()
{
    static TrackVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
