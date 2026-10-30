#include "cmdgen_vm.h"

extern uint16_t gen_fnccode;
extern bool raw_data;

namespace mim {
namespace vm {

int * CmdGeneratorVM::SelectedCategory()               { return &selected_category_; }
int CmdGeneratorVM::CategoryCount() const              { return mim::tc::CategoryCount(); }
const char * CmdGeneratorVM::CategoryName(int i) const { return mim::tc::CategoryName(i); }

const char * CmdGeneratorVM::SelectedLabel() const
{
    const mim::tc::CommandDef* current = mim::tc::FindCommand(gen_fnccode);
    return current ? current->label : "Select CMD type";
}

int CmdGeneratorVM::PickerSize(bool initial_mode) const
{
    if (initial_mode)
    {
        const int* initial_ops = mim::tc::InitialOpsCommands();
        int n = 0;
        while (initial_ops[n] >= 0)
            n++;
        return n;
    }
    return mim::tc::CommandCount();
}

const mim::tc::CommandDef * CmdGeneratorVM::PickerItem(bool initial_mode, int k) const
{
    const mim::tc::CommandDef* cmd;
    if (initial_mode)
    {
        cmd = mim::tc::FindCommand(mim::tc::InitialOpsCommands()[k]);
        if (cmd == nullptr)
            return nullptr;
    }
    else
        cmd = &mim::tc::CommandAt(k);
    if (!mim::tc::InCategory(*cmd, selected_category_))
        return nullptr;
    return cmd;
}

bool CmdGeneratorVM::IsSelected(int command_id) const { return gen_fnccode == command_id; }

void CmdGeneratorVM::Select(int command_id)
{
    if (gen_fnccode != command_id)
        gen_fnccode = command_id;
}

uint16_t CmdGeneratorVM::SelectedCommand() const { return gen_fnccode; }
bool * CmdGeneratorVM::RawData()                 { return &raw_data; }

CmdGeneratorVM & CmdGeneratorViewModel()
{
    static CmdGeneratorVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
