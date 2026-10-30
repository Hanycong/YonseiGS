// COSMIC 미션 명령 정의
//
// BEE-1000 지상국 코드에 덧붙여져 있던 COSMIC 위성 명령을 분리한 것.
// COSMIC 의 MID 는 십진수 체계(6257 등)를 쓴다. 207/208 은 여러 패킷을 보내는 운용 절차.
#include "command_catalog.h"
#include "miman_config.h"

namespace mim {
namespace tc {

static const CommandDef kCommands[] = {
    // id   label                                      mission    app        category          mid                          cc                                   kind              criticality            implemented
    { 203, "COSMIC FTP Send File Command",            "COSMIC",  "FTP",     "FTP",            0,                           0,                                   CmdKind::Single,  Criticality::Query,       true },
    { 207, "To COSMIC... From Yonsei...",             "COSMIC",  "TO_LAB",  "cFE",            0,                           0,                                   CmdKind::Sequence, Criticality::Config,      true },
    { 208, "COSMIC S/UHF Dual Emission For DoSsam",   "COSMIC",  "TO_LAB",  "cFE",            0,                           0,                                   CmdKind::Sequence, Criticality::Config,      true },
    { 209, "COSMIC EPS P31U SET CONFIG2",             "COSMIC",  "EPS",     "EPS",            6257,                        54,                                  CmdKind::Single,  Criticality::Config,      true },
    { 210, "COSMIC EPS P31U CONFIG2",                 "COSMIC",  "EPS",     "EPS",            6257,                        55,                                  CmdKind::Single,  Criticality::Config,      true },
    { 211, "COSMIC EPS P31U SET OUT SINGLE",          "COSMIC",  "EPS",     "EPS",            6257,                        10,                                  CmdKind::Single,  Criticality::Hazardous,   true },
    { 212, "COSMIC SP DEPLOY",                        "COSMIC",  "SP",      "SP",             6232,                        2,                                   CmdKind::Single,  Criticality::Hazardous,   true },
    { 214, "COSMIC UEL Pi ON",                        "COSMIC",  "UEL_PI",  "Cosmic UEL Pi",  6277,                        8,                                   CmdKind::Single,  Criticality::Config,      true },
    { 215, "COSMIC UEL Pi OFF",                       "COSMIC",  "UEL_PI",  "Cosmic UEL Pi",  6277,                        9,                                   CmdKind::Single,  Criticality::Config,      true },
};

extern const MissionCommands kCosmicCommands = {
    "COSMIC", kCommands, (int)(sizeof(kCommands) / sizeof(kCommands[0])), nullptr
};

} // namespace tc
} // namespace mim
