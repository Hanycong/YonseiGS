#include "switch_box.h"

#include <csp/switch.h>

namespace mim {
namespace model {

int SwitchBox::SwitchToRx(uint8_t node) { return switch_to_rx(node); }
int SwitchBox::SwitchToTx(uint8_t node) { return switch_to_tx(node); }

SwitchBox & RfSwitch()
{
    static SwitchBox sw;
    return sw;
}

} // namespace model
} // namespace mim
