#include "beacon_vm.h"

#include "telemetry_store.h"
#include "ax100_radio.h"

namespace mim {
namespace vm {

using mim::model::Telemetry;

Beacon *        BeaconVM::LastBeacon() const        { return Telemetry().LastBeacon(); }
MissionBeacon * BeaconVM::LastMissionBeacon() const { return Telemetry().LastMissionBeacon(); }
Event *         BeaconVM::LastEvent() const         { return Telemetry().LastEvent(); }
GETFILEINFO *   BeaconVM::LastFileInfo() const      { return Telemetry().LastFileInfo(); }

ReportView_t BeaconVM::ReportSnapshot() const { return Telemetry().ReportSnapshot(); }

const BEE1012_Beacon_t * BeaconVM::LastBee1012Beacon() const { return bee1012_beacon; }
const UELYSYS_Beacon_t * BeaconVM::LastUelysysBeacon() const { return uelysys_beacon; }
const SD1SAT_Beacon_t *  BeaconVM::LastSd1satBeacon() const  { return sd1sat_beacon; }

uint32_t BeaconVM::TotalRxBytes() const { return mim::model::Ax100().TotalRxBytes(); }

BeaconVM & BeaconViewModel()
{
    static BeaconVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
