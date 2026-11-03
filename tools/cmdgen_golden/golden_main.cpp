// 명령 생성기 골든 패킷 하네스
//
// DrawCmdGeneratorBody 의 모든 명령에 대해 "Generate CMD" 를 화면 없이 눌러 보고,
// 송신 큐로 들어가는 packetsign 의 바이트를 그대로 출력한다.
// 리팩토링 전후 출력이 1바이트라도 다르면 위성으로 나가는 패킷이 바뀐 것이다.
//
// 동작 방식
//  - YonseiGS_imgui.cpp 를 이 파일에 직접 include 해서 static 함수/변수에 접근한다.
//  - ImGui::Button 을 이 파일에서 다시 정의해 "Generate CMD" 만 눌린 것으로 만든다
//    (링크 시 --allow-multiple-definition, 이 오브젝트를 라이브러리보다 먼저 둔다).
//  - svc_tx 대신 아래 가짜 구현을 링크해 SendUplink 로 들어온 패킷을 기록한다.
//  - 각 명령을 3번 누른다: 같은 명령 재전송 시 바이트가 같은지(D1) 확인용.
//  - 폼 지문(forms.txt): 명령마다 그려지는 위젯(종류, 라벨, 타입, 표시 문자열)을 순서대로 기록하고,
//    명령 선택 콤보의 항목 목록을 (전체/초기운용) x (서브시스템 필터) 조합별로 기록한다.
//    위젯 함수들도 이 파일에서 재정의하므로 실제로 그려지지는 않는다.
//  - 카탈로그 검사(catalog.txt): model/telecommand 명령 카탈로그를 생성기의 기존 표
//    (Templabels, FilterRules, INITIAL_CMD_INDICES)와 대조하고, 카탈로그의 MID/CC 가
//    실제로 만들어진 패킷 헤더와 같은지 확인한다. 불일치가 있으면 종료 코드 1.
//
// 빌드/실행: tools/cmdgen_golden.sh <OUT_DIR>

#define STB_IMAGE_IMPLEMENTATION
#include "../../src/view/YonseiGS_imgui.cpp"
#include "legacy_tables.h"

#include <string>
#include <vector>
#include <map>
#include "command_catalog.h"
#include "sat_mode.h"
#include <algorithm>

// ---- main.cpp 가 정의하던 전역 (main.cpp 는 링크하지 않는다) ----
FILE *log_ptr;
StateCheckUnit State;
Command * command;
Setup * setup = new Setup;
dlreqdata * request = (dlreqdata *)malloc(sizeof(dlreqdata));
bool Showstate = true;
volatile sig_atomic_t g_shutdown_requested = 0;
pthread_t p_thread[16];
pthread_mutex_t conn_lock = PTHREAD_MUTEX_INITIALIZER;
csp_debug_level_t debug_level;
char * fontfile = (char *)"../bin/font/Inconsolata.ttf";

// ---- 위젯 재정의: 버튼 주입 + 폼 지문 기록 ----
static bool g_press = false;                         // "Generate CMD" 를 눌린 것으로
static std::vector<std::string> * g_rec = nullptr;   // nullptr 이 아니면 위젯을 기록
static bool g_force_combo = false;                   // 콤보를 열린 것으로 (항목 목록 수집용)
static std::string g_combo;                          // 지금 열린 것으로 만든 콤보 라벨
static std::string g_pick_sub;                       // ##Subsystem 콤보에서 고를 항목 이름

static void Rec(const char * kind, const char * label, const char * extra = "")
{
    if (!g_rec) return;
    std::string line = std::string(kind) + " \"" + (label ? label : "") + "\"";
    if (extra && *extra) line += std::string(" ") + extra;
    g_rec->push_back(line);
}

static const char * TypeName(ImGuiDataType t)
{
    static const char * n[] = { "S8", "U8", "S16", "U16", "S32", "U32", "S64", "U64", "Float", "Double" };
    return (t >= 0 && t < (int)(sizeof(n) / sizeof(n[0]))) ? n[t] : "?";
}

