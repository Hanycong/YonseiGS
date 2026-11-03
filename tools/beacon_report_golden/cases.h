// Beacon / Report 탭 화면 지문 하네스: 사례 만들기와 실행 (이 저장소 / GS_nuri5 양쪽 하네스가 같이 쓴다)
//
// 각 하네스가 정의할 것:
//   static void SideSetSat(int sat);                 // 위성 모드 (0 BEE1000, 1 BEE1012, 2 UELYSYS, 3 SD1SAT)
//   static void SideBee1000Beacon(bool null);        // BEE-1000 비콘 버퍼를 채울 수 있게 (null 이면 NULL 로)
//   static void * SideBee1000BeaconPtr(size_t * sz);
//   static void SideDrawBeaconWindow();              // Beacon 창 (Begin ~ End). 탭은 g_tab 으로 고른다
//
// 사례
//   beacon: 위성 4종 x {NULL, 무늬 1, 무늬 2, 0}
//   report: 위성 x 리포트 종류(0 ~ REPORT_KIND_SC_GENERIC) x reflected_cc x raw_size.
//     raw_bytes 와 union u 는 같은 바이트 무늬, 나머지 필드(msg id, ret_type, payload_valid, is_critical,
//     RPT Ops, EPS HK 상태, RPARAM 재조립 ...)는 (종류, cc, n) 으로 정해지는 값을 돌려 쓴다.
// 출력
//   list: (위성, 종류, cc) 마다 한 줄 "R sat kind cc hash" (n 전체를 묶은 지문), beacon 은 사례마다 "B ..."
//   dump sat kind cc n: 한 사례의 기록 전체
#pragma once

#include <stdlib.h>
#include <vector>

static void FillPattern(void * dst, size_t size, unsigned seed)
{
    uint8_t * p = (uint8_t *)dst;
    for (size_t i = 0; i < size; i++)
        p[i] = (uint8_t)((i * 131u + seed * 17u + (i >> 8) * 7u + 3u) & 0xFF);
}

static unsigned Mix(unsigned x0, unsigned x1, unsigned x2)
{
    unsigned h = 2166136261u;
    h = (h ^ x0) * 16777619u; h = (h ^ x1) * 16777619u; h = (h ^ x2) * 16777619u;
    h ^= h >> 13; h *= 0x5bd1e995u; h ^= h >> 15;
    return h;
}

static BEE1012_Beacon_t * g_save1012; static UELYSYS_Beacon_t * g_saveUel; static SD1SAT_Beacon_t * g_saveSd1;

static void SetupBeacons(int variant)
{
    if (!g_save1012) { g_save1012 = bee1012_beacon; g_saveUel = uelysys_beacon; g_saveSd1 = sd1sat_beacon; }
    bee1012_beacon = g_save1012; uelysys_beacon = g_saveUel; sd1sat_beacon = g_saveSd1;
    SideBee1000Beacon(variant == 0);
    if (variant == 0) { bee1012_beacon = NULL; uelysys_beacon = NULL; sd1sat_beacon = NULL; return; }
    size_t sz = 0;
    void * b0 = SideBee1000BeaconPtr(&sz);
    if (variant == 3) {
        memset(bee1012_beacon, 0, sizeof(*bee1012_beacon)); memset(uelysys_beacon, 0, sizeof(*uelysys_beacon));
        memset(sd1sat_beacon, 0, sizeof(*sd1sat_beacon)); memset(b0, 0, sz);
        return;
    }
    FillPattern(bee1012_beacon, sizeof(*bee1012_beacon), variant);
    FillPattern(uelysys_beacon, sizeof(*uelysys_beacon), variant + 10);
    FillPattern(sd1sat_beacon, sizeof(*sd1sat_beacon), variant + 20);
    FillPattern(b0, sz, variant + 30);
    // 호출 부호는 글자로 (끝은 널 문자 없이 꽉 채움)
    memcpy(bee1012_beacon->header.call_sign, "BEE1012", 7);
    memcpy(uelysys_beacon->header.call_sign, "UELYSYS", 7);
    memcpy(sd1sat_beacon->header.call_sign, "SD1S", 4);
}

