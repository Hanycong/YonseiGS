#include "pass_script.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"

extern Console console;

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

int SendTcStep(const TcStep & step, bool (*keep_going)())
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

        console.AddLog("[Auto]%s sent (%d/%d)", step.name.c_str(), i + 1, step.repeat);
        sleep(step.delay_s);
    }
    return sent;
}

} // namespace svc
} // namespace mim
