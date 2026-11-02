#include "cosmic_procedures.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "miman_config.h"
#include "miman_coms.h"
#include "svc_tx.h"

namespace mim {
namespace model {
namespace cosmic {

void RunAddPacketProcedure(uint16_t msgid, uint8_t fnccode)
{

  
        mim::svc::WaitTx();


        uint8_t cmd[15] = {0};


        uint16_t mid_be = htons(msgid);
        memcpy(cmd, &mid_be, sizeof(uint16_t));   

        cmd[2] = 0xC0;
        cmd[3] = 0x00;


        uint16_t ccsds_len = htons(sizeof(cmd) - 7); 
        memcpy(cmd + 4, &ccsds_len, sizeof(uint16_t)); 

        // CC
        cmd[6] = fnccode;   // 0


        cmd[7] = 0x18;     // CRC


        cmd[8] = 0x1a;
        cmd[9] = 0x08;
        cmd[10] = 0x00;
        cmd[11] = 0x00;
        cmd[12] = 0x00;
        cmd[13] = 0x00;
        cmd[14] = 0x04;



        packetsign* TestPacket =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd));
        TestPacket->Identifier = HVD_TEST;
        TestPacket->PacketType = MIM_PT_TMTC_TEST;
        TestPacket->Length     = sizeof(cmd);   
        memcpy(TestPacket->Data, cmd, sizeof(cmd));

        mim::svc::SendUplink((void*)TestPacket);

        mim::svc::WaitTx();




        sleep(1);

        uint8_t cmd2[12] = {0};


        cmd2[0] = 0x18;
        cmd2[1] = 0x19;

        cmd2[2] = 0xC0;
        cmd2[3] = 0x00;
        cmd2[4] = 0x00;
        cmd2[5] = 0x05;

        // CC
        cmd2[6] = 0x00;   // 0


        cmd2[7] = 0x29;     // CRC


        cmd2[8] = 0x1a;
        cmd2[9] = 0x08;
        cmd2[10] = 0x00;
        cmd2[11] = 0x00;




        packetsign* TestPacket1 =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd2));
        TestPacket1->Identifier = HVD_TEST;
        TestPacket1->PacketType = MIM_PT_TMTC_TEST;
        TestPacket1->Length     = sizeof(cmd2);   
        memcpy(TestPacket1->Data, cmd2, sizeof(cmd2));

        mim::svc::SendUplink((void*)TestPacket1);
        mim::svc::WaitTx();


            }

void RunDualEmissionProcedure(uint16_t msgid, uint8_t fnccode)
{

  


        uint8_t cmd0[8] = {0};

            //mid
        cmd0[0] = 0x18;
        cmd0[1] = 0x23;
            //seq
        cmd0[2] = 0xC0;
        cmd0[3] = 0x00;
            //length
        cmd0[4] = 0x00;
        cmd0[5] = 0x01;

        // CC
        cmd0[6] = 27;   // 0

        uint8_t checksum = 0xFF;
        for (size_t i = 0; i < sizeof(cmd0); ++i)
            checksum ^= cmd0[i];
        cmd0[7] = checksum;






        packetsign* TestPacket0 =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd0));
        TestPacket0->Identifier = HVD_TEST;
        TestPacket0->PacketType = MIM_PT_TMTC_TEST;
        TestPacket0->Length     = sizeof(cmd0);   
        memcpy(TestPacket0->Data, cmd0, sizeof(cmd0));

        mim::svc::SendUplink((void*)TestPacket0);
        mim::svc::WaitTx();


            sleep(0.1);

        mim::svc::WaitTx();


        uint8_t cmd[15] = {0};


        uint16_t mid_be = htons(msgid);
        memcpy(cmd, &mid_be, sizeof(uint16_t));   

        cmd[2] = 0xC0;
        cmd[3] = 0x00;


        uint16_t ccsds_len = htons(sizeof(cmd) - 7); 
        memcpy(cmd + 4, &ccsds_len, sizeof(uint16_t)); 

        // CC
        cmd[6] = fnccode;   // 0


        cmd[7] = 0x18;     // CRC


        cmd[8] = 0x1a;
        cmd[9] = 0x08;
        cmd[10] = 0x00;
        cmd[11] = 0x00;
        cmd[12] = 0x00;
        cmd[13] = 0x00;
        cmd[14] = 0x04;



        packetsign* TestPacket =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd));
        TestPacket->Identifier = HVD_TEST;
        TestPacket->PacketType = MIM_PT_TMTC_TEST;
        TestPacket->Length     = sizeof(cmd);   
        memcpy(TestPacket->Data, cmd, sizeof(cmd));

        mim::svc::SendUplink((void*)TestPacket);

        mim::svc::WaitTx();




        sleep(0.1);

        uint8_t cmd2[12] = {0};


        cmd2[0] = 0x18;
        cmd2[1] = 0x19;

        cmd2[2] = 0xC0;
        cmd2[3] = 0x00;
        cmd2[4] = 0x00;
        cmd2[5] = 0x05;

        // CC
        cmd2[6] = 0x00;   // 0


        cmd2[7] = 0x29;     // CRC


        cmd2[8] = 0x1a;
        cmd2[9] = 0x08;
        cmd2[10] = 0x00;
        cmd2[11] = 0x00;




        packetsign* TestPacket1 =
            (packetsign*)malloc(2 + 2 + 4 + sizeof(cmd2));
        TestPacket1->Identifier = HVD_TEST;
        TestPacket1->PacketType = MIM_PT_TMTC_TEST;
        TestPacket1->Length     = sizeof(cmd2);   
        memcpy(TestPacket1->Data, cmd2, sizeof(cmd2));

        mim::svc::SendUplink((void*)TestPacket1);
        mim::svc::WaitTx();


            }

} // namespace cosmic
} // namespace model
} // namespace mim
