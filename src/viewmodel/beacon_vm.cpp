#include "beacon_vm.h"

#include <mutex>

extern Beacon * beacon;
extern MissionBeacon * missionbeacon;
extern GETFILEINFO * getfileinfo;
extern Event * event;

namespace mim {
namespace vm {

Beacon *        BeaconVM::LastBeacon() const        { return beacon; }
MissionBeacon * BeaconVM::LastMissionBeacon() const { return missionbeacon; }
Event *         BeaconVM::LastEvent() const         { return event; }
GETFILEINFO *   BeaconVM::LastFileInfo() const      { return getfileinfo; }

ReportView_t BeaconVM::ReportSnapshot() const
{
    ReportView_t view;
    {
        std::lock_guard<std::mutex> lk(g_report_view_mtx);
        view = g_report_view;
    }
    return view;
}

uint32_t BeaconVM::TotalRxBytes() const { return get_rx_bytes(); }

BeaconVM & BeaconViewModel()
{
    static BeaconVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
