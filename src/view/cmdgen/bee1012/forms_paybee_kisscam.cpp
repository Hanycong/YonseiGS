// View - 명령 생성기 입력 폼: BEE1012 paybee_kisscam (명령 번호 2400, 2401, 2402, 2403, 2404, 2405, 2406, 2407, 2408, 2409, 2410, 2411)
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

bool DrawBee1012PaybeeKisscamForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 2400: { // PAYBEE_KISSCAM NOOP
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_NOOP_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_NoopCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamnoopcmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamnoopcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamnoopcmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamnoopcmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamnoopcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_NoopCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamnoopcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamnoopcmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_NoopCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_NoopCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamnoopcmd,
                   sizeof(PAYBEE_KISSCAM_NoopCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamnoopcmd.CmdHeader[0],
                    command5->paybee_kisscamnoopcmd.CmdHeader[1],
                    command5->paybee_kisscamnoopcmd.CmdHeader[2],
                    command5->paybee_kisscamnoopcmd.CmdHeader[3],
                    command5->paybee_kisscamnoopcmd.CmdHeader[4],
                    command5->paybee_kisscamnoopcmd.CmdHeader[5],
                    command5->paybee_kisscamnoopcmd.CmdHeader[6],
                    command5->paybee_kisscamnoopcmd.CmdHeader[7]);
        break;
    }

    case 2401: { // PAYBEE_KISSCAM Reset Counters
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_RESET_COUNTERS_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_ResetCountersCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamresetcounterscmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamresetcounterscmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamresetcounterscmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamresetcounterscmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamresetcounterscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_ResetCountersCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamresetcounterscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamresetcounterscmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_ResetCountersCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_ResetCountersCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamresetcounterscmd,
                   sizeof(PAYBEE_KISSCAM_ResetCountersCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[0],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[1],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[2],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[3],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[4],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[5],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[6],
                    command5->paybee_kisscamresetcounterscmd.CmdHeader[7]);
        break;
    }

    case 2402: { // PAYBEE_KISSCAM Ping
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_PING_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_Ping_Payload_t *pl = &command5->paybee_kisscampingcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Ping Payload");
        ImGui::Text("PN: 0 = MCU only, 1 = MCU + Img Sensor");
        ImGui::InputScalar("PN", ImGuiDataType_U8, &pl->PN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_PingCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscampingcmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscampingcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscampingcmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscampingcmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscampingcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_PingCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscampingcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscampingcmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_PingCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_PingCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscampingcmd,
                   sizeof(PAYBEE_KISSCAM_PingCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscampingcmd.CmdHeader[0],
                    command5->paybee_kisscampingcmd.CmdHeader[1],
                    command5->paybee_kisscampingcmd.CmdHeader[2],
                    command5->paybee_kisscampingcmd.CmdHeader[3],
                    command5->paybee_kisscampingcmd.CmdHeader[4],
                    command5->paybee_kisscampingcmd.CmdHeader[5],
                    command5->paybee_kisscampingcmd.CmdHeader[6],
                    command5->paybee_kisscampingcmd.CmdHeader[7]);
        ImGui::Text("Payload: PN=%u", pl->PN);
        break;
    }

    case 2403: { // PAYBEE_KISSCAM Set Mode
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_SET_MODE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_SetMode_Payload_t *pl = &command5->paybee_kisscamsetmodecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Set Mode Payload");
        ImGui::Text("MD: 0 = Sleep, 1 = SD, 2 = HD");
        ImGui::InputScalar("MD", ImGuiDataType_U8, &pl->MD);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_SetModeCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamsetmodecmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamsetmodecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamsetmodecmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamsetmodecmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamsetmodecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_SetModeCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamsetmodecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamsetmodecmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_SetModeCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_SetModeCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamsetmodecmd,
                   sizeof(PAYBEE_KISSCAM_SetModeCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamsetmodecmd.CmdHeader[0],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[1],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[2],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[3],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[4],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[5],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[6],
                    command5->paybee_kisscamsetmodecmd.CmdHeader[7]);
        ImGui::Text("Payload: MD=%u", pl->MD);
        break;
    }

    case 2404: { // PAYBEE_KISSCAM Memory Status (no-arg)
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_MEMORY_STATUS_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_MemoryStatusCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscammemorystatuscmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscammemorystatuscmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscammemorystatuscmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscammemorystatuscmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscammemorystatuscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_MemoryStatusCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscammemorystatuscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscammemorystatuscmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_MemoryStatusCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_MemoryStatusCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscammemorystatuscmd,
                   sizeof(PAYBEE_KISSCAM_MemoryStatusCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[0],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[1],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[2],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[3],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[4],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[5],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[6],
                    command5->paybee_kisscammemorystatuscmd.CmdHeader[7]);
        break;
    }

    case 2405: { // PAYBEE_KISSCAM Set Exposure
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_SET_EXPOSURE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_SetExposure_Payload_t *pl = &command5->paybee_kisscamsetexposurecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Set Exposure Payload");
        ImGui::InputScalar("EX1", ImGuiDataType_U8, &pl->EX1);
        ImGui::InputScalar("EX2", ImGuiDataType_U8, &pl->EX2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_SetExposureCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamsetexposurecmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamsetexposurecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamsetexposurecmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamsetexposurecmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamsetexposurecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_SetExposureCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamsetexposurecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamsetexposurecmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_SetExposureCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_SetExposureCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamsetexposurecmd,
                   sizeof(PAYBEE_KISSCAM_SetExposureCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[0],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[1],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[2],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[3],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[4],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[5],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[6],
                    command5->paybee_kisscamsetexposurecmd.CmdHeader[7]);
        ImGui::Text("Payload: EX1=%u, EX2=%u", pl->EX1, pl->EX2);
        break;
    }

    case 2406: { // PAYBEE_KISSCAM Capture
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_CAPTURE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_Capture_Payload_t *pl = &command5->paybee_kisscamcapturecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Capture Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);
        ImGui::InputScalar("TST", ImGuiDataType_U8, &pl->TST);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_CaptureCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamcapturecmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamcapturecmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamcapturecmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamcapturecmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamcapturecmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_CaptureCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamcapturecmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamcapturecmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_CaptureCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_CaptureCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamcapturecmd,
                   sizeof(PAYBEE_KISSCAM_CaptureCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamcapturecmd.CmdHeader[0],
                    command5->paybee_kisscamcapturecmd.CmdHeader[1],
                    command5->paybee_kisscamcapturecmd.CmdHeader[2],
                    command5->paybee_kisscamcapturecmd.CmdHeader[3],
                    command5->paybee_kisscamcapturecmd.CmdHeader[4],
                    command5->paybee_kisscamcapturecmd.CmdHeader[5],
                    command5->paybee_kisscamcapturecmd.CmdHeader[6],
                    command5->paybee_kisscamcapturecmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, TST=%u", pl->MEM, pl->TST);
        break;
    }

    case 2407: { // PAYBEE_KISSCAM Download
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_DOWNLOAD_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_Download_Payload_t *pl = &command5->paybee_kisscamdownloadcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Download Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);
        ImGui::InputScalar("PRE", ImGuiDataType_U8, &pl->PRE);
        ImGui::InputScalar("LN1", ImGuiDataType_U8, &pl->LN1);
        ImGui::InputScalar("LN2", ImGuiDataType_U8, &pl->LN2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_DownloadCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamdownloadcmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamdownloadcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamdownloadcmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamdownloadcmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamdownloadcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_DownloadCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamdownloadcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamdownloadcmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_DownloadCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_DownloadCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamdownloadcmd,
                   sizeof(PAYBEE_KISSCAM_DownloadCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamdownloadcmd.CmdHeader[0],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[1],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[2],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[3],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[4],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[5],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[6],
                    command5->paybee_kisscamdownloadcmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, PRE=%u, LN1=%u, LN2=%u",
                    pl->MEM, pl->PRE, pl->LN1, pl->LN2);
        break;
    }

    case 2408: { // PAYBEE_KISSCAM Read Register
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_READ_REGISTER_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_ReadRegister_Payload_t *pl = &command5->paybee_kisscamreadregistercmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Read Register Payload");
        ImGui::InputScalar("AD1", ImGuiDataType_U8, &pl->AD1);
        ImGui::InputScalar("AD2", ImGuiDataType_U8, &pl->AD2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_ReadRegisterCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamreadregistercmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamreadregistercmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamreadregistercmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamreadregistercmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamreadregistercmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_ReadRegisterCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamreadregistercmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamreadregistercmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_ReadRegisterCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_ReadRegisterCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamreadregistercmd,
                   sizeof(PAYBEE_KISSCAM_ReadRegisterCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamreadregistercmd.CmdHeader[0],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[1],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[2],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[3],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[4],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[5],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[6],
                    command5->paybee_kisscamreadregistercmd.CmdHeader[7]);
        ImGui::Text("Payload: AD1=%u, AD2=%u", pl->AD1, pl->AD2);
        break;
    }

    case 2409: { // PAYBEE_KISSCAM Write Register
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_WRITE_REGISTER_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_WriteRegister_Payload_t *pl = &command5->paybee_kisscamwriteregistercmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Write Register Payload");
        ImGui::InputScalar("AD1", ImGuiDataType_U8, &pl->AD1);
        ImGui::InputScalar("AD2", ImGuiDataType_U8, &pl->AD2);
        ImGui::InputScalar("RG1", ImGuiDataType_U8, &pl->RG1);
        ImGui::InputScalar("RG2", ImGuiDataType_U8, &pl->RG2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_WriteRegisterCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamwriteregistercmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamwriteregistercmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamwriteregistercmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamwriteregistercmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamwriteregistercmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_WriteRegisterCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamwriteregistercmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamwriteregistercmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_WriteRegisterCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_WriteRegisterCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamwriteregistercmd,
                   sizeof(PAYBEE_KISSCAM_WriteRegisterCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[0],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[1],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[2],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[3],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[4],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[5],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[6],
                    command5->paybee_kisscamwriteregistercmd.CmdHeader[7]);
        ImGui::Text("Payload: AD1=%u, AD2=%u, RG1=%u, RG2=%u",
                    pl->AD1, pl->AD2, pl->RG1, pl->RG2);
        break;
    }

    case 2410: { // PAYBEE_KISSCAM Download All
        static uint16_t msgid   = 0x1833;
        static uint8_t  fnccode = PAYBEE_KISSCAM_DOWNLOAD_ALL_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_DownloadAll_Payload_t *pl = &command5->paybee_kisscamdownloadallcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Download All Payload");
        ImGui::InputScalar("MEM",       ImGuiDataType_U8,  &pl->MEM);
        ImGui::InputScalar("PRE",       ImGuiDataType_U8,  &pl->PRE);
        ImGui::InputScalar("StartLine", ImGuiDataType_U16, &pl->StartLine);
        ImGui::InputScalar("LineNum",   ImGuiDataType_U16, &pl->LineNum);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_DownloadAllCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamdownloadallcmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamdownloadallcmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamdownloadallcmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamdownloadallcmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamdownloadallcmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_DownloadAllCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamdownloadallcmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamdownloadallcmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_DownloadAllCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_DownloadAllCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamdownloadallcmd,
                   sizeof(PAYBEE_KISSCAM_DownloadAllCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[0],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[1],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[2],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[3],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[4],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[5],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[6],
                    command5->paybee_kisscamdownloadallcmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, PRE=%u, StartLine=%u, LineNum=%u",
                    pl->MEM, pl->PRE, pl->StartLine, pl->LineNum);
        break;
    }

    case 2411: { // PAYBEE_KISSCAM Image Compress
        static uint16_t msgid   = PAYBEE_KISSCAM_CMD_ID;
        static uint8_t  fnccode = PAYBEE_KISSCAM_IMAGE_COMPRESS_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYBEE_KISSCAM_ImageCompress_Payload_t *pl =
            &command5->paybee_kisscamimagecompresscmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYBEE_KISSCAM Image Compress Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);
        ImGui::InputScalar("TargetIdx", ImGuiDataType_U8, &pl->TargetIdx);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            uint16_t mid = htons(msgid);
            uint8_t  seq[2] = {0xC0, 0x00};
            uint8_t  len[2] = {
                0x00,
                (uint8_t)(sizeof(PAYBEE_KISSCAM_ImageCompressCmd_t) - 7)
            };

            memcpy(command5->paybee_kisscamimagecompresscmd.CmdHeader,     &mid, 2);
            memcpy(command5->paybee_kisscamimagecompresscmd.CmdHeader + 2, seq,  2);
            memcpy(command5->paybee_kisscamimagecompresscmd.CmdHeader + 4, len,  2);
            memcpy(command5->paybee_kisscamimagecompresscmd.CmdHeader + 6, &fnccode, 1);

            command5->paybee_kisscamimagecompresscmd.CmdHeader[7] = 0;
            uint16_t total = sizeof(PAYBEE_KISSCAM_ImageCompressCmd_t);
            const uint8_t *p = (const uint8_t *)&command5->paybee_kisscamimagecompresscmd;
            uint8_t crc = 0xFF;
            while (total--) crc ^= *(p++);
            command5->paybee_kisscamimagecompresscmd.CmdHeader[7] = crc;

            packetsign *pkt =
                (packetsign *)malloc(2 + 2 + 4 + sizeof(PAYBEE_KISSCAM_ImageCompressCmd_t));
            pkt->Identifier = HVD_TEST;
            pkt->PacketType = MIM_PT_TMTC_TEST;
            pkt->Length     = sizeof(PAYBEE_KISSCAM_ImageCompressCmd_t);
            memcpy(pkt->Data, &command5->paybee_kisscamimagecompresscmd,
                   sizeof(PAYBEE_KISSCAM_ImageCompressCmd_t));

            mim::svc::WaitTx();
            mim::svc::SendUplink((void *)pkt);
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[0],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[1],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[2],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[3],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[4],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[5],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[6],
                    command5->paybee_kisscamimagecompresscmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, TargetIdx=%u", pl->MEM, pl->TargetIdx);
        break;
    }












        // ==================== utrx ====================
    default:
        return false;
    }
    return true;
}
