#include "tle_vm.h"

#include <string.h>

#include "imgui.h"
#include "miman_config.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_satcontext.h"
#include "svc_tracking.h"

extern StateCheckUnit State;
extern Console console;
extern int NowTLE;
extern bool sgp4check;

namespace mim {
namespace vm {

int * TleVM::SelectedList() { return &NowTLE; }
const char * TleVM::SelectedListLabel() const { return State.tleinfolistup[NowTLE]->label; }
int TleVM::ListCapacity() const { return IM_ARRAYSIZE(FtpShared.ftplistup); }
const char * TleVM::ListLabel(int i) const
{
    if (!State.tleinfolistup[i])
        return nullptr;
    return State.tleinfolistup[i]->label;
}

char * TleVM::SearchText() { return search_; }
size_t TleVM::SearchTextSize() const { return sizeof(search_); }
bool * TleVM::Sgp4Only() { return &sgp4check; }
char * TleVM::LocalPath() { return local_path_; }
size_t TleVM::LocalPathSize() const { return sizeof(local_path_); }

void TleVM::LoadFromLocal()
{
    State.NowTracking = false;
    memset(State.tleinfolistup[NowTLE]->local, 0, sizeof(State.tleinfolistup[NowTLE]->local));
    memcpy(State.tleinfolistup[NowTLE]->local, local_path_, sizeof(local_path_));

    console.AddLog("[OK]##TLE \"%s\" Download Completed.", State.tleinfolistup[NowTLE]->label);

    mim::svc::StartSatelliteInitialize(&sgp4check);
}

void TleVM::DownloadAndLoad()
{
    if(DownloadTLE(State.tleinfolistup[NowTLE]->remote, State.tleinfolistup[NowTLE]->local))
    {
        State.NowTracking = false;

        console.AddLog("[OK]##TLE \"%s\" Download Completed.", State.tleinfolistup[NowTLE]->label);

        mim::svc::StartSatelliteInitialize(&sgp4check);
    }
    else
    {
        console.AddLog("[ERROR]##TLE Download failed. Please check again.");
    }
}

bool TleVM::HasSatellite(int i) const { return strlen(State.Satellites[i]->Name()) != 0; }

bool TleVM::IsFilteredOut(int i) const
{
    if(State.Satellites[i]->_max_elevation[0] < 0 && sgp4check)
        return true;
    if(strlen(search_) != 0 && strstr(State.Satellites[i]->Name(), search_) == NULL)
        return true;
    return false;
}

const char * TleVM::SatelliteName(int i) const { return State.Satellites[i]->Name(); }
bool TleVM::InUse(int i) const { return State.Satellites[i]->use; }

void TleVM::Use(int i)
{
    State.Satellites[i]->use = true;
    if(State.Satellites[i]->cal == false)
        State.Satellites[i]->Refresh(State.Satellites[i]->tle, State.Satellites[i]->obs, true, true);
    SatelliteModelInitialize(i);
    mim::Initialize_SatContext();   // ← VisibleSats 재계산
}

void TleVM::Unuse(int i)
{
    State.Satellites[i]->use = false;
    SatelliteModelDelete(i);
    mim::Initialize_SatContext();   // ← VisibleSats 재계산
}

LocalTime TleVM::NextAos(int i) const
{
    LocalTime t;
    t.month  = State.Satellites[i]->_nextaos[0].AddHours(9).Month();
    t.day    = State.Satellites[i]->_nextaos[0].AddHours(9).Day();
    t.hour   = State.Satellites[i]->_nextaos[0].AddHours(9).Hour();
    t.minute = State.Satellites[i]->_nextaos[0].AddHours(9).Minute();
    t.second = State.Satellites[i]->_nextaos[0].AddHours(9).Second();
    return t;
}

double TleVM::MaxElevationDeg(int i) const { return State.Satellites[i]->_max_elevation[0] * RAD_TO_DEG; }

int TleVM::LoadProgressPercent() const { return State.loadindex * 100 / State.tleallindex; }

TleVM & TleViewModel()
{
    static TleVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
