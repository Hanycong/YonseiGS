#include "command_catalog.h"

#include <string.h>
#include <algorithm>
#include <vector>

namespace mim {
namespace tc {

// 미션별 명령 정의 (src/model/missions/*, model/telecommand/raw_commands.cpp)
extern const MissionCommands kRawCommands;
extern const MissionCommands kBee1000Commands;
extern const MissionCommands kCosmicCommands;

static const MissionCommands * const kMissions[] = {
    &kRawCommands,
    &kBee1000Commands,
    &kCosmicCommands,
};

// 기존 명령 생성기의 필터 콤보 순서 그대로.
// LGPM ~ PAYUEL_LGBAT 은 아직 명령이 하나도 없는 항목이다 (docs/CMDGEN_ANALYSIS.md D7).
static const char * const kCategories[] = {
    "All", "Test&Debug", "EPS", "ADCS", "UANT", "SP", "PAYUZUC", "Cosmic UEL Pi",
    "cFE", "SC APP", "FTP", "UTRX GNDWDT",
    "LGPM", "LTRX", "STX", "PAYUEL_ROMA", "PAYUEL_LGBAT"
};

static const std::vector<const CommandDef *> & Merged()
{
    static std::vector<const CommandDef *> all;
    if (all.empty())
    {
        for (const MissionCommands * m : kMissions)
            for (int i = 0; i < m->count; i++)
                all.push_back(&m->commands[i]);
        std::sort(all.begin(), all.end(),
                  [](const CommandDef * a, const CommandDef * b) { return a->id < b->id; });
    }
    return all;
}

int CommandCount()
{
    return (int)Merged().size();
}

const CommandDef & CommandAt(int index)
{
    return *Merged()[index];
}

const CommandDef * FindCommand(int id)
{
    for (const CommandDef * c : Merged())
        if (c->id == id)
            return c;
    return nullptr;
}

int CategoryCount()
{
    return (int)(sizeof(kCategories) / sizeof(kCategories[0]));
}

const char * CategoryName(int index)
{
    return kCategories[index];
}

bool InCategory(const CommandDef & cmd, int category_index)
{
    if (category_index == 0)
        return true;    // "All"
    return cmd.category != nullptr && strcmp(cmd.category, kCategories[category_index]) == 0;
}

const int * InitialOpsCommands()
{
    // 초기 운용 세트는 현재 BEE-1000 만 정의한다
    return kBee1000Commands.initial_ops;
}

} // namespace tc
} // namespace mim
