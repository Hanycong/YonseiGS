#include "frequency_vm.h"

#include "miman_config.h"
#include "svc_tracking.h"
#include "gs100_radio.h"
#include "switch_box.h"
#include "gs_link.h"

extern Setup * setup;
extern StateCheckUnit State;
extern Console console;

namespace mim {
namespace vm {

double *   FrequencyVM::CenterFrequencyHz() { return &setup->default_freq; }
uint16_t * FrequencyVM::PingSize()          { return &setup->pingsize; }
uint32_t * FrequencyVM::DefaultDelay()      { return &setup->default_timeout; }
uint32_t * FrequencyVM::GuardDelay()        { return &setup->guard_delay; }
uint32_t * FrequencyVM::QueueDelay()        { return &setup->queue_delay; }

float    FrequencyVM::RxFrequencyMHz() const { return ((float)mim::model::Gs100().RxFrequencyHz()) / 1000000; }
float    FrequencyVM::TxFrequencyMHz() const { return ((float)mim::model::Gs100().TxFrequencyHz()) / 1000000; }
uint32_t FrequencyVM::RxBaud() const         { return mim::model::Gs100().RxBaud(); }
uint32_t FrequencyVM::TxBaud() const         { return mim::model::Gs100().TxBaud(); }
int16_t  FrequencyVM::LastRssi() const       { return mim::model::Gs100().LastRssi(); }
bool     FrequencyVM::DopplerRunning() const { return State.Doppler; }

void FrequencyVM::ApplyCenterFrequency()
{
    mim::model::Gs100Radio & radio = mim::model::Gs100();
    radio.SetRxFrequency(setup->default_freq);
    radio.SetTxFrequency(setup->default_freq);
    radio.SetRxBaud(4800);
    radio.SetTxBaud(4800);
    radio.ReadFrequencies();
    radio.ReadBauds();
}

void FrequencyVM::SwitchToRx()
{
    mim::model::RfSwitch().SwitchToRx(setup->ax100_node);
}

void FrequencyVM::SwitchToTx()
{
    mim::model::RfSwitch().SwitchToTx(setup->ax100_node);
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
    mim::model::Link().ApplyDelaySpec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);
    console.AddLog("Set Delay Spec : Default : %u, Guard : %u, Queue : %u", setup->default_timeout, setup->guard_delay, setup->queue_delay);
}

FrequencyVM & FrequencyViewModel()
{
    static FrequencyVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
