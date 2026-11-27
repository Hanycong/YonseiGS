// View - 명령 생성기 입력 폼: BEE1012 utrx (명령 번호 2500, 2501, 2502, 2503, 2506, 2507, 2508, 2509, 2510, 2511, 2512, 2513, 2514, 2515, 2516, 2520, 2521, 2522, 2523, 2524, 2525, 2526, 2527, 2528, 2529, 2530, 2531, 2532, 2533)
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
#include "telecommand_sender.h"
#include "ccsds_cmd.h"
#include "sat_mode.h"
#include "cmdgen5_common.h"
#include "cmdgen_forms.h"
#include "noarg_form.h"

using namespace std;

extern StateCheckUnit State;
extern Console console;
extern Command * command;
extern Setup * setup;
extern uint16_t gen_fnccode;
extern bool raw_data;

// 인자 없는 명령 (머리 8바이트뿐): { 명령 번호, 기본 MID, 기본 CC, 위젯 ID 꼬리표 }. 폼은 noarg_form 이 그린다
static mim::ui::NoArgCommandForm kNoArgForms[] = {
    {  2500, UELYSYS_UTRX_CMD_ID, UELYSYS_UTRX_NOOP_CC,              "##direct_2500" },
    {  2501, UTRX_CMD_ID,         UTRX_RESET_COUNTERS_CC,            "##direct_2501" },   // UTRX RESET_COUNTERS
    {  2502, UTRX_CMD_ID,         UTRX_RESET_APP_CMD_COUNTERS_CC,    "##direct_2502" },   // UTRX RESET_APP_CMD_COUNTERS
    {  2503, UTRX_CMD_ID,         UTRX_RESET_DEVICE_CMD_COUNTERS_CC, "##direct_2503" },   // UTRX RESET_DEVICE_CMD_COUNTERS
    {  2507, UTRX_CMD_ID,         UTRX_REBOOT_CC,                    "##direct_2507" },   // UTRX REBOOT
    {  2510, UTRX_CMD_ID,         UTRX_SET_DEFAULT_BAUD_CC,          "##direct_2510" },   // UTRX SET_DEFAULT_BAUD
    {  2511, UTRX_CMD_ID,         UTRX_RPARAM_SAVE_1_CC,             "##direct_2511" },   // UTRX RPARAM_SAVE_1
    {  2512, UTRX_CMD_ID,         UTRX_RPARAM_SAVE_5_CC,             "##direct_2512" },   // UTRX RPARAM_SAVE_5
    {  2513, UTRX_CMD_ID,         UTRX_RPARAM_SAVE_ALL_CC,           "##direct_2513" },   // UTRX RPARAM_SAVE_ALL
    {  2516, UTRX_CMD_ID,         UTRX_CHECK_STATE_PING_CC,          "##direct_2516" },   // UTRX CHECK_STATE_PING
    {  2520, UELYSYS_UTRX_CMD_ID, UELYSYS_UTRX_RXCONF_GET_BAUD_CC,   "##direct_2520" },
    {  2521, UTRX_CMD_ID,         UTRX_RXCONF_GET_GUARD_CC,          "##direct_2521" },   // UTRX RXCONF_GET_GUARD
    {  2522, UTRX_CMD_ID,         UTRX_TXCONF_GET_BAUD_CC,           "##direct_2522" },   // UTRX TXCONF_GET_BAUD
    {  2523, UTRX_CMD_ID,         UTRX_TLM_GET_TEMP_BRD_CC,          "##direct_2523" },   // UTRX TLM_GET_TEMP_BRD
    {  2524, UTRX_CMD_ID,         UTRX_TLM_GET_LAST_RSSI_CC,         "##direct_2524" },   // UTRX TLM_GET_LAST_RSSI
    {  2525, UTRX_CMD_ID,         UTRX_TLM_GET_LAST_RFERR_CC,        "##direct_2525" },   // UTRX TLM_GET_LAST_RFERR
    {  2526, UTRX_CMD_ID,         UTRX_TLM_GET_ACTIVE_CONF_CC,       "##direct_2526" },   // UTRX TLM_GET_ACTIVE_CONF
    {  2527, UTRX_CMD_ID,         UTRX_TLM_GET_BOOT_COUNT_CC,        "##direct_2527" },   // UTRX TLM_GET_BOOT_COUNT
    {  2528, UTRX_CMD_ID,         UTRX_TLM_GET_BOOT_CAUSE_CC,        "##direct_2528" },   // UTRX TLM_GET_BOOT_CAUSE
    {  2529, UTRX_CMD_ID,         UTRX_TLM_GET_LAST_CONTACT_CC,      "##direct_2529" },   // UTRX TLM_GET_LAST_CONTACT
    {  2530, UTRX_CMD_ID,         UTRX_TLM_GET_TOT_TX_BYTES_CC,      "##direct_2530" },   // UTRX TLM_GET_TOT_TX_BYTES
    {  2531, UTRX_CMD_ID,         UTRX_TLM_GET_TOT_RX_BYTES_CC,      "##direct_2531" },   // UTRX TLM_GET_TOT_RX_BYTES
    {  2532, UTRX_CMD_ID,         UTRX_RXCONF_GET_FREQ_CC,           "##direct_2532" },   // UTRX RXCONF_GET_FREQ
    {  2533, UTRX_CMD_ID,         UTRX_TXCONF_GET_FREQ_CC,           "##direct_2533" },   // UTRX TXCONF_GET_FREQ
};

