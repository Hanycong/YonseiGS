#include "shell_vm.h"

#include <stdio.h>

#include "direct_shell.h"

extern StateCheckUnit State;

namespace mim {
namespace vm {

using mim::model::Shell;

int * ShellVM::Type()             { return &LinkFlags.Shelltype; }
cmd_packet_t * ShellVM::Command() { return Shell().Command(); }

// (RadioButton 다음 매 프레임)
void ShellVM::SyncTypeChange()
{
    if(LinkFlags.Shelltype != (int)Shell().Type())
    {
        Shell().ChangeType((uint8_t)LinkFlags.Shelltype);
    }
}

// "Send##SendShellcmd"
void ShellVM::Send()
{
    if(State.Debugmode)
    {
        printf("Input Shell Data : ");
        for(int i = 0; i < sizeof(cmd_packet_t); i++)
            printf("%u\t", ((uint8_t *)Shell().Command())[i]);
        printf("\n");
    }
    Shell().Send();
}

ShellVM & ShellViewModel()
{
    static ShellVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
