// View - 명령 생성기 입력 폼: 5차 위성 공용 (BEE1012/UELYSYS/SD1-SAT) generic (명령 번호 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
//
// 누리호 5차 GS(GS_nuri5) DrawCmdGeneratorBody 의 case 를 내용 그대로 옮긴 것.
// 송신은 svc_tx(WaitTx/SendUplink), TC 패킷 기록은 업링크 스레드가 한다. 처리한 명령이면 true.
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <string.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "YonseiGS_config.h"
#include "mission_defs.h"
#include "YonseiGS_imgui.h"
#include "YonseiGS_coms.h"
#include "svc_tx.h"
#include "sat_mode.h"
#include "cmdgen5_common.h"
#include "cmdgen_forms.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawGeneric5Forms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 0:
        {
            static uint16_t msgid = 0;
            static uint8_t fnccode = 0;
            static uint16_t node = 0;
            ImGui::Text("For ADCS NoOp CMD, MsgID = 6245");
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("node", ImGuiDataType_U16, &node);

            if (ImGui::Button("Generate CMD"))
            {
                WriteSystemName(msgid);
                mim::svc::WaitTx();

                packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE + sizeof(node));
                TestPacket->Identifier = HVD_TEST;
                TestPacket->PacketType = MIM_PT_NODE_TC;
                TestPacket->Length = CFE_SB_CMD_HDR_SIZE;
                memset(TestPacket->Data, 0, (TestPacket->Length + sizeof(node)));

                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0,};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(uint16_t));
                memcpy(cmd + 6, &fnccode, sizeof(uint8_t));
                cmd[2] = 0xc0;
                cmd[3] = 0x00;
                cmd[4] = 0x00;
                cmd[5] = 0x01;
                cmd[7] = 0;

                uint16_t len = CFE_SB_CMD_HDR_SIZE;
                const uint8_t* byteptr = cmd;
                uint8_t checksum = 0xFF;
                while (len--) checksum ^= *(byteptr++);
                cmd[7] = checksum;
                memcpy(TestPacket->Data, &node, sizeof(node));
                memcpy(TestPacket->Data+2, cmd, sizeof(cmd));
                mim::svc::SendUplink((void*)TestPacket);
            }
            break;
        }
    case 1:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint16_t node = 0;
        static uint8_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg u8", ImGuiDataType_U8, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE + sizeof(uint8_t) + sizeof(node));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = CFE_SB_CMD_HDR_SIZE + sizeof(uint8_t);
            memset(TestPacket->Data, 0, TestPacket->Length + sizeof(node));

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint8_t)] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(uint8_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x02;

            cmd2[7] = 0;
            uint16_t len = 9;
            const uint8_t* byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd2[7] = checksum;

            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for (int i = 0; i < TestPacket->Length; i++)
            {
                if (i < 8)
                    printf("0x%x ", cmd2[i]);
                else
                    printf("%u", cmd2[i]);
            }
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 2:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint16_t node = 0;
        static uint16_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg u16", ImGuiDataType_U16, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);
            mim::svc::WaitTx();

            const uint16_t command_size = CFE_SB_CMD_HDR_SIZE + sizeof(uint16_t);
            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + command_size + sizeof(node));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = command_size;
            memset(TestPacket->Data, 0, TestPacket->Length + sizeof(node));

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint16_t)] = {0,};
            uint16_t mid = htons(msgid);
            memcpy(cmd2, &mid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(uint16_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x03;

            cmd2[7] = 0;
            uint16_t len = command_size;
            const uint8_t* byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd2[7] = checksum;
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for (int i = 0; i < TestPacket->Length; i++)
            {
                if (i < 8)
                    printf("0x%x ", cmd2[i]);
                else
                    printf("%u", arg);
            }
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
        }
        break;
    }
    case 3:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint16_t node = 0;
        static uint32_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg u32", ImGuiDataType_U32, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE + sizeof(uint32_t) + sizeof(node));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = CFE_SB_CMD_HDR_SIZE + sizeof(uint32_t);
            memset(TestPacket->Data, 0, TestPacket->Length + sizeof(node));

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint32_t)] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(uint32_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x05;

            cmd2[7] = 0;
            uint16_t len = 12;
            const uint8_t* byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--)
                checksum ^= *(byteptr++);
            cmd2[7] = checksum;

            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data + 2, cmd2, sizeof(cmd2));
            for (int i = 0; i < TestPacket->Length; i++)
                printf("0x%x ", cmd2[i]);
            printf("\n");

            mim::svc::SendUplink((void*)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 4:
    {
        static uint16_t msgid = 0;
        static uint8_t fnccode = 0;
        static uint16_t node = 0;
        static uint64_t arg = 0;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg u64", ImGuiDataType_U64, &arg);

        if (ImGui::Button("Generate CMD"))
        {
            WriteSystemName(msgid);
            mim::svc::WaitTx();

            packetsign* TestPacket = (packetsign*)malloc(2 + 2 + 4 + CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t) + sizeof(node));
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t);
            memset(TestPacket->Data, 0, TestPacket->Length + sizeof(node));

            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t)] = {0,};
            uint16_t mid = htons(msgid);
            memcpy(cmd2, &mid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(uint64_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x09;
            cmd2[7] = 0;

            uint16_t len = CFE_SB_CMD_HDR_SIZE + sizeof(uint64_t);
            const uint8_t* byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) checksum ^= *(byteptr++);
            cmd2[7] = checksum;
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            mim::svc::SendUplink((void*)TestPacket);
        }
        break;
    }
    case 5:  {// s8
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static uint16_t node = 0;
        static int8_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg s8",ImGuiDataType_S8, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();
            packetsign * TestPacket = (packetsign *)malloc(2+2+4+CFE_SB_CMD_HDR_SIZE+sizeof(int8_t) + sizeof(node)); // 8 is for data
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = CFE_SB_CMD_HDR_SIZE+sizeof(int8_t);
            memset(TestPacket->Data,0,TestPacket->Length + sizeof(node));
            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE+sizeof(int8_t)] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(int8_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x02;
            cmd2[7] = 0;
            uint16_t len = CFE_SB_CMD_HDR_SIZE+sizeof(int8_t);
            const uint8_t *byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) {
                checksum ^= *(byteptr++);
            }
            cmd2[7]=checksum; // checksum
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2 ,cmd2,sizeof(cmd2));
            for(int i=0; i<TestPacket->Length; i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::svc::SendUplink((void *)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 6:  {// s16
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static uint16_t node = 0;
        static int16_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg s16",ImGuiDataType_S16, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();
            packetsign * TestPacket = (packetsign *)malloc(2+2+4+CFE_SB_CMD_HDR_SIZE+sizeof(int16_t) + sizeof(node)); // 8 is for data
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = CFE_SB_CMD_HDR_SIZE+sizeof(int16_t);
            memset(TestPacket->Data,0,TestPacket->Length + sizeof(node));
            uint8_t cmd2[CFE_SB_CMD_HDR_SIZE+sizeof(int16_t)] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(int16_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x03;
            cmd2[7] = 0;
            uint16_t len = 10;
            const uint8_t *byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) {
                checksum ^= *(byteptr++);
            }
            cmd2[7]=checksum; // checksum
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for(int i=0; i<TestPacket->Length; i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::svc::SendUplink((void *)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 7:  {// s32
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static uint16_t node = 0;
        static int32_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg s32",ImGuiDataType_S32, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();
            packetsign * TestPacket = (packetsign *)malloc(2+2+4+12 + sizeof(node)); // 8 is for data
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = 12;
            memset(TestPacket->Data,0,TestPacket->Length + sizeof(node));
            uint8_t cmd2[12] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(int32_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x05;
            cmd2[7] = 0;
            uint16_t len = 12;
            const uint8_t *byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) {
                checksum ^= *(byteptr++);
            }
            cmd2[7]=checksum; // checksum
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for(int i=0; i<TestPacket->Length; i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::svc::SendUplink((void *)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 8:  {// s64
        static uint16_t msgid=0;
        static uint8_t fnccode=0;
        static uint16_t node = 0;
        static int64_t arg=0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg s64",ImGuiDataType_S64, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();
            packetsign * TestPacket = (packetsign *)malloc(2+2+4+16 + sizeof(node)); // 8 is for data
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = 16;
            memset(TestPacket->Data,0,TestPacket->Length + sizeof(node));
            uint8_t cmd2[16] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(int64_t));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x09;
            cmd2[7] = 0;
            uint16_t len = 16;
            const uint8_t *byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) {
                checksum ^= *(byteptr++);
            }
            cmd2[7]=checksum; // checksum
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for(int i=0; i<TestPacket->Length; i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::svc::SendUplink((void *)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }
    case 9:  { // float
        static uint16_t msgid = 0;
        static uint8_t fnccode=0;
        static uint16_t node = 0;
        static float arg =0;
        ImGui::InputScalar("msgid",ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("node", ImGuiDataType_U16, &node);
        ImGui::InputScalar("arg float",ImGuiDataType_Float, &arg);
        if(ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            mim::svc::WaitTx();
            packetsign * TestPacket = (packetsign *)malloc(2+2+4+12 + sizeof(node)); // 8 is for data
            TestPacket->Identifier = HVD_TEST;
            TestPacket->PacketType = MIM_PT_NODE_TC;
            TestPacket->Length = 12;
            memset(TestPacket->Data,0,TestPacket->Length + sizeof(node));
            uint8_t cmd2[12] = {0,};
            msgid = htons(msgid);
            memcpy(cmd2, &msgid, sizeof(uint16_t));
            memcpy(cmd2 + 6, &fnccode, sizeof(uint8_t));
            memcpy(cmd2 + 8, &arg, sizeof(float));
            cmd2[2] = 0xc0;
            cmd2[3] = 0x00;
            cmd2[4] = 0x00;
            cmd2[5] = 0x05;
            cmd2[7] = 0;
            uint16_t len = 12;
            const uint8_t *byteptr = cmd2;
            uint8_t checksum = 0xFF;
            while (len--) {
                checksum ^= *(byteptr++);
            }
            cmd2[7]=checksum; // checksum
            memcpy(TestPacket->Data, &node, sizeof(node));
            memcpy(TestPacket->Data+2, cmd2, sizeof(cmd2));
            for(int i=0; i<TestPacket->Length; i++) {
                printf("0x%x ",cmd2[i]);
            } printf("\n");
            mim::svc::SendUplink((void *)TestPacket);
            msgid = htons(msgid);
        }
        break;
    }












        // ==================== ci_lab ====================
    default:
        return false;
    }
    return true;
}
