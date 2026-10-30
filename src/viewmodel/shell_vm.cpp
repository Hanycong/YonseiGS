#include "shell_vm.h"

#include <stdio.h>
#include <string.h>

#include "svc_tx.h"

extern StateCheckUnit State;
extern cmd_packet_t * shellcmd;

namespace mim {
namespace vm {

int * ShellVM::Type()             { return &State.Shelltype; }
cmd_packet_t * ShellVM::Command() { return shellcmd; }

// "(RadioButton 다음 매 프레임)"
void ShellVM::SyncTypeChange()
{
    if(State.Shelltype != (int)shellcmd->type)
    {
        shellcmd->type = (uint8_t)State.Shelltype;
        memset(&shellcmd->required, 0, sizeof(shellcmd->required));
    }
}

// "Send##SendShellcmd"
void ShellVM::Send()
{
    if(State.Debugmode)
    {
        printf("Input Shell Data : ");
        for(int i = 0; i < sizeof(cmd_packet_t); i++)
            printf("%u\t", ((uint8_t *)shellcmd)[i]);
        printf("\n");
    }
    mim::svc::StartDirectShell((void *)shellcmd); 
}

ShellVM & ShellViewModel()
{
    static ShellVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
