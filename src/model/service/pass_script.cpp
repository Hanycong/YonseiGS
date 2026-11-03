#include "pass_script.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"

extern Console console;
extern int PingCounter;
extern int BeaconCounter;

namespace mim {
namespace svc {

std::vector<TcStep> DefaultPassScript()
{
    const TcStep eps_hk[3] = {
        {"EPS P60 DOCK GET TABLE HK", {0x18,0x75,0xC0,0x00,0x00,0x01,0x14,0x47}},
        {"EPS P60 PDU GET TABLE HK",  {0x18,0x75,0xC0,0x00,0x00,0x01,0x17,0x44}},
        {"EPS P60 ACU GET TABLE HK",  {0x18,0x75,0xC0,0x00,0x00,0x01,0x1a,0x00}},
    };
    std::vector<TcStep> s(eps_hk, eps_hk + 3);
    s.push_back({"AX100 GND WDT",        {0x18,0x50,0xC0,0x00,0x00,0x01,0x06,0x70}});
    s.push_back({"EPS P60 DOCK GND WDT", {0x18,0x75,0xC0,0x00,0x00,0x05,0x1e,0xa2,0xe8,0x03,0x00,0x00}});
    s.insert(s.end(), eps_hk, eps_hk + 3);
    return s;
}

int SendTcStep(const TcStep & step, bool (*keep_going)(), std::atomic<int> * sent_total)
{
    int sent = 0;
    for (int i = 0; i < step.repeat && keep_going(); i++)
    {
        packetsign * pkt = (packetsign *)malloc(2 + 2 + 4 + step.bytes.size());
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = step.bytes.size();
        memcpy(pkt->Data, step.bytes.data(), step.bytes.size());

        SendUplink((void *)pkt);
        WaitTx();
        free(pkt);
        sent++;
        if (sent_total)
            (*sent_total)++;

        console.AddLog("[Auto]%s sent (%d/%d)", step.name.c_str(), i + 1, step.repeat);

        // Stop 을 누르면 대기 중에도 바로 멈춘다
        for (int t = 0; t < step.delay_s * 10 && keep_going(); t++)
            usleep(100000);
    }
    return sent;
}

bool ParseHexBytes(const std::string & text, std::vector<uint8_t> & out, std::string & err)
{
    std::vector<uint8_t> bytes;
    std::string norm = text;
    for (char & c : norm)
        if (c == ',')
            c = ' ';
    std::istringstream in(norm);
    std::string tok;
    while (in >> tok)
    {
        std::string h = tok;
        if (h.size() > 2 && h[0] == '0' && (h[1] == 'x' || h[1] == 'X'))
            h = h.substr(2);
        if (h.empty() || h.size() > 2 || !isxdigit((unsigned char)h[0]) ||
            (h.size() == 2 && !isxdigit((unsigned char)h[1])))
        {
            err = "not a hex byte: \"" + tok + "\"";
            return false;
        }
        bytes.push_back((uint8_t)strtoul(h.c_str(), NULL, 16));
    }
    if (bytes.empty())
    {
        err = "no bytes";
        return false;
    }
    if (bytes.size() > TC_MAX_BYTES)
    {
        err = "too many bytes (max " + std::to_string(TC_MAX_BYTES) + ")";
        return false;
    }
    out.swap(bytes);
    return true;
}

std::string FormatHexBytes(const std::vector<uint8_t> & bytes)
{
    std::string s;
    char b[4];
    for (size_t i = 0; i < bytes.size(); i++)
    {
        snprintf(b, sizeof(b), i ? " %02X" : "%02X", bytes[i]);
        s += b;
    }
    return s;
}

// ---------------------------------------------------------------- PassScript

const char * PassScript::DefaultPath() { return "../data/autopilot/pass_script.txt"; }

PassScript::PassScript() : steps_(DefaultPassScript()), path_(DefaultPath()) {}

int PassScript::Count() const
{
    std::lock_guard<std::mutex> lk(mu_);
    return (int)steps_.size();
}

TcStep PassScript::At(int i) const
{
    std::lock_guard<std::mutex> lk(mu_);
    return (i >= 0 && i < (int)steps_.size()) ? steps_[i] : TcStep();
}

std::vector<TcStep> PassScript::Snapshot() const
{
    std::lock_guard<std::mutex> lk(mu_);
    return steps_;
}

void PassScript::Set(int i, const TcStep & step)
{
    std::lock_guard<std::mutex> lk(mu_);
    if (i >= 0 && i < (int)steps_.size())
        steps_[i] = step;
}

void PassScript::Insert(int i, const TcStep & step)
{
    std::lock_guard<std::mutex> lk(mu_);
    if (i < 0 || i > (int)steps_.size())
        i = (int)steps_.size();
    steps_.insert(steps_.begin() + i, step);
}

void PassScript::Remove(int i)
{
    std::lock_guard<std::mutex> lk(mu_);
    if (i >= 0 && i < (int)steps_.size())
        steps_.erase(steps_.begin() + i);
}

void PassScript::Swap(int i, int j)
{
    std::lock_guard<std::mutex> lk(mu_);
    int n = (int)steps_.size();
    if (i >= 0 && i < n && j >= 0 && j < n)
        std::swap(steps_[i], steps_[j]);
}

void PassScript::ResetToDefault()
{
    std::lock_guard<std::mutex> lk(mu_);
    steps_ = DefaultPassScript();
}

std::string PassScript::Path() const
{
    std::lock_guard<std::mutex> lk(mu_);
    return path_;
}

static std::string Trim(const std::string & s)
{
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

// 0 ~ max 사이 정수만 받는다
static bool ParseInt(const std::string & s, int min, int max, int & out)
{
    if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos || s.size() > 6)
        return false;
    int v = atoi(s.c_str());
    if (v < min || v > max)
        return false;
    out = v;
    return true;
}

bool PassScript::Load(const std::string & path, std::string & err)
{
    std::ifstream in(path);
    if (!in)
    {
        err = "cannot open " + path;
        return false;
    }
    std::vector<TcStep> steps;
    std::string line;
    int no = 0;
    while (std::getline(in, line))
    {
        no++;
        std::string t = Trim(line);
        if (t.empty() || t[0] == '#')
            continue;

        std::vector<std::string> f;
        std::istringstream ls(t);
        std::string part;
        while (std::getline(ls, part, '|'))
            f.push_back(Trim(part));
        std::string where = "line " + std::to_string(no) + ": ";
        if (f.size() != 4)
        {
            err = where + "expected \"name | repeat | delay | bytes\"";
            return false;
        }
        TcStep st;
        st.name = f[0];
        if (st.name.empty())
        {
            err = where + "empty name";
            return false;
        }
        if (!ParseInt(f[1], 1, TC_MAX_REPEAT, st.repeat))
        {
            err = where + "repeat must be 1-" + std::to_string(TC_MAX_REPEAT);
            return false;
        }
        if (!ParseInt(f[2], 0, TC_MAX_DELAY, st.delay_s))
        {
            err = where + "delay must be 0-" + std::to_string(TC_MAX_DELAY) + " s";
            return false;
        }
        std::string e;
        if (!ParseHexBytes(f[3], st.bytes, e))
        {
            err = where + e;
            return false;
        }
        steps.push_back(st);
    }
    if (steps.empty())
    {
        err = "no steps in " + path;
        return false;
    }
    std::lock_guard<std::mutex> lk(mu_);
    steps_.swap(steps);
    path_ = path;
    return true;
}

bool PassScript::Save(const std::string & path, std::string & err)
{
    // 기본 경로의 폴더(data/autopilot)가 없으면 만든다
    size_t slash = path.find_last_of('/');
    if (slash != std::string::npos && slash > 0)
        mkdir(path.substr(0, slash).c_str(), 0755);

    std::vector<TcStep> steps = Snapshot();
    std::ofstream out(path);
    if (!out)
    {
        err = "cannot write " + path;
        return false;
    }
    out << "# BEE-1000 pass TC script (AutoPilot)\n";
    out << "# name | repeat | delay [s] | bytes (hex)\n";
    for (const TcStep & s : steps)
    {
        std::string name = s.name;
        for (char & c : name)
            if (c == '|' || c == '\n' || c == '\r')
                c = ' ';
        out << name << " | " << s.repeat << " | " << s.delay_s << " | " << FormatHexBytes(s.bytes) << "\n";
    }
    out.close();
    if (!out)
    {
        err = "write failed: " + path;
        return false;
    }
    std::lock_guard<std::mutex> lk(mu_);
    path_ = path;
    return true;
}

PassScript & CurrentPassScript()
{
    static PassScript * script = [] {
        PassScript * p = new PassScript();
        std::string err;
        if (access(PassScript::DefaultPath(), F_OK) == 0 && !p->Load(PassScript::DefaultPath(), err))
            console.AddLog("[ERROR]##[Auto]Pass TC script not loaded (%s). Using the built-in script.", err.c_str());
        return p;
    }();
    return *script;
}

// ---------------------------------------------------------------- 송신 조건

PassGate EvaluatePassGate(int sat)
{
    PassGate g;
    if (OrbitData.NowTracking && sat >= 0 && sat < SAT_MAX_NUM && OrbitData.Satellites[sat] != NULL)
    {
        SatelliteObject * s = OrbitData.Satellites[sat];
        try
        {
            // 추적 스레드가 쓰는 obs 를 건드리지 않도록 같은 위치로 새 Observer 를 만든다
            Observer obs(s->obs.GetLocation());
            CoordTopocentric topo = obs.GetLookAngle(s->sgp4.FindPosition(DateTime::Now(true)));
            g.elevation_deg = topo.elevation * RAD_TO_DEG;
            g.has_elevation = true;
        }
        catch (const std::exception &)
        {
        }
    }
    g.elevation_ok = g.has_elevation && g.elevation_deg >= AutoPilotCfg.MinElevation;
    g.rotator_ok   = RotatorFlags.RotatorConnection;
    g.engage_ok    = RotatorFlags.Engage;
    g.radio_ok     = LinkFlags.GS100_Connection;
    g.link_ok      = !AutoPilotCfg.RequireLink || PingCounter > 0 || BeaconCounter > 0;
    return g;
}

} // namespace svc
} // namespace mim
