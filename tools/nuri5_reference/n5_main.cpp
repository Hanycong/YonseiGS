// 누리호 5차 GS (GS_nuri5) 명령 생성기 기준값 하네스
// - 위성 모드(BEE1012/UELYSYS/SD1SAT)별 서브시스템 콤보 항목과 명령 콤보 목록을 덤프
// - 모든 명령 번호에 대해 "Generate CMD..." 버튼을 눌러 업링크로 들어가는 packetsign 바이트를 덤프
#define STB_IMAGE_IMPLEMENTATION
#include "miman_imgui.cpp"
#include <string>
#include <vector>
#include <map>

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
void LockTcLog() {}
void UnlockTcLog() {}
bool SwitchTcLogFile(const char*) { return true; }

static bool g_press = false;

namespace ImGui {
bool Button(const char* label, const ImVec2&)
{
    return g_press && strncmp(label, "Generate CMD", 12) == 0;
}
}

static std::vector<std::string> g_sent;
static std::string Hex(const uint8_t * p, size_t n)
{
    std::string s; char b[4];
    for (size_t i = 0; i < n; i++) { snprintf(b, sizeof(b), "%02x", p[i]); if (i) s += ' '; s += b; }
    return s;
}
void * task_uplink_onorbit(void * sign_)
{
    packetsign * ps = (packetsign *)sign_;
    char head[64];
    snprintf(head, sizeof(head), "id=%u type=%u len=%u data=", ps->Identifier, ps->PacketType, ps->Length);
    // BEE1000(노드 지정) 타입은 앞 2바이트 노드 + Length 바이트
    size_t n = ps->Length + (ps->PacketType == 14 ? 2 : 0);
    g_sent.push_back(std::string(head) + Hex(ps->Data, n));
    return NULL;
}
void * Record_TC_Packet(packetsign*, const char*) { return NULL; }

static void Frame()
{
    ImGui::NewFrame();
    ImGui::Begin("golden");
    DrawCmdGeneratorBody(false);
    ImGui::End();
    ImGui::EndFrame();
}

static const char * kSat[] = { "BEE1000", "BEE1012", "UELYSYS", "SD1SAT" };

int main(int argc, char ** argv)
{
    log_ptr = fopen("/dev/null", "w");
    command = new Command;
    memset(command, 0, sizeof(Command));
    Initialize_CMDLabels();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = NULL; io.DisplaySize = ImVec2(1920, 1080); io.DeltaTime = 1.0f / 60.0f;
    unsigned char * px; int w, h; io.Fonts->GetTexDataAsRGBA32(&px, &w, &h);
    FILE * pick = fopen(argc > 1 ? argv[1] : "/dev/null", "w");

    static const char* bee1012_index_names[] = {
        "All",
        "adcs", "ci_lab", "ds", "eps", "fm", "aiobc",
        "gpio", "gps", "hk", "ltrx", "meow", "mission", "pay_slt",
        "paybee_kisscam", "rpt", "sch_lab", "stx", "to_lab", "ttc", "utrx",
        "cfe_es", "cfe_evs", "cfe_sb", "cfe_tbl", "cfe_time", "cfe_srl",
        "thrust",
        "Generic / Manual"
    };
    static const char* uelysys_index_names[] = {
        "All",
        "MEOW", "adcs", "ci_lab", "ds", "eo", "eps", "fm",
        "gpio", "gps", "hk", "payuel_aos",
        "payuel_cam", "payuel_lgbat", "payuel_lgpm", "payuel_obc",
        "payuel_roma", "payuel_yonsei", "rpt", "sch_lab", "sp", "stx",
        "to_lab", "ttc", "uant", "utrx",
        "cfe_es", "cfe_evs", "cfe_sb", "cfe_tbl", "cfe_time", "cfe_srl",
        "PAYYONSEI_OBC", "SDR_CONTROLLER",
        "Generic / Manual"
    };
    static const char* sd1sat_index_names[] = {
        "All",
        "ci", "cr", "ds", "eps", "hk", "ops", "paysd1_roma", "paysd1_lothan", "sch_lab",
        "to", "uant", "utrx", "SNBP", "MEOW", "TTC", "FM", "GPS",
        "Generic / Manual"
    };
    for (int sat = 1; sat < 4; sat++)
    {
        const char* const* names = sat == 3 ? sd1sat_index_names : sat == 2 ? uelysys_index_names : bee1012_index_names;
        int count = sat == 3 ? IM_ARRAYSIZE(sd1sat_index_names) : sat == 2 ? IM_ARRAYSIZE(uelysys_index_names) : IM_ARRAYSIZE(bee1012_index_names);
        for (int idx = 0; idx < count; idx++)
        {
            fprintf(pick, "picker sat=%s idx=%d name=\"%s\":", kSat[sat], idx, names[idx]);
            for (int i = 0; i < CMD_LABEL_MAX; i++)
            {
                if (strlen(Templabels[i]) == 0) continue;
                bool v;
                if (idx == 0) v = sat == 3 ? IsSd1satCommandIndex(i) : sat == 2 ? IsUelysysCommandIndex(i) : IsBee1012CommandIndex(i);
                else if (idx == count - 1) v = IsGenericCommandIndex(i);
                else if (sat == 3) v = IsSd1satCommandForApp(idx, i);
                else if (sat == 2) v = IsUelysysCommandForApp(idx, i);
                else { v = false; for (const auto& r : FilterRules) if (r.sub_index == idx && i >= r.start && i <= r.end) v = true; }
                if (v) fprintf(pick, " %d", i);
            }
            fprintf(pick, "\n");
        }
    }
    for (int i = 0; i < CMD_LABEL_MAX; i++)
        if (strlen(Templabels[i]) > 0)
            fprintf(pick, "label %d \"%s\"\n", i, Templabels[i]);
    fclose(pick);

    // 패킷: 위성 모드별, 모든 명령 번호
    for (int sat = 1; sat < 4; sat++)
    {
        current_sat_mode = sat;
        Frame();
        for (int i = 0; i < CMD_LABEL_MAX; i++)
        {
            if (strlen(Templabels[i]) == 0)
                continue;
            gen_fnccode = i;
            Frame();
            for (int p = 1; p <= 2; p++)
            {
                g_sent.clear();
                g_press = true; Frame(); g_press = false;
                pthread_join(p_thread[4], NULL);
                if (g_sent.empty())
                    fprintf(stderr, "sat=%s cmd=%d press=%d label=\"%s\" (no packet)\n", kSat[sat], i, p, Templabels[i]);
                for (size_t k = 0; k < g_sent.size(); k++)
                    fprintf(stderr, "sat=%s cmd=%d press=%d pkt=%zu label=\"%s\" %s\n", kSat[sat], i, p, k, Templabels[i], g_sent[k].c_str());
            }
        }
    }
    return 0;
}
