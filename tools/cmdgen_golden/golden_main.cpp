// 명령 생성기 골든 패킷 하네스
//
// DrawCmdGeneratorBody 의 모든 명령에 대해 "Generate CMD" 를 화면 없이 눌러 보고,
// 송신 큐로 들어가는 packetsign 의 바이트를 그대로 출력한다.
// 리팩토링 전후 출력이 1바이트라도 다르면 위성으로 나가는 패킷이 바뀐 것이다.
//
// 동작 방식
//  - miman_imgui.cpp 를 이 파일에 직접 include 해서 static 함수/변수에 접근한다.
//  - ImGui::Button 을 이 파일에서 다시 정의해 "Generate CMD" 만 눌린 것으로 만든다
//    (링크 시 --allow-multiple-definition, 이 오브젝트를 라이브러리보다 먼저 둔다).
//  - svc_tx 대신 아래 가짜 구현을 링크해 SendUplink 로 들어온 패킷을 기록한다.
//  - 각 명령을 3번 누른다: 같은 명령 재전송 시 바이트가 같은지(D1) 확인용.
//
// 빌드/실행: tools/cmdgen_golden.sh <OUT_DIR>

#define STB_IMAGE_IMPLEMENTATION
#include "../../src/miman_imgui.cpp"

#include <string>
#include <vector>

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

// ---- 버튼 주입 ----
static bool g_press = false;

namespace ImGui {
bool Button(const char* label, const ImVec2& size)
{
    if (g_press && strcmp(label, "Generate CMD") == 0)
        return true;
    return ButtonEx(label, size, ImGuiButtonFlags_None);
}
}

// ---- svc_tx 가짜 구현: 송신 대신 기록 ----
static std::vector<std::string> g_sent;

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
    g_sent.push_back(std::string(head) + Hex(ps->Data, ps->Length));
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

    // 콘솔/표준출력 잡음을 막기 위해 패킷 기록만 stderr 로 쓴다
    const int presses = 3;
    for (int i = 0; i < CMD_LABEL_MAX; i++)
    {
        if (strlen(Templabels[i]) == 0)
            continue;
        // D12: "u16" 은 9바이트 버퍼에 2바이트 인자를 써서 스택을 넘친다 (fortify 가 abort).
        // 결과가 정의되지 않으므로 비교 대상에서 뺀다. (docs/CMDGEN_ANALYSIS.md §4)
        if (i == 2)
        {
            fprintf(stderr, "cmd=%d press=- label=\"%s\" (skipped: D12 stack overflow)\n", i, Templabels[i]);
            continue;
        }
        gen_fnccode = i;
        Frame(false);                       // 폼 static 초기화용 빈 프레임
        for (int p = 1; p <= presses; p++)
        {
            g_sent.clear();
            g_press = true;
            Frame(false);
            g_press = false;
            if (g_sent.empty())
                fprintf(stderr, "cmd=%d press=%d label=\"%s\" (no packet)\n", i, p, Templabels[i]);
            for (size_t k = 0; k < g_sent.size(); k++)
                fprintf(stderr, "cmd=%d press=%d pkt=%zu label=\"%s\" %s\n", i, p, k, Templabels[i], g_sent[k].c_str());
        }
    }
    return 0;
}