static void SetupReport(int kind, int cc, int n)
{
    const unsigned h = Mix((unsigned)kind, (unsigned)cc, (unsigned)n);
    ReportView_t & v = g_report_view;
    memset(&v, 0, sizeof(v));
    FillPattern(&v.u, sizeof(v.u), (unsigned)cc);
    memcpy(v.raw_bytes, &v.u, sizeof(v.raw_bytes) < sizeof(v.u) ? sizeof(v.raw_bytes) : sizeof(v.u));
    if (kind == REPORT_KIND_EPS_QUERY_RAW)
        v.u.uelysys.size = (uint16_t)n;
    // STX DIR/DIRNEXT: 항목 이름은 널 문자로 끝나게 (GS_nuri5 는 %s 로 옆 필드까지 읽는다)
    if (kind == REPORT_KIND_UELYSYS_STX && (cc == UELYSYS_STX_FILESYS_CC_DIR || cc == UELYSYS_STX_FILESYS_CC_DIRNEXT))
        for (size_t off = 4 + 30; off < sizeof(v.raw_bytes); off += sizeof(stx_dir_entry_t)) v.raw_bytes[off] = 0;
    v.valid = true;
    v.CCMessage_ID = (uint16_t)(0x1800 + kind);
    v.CCCount = (uint16_t)(h & 0x3FFF);
    v.CCLength = (uint16_t)(n + 15);
    for (int i = 0; i < 6; i++) v.CCTime_code[i] = (uint8_t)(h >> (i * 4));
    static const uint16_t mids[] = { 0x0000, BEE1012_MEOW_CMD_ID, 0x8218, UELYSYS_MEOW_CMD_ID, 0x8618, 0x1830, 0x18A0 };
    v.reflected_msg_id = mids[h % (sizeof(mids) / sizeof(mids[0]))];
    v.reflected_cc = (uint8_t)cc;
    static const uint8_t rts[] = { 0, 0, 1, 3 };
    v.ret_type = rts[(h >> 3) & 3];
    v.ret_code = (int32_t)((h >> 5) % 7) - 3;
    v.ret_val_size = (uint16_t)n;
    if ((h & 0x700) == 0x100) v.ret_val_size = (uint16_t)(n + 9);      // raw 보다 큼 (truncated)
    v.is_critical = (h >> 11) & 1;
    v.crit_time_sec = h ^ 0x5A5A;
    v.crit_time_subsec = h >> 7;
    v.payload_valid = ((h >> 12) & 15) != 0;                         // 1/16 은 해석 실패
    static const int8_t romas[] = { -1, 7, 9 };
    v.roma_parget_index = romas[(h >> 16) % 3];
    v.raw_size = (uint16_t)n;
    v.kind = (ReportKind_t)kind;
    FillPattern(&v.eps_meta, sizeof(v.eps_meta), h & 0xFF);
    if ((h >> 18) & 1) v.eps_meta.data_id = EPS_QUERY_REPORT_CSP_PING_MS;

    // EPS HK 마지막 값 / RPARAM 재조립 / RPT Ops
    EPS_Report_State_t & es = g_eps_report_state;
    FillPattern(&es, sizeof(es), (h >> 20) & 0xFF);
    es.power_if_status_valid = (h >> 19) & 1; es.power_if_list_valid = (h >> 20) & 1;
    es.pmu_hk_valid = (h >> 21) & 1; es.pdu_hk_valid = (h >> 22) & 1; es.acu1_hk_valid = (h >> 23) & 1;
    es.acu2_hk_valid = (h >> 24) & 1; es.bp8_hk_valid = (h >> 25) & 1;
    EPS_RParam_Reassembly_State_t & er = g_eps_rparam_reassembly;
    FillPattern(er.buffer, 600, h & 0xFF);
    er.active = (h >> 26) & 1; er.valid = (h >> 27) & 1; er.complete = (h >> 28) & 1; er.error = (h >> 29) & 1;
    er.duplicate_seen = (h >> 30) & 1; er.missing_sequence_seen = (h >> 31) & 1;
    er.source = 1; er.data_id = 2; er.arg0 = 3; er.arg1 = 4;
    er.expected_sequence = (uint16_t)(h & 31); er.last_sequence = (uint16_t)((h >> 5) & 31);
    er.total_size = (uint16_t)(h % 600); er.received_size = (uint16_t)((h >> 9) % 600);
    er.last_offset = (uint16_t)(h >> 3); er.last_chunk_size = (uint16_t)(h >> 13);
    FillPattern(&g_rpt_ops_data, sizeof(g_rpt_ops_data), h & 0xFF);
    g_rpt_ops_data_valid = (h >> 14) & 1;
    // BRG_LEGACY=1: 지금의 BEE-1000 수신 모델처럼 5차 필드를 비운다
    // (is_critical / RPT Ops / payload_valid 를 채우지 않음) — 예전 커밋과 BEE-1000 화면 비교용
    if (getenv("BRG_LEGACY"))
    {
        v.is_critical = false; v.crit_time_sec = 0; v.crit_time_subsec = 0;
        v.payload_valid = false; g_rpt_ops_data_valid = false;
    }
}

