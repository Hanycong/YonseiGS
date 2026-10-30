#include "telemetry_store.h"

#include <mutex>

extern Beacon * beacon;
extern MissionBeacon * missionbeacon;
extern GETFILEINFO * getfileinfo;
extern Event * event;

namespace mim {
namespace model {

Beacon *        TelemetryStore::LastBeacon() const        { return beacon; }
MissionBeacon * TelemetryStore::LastMissionBeacon() const { return missionbeacon; }
Event *         TelemetryStore::LastEvent() const         { return event; }
GETFILEINFO *   TelemetryStore::LastFileInfo() const      { return getfileinfo; }

ReportView_t TelemetryStore::ReportSnapshot() const
{
    ReportView_t view;
    {
        std::lock_guard<std::mutex> lk(g_report_view_mtx);
        view = g_report_view;
    }
    return view;
}

TelemetryStore & Telemetry()
{
    static TelemetryStore store;
    return store;
}

} // namespace model
} // namespace mim
