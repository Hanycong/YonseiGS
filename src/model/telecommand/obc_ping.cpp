#include "obc_ping.h"

#include <stdlib.h>

#include "YonseiGS_config.h"
#include "YonseiGS_orbital.h"

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
        FSWTleConverter(OrbitData.Fatellites->tle, buffer);
    else
        FSWTleConverter(OrbitData.Satellites[tracking_index]->tle, buffer);
    return PingInit(buffer);
}

} // namespace model
} // namespace mim
