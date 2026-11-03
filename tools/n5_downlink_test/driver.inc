// 누리호 5차 수신(비콘/리포트) 비교 테스트 - 공통 드라이버
//
// ours_main.cpp (이 저장소) 와 n5_main.cpp (GS_nuri5 miman_coms.cpp 를 그대로 include) 가 이 파일을 include 한다.
// 같은 합성 패킷을 양쪽 함수에 넣고, 화면용 값(g_report_view 등)과 저장 파일 내용을 결정적인 텍스트로 출력한다.
// 두 출력의 "#### END COMMON" 앞부분은 같아야 한다 (파일 이름의 시각은 TS 로 바꿔 비교).
//
// include 하는 쪽이 정의할 것:
//   T_ASSEMBLE(tag, port, pkt, dir, prefix, saver)   15/10 리포트 조립기
//   T_SD1SAT_ASSEMBLE(port, pkt)                     SD1SAT 조각 조립기
//   T_VIEW(rpt, port), T_SD1SAT_VIEW(rpt)            화면용 갱신
//   T_DUMP(fp, rpt, port)                            DumpReportPayloadParsed_ByMidCc
//   Process{BEE1012,UELYSYS,SD1SAT}BeaconPort, BEE1012ReportSaver, UELYSYSReportSaver
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <regex>
#include <string>
#include <vector>

// 드라이버 출력은 g_out 으로 (테스트 대상의 printf 는 main 이 /dev/null 로 보낸다)
static FILE * g_out = stdout;
#define printf(...) fprintf(g_out, __VA_ARGS__)

static uint32_t g_rng = 12345;
static uint8_t Rnd() { g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return (uint8_t)g_rng; }

static uint64_t Fnv(const void * p, size_t n, uint64_t h = 1469598103934665603ULL)
{
    const uint8_t * b = (const uint8_t *)p;
    for (size_t i = 0; i < n; i++) { h ^= b[i]; h *= 1099511628211ULL; }
    return h;
}

static csp_packet_t * MakePkt(const uint8_t * d, size_t n)
{
    csp_packet_t * p = (csp_packet_t *)calloc(1, sizeof(csp_packet_t) + 4096);
    memcpy(p->data, d, n);
    p->length = (uint16_t)n;
    return p;
}

// ---- ../data 아래 파일 목록 + 내용 해시 (이름의 숫자 시각은 TS 로) ----
static void Walk(const std::string & dir, std::vector<std::string> & out)
{
    DIR * d = opendir(dir.c_str());
    if (!d) return;
    while (dirent * e = readdir(d)) {
        std::string n = e->d_name;
        if (n == "." || n == "..") continue;
        std::string p = dir + "/" + n;
        struct stat st;
        if (stat(p.c_str(), &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) { Walk(p, out); continue; }
        FILE * f = fopen(p.c_str(), "rb");
        std::string c; char buf[4096]; size_t k;
        while (f && (k = fread(buf, 1, sizeof(buf), f)) > 0) c.append(buf, k);
        if (f) fclose(f);
        static const std::regex ts("[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}(-[0-9]{3})?(_[0-9]+)?");
        char line[64];
        snprintf(line, sizeof(line), " size=%zu hash=%016llx", c.size(), (unsigned long long)Fnv(c.data(), c.size()));
        out.push_back(std::regex_replace(p, ts, "TS") + line);
    }
    closedir(d);
}

static void DumpTree(const char * title)
{
    std::vector<std::string> v;
    Walk("../data", v);
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());   // 같은 이름+내용은 한 줄 (덮어쓰기 여부와 무관하게 비교)
    printf("[files %s]\n", title);
    for (auto & s : v) printf("  %s\n", s.c_str());
    system("rm -rf ../data");
}

static void PrintView(const char * tag)
{
    printf("  view %s kind=%d valid=%d pv=%d mid=0x%04x cc=%u size=%u crit=%d hash=%016llx\n", tag,
           (int)g_report_view.kind, (int)g_report_view.valid, (int)g_report_view.payload_valid,
           g_report_view.reflected_msg_id, g_report_view.reflected_cc, g_report_view.ret_val_size,
           (int)g_report_view.is_critical,
           (unsigned long long)Fnv(&g_report_view, sizeof(g_report_view)));
}

