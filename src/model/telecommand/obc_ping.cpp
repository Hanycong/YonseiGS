#include "obc_ping.h"

#include <stdlib.h>

#include "miman_config.h"
#include "miman_orbital.h"

extern StateCheckUnit State;

namespace mim {
namespace model {

FSWTle * NewFswTleBuffer()
{
    return (FSWTle*)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
}

packetsign * BuildObcPing(FSWTle * buffer, int tracking_index)
{
    if (tracking_index == -1)
        FSWTleConverter(State.Fatellites->tle, buffer);
    else
        FSWTleConverter(State.Satellites[tracking_index]->tle, buffer);
    return PingInit(buffer);
}

} // namespace model
} // namespace mim
