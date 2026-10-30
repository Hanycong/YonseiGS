#include "rotator.h"

#include "miman_config.h"
#include "miman_orbital.h"

extern struct Antenna Now;

namespace mim {
namespace model {

void Gs232bRotator::MoveTo(int azimuth_deg, int elevation_deg)
{
    RotatorSetTo(azimuth_deg, elevation_deg);
}

void Gs232bRotator::FlushIo()
{
    RotatorEnter(NULL);
}

int * Gs232bRotator::TargetAzimuth()   { return &Now.az_tag; }
int * Gs232bRotator::TargetElevation() { return &Now.el_tag; }

int Gs232bRotator::CurrentAzimuth() const   { return Now.az_now; }
int Gs232bRotator::CurrentElevation() const { return Now.el_now; }

Gs232bRotator & Rotator()
{
    static Gs232bRotator rotator;
    return rotator;
}

} // namespace model
} // namespace mim