// 창 크기를 고정하고 그린다 (창은 InitImGui 에서 미리 만들어 둔다)
static std::string Render()
{
    for (int i = 0; i < 1; i++)
    {
        g_rec.clear();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(1200, 1000));
        SideDrawBeaconWindow();
        ImGui::EndFrame();
    }
    return g_rec;
}

static void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = NULL; io.DisplaySize = ImVec2(1920, 1080); io.DeltaTime = 1.0f / 60.0f;
    unsigned char * px; int w, h; io.Fonts->GetTexDataAsRGBA32(&px, &w, &h);
    // 창을 먼저 만들어 둔다 (새 창의 첫 프레임은 크기가 0 이라 BeginTable 이 실패하고,
    // GS_nuri5 의 Beacon 탭은 그때도 표 함수를 불러 assert 로 죽는다)
    for (int i = 0; i < 3; i++)
    {
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(1200, 1000));
        ImGui::Begin("Beacon GUI");
        ImGui::End();
        ImGui::EndFrame();
    }
}

// 양쪽 모두 건너뛰는 리포트 종류 (GS_nuri5 쪽이 죽는 것. 이 저장소 쪽은 dump 로 따로 본다)
//   ADCS_GET_ERROR_LOG_SETTING: GS_nuri5 가 ImGui::Text("%u (%s)", x) 로 없는 문자열 인자를 읽는다
static bool SkipKind(int kind)
{
    return kind == REPORT_KIND_ADCS_GET_ERROR_LOG_SETTING;
}

// argv: list <sats> <cc_max> <n_max> [<kind_first> <kind_last> <cc_first>] | dump <sat> <kind> <cc> <n> | beacon
static int RunCases(int argc, char ** argv)
{
    InitImGui();
    const std::string mode = argc > 1 ? argv[1] : "list";
    if (mode == "beacon" || mode == "list")
    {
        g_tab = "Beacon";
        for (int sat = 0; sat < 4; sat++)
            for (int var = 0; var < 4; var++)
            {
                SideSetSat(sat);
                SetupBeacons(var);
                std::string r = Render();
                if (mode == "beacon") printf("=== B sat=%d var=%d\n%s", sat, var, r.c_str());
                else printf("B %d %d %016llx\n", sat, var, Fnv(r));
            }
        SetupBeacons(1);
        if (mode == "beacon") return 0;
    }
    g_tab = "Report";
    if (mode == "dump")
    {
        int sat = atoi(argv[2]), kind = atoi(argv[3]), cc = atoi(argv[4]), n = atoi(argv[5]);
        SideSetSat(sat);
        SetupReport(kind, cc, n);
        printf("%s", Render().c_str());
        return 0;
    }
    const std::string sats = argc > 2 ? argv[2] : "123";
    const int cc_max = argc > 3 ? atoi(argv[3]) : 63;
    const int n_max = argc > 4 ? atoi(argv[4]) : 64;
    const int kind_first = argc > 5 ? atoi(argv[5]) : 0;
    const int kind_last = argc > 6 ? atoi(argv[6]) : (int)REPORT_KIND_SC_GENERIC;
    const int cc_first = argc > 7 ? atoi(argv[7]) : 0;
    for (char sc : sats)
    {
        const int sat = sc - '0';
        SideSetSat(sat);
        for (int kind = kind_first; kind <= kind_last; kind++)
            for (int cc = cc_first; cc <= cc_max && !SkipKind(kind); cc++)
            {
                unsigned long long hh = 0;
                for (int n = 0; n <= n_max; n++)
                {
                    SetupReport(kind, cc, n);
                    hh = hh * 1000003ULL ^ Fnv(Render());
                }
                printf("R %d %d %d %016llx\n", sat, kind, cc, hh);
            }
    }
    return 0;
}
