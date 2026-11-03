// View - 명령 생성기 입력 폼: BEE1012 adcs (명령 번호 2700, 2701, 2705, 2706, 2708, 2709, 2710, 2711, 2712, 2713, 2714, 2715, 2716, 2717, 2718, 2719, 2720, 2721, 2722, 2723, 2724, 2725, 2726, 2727, 2728, 2729, 2730, 2731, 2732, 2733, 2734, 2735, 2736, 2737, 2738, 2739, 2740, 2741, 2742, 2743, 2744, 2745, 2751, 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759, 2760, 2761, 2762, 2763, 2764, 2765, 2766, 2767, 2768, 2769, 2770, 2771, 2772, 2773, 2774, 2775, 2776, 2777, 2778, 2779, 2780, 2781, 2782, 2783, 2784, 2785, 2786, 2787, 2790, 2791, 2800, 2801, 2802, 2803, 2804, 2805, 2810, 2814, 2818, 2819, 2820, 2821, 2822, 2823, 2824, 2825, 2826, 2827)
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

bool DrawBee1012AdcsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 2700: {
            UELYSYS_NoArgCmd_t &cmd = command5->uelysys_noargcmd;
            static uint16_t msgid = (uint16_t)(BEE1012_ADCS2_CMD_ID);
            static uint8_t fnccode = (uint8_t)(BEE1012_ADCS2_NOOP_CC);
            ImGui::InputScalar("msgid##direct_2700", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2700", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2700")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(UELYSYS_NoArgCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(UELYSYS_NoArgCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(UELYSYS_NoArgCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(UELYSYS_NoArgCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

    case 2701: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_RESET_COUNTERS_CC;   // 1

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_ResetCountersCmd_t) - 7};

            memcpy(command5->adcsresetcounterscmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsresetcounterscmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsresetcounterscmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsresetcounterscmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsresetcounterscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ResetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsresetcounterscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsresetcounterscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ResetCountersCmd_t);
            memcpy(pkt->Data, &command5->adcsresetcounterscmd,
                   sizeof(ADCS_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 51 : ADCS Set Interface Transport (CC 91) */
    case 2705: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_ENABLE_HIGH_CC;   // 5

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_GpioEnHighCmd_t) - 7};

            memcpy(command5->adcsgpioenhighcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsgpioenhighcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsgpioenhighcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsgpioenhighcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsgpioenhighcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_GpioEnHighCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsgpioenhighcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsgpioenhighcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_GpioEnHighCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_GpioEnHighCmd_t);
            memcpy(pkt->Data, &command5->adcsgpioenhighcmd,
                   sizeof(ADCS_GpioEnHighCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 53 : ADCS GPIO Enable Low (CC 6, no-arg) */
    case 2706: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_ENABLE_LOW_CC;   // 6

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_GpioEnLowCmd_t) - 7};

            memcpy(command5->adcsgpioenlowcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsgpioenlowcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsgpioenlowcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsgpioenlowcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsgpioenlowcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_GpioEnLowCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsgpioenlowcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsgpioenlowcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_GpioEnLowCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_GpioEnLowCmd_t);
            memcpy(pkt->Data, &command5->adcsgpioenlowcmd,
                   sizeof(ADCS_GpioEnLowCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 54 : ADCS GPIO Boot High (CC 8, no-arg) */
    case 2708: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_BOOT_HIGH_CC;   // 8

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_GpioBootHighCmd_t) - 7};

            memcpy(command5->adcsgioboothighcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsgioboothighcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsgioboothighcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsgioboothighcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsgioboothighcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_GpioBootHighCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsgioboothighcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsgioboothighcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_GpioBootHighCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_GpioBootHighCmd_t);
            memcpy(pkt->Data, &command5->adcsgioboothighcmd,
                   sizeof(ADCS_GpioBootHighCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 55 : ADCS GPIO Boot Low (CC 9, no-arg) */
    case 2709: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_BOOT_LOW_CC;   // 9

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_GpioBootLowCmd_t) - 7};

            memcpy(command5->adcsgiobootlowcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsgiobootlowcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsgiobootlowcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsgiobootlowcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsgiobootlowcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_GpioBootLowCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsgiobootlowcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsgiobootlowcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_GpioBootLowCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_GpioBootLowCmd_t);
            memcpy(pkt->Data, &command5->adcsgiobootlowcmd,
                   sizeof(ADCS_GpioBootLowCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 56 : ADCS Exit Bootloader (CC 10, no-arg) */
    case 2710: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_EXIT_BOOTLOADER_CC;   // 10

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_ExitBootLoaderCmd_t) - 7};

            memcpy(command5->adcsexitbootloadercmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsexitbootloadercmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsexitbootloadercmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsexitbootloadercmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsexitbootloadercmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ExitBootLoaderCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsexitbootloadercmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsexitbootloadercmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_ExitBootLoaderCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ExitBootLoaderCmd_t);
            memcpy(pkt->Data, &command5->adcsexitbootloadercmd,
                   sizeof(ADCS_ExitBootLoaderCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 57 : ADCS Reset (CC 11, no-arg) */
    case 2711: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_RESET_CC;   // 11

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00, sizeof(ADCS_ResetCmd_t) - 7};

            memcpy(command5->adcsresetcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsresetcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsresetcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsresetcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsresetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ResetCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->adcsresetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsresetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_ResetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ResetCmd_t);
            memcpy(pkt->Data, &command5->adcsresetcmd,
                   sizeof(ADCS_ResetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 58 : ADCS Set Current Unix Time (CC 12, payload) */
    case 2712: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CURRENT_UNIX_TIME_CC;   // 12

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Unix Seconds",     ImGuiDataType_U32,
                           &command5->adcscurrentunixtimecmd.Payload.CurrentUnixseconds);
        ImGui::InputScalar("Unix Nanoseconds", ImGuiDataType_U32,
                           &command5->adcscurrentunixtimecmd.Payload.CurrentUnixNanoseconds);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00,
                               sizeof(ADCS_CurrentUnixTimeCmd_t) - 7};

            memcpy(command5->adcscurrentunixtimecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcscurrentunixtimecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcscurrentunixtimecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcscurrentunixtimecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcscurrentunixtimecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_CurrentUnixTimeCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcscurrentunixtimecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcscurrentunixtimecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_CurrentUnixTimeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_CurrentUnixTimeCmd_t);
            memcpy(pkt->Data, &command5->adcscurrentunixtimecmd,
                   sizeof(ADCS_CurrentUnixTimeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

/* 59 : ADCS Error Log Setting (CC 13, payload) */
case 2713: {
    static uint16_t msgid   = ADCS_CMD_ID;
    static uint8_t  fnccode = ADCS_SET_ERROR_LOG_SETTING_CC;   // 13

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);


    uint8_t active_tmp =
        command5->adcserrorlogsettingcmd.Payload.ActiveState ? 1 : 0;
    uint8_t buffer_tmp =
        command5->adcserrorlogsettingcmd.Payload.BufferFullAction ? 1 : 0;

    ImGui::InputScalar("ActiveState",      ImGuiDataType_U8, &active_tmp);
    ImGui::InputScalar("BufferFullAction", ImGuiDataType_U8, &buffer_tmp);

    command5->adcserrorlogsettingcmd.Payload.ActiveState      = active_tmp & 0x1;
    command5->adcserrorlogsettingcmd.Payload.BufferFullAction = buffer_tmp & 0x1;

    if (ImGui::Button("Generate CMD")) {

        WriteSystemName(msgid);
        uint16_t mid = htons(msgid);
        uint8_t  seq[2] = {0xC0, 0x00};
        uint8_t  len[2] = {0x00,
                           sizeof(ADCS_ErrorLogSettingCmd_t) - 7};

        memcpy(command5->adcserrorlogsettingcmd.CmdHeader,     &mid, 2);
        memcpy(command5->adcserrorlogsettingcmd.CmdHeader + 2, seq,  2);
        memcpy(command5->adcserrorlogsettingcmd.CmdHeader + 4, len,  2);
        memcpy(command5->adcserrorlogsettingcmd.CmdHeader + 6, &fnccode, 1);

        command5->adcserrorlogsettingcmd.CmdHeader[7] = 0;
        uint16_t total = sizeof(ADCS_ErrorLogSettingCmd_t);
        const uint8_t *p =
            (const uint8_t *)&command5->adcserrorlogsettingcmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(p++);

        memcpy(command5->adcserrorlogsettingcmd.CmdHeader + 7, &crc, 1);

        packetsign *pkt =
            (packetsign *)malloc(2 + 2 + 4 +
                                 sizeof(ADCS_ErrorLogSettingCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(ADCS_ErrorLogSettingCmd_t);
        memcpy(pkt->Data, &command5->adcserrorlogsettingcmd,
               sizeof(ADCS_ErrorLogSettingCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }
    break;
}


    /* 60 : ADCS Persist Config (CC 14, no-arg) */
    case 2714: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_PERSIST_CONFIG_CC;   // 14

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {0x00,
                               sizeof(ADCS_PersistConfigCmd_t) - 7};

            memcpy(command5->adcspersistconfigcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcspersistconfigcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcspersistconfigcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcspersistconfigcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcspersistconfigcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_PersistConfigCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcspersistconfigcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcspersistconfigcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_PersistConfigCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_PersistConfigCmd_t);
            memcpy(pkt->Data, &command5->adcspersistconfigcmd,
                   sizeof(ADCS_PersistConfigCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 61 : ADCS Set Control Estimation Mode (CC 15) */
    case 2715: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CONTROL_ESTIMATION_MODE_CC;   // 15

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("ControlMode",        ImGuiDataType_U8,
                           &command5->adcscontrolestimationmodecmd.Payload.ControlMode);
        ImGui::InputScalar("MainEstimatorMode",  ImGuiDataType_U8,
                           &command5->adcscontrolestimationmodecmd.Payload.MainEstimatorMode);
        ImGui::InputScalar("BackupEstimatorMode",ImGuiDataType_U8,
                           &command5->adcscontrolestimationmodecmd.Payload.BackupEstimatorMode);
        ImGui::InputScalar("ControlTimeout",     ImGuiDataType_U16,
                           &command5->adcscontrolestimationmodecmd.Payload.ControlTimeout);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_ControlEstimationModeCmd_t) - 7)
            };

            memcpy(command5->adcscontrolestimationmodecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcscontrolestimationmodecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcscontrolestimationmodecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcscontrolestimationmodecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcscontrolestimationmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ControlEstimationModeCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcscontrolestimationmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcscontrolestimationmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_ControlEstimationModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ControlEstimationModeCmd_t);
            memcpy(pkt->Data, &command5->adcscontrolestimationmodecmd,
                   sizeof(ADCS_ControlEstimationModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 62 : ADCS Disable Mag/RWL/MntMng (CC 16) */
    case 2716: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_DISABLE_MAG_RWL_MNT_MNG_CC;   // 16

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Duration", ImGuiDataType_U16,
                           &command5->adcsdisablemagrwlmntmngcmd.Payload.Duration);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_DisableMagRwlMntMngCmd_t) - 7)
            };

            memcpy(command5->adcsdisablemagrwlmntmngcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsdisablemagrwlmntmngcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsdisablemagrwlmntmngcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsdisablemagrwlmntmngcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsdisablemagrwlmntmngcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_DisableMagRwlMntMngCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsdisablemagrwlmntmngcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsdisablemagrwlmntmngcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_DisableMagRwlMntMngCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_DisableMagRwlMntMngCmd_t);
            memcpy(pkt->Data, &command5->adcsdisablemagrwlmntmngcmd,
                   sizeof(ADCS_DisableMagRwlMntMngCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 63 : ADCS Set Reference IRC Vector (CC 17) */
    case 2717: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_IRC_VECTOR_CC;   // 17

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("ECI X", ImGuiDataType_Float,
                           &command5->adcsreferenceircvectorcmd.Payload.ECIPointingVectorX);
        ImGui::InputScalar("ECI Y", ImGuiDataType_Float,
                           &command5->adcsreferenceircvectorcmd.Payload.ECIPointingVectorY);
        ImGui::InputScalar("ECI Z", ImGuiDataType_Float,
                           &command5->adcsreferenceircvectorcmd.Payload.ECIPointingVectorZ);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_ReferenceIRCVectorCmd_t) - 7)
            };

            memcpy(command5->adcsreferenceircvectorcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsreferenceircvectorcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsreferenceircvectorcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsreferenceircvectorcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsreferenceircvectorcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ReferenceIRCVectorCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsreferenceircvectorcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsreferenceircvectorcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_ReferenceIRCVectorCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ReferenceIRCVectorCmd_t);
            memcpy(pkt->Data, &command5->adcsreferenceircvectorcmd,
                   sizeof(ADCS_ReferenceIRCVectorCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 64 : ADCS Set Reference LLH Target (CC 18) */
    case 2718: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_LLH_TARGET_CC;   // 18

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Lat [deg]", ImGuiDataType_Float,
                           &command5->adcsreferencellhtargetcmd.Payload.TargetLatitude);
        ImGui::InputScalar("Lon [deg]", ImGuiDataType_Float,
                           &command5->adcsreferencellhtargetcmd.Payload.TargetLongiTude);
        ImGui::InputScalar("Alt [m]",   ImGuiDataType_Float,
                           &command5->adcsreferencellhtargetcmd.Payload.TargetAltitude);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_ReferenceLLHTargetCmd_t) - 7)
            };

            memcpy(command5->adcsreferencellhtargetcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsreferencellhtargetcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsreferencellhtargetcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsreferencellhtargetcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsreferencellhtargetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ReferenceLLHTargetCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsreferencellhtargetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsreferencellhtargetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_ReferenceLLHTargetCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ReferenceLLHTargetCmd_t);
            memcpy(pkt->Data, &command5->adcsreferencellhtargetcmd,
                   sizeof(ADCS_ReferenceLLHTargetCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 65 : ADCS Set Orbit Mode (CC 19) */
    case 2719: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_ORBIT_MODE_CC;   // 19

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("OrbitMode",ImGuiDataType_U8,
                           &command5->adcsorbitmodecmd.Payload.OrbitMode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_OrbitModeCmd_t) - 7)
            };

            memcpy(command5->adcsorbitmodecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsorbitmodecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsorbitmodecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsorbitmodecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsorbitmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_OrbitModeCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsorbitmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsorbitmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_OrbitModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_OrbitModeCmd_t);
            memcpy(pkt->Data, &command5->adcsorbitmodecmd,
                   sizeof(ADCS_OrbitModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }
/* 66 : ADCS MAG Deploy Command (CC 20) */
case 2720: {
    static uint16_t msgid   = ADCS_CMD_ID;
    static uint8_t  fnccode = ADCS_SET_MAG_DEPLOY_CMD_CC;   // 20

    static uint8_t deploy0 = 0;
    static uint8_t deploy1 = 0;


    deploy0 = command5->adcsmagdeploycmd.Payload.DeployMAG0;
    deploy1 = command5->adcsmagdeploycmd.Payload.DeployMAG1;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::InputScalar("DeployMAG0", ImGuiDataType_U8, &deploy0);
    ImGui::InputScalar("DeployMAG1", ImGuiDataType_U8, &deploy1);

    if (ImGui::Button("Generate CMD")) {


        command5->adcsmagdeploycmd.Payload.DeployMAG0 = deploy0 & 0x1;
        command5->adcsmagdeploycmd.Payload.DeployMAG1 = deploy1 & 0x1;

        WriteSystemName(msgid);
        uint16_t mid  = htons(msgid);
        uint8_t  seq[2] = {0xC0, 0x00};
        uint8_t  len[2] = {
            0x00, (uint8_t)(sizeof(ADCS_MagDeployCmd_t) - 7)
        };

        memcpy(command5->adcsmagdeploycmd.CmdHeader,     &mid, 2);
        memcpy(command5->adcsmagdeploycmd.CmdHeader + 2, seq,  2);
        memcpy(command5->adcsmagdeploycmd.CmdHeader + 4, len,  2);
        memcpy(command5->adcsmagdeploycmd.CmdHeader + 6, &fnccode, 1);

        command5->adcsmagdeploycmd.CmdHeader[7] = 0;
        uint16_t total = sizeof(ADCS_MagDeployCmd_t);
        const uint8_t *p =
            (const uint8_t *)&command5->adcsmagdeploycmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(p++);

        memcpy(command5->adcsmagdeploycmd.CmdHeader + 7, &crc, 1);

        packetsign *pkt = (packetsign *)malloc(
            2 + 2 + 4 + sizeof(ADCS_MagDeployCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(ADCS_MagDeployCmd_t);
        memcpy(pkt->Data, &command5->adcsmagdeploycmd,
               sizeof(ADCS_MagDeployCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }
    break;
}

    /* 67 : ADCS Set Reference RPY Values (CC 21) */
    case 2721: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_RPY_VALUES_CC;   // 21

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Roll [deg]",  ImGuiDataType_Float,
                           &command5->adcsreferencerpyvaluescmd.Payload.Roll);
        ImGui::InputScalar("Pitch [deg]", ImGuiDataType_Float,
                           &command5->adcsreferencerpyvaluescmd.Payload.Pitch);
        ImGui::InputScalar("Yaw [deg]",   ImGuiDataType_Float,
                           &command5->adcsreferencerpyvaluescmd.Payload.Yaw);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_ReferenceRPYvaluesCmd_t) - 7)
            };

            memcpy(command5->adcsreferencerpyvaluescmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsreferencerpyvaluescmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsreferencerpyvaluescmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsreferencerpyvaluescmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsreferencerpyvaluescmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ReferenceRPYvaluesCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsreferencerpyvaluescmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsreferencerpyvaluescmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_ReferenceRPYvaluesCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ReferenceRPYvaluesCmd_t);
            memcpy(pkt->Data, &command5->adcsreferencerpyvaluescmd,
                   sizeof(ADCS_ReferenceRPYvaluesCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 68 : ADCS Set Open Loop MTQ Command (CC 22) */
    case 2722: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_OPENLOOPCMD_MTQ_CC;   // 22

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("MTQ0 Cmd", ImGuiDataType_S16,
                           &command5->adcsopenloopcmdmtqcmd.Payload.MTQ0_OpenLoopCmd);
        ImGui::InputScalar("MTQ1 Cmd", ImGuiDataType_S16,
                           &command5->adcsopenloopcmdmtqcmd.Payload.MTQ1_OpenLoopCmd);
        ImGui::InputScalar("MTQ2 Cmd", ImGuiDataType_S16,
                           &command5->adcsopenloopcmdmtqcmd.Payload.MTQ2_OpenLoopCmd);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_OpenLoopCmdMTQCmd_t) - 7)
            };

            memcpy(command5->adcsopenloopcmdmtqcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsopenloopcmdmtqcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsopenloopcmdmtqcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsopenloopcmdmtqcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsopenloopcmdmtqcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_OpenLoopCmdMTQCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsopenloopcmdmtqcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsopenloopcmdmtqcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_OpenLoopCmdMTQCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_OpenLoopCmdMTQCmd_t);
            memcpy(pkt->Data, &command5->adcsopenloopcmdmtqcmd,
                   sizeof(ADCS_OpenLoopCmdMTQCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }


    case 2723: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_POWER_STATE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("RWL0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.RWL0);
        ImGui::InputScalar("RWL1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.RWL1);
        ImGui::InputScalar("RWL2", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.RWL2);
        ImGui::InputScalar("RWL3", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.RWL3);

        ImGui::InputScalar("MAG0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.MAG0);
        ImGui::InputScalar("MAG1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.MAG1);

        ImGui::InputScalar("GYR0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.GYR0);
        ImGui::InputScalar("GYR1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.GYR1);

        ImGui::InputScalar("FSS0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.FSS0);
        ImGui::InputScalar("FSS1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.FSS1);
        ImGui::InputScalar("FSS2", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.FSS2);
        ImGui::InputScalar("FSS3", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.FSS3);

        ImGui::InputScalar("HSS0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.HSS0);
        ImGui::InputScalar("HSS1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.HSS1);

        ImGui::InputScalar("STR0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.STR0);
        ImGui::InputScalar("STR1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.STR1);

        ImGui::InputScalar("ExtSensor0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.ExtSensor0);
        ImGui::InputScalar("ExtSensor1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.ExtSensor1);

        ImGui::InputScalar("ExtGYR0", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.ExtGYR0);
        ImGui::InputScalar("ExtGYR1", ImGuiDataType_U8,
                           &command5->adcspowerstatecmd.Payload.ExtGYR1);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_PowerStateCmd_t) - 7)
            };

            memcpy(command5->adcspowerstatecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcspowerstatecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcspowerstatecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcspowerstatecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcspowerstatecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_PowerStateCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcspowerstatecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcspowerstatecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_PowerStateCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_PowerStateCmd_t);
            memcpy(pkt->Data, &command5->adcspowerstatecmd,
                   sizeof(ADCS_PowerStateCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2724: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_RUN_MODE_CC;   // 24

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);
        ImGui::InputScalar("RunMode",  ImGuiDataType_U8,
                           &command5->adcsrunmodecmd.Payload.RunMode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_RunModeCmd_t) - 7)
            };

            memcpy(command5->adcsrunmodecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsrunmodecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsrunmodecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsrunmodecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsrunmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_RunModeCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsrunmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsrunmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_RunModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_RunModeCmd_t);
            memcpy(pkt->Data, &command5->adcsrunmodecmd,
                   sizeof(ADCS_RunModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 71 : ADCS Set Control Mode (CC 25) */
    case 2725: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CONTROL_MODE_CC;   // 25

        ImGui::InputScalar("msgid",        ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",      ImGuiDataType_U8,  &fnccode);
        ImGui::InputScalar("ControlMode",  ImGuiDataType_U8,
                           &command5->adcscontrolmodecmd.Payload.ControlMode);
        ImGui::InputScalar("ControlTimeout", ImGuiDataType_U16,
                           &command5->adcscontrolmodecmd.Payload.Controltimeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00, (uint8_t)(sizeof(ADCS_ControlModeCmd_t) - 7)
            };

            memcpy(command5->adcscontrolmodecmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcscontrolmodecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcscontrolmodecmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcscontrolmodecmd.CmdHeader + 6, &fnccode, 1);

            command5->adcscontrolmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_ControlModeCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcscontrolmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcscontrolmodecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(
                2 + 2 + 4 + sizeof(ADCS_ControlModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_ControlModeCmd_t);
            memcpy(pkt->Data, &command5->adcscontrolmodecmd,
                   sizeof(ADCS_ControlModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }



    // case 72: {
    //     static uint16_t msgid   = ADCS_CMD_ID;
    //     static uint8_t  fnccode = ADCS_SET_WHL_CONFIG_CC;

    //     ImGui::InputScalar("msgid",   ImGuiDataType_U16,  &msgid);
    //     ImGui::InputScalar("fnccode", ImGuiDataType_U8,   &fnccode);

    //     ImGui::InputScalar("Rwl0Inertia",      ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl0Inertia);
    //     ImGui::InputScalar("Rwl0MaxMomentum",  ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl0MaxMomentum);
    //     ImGui::InputScalar("Rwl0MaxToque",     ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl0MaxToque);

    //     ImGui::InputScalar("Rwl1Inertia",      ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl1Inertia);
    //     ImGui::InputScalar("Rwl1MaxMomentum",  ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl1MaxMomentum);
    //     ImGui::InputScalar("Rwl1MaxToque",     ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl1MaxToque);

    //     ImGui::InputScalar("Rwl2Inertia",      ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl2Inertia);
    //     ImGui::InputScalar("Rwl2MaxMomentum",  ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl2MaxMomentum);
    //     ImGui::InputScalar("Rwl2MaxToque",     ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl2MaxToque);

    //     ImGui::InputScalar("Rwl3Inertia",      ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl3Inertia);
    //     ImGui::InputScalar("Rwl3MaxMomentum",  ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl3MaxMomentum);
    //     ImGui::InputScalar("Rwl3MaxToque",     ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.Rwl3MaxToque);

    //     ImGui::InputScalar("WheelRampTorque",  ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.WheelRampTorque);

    //     ImGui::InputScalar("WheelScheme",      ImGuiDataType_U8,
    //                        &command5->adcswhlconfigcmd.Payload.WheelScheme);
    //     ImGui::InputScalar("FailedWheelID",    ImGuiDataType_U8,
    //                        &command5->adcswhlconfigcmd.Payload.FailedWheelID);

    //     ImGui::InputScalar("PyramidNominalMomentum",
    //                        ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.PyramidNominalMomentum);
    //     ImGui::InputScalar("PyramidTiltAngle",
    //                        ImGuiDataType_Float,
    //                        &command5->adcswhlconfigcmd.Payload.PyramidTiltAngle);

    //     if (ImGui::Button("Generate CMD")) {
    //         WriteSystemName(msgid);
    //         uint16_t mid  = htons(msgid);
    //         uint8_t  seq[2] = {0xC0, 0x00};
    //         uint8_t  len[2] = {
    //             0x00, (uint8_t)(sizeof(ADCS_WhlConfigCmd_t) - 7)
    //         };

    //         memcpy(command5->adcswhlconfigcmd.CmdHeader,     &mid, 2);
    //         memcpy(command5->adcswhlconfigcmd.CmdHeader + 2, seq,  2);
    //         memcpy(command5->adcswhlconfigcmd.CmdHeader + 4, len,  2);
    //         memcpy(command5->adcswhlconfigcmd.CmdHeader + 6, &fnccode, 1);

    //         uint16_t total = sizeof(ADCS_WhlConfigCmd_t);
    //         const uint8_t *p =
    //             (const uint8_t *)&command5->adcswhlconfigcmd;
    //         uint8_t crc = 0xFF;
    //         while (total--) crc ^= *(p++);

    //         memcpy(command5->adcswhlconfigcmd.CmdHeader + 7, &crc, 1);

    //         packetsign *pkt = (packetsign *)malloc(
    //             2 + 2 + 4 + sizeof(ADCS_WhlConfigCmd_t));
    //         pkt->Identifier = HVD_TEST;
    //         pkt->PacketType = MIM_PT_TMTC_TEST;
    //         pkt->Length     = sizeof(ADCS_WhlConfigCmd_t);
    //         memcpy(pkt->Data, &command5->adcswhlconfigcmd,
    //                sizeof(ADCS_WhlConfigCmd_t));

    //         mim::svc::WaitTx();
    //         pthread_create(&p_thread[4], NULL,
    //                        task_uplink_onorbit, (void *)pkt);
    //     }
    //     break;
    // }
        case 2726: {

            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_WHL_CONFIG_CC;


            static bool initialized = false;
            if (!initialized) {
                command5->adcswhlconfigcmd.Payload.Rwl0Inertia        = 9.51015E-06 ;
                command5->adcswhlconfigcmd.Payload.Rwl0MaxMomentum    = 0.00996 ;
                command5->adcswhlconfigcmd.Payload.Rwl0MaxToque       = 0.002 ;

                command5->adcswhlconfigcmd.Payload.Rwl1Inertia        = 9.51015E-06;
                command5->adcswhlconfigcmd.Payload.Rwl1MaxMomentum    = 0.00996;
                command5->adcswhlconfigcmd.Payload.Rwl1MaxToque       = 0.002;

                command5->adcswhlconfigcmd.Payload.Rwl2Inertia        = 9.51015E-06;
                command5->adcswhlconfigcmd.Payload.Rwl2MaxMomentum    = 0.00996;
                command5->adcswhlconfigcmd.Payload.Rwl2MaxToque       = 0.002;

                command5->adcswhlconfigcmd.Payload.Rwl3Inertia        = 9.51015E-06;
                command5->adcswhlconfigcmd.Payload.Rwl3MaxMomentum    = 0.00996;
                command5->adcswhlconfigcmd.Payload.Rwl3MaxToque       = 0.002;

                command5->adcswhlconfigcmd.Payload.WheelRampTorque    = 0.0002;

                command5->adcswhlconfigcmd.Payload.WheelScheme        = 3;
                command5->adcswhlconfigcmd.Payload.FailedWheelID      = 0;

                command5->adcswhlconfigcmd.Payload.PyramidNominalMomentum = 0.001;
                command5->adcswhlconfigcmd.Payload.PyramidTiltAngle       = 26.565;

                initialized = true;
            }


            ImGui::Text("ADCS Wheel Config (fixed parameters)");
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ImGui::Text("=== ADCS Wheel Config Parameters (Fixed) ===");
            ImGui::Separator();

            ImGui::Text("Rwl0Inertia          : %.6f", command5->adcswhlconfigcmd.Payload.Rwl0Inertia);
            ImGui::Text("Rwl0MaxMomentum      : %.6f", command5->adcswhlconfigcmd.Payload.Rwl0MaxMomentum);
            ImGui::Text("Rwl0MaxTorque        : %.6f", command5->adcswhlconfigcmd.Payload.Rwl0MaxToque);

            ImGui::Text("Rwl1Inertia          : %.6f", command5->adcswhlconfigcmd.Payload.Rwl1Inertia);
            ImGui::Text("Rwl1MaxMomentum      : %.6f", command5->adcswhlconfigcmd.Payload.Rwl1MaxMomentum);
            ImGui::Text("Rwl1MaxTorque        : %.6f", command5->adcswhlconfigcmd.Payload.Rwl1MaxToque);

            ImGui::Text("Rwl2Inertia          : %.6f", command5->adcswhlconfigcmd.Payload.Rwl2Inertia);
            ImGui::Text("Rwl2MaxMomentum      : %.6f", command5->adcswhlconfigcmd.Payload.Rwl2MaxMomentum);
            ImGui::Text("Rwl2MaxTorque        : %.6f", command5->adcswhlconfigcmd.Payload.Rwl2MaxToque);

            ImGui::Text("Rwl3Inertia          : %.6f", command5->adcswhlconfigcmd.Payload.Rwl3Inertia);
            ImGui::Text("Rwl3MaxMomentum      : %.6f", command5->adcswhlconfigcmd.Payload.Rwl3MaxMomentum);
            ImGui::Text("Rwl3MaxTorque        : %.6f", command5->adcswhlconfigcmd.Payload.Rwl3MaxToque);

            ImGui::Text("WheelRampTorque      : %.6f", command5->adcswhlconfigcmd.Payload.WheelRampTorque);

            ImGui::Text("WheelScheme          : %u", command5->adcswhlconfigcmd.Payload.WheelScheme);
            ImGui::Text("FailedWheelID        : %u", command5->adcswhlconfigcmd.Payload.FailedWheelID);

            ImGui::Text("PyramidNominalMomentum : %.6f", command5->adcswhlconfigcmd.Payload.PyramidNominalMomentum);
            ImGui::Text("PyramidTiltAngle       : %.6f", command5->adcswhlconfigcmd.Payload.PyramidTiltAngle);

            ImGui::Separator();


            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid  = htons(msgid);
                uint8_t  seq[2] = { 0xC0, 0x00 };
                uint8_t  len[2] = {
                    0x00, (uint8_t)(sizeof(ADCS_WhlConfigCmd_t) - 7)
                };

                memcpy(command5->adcswhlconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcswhlconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcswhlconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcswhlconfigcmd.CmdHeader + 6, &fnccode, 1);

                command5->adcswhlconfigcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_WhlConfigCmd_t);
                const uint8_t *p =
                    (const uint8_t *)&command5->adcswhlconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command5->adcswhlconfigcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_WhlConfigCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_WhlConfigCmd_t);
                memcpy(pkt->Data, &command5->adcswhlconfigcmd,
                    sizeof(ADCS_WhlConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        case 2727: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_SATELLITE_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_SatConfigCmd_Payload_t &p = command5->adcssatconfigcmd.Payload;

                p.Ixx = 0.2252612;
                p.Iyy = 0.2369665;
                p.Izz = 0.1360058;
                p.Ixy = -0.0001218231;
                p.Ixz = 0.0002665824;
                p.Iyz = 0.001189326;

                p.SunPointingBodyVectorX = 0;
                p.SunPointingBodyVectorY = 0;
                p.SunPointingBodyVectorZ = -10000;

                p.TargetTrackingBodyVectorX = 0;
                p.TargetTrackingBodyVectorY = 0;
                p.TargetTrackingBodyVectorZ = 10000;

                p.SatTrackingBodyVectorX = 0;
                p.SatTrackingBodyVectorY = 0;
                p.SatTrackingBodyVectorZ = -10000;

                initialized = true;
            }
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ADCS_SatConfigCmd_Payload_t &p = command5->adcssatconfigcmd.Payload;

            ImGui::Text("=== Satellite Inertia Matrix ===");
            ImGui::Text("Ixx : %.6f", p.Ixx);
            ImGui::Text("Iyy : %.6f", p.Iyy);
            ImGui::Text("Izz : %.6f", p.Izz);
            ImGui::Text("Ixy : %.6f", p.Ixy);
            ImGui::Text("Ixz : %.6f", p.Ixz);
            ImGui::Text("Iyz : %.6f", p.Iyz);

            ImGui::Separator();
            ImGui::Text("=== Sun Pointing Body Vector ===");
            ImGui::Text("X : %d", p.SunPointingBodyVectorX);
            ImGui::Text("Y : %d", p.SunPointingBodyVectorY);
            ImGui::Text("Z : %d", p.SunPointingBodyVectorZ);

            ImGui::Separator();
            ImGui::Text("=== Target Tracking Body Vector ===");
            ImGui::Text("X : %d", p.TargetTrackingBodyVectorX);
            ImGui::Text("Y : %d", p.TargetTrackingBodyVectorY);
            ImGui::Text("Z : %d", p.TargetTrackingBodyVectorZ);

            ImGui::Separator();
            ImGui::Text("=== Sat Tracking Body Vector ===");
            ImGui::Text("X : %d", p.SatTrackingBodyVectorX);
            ImGui::Text("Y : %d", p.SatTrackingBodyVectorY);
            ImGui::Text("Z : %d", p.SatTrackingBodyVectorZ);

            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid     = htons(msgid);
                uint8_t  seq[2]  = { 0xC0, 0x00 };
                uint8_t  len[2]  = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_SatConfigCmd_t) - 7)
                };

                memcpy(command5->adcssatconfigcmd.CmdHeader,      &mid, 2);
                memcpy(command5->adcssatconfigcmd.CmdHeader + 2,  seq,  2);
                memcpy(command5->adcssatconfigcmd.CmdHeader + 4,  len,  2);
                memcpy(command5->adcssatconfigcmd.CmdHeader + 6,  &fnccode, 1);


                command5->adcssatconfigcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_SatConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcssatconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) {
                    crc ^= *(pp++);
                }
                memcpy(command5->adcssatconfigcmd.CmdHeader + 7, &crc, 1);


                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_SatConfigCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_SatConfigCmd_t);
                memcpy(pkt->Data,
                    &command5->adcssatconfigcmd,
                    sizeof(ADCS_SatConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }

        case 2728: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_CONTROLLER_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_ControllerConfig_Payload_t &p = command5->adcscontrollerconfigcmd.Payload;

                p.DefaultControlMode             = 0;

                p.DetumblingDampingGain          = 0.1;
                p.SunSpinGainSunlit              = -0.2;
                p.SunSpinGainEclipse             = 0.02;
                p.DetumblingSpinGain             = 0.05;
                p.FastBDotGain                   = 1;
                p.YMomentumNutationDampingGain   = 0.5;
                p.YMomentumQuatGain              = 0.4;
                p.XAxisGGQuatGain                = 1;
                p.YAxisGGQuatGain                = 1;
                p.ZAxisGGQuatGain                = 1;
                p.WheelDesaturationGain          = 0.2;
                p.YMomentumProportionalGain      = 0.005;
                p.YMomentumDerivativeGain        = 0.048;
                p.RWheelProportionalGain         = 0.02;
                p.RWheelDerivativeGain           = 0.096;
                p.TrackingProportionalGain       = 0.08;
                p.TrackingDerivativeGain         = 0.192;
                p.TrackingIntegralGain           = 0.0004;
                p.ReferenceSpinRate              = -1;
                p.ReferenceWheelMomentum         = -0.004;
                p.YWheelBiasMomentum             = 0;
                p.ReferenceSpinRateRWspinControl = 0;
                p.SunKeepOutAngle                = 0;
                p.RollLimitAngle                 = 0;

                p.flags.YawCompensationForEarthRotation = 0;
                p.flags.EnableSunTrackingInEclipse      = 1;
                p.flags.EnableSunAvoidance              = 0;
                p.flags.Reserved                        = 0;

                initialized = true;
            }
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ADCS_ControllerConfig_Payload_t &p = command5->adcscontrollerconfigcmd.Payload;

            ImGui::Text("=== Default Control Mode ===");
            ImGui::Text("DefaultControlMode : %u", p.DefaultControlMode);

            ImGui::Separator();
            ImGui::Text("=== Detumbling / Spin / B-dot Gains ===");
            ImGui::Text("DetumblingDampingGain        : %.6f", p.DetumblingDampingGain);
            ImGui::Text("SunSpinGainSunlit            : %.6f", p.SunSpinGainSunlit);
            ImGui::Text("SunSpinGainEclipse           : %.6f", p.SunSpinGainEclipse);
            ImGui::Text("DetumblingSpinGain           : %.6f", p.DetumblingSpinGain);
            ImGui::Text("FastBDotGain                 : %.6f", p.FastBDotGain);

            ImGui::Separator();
            ImGui::Text("=== Y-Momentum / Quat / GG Gains ===");
            ImGui::Text("YMomentumNutationDampingGain : %.6f", p.YMomentumNutationDampingGain);
            ImGui::Text("YMomentumQuatGain            : %.6f", p.YMomentumQuatGain);
            ImGui::Text("XAxisGGQuatGain              : %.6f", p.XAxisGGQuatGain);
            ImGui::Text("YAxisGGQuatGain              : %.6f", p.YAxisGGQuatGain);
            ImGui::Text("ZAxisGGQuatGain              : %.6f", p.ZAxisGGQuatGain);
            ImGui::Text("WheelDesaturationGain        : %.6f", p.WheelDesaturationGain);

            ImGui::Separator();
            ImGui::Text("=== Wheel / Tracking Gains ===");
            ImGui::Text("YMomentumProportionalGain    : %.6f", p.YMomentumProportionalGain);
            ImGui::Text("YMomentumDerivativeGain      : %.6f", p.YMomentumDerivativeGain);
            ImGui::Text("RWheelProportionalGain       : %.6f", p.RWheelProportionalGain);
            ImGui::Text("RWheelDerivativeGain         : %.6f", p.RWheelDerivativeGain);
            ImGui::Text("TrackingProportionalGain     : %.6f", p.TrackingProportionalGain);
            ImGui::Text("TrackingDerivativeGain       : %.6f", p.TrackingDerivativeGain);
            ImGui::Text("TrackingIntegralGain         : %.6f", p.TrackingIntegralGain);

            ImGui::Separator();
            ImGui::Text("=== Reference Values ===");
            ImGui::Text("ReferenceSpinRate              [deg/s] : %.6f", p.ReferenceSpinRate);
            ImGui::Text("ReferenceWheelMomentum         [Nms]   : %.6f", p.ReferenceWheelMomentum);
            ImGui::Text("YWheelBiasMomentum             [Nms]   : %.6f", p.YWheelBiasMomentum);
            ImGui::Text("ReferenceSpinRateRWspinControl [deg/s] : %.6f", p.ReferenceSpinRateRWspinControl);
            ImGui::Text("SunKeepOutAngle                [deg]   : %.6f", p.SunKeepOutAngle);
            ImGui::Text("RollLimitAngle                 [deg]   : %.6f", p.RollLimitAngle);

            ImGui::Separator();
            ImGui::Text("=== Flags ===");
            ImGui::Text("YawCompensationForEarthRotation : %u", p.flags.YawCompensationForEarthRotation);
            ImGui::Text("EnableSunTrackingInEclipse      : %u", p.flags.EnableSunTrackingInEclipse);
            ImGui::Text("EnableSunAvoidance              : %u", p.flags.EnableSunAvoidance);
            ImGui::Text("Reserved                        : %u", p.flags.Reserved);

            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid    = htons(msgid);
                uint8_t  seq[2] = { 0xC0, 0x00 };
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_ControllerConfig_t) - 7)
                };

                memcpy(command5->adcscontrollerconfigcmd.CmdHeader,      &mid, 2);
                memcpy(command5->adcscontrollerconfigcmd.CmdHeader + 2,  seq,  2);
                memcpy(command5->adcscontrollerconfigcmd.CmdHeader + 4,  len,  2);
                memcpy(command5->adcscontrollerconfigcmd.CmdHeader + 6,  &fnccode, 1);

                command5->adcscontrollerconfigcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_ControllerConfig_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcscontrollerconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) {
                    crc ^= *(pp++);
                }
                memcpy(command5->adcscontrollerconfigcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_ControllerConfig_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_ControllerConfig_t);
                memcpy(pkt->Data,
                    &command5->adcscontrollerconfigcmd,
                    sizeof(ADCS_ControllerConfig_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }

        case 2729: { // ADCS Set Mag0 MMT Calib Config (CC 29)
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_MAG0_MMT_CALIB_CONFIG_CC;

            static int16_t MMT_Ch1Offset = 0;
            static int16_t MMT_Ch2Offset = -4;
            static int16_t MMT_Ch3Offset = 7;

            static int16_t MMT_SensitivityMAT_S11 = 1059;
            static int16_t MMT_SensitivityMAT_S22 = 1055;
            static int16_t MMT_SensitivityMAT_S33 = 1003;

            static int16_t MMT_SensitivityMAT_S12 = -2;
            static int16_t MMT_SensitivityMAT_S13 = -3;
            static int16_t MMT_SensitivityMAT_S21 = -2;

            static int16_t MMT_SensitivityMAT_S23 = 8;
            static int16_t MMT_SensitivityMAT_S31 = -3;
            static int16_t MMT_SensitivityMAT_S32 = 8;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ADCS_Mag0MMTCalibConfigCmd_Payload_t *pl =
                &command5->adcsmag0mmtcalibconfigcmd.Payload;

            ImGui::Separator();
            ImGui::Text("ADCS Mag0 MMT Calib Config Payload");

            ImGui::InputScalar("MMT_Ch1Offset", ImGuiDataType_S16, &MMT_Ch1Offset);
            ImGui::InputScalar("MMT_Ch2Offset", ImGuiDataType_S16, &MMT_Ch2Offset);
            ImGui::InputScalar("MMT_Ch3Offset", ImGuiDataType_S16, &MMT_Ch3Offset);

            ImGui::InputScalar("MMT_SensitivityMAT_S11", ImGuiDataType_S16, &MMT_SensitivityMAT_S11);
            ImGui::InputScalar("MMT_SensitivityMAT_S22", ImGuiDataType_S16, &MMT_SensitivityMAT_S22);
            ImGui::InputScalar("MMT_SensitivityMAT_S33", ImGuiDataType_S16, &MMT_SensitivityMAT_S33);

            ImGui::InputScalar("MMT_SensitivityMAT_S12", ImGuiDataType_S16, &MMT_SensitivityMAT_S12);
            ImGui::InputScalar("MMT_SensitivityMAT_S13", ImGuiDataType_S16, &MMT_SensitivityMAT_S13);
            ImGui::InputScalar("MMT_SensitivityMAT_S21", ImGuiDataType_S16, &MMT_SensitivityMAT_S21);

            ImGui::InputScalar("MMT_SensitivityMAT_S23", ImGuiDataType_S16, &MMT_SensitivityMAT_S23);
            ImGui::InputScalar("MMT_SensitivityMAT_S31", ImGuiDataType_S16, &MMT_SensitivityMAT_S31);
            ImGui::InputScalar("MMT_SensitivityMAT_S32", ImGuiDataType_S16, &MMT_SensitivityMAT_S32);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                pl->MMT_Ch1Offset = MMT_Ch1Offset;
                pl->MMT_Ch2Offset = MMT_Ch2Offset;
                pl->MMT_Ch3Offset = MMT_Ch3Offset;

                pl->MMT_SensitivityMAT_S11 = MMT_SensitivityMAT_S11;
                pl->MMT_SensitivityMAT_S22 = MMT_SensitivityMAT_S22;
                pl->MMT_SensitivityMAT_S33 = MMT_SensitivityMAT_S33;

                pl->MMT_SensitivityMAT_S12 = MMT_SensitivityMAT_S12;
                pl->MMT_SensitivityMAT_S13 = MMT_SensitivityMAT_S13;
                pl->MMT_SensitivityMAT_S21 = MMT_SensitivityMAT_S21;

                pl->MMT_SensitivityMAT_S23 = MMT_SensitivityMAT_S23;
                pl->MMT_SensitivityMAT_S31 = MMT_SensitivityMAT_S31;
                pl->MMT_SensitivityMAT_S32 = MMT_SensitivityMAT_S32;

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_Mag0MMTCalibConfigCmd_t) - 7)
                };

                memcpy(command5->adcsmag0mmtcalibconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsmag0mmtcalibconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsmag0mmtcalibconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsmag0mmtcalibconfigcmd.CmdHeader + 6, &fnccode, 1);

                command5->adcsmag0mmtcalibconfigcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_Mag0MMTCalibConfigCmd_t);
                const uint8_t *p = (const uint8_t *)&command5->adcsmag0mmtcalibconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);
                command5->adcsmag0mmtcalibconfigcmd.CmdHeader[7] = crc;

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_Mag0MMTCalibConfigCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_Mag0MMTCalibConfigCmd_t);
                memcpy(pkt->Data, &command5->adcsmag0mmtcalibconfigcmd,
                    sizeof(ADCS_Mag0MMTCalibConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[0],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[1],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[2],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[3],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[4],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[5],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[6],
                        command5->adcsmag0mmtcalibconfigcmd.CmdHeader[7]);

            ImGui::Text("Payload (UI values):");
            ImGui::Text("Offsets: Ch1=%d, Ch2=%d, Ch3=%d",
                        (int)MMT_Ch1Offset, (int)MMT_Ch2Offset, (int)MMT_Ch3Offset);
            ImGui::Text("S11=%d S22=%d S33=%d",
                        (int)MMT_SensitivityMAT_S11, (int)MMT_SensitivityMAT_S22, (int)MMT_SensitivityMAT_S33);
            ImGui::Text("S12=%d S13=%d S21=%d S23=%d S31=%d S32=%d",
                        (int)MMT_SensitivityMAT_S12, (int)MMT_SensitivityMAT_S13, (int)MMT_SensitivityMAT_S21,
                        (int)MMT_SensitivityMAT_S23, (int)MMT_SensitivityMAT_S31, (int)MMT_SensitivityMAT_S32);
            break;
        }






        case 2730: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_DEFAULT_MODE_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_DefaultModeConfigCmd_Payload_t &p =
                    command5->adcsdefaultmodeconfigcmd.Payload;

                p.DefaultRunMode                   = 1;
                p.DefaultOperationalState          = 0;
                p.DefaultControlModeInOpStateSafe  = 1;
                p.DefaultControlModeInOpStateAuto  = 13;


                initialized = true;
            }
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ADCS_DefaultModeConfigCmd_Payload_t &p =
                command5->adcsdefaultmodeconfigcmd.Payload;

            ImGui::Text("=== Default Mode Settings ===");
            ImGui::Text("DefaultRunMode                  : %u", p.DefaultRunMode);
            ImGui::Text("DefaultOperationalState         : %u", p.DefaultOperationalState);
            ImGui::Text("DefaultControlModeInOpStateSafe : %u", p.DefaultControlModeInOpStateSafe);
            ImGui::Text("DefaultControlModeInOpStateAuto : %u", p.DefaultControlModeInOpStateAuto);

            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                uint16_t mid    = htons(msgid);
                uint8_t  seq[2] = { 0xC0, 0x00 };
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_DefaultModeConfigCmd_t) - 7)
                };

                memcpy(command5->adcsdefaultmodeconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsdefaultmodeconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsdefaultmodeconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsdefaultmodeconfigcmd.CmdHeader + 6, &fnccode, 1);


                            command5->adcsdefaultmodeconfigcmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(ADCS_DefaultModeConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsdefaultmodeconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(pp++);
                memcpy(command5->adcsdefaultmodeconfigcmd.CmdHeader + 7, &crc, 1);


                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_DefaultModeConfigCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_DefaultModeConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsdefaultmodeconfigcmd,
                    sizeof(ADCS_DefaultModeConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }

        case 2731: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_MOUNTING_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_MountingConfigCmd_Payload_t &p =
                    command5->adcsmountingconfigcmd.Payload;


                // ==== Stack & Actuators ====
                p.StackX_mounting = 6;
                p.StackY_mounting = 3;
                p.StackZ_mounting = 1;

                p.MTQ0_mounting = 1;
                p.MTQ1_mounting = 3;
                p.MTQ2_mounting = 5;

                p.Wheel0_mounting = 11;
                p.Wheel1_mounting = 12;
                p.Wheel2_mounting = 13;
                p.Wheel3_mounting = 14;

                // ==== Pyramid RWL angles (raw = deg * 100) ====
                p.PyramidRWL_alpha = 0;
                p.PyramidRWL_beta  = -9000;
                p.PyramidRWL_gamma = 0;

                // ==== CSS Mountings ====
                p.CSS0_mounting = 1;
                p.CSS1_mounting = 2;
                p.CSS2_mounting = 3;
                p.CSS3_mounting = 4;
                p.CSS4_mounting = 5;
                p.CSS5_mounting = 6;
                p.CSS6_mounting = 0;
                p.CSS7_mounting = 0;
                p.CSS8_mounting = 0;
                p.CSS9_mounting = 0;

                // ==== FSS0..3 ====
                p.FSS0_alpha = 9000; p.FSS0_beta = 0; p.FSS0_gamma = 18000;
                p.FSS1_alpha = p.FSS1_beta = p.FSS1_gamma = 0;
                p.FSS2_alpha = p.FSS2_beta = p.FSS2_gamma = 0;
                p.FSS3_alpha = p.FSS3_beta = p.FSS3_gamma = 0;

                // ==== HSS0..1 ====
                p.HSS0_alpha = 0; p.HSS0_beta = -7500; p.HSS0_gamma = 0;
                p.HSS1_alpha = p.HSS1_beta = p.HSS1_gamma = 0;

                // ==== MAG0..1 ====
                p.MAG0_alpha = 0; p.MAG0_beta = 0; p.MAG0_gamma = 18000;
                p.MAG1_alpha = 0; p.MAG1_beta = 0; p.MAG1_gamma = 0;

                // ==== STR0..1 ====
                p.STR0_alpha = p.STR0_beta = p.STR0_gamma = 0;
                p.STR1_alpha = p.STR1_beta = p.STR1_gamma = 0;

                // ==== Ext Sensor 0/1 ====
                p.ExtSensor0_alpha = p.ExtSensor0_beta = p.ExtSensor0_gamma = 0;
                p.ExtSensor1_alpha = p.ExtSensor1_beta = p.ExtSensor1_gamma = 0;

                // ==== External Gyro axes ====
                p.ExtGyro0_axis1_mounting = 0;
                p.ExtGyro0_axis2_mounting = 0;
                p.ExtGyro0_axis3_mounting = 0;

                p.ExtGyro1_axis1_mounting = 0;
                p.ExtGyro1_axis2_mounting = 0;
                p.ExtGyro1_axis3_mounting = 0;

                // ============================================================
                initialized = true;
            }
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ADCS_MountingConfigCmd_Payload_t &p =
                command5->adcsmountingconfigcmd.Payload;

            ImGui::Text("=== Stack & Actuators ===");
            ImGui::Text("StackX: %u  StackY: %u  StackZ: %u", p.StackX_mounting, p.StackY_mounting, p.StackZ_mounting);
            ImGui::Text("MTQ0: %u  MTQ1: %u  MTQ2: %u", p.MTQ0_mounting, p.MTQ1_mounting, p.MTQ2_mounting);
            ImGui::Text("Wheels: %u %u %u %u", p.Wheel0_mounting, p.Wheel1_mounting, p.Wheel2_mounting, p.Wheel3_mounting);

            ImGui::Separator();
            ImGui::Text("=== Pyramid RWL Angles (deg = raw/100) ===");
            ImGui::Text("alpha: %d  beta: %d  gamma: %d", p.PyramidRWL_alpha, p.PyramidRWL_beta, p.PyramidRWL_gamma);

            ImGui::Separator();
            ImGui::Text("=== CSS Mountings ===");
            ImGui::Text("CSS0..9: %u %u %u %u %u %u %u %u %u %u",
                        p.CSS0_mounting, p.CSS1_mounting, p.CSS2_mounting, p.CSS3_mounting, p.CSS4_mounting,
                        p.CSS5_mounting, p.CSS6_mounting, p.CSS7_mounting, p.CSS8_mounting, p.CSS9_mounting);

            ImGui::Separator();
            ImGui::Text("=== FSS0..3 Angles ===");
            ImGui::Text("FSS0: %d %d %d", p.FSS0_alpha, p.FSS0_beta, p.FSS0_gamma);
            ImGui::Text("FSS1: %d %d %d", p.FSS1_alpha, p.FSS1_beta, p.FSS1_gamma);
            ImGui::Text("FSS2: %d %d %d", p.FSS2_alpha, p.FSS2_beta, p.FSS2_gamma);
            ImGui::Text("FSS3: %d %d %d", p.FSS3_alpha, p.FSS3_beta, p.FSS3_gamma);

            ImGui::Separator();
            ImGui::Text("=== HSS0..1 ===");
            ImGui::Text("HSS0: %d %d %d", p.HSS0_alpha, p.HSS0_beta, p.HSS0_gamma);
            ImGui::Text("HSS1: %d %d %d", p.HSS1_alpha, p.HSS1_beta, p.HSS1_gamma);

            ImGui::Separator();
            ImGui::Text("=== MAG0..1 ===");
            ImGui::Text("MAG0: %d %d %d", p.MAG0_alpha, p.MAG0_beta, p.MAG0_gamma);
            ImGui::Text("MAG1: %d %d %d", p.MAG1_alpha, p.MAG1_beta, p.MAG1_gamma);

            ImGui::Separator();
            ImGui::Text("=== STR0..1 ===");
            ImGui::Text("STR0: %d %d %d", p.STR0_alpha, p.STR0_beta, p.STR0_gamma);
            ImGui::Text("STR1: %d %d %d", p.STR1_alpha, p.STR1_beta, p.STR1_gamma);

            ImGui::Separator();
            ImGui::Text("=== External Sensor 0/1 ===");
            ImGui::Text("Ext0: %d %d %d", p.ExtSensor0_alpha, p.ExtSensor0_beta, p.ExtSensor0_gamma);
            ImGui::Text("Ext1: %d %d %d", p.ExtSensor1_alpha, p.ExtSensor1_beta, p.ExtSensor1_gamma);

            ImGui::Separator();
            ImGui::Text("=== External Gyro Mountings ===");
            ImGui::Text("Gyro0: %u %u %u", p.ExtGyro0_axis1_mounting, p.ExtGyro0_axis2_mounting, p.ExtGyro0_axis3_mounting);
            ImGui::Text("Gyro1: %u %u %u", p.ExtGyro1_axis1_mounting, p.ExtGyro1_axis2_mounting, p.ExtGyro1_axis3_mounting);

            ImGui::Separator();


            // ======================== Generate CMD ===========================
            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                uint8_t len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_MountingConfigCmd_t) - 7)
                };

                memcpy(command5->adcsmountingconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsmountingconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsmountingconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsmountingconfigcmd.CmdHeader + 6, &fnccode, 1);

                command5->adcsmountingconfigcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_MountingConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsmountingconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(pp++);
                memcpy(command5->adcsmountingconfigcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_MountingConfigCmd_t));

                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_MountingConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsmountingconfigcmd,
                    sizeof(ADCS_MountingConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }


        case 2732: { // adcs SET_MAG1_MMT_CALIB_CONFIG
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 32;
        struct __attribute__((packed)) Payload {
            int16_t MMT_Ch1Offset;
            int16_t MMT_Ch2Offset;
            int16_t MMT_Ch3Offset;
            int16_t MMT_SensitivityMAT_S11;
            int16_t MMT_SensitivityMAT_S22;
            int16_t MMT_SensitivityMAT_S33;
            int16_t MMT_SensitivityMAT_S12;
            int16_t MMT_SensitivityMAT_S13;
            int16_t MMT_SensitivityMAT_S21;
            int16_t MMT_SensitivityMAT_S23;
            int16_t MMT_SensitivityMAT_S31;
            int16_t MMT_SensitivityMAT_S32;
        };
        static_assert(sizeof(Payload) == 24, "adcs SET_MAG1_MMT_CALIB_CONFIG payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2732", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2732", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MMT_Ch1Offset##direct_2732", ImGuiDataType_S16, &payload.MMT_Ch1Offset);
            ImGui::InputScalar("MMT_Ch2Offset##direct_2732", ImGuiDataType_S16, &payload.MMT_Ch2Offset);
            ImGui::InputScalar("MMT_Ch3Offset##direct_2732", ImGuiDataType_S16, &payload.MMT_Ch3Offset);
            ImGui::InputScalar("MMT_SensitivityMAT_S11##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S11);
            ImGui::InputScalar("MMT_SensitivityMAT_S22##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S22);
            ImGui::InputScalar("MMT_SensitivityMAT_S33##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S33);
            ImGui::InputScalar("MMT_SensitivityMAT_S12##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S12);
            ImGui::InputScalar("MMT_SensitivityMAT_S13##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S13);
            ImGui::InputScalar("MMT_SensitivityMAT_S21##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S21);
            ImGui::InputScalar("MMT_SensitivityMAT_S23##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S23);
            ImGui::InputScalar("MMT_SensitivityMAT_S31##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S31);
            ImGui::InputScalar("MMT_SensitivityMAT_S32##direct_2732", ImGuiDataType_S16, &payload.MMT_SensitivityMAT_S32);
            if (ImGui::Button("Generate CMD##direct_2732")) {
                WriteSystemName(msgid);
                uint8_t cmd[32] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2733: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_ESTIMATOR_CONFIG_CC;
            ADCS_EstimatorConfigCmd_Payload_t &p = command5->adcsestimatorconfigcmd.Payload;

//             static bool initialized = false;
//             if (!initialized) {
//                 p.DefaultMainEstimatorMode   = 0;
//                 p.DefaultBackupEstimatorMode = 0;

//                 p.MAGMeasurementNoise  =0.001;
//                 p.CSSMeasurementNoise  =10;
//                 p.FSSMeasurementNoise = 0.0001;
//                 p.HSSMeasurementNoise = 0.0001;
//                 p.STRMeasurementNoise  = 1E-8;

//                 p.MMTRKFSystemNoise    = 0.0002;
//                 p.EKFSystemNoise       = 1E-9;
//                 p.NutationEpsilonCorrection = 4.243392E-05;
//                 p.NutationPsiCorrection     = 1.607126E-05;

//                 // // ---- Bit-fields ----
//                 // p.UseFSSinEKF = 1;
//                 // p.UseCSSinEKF = 0;
//                 // p.UseHSSinEKF = 1;
//                 // p.UseSTRinEKF = 0;

//                 // p.TriadVector1 = 1;   // 4-bit
//                 // p.TriadVector2 = 0;   // 4-bit
//                 // p.Spare        = 0;   // unused padding
// //revise
//                 // ---- Bit-fields ----
//                 p.UseFSSinEKF = 0;
//                 p.UseCSSinEKF = 0;
//                 p.UseHSSinEKF = 0;
//                 p.UseSTRinEKF = 0;

//                 p.TriadVector1 = 1;   // 4-bit
//                 p.TriadVector2 = 0;   // 4-bit


//                 // ============================================================

//                 initialized = true;
//             }
            uint8_t useFSS = p.UseFSSinEKF;
            uint8_t useCSS = p.UseCSSinEKF;
            uint8_t useHSS = p.UseHSSinEKF;
            uint8_t useSTR = p.UseSTRinEKF;
            uint8_t Triad1 = p.TriadVector1;
            uint8_t Triad2 = p.TriadVector2;
            // GUI 출력 (read-only)
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("DefaultMainEstimatorMode", ImGuiDataType_U8, &p.DefaultMainEstimatorMode);
            ImGui::InputScalar("DefaultBackupEstimatorMode", ImGuiDataType_U8, &p.DefaultBackupEstimatorMode);
            ImGui::InputScalar("MAGMeasurementNoise", ImGuiDataType_Float, &p.MAGMeasurementNoise, NULL, NULL, "%.3f");
            ImGui::InputScalar("CSSMeasurementNoise", ImGuiDataType_Float, &p.CSSMeasurementNoise);
            ImGui::InputScalar("FSSMeasurementNoise", ImGuiDataType_Float, &p.FSSMeasurementNoise, NULL, NULL, "%.4f");
            ImGui::InputScalar("HSSMeasurementNoise", ImGuiDataType_Float, &p.HSSMeasurementNoise, NULL, NULL, "%.4f");
            ImGui::InputScalar("STRMeasurementNoise", ImGuiDataType_Float, &p.STRMeasurementNoise, NULL, NULL, "%.1g", ImGuiInputTextFlags_CharsScientific);
            ImGui::InputScalar("MMTRKFSystemNoise", ImGuiDataType_Float, &p.MMTRKFSystemNoise, NULL, NULL, "%.4f");
            ImGui::InputScalar("EKFSystemNoise", ImGuiDataType_Float, &p.EKFSystemNoise, NULL, NULL, "%.1g", ImGuiInputTextFlags_CharsScientific);
            ImGui::InputScalar("NutationEpsilonCorrection", ImGuiDataType_Float, &p.NutationEpsilonCorrection, NULL, NULL, "%.9g", ImGuiInputTextFlags_CharsScientific);
            ImGui::InputScalar("NutationPsiCorrection", ImGuiDataType_Float, &p.NutationPsiCorrection, NULL, NULL, "%.9g", ImGuiInputTextFlags_CharsScientific);
            ImGui::InputScalar("UseFSSinEKF", ImGuiDataType_U8, &useFSS);
            ImGui::InputScalar("UseCSSinEKF", ImGuiDataType_U8, &useCSS);
            ImGui::InputScalar("UseHSSinEKF", ImGuiDataType_U8, &useHSS);
            ImGui::InputScalar("UseSTRinEKF", ImGuiDataType_U8, &useSTR);
            ImGui::InputScalar("TriadVector1", ImGuiDataType_U8, &Triad1);
            ImGui::InputScalar("TriadVector2", ImGuiDataType_U8, &Triad2);
            p.UseFSSinEKF = useFSS;
            p.UseCSSinEKF = useCSS;
            p.UseHSSinEKF = useHSS;
            p.UseSTRinEKF = useSTR;
            p.TriadVector1 = Triad1;
            p.TriadVector2 = Triad2;
            ImGui::Separator();

            
            // ImGui::Text("=== Default Estimator Modes ===");
            // ImGui::Text("Main   : %u", p.DefaultMainEstimatorMode);
            // ImGui::Text("Backup : %u", p.DefaultBackupEstimatorMode);

            // ImGui::Separator();
            // ImGui::Text("=== Measurement Noise ===");
            // ImGui::Text("MAG : %.6f", p.MAGMeasurementNoise);
            // ImGui::Text("CSS : %.6f", p.CSSMeasurementNoise);
            // ImGui::Text("FSS : %.6f", p.FSSMeasurementNoise);
            // ImGui::Text("HSS : %.6f", p.HSSMeasurementNoise);
            // ImGui::Text("STR : %.6f", p.STRMeasurementNoise);

            // ImGui::Separator();
            // ImGui::Text("=== System Noise ===");
            // ImGui::Text("MMT-RKF : %.6f", p.MMTRKFSystemNoise);
            // ImGui::Text("EKF     : %.6f", p.EKFSystemNoise);

            // ImGui::Separator();
            // ImGui::Text("=== Nutation Correction ===");
            // ImGui::Text("Epsilon : %.6f", p.NutationEpsilonCorrection);
            // ImGui::Text("Psi     : %.6f", p.NutationPsiCorrection);

            // ImGui::Separator();
            // ImGui::Text("=== Flags ===");
            // ImGui::Text("UseFSSinEKF : %u", p.UseFSSinEKF);
            // ImGui::Text("UseCSSinEKF : %u", p.UseCSSinEKF);
            // ImGui::Text("UseHSSinEKF : %u", p.UseHSSinEKF);
            // ImGui::Text("UseSTRinEKF : %u", p.UseSTRinEKF);

            // ImGui::Separator();
            // ImGui::Text("TriadVector1 : %u", p.TriadVector1);
            // ImGui::Text("TriadVector2 : %u", p.TriadVector2);
            // // ImGui::Text("Spare        : %u", p.Spare);

            ImGui::Separator();


            // ============================ Generate CMD ============================
            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                uint8_t len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_EstimatorConfigCmd_t) - 7)
                };

                memcpy(command5->adcsestimatorconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsestimatorconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsestimatorconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsestimatorconfigcmd.CmdHeader + 6, &fnccode, 1);


                command5->adcsestimatorconfigcmd.CmdHeader[7] = 0x00;
                uint16_t total = sizeof(ADCS_EstimatorConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsestimatorconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(pp++);

                memcpy(command5->adcsestimatorconfigcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_EstimatorConfigCmd_t));

                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_EstimatorConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsestimatorconfigcmd,
                    sizeof(ADCS_EstimatorConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

case 2734: {
    static uint16_t msgid   = ADCS_CMD_ID;
    static uint8_t  fnccode = ADCS_SET_SAT_ORBIT_PARAMS_CONFIG_CC;


    ADCS_SatOrbitParamConfigCmd_Payload_t &p =
        command5->adcssatorbitparamconfigcmd.Payload;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
    ImGui::Separator();

    ImGui::Text("=== Sat Orbit Params (double) ===");
    ImGui::InputDouble("Epoch",        &p.Epoch);
    ImGui::InputDouble("Inclination",  &p.Inclination);
    ImGui::InputDouble("RAAN",         &p.RAAN);
    ImGui::InputDouble("Eccentricity", &p.Eccentricity);
    ImGui::InputDouble("AOP",          &p.AOP);
    ImGui::InputDouble("MeanAnomaly",  &p.MeanAnomaly);
    ImGui::InputDouble("MeanMotion",   &p.MeanMotion);
    ImGui::InputDouble("B_StarDrag",   &p.B_StarDrag);

    ImGui::Separator();

    if (ImGui::Button("Generate CMD")) {
        WriteSystemName(msgid);

        uint16_t mid_be = htons(msgid);
        uint16_t ccsds_len_be = htons((uint16_t)(sizeof(ADCS_SatOrbitParamConfigCmd_t) - 7));

        memcpy(command5->adcssatorbitparamconfigcmd.CmdHeader + 0, &mid_be, 2);
        command5->adcssatorbitparamconfigcmd.CmdHeader[2] = 0xC0;
        command5->adcssatorbitparamconfigcmd.CmdHeader[3] = 0x00;
        memcpy(command5->adcssatorbitparamconfigcmd.CmdHeader + 4, &ccsds_len_be, 2);
        memcpy(command5->adcssatorbitparamconfigcmd.CmdHeader + 6, &fnccode, 1);
        command5->adcssatorbitparamconfigcmd.CmdHeader[7] = 0x00;

        uint16_t total = (uint16_t)sizeof(ADCS_SatOrbitParamConfigCmd_t);
        const uint8_t *pp = (const uint8_t *)&command5->adcssatorbitparamconfigcmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(pp++);
        command5->adcssatorbitparamconfigcmd.CmdHeader[7] = crc;

        packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_SatOrbitParamConfigCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(ADCS_SatOrbitParamConfigCmd_t);

        memcpy(pkt->Data, &command5->adcssatorbitparamconfigcmd, sizeof(ADCS_SatOrbitParamConfigCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }

    break;
}




        case 2735: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_NODE_SELECTION_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_NodeSelectionConfigCmd_Payload_t &p =
                    command5->adcsnodeselectionconfigcmd.Payload;

                p.RSWSelectionFlags        = 15;
                p.MAGSelectionFlags        = 1;
                p.FSSSelectionFlags        = 1;
                p.HSSSelectionFlags        = 1;
                p.GYRSelectionFlags        = 12;
                p.STRSelectionFlags        = 0;
                p.GNSSSelectionFlags       = 1;
                p.ExtSensorSelectionFlags  = 0;

                initialized = true;
            }

            ADCS_NodeSelectionConfigCmd_Payload_t &p =
                command5->adcsnodeselectionconfigcmd.Payload;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ImGui::Text("=== Node Selection Flags ===");
            ImGui::Text("RSWSelectionFlags       : %d", p.RSWSelectionFlags);
            ImGui::Text("MAGSelectionFlags       : %d", p.MAGSelectionFlags);
            ImGui::Text("FSSSelectionFlags       : %d", p.FSSSelectionFlags);
            ImGui::Text("HSSSelectionFlags       : %d", p.HSSSelectionFlags);
            ImGui::Text("GYRSelectionFlags       : %d", p.GYRSelectionFlags);
            ImGui::Text("STRSelectionFlags       : %d", p.STRSelectionFlags);
            ImGui::Text("GNSSSelectionFlags      : %d", p.GNSSSelectionFlags);
            ImGui::Text("ExtSensorSelectionFlags : %d", p.ExtSensorSelectionFlags);

            ImGui::Separator();


            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);


                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                uint8_t len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_NodeSelectionConfigCmd_t) - 7)
                };

                memcpy(command5->adcsnodeselectionconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsnodeselectionconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsnodeselectionconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsnodeselectionconfigcmd.CmdHeader + 6, &fnccode, 1);
        command5->adcsnodeselectionconfigcmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(ADCS_NodeSelectionConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsnodeselectionconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) {
                    crc ^= *(pp++);
                }
                memcpy(command5->adcsnodeselectionconfigcmd.CmdHeader + 7, &crc, 1);


                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_NodeSelectionConfigCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_NodeSelectionConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsnodeselectionconfigcmd,
                    sizeof(ADCS_NodeSelectionConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }



        case 2736: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_MTQ_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_MTQConfigCmd_Payload_t &p =
                    command5->adcsmtqconfigcmd.Payload;


                p.MTQ0MaxDipoleMoment = 0.4;
                p.MTQ1MaxDipoleMoment = 0.4;
                p.MTQ2MaxDipoleMoment = 0.4;

                p.MaxMTQOnTime  = 800;
                p.MinMTQOnTime  = 10;

                p.MagneticControlFilterFactor = 0;
                // =====================================================

                initialized = true;
            }

            ADCS_MTQConfigCmd_Payload_t &p =
                command5->adcsmtqconfigcmd.Payload;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ImGui::Text("=== MTQ Maximum Dipole Moments ===");
            ImGui::Text("MTQ0 Max Dipole : %.4f", p.MTQ0MaxDipoleMoment);
            ImGui::Text("MTQ1 Max Dipole : %.4f", p.MTQ1MaxDipoleMoment);
            ImGui::Text("MTQ2 Max Dipole : %.4f", p.MTQ2MaxDipoleMoment);

            ImGui::Separator();
            ImGui::Text("=== MTQ On-Time Settings ===");
            ImGui::Text("Max MTQ On Time : %u ms", p.MaxMTQOnTime);
            ImGui::Text("Min MTQ On Time : %u ms", p.MinMTQOnTime);

            ImGui::Separator();
            ImGui::Text("=== Magnetic Control Filter ===");
            ImGui::Text("Filter factor : %.4f", p.MagneticControlFilterFactor);

            ImGui::Separator();


            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                // CCSDS Header
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                uint8_t len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_MTQConfigCmd_t) - 7)
                };

                memcpy(command5->adcsmtqconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsmtqconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsmtqconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsmtqconfigcmd.CmdHeader + 6, &fnccode, 1);
        command5->adcsmtqconfigcmd.CmdHeader[7] = 0x00;
                uint16_t total = sizeof(ADCS_MTQConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsmtqconfigcmd;
                uint8_t crc = 0xFF;
                while (total--) {
                    crc ^= *(pp++);
                }
                memcpy(command5->adcsmtqconfigcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_MTQConfigCmd_t));

                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_MTQConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsmtqconfigcmd,
                    sizeof(ADCS_MTQConfigCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }

            break;
        }

        case 2737: { // adcs SET_ESTIMATION_MODE
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 37;
        struct __attribute__((packed)) Payload {
            uint8_t MainEstimatorMode;
            uint8_t BackupEstimatorMode;
        };
        static_assert(sizeof(Payload) == 2, "adcs SET_ESTIMATION_MODE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2737", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2737", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("MainEstimatorMode##direct_2737", ImGuiDataType_U8, &payload.MainEstimatorMode);
            ImGui::InputScalar("BackupEstimatorMode##direct_2737", ImGuiDataType_U8, &payload.BackupEstimatorMode);
            if (ImGui::Button("Generate CMD##direct_2737")) {
                WriteSystemName(msgid);
                uint8_t cmd[10] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2738: { // adcs SET_OPERATIONAL_STATE
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 38;
        struct __attribute__((packed)) Payload {
            uint8_t OperationalState;
        };
        static_assert(sizeof(Payload) == 1, "adcs SET_OPERATIONAL_STATE payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2738", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2738", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("OperationalState##direct_2738", ImGuiDataType_U8, &payload.OperationalState);
            if (ImGui::Button("Generate CMD##direct_2738")) {
                WriteSystemName(msgid);
                uint8_t cmd[9] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2739: {

            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_MAG_SENSING_ELM_CONFIG_CC;


            static bool initialized = false;
            if (!initialized) {

                ADCS_MagSensingElmConfigCmd_Payload_t &p =
                    command5->adcsmagsensingelmconfigcmd.Payload;

                p.Mag0SensingElement = 0;
                p.Mag1SensingElement = 0;
                p.Spare = 0;


                initialized = true;
            }

            // Payload alias
            ADCS_MagSensingElmConfigCmd_Payload_t &p =
                command5->adcsmagsensingelmconfigcmd.Payload;
            ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);
            ImGui::Separator();

            ImGui::Text("=== Magnetic Sensing Element Enable Flags ===");
            ImGui::Text("MAG0 Sensing Element : %u", p.Mag0SensingElement);
            ImGui::Text("MAG1 Sensing Element : %u", p.Mag1SensingElement);
            ImGui::Text("Spare Bits           : %u", p.Spare);
            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                // CCSDS Header (7바이트 + CRC)
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                uint8_t len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_MagSensingElmConfigCmd_t) - 7)
                };

                memcpy(command5->adcsmagsensingelmconfigcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsmagsensingelmconfigcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsmagsensingelmconfigcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsmagsensingelmconfigcmd.CmdHeader + 6, &fnccode, 1);
        command5->adcsmagsensingelmconfigcmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(ADCS_MagSensingElmConfigCmd_t);
                const uint8_t *pp =
                    (const uint8_t *)&command5->adcsmagsensingelmconfigcmd;

                uint8_t crc = 0xFF;
                while (total--) crc ^= *(pp++);
                memcpy(command5->adcsmagsensingelmconfigcmd.CmdHeader + 7, &crc, 1);


                packetsign *pkt = (packetsign *)malloc(
                    2 + 2 + 4 + sizeof(ADCS_MagSensingElmConfigCmd_t));

                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_MagSensingElmConfigCmd_t);

                memcpy(pkt->Data,
                    &command5->adcsmagsensingelmconfigcmd,
                    sizeof(ADCS_MagSensingElmConfigCmd_t));

                // uplink thread 실행
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt
                );
            }

            break;
        }


        case 2740: {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SET_UNSOLICIT_TLM_MSG_SETUP_CC;

            static uint8_t uart_interval   = 0;
            static uint8_t uart2_interval  = 0;
            static uint8_t can_interval    = 0;
            static uint8_t reserved_field  = 0;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::Separator();
            ImGui::Text("Return Interval (0~15)");
            ImGui::InputScalar("UART Return Interval",  ImGuiDataType_U8, &uart_interval);
            ImGui::InputScalar("UART2 Return Interval", ImGuiDataType_U8, &uart2_interval);
            ImGui::InputScalar("CAN Return Interval",   ImGuiDataType_U8, &can_interval);
            ImGui::InputScalar("Reserved (4bit)",       ImGuiDataType_U8, &reserved_field);

            ADCS_UnsolicitTlmMsgSetupCmd_Payload_t &pl =
                command5->adcsunsolicittlmmsgsetupcmd.Payload;

            ImGui::Separator();
            ImGui::Text("UART TLM ED Inclusion Bitmask");
            for (int i = 0; i < 5; i++) {
                ImGui::InputScalar(
                    (std::string("UART Bitmask[") + std::to_string(i) + "]").c_str(),
                    ImGuiDataType_U8,
                    &pl.UARTTlmEDInclusionBitmask[i]
                );
            }

            ImGui::Separator();
            ImGui::Text("UART2 TLM ED Inclusion Bitmask");
            for (int i = 0; i < 5; i++) {
                ImGui::InputScalar(
                    (std::string("UART2 Bitmask[") + std::to_string(i) + "]").c_str(),
                    ImGuiDataType_U8,
                    &pl.UART2TlmEDInclusionBitmask[i]
                );
            }

            ImGui::Separator();
            ImGui::Text("CAN TLM ED Inclusion Bitmask");
            for (int i = 0; i < 5; i++) {
                ImGui::InputScalar(
                    (std::string("CAN Bitmask[") + std::to_string(i) + "]").c_str(),
                    ImGuiDataType_U8,
                    &pl.CANTlmEDInclusionBitmask[i]
                );
            }

            if (ImGui::Button("Generate CMD")) {
                pl.UARTTlmReturnInterval  = uart_interval  & 0x0F;
                pl.UART2TlmReturnInterval = uart2_interval & 0x0F;
                pl.CANTlmRetrunInterval   = can_interval   & 0x0F;
                pl.Reserved               = reserved_field & 0x0F;

                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    static_cast<uint8_t>(sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t) - 7)
                };

                memcpy(command5->adcsunsolicittlmmsgsetupcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsunsolicittlmmsgsetupcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsunsolicittlmmsgsetupcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsunsolicittlmmsgsetupcmd.CmdHeader + 6, &fnccode, 1);
        command5->adcsunsolicittlmmsgsetupcmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t);
                const uint8_t *p =
                    (const uint8_t *)&command5->adcsunsolicittlmmsgsetupcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command5->adcsunsolicittlmmsgsetupcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 +
                                        sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t);
                memcpy(pkt->Data, &command5->adcsunsolicittlmmsgsetupcmd,
                    sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }




        case 2741: { // adcs SET_UNSOLICIT_EVENT_MSG_SETUP
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 41;
        struct __attribute__((packed)) Payload {
            uint8_t Flag;
        };
        static_assert(sizeof(Payload) == 1, "adcs SET_UNSOLICIT_EVENT_MSG_SETUP payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2741", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2741", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("Flag##direct_2741", ImGuiDataType_U8, &payload.Flag);
            if (ImGui::Button("Generate CMD##direct_2741")) {
                WriteSystemName(msgid);
                uint8_t cmd[9] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2742: { // adcs SET_INITIATE_EVENT_LOG_TRANSFER
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 42;
        struct __attribute__((packed)) Payload {
            uint8_t FilterType;
            uint32_t UnixStartTime;
            uint32_t UnixEndTime;
            uint32_t NumberOfEntry;
            uint32_t WriteCounter;
            uint8_t Flags[3];
        };
        static_assert(sizeof(Payload) == 20, "adcs SET_INITIATE_EVENT_LOG_TRANSFER payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2742", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2742", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("FilterType##direct_2742", ImGuiDataType_U8, &payload.FilterType);
            ImGui::InputScalar("UnixStartTime##direct_2742", ImGuiDataType_U32, &payload.UnixStartTime);
            ImGui::InputScalar("UnixEndTime##direct_2742", ImGuiDataType_U32, &payload.UnixEndTime);
            ImGui::InputScalar("NumberOfEntry##direct_2742", ImGuiDataType_U32, &payload.NumberOfEntry);
            ImGui::InputScalar("WriteCounter##direct_2742", ImGuiDataType_U32, &payload.WriteCounter);
            static bool IncludeCriticalEVS = false;
            ImGui::Checkbox("IncludeCriticalEVS##direct_2742", &IncludeCriticalEVS);
            static bool IncludeMajorWarningEVS = false;
            ImGui::Checkbox("IncludeMajorWarningEVS##direct_2742", &IncludeMajorWarningEVS);
            static bool IncludeMinorWarningEVS = false;
            ImGui::Checkbox("IncludeMinorWarningEVS##direct_2742", &IncludeMinorWarningEVS);
            static bool IncludeInfoEVS = false;
            ImGui::Checkbox("IncludeInfoEVS##direct_2742", &IncludeInfoEVS);
            static bool IncludeCubeCom = false;
            ImGui::Checkbox("IncludeCubeCom##direct_2742", &IncludeCubeCom);
            static bool IncludeRWL0 = false;
            ImGui::Checkbox("IncludeRWL0##direct_2742", &IncludeRWL0);
            static bool IncludeRWL1 = false;
            ImGui::Checkbox("IncludeRWL1##direct_2742", &IncludeRWL1);
            static bool IncludeRWL2 = false;
            ImGui::Checkbox("IncludeRWL2##direct_2742", &IncludeRWL2);
            static bool IncludeRWL3 = false;
            ImGui::Checkbox("IncludeRWL3##direct_2742", &IncludeRWL3);
            static bool IncludeFSS0 = false;
            ImGui::Checkbox("IncludeFSS0##direct_2742", &IncludeFSS0);
            static bool IncludeFSS1 = false;
            ImGui::Checkbox("IncludeFSS1##direct_2742", &IncludeFSS1);
            static bool IncludeFSS2 = false;
            ImGui::Checkbox("IncludeFSS2##direct_2742", &IncludeFSS2);
            static bool IncludeFSS3 = false;
            ImGui::Checkbox("IncludeFSS3##direct_2742", &IncludeFSS3);
            static bool IncludeHSS0 = false;
            ImGui::Checkbox("IncludeHSS0##direct_2742", &IncludeHSS0);
            static bool IncludeHSS1 = false;
            ImGui::Checkbox("IncludeHSS1##direct_2742", &IncludeHSS1);
            static bool IncludeSTR0 = false;
            ImGui::Checkbox("IncludeSTR0##direct_2742", &IncludeSTR0);
            static bool IncludeSTR1 = false;
            ImGui::Checkbox("IncludeSTR1##direct_2742", &IncludeSTR1);
            static bool IncludeMAG0 = false;
            ImGui::Checkbox("IncludeMAG0##direct_2742", &IncludeMAG0);
            static bool IncludeMAG1 = false;
            ImGui::Checkbox("IncludeMAG1##direct_2742", &IncludeMAG1);
            static bool IncludeExt0 = false;
            ImGui::Checkbox("IncludeExt0##direct_2742", &IncludeExt0);
            static bool IncludeExt1 = false;
            ImGui::Checkbox("IncludeExt1##direct_2742", &IncludeExt1);
            if (ImGui::Button("Generate CMD##direct_2742")) {
                payload.Flags[0] = 0;
                payload.Flags[1] = 0;
                payload.Flags[2] = 0;
                if (IncludeCriticalEVS) payload.Flags[0] |= (uint8_t)(1u << 0);
                if (IncludeMajorWarningEVS) payload.Flags[0] |= (uint8_t)(1u << 1);
                if (IncludeMinorWarningEVS) payload.Flags[0] |= (uint8_t)(1u << 2);
                if (IncludeInfoEVS) payload.Flags[0] |= (uint8_t)(1u << 3);
                if (IncludeCubeCom) payload.Flags[0] |= (uint8_t)(1u << 4);
                if (IncludeRWL0) payload.Flags[0] |= (uint8_t)(1u << 5);
                if (IncludeRWL1) payload.Flags[0] |= (uint8_t)(1u << 6);
                if (IncludeRWL2) payload.Flags[0] |= (uint8_t)(1u << 7);
                if (IncludeRWL3) payload.Flags[1] |= (uint8_t)(1u << 0);
                if (IncludeFSS0) payload.Flags[1] |= (uint8_t)(1u << 1);
                if (IncludeFSS1) payload.Flags[1] |= (uint8_t)(1u << 2);
                if (IncludeFSS2) payload.Flags[1] |= (uint8_t)(1u << 3);
                if (IncludeFSS3) payload.Flags[1] |= (uint8_t)(1u << 4);
                if (IncludeHSS0) payload.Flags[1] |= (uint8_t)(1u << 5);
                if (IncludeHSS1) payload.Flags[1] |= (uint8_t)(1u << 6);
                if (IncludeSTR0) payload.Flags[1] |= (uint8_t)(1u << 7);
                if (IncludeSTR1) payload.Flags[2] |= (uint8_t)(1u << 0);
                if (IncludeMAG0) payload.Flags[2] |= (uint8_t)(1u << 1);
                if (IncludeMAG1) payload.Flags[2] |= (uint8_t)(1u << 2);
                if (IncludeExt0) payload.Flags[2] |= (uint8_t)(1u << 3);
                if (IncludeExt1) payload.Flags[2] |= (uint8_t)(1u << 4);
                WriteSystemName(msgid);
                uint8_t cmd[28] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2743: { // adcs SET_COMMANDED_GNSS_MEASUREMENTS
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 43;
        struct __attribute__((packed)) Payload {
            uint32_t GNSSUnixTimeSeconds;
            uint32_t GNSSUnixTimeNanoseconds;
            int32_t PositionX;
            int32_t PositionY;
            int32_t PositionZ;
            int32_t VelocityX;
            int32_t VelocityY;
            int32_t VelocityZ;
            uint8_t SyncTime;
        };
        static_assert(sizeof(Payload) == 33, "adcs SET_COMMANDED_GNSS_MEASUREMENTS payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2743", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2743", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("GNSSUnixTimeSeconds##direct_2743", ImGuiDataType_U32, &payload.GNSSUnixTimeSeconds);
            ImGui::InputScalar("GNSSUnixTimeNanoseconds##direct_2743", ImGuiDataType_U32, &payload.GNSSUnixTimeNanoseconds);
            ImGui::InputScalar("PositionX##direct_2743", ImGuiDataType_S32, &payload.PositionX);
            ImGui::InputScalar("PositionY##direct_2743", ImGuiDataType_S32, &payload.PositionY);
            ImGui::InputScalar("PositionZ##direct_2743", ImGuiDataType_S32, &payload.PositionZ);
            ImGui::InputScalar("VelocityX##direct_2743", ImGuiDataType_S32, &payload.VelocityX);
            ImGui::InputScalar("VelocityY##direct_2743", ImGuiDataType_S32, &payload.VelocityY);
            ImGui::InputScalar("VelocityZ##direct_2743", ImGuiDataType_S32, &payload.VelocityZ);
            ImGui::InputScalar("SyncTime##direct_2743", ImGuiDataType_U8, &payload.SyncTime);
            if (ImGui::Button("Generate CMD##direct_2743")) {
                WriteSystemName(msgid);
                uint8_t cmd[41] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2744: { // adcs SET_OPENLOOPCMD_RWL
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 44;
        struct __attribute__((packed)) Payload {
            float RWL0_OpenLoopSpeedCmd;
            float RWL1_OpenLoopSpeedCmd;
            float RWL2_OpenLoopSpeedCmd;
            float RWL3_OpenLoopSpeedCmd;
        };
        static_assert(sizeof(Payload) == 16, "adcs SET_OPENLOOPCMD_RWL payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2744", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2744", ImGuiDataType_U8, &fnccode);
            ImGui::InputFloat("RWL0_OpenLoopSpeedCmd##direct_2744", &payload.RWL0_OpenLoopSpeedCmd);
            ImGui::InputFloat("RWL1_OpenLoopSpeedCmd##direct_2744", &payload.RWL1_OpenLoopSpeedCmd);
            ImGui::InputFloat("RWL2_OpenLoopSpeedCmd##direct_2744", &payload.RWL2_OpenLoopSpeedCmd);
            ImGui::InputFloat("RWL3_OpenLoopSpeedCmd##direct_2744", &payload.RWL3_OpenLoopSpeedCmd);
            if (ImGui::Button("Generate CMD##direct_2744")) {
                WriteSystemName(msgid);
                uint8_t cmd[24] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2745: { // adcs SET_OPENLOOP_CMD_HXYZ_RW
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 45;
        struct __attribute__((packed)) Payload {
            float cmdHx;
            float cmdHy;
            float cmdHz;
        };
        static_assert(sizeof(Payload) == 12, "adcs SET_OPENLOOP_CMD_HXYZ_RW payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2745", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2745", ImGuiDataType_U8, &fnccode);
            ImGui::InputFloat("cmdHx##direct_2745", &payload.cmdHx);
            ImGui::InputFloat("cmdHy##direct_2745", &payload.cmdHy);
            ImGui::InputFloat("cmdHz##direct_2745", &payload.cmdHz);
            if (ImGui::Button("Generate CMD##direct_2745")) {
                WriteSystemName(msgid);
                uint8_t cmd[20] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2751: { // ADCS GET_ERROR_LOG_SETTING
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_ERROR_LOG_SETTING_CC;
            ImGui::InputScalar("msgid##direct_2751", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2751", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2751")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2752: {
            ADCS_GetCurrentUnixTimeCmd_t &cmd = command5->adcsgetcurrentunixtimecmd;
            static uint16_t msgid = (uint16_t)(ADCS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(ADCS_GET_CURRENT_UNIX_TIME_CC);
            ImGui::InputScalar("msgid##direct_2752", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2752", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2752")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(ADCS_GetCurrentUnixTimeCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(ADCS_GetCurrentUnixTimeCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_GetCurrentUnixTimeCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(ADCS_GetCurrentUnixTimeCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(ADCS_GetCurrentUnixTimeCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 2753: { // ADCS GET_PERSIST_CONFIG_DIAGNOSTIC
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_PERSIST_CONFIG_DIAGNOSTIC_CC;
            ImGui::InputScalar("msgid##direct_2753", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2753", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2753")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2754: { // ADCS GET_COMMUNICATION_STATUS
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_COMMUNICATION_STATUS_CC;
            ImGui::InputScalar("msgid##direct_2754", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2754", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2754")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2755: { // ADCS GET_CONTROL_ESTIMATION_MODE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_CONTROL_ESTIMATION_MODE_CC;
            ImGui::InputScalar("msgid##direct_2755", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2755", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2755")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2756: { // ADCS GET_REFERENCE_IRC_VECTOR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_REFERENCE_IRC_VECTOR_CC;
            ImGui::InputScalar("msgid##direct_2756", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2756", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2756")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2757: { // ADCS GET_REFERENCE_LLH_TARGET
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_REFERENCE_LLH_TARGET_CC;
            ImGui::InputScalar("msgid##direct_2757", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2757", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2757")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2758: { // ADCS GET_ORBIT_MODE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_ORBIT_MODE_CC;
            ImGui::InputScalar("msgid##direct_2758", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2758", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2758")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2759: { // ADCS GET_HEALTH_TLM_MMT
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_HEALTH_TLM_MMT_CC;
            ImGui::InputScalar("msgid##direct_2759", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2759", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2759")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2760: {
            ADCS_GetRawCubeSenseSunCmd_t &cmd = command5->adcsgetrawcubesensesuncmd;
            static uint16_t msgid = (uint16_t)(ADCS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(ADCS_GET_RAW_CALIBRATED_CUBESENSE_SUN_CC);
            ImGui::InputScalar("msgid##direct_2760", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2760", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2760")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(ADCS_GetRawCubeSenseSunCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(ADCS_GetRawCubeSenseSunCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_GetRawCubeSenseSunCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(ADCS_GetRawCubeSenseSunCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(ADCS_GetRawCubeSenseSunCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }
        case 2761: { // ADCS GET_REFERENCE_RPY_VALUES
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_REFERENCE_RPY_VALUES_CC;
            ImGui::InputScalar("msgid##direct_2761", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2761", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2761")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2762: { // ADCS GET_OPENLOOPCMD_MTQ
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_OPENLOOPCMD_MTQ_CC;
            ImGui::InputScalar("msgid##direct_2762", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2762", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2762")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2763: { // ADCS GET_POWER_STATE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_POWER_STATE_CC;
            ImGui::InputScalar("msgid##direct_2763", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2763", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2763")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2764: { // ADCS GET_RUN_MODE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_RUN_MODE_CC;
            ImGui::InputScalar("msgid##direct_2764", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2764", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2764")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2765: { // ADCS GET_CONTROL_MODE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_CONTROL_MODE_CC;
            ImGui::InputScalar("msgid##direct_2765", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2765", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2765")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2766: { // ADCS GET_WHL_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_WHL_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2766", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2766", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2766")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2767: { // ADCS GET_SATELLITE_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_SATELLITE_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2767", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2767", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2767")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2768: { // ADCS GET_CONTROLLER_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_CONTROLLER_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2768", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2768", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2768")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2769: { // ADCS GET_MAG0_MMT_CALIB_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_MAG0_MMT_CALIB_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2769", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2769", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2769")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2770: { // ADCS GET_DEFAULT_MODE_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_DEFAULT_MODE_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2770", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2770", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2770")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2771: { // ADCS GET_MOUNTING_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_MOUNTING_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2771", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2771", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2771")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2772: { // ADCS GET_MAG1_MMT_CALIB_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_MAG1_MMT_CALIB_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2772", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2772", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2772")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2773: { // ADCS GET_ESTIMATOR_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_ESTIMATOR_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2773", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2773", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2773")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2774: { // ADCS GET_SAT_ORBIT_PARAM_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_SAT_ORBIT_PARAM_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2774", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2774", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2774")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2775: { // ADCS GET_NODE_SELECTION_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_NODE_SELECTION_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2775", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2775", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2775")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2776: { // ADCS GET_MTQ_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_MTQ_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2776", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2776", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2776")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2777: { // ADCS GET_ESTIMATION_MODE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_ESTIMATION_MODE_CC;
            ImGui::InputScalar("msgid##direct_2777", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2777", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2777")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2778: { // ADCS GET_OPERATIONAL_STATE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_OPERATIONAL_STATE_CC;
            ImGui::InputScalar("msgid##direct_2778", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2778", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2778")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2779: { // ADCS GET_RAW_CALIBRATED_CSS_SENSOR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_RAW_CALIBRATED_CSS_SENSOR_CC;
            ImGui::InputScalar("msgid##direct_2779", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2779", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2779")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2780: {
            ADCS_GetRawGYRSensorCmd_t &cmd = command5->adcsgetrawgyrsensorcmd;
            static uint16_t msgid = (uint16_t)(ADCS_CMD_ID);
            static uint8_t fnccode = (uint8_t)(ADCS_GET_RAW_CALIBRATED_GYR_SENSOR_CC);
            ImGui::InputScalar("msgid##direct_2780", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2780", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2780")) {
                WriteSystemName(msgid);
                uint16_t mid = htons(msgid);
                uint8_t seq[2] = {0xC0, 0x00};
                const uint16_t wire_length = (uint16_t)(sizeof(ADCS_GetRawGYRSensorCmd_t) - 7);
                uint8_t len[2] = {(uint8_t)(wire_length >> 8), (uint8_t)(wire_length & 0xFF)};
                memcpy(cmd.CmdHeader, &mid, 2);
                memcpy(cmd.CmdHeader + 2, seq, 2);
                memcpy(cmd.CmdHeader + 4, len, 2);
                memcpy(cmd.CmdHeader + 6, &fnccode, 1);
                cmd.CmdHeader[7] = 0;
                uint16_t total = (uint16_t)sizeof(ADCS_GetRawGYRSensorCmd_t);
                const uint8_t *bytes = (const uint8_t *)&cmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(bytes++);
                cmd.CmdHeader[7] = crc;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_GetRawGYRSensorCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(ADCS_GetRawGYRSensorCmd_t);
                memcpy(pkt->Data, &cmd, sizeof(ADCS_GetRawGYRSensorCmd_t));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2781: { // ADCS GET_CALIBRATED_GYR_SENSOR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_CALIBRATED_GYR_SENSOR_CC;
            ImGui::InputScalar("msgid##direct_2781", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2781", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2781")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2782: { // ADCS GET_MAG_SENSING_ELM_CONFIG
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_MAG_SENSING_ELM_CONFIG_CC;
            ImGui::InputScalar("msgid##direct_2782", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2782", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2782")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2783: { // ADCS GET_TLM_LOG_INCLMASK
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_TLM_LOG_INCLMASK_CC;
            ImGui::InputScalar("msgid##direct_2783", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2783", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2783")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2784: { // ADCS GET_UNSOLICIT_TLM_MSG_SETUP
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_UNSOLICIT_TLM_MSG_SETUP_CC;
            ImGui::InputScalar("msgid##direct_2784", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2784", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2784")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2785: { // ADCS GET_UNSOLICIT_EVENT_MSG_SETUP
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_UNSOLICIT_EVENT_MSG_SETUP_CC;
            ImGui::InputScalar("msgid##direct_2785", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2785", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2785")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2786: { // ADCS GET_EVENT_LOG_STATUS_RESPONSE
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_EVENT_LOG_STATUS_RESPONSE_CC;
            ImGui::InputScalar("msgid##direct_2786", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2786", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2786")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2787: { // ADCS GET_PORTMAP
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_PORTMAP_CC;
            ImGui::InputScalar("msgid##direct_2787", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2787", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2787")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2790: { // ADCS GET_RAW_CALIBRATED_RWL_SENSOR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_RAW_CALIBRATED_RWL_SENSOR_CC;
            ImGui::InputScalar("msgid##direct_2790", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2790", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2790")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

    case 2791: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_INTERFACE_TRANSPORT_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);
        ImGui::InputScalar("TransportType", ImGuiDataType_U8,
                           &command5->adcsinterfacetransportcmd.Payload.TransportType);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            uint16_t mid  = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};

            uint8_t  len[2] = {0x00, sizeof(ADCS_InterfaceTransportCmd_t) - 7};

            memcpy(command5->adcsinterfacetransportcmd.CmdHeader,     &mid, 2);
            memcpy(command5->adcsinterfacetransportcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->adcsinterfacetransportcmd.CmdHeader + 4, len,  2);
            memcpy(command5->adcsinterfacetransportcmd.CmdHeader + 6, &fnccode, 1);

            command5->adcsinterfacetransportcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(ADCS_InterfaceTransportCmd_t);
            const uint8_t *p =
                (const uint8_t *)&command5->adcsinterfacetransportcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);

            memcpy(command5->adcsinterfacetransportcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 +
                                     sizeof(ADCS_InterfaceTransportCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(ADCS_InterfaceTransportCmd_t);
            memcpy(pkt->Data, &command5->adcsinterfacetransportcmd,
                   sizeof(ADCS_InterfaceTransportCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    /* 52 : ADCS GPIO Enable High (CC 5, no-arg) */
        case 2800: { // ADCS SEQ_DTUMB
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_SEQ_DTUMB_CC;
            ImGui::InputScalar("msgid##direct_2800", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2800", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2800")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2801: // ADCS_SEQ_GNDPT_CC
        {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = ADCS_SEQ_GNDPT_CC;
            static uint8_t  flag_estmode = 0;
            static uint16_t target_duration = 0;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("Flag_EST Mode", ImGuiDataType_U8, &flag_estmode);
            ImGui::InputScalar("Target Duration", ImGuiDataType_U16, &target_duration);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                const ADCS_SequenceCmdGNDpointing_Payload_t payload = {
                    flag_estmode,
                    target_duration
                };
                memcpy(&command5->adcsseqgndpointingcmd.Payload, &payload,
                       sizeof(payload));

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(ADCS_SequenceCmdGNDpointingCmd_t) - 7)
                };

                memcpy(command5->adcsseqgndpointingcmd.CmdHeader,     &mid, 2);
                memcpy(command5->adcsseqgndpointingcmd.CmdHeader + 2, seq,  2);
                memcpy(command5->adcsseqgndpointingcmd.CmdHeader + 4, len,  2);
                memcpy(command5->adcsseqgndpointingcmd.CmdHeader + 6, &fnccode, 1);

                command5->adcsseqgndpointingcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(ADCS_SequenceCmdGNDpointingCmd_t);
                const uint8_t *p = (const uint8_t *)&command5->adcsseqgndpointingcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command5->adcsseqgndpointingcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_SequenceCmdGNDpointingCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(ADCS_SequenceCmdGNDpointingCmd_t);
                memcpy(pkt->Data, &command5->adcsseqgndpointingcmd,
                    sizeof(ADCS_SequenceCmdGNDpointingCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2802: { // ADCS SEQ_SUN
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_SEQ_SUN_CC;
            ImGui::InputScalar("msgid##direct_2802", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2802", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2802")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2803: { // ADCS SEQ_TGT
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_SEQ_TGT_CC;
            struct __attribute__((packed)) Payload {
                uint8_t flag_estmode;
                uint16_t target_duration;
            };
            static_assert(sizeof(Payload) == 3, "ADCS SEQ_TGT payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2803", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2803", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_estmode##direct_2803", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("target_duration##direct_2803", ImGuiDataType_U16, &payload.target_duration);
            if (ImGui::Button("Generate CMD##direct_2803")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(Payload)] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                memcpy(cmd + CFE_SB_CMD_HDR_SIZE, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2804: { // ADCS SEQ_NADIR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_SEQ_NADIR_CC;
            struct __attribute__((packed)) Payload {
                uint8_t flag_estmode;
                uint16_t target_duration;
            };
            static_assert(sizeof(Payload) == 3, "ADCS SEQ_NADIR payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2804", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2804", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_estmode##direct_2804", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("target_duration##direct_2804", ImGuiDataType_U16, &payload.target_duration);
            if (ImGui::Button("Generate CMD##direct_2804")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE + sizeof(Payload)] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                memcpy(cmd + CFE_SB_CMD_HDR_SIZE, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2805: { // adcs SEQ_RPYPT
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 105;
        struct __attribute__((packed)) Payload {
            float Roll;
            float Pitch;
            float Yaw;
        };
        static_assert(sizeof(Payload) == 12, "adcs SEQ_RPYPT payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2805", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2805", ImGuiDataType_U8, &fnccode);
            ImGui::InputFloat("Roll##direct_2805", &payload.Roll);
            ImGui::InputFloat("Pitch##direct_2805", &payload.Pitch);
            ImGui::InputFloat("Yaw##direct_2805", &payload.Yaw);
            if (ImGui::Button("Generate CMD##direct_2805")) {
                WriteSystemName(msgid);
                uint8_t cmd[20] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2810: { // ADCS SET_ERROR_LOG_CLEAR
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_SET_ERROR_LOG_CLEAR_CC;
            ImGui::InputScalar("msgid##direct_2810", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2810", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2810")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2814: { // ADCS GET_CURRENT_UNIX_TIME_INTERNAL
            static uint16_t msgid = ADCS_CMD_ID;
            static uint8_t fnccode = ADCS_GET_CURRENT_UNIX_TIME_INTERNAL_CC;
            ImGui::InputScalar("msgid##direct_2814", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2814", ImGuiDataType_U8, &fnccode);
            if (ImGui::Button("Generate CMD##direct_2814")) {
                WriteSystemName(msgid);
                uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }



        case 2818: { // adcs COMM_01
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 118;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_01 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2818", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2818", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2818", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2818", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2818", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2818")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2819: { // adcs COMM_02
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 119;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_02 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2819", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2819", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2819", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2819", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2819", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2819")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2820: { // adcs COMM_03
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 120;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_03 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2820", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2820", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2820", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2820", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2820", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2820")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2821: { // adcs COMM_04
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 121;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_04 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2821", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2821", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2821", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2821", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2821", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2821")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2822: { // adcs COMM_05
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 122;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_05 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2822", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2822", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2822", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2822", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2822", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2822")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2823: { // adcs COMM_06
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 123;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_06 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2823", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2823", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2823", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2823", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2823", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2823")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2824: { // adcs COMM_07
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 124;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_07 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2824", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2824", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2824", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2824", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2824", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2824")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2825: { // adcs COMM_08
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 125;
        struct __attribute__((packed)) Payload {
            uint8_t flag_tlmtype;
            uint8_t flag_estmode;
            uint8_t flag_contmode;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_08 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2825", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2825", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_tlmtype##direct_2825", ImGuiDataType_U8, &payload.flag_tlmtype);
            ImGui::InputScalar("flag_estmode##direct_2825", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("flag_contmode##direct_2825", ImGuiDataType_U8, &payload.flag_contmode);
            if (ImGui::Button("Generate CMD##direct_2825")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2826: { // adcs COMM_10
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 126;
        struct __attribute__((packed)) Payload {
            uint8_t flag_estmode;
            uint16_t target_duration;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_10 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2826", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2826", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_estmode##direct_2826", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("target_duration##direct_2826", ImGuiDataType_U16, &payload.target_duration);
            if (ImGui::Button("Generate CMD##direct_2826")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 2827: { // adcs COMM_11
            static uint16_t msgid = 0x1865;
            static uint8_t fnccode = 127;
        struct __attribute__((packed)) Payload {
            uint8_t flag_estmode;
            uint16_t target_duration;
        };
        static_assert(sizeof(Payload) == 3, "adcs COMM_11 payload size");
        static Payload payload{};
            ImGui::InputScalar("msgid##direct_2827", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2827", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("flag_estmode##direct_2827", ImGuiDataType_U8, &payload.flag_estmode);
            ImGui::InputScalar("target_duration##direct_2827", ImGuiDataType_U16, &payload.target_duration);
            if (ImGui::Button("Generate CMD##direct_2827")) {
                WriteSystemName(msgid);
                uint8_t cmd[11] = {0};
                uint16_t mid = htons(msgid);
                memcpy(cmd, &mid, sizeof(mid));
                cmd[2] = 0xC0;
                cmd[3] = 0x00;
                const uint16_t wire_length = (uint16_t)(sizeof(cmd) - 7);
                cmd[4] = (uint8_t)(wire_length >> 8);
                cmd[5] = (uint8_t)(wire_length & 0xFF);
                cmd[6] = fnccode;
            memcpy(cmd + 8, &payload, sizeof(payload));
                cmd[7] = 0;
                uint8_t checksum = 0xFF;
                for (size_t i = 0; i < sizeof(cmd); ++i) checksum ^= cmd[i];
                cmd[7] = checksum;
                packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(cmd));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length = (uint16_t)sizeof(cmd);
                memcpy(pkt->Data, cmd, sizeof(cmd));
                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // ==================== eps ====================
    default:
        return false;
    }
    return true;
}
