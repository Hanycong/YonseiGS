#include "tle_vm.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imgui.h"
#include "YonseiGS_config.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_model.h"
#include "YonseiGS_satcontext.h"
#include "svc_tracking.h"

extern StateCheckUnit State;
extern Console console;
extern int NowTLE;
extern bool sgp4check;

namespace mim {
namespace vm {

TleVM::TleVM()
{
    char abs[PATH_MAX];
    if (realpath(local_path_, abs))
        snprintf(local_path_, sizeof(local_path_), "%s", abs);
}

int * TleVM::SelectedList() { return &NowTLE; }
const char * TleVM::SelectedListLabel() const { return OrbitData.tleinfolistup[NowTLE]->label; }
int TleVM::ListCapacity() const { return IM_ARRAYSIZE(FtpShared.ftplistup); }
const char * TleVM::ListLabel(int i) const
{
    if (!OrbitData.tleinfolistup[i])
        return nullptr;
    return OrbitData.tleinfolistup[i]->label;
}

char * TleVM::SearchText() { return search_; }
size_t TleVM::SearchTextSize() const { return sizeof(search_); }
bool * TleVM::Sgp4Only() { return &sgp4check; }
char * TleVM::LocalPath() { return local_path_; }
size_t TleVM::LocalPathSize() const { return sizeof(local_path_); }

void TleVM::LoadFromLocal()
{
    OrbitData.NowTracking = false;
    memset(OrbitData.tleinfolistup[NowTLE]->local, 0, sizeof(OrbitData.tleinfolistup[NowTLE]->local));
    memcpy(OrbitData.tleinfolistup[NowTLE]->local, local_path_, sizeof(local_path_));

    console.AddLog("[OK]##TLE \"%s\" Download Completed.", OrbitData.tleinfolistup[NowTLE]->label);

    mim::svc::StartSatelliteInitialize(&sgp4check);
}

void TleVM::DownloadAndLoad()
{
    if(DownloadTLE(OrbitData.tleinfolistup[NowTLE]->remote, OrbitData.tleinfolistup[NowTLE]->local))
    {
        OrbitData.NowTracking = false;

        console.AddLog("[OK]##TLE \"%s\" Download Completed.", OrbitData.tleinfolistup[NowTLE]->label);

        mim::svc::StartSatelliteInitialize(&sgp4check);
    }
    else
    {
        console.AddLog("[ERROR]##TLE Download failed. Please check again.");
    }
}

bool TleVM::HasSatellite(int i) const { return strlen(OrbitData.Satellites[i]->Name()) != 0; }

bool TleVM::IsFilteredOut(int i) const
{
    if(OrbitData.Satellites[i]->_max_elevation[0] < 0 && sgp4check)
        return true;
    if(strlen(search_) != 0 && strstr(OrbitData.Satellites[i]->Name(), search_) == NULL)
        return true;
    return false;
}

const char * TleVM::SatelliteName(int i) const { return OrbitData.Satellites[i]->Name(); }
bool TleVM::InUse(int i) const { return OrbitData.Satellites[i]->use; }

void TleVM::Use(int i)
{
    OrbitData.Satellites[i]->use = true;
    if(OrbitData.Satellites[i]->cal == false)
        OrbitData.Satellites[i]->Refresh(OrbitData.Satellites[i]->tle, OrbitData.Satellites[i]->obs, true, true);
    SatelliteModelInitialize(i);
    mim::Initialize_SatContext();   // ← VisibleSats 재계산
}

void TleVM::Unuse(int i)
{
    OrbitData.Satellites[i]->use = false;
    SatelliteModelDelete(i);
    mim::Initialize_SatContext();   // ← VisibleSats 재계산
}

LocalTime TleVM::NextAos(int i) const
{
    LocalTime t;
    t.month  = OrbitData.Satellites[i]->_nextaos[0].AddHours(9).Month();
    t.day    = OrbitData.Satellites[i]->_nextaos[0].AddHours(9).Day();
    t.hour   = OrbitData.Satellites[i]->_nextaos[0].AddHours(9).Hour();
    t.minute = OrbitData.Satellites[i]->_nextaos[0].AddHours(9).Minute();
    t.second = OrbitData.Satellites[i]->_nextaos[0].AddHours(9).Second();
    return t;
}

double TleVM::MaxElevationDeg(int i) const { return OrbitData.Satellites[i]->_max_elevation[0] * RAD_TO_DEG; }

int TleVM::LoadProgressPercent() const { return OrbitData.loadindex * 100 / OrbitData.tleallindex; }

TleVM & TleViewModel()
{
    static TleVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