static uint64_t DumpHash(const Report * r, int port)
{
    char * buf = NULL; size_t n = 0;
    FILE * fp = open_memstream(&buf, &n);
    T_DUMP(fp, r, port);
    fclose(fp);
    uint64_t h = Fnv(buf, n);
    free(buf);
    return h;
}

static int g_hook_port = 0;
static int HookSaver(Report * r)   // 여러 리포트 묶음: 리포트마다 화면값 + 해석 덤프 해시
{
    PrintView("hook");
    printf("  dump=%016llx\n", (unsigned long long)DumpHash(r, g_hook_port));
    return 0;
}

static const uint16_t MIDS[] = {
    UANT_APP_CMD_ID, ADCS_CMD_ID, 0x6518, EPS_CMD_ID, 0x7518, UELYSYS_TTC_CMD_ID, 0x8018, BEE1012_MEOW_CMD_ID, 0x8218,
    UELYSYS_MEOW_CMD_ID, 0x8618, UELYSYS_STX_CMD_ID, 0x5418, UELYSYS_UTRX_CMD_ID, 0x5018, UELYSYS_UANT_CMD_ID, 0x4018,
    UELYSYS_GPIO_CMD_ID, 0x8A18, UELYSYS_PAYUEL_OBC_CMD_ID, 0xC018, UELYSYS_PAYUEL_CAM_CMD_ID, 0xC318, UELYSYS_LGBAT_CMD_ID,
    0xC618, UELYSYS_PAYUEL_AOS_CMD_ID, 0x4718, UELYSYS_PAYUEL_YONSEI_CMD_ID, 0xCC18, UELYSYS_CI_LAB_CMD_ID, 0x1018,
    UELYSYS_EO_CMD_ID, 0x9018, UELYSYS_GPS_CMD_ID, 0x9418, UELYSYS_SCH_LAB_CMD_ID, 0xA318, UELYSYS_SP_CMD_ID, 0x5818,
    UELYSYS_TO_LAB_CMD_ID, 0x2318, GPIO_CMD_ID, 0x9218, PAYUEL_ROMA_CMD_MID, 0x3018, PAYUEL_LGPM_CMD_ID, 0x3518, LTRX_CMD_ID,
    PAYBEE_KISSCAM_CMD_ID, BEE1012_PAY_SLT_CMD_ID, BEE1012_THRUST_CMD_ID, UELYSYS_PAYYONSEI_TEMP_CMD_ID,
    UELYSYS_PAYYONSEI_IMU_CMD_ID, UELYSYS_PAYYONSEI_IWSB_CMD_ID, UELYSYS_PAYYONSEI_I2C_CMD_ID, UELYSYS_SDR_CONTROLLER_CMD_ID,
    0x1840, 0x1823,
};
static const uint16_t SD1_MIDS[] = {
    UELYSYS_TTC_CMD_ID, UELYSYS_MEOW_CMD_ID, SD1SAT_CI_CMD_ID, SD1SAT_CI_GATE_CMD_ID, SD1SAT_CR_CMD_ID, SD1SAT_DS_CMD_ID,
    SD1SAT_FM_CMD_ID, SD1SAT_GPS_CMD_ID, SD1SAT_HK_CMD_ID, SD1SAT_MEOW_CMD_ID, SD1SAT_OPS_CMD_ID, SD1SAT_PAYSD1_LOTHAN_CMD_ID,
    SD1SAT_PAYSD1_ROMA_CMD_ID, SD1SAT_SCH_LAB_CMD_ID, SD1SAT_SNBP_CMD_ID, SD1SAT_TO_CMD_ID, SD1SAT_TTC_CMD_ID,
    SD1SAT_UANT_CMD_ID, SD1SAT_UTRX_CMD_ID, SD1_EPS_CMD_ID,
};

// 540 바이트 단일 리포트 (0x0825) 와이어: 16B 헤더 + 524B 항목
static std::vector<uint8_t> ReportWire(uint16_t outer, uint16_t mid, uint8_t cc, uint16_t size, int n_entries, size_t entry)
{
    std::vector<uint8_t> w(16 + entry * n_entries);
    for (auto & b : w) b = Rnd();
    w[0] = outer >> 8; w[1] = outer & 0xff;
    const uint16_t lenf = (uint16_t)(w.size() - 7);
    w[4] = lenf >> 8; w[5] = lenf & 0xff;
    for (int i = 0; i < n_entries; i++) {
        uint8_t * e = w.data() + 16 + i * entry;
        memcpy(e, &mid, 2); e[2] = cc;
        uint16_t s = (uint16_t)(size + i); memcpy(e + 8, &s, 2);
    }
    return w;
}

