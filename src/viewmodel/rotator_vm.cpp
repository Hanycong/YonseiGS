#include "rotator_vm.h"

#include "miman_config.h"
#include "svc_tracking.h"
#include "rotator.h"

extern StateCheckUnit State;

namespace mim {
namespace vm {

int * RotatorVM::TargetAzimuth()   { return mim::model::Rotator().TargetAzimuth(); }
int * RotatorVM::TargetElevation() { return mim::model::Rotator().TargetElevation(); }

bool RotatorVM::Engaging() const         { return State.Engage; }
int  RotatorVM::CurrentAzimuth() const   { return mim::model::Rotator().CurrentAzimuth(); }
int  RotatorVM::CurrentElevation() const { return mim::model::Rotator().CurrentElevation(); }

void RotatorVM::StartEngage() { mim::svc::StartEngage(); }
void RotatorVM::StopEngage()  { mim::svc::StopEngage(); }

void RotatorVM::MoveToTarget()
{
    mim::model::Gs232bRotator & rotator = mim::model::Rotator();
    rotator.MoveTo(*rotator.TargetAzimuth(), *rotator.TargetElevation());
}

void RotatorVM::MoveToHome()
{
    mim::model::Rotator().MoveTo(0, 0);
}

void RotatorVM::RotatorOn()  { mim::svc::RotatorOn(); }
void RotatorVM::RotatorOff() { mim::svc::RotatorOff(); }

void RotatorVM::FlushIo()
{
    mim::model::Rotator().FlushIo();
}

RotatorVM & RotatorViewModel()
{
    static RotatorVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
