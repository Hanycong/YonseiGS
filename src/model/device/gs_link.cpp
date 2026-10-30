#include "gs_link.h"

#include <csp/delay.h>

namespace mim {
namespace model {

void GsLink::ApplyDelaySpec(uint32_t kiss_baud, uint32_t switch_baud, uint32_t can_baud,
                            uint32_t queue_delay, uint8_t gs100_node,
                            uint32_t default_delay, uint32_t guard_delay)
{
    set_serial_spec_micsec(kiss_baud, switch_baud, can_baud, queue_delay, gs100_node, default_delay, guard_delay);
}

GsLink & Link()
{
    static GsLink link;
    return link;
}

} // namespace model
} // namespace mim
