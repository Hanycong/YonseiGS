#include "gs100_radio.h"

#include "miman_config.h"
#include "miman_radial.h"

extern Ptable_1 param1;
extern Ptable_5 param5;

namespace mim {
namespace model {

void Gs100Radio::SetRxFrequency(uint32_t hz) { SetRxFreq(hz); }
void Gs100Radio::SetTxFrequency(uint32_t hz) { SetTxFreq(hz); }
void Gs100Radio::SetRxBaud(uint32_t bps)     { ::SetRxBaud(bps); }
void Gs100Radio::SetTxBaud(uint32_t bps)     { ::SetTxBaud(bps); }

void Gs100Radio::ReadFrequencies() { UpdateFreq(); }
void Gs100Radio::ReadBauds()       { UpdateBaud(); }

uint32_t Gs100Radio::RxFrequencyHz() const { return param1.freq; }
uint32_t Gs100Radio::TxFrequencyHz() const { return param5.freq; }
uint32_t Gs100Radio::RxBaud() const        { return param1.baud; }
uint32_t Gs100Radio::TxBaud() const        { return param5.baud; }
int16_t  Gs100Radio::LastRssi() const      { return Return_RSSI(); }

Gs100Radio & Gs100()
{
    static Gs100Radio radio;
    return radio;
}

} // namespace model
} // namespace mim