namespace ImGui {
bool Button(const char* label, const ImVec2&)
{
    Rec("Button", label);
    if (!g_press)
        return false;
    // 5차 위성 폼은 "Generate CMD##<id>" 처럼 ID 를 붙인 버튼을 쓴다 (GS_nuri5 하네스와 같은 기준)
    if (mim::sat::Current() != mim::sat::SAT_BEE1000)
        return strncmp(label, "Generate CMD", 12) == 0;
    return strcmp(label, "Generate CMD") == 0;
}
bool InputScalar(const char* label, ImGuiDataType data_type, void*, const void*, const void*, const char* format, ImGuiInputTextFlags)
{
    char extra[64];
    snprintf(extra, sizeof(extra), "%s%s%s", TypeName(data_type), format ? " fmt=" : "", format ? format : "");
    Rec("InputScalar", label, extra);
    return false;
}
bool InputDouble(const char* label, double*, double, double, const char* format, ImGuiInputTextFlags)
{
    Rec("InputDouble", label, format);
    return false;
}
bool InputText(const char* label, char*, size_t buf_size, ImGuiInputTextFlags, ImGuiInputTextCallback, void*)
{
    char extra[32];
    snprintf(extra, sizeof(extra), "size=%zu", buf_size);
    Rec("InputText", label, extra);
    return false;
}
bool Checkbox(const char* label, bool*)
{
    Rec("Checkbox", label);
    return false;
}
void Text(const char* fmt, ...)
{
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Rec("Text", buf);
}
void TextUnformatted(const char* text, const char* text_end)
{
    std::string t = text_end ? std::string(text, text_end) : std::string(text);
    Rec("TextUnformatted", t.c_str());
}
void Separator()
{
    Rec("Separator", "");
}
bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags)
{
    Rec("Combo", label, preview_value);
    if (!g_force_combo)
        return false;
    g_combo = label;
    return true;
}
void EndCombo()
{
    g_combo.clear();
}
bool Selectable(const char* label, bool, ImGuiSelectableFlags, const ImVec2&)
{
    if (g_combo == "##Subsystem")
        return g_pick_sub == label;          // 원하는 필터를 고른다
    Rec("Item", label);                      // 명령 콤보 항목은 기록만
    return false;
}
}

// ---- svc_tx 가짜 구현: 송신 대신 기록 ----
static std::vector<std::string> g_sent;
static std::vector<std::vector<uint8_t>> g_sent_bytes;

static std::string Hex(const uint8_t * p, size_t n)
{
    std::string s;
    char b[4];
    for (size_t i = 0; i < n; i++) {
        snprintf(b, sizeof(b), "%02x", p[i]);
        if (i) s += ' ';
        s += b;
    }
    return s;
}

namespace mim {
namespace svc {
int WaitTx() { return 0; }
int SendUplink(void * packet)
{
    packetsign * ps = (packetsign *)packet;
    char head[64];
    snprintf(head, sizeof(head), "id=%u type=%u len=%u data=", ps->Identifier, ps->PacketType, ps->Length);
    // 노드 지정 명령(MIM_PT_NODE_TC)은 Data 앞 2바이트가 목적 노드
    const size_t n = ps->Length + (ps->PacketType == MIM_PT_NODE_TC ? 2 : 0);
    g_sent.push_back(std::string(head) + Hex(ps->Data, n));
    g_sent_bytes.push_back(std::vector<uint8_t>(ps->Data, ps->Data + ps->Length));
    return 0;
}
int StartPingScan() { return 0; }
int StartPingConsole() { return 0; }
int StartPingEveryNode(void *) { return 0; }
int StartDirectShell(void *) { return 0; }
} // namespace svc
} // namespace mim

static void Frame(bool initial_mode)
{
    ImGui::NewFrame();
    ImGui::Begin("golden");
    DrawCmdGeneratorBody(initial_mode);
    ImGui::End();
    ImGui::EndFrame();
}

