#include "beacon_vm.h"
#include "n5_beacon.h"

#include <mutex>

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

// 수신 스레드가 쓰는 비콘 버퍼를 잠금 안에서 복사해 둔 사본 (받기 전에는 0, GS_nuri5 화면처럼 0 으로 그린다)
const BEE1012_Beacon_t * BeaconVM::LastBee1012Beacon() const { CopyBEE1012Beacon(bee1012_copy_); return &bee1012_copy_; }
const UELYSYS_Beacon_t * BeaconVM::LastUelysysBeacon() const { CopyUELYSYSBeacon(uelysys_copy_); return &uelysys_copy_; }
const SD1SAT_Beacon_t *  BeaconVM::LastSd1satBeacon() const  { CopySD1SATBeacon(sd1sat_copy_);   return &sd1sat_copy_; }

void BeaconVM::ReportSnapshot5(ReportView_t & view, EPS_Report_State_t & eps_state,
                               EPS_RParam_Reassembly_State_t & eps_rparam,
                               RPT_OpsData_t & rpt_ops, bool & rpt_ops_valid) const
{
    std::lock_guard<std::mutex> lk(g_report_view_mtx);
    view = g_report_view;
    eps_state = g_eps_report_state;
    eps_rparam = g_eps_rparam_reassembly;
    rpt_ops = g_rpt_ops_data;
    rpt_ops_valid = g_rpt_ops_data_valid;
}

uint32_t BeaconVM::TotalRxBytes() const { return mim::model::Ax100().TotalRxBytes(); }

BeaconVM & BeaconViewModel()
{
    static BeaconVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
