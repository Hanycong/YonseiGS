// View - 명령 생성기 입력 폼: BEE1000 PAYUZUC 앱 (명령 번호 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180)
//
// cmdgen_view.cpp 의 명령 번호 switch 에서 이 앱의 case 를 내용 그대로 옮긴 것.
// 처리한 명령이면 true.
#include <pthread.h>
#include <inttypes.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <gs/param/rparam.h>
#include <csp/switch.h>
#include <csp/delay.h>

#include "miman_config.h"
#include "miman_imgui.h"
#include "miman_csp.h"
#include "miman_radial.h"
#include "miman_coms.h"
#include "miman_orbital.h"
#include "miman_model.h"
#include "miman_s_control.h"
#include "miman_ftp.h"
#include "miman_autopilot.h"

#include "miman_theme.h"
#include "miman_satcontext.h"
#include "svc_tx.h"
#include "svc_tracking.h"
#include "svc_radio.h"
#include "svc_ftp.h"
#include "command_catalog.h"
#include "ccsds_cmd.h"
#include "telecommand_sender.h"
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1000PayuzucForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 168: { // PAYUZUC NOOP
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_NOOP_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucnoopcmd, sizeof(PAYUZUC_NoopCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucnoopcmd, sizeof(PAYUZUC_NoopCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucnoopcmd.CmdHeader[0],
                    command->payuzucnoopcmd.CmdHeader[1],
                    command->payuzucnoopcmd.CmdHeader[2],
                    command->payuzucnoopcmd.CmdHeader[3],
                    command->payuzucnoopcmd.CmdHeader[4],
                    command->payuzucnoopcmd.CmdHeader[5],
                    command->payuzucnoopcmd.CmdHeader[6],
                    command->payuzucnoopcmd.CmdHeader[7]);
        break;
    }

    case 169: { // PAYUZUC Reset Counters
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_RESET_COUNTERS_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucresetcounterscmd, sizeof(PAYUZUC_ResetCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucresetcounterscmd, sizeof(PAYUZUC_ResetCountersCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucresetcounterscmd.CmdHeader[0],
                    command->payuzucresetcounterscmd.CmdHeader[1],
                    command->payuzucresetcounterscmd.CmdHeader[2],
                    command->payuzucresetcounterscmd.CmdHeader[3],
                    command->payuzucresetcounterscmd.CmdHeader[4],
                    command->payuzucresetcounterscmd.CmdHeader[5],
                    command->payuzucresetcounterscmd.CmdHeader[6],
                    command->payuzucresetcounterscmd.CmdHeader[7]);
        break;
    }

    case 170: { // PAYUZUC Ping
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_PING_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_Ping_Payload_t *pl = &command->payuzucpingcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Ping Payload");
        ImGui::Text("PN: 0 = MCU only, 1 = MCU + Img Sensor");
        ImGui::InputScalar("PN", ImGuiDataType_U8, &pl->PN);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucpingcmd, sizeof(PAYUZUC_PingCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucpingcmd, sizeof(PAYUZUC_PingCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucpingcmd.CmdHeader[0],
                    command->payuzucpingcmd.CmdHeader[1],
                    command->payuzucpingcmd.CmdHeader[2],
                    command->payuzucpingcmd.CmdHeader[3],
                    command->payuzucpingcmd.CmdHeader[4],
                    command->payuzucpingcmd.CmdHeader[5],
                    command->payuzucpingcmd.CmdHeader[6],
                    command->payuzucpingcmd.CmdHeader[7]);
        ImGui::Text("Payload: PN=%u", pl->PN);
        break;
    }

    case 171: { // PAYUZUC Set Mode
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_SET_MODE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_SetMode_Payload_t *pl = &command->payuzucsetmodecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Set Mode Payload");
        ImGui::Text("MD: 0 = Sleep, 1 = SD, 2 = HD");
        ImGui::InputScalar("MD", ImGuiDataType_U8, &pl->MD);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucsetmodecmd, sizeof(PAYUZUC_SetModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucsetmodecmd, sizeof(PAYUZUC_SetModeCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucsetmodecmd.CmdHeader[0],
                    command->payuzucsetmodecmd.CmdHeader[1],
                    command->payuzucsetmodecmd.CmdHeader[2],
                    command->payuzucsetmodecmd.CmdHeader[3],
                    command->payuzucsetmodecmd.CmdHeader[4],
                    command->payuzucsetmodecmd.CmdHeader[5],
                    command->payuzucsetmodecmd.CmdHeader[6],
                    command->payuzucsetmodecmd.CmdHeader[7]);
        ImGui::Text("Payload: MD=%u", pl->MD);
        break;
    }

    case 172: { // PAYUZUC Memory Status (no-arg)
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_MEMORY_STATUS_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucmemorystatuscmd, sizeof(PAYUZUC_MemoryStatusCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucmemorystatuscmd, sizeof(PAYUZUC_MemoryStatusCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucmemorystatuscmd.CmdHeader[0],
                    command->payuzucmemorystatuscmd.CmdHeader[1],
                    command->payuzucmemorystatuscmd.CmdHeader[2],
                    command->payuzucmemorystatuscmd.CmdHeader[3],
                    command->payuzucmemorystatuscmd.CmdHeader[4],
                    command->payuzucmemorystatuscmd.CmdHeader[5],
                    command->payuzucmemorystatuscmd.CmdHeader[6],
                    command->payuzucmemorystatuscmd.CmdHeader[7]);
        break;
    }

    case 173: { // PAYUZUC Set Exposure
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_SET_EXPOSURE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_SetExposure_Payload_t *pl = &command->payuzucsetexposurecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Set Exposure Payload");
        ImGui::InputScalar("EX1", ImGuiDataType_U8, &pl->EX1);
        ImGui::InputScalar("EX2", ImGuiDataType_U8, &pl->EX2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucsetexposurecmd, sizeof(PAYUZUC_SetExposureCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucsetexposurecmd, sizeof(PAYUZUC_SetExposureCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucsetexposurecmd.CmdHeader[0],
                    command->payuzucsetexposurecmd.CmdHeader[1],
                    command->payuzucsetexposurecmd.CmdHeader[2],
                    command->payuzucsetexposurecmd.CmdHeader[3],
                    command->payuzucsetexposurecmd.CmdHeader[4],
                    command->payuzucsetexposurecmd.CmdHeader[5],
                    command->payuzucsetexposurecmd.CmdHeader[6],
                    command->payuzucsetexposurecmd.CmdHeader[7]);
        ImGui::Text("Payload: EX1=%u, EX2=%u", pl->EX1, pl->EX2);
        break;
    }

    case 174: { // PAYUZUC Capture
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_CAPTURE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_Capture_Payload_t *pl = &command->payuzuccapturecmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Capture Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);
        ImGui::InputScalar("TST", ImGuiDataType_U8, &pl->TST);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzuccapturecmd, sizeof(PAYUZUC_CaptureCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzuccapturecmd, sizeof(PAYUZUC_CaptureCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzuccapturecmd.CmdHeader[0],
                    command->payuzuccapturecmd.CmdHeader[1],
                    command->payuzuccapturecmd.CmdHeader[2],
                    command->payuzuccapturecmd.CmdHeader[3],
                    command->payuzuccapturecmd.CmdHeader[4],
                    command->payuzuccapturecmd.CmdHeader[5],
                    command->payuzuccapturecmd.CmdHeader[6],
                    command->payuzuccapturecmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, TST=%u", pl->MEM, pl->TST);
        break;
    }

    case 175: { // PAYUZUC Download
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_DOWNLOAD_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_Download_Payload_t *pl = &command->payuzucdownloadcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Download Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);
        ImGui::InputScalar("PRE", ImGuiDataType_U8, &pl->PRE);
        ImGui::InputScalar("LN1", ImGuiDataType_U8, &pl->LN1);
        ImGui::InputScalar("LN2", ImGuiDataType_U8, &pl->LN2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucdownloadcmd, sizeof(PAYUZUC_DownloadCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucdownloadcmd, sizeof(PAYUZUC_DownloadCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucdownloadcmd.CmdHeader[0],
                    command->payuzucdownloadcmd.CmdHeader[1],
                    command->payuzucdownloadcmd.CmdHeader[2],
                    command->payuzucdownloadcmd.CmdHeader[3],
                    command->payuzucdownloadcmd.CmdHeader[4],
                    command->payuzucdownloadcmd.CmdHeader[5],
                    command->payuzucdownloadcmd.CmdHeader[6],
                    command->payuzucdownloadcmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, PRE=%u, LN1=%u, LN2=%u",
                    pl->MEM, pl->PRE, pl->LN1, pl->LN2);
        break;
    }

    case 176: { // PAYUZUC Read Register
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_READ_REGISTER_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_ReadRegister_Payload_t *pl = &command->payuzucreadregistercmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Read Register Payload");
        ImGui::InputScalar("AD1", ImGuiDataType_U8, &pl->AD1);
        ImGui::InputScalar("AD2", ImGuiDataType_U8, &pl->AD2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucreadregistercmd, sizeof(PAYUZUC_ReadRegisterCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucreadregistercmd, sizeof(PAYUZUC_ReadRegisterCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucreadregistercmd.CmdHeader[0],
                    command->payuzucreadregistercmd.CmdHeader[1],
                    command->payuzucreadregistercmd.CmdHeader[2],
                    command->payuzucreadregistercmd.CmdHeader[3],
                    command->payuzucreadregistercmd.CmdHeader[4],
                    command->payuzucreadregistercmd.CmdHeader[5],
                    command->payuzucreadregistercmd.CmdHeader[6],
                    command->payuzucreadregistercmd.CmdHeader[7]);
        ImGui::Text("Payload: AD1=%u, AD2=%u", pl->AD1, pl->AD2);
        break;
    }

    case 177: { // PAYUZUC Write Register
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_WRITE_REGISTER_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_WriteRegister_Payload_t *pl = &command->payuzucwriteregistercmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Write Register Payload");
        ImGui::InputScalar("AD1", ImGuiDataType_U8, &pl->AD1);
        ImGui::InputScalar("AD2", ImGuiDataType_U8, &pl->AD2);
        ImGui::InputScalar("RG1", ImGuiDataType_U8, &pl->RG1);
        ImGui::InputScalar("RG2", ImGuiDataType_U8, &pl->RG2);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucwriteregistercmd, sizeof(PAYUZUC_WriteRegisterCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucwriteregistercmd, sizeof(PAYUZUC_WriteRegisterCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucwriteregistercmd.CmdHeader[0],
                    command->payuzucwriteregistercmd.CmdHeader[1],
                    command->payuzucwriteregistercmd.CmdHeader[2],
                    command->payuzucwriteregistercmd.CmdHeader[3],
                    command->payuzucwriteregistercmd.CmdHeader[4],
                    command->payuzucwriteregistercmd.CmdHeader[5],
                    command->payuzucwriteregistercmd.CmdHeader[6],
                    command->payuzucwriteregistercmd.CmdHeader[7]);
        ImGui::Text("Payload: AD1=%u, AD2=%u, RG1=%u, RG2=%u",
                    pl->AD1, pl->AD2, pl->RG1, pl->RG2);
        break;
    }

    case 178: { // PAYUZUC Download All
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_DOWNLOAD_ALL_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_DownloadAll_Payload_t *pl = &command->payuzucdownloadallcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Download All Payload");
        ImGui::InputScalar("MEM",       ImGuiDataType_U8,  &pl->MEM);
        ImGui::InputScalar("PRE",       ImGuiDataType_U8,  &pl->PRE);
        ImGui::InputScalar("StartLine", ImGuiDataType_U16, &pl->StartLine);
        ImGui::InputScalar("LineNum",   ImGuiDataType_U16, &pl->LineNum);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucdownloadallcmd, sizeof(PAYUZUC_DownloadAllCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucdownloadallcmd, sizeof(PAYUZUC_DownloadAllCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucdownloadallcmd.CmdHeader[0],
                    command->payuzucdownloadallcmd.CmdHeader[1],
                    command->payuzucdownloadallcmd.CmdHeader[2],
                    command->payuzucdownloadallcmd.CmdHeader[3],
                    command->payuzucdownloadallcmd.CmdHeader[4],
                    command->payuzucdownloadallcmd.CmdHeader[5],
                    command->payuzucdownloadallcmd.CmdHeader[6],
                    command->payuzucdownloadallcmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, PRE=%u, StartLine=%u, LineNum=%u",
                    pl->MEM, pl->PRE, pl->StartLine, pl->LineNum);
        break;
    }

    case 179: { // PAYUZUC Mosaic
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_MOSAIC_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_Mosaic_Payload_t *pl = &command->payuzucmosaiccmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Mosaic Payload");
        ImGui::InputScalar("MEM", ImGuiDataType_U8, &pl->MEM);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucmosaiccmd, sizeof(PAYUZUC_MosaicCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucmosaiccmd, sizeof(PAYUZUC_MosaicCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucmosaiccmd.CmdHeader[0],
                    command->payuzucmosaiccmd.CmdHeader[1],
                    command->payuzucmosaiccmd.CmdHeader[2],
                    command->payuzucmosaiccmd.CmdHeader[3],
                    command->payuzucmosaiccmd.CmdHeader[4],
                    command->payuzucmosaiccmd.CmdHeader[5],
                    command->payuzucmosaiccmd.CmdHeader[6],
                    command->payuzucmosaiccmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u", pl->MEM);
        break;
    }

    case 180: { // PAYUZUC Download All Child (CC 23, payload same as DownloadAll)
        static uint16_t msgid   = PAYUZUC_CMD_ID;
        static uint8_t  fnccode = PAYUZUC_DOWNLOAD_ALL_CHILD_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        PAYUZUC_DownloadAll_Payload_t *pl = &command->payuzucdownloadallchildcmd.Payload;

        ImGui::Separator();
        ImGui::Text("PAYUZUC Download All Child Payload");
        ImGui::InputScalar("MEM",       ImGuiDataType_U8,  &pl->MEM);
        ImGui::InputScalar("PRE",       ImGuiDataType_U8,  &pl->PRE);
        ImGui::InputScalar("StartLine", ImGuiDataType_U16, &pl->StartLine);
        ImGui::InputScalar("LineNum",   ImGuiDataType_U16, &pl->LineNum);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->payuzucdownloadallchildcmd, sizeof(PAYUZUC_DownloadAllCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->payuzucdownloadallchildcmd, sizeof(PAYUZUC_DownloadAllCmd_t));
        }

        ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                    command->payuzucdownloadallchildcmd.CmdHeader[0],
                    command->payuzucdownloadallchildcmd.CmdHeader[1],
                    command->payuzucdownloadallchildcmd.CmdHeader[2],
                    command->payuzucdownloadallchildcmd.CmdHeader[3],
                    command->payuzucdownloadallchildcmd.CmdHeader[4],
                    command->payuzucdownloadallchildcmd.CmdHeader[5],
                    command->payuzucdownloadallchildcmd.CmdHeader[6],
                    command->payuzucdownloadallchildcmd.CmdHeader[7]);
        ImGui::Text("Payload: MEM=%u, PRE=%u, StartLine=%u, LineNum=%u",
                    pl->MEM, pl->PRE, pl->StartLine, pl->LineNum);
        break;
    }











    default:
        return false;
    }
    return true;
}
