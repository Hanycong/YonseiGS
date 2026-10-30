#include "rotator_vm.h"

#include "miman_config.h"
#include "miman_orbital.h"
#include "svc_tracking.h"

extern StateCheckUnit State;
extern struct Antenna Now;

namespace mim {
namespace vm {

int * RotatorVM::TargetAzimuth()   { return &Now.az_tag; }
int * RotatorVM::TargetElevation() { return &Now.el_tag; }

bool RotatorVM::Engaging() const         { return State.Engage; }
int  RotatorVM::CurrentAzimuth() const   { return Now.az_now; }
int  RotatorVM::CurrentElevation() const { return Now.el_now; }

void RotatorVM::StartEngage() { mim::svc::StartEngage(); }
void RotatorVM::StopEngage()  { mim::svc::StopEngage(); }

void RotatorVM::MoveToTarget()
{
    RotatorSetTo(Now.az_tag, Now.el_tag);
}

void RotatorVM::MoveToHome()
{
    RotatorSetTo(0, 0);
}

void RotatorVM::RotatorOn()  { mim::svc::RotatorOn(); }
void RotatorVM::RotatorOff() { mim::svc::RotatorOff(); }

void RotatorVM::FlushIo()
{
    RotatorEnter(NULL);
}

RotatorVM & RotatorViewModel()
{
    static RotatorVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
