#include "svc_tx.h"
#include "svc_thread.h"

#include "miman_config.h"
#include "miman_coms.h"
#include "miman_csp.h"

namespace mim {
namespace svc {

int WaitTx()
{
    return JoinThread(SLOT_TX);
}

int SendUplink(void * packet)
{
    return StartThread(SLOT_TX, task_uplink_onorbit, packet);
}

int StartPingScan()
{
    return StartThread(SLOT_TX, csp_ping_scan, NULL);
}

int StartPingConsole()
{
    return StartThread(SLOT_TX, csp_ping_console, NULL);
}

int StartPingEveryNode(void * arg)
{
    return StartThread(SLOT_TX, csp_ping_everynode_console, arg);
}

int StartDirectShell(void * shellcmd)
{
    return StartThread(SLOT_TX, Direct_Shell, shellcmd);
}

} // namespace svc
} // namespace mim
