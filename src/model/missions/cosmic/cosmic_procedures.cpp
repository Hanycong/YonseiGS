#include "cosmic_procedures.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"
#include "telecommand_sender.h"

namespace mim {
namespace model {
namespace cosmic {

// 두 절차가 같이 쓰는 패킷 2개 (TO_LAB 패킷 추가 15바이트 -> gap_seconds 대기 -> 12바이트).
// 바이트는 운용 때 쓰던 값 그대로다: 체크섬 자리([7])도 계산하지 않고 상수(0x18, 0x29)를 쓴다.
static void SendAddPacketPair(uint16_t msgid, uint8_t fnccode, unsigned gap_seconds)
{
    mim::svc::WaitTx();

    uint8_t cmd[15] = {0};
    uint16_t mid_be = htons(msgid);
    memcpy(cmd, &mid_be, sizeof(uint16_t));
    cmd[2] = 0xC0;
    cmd[3] = 0x00;
    uint16_t ccsds_len = htons(sizeof(cmd) - 7);
    memcpy(cmd + 4, &ccsds_len, sizeof(uint16_t));
    cmd[6] = fnccode;
    cmd[7] = 0x18;     // CRC (상수)
    cmd[8] = 0x1a;
    cmd[9] = 0x08;
    cmd[10] = 0x00;
    cmd[11] = 0x00;
    cmd[12] = 0x00;
    cmd[13] = 0x00;
    cmd[14] = 0x04;

    mim::tc::QueueTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
    mim::svc::WaitTx();

    sleep(gap_seconds);

    uint8_t cmd2[12] = {0};
    cmd2[0] = 0x18;
    cmd2[1] = 0x19;
    cmd2[2] = 0xC0;
    cmd2[3] = 0x00;
    cmd2[4] = 0x00;
    cmd2[5] = 0x05;
    cmd2[6] = 0x00;
    cmd2[7] = 0x29;     // CRC (상수)
    cmd2[8] = 0x1a;
    cmd2[9] = 0x08;
    cmd2[10] = 0x00;
    cmd2[11] = 0x00;

    mim::tc::QueueTelecommand(cmd2, sizeof(cmd2), MIM_PT_TMTC_TEST);
    mim::svc::WaitTx();
}

void RunAddPacketProcedure(uint16_t msgid, uint8_t fnccode)
{
    SendAddPacketPair(msgid, fnccode, 1);
}

void RunDualEmissionProcedure(uint16_t msgid, uint8_t fnccode)
{
    uint8_t cmd0[8] = {0};
    cmd0[0] = 0x18;     // mid
    cmd0[1] = 0x23;
    cmd0[2] = 0xC0;     // seq
    cmd0[3] = 0x00;
    cmd0[4] = 0x00;     // length
    cmd0[5] = 0x01;
    cmd0[6] = 27;       // CC

    uint8_t checksum = 0xFF;
    for (size_t i = 0; i < sizeof(cmd0); ++i)
        checksum ^= cmd0[i];
    cmd0[7] = checksum;

    mim::tc::QueueTelecommand(cmd0, sizeof(cmd0), MIM_PT_TMTC_TEST);
    mim::svc::WaitTx();

    // 원래 코드는 여기와 두 패킷 사이에서 sleep(0.1) 을 불렀다. sleep 은 정수 초를 받아 0.1 이 0 으로
    // 잘리므로 실제로는 쉬지 않는다. 동작을 그대로 두려고 0 으로 적는다 (CLAUDE.md Known defects).
    sleep(0);
    SendAddPacketPair(msgid, fnccode, 0);
}

} // namespace cosmic
} // namespace model
} // namespace mim
