#include "command_catalog.h"

#include <string.h>
#include <algorithm>
#include <vector>
#include <iterator>

#include "sat_mode.h"

namespace mim {
namespace tc {

// 미션별 명령 정의 (src/model/missions/*, model/telecommand/raw_commands.cpp)
extern const MissionCommands kRawCommands;
extern const MissionCommands kBee1000Commands;
extern const MissionCommands kCosmicCommands;
extern const MissionCommands kBee1012Commands;
extern const MissionCommands kUelysysCommands;
extern const MissionCommands kSd1satCommands;

static const MissionCommands * const kMissions[] = {
    &kRawCommands,
    &kBee1000Commands,
    &kCosmicCommands,
};

// 누리호 5차 위성 (위성 모드 순서: mim::sat::SAT_BEE1012 .. SAT_SD1SAT)
static const MissionCommands * const kSatMissions[] = {
    nullptr,                // SAT_BEE1000: kMissions
    &kBee1012Commands,
    &kUelysysCommands,
    &kSd1satCommands,
};

// 지금 위성의 5차 미션 (BEE1000 이면 nullptr)
static const MissionCommands * SatMission()
{
    int mode = mim::sat::Current();
    if (mode <= 0 || mode >= (int)(sizeof(kSatMissions) / sizeof(kSatMissions[0])))
        return nullptr;
    return kSatMissions[mode];
}

// 기존 명령 생성기의 필터 콤보 순서 그대로.
// LGPM ~ PAYUEL_LGBAT 은 아직 명령이 하나도 없는 항목이다 (docs/CMDGEN_ANALYSIS.md D7).
static const char * const kCategories[] = {
    "All", "Test&Debug", "EPS", "ADCS", "UANT", "SP", "PAYUZUC", "Cosmic UEL Pi",
    "cFE", "SC APP", "FTP", "UTRX GNDWDT",
    "LGPM", "LTRX", "STX", "PAYUEL_ROMA", "PAYUEL_LGBAT"
};

static std::vector<const CommandDef *> Sorted(const std::vector<const MissionCommands *> & missions)
{
    std::vector<const CommandDef *> all;
    for (const MissionCommands * m : missions)
        for (int i = 0; i < m->count; i++)
            all.push_back(&m->commands[i]);
    std::stable_sort(all.begin(), all.end(),
                     [](const CommandDef * a, const CommandDef * b) { return a->id < b->id; });
    return all;
}

static const std::vector<const CommandDef *> & Merged()
{
    static std::vector<const CommandDef *> legacy;
    static std::vector<const CommandDef *> sat[sizeof(kSatMissions) / sizeof(kSatMissions[0])];
    const MissionCommands * m = SatMission();
    if (m == nullptr)
    {
        if (legacy.empty())
            legacy = Sorted(std::vector<const MissionCommands *>(std::begin(kMissions), std::end(kMissions)));
        return legacy;
    }
    std::vector<const CommandDef *> & list = sat[mim::sat::Current()];
    if (list.empty())
        list = Sorted({ &kRawCommands, m });
    return list;
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
    const MissionCommands * m = SatMission();
    if (m != nullptr)
        return m->category_count;
    return (int)(sizeof(kCategories) / sizeof(kCategories[0]));
}

const char * CategoryName(int index)
{
    const MissionCommands * m = SatMission();
    if (m != nullptr)
        return (index >= 0 && index < m->category_count) ? m->categories[index] : "";
    return kCategories[index];
}

bool InCategory(const CommandDef & cmd, int category_index)
{
    const MissionCommands * m = SatMission();
    if (m != nullptr)
    {
        // 누리호 5차 GS 화면: "All" 은 위성 명령만, 마지막 "Generic / Manual" 은 원시 명령만
        const bool raw = strcmp(cmd.mission, kRawCommands.mission) == 0;
        if (category_index == 0)
            return !raw;
        if (category_index == m->category_count - 1)
            return raw;
        return !raw && cmd.category != nullptr && strcmp(cmd.category, m->categories[category_index]) == 0;
    }
    if (category_index == 0)
        return true;    // "All"
    return cmd.category != nullptr && strcmp(cmd.category, kCategories[category_index]) == 0;
}

const int * InitialOpsCommands()
{
    // 초기 운용 세트는 BEE-1000 만 정의한다 (누리호 5차 위성은 없음)
    static const int kNone[] = { -1 };
    if (SatMission() != nullptr)
        return kNone;
    return kBee1000Commands.initial_ops;
}

} // namespace tc
} // namespace mim
