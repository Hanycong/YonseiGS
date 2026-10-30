#include "direct_shell.h"

#include <string.h>

#include "svc_tx.h"

extern cmd_packet_t * shellcmd;

namespace mim {
namespace model {

cmd_packet_t * DirectShell::Command() { return shellcmd; }
uint8_t DirectShell::Type() const     { return shellcmd->type; }

void DirectShell::ChangeType(uint8_t type)
{
    shellcmd->type = type;
    memset(&shellcmd->required, 0, sizeof(shellcmd->required));
}

void DirectShell::Send()
{
    mim::svc::StartDirectShell((void *)shellcmd);
}

DirectShell & Shell()
{
    static DirectShell shell;
    return shell;
}

} // namespace model
} // namespace mim
