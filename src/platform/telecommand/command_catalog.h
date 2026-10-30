// 플랫폼 계층 - 텔레커맨드 카탈로그 (위성에 독립적인 형식)
//
// 지상국이 보낼 수 있는 명령의 "정체"(라벨, 대상 미션과 FSW 앱, MID, CC, 분류, 위험도)를
// 데이터로 표현한다. 명령마다 무엇을 입력받고 어떻게 조립하는지는 아직 명령 생성기
// (miman_imgui.cpp DrawCmdGeneratorBody)에 있고, 이후 단계에서 옮긴다.
//
// 위성별 명령 목록은 src/missions/<위성>/ 에서 MissionCommands 로 정의하고,
// command_catalog.cpp 의 미션 목록에 등록한다. 플랫폼 코드는 특정 위성을 모른다.
// 자세한 설계: docs/CMDGEN_ANALYSIS.md §6
#pragma once

#include <stdint.h>

namespace mim {
namespace tc {

// 명령 형태
enum class CmdKind : uint8_t
{
    Single,     // 패킷 1개, MID/CC 고정 (기본값으로 제시, 사용자가 바꿀 수 있는 경우도 있음)
    RawTyped,   // 사용자가 MID/CC/인자 타입을 직접 입력하는 디버그용 명령
    Sequence,   // 여러 패킷을 순서대로 보내는 운용 절차
};

// 운용 위험도 (메타데이터만, 현재 동작에는 영향 없음)
enum class Criticality : uint8_t
{
    Unclassified,   // 내용을 알 수 없음 (Raw, 실험용)
    Query,          // 조회. 위성 상태를 바꾸지 않음 (NoOp, Get, Query, Download ...)
    Config,         // 설정. 상태를 바꾸지만 되돌릴 수 있음
    Hazardous,      // 되돌릴 수 없거나 링크/전원/FSW 를 잃을 수 있음
                    // (전개, 버닝, 서브시스템 리셋, 전원 채널, 부트로더, 앱 정지, 파일 삭제)
};

struct CommandDef
{
    int         id;             // 명령 생성기 번호 (콤보 정렬 순서이자 생성기 case 번호)
    const char* label;          // 콤보에 보이는 이름
    const char* mission;        // "RAW"(플랫폼 공용), "BEE1000", "COSMIC" ...
    const char* app;            // 대상 FSW 앱 ("EPS", "ADCS", "SC", "TO_LAB", "ES", "FM" ...)
    const char* category;       // 명령 선택 콤보의 필터 항목 이름. nullptr 이면 "All" 에서만 보임
    uint16_t    mid;            // 기본 Message ID (RawTyped/Sequence/미구현은 0)
    uint8_t     cc;             // 기본 Function Code
    CmdKind     kind;
    Criticality criticality;
    bool        implemented;    // 생성기에 입력 폼/조립 코드가 있는지 (false 면 골라도 폼이 비어 있음)
};

// 한 미션(또는 플랫폼 공용)이 제공하는 명령 묶음
struct MissionCommands
{
    const char*       mission;
    const CommandDef* commands;
    int               count;
    const int*        initial_ops;        // 초기 운용(LEOP) 명령 세트. 순서가 운용 순서. -1 로 끝남. 없으면 nullptr
};

// 등록된 모든 미션의 명령을 id 순으로 합친 목록
int               CommandCount();
const CommandDef& CommandAt(int index);
const CommandDef* FindCommand(int id);      // 없으면 nullptr

// 명령 선택 콤보의 필터 항목 (0번은 "All"). 현재는 모든 미션 명령을 한 콤보에 보여 주는
// 기존 화면 구성을 그대로 따른다. 미션 선택 기능(설계 7단계)에서 미션별로 나뉜다.
int         CategoryCount();
const char* CategoryName(int index);
bool        InCategory(const CommandDef& cmd, int category_index);

// 초기 운용 창에 보일 명령 id 목록 (-1 로 끝남)
const int*  InitialOpsCommands();

} // namespace tc
} // namespace mim
