// 플랫폼 공용 Raw 명령: 사용자가 MID / CC / 인자 타입을 직접 입력하는 디버그용 명령.
// 어느 cFS 위성에나 쓸 수 있으므로 미션이 아니라 플랫폼에 둔다.
#include "command_catalog.h"

namespace mim {
namespace tc {

static const CommandDef kCommands[] = {
    // id   label                                      mission    app        category          mid                          cc                                   kind              criticality            implemented
    {   0, "no arg",                                  "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   1, "u8",                                      "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   2, "u16",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   3, "u32",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   4, "u64",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   5, "s8",                                      "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   6, "s16",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   7, "s32",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   8, "s64",                                     "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
    {   9, "float",                                   "RAW",     "RAW",     "Test&Debug",     0,                           0,                                   CmdKind::RawTyped, Criticality::Unclassified, true },
};

extern const MissionCommands kRawCommands = {
    "RAW", kCommands, (int)(sizeof(kCommands) / sizeof(kCommands[0])), nullptr
};

} // namespace tc
} // namespace mim
