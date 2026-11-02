#include "s_band_vm.h"

#include "miman_config.h"
#include "miman_s_control.h"

extern Sband * sgs;

namespace mim {
namespace vm {

char * SbandVM::Command() { return command_; }
size_t SbandVM::CommandSize() const { return sizeof(command_); }
void SbandVM::Send() { sgs->cmd(command_); }
const char * SbandVM::Output() const { return sgs->buffer; }

SbandVM & SbandViewModel()
{
    static SbandVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
