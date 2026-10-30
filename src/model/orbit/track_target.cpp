#include "track_target.h"

#include "miman_orbital.h"

namespace mim {
namespace model {

int  TrackTarget::Index() const  { return GetNowTracking(); }
void TrackTarget::Set(int index) { SetNowTracking(index); }

TrackTarget & Tracking()
{
    static TrackTarget target;
    return target;
}

} // namespace model
} // namespace mim
