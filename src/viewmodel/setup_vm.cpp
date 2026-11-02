#include "setup_vm.h"

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"

extern StateCheckUnit State;
extern Setup * setup;

namespace mim {
namespace vm {

Setup & SetupVM::Config() { return *setup; }
bool * SetupVM::UseSband() { return &State.SbandUse; }

// "Start"
void SetupVM::Start()
{
    State.GUI = true;
    State.InitializeRequired = true;
    set_serial_spec_micsec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);
}

// "Reset Watchdog"
void SetupVM::ResetWatchdog() { ::ResetWatchdog(); }

// "Quit"
void SetupVM::Quit() { State.GUI = false; }

SetupVM & SetupViewModel()
{
    static SetupVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
