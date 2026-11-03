// View - 명령 생성기 입력 폼: UELYSYS payuel_lgpm (명령 번호 7000, 7001, 7002, 7003, 7004, 7005, 7006, 7007, 7008, 7009, 7010, 7011, 7012, 7013, 7014, 7015, 7016, 7017, 7018, 7019, 7020, 7021, 7022, 7023)
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

bool DrawUelysysPayuelLgpmForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 7000: { /* 230 : LGPM NOOP (CC 0, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_NOOP_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_NoopCmd_t) - 7};

            memcpy(command5->lgpmnoopcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmnoopcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmnoopcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmnoopcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmnoopcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_NoopCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmnoopcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmnoopcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_NoopCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_NoopCmd_t);

            memcpy(pkt->Data, &command5->lgpmnoopcmd, sizeof(PAYUEL_LGPM_NoopCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7001: { /* 231 : LGPM Reset Counter (CC 1, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RESET_COUNTERS_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_ResetCountersCmd_t) - 7};

            memcpy(command5->lgpmresetcountercmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmresetcountercmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmresetcountercmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmresetcountercmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmresetcountercmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_ResetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmresetcountercmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmresetcountercmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_ResetCountersCmd_t);

            memcpy(pkt->Data, &command5->lgpmresetcountercmd, sizeof(PAYUEL_LGPM_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7002: { /* 232 : LGPM MCU_ALIVE_CHECK (CC 2, With Payload) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_MCU_ALIVE_CHECK_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Ground UTC Time", ImGuiDataType_U32,
            &command5->lgpmmcualivecheckcmd.Ground_UTC_Time);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t) - 7};

            memcpy(command5->lgpmmcualivecheckcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmmcualivecheckcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmmcualivecheckcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmmcualivecheckcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmmcualivecheckcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmmcualivecheckcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmmcualivecheckcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmmcualivecheckcmd, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7003: { /* 233 : LGPM 3V3_PWR_ON (CC 3, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_3V3_PWR_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t) - 7};

            memcpy(command5->lgpm3v3pwroncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpm3v3pwroncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpm3v3pwroncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpm3v3pwroncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpm3v3pwroncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpm3v3pwroncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpm3v3pwroncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpm3v3pwroncmd, sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7004: { /* 234 : LGPM 3V3_PWR_OFF (CC 4, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_3V3_PWR_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t) - 7};

            memcpy(command5->lgpm3v3pwroffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpm3v3pwroffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpm3v3pwroffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpm3v3pwroffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpm3v3pwroffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpm3v3pwroffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpm3v3pwroffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpm3v3pwroffcmd, sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7005: { /* 235 : LGPM MAIN_BOOST_SW_ON (CC 5, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_MAIN_BOOST_SW_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t) - 7};

            memcpy(command5->lgpmmainboostswoncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmmainboostswoncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmmainboostswoncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmmainboostswoncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmmainboostswoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmmainboostswoncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmmainboostswoncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmmainboostswoncmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7006: { /* 236 : LGPM MAIN_BOOST_SW_OFF (CC 6, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_MAIN_BOOST_SW_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmmainboostswoffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmmainboostswoffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmmainboostswoffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmmainboostswoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmmainboostswoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmmainboostswoffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmmainboostswoffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmmainboostswoffcmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7007: { /* 237 : LGPM SUB_BOOST_SW_ON (CC 7, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_SUB_BOOST_SW_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t) - 7};

            memcpy(command5->lgpmsubboostswoncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmsubboostswoncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmsubboostswoncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmsubboostswoncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmsubboostswoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmsubboostswoncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmsubboostswoncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmsubboostswoncmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7008: { /* 238 : LGPM SUB_BOOST_SW_OFF (CC 8, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_SUB_BOOST_SW_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmsubboostswoffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmsubboostswoffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmsubboostswoffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmsubboostswoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmsubboostswoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmsubboostswoffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmsubboostswoffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmsubboostswoffcmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7009: { /* 239 : LGPM V28_MAIN_ON (CC 9, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V28_MAIN_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t) - 7};

            memcpy(command5->lgpmv28mainoncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv28mainoncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv28mainoncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv28mainoncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv28mainoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv28mainoncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv28mainoncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv28mainoncmd, sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7010: { /* 240 : LGPM V28_MAIN_OFF (CC 10, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V28_MAIN_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmv28mainoffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv28mainoffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv28mainoffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv28mainoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv28mainoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv28mainoffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv28mainoffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv28mainoffcmd, sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7011: { /* 241 : LGPM V28_SUB_ON (CC 11, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V28_SUB_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t) - 7};

            memcpy(command5->lgpmv28suboncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv28suboncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv28suboncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv28suboncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv28suboncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv28suboncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv28suboncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv28suboncmd, sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7012: { /* 242 : LGPM V28_SUB_OFF (CC 12, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V28_SUB_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmv28suboffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv28suboffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv28suboffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv28suboffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv28suboffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv28suboffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv28suboffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv28suboffcmd, sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7013: { /* 243 : LGPM V12_MAIN_ON (CC 13, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V12_MAIN_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t) - 7};

            memcpy(command5->lgpmv12mainoncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv12mainoncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv12mainoncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv12mainoncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv12mainoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv12mainoncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv12mainoncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv12mainoncmd, sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7014: { /* 244 : LGPM V12_MAIN_OFF (CC 14, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_V12_MAIN_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmv12mainoffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmv12mainoffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmv12mainoffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmv12mainoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmv12mainoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmv12mainoffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmv12mainoffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmv12mainoffcmd, sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7015: { /* 245 : LGPM PWR_SENSE_INFO (CC 15, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_PWR_SENSE_INFO_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t) - 7};

            memcpy(command5->lgpmpwrsenseinfocmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmpwrsenseinfocmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmpwrsenseinfocmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmpwrsenseinfocmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmpwrsenseinfocmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmpwrsenseinfocmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmpwrsenseinfocmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmpwrsenseinfocmd, sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7016: { /* 246 : LGPM PWR_SEQ_ON (CC 16, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_PWR_SEQ_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t) - 7};

            memcpy(command5->lgpmpwrseqoncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmpwrseqoncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmpwrseqoncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmpwrseqoncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmpwrseqoncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmpwrseqoncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmpwrseqoncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmpwrseqoncmd, sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7017: { /* 247 : LGPM PWR_SEQ_OFF (CC 17, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_PWR_SEQ_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmpwrseqoffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmpwrseqoffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmpwrseqoffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmpwrseqoffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmpwrseqoffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmpwrseqoffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmpwrseqoffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmpwrseqoffcmd, sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7018: { /* 248 : LGPM RWA_CONTROL_idx1 (CC 18, With Payload) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_CONTROL_idx1_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Target Speed", ImGuiDataType_S16,
             &command5->lgpmrwacontrolidx1cmd.payload.TargetSpeed_RPM);
        ImGui::InputScalar("Target Acc", ImGuiDataType_S16,
             &command5->lgpmrwacontrolidx1cmd.payload.TargetAcc_RPM);
        ImGui::InputScalar("Duration", ImGuiDataType_S16,
             &command5->lgpmrwacontrolidx1cmd.payload.Operating_Time);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t) - 7};

            memcpy(command5->lgpmrwacontrolidx1cmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwacontrolidx1cmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwacontrolidx1cmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwacontrolidx1cmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwacontrolidx1cmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwacontrolidx1cmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwacontrolidx1cmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwacontrolidx1cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7019: { /* 249 : LGPM RWA_CONTROL_idx2 (CC 19, With Payload) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_CONTROL_idx2_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        // Payload Input
        ImGui::InputScalar("Motor Power", ImGuiDataType_U8, &command5->lgpmrwacontrolidx2cmd.payload.Motor_power);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t) - 7};

            memcpy(command5->lgpmrwacontrolidx2cmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwacontrolidx2cmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwacontrolidx2cmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwacontrolidx2cmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwacontrolidx2cmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwacontrolidx2cmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwacontrolidx2cmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwacontrolidx2cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7020: { /* 250 : LGPM RWA_CONTROL_idx3 (CC 20, With Payload) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_CONTROL_idx3_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        // Payload Input
        ImGui::InputScalar("Motor Driver EN", ImGuiDataType_U8, &command5->lgpmrwacontrolidx3cmd.payload.Motor_driver_EN);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t) - 7};

            memcpy(command5->lgpmrwacontrolidx3cmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwacontrolidx3cmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwacontrolidx3cmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwacontrolidx3cmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwacontrolidx3cmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwacontrolidx3cmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwacontrolidx3cmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwacontrolidx3cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7021: { /* 251 : LGPM RWA_PWR_ON (CC 21, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_PWR_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t) - 7};

            memcpy(command5->lgpmrwapwroncmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwapwroncmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwapwroncmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwapwroncmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwapwroncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwapwroncmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwapwroncmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwapwroncmd, sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7022: { /* 252 : LGPM RWA_PWR_OFF (CC 22, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_PWR_OFF_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t) - 7};

            memcpy(command5->lgpmrwapwroffcmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwapwroffcmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwapwroffcmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwapwroffcmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwapwroffcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwapwroffcmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwapwroffcmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwapwroffcmd, sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 7023: { /* 253 : LGPM RWA_SENSE_INFO (CC 23, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_SENSE_INFO_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t) - 7};

            memcpy(command5->lgpmrwasenseinfocmd.CmdHeader    , &mid    , 2);
            memcpy(command5->lgpmrwasenseinfocmd.CmdHeader + 2, seq     , 2);
            memcpy(command5->lgpmrwasenseinfocmd.CmdHeader + 4, len     , 2);
            memcpy(command5->lgpmrwasenseinfocmd.CmdHeader + 6, &fnccode, 1);

            command5->lgpmrwasenseinfocmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->lgpmrwasenseinfocmd;
            uint8_t crc = 0xFF;

            while (total--) crc ^= *(p++);
            command5->lgpmrwasenseinfocmd.CmdHeader[7] = crc;

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t);

            memcpy(pkt->Data, &command5->lgpmrwasenseinfocmd, sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    default:
        return false;
    }
    return true;
}
