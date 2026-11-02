#include "gs100_param_vm.h"

#include "svc_radio.h"

extern Ptable_0 param0;
extern Ptable_1 param1;
extern Ptable_5 param5;

namespace mim {
namespace vm {

Ptable_0 & Gs100ParamVM::Table0() { return param0; }
Ptable_1 & Gs100ParamVM::Table1() { return param1; }
Ptable_5 & Gs100ParamVM::Table5() { return param5; }

void Gs100ParamVM::Load(int table) { mim::svc::LoadParamTable(table); }
void Gs100ParamVM::Save(int table) { mim::svc::SaveParamTable(table); }

void Gs100ParamVM::SaveToFram(int8_t table)
{
    int8_t id = table;
    mim::svc::SaveParamToFRAM(&id);
}

Gs100ParamVM & Gs100ParamViewModel()
{
    static Gs100ParamVM vm;
    return vm;
}

} // namespace vm
} // namespace mim