bool DrawBee1012UtrxForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
case 2506: {
    static uint16_t msgid   = 0x1850;
    static uint8_t  fnccode = 6;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    if (ImGui::Button("Generate CMD"))
    {
        WriteSystemName(msgid);
        mim::svc::WaitTx();


        uint8_t cmd[CFE_SB_CMD_HDR_SIZE] = {0};


        uint16_t mid_be = htons(msgid);
        memcpy(cmd, &mid_be, sizeof(uint16_t));

        cmd[2] = 0xC0;
        cmd[3] = 0x00;


        uint16_t ccsds_len = htons(sizeof(cmd) - 7);
        memcpy(cmd + 4, &ccsds_len, sizeof(uint16_t));

        // CC
        cmd[6] = fnccode;   // 0


        cmd[7] = 0x00;


        uint8_t checksum = 0xFF;
        for (size_t i = 0; i < sizeof(cmd); ++i)
            checksum ^= cmd[i];
        cmd[7] = checksum;


        mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
    }

    break;
}


        case 2508: {
            UELYSYS_U32ArgCmd_t &cmd = command5->uelysys_u32argcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UTRX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UTRX_RXCONF_SET_BAUD_CC);
            ImGui::InputScalar("msgid##direct_2508", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2508", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("baud", ImGuiDataType_U32, &cmd.arg);
            if (ImGui::Button("Generate CMD##direct_2508")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2509: {
            UELYSYS_U32ArgCmd_t &cmd = command5->uelysys_u32argcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UTRX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UTRX_TXCONF_SET_BAUD_CC);
            ImGui::InputScalar("msgid##direct_2509", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2509", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("baud", ImGuiDataType_U32, &cmd.arg);
            if (ImGui::Button("Generate CMD##direct_2509")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }

        case 2514: {
            UELYSYS_U32ArgCmd_t &cmd = command5->uelysys_u32argcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UTRX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UTRX_RXCONF_SET_FREQ_CC);
            ImGui::InputScalar("msgid##direct_2514", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2514", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("frequency", ImGuiDataType_U32, &cmd.arg);
            if (ImGui::Button("Generate CMD##direct_2514")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }
        case 2515: {
            UELYSYS_U32ArgCmd_t &cmd = command5->uelysys_u32argcmd;
            static uint16_t msgid = (uint16_t)(UELYSYS_UTRX_CMD_ID);
            static uint8_t fnccode = (uint8_t)(UELYSYS_UTRX_TXCONF_SET_FREQ_CC);
            ImGui::InputScalar("msgid##direct_2515", ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode##direct_2515", ImGuiDataType_U8, &fnccode);
            ImGui::InputScalar("frequency", ImGuiDataType_U32, &cmd.arg);
            if (ImGui::Button("Generate CMD##direct_2515")) {
                WriteSystemName(msgid);
                mim::tc::EncodeCfeCommand16(&cmd, sizeof(cmd), msgid, fnccode);
                mim::tc::SendTelecommand(&cmd, (sizeof(cmd)), MIM_PT_TMTC_TEST);
            }
            break;
        }


        // ==================== stx ====================
    default:
        return false;
    }
    return true;
}