static void Feed(const char * tag, int port, const std::vector<uint8_t> & w, size_t chunk,
                 const char * dir, const char * prefix, int (*saver)(Report *))
{
    for (size_t off = 0; off < w.size(); off += chunk) {
        csp_packet_t * p = MakePkt(w.data() + off, std::min(chunk, w.size() - off));
        T_ASSEMBLE(tag, port, p, dir, prefix, saver);
        free(p);
    }
}

static void RunCommon()
{
    system("rm -rf ../data");
    g_roma_parget_requested_index = 3;

    // ---- 1. 비콘 ----
    struct { const char * name; int port; size_t len; void (*fn)(const csp_packet_t *, int, const struct timeval *); } bcn[] = {
        { "BEE1012", 17, sizeof(BEE1012_Beacon_t), ProcessBEE1012BeaconPort },
        { "UELYSYS", 12, sizeof(UELYSYS_Beacon_t), ProcessUELYSYSBeaconPort },
        { "SD1SAT",  22, sizeof(SD1SAT_Beacon_t),  ProcessSD1SATBeaconPort },
    };
    for (auto & b : bcn) {
        for (int k = 0; k < 4; k++) {
            uint8_t d[400]; for (auto & x : d) x = Rnd();
            const size_t len = (k == 3) ? b.len + 5 : b.len;    // 마지막은 길이가 다른 패킷 (미션비콘/깨진 비콘)
            csp_packet_t * p = MakePkt(d, len);
            b.fn(p, b.port, NULL);
            free(p);
            printf("beacon %s #%d len=%zu\n", b.name, k, len);
            printf("  globals b12=%016llx uel=%016llx sd1=%016llx\n",
                   (unsigned long long)Fnv(bee1012_beacon, sizeof(BEE1012_Beacon_t)),
                   (unsigned long long)Fnv(uelysys_beacon, sizeof(UELYSYS_Beacon_t)),
                   (unsigned long long)Fnv(sd1sat_beacon, sizeof(SD1SAT_Beacon_t)));
            DumpTree(b.name);
        }
    }

    // ---- 2. 리포트 해석 전수 (MID x CC 0..63 x 길이 0..512): 화면값 + 해석 덤프 ----
    for (uint16_t mid : MIDS) {
        const int ports[2] = { 10, 15 };
        for (int port : ports) {
            if (port == 15 && mid != BEE1012_PAY_SLT_CMD_ID) continue;
            for (int cc = 0; cc < 64; cc++) {
                uint64_t hv = 0, hd = 0;
                for (int len = 0; len <= 512; len++) {
                    Report r; memset(&r, 0, sizeof(r));
                    r.CCSDS_MsgId = 0x0825; r.ReflectedMID = mid; r.ReflectedCC = (uint8_t)cc;
                    r.RetCode = (int32_t)len - 3; r.RetValSize = (uint16_t)len;
                    for (int i = 0; i < len; i++) r.RetVal[i] = Rnd();
                    T_VIEW(r, port);
                    hv = Fnv(&g_report_view, sizeof(g_report_view), hv ? hv : 1469598103934665603ULL);
                    hv = Fnv(&g_eps_report_state, sizeof(g_eps_report_state), hv);
                    hv = Fnv(&g_eps_rparam_reassembly, sizeof(g_eps_rparam_reassembly), hv);
                    hd ^= DumpHash(&r, port) + (uint64_t)len * 0x9E3779B97F4A7C15ULL;
                }
                printf("decode port=%d mid=0x%04x cc=%d view=%016llx dump=%016llx\n", port, mid, cc,
                       (unsigned long long)hv, (unsigned long long)hd);
            }
        }
    }

    // ---- 3. SD1SAT 화면값 전수 (봉투 해석 후의 SD1SATReport) ----
    for (uint16_t mid : SD1_MIDS) {
        for (int cc = 0; cc < 64; cc++) {
            uint64_t hv = 1469598103934665603ULL;
            for (int len = 0; len <= 512; len += 1) {
                SD1SATReport r; memset(&r, 0, sizeof(r));
                r.CmdMsgId = mid; r.CmdCode = (uint8_t)cc; r.RetVal = len; r.DataLength = (uint16_t)len; r.CrSeq = (uint16_t)len;
                for (int i = 0; i < len; i++) r.Data[i] = Rnd();
                T_SD1SAT_VIEW(r);
                hv = Fnv(&g_report_view, sizeof(g_report_view), hv);
            }
            printf("sd1sat-view mid=0x%04x cc=%d view=%016llx\n", mid, cc, (unsigned long long)hv);
        }
    }

    // ---- 4. 조립기 (포트 15 / 10) ----
    struct { uint16_t mid; uint8_t cc; uint16_t size; } one[] = {
        { BEE1012_PAY_SLT_CMD_ID, 0, 32 }, { PAYUEL_ROMA_CMD_MID, 0, 2 }, { EPS_CMD_ID, 0, 16 }, { UELYSYS_TTC_CMD_ID, 1, 20 },
        { ADCS_CMD_ID, 3, 40 }, { PAYUEL_LGPM_CMD_ID, 2, 8 }, { UELYSYS_STX_CMD_ID, 5, 9 },
    };
    for (auto & o : one) {
        for (int port = 10; port <= 15; port += 5) {
            std::vector<uint8_t> w = ReportWire(0x0825, o.mid, o.cc, o.size, 1, 524);
            Feed(port == 15 ? "case15" : "case10", port, w, 200,
                 port == 15 ? "../data/BEE1012/report/report_raw" : "../data/UELYSYS/report/report_raw",
                 port == 15 ? "rpt_raw--" : "report_raw_", port == 15 ? BEE1012ReportSaver : UELYSYSReportSaver);
            printf("assemble single port=%d mid=0x%04x cc=%u\n", port, o.mid, o.cc);
            PrintView("last");
            DumpTree("single");
        }
    }
    for (int port = 10; port <= 15; port += 5) {
        g_hook_port = port;
        printf("assemble multi 0x0825 x3 port=%d\n", port);
        Feed("multi", port, ReportWire(0x0825, PAYUEL_ROMA_CMD_MID, 14, 6, 3, 524), 190, "../data/T/report", "raw--", HookSaver);
        DumpTree("multi");   // 원본 저장 이름이 같은 밀리초면 GS_nuri5 는 덮어쓰므로 조립마다 확인
        printf("assemble critical 0x0826 x2 port=%d\n", port);
        Feed("crit", port, ReportWire(0x0826, UELYSYS_TTC_CMD_ID, 1, 12, 2, 532), 250, "../data/T/report", "raw--", HookSaver);
        DumpTree("crit");
        printf("assemble ops 0x0827 port=%d\n", port);
        Feed("ops", port, ReportWire(0x0827, 0, 0, 0, 1, 28), 44, "../data/T/report", "raw--", HookSaver);
        printf("  ops valid=%d hash=%016llx\n", (int)g_rpt_ops_data_valid,
               (unsigned long long)Fnv(&g_rpt_ops_data, sizeof(g_rpt_ops_data)));
        DumpTree("ops");
        printf("assemble bad length 16+500 port=%d\n", port);
        Feed("bad", port, ReportWire(0x0825, PAYUEL_ROMA_CMD_MID, 0, 2, 1, 500), 516, "../data/T/report", "raw--", HookSaver);
        DumpTree("bad");
    }

    // ---- 5. SD1SAT 조각 (포트 20) ----
    for (int t = 0; t < 4; t++) {
        const uint16_t dlen = (uint16_t)(t * 90 + 5);
        std::vector<uint8_t> w(16 + dlen);
        for (auto & b : w) b = Rnd();
        uint16_t mid = SD1_MIDS[t * 3]; memcpy(&w[4], &mid, 2); w[12] = (uint8_t)t;
        memcpy(&w[14], &dlen, 2);
        const size_t per = 150;
        const uint8_t count = (uint8_t)((w.size() + per - 1) / per);
        for (uint8_t i = 0; i < count; i++) {
            std::vector<uint8_t> f = { 0x12, (uint8_t)(0x30 + t), i, count };
            const size_t off = i * per;
            f.insert(f.end(), w.begin() + off, w.begin() + std::min(w.size(), off + per));
            csp_packet_t * p = MakePkt(f.data(), f.size());
            T_SD1SAT_ASSEMBLE(20, p);
            free(p);
        }
        printf("sd1sat fragments t=%d count=%u\n", t, count);
        PrintView("sd1sat");
        DumpTree("sd1sat");
    }
    printf("#### END COMMON\n");
}
