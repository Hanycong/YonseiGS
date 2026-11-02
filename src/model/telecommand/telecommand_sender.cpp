#include "telecommand_sender.h"

#include <stdlib.h>
#include <string.h>

#include "YonseiGS_config.h"   // HVD_TEST, MIM_PT_TMTC_TEST
#include "YonseiGS_coms.h"     // packetsign
#include "svc_tx.h"

namespace mim {
namespace tc {

void SendTelecommand(const void * cmd, uint16_t size)
{
    mim::svc::WaitTx();
    packetsign * packet = (packetsign *)malloc(2 + 2 + 4 + size);
    packet->Identifier = HVD_TEST;
    packet->PacketType = MIM_PT_TMTC_TEST;
    packet->Length = size;
    memcpy(packet->Data, cmd, size);
    mim::svc::SendUplink((void *)packet);
}

} // namespace tc
} // namespace mim
