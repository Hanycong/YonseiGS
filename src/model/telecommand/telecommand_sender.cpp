#include "telecommand_sender.h"

#include <stdlib.h>
#include <string.h>

#include "YonseiGS_config.h"   // HVD_TEST, MIM_PT_TMTC_TEST
#include "YonseiGS_coms.h"     // packetsign
#include "svc_tx.h"

namespace mim {
namespace tc {

void QueueTelecommand(const void * cmd, size_t size, uint16_t packet_type)
{
    packetsign * packet = (packetsign *)malloc(2 + 2 + 4 + size);
    packet->Identifier = HVD_TEST;
    packet->PacketType = packet_type;
    packet->Length = (uint16_t)size;
    memcpy(packet->Data, cmd, size);
    mim::svc::SendUplink((void *)packet);
}

void SendTelecommand(const void * cmd, size_t size, uint16_t packet_type)
{
    mim::svc::WaitTx();
    QueueTelecommand(cmd, size, packet_type);
}

void SendTelecommand(const void * cmd, uint16_t size)
{
    SendTelecommand(cmd, (size_t)size, MIM_PT_TMTC_TEST);
}

} // namespace tc
} // namespace mim
