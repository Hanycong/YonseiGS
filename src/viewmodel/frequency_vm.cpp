#include "frequency_vm.h"

#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_radial.h"
#include "svc_tracking.h"

extern Setup * setup;
extern StateCheckUnit State;
extern Console console;
extern Ptable_1 param1;
extern Ptable_5 param5;

namespace mim {
namespace vm {

double *   FrequencyVM::CenterFrequencyHz() { return &setup->default_freq; }
uint16_t * FrequencyVM::PingSize()          { return &setup->pingsize; }
uint32_t * FrequencyVM::DefaultDelay()      { return &setup->default_timeout; }
uint32_t * FrequencyVM::GuardDelay()        { return &setup->guard_delay; }
uint32_t * FrequencyVM::QueueDelay()        { return &setup->queue_delay; }

float    FrequencyVM::RxFrequencyMHz() const { return ((float)param1.freq) / 1000000; }
float    FrequencyVM::TxFrequencyMHz() const { return ((float)param5.freq) / 1000000; }
uint32_t FrequencyVM::RxBaud() const         { return param1.baud; }
uint32_t FrequencyVM::TxBaud() const         { return param5.baud; }
int16_t  FrequencyVM::LastRssi() const       { return Return_RSSI(); }
bool     FrequencyVM::DopplerRunning() const { return State.Doppler; }

void FrequencyVM::ApplyCenterFrequency()
{
    SetRxFreq(setup->default_freq);
    SetTxFreq(setup->default_freq);
    SetRxBaud(4800);
    SetTxBaud(4800);
    UpdateFreq();
    UpdateBaud();
}

void FrequencyVM::SwitchToRx()
{
    switch_to_rx(setup->ax100_node);
}

void FrequencyVM::SwitchToTx()
{
    switch_to_tx(setup->ax100_node);
}

void FrequencyVM::StartDoppler()
{
    mim::svc::StartDoppler();
}

void FrequencyVM::StopDoppler()
{
    mim::svc::StopDoppler();
}

void FrequencyVM::ApplyDelaySpec()
{
    set_serial_spec_micsec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);
    console.AddLog("Set Delay Spec : Default : %u, Guard : %u, Queue : %u", setup->default_timeout, setup->guard_delay, setup->queue_delay);
}

FrequencyVM & FrequencyViewModel()
{
    static FrequencyVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
