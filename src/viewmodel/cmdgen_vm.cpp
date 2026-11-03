#include "cmdgen_vm.h"
#include "sat_mode.h"

extern uint16_t gen_fnccode;
extern bool raw_data;

namespace mim {
namespace vm {

// 위성을 바꾸면 콤보를 "All" 로, 선택 명령을 그 위성의 첫 명령으로 (누리호 5차 GS 와 같음).
// BEE1000 으로 돌아올 때는 선택 명령을 비운다 (BEE-1000 폼에 없는 번호이므로)
void CmdGeneratorVM::SyncSatellite()
{
    const int sat = mim::sat::Current();
    if (sat == last_sat_)
        return;
    if (last_sat_ >= 0)
    {
        selected_category_ = 0;
        switch (sat)
        {
        case mim::sat::SAT_BEE1012: gen_fnccode = 1000;  break;
        case mim::sat::SAT_UELYSYS: gen_fnccode = 4000;  break;
        case mim::sat::SAT_SD1SAT:  gen_fnccode = 10000; break;
        default:                    gen_fnccode = 0xFFFF; break;
        }
    }
    last_sat_ = sat;
}

int * CmdGeneratorVM::SelectedCategory()               { SyncSatellite(); return &selected_category_; }
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
