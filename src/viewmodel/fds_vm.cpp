#include "fds_vm.h"

#include <string.h>

#include "miman_config.h"
#include "miman_orbital.h"
#include "miman_model.h"

extern StateCheckUnit State;
extern Console console;

namespace mim {
namespace vm {

char * FdsVM::Path() { return path_; }
size_t FdsVM::PathSize() const { return sizeof(path_); }

void FdsVM::Load()
{
    ReadTLELines(path_, true);
    if(strlen(State.Fatellites->Name()) != 0)
    {
        State.Fatellites->use = true;
    }
    else
        console.AddLog("[ERROR]##Cannot Load FDS File.");
    SatelliteModelInitialize(-1);
}

FdsVM & FdsViewModel()
{
    static FdsVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
