#include "satinfo_vm.h"

#include <stdio.h>

#include "miman_config.h"
#include "miman_orbital.h"   // RAD_TO_DEG

extern StateCheckUnit State;

namespace mim {
namespace vm {

const char * SatInfoVM::Title(int index)
{
    if(index == -1)
        sprintf(title_, "Details : %s", State.Fatellites->Name());
    else
        sprintf(title_, "Details : %s", State.Satellites[index]->Name());
    return title_;
}

static LocalTime ToLocal(DateTime t)
{
    LocalTime l;
    l.month  = t.AddHours(9).Month();
    l.day    = t.AddHours(9).Day();
    l.hour   = t.AddHours(9).Hour();
    l.minute = t.AddHours(9).Minute();
    l.second = t.AddHours(9).Second();
    return l;
}

bool SatInfoVM::Pass(int index, int i, PassRow * row) const
{
    SatelliteObject * sat;
    if(index == -1)
    {
        if(State.Fatellites->_max_elevation[i] <= 0.0f || State.Satellites[i]->_max_elevation[i] > 90.0f)
            return false;
        sat = State.Fatellites;
    }
    else
    {
        if(State.Satellites[index]->_max_elevation[i] <= 0.0f || State.Satellites[index]->_max_elevation[i] > 90.0f)
            return false;
        sat = State.Satellites[index];
    }
    row->aos = ToLocal(sat->_nextaos[i]);
    row->los = ToLocal(sat->_nextlos[i]);
    row->max_elevation_deg = sat->_max_elevation[i] * RAD_TO_DEG;
    return true;
}

SatInfoVM & SatInfoViewModel()
{
    static SatInfoVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