// 명령 선택 콤보: (전체/초기운용) x (서브시스템 필터) 별 항목 목록
static void DumpPicker(FILE * out)
{
    // 필터 이름은 DrawCmdGeneratorBody 의 index_names 와 같은 순서로 둔다
    // (이름이 틀리면 필터가 바뀌지 않아 "All" 과 같은 목록이 나오므로 diff 에서 드러난다)
    static const char* index_names[] = {
        "All", "Test&Debug", "EPS", "ADCS", "UANT", "SP", "PAYUZUC", "Cosmic UEL Pi",
        "cFE", "SC APP", "FTP", "UTRX GNDWDT",
        "LGPM", "LTRX", "STX", "PAYUEL_ROMA", "PAYUEL_LGBAT"
    };
    for (int mode = 0; mode < 2; mode++)
        for (const char * sub : index_names)
        {
            std::vector<std::string> rec;
            g_force_combo = true; g_pick_sub = sub;
            Frame(mode == 1);                 // 이 프레임에서 필터가 바뀐다
            g_pick_sub = "";
            g_rec = &rec;
            Frame(mode == 1);                 // 바뀐 필터로 항목 수집
            g_rec = nullptr; g_force_combo = false;
            fprintf(out, "picker mode=%s filter=\"%s\"\n", mode ? "initial" : "all", sub);
            for (auto & l : rec)
                if (l.compare(0, 4, "Item") == 0)
                    fprintf(out, "  %s\n", l.c_str());
        }
    // 필터를 다시 All 로
    g_force_combo = true; g_pick_sub = "All"; Frame(false); g_pick_sub = ""; g_force_combo = false;
}

