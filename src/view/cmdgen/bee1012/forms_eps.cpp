// View - 명령 생성기 입력 폼: BEE1012 eps (명령 번호 2900, 2901, 2910, 2911, 2912, 2920, 2921, 2930, 2931, 2940, 2941, 2942, 2950, 2951, 2952, 2953, 2954, 2970, 2980, 2981, 2982, 2983, 2984, 2985)
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

bool DrawBee1012EpsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 2900: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_NOOP_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_NoopCmd_t) - 7};

            memcpy(command5->epsnoopcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsnoopcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsnoopcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsnoopcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsnoopcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_NoopCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsnoopcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsnoopcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_NoopCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_NoopCmd_t);
            memcpy(pkt->Data, &command5->epsnoopcmd, sizeof(EPS_NoopCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2901: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RESET_COUNTERS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_ResetCountersCmd_t) - 7};

            memcpy(command5->epsresetcounterscmd.CmdHeader, &mid, 2);
            memcpy(command5->epsresetcounterscmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsresetcounterscmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsresetcounterscmd.CmdHeader + 6, &fnccode, 1);

            command5->epsresetcounterscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_ResetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsresetcounterscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsresetcounterscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_ResetCountersCmd_t);
            memcpy(pkt->Data, &command5->epsresetcounterscmd, sizeof(EPS_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2910: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P80_POWER_IF_GET_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsp80powerifgetcmd.Payload.csp_node);
        ImGui::InputText("name", command5->epsp80powerifgetcmd.Payload.name, EPS_P80_POWER_IF_NAME_LEN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P80_Power_If_Get_Cmd_t) - 7};

            memcpy(command5->epsp80powerifgetcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsp80powerifgetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsp80powerifgetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsp80powerifgetcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsp80powerifgetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P80_Power_If_Get_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsp80powerifgetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsp80powerifgetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P80_Power_If_Get_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P80_Power_If_Get_Cmd_t);
            memcpy(pkt->Data, &command5->epsp80powerifgetcmd, sizeof(EPS_P80_Power_If_Get_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2911: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P80_POWER_IF_SET_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsp80powerifsetcmd.Payload.csp_node);
        ImGui::InputScalar("mode", ImGuiDataType_U8, &command5->epsp80powerifsetcmd.Payload.mode);
        ImGui::InputScalar("on count", ImGuiDataType_U16, &command5->epsp80powerifsetcmd.Payload.on_cnt);
        ImGui::InputScalar("off count", ImGuiDataType_U16, &command5->epsp80powerifsetcmd.Payload.off_cnt);
        ImGui::InputText("name", command5->epsp80powerifsetcmd.Payload.name, EPS_P80_POWER_IF_NAME_LEN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P80_Power_If_Set_Cmd_t) - 7};

            memcpy(command5->epsp80powerifsetcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsp80powerifsetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsp80powerifsetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsp80powerifsetcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsp80powerifsetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P80_Power_If_Set_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsp80powerifsetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsp80powerifsetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P80_Power_If_Set_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P80_Power_If_Set_Cmd_t);
            memcpy(pkt->Data, &command5->epsp80powerifsetcmd, sizeof(EPS_P80_Power_If_Set_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2912: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P80_POWER_IF_LIST_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsp80poweriflistcmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P80_Power_If_List_Cmd_t) - 7};

            memcpy(command5->epsp80poweriflistcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsp80poweriflistcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsp80poweriflistcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsp80poweriflistcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsp80poweriflistcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P80_Power_If_List_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsp80poweriflistcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsp80poweriflistcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P80_Power_If_List_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P80_Power_If_List_Cmd_t);
            memcpy(pkt->Data, &command5->epsp80poweriflistcmd, sizeof(EPS_P80_Power_If_List_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2920: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_GET_HK_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsgethkcmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_Get_HK_Cmd_t) - 7};

            memcpy(command5->epsgethkcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsgethkcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsgethkcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsgethkcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsgethkcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_Get_HK_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsgethkcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsgethkcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_Get_HK_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_Get_HK_Cmd_t);
            memcpy(pkt->Data, &command5->epsgethkcmd, sizeof(EPS_Get_HK_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2921: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_GET_HK_ALL_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_Get_HK_All_Cmd_t) - 7};

            memcpy(command5->epsgethkallcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsgethkallcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsgethkallcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsgethkallcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsgethkallcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_Get_HK_All_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsgethkallcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsgethkallcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_Get_HK_All_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_Get_HK_All_Cmd_t);
            memcpy(pkt->Data, &command5->epsgethkallcmd,
                   sizeof(EPS_Get_HK_All_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2930: {
        static uint16_t msgid = 0x1875;  // 실제 msgid로 변경 필요
        static uint8_t fnccode = EPS_P80_GND_WDT_CLEAR_CC;  // 실제 function code로 변경 필요

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsp80gndwatchdogclearcmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_t) - 7};

            memcpy(command5->epsp80gndwatchdogclearcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsp80gndwatchdogclearcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsp80gndwatchdogclearcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsp80gndwatchdogclearcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsp80gndwatchdogclearcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsp80gndwatchdogclearcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsp80gndwatchdogclearcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_t);
            memcpy(pkt->Data, &command5->epsp80gndwatchdogclearcmd, sizeof(EPS_P80_Gnd_Watchdog_Clear_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2931: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_P80_GND_WDT_CLEAR_ALL_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t) - 7};

            memcpy(command5->epsp80gndwatchdogclearallcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsp80gndwatchdogclearallcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsp80gndwatchdogclearallcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsp80gndwatchdogclearallcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsp80gndwatchdogclearallcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsp80gndwatchdogclearallcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsp80gndwatchdogclearallcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt =
            (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t);
            memcpy(pkt->Data, &command5->epsp80gndwatchdogclearallcmd,
                   sizeof(EPS_P80_Gnd_Watchdog_Clear_All_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2940: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_GET_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamgetcmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamgetcmd.Payload.table_id);
        ImGui::InputScalar("addr", ImGuiDataType_U16, &command5->epsrparamgetcmd.Payload.addr);
        ImGui::InputScalar("type", ImGuiDataType_U8, &command5->epsrparamgetcmd.Payload.type);
        ImGui::InputScalar("size", ImGuiDataType_U16, &command5->epsrparamgetcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Get_Cmd_t) - 7};

            memcpy(command5->epsrparamgetcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamgetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamgetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamgetcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamgetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Get_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamgetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamgetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Get_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Get_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamgetcmd, sizeof(EPS_RParam_Get_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2941: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_SET_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamsetcmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamsetcmd.Payload.table_id);
        ImGui::InputScalar("addr", ImGuiDataType_U16, &command5->epsrparamsetcmd.Payload.addr);
        ImGui::InputScalar("type", ImGuiDataType_U8, &command5->epsrparamsetcmd.Payload.type);
        ImGui::InputText("data", (char *)command5->epsrparamsetcmd.Payload.data, EPS_RPARAM_DATA_MAX_LEN);
        ImGui::InputScalar("size", ImGuiDataType_U16, &command5->epsrparamsetcmd.Payload.size);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Set_Cmd_t) - 7};

            memcpy(command5->epsrparamsetcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamsetcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamsetcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamsetcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamsetcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Set_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamsetcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamsetcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Set_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Set_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamsetcmd, sizeof(EPS_RParam_Set_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2942: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_GET_FULL_TABLE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamgetfulltablecmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamgetfulltablecmd.Payload.table_id);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Get_Full_Table_Cmd_t) - 7};

            memcpy(command5->epsrparamgetfulltablecmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamgetfulltablecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamgetfulltablecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamgetfulltablecmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamgetfulltablecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Get_Full_Table_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamgetfulltablecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamgetfulltablecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Get_Full_Table_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Get_Full_Table_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamgetfulltablecmd, sizeof(EPS_RParam_Get_Full_Table_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        break;
    }

    case 2950: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_SAVE_ALL_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamsaveallcmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Save_All_Cmd_t) - 7};

            memcpy(command5->epsrparamsaveallcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamsaveallcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamsaveallcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamsaveallcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamsaveallcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Save_All_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamsaveallcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamsaveallcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Save_All_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Save_All_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamsaveallcmd, sizeof(EPS_RParam_Save_All_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2951: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_TABLE_SAVE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamtablesavecmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamtablesavecmd.Payload.table_id);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Table_Save_Cmd_t) - 7};

            memcpy(command5->epsrparamtablesavecmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamtablesavecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamtablesavecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamtablesavecmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamtablesavecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Table_Save_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamtablesavecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamtablesavecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Table_Save_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Table_Save_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamtablesavecmd, sizeof(EPS_RParam_Table_Save_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2952: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_TABLE_LOAD_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamtableloadcmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamtableloadcmd.Payload.table_id);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Table_Load_Cmd_t) - 7};

            memcpy(command5->epsrparamtableloadcmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamtableloadcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamtableloadcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamtableloadcmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamtableloadcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Table_Load_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamtableloadcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamtableloadcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Table_Load_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Table_Load_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamtableloadcmd, sizeof(EPS_RParam_Table_Load_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2953: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_SAVE_TO_STORE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamsavetostorecmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamsavetostorecmd.Payload.table_id);
        ImGui::InputText("store", command5->epsrparamsavetostorecmd.Payload.store, EPS_RPARAM_STORE_NAME_LEN);
        ImGui::InputText("slot", command5->epsrparamsavetostorecmd.Payload.slot, EPS_RPARAM_STORE_SLOT_LEN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Save_To_Store_Cmd_t) - 7};

            memcpy(command5->epsrparamsavetostorecmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamsavetostorecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamsavetostorecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamsavetostorecmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamsavetostorecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Save_To_Store_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamsavetostorecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamsavetostorecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Save_To_Store_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Save_To_Store_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamsavetostorecmd, sizeof(EPS_RParam_Save_To_Store_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2954: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_RPARAM_LOAD_FROM_STORE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epsrparamloadfromstorecmd.Payload.csp_node);
        ImGui::InputScalar("table id", ImGuiDataType_U8, &command5->epsrparamloadfromstorecmd.Payload.table_id);
        ImGui::InputText("store", command5->epsrparamloadfromstorecmd.Payload.store, EPS_RPARAM_STORE_NAME_LEN);
        ImGui::InputText("slot", command5->epsrparamloadfromstorecmd.Payload.slot, EPS_RPARAM_STORE_SLOT_LEN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_RParam_Load_From_Store_Cmd_t) - 7};

            memcpy(command5->epsrparamloadfromstorecmd.CmdHeader, &mid, 2);
            memcpy(command5->epsrparamloadfromstorecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsrparamloadfromstorecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsrparamloadfromstorecmd.CmdHeader + 6, &fnccode, 1);

            command5->epsrparamloadfromstorecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_RParam_Load_From_Store_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsrparamloadfromstorecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsrparamloadfromstorecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_RParam_Load_From_Store_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_RParam_Load_From_Store_Cmd_t);
            memcpy(pkt->Data, &command5->epsrparamloadfromstorecmd, sizeof(EPS_RParam_Load_From_Store_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2970: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_REPORT_BCN_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_ReportBcnCmd_t) - 7};

            memcpy(command5->epsreportbcncmd.CmdHeader, &mid, 2);
            memcpy(command5->epsreportbcncmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epsreportbcncmd.CmdHeader + 4, len, 2);
            memcpy(command5->epsreportbcncmd.CmdHeader + 6, &fnccode, 1);

            command5->epsreportbcncmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_ReportBcnCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epsreportbcncmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epsreportbcncmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_ReportBcnCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_ReportBcnCmd_t);
            memcpy(pkt->Data, &command5->epsreportbcncmd, sizeof(EPS_ReportBcnCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }



    //     /********************************************************************/
    // /*                       ADCS Commands (47~)                        */
    // /********************************************************************/

    // /* 47 : ADCS NoOp (CC 0) */
    // case 47: {
    //     static uint16_t msgid   = ADCS_CMD_ID;      // 0x1865
    //     static uint8_t  fnccode = ADCS_NOOP_CC;     // 0

    //     ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    //     ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    //     if (ImGui::Button("Generate CMD")) {

    //         WriteSystemName(msgid);
    //         uint16_t mid  = htons(msgid);
    //         uint8_t  seq[2] = {0xC0, 0x00};
    //         uint8_t  len[2] = {0x00, sizeof(ADCS_NoopCmd_t) - 7};

    //         memcpy(command5->adcsnoopcmd.CmdHeader,     &mid, 2);
    //         memcpy(command5->adcsnoopcmd.CmdHeader + 2, seq,  2);
    //         memcpy(command5->adcsnoopcmd.CmdHeader + 4, len,  2);
    //         memcpy(command5->adcsnoopcmd.CmdHeader + 6, &fnccode, 1);

    //         uint16_t total = sizeof(ADCS_NoopCmd_t);
    //         const uint8_t *p = (const uint8_t *)&command5->adcsnoopcmd;
    //         uint8_t crc = 0xFF;
    //         while (total--) crc ^= *(p++);

    //         memcpy(command5->adcsnoopcmd.CmdHeader + 7, &crc, 1);

    //         packetsign *pkt =
    //             (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_NoopCmd_t));
    //         pkt->Identifier = HVD_TEST;
    //         pkt->PacketType = MIM_PT_TMTC_TEST;
    //         pkt->Length     = sizeof(ADCS_NoopCmd_t);
    //         memcpy(pkt->Data, &command5->adcsnoopcmd,
    //                sizeof(ADCS_NoopCmd_t));

    //         mim::svc::WaitTx();
    //         pthread_create(&p_thread[4], NULL,
    //                        task_uplink_onorbit, (void *)pkt);
    //     }
    //     break;
    // }

    /* 48 : ADCS Reset Counters (CC 1) */
    case 2980: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_PING_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscsppingcmd.Payload.csp_node);
        ImGui::InputScalar("size", ImGuiDataType_U16, &command5->epscsppingcmd.Payload.size);
        ImGui::InputScalar("opts", ImGuiDataType_U8, &command5->epscsppingcmd.Payload.opts);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_Ping_Cmd_t) - 7};

            memcpy(command5->epscsppingcmd.CmdHeader, &mid, 2);
            memcpy(command5->epscsppingcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscsppingcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscsppingcmd.CmdHeader + 6, &fnccode, 1);

            command5->epscsppingcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_Ping_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscsppingcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscsppingcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_Ping_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_Ping_Cmd_t);
            memcpy(pkt->Data, &command5->epscsppingcmd, sizeof(EPS_CSP_Ping_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2981: {
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_REBOOT_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscsprebootcmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_Reboot_Cmd_t) - 7};

            memcpy(command5->epscsprebootcmd.CmdHeader, &mid, 2);
            memcpy(command5->epscsprebootcmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscsprebootcmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscsprebootcmd.CmdHeader + 6, &fnccode, 1);

            command5->epscsprebootcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_Reboot_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscsprebootcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscsprebootcmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_Reboot_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_Reboot_Cmd_t);
            memcpy(pkt->Data, &command5->epscsprebootcmd, sizeof(EPS_CSP_Reboot_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2982:{
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_PS_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscsppscmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_PS_Cmd_t) - 7};

            memcpy(command5->epscsppscmd.CmdHeader, &mid, 2);
            memcpy(command5->epscsppscmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscsppscmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscsppscmd.CmdHeader + 6, &fnccode, 1);

            command5->epscsppscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_PS_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscsppscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscsppscmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_PS_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_PS_Cmd_t);
            memcpy(pkt->Data, &command5->epscsppscmd, sizeof(EPS_CSP_PS_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2983:{
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_MEMFREE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscspmemfreecmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_MemFree_Cmd_t) - 7};

            memcpy(command5->epscspmemfreecmd.CmdHeader, &mid, 2);
            memcpy(command5->epscspmemfreecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscspmemfreecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscspmemfreecmd.CmdHeader + 6, &fnccode, 1);

            command5->epscspmemfreecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_MemFree_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscspmemfreecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscspmemfreecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_MemFree_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_MemFree_Cmd_t);
            memcpy(pkt->Data, &command5->epscspmemfreecmd, sizeof(EPS_CSP_MemFree_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2984:{
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_BUF_FREE_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscspbuffreecmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_BufFree_Cmd_t) - 7};

            memcpy(command5->epscspbuffreecmd.CmdHeader, &mid, 2);
            memcpy(command5->epscspbuffreecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscspbuffreecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscspbuffreecmd.CmdHeader + 6, &fnccode, 1);

            command5->epscspbuffreecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_BufFree_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscspbuffreecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscspbuffreecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_Reboot_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_Reboot_Cmd_t);
            memcpy(pkt->Data, &command5->epscspbuffreecmd, sizeof(EPS_CSP_Reboot_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }

    case 2985:{
        static uint16_t msgid = 0x1875;
        static uint8_t fnccode = EPS_CSP_UPTIME_CC;

        ImGui::InputScalar("msgid", ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8, &fnccode);

        ImGui::InputScalar("csp node", ImGuiDataType_U8, &command5->epscspuptimecmd.Payload.csp_node);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);
            uint16_t mid = htons(msgid);
            uint8_t seq[2] = {0xC0, 0x00};
            uint8_t len[2] = {0x00, sizeof(EPS_CSP_Uptime_Cmd_t) - 7};

            memcpy(command5->epscspuptimecmd.CmdHeader, &mid, 2);
            memcpy(command5->epscspuptimecmd.CmdHeader + 2, seq, 2);
            memcpy(command5->epscspuptimecmd.CmdHeader + 4, len, 2);
            memcpy(command5->epscspuptimecmd.CmdHeader + 6, &fnccode, 1);

            command5->epscspuptimecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(EPS_CSP_Uptime_Cmd_t);
            const uint8_t *p = (const uint8_t *)&command5->epscspuptimecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            memcpy(command5->epscspuptimecmd.CmdHeader + 7, &crc, 1);

            packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(EPS_CSP_Uptime_Cmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length = sizeof(EPS_CSP_Uptime_Cmd_t);
            memcpy(pkt->Data, &command5->epscspuptimecmd, sizeof(EPS_CSP_Uptime_Cmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }
        break;
    }


        // ==================== ttc ====================
    default:
        return false;
    }
    return true;
}
