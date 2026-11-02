#include "command_queue.h"

#include <stddef.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"

extern int NowCMD;
extern CmdGenerator_GS * SatCMD[256];

namespace mim {
namespace model {

void CommandQueue::Clear()
{
    NowCMD = 0;
    for (int i = 0; i < 256; i++)
    {
        if (SatCMD[i] != NULL)
        {
            delete SatCMD[i];
            SatCMD[i] = NULL;
        }
    }
}

CommandQueue & Commands()
{
    static CommandQueue queue;
    return queue;
}

} // namespace model
} // namespace mim