// 카탈로그 <-> 기존 표 대조. 불일치 수를 돌려준다.
static int CheckCatalog(FILE * out, const std::map<int, std::vector<uint8_t>> & first_packet)
{
    using namespace mim::tc;
    int bad = 0;
    auto fail = [&](const char * what, int id, const std::string & detail) {
        fprintf(out, "MISMATCH %s id=%d %s\n", what, id, detail.c_str());
        bad++;
    };

    // 0. 카탈로그 내용 덤프 (검토용)
    static const char * kinds[] = { "Single", "RawTyped", "Sequence" };
    static const char * crits[] = { "Unclassified", "Query", "Config", "Hazardous" };
    for (int k = 0; k < CommandCount(); k++)
    {
        const CommandDef & c = CommandAt(k);
        fprintf(out, "cmd id=%d mission=%s app=%s category=%s mid=0x%04x cc=%u kind=%s crit=%s impl=%d label=\"%s\"\n",
                c.id, c.mission, c.app, c.category ? c.category : "-", c.mid, c.cc,
                kinds[(int)c.kind], crits[(int)c.criticality], c.implemented, c.label);
    }

    // 1. 라벨: 기존 Templabels 와 같은 번호에 같은 문자열
    for (int i = 0; i < CMD_LABEL_MAX; i++)
    {
        const CommandDef * c = FindCommand(i);
        bool legacy = strlen(Templabels[i]) > 0;
        if (legacy != (c != nullptr))
            fail("exists", i, legacy ? Templabels[i] : c->label);
        else if (c && strcmp(c->label, Templabels[i]) != 0)
            fail("label", i, std::string(c->label) + " != " + Templabels[i]);
    }

    // 2. 필터: 기존 FilterRules 범위 소속 == 카탈로그 category
    static const char* index_names[] = {
        "All", "Test&Debug", "EPS", "ADCS", "UANT", "SP", "PAYUZUC", "Cosmic UEL Pi",
        "cFE", "SC APP", "FTP", "UTRX GNDWDT",
        "LGPM", "LTRX", "STX", "PAYUEL_ROMA", "PAYUEL_LGBAT"
    };
    int ncat = (int)(sizeof(index_names) / sizeof(index_names[0]));
    if (CategoryCount() != ncat)
        fail("category-count", -1, std::to_string(CategoryCount()));
    for (int k = 0; k < ncat && k < CategoryCount(); k++)
        if (strcmp(CategoryName(k), index_names[k]) != 0)
            fail("category-name", k, CategoryName(k));
    for (int k = 0; k < CommandCount(); k++)
    {
        const CommandDef & c = CommandAt(k);
        for (int sub = 1; sub < ncat; sub++)
        {
            bool legacy = false;
            for (const auto & rule : FilterRules)
                if (rule.sub_index == sub && c.id >= rule.start && c.id <= rule.end)
                    legacy = true;
            if (legacy != InCategory(c, sub))
                fail("category", c.id, index_names[sub]);
        }
    }

    // 3. 초기 운용 목록: 순서까지 같아야 함
    const int * ops = InitialOpsCommands();
    for (int k = 0; ; k++)
    {
        if (ops[k] != INITIAL_CMD_INDICES[k])
        {
            fail("initial-ops", k, std::to_string(ops[k]) + " != " + std::to_string(INITIAL_CMD_INDICES[k]));
            break;
        }
        if (ops[k] < 0)
            break;
    }

    // 4. MID/CC: Single 명령의 첫 패킷 헤더(바이트 0-1 MID 빅엔디안, 바이트 6 CC)와 비교
    for (int k = 0; k < CommandCount(); k++)
    {
        const CommandDef & c = CommandAt(k);
        if (c.kind != CmdKind::Single || !c.implemented)
            continue;
        auto it = first_packet.find(c.id);
        if (it == first_packet.end() || it->second.size() < 8)
        {
            fail("no-packet", c.id, c.label);
            continue;
        }
        const std::vector<uint8_t> & b = it->second;
        uint16_t mid = (uint16_t)((b[0] << 8) | b[1]);
        if (mid != c.mid || b[6] != c.cc)
        {
            char d[96];
            snprintf(d, sizeof(d), "catalog mid=0x%04x cc=%u, packet mid=0x%04x cc=%u", c.mid, c.cc, mid, b[6]);
            fail("mid/cc", c.id, d);
        }
    }

    fprintf(out, "result mismatches=%d\n", bad);
    return bad;
}

// ---- 누리호 5차 위성 (BEE1012 / UELYSYS / SD1-SAT) ----
// GS_nuri5 기준값 하네스와 같은 형식으로 쓴다 (diff 로 비교):
//   picker sat=<위성> idx=<콤보 번호> name="<항목>": <명령 번호...>
//   sat=<위성> cmd=<번호> press=<1|2> pkt=<k> label="..." id= type= len= data=...
// 패킷은 GS_nuri5 하네스처럼 0~CMD_LABEL5_MAX 의 모든 명령 번호를 위성마다 순서대로 누른다
// (폼끼리 공유하는 command 버퍼 상태까지 같은 순서로 만들기 위해)
static void Frame5()
{
    ImGui::NewFrame();
    ImGui::Begin("golden");
    DrawCmdGeneratorBody(false);
    ImGui::End();
    ImGui::EndFrame();
}

static void DumpSat5(FILE * pick, FILE * pkts, const std::vector<int> & press_ids)
{
    using namespace mim::tc;
    static const char * kSat[] = { "BEE1000", "BEE1012", "UELYSYS", "SD1SAT" };
    for (int sat = 1; sat < 4; sat++)
    {
        mim::sat::SetCurrent(sat);
        Frame5();
        for (int idx = 0; idx < CategoryCount(); idx++)
        {
            fprintf(pick, "picker sat=%s idx=%d name=\"%s\":", kSat[sat], idx, CategoryName(idx));
            for (int k = 0; k < CommandCount(); k++)
                if (InCategory(CommandAt(k), idx))
                    fprintf(pick, " %d", CommandAt(k).id);
            fprintf(pick, "\n");
        }
    }
    for (int sat = 1; sat < 4; sat++)
    {
        mim::sat::SetCurrent(sat);
        Frame5();
        for (int i : press_ids)
        {
            gen_fnccode = i;
            Frame5();
            const CommandDef * c = FindCommand(i);
            const char * label = c ? c->label : "";
            for (int p = 1; p <= 2; p++)
            {
                g_sent.clear();
                g_press = true; Frame5(); g_press = false;
                if (g_sent.empty())
                    fprintf(pkts, "sat=%s cmd=%d press=%d label=\"%s\" (no packet)\n", kSat[sat], i, p, label);
                for (size_t k = 0; k < g_sent.size(); k++)
                    fprintf(pkts, "sat=%s cmd=%d press=%d pkt=%zu label=\"%s\" %s\n", kSat[sat], i, p, k, label, g_sent[k].c_str());
            }
        }
    }
    mim::sat::SetCurrent(mim::sat::SAT_BEE1000);
}

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    command = new Command;
    memset(command, 0, sizeof(Command));
    Initialize_CMDLabels();

    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.DisplaySize = ImVec2(1920, 1080);
    io.DeltaTime = 1.0f / 60.0f;
    unsigned char * px; int w, h;
    io.Fonts->GetTexDataAsRGBA32(&px, &w, &h);

    FILE * forms = fopen(argc > 1 ? argv[1] : "/dev/null", "w");
    FILE * catalog = fopen(argc > 2 ? argv[2] : "/dev/null", "w");
    std::map<int, std::vector<uint8_t>> first_packet;
    DumpPicker(forms);

    // 콘솔/표준출력 잡음을 막기 위해 패킷 기록만 stderr 로 쓴다
    const int presses = 3;
    for (int i = 0; i < CMD_LABEL_MAX; i++)
    {
        if (strlen(Templabels[i]) == 0)
            continue;
        gen_fnccode = i;
        Frame(false);                       // 폼 static 초기화용 빈 프레임
        {
            std::vector<std::string> rec;
            g_rec = &rec;
            Frame(false);
            g_rec = nullptr;
            fprintf(forms, "form cmd=%d label=\"%s\"\n", i, Templabels[i]);
            for (auto & l : rec)
                fprintf(forms, "  %s\n", l.c_str());
        }
        for (int p = 1; p <= presses; p++)
        {
            g_sent.clear();
            g_sent_bytes.clear();
            g_press = true;
            Frame(false);
            g_press = false;
            if (g_sent.empty())
                fprintf(stderr, "cmd=%d press=%d label=\"%s\" (no packet)\n", i, p, Templabels[i]);
            if (p == 1 && !g_sent_bytes.empty())
                first_packet[i] = g_sent_bytes[0];
            for (size_t k = 0; k < g_sent.size(); k++)
                fprintf(stderr, "cmd=%d press=%d pkt=%zu label=\"%s\" %s\n", i, p, k, Templabels[i], g_sent[k].c_str());
        }
    }
    fclose(forms);
    int bad = CheckCatalog(catalog, first_packet);
    fclose(catalog);

    // 5차 위성: 누를 명령 번호 = 세 위성 카탈로그의 번호 + 원시 명령 (GS_nuri5 라벨이 있는 번호와 같아야 한다)
    if (argc > 4)
    {
        std::vector<int> ids;
        for (int sat = 1; sat < 4; sat++)
        {
            mim::sat::SetCurrent(sat);
            for (int k = 0; k < mim::tc::CommandCount(); k++)
                ids.push_back(mim::tc::CommandAt(k).id);
        }
        mim::sat::SetCurrent(mim::sat::SAT_BEE1000);
        std::sort(ids.begin(), ids.end());
        ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
        FILE * pick5 = fopen(argv[3], "w");
        FILE * pkts5 = fopen(argv[4], "w");
        DumpSat5(pick5, pkts5, ids);
        fclose(pick5);
        fclose(pkts5);
    }
    return bad ? 1 : 0;
}
