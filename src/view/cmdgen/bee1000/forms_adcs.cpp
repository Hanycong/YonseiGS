// View - 명령 생성기 입력 폼: BEE1000 ADCS 앱 (명령 번호 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 82, 85, 86, 88, 89)
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

bool DrawBee1000AdcsForms(uint16_t fnccode)
{
    switch (fnccode)
    {
    case 47: {
        static uint16_t msgid   = ADCS_CMD_ID;      // 0x1865
        static uint8_t  fnccode = ADCS_NOOP_CC;     // 0

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsnoopcmd, sizeof(ADCS_NoopCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsnoopcmd, sizeof(ADCS_NoopCmd_t));
        }
        break;
    }

    /* 48 : ADCS Reset Counters (CC 1) */
    case 48: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_RESET_COUNTERS_CC;   // 1

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsresetcounterscmd, sizeof(ADCS_ResetCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsresetcounterscmd, sizeof(ADCS_ResetCountersCmd_t));
        }
        break;
    }

    /* 49 : ADCS Reset App Cmd Counters (CC 2, no-arg로 사용) */
    case 49: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_RESET_APP_CMD_COUNTERS_CC;   // 2

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            /* no-arg 구조체: 헤더만 있는 타입으로 정의했다고 가정
               typedef struct { uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; } ADCS_ResetAppCmdCountersCmd_t;
            */

            mim::tc::EncodeCfeCommand(&command->adcsresetappcmdcounterscmd, sizeof(ADCS_ResetAppCmdCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsresetappcmdcounterscmd, sizeof(ADCS_ResetAppCmdCountersCmd_t));
        }
        break;
    }

    /* 50 : ADCS Reset Device Cmd Counters (CC 3, no-arg) */
    case 50: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_RESET_DEVICE_CMD_COUNTERS_CC;   // 3

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);
            /* no-arg 구조체:
               typedef struct { uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; } ADCS_ResetDeviceCmdCountersCmd_t;
            */

            mim::tc::EncodeCfeCommand(&command->adcsresetdevicecmdcounterscmd, sizeof(ADCS_ResetDeviceCmdCountersCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsresetdevicecmdcounterscmd, sizeof(ADCS_ResetDeviceCmdCountersCmd_t));
        }
        break;
    }

    /* 51 : ADCS Set Communication Mode as CAN (CC 4, no-arg) */
    case 51: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_COMMUNICATION_MODE_AS_CAN_CC;   // 4

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            /* no-arg struct:
               typedef struct { uint8_t CmdHeader[CFE_SB_CMD_HDR_SIZE]; } ADCS_SetCommunicationModeAsCanCmd_t;
            */

            mim::tc::EncodeCfeCommand(&command->adcssetcommunicationmodeascan, sizeof(ADCS_SetCommunicationModeAsCanCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcssetcommunicationmodeascan, sizeof(ADCS_SetCommunicationModeAsCanCmd_t));
        }
        break;
    }

    /* 52 : ADCS GPIO Enable High (CC 5, no-arg) */
    case 52: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_ENABLE_HIGH_CC;   // 5

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsgpioenhighcmd, sizeof(ADCS_GpioEnHighCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsgpioenhighcmd, sizeof(ADCS_GpioEnHighCmd_t));
        }
        break;
    }

    /* 53 : ADCS GPIO Enable Low (CC 6, no-arg) */
    case 53: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_ENABLE_LOW_CC;   // 6

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsgpioenlowcmd, sizeof(ADCS_GpioEnLowCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsgpioenlowcmd, sizeof(ADCS_GpioEnLowCmd_t));
        }
        break;
    }

    /* 54 : ADCS GPIO Boot High (CC 8, no-arg) */
    case 54: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_BOOT_HIGH_CC;   // 8

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsgioboothighcmd, sizeof(ADCS_GpioBootHighCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsgioboothighcmd, sizeof(ADCS_GpioBootHighCmd_t));
        }
        break;
    }

    /* 55 : ADCS GPIO Boot Low (CC 9, no-arg) */
    case 55: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_GPIO_BOOT_LOW_CC;   // 9

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsgiobootlowcmd, sizeof(ADCS_GpioBootLowCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsgiobootlowcmd, sizeof(ADCS_GpioBootLowCmd_t));
        }
        break;
    }

    /* 56 : ADCS Exit Bootloader (CC 10, no-arg) */
    case 56: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_EXIT_BOOTLOADER_CC;   // 10

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsexitbootloadercmd, sizeof(ADCS_ExitBootLoaderCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsexitbootloadercmd, sizeof(ADCS_ExitBootLoaderCmd_t));
        }
        break;
    }

    /* 57 : ADCS Reset (CC 11, no-arg) */
    case 57: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_RESET_CC;   // 11

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsresetcmd, sizeof(ADCS_ResetCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsresetcmd, sizeof(ADCS_ResetCmd_t));
        }
        break;
    }

    /* 58 : ADCS Set Current Unix Time (CC 12, payload) */
    case 58: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CURRENT_UNIX_TIME_CC;   // 12

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Unix Seconds",     ImGuiDataType_U32,
                           &command->adcscurrentunixtimecmd.Payload.CurrentUnixseconds);
        ImGui::InputScalar("Unix Nanoseconds", ImGuiDataType_U32,
                           &command->adcscurrentunixtimecmd.Payload.CurrentUnixNanoseconds);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcscurrentunixtimecmd, sizeof(ADCS_CurrentUnixTimeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcscurrentunixtimecmd, sizeof(ADCS_CurrentUnixTimeCmd_t));
        }
        break;
    }

/* 59 : ADCS Error Log Setting (CC 13, payload) */
case 59: {
    static uint16_t msgid   = ADCS_CMD_ID;
    static uint8_t  fnccode = ADCS_SET_ERROR_LOG_SETTING_CC;   // 13

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);


    uint8_t active_tmp =
        command->adcserrorlogsettingcmd.Payload.ActiveState ? 1 : 0;
    uint8_t buffer_tmp =
        command->adcserrorlogsettingcmd.Payload.BufferFullAction ? 1 : 0;

    ImGui::InputScalar("ActiveState",      ImGuiDataType_U8, &active_tmp);
    ImGui::InputScalar("BufferFullAction", ImGuiDataType_U8, &buffer_tmp);

    command->adcserrorlogsettingcmd.Payload.ActiveState      = active_tmp & 0x1;
    command->adcserrorlogsettingcmd.Payload.BufferFullAction = buffer_tmp & 0x1;

    if (ImGui::Button("Generate CMD")) {

        WriteSystemName(msgid);

        mim::tc::EncodeCfeCommand(&command->adcserrorlogsettingcmd, sizeof(ADCS_ErrorLogSettingCmd_t), msgid, fnccode);
        mim::tc::SendTelecommand(&command->adcserrorlogsettingcmd, sizeof(ADCS_ErrorLogSettingCmd_t));
    }
    break;
}


    /* 60 : ADCS Persist Config (CC 14, no-arg) */
    case 60: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_PERSIST_CONFIG_CC;   // 14

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcspersistconfigcmd, sizeof(ADCS_PersistConfigCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcspersistconfigcmd, sizeof(ADCS_PersistConfigCmd_t));
        }
        break;
    }

    /* 61 : ADCS Set Control Estimation Mode (CC 15) */
    case 61: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CONTROL_ESTIMATION_MODE_CC;   // 15

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("ControlMode",        ImGuiDataType_U8,
                           &command->adcscontrolestimationmodecmd.Payload.ControlMode);
        ImGui::InputScalar("MainEstimatorMode",  ImGuiDataType_U8,
                           &command->adcscontrolestimationmodecmd.Payload.MainEstimatorMode);
        ImGui::InputScalar("BackupEstimatorMode",ImGuiDataType_U8,
                           &command->adcscontrolestimationmodecmd.Payload.BackupEstimatorMode);
        ImGui::InputScalar("ControlTimeout",     ImGuiDataType_U16,
                           &command->adcscontrolestimationmodecmd.Payload.ControlTimeout);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcscontrolestimationmodecmd, sizeof(ADCS_ControlEstimationModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcscontrolestimationmodecmd, sizeof(ADCS_ControlEstimationModeCmd_t));
        }
        break;
    }

    /* 62 : ADCS Disable Mag/RWL/MntMng (CC 16) */
    case 62: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_DISABLE_MAG_RWL_MNT_MNG_CC;   // 16

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Duration", ImGuiDataType_U16,
                           &command->adcsdisablemagrwlmntmngcmd.Payload.Duration);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsdisablemagrwlmntmngcmd, sizeof(ADCS_DisableMagRwlMntMngCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsdisablemagrwlmntmngcmd, sizeof(ADCS_DisableMagRwlMntMngCmd_t));
        }
        break;
    }

    /* 63 : ADCS Set Reference IRC Vector (CC 17) */
    case 63: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_IRC_VECTOR_CC;   // 17

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("ECI X", ImGuiDataType_Float,
                           &command->adcsreferenceircvectorcmd.Payload.ECIPointingVectorX);
        ImGui::InputScalar("ECI Y", ImGuiDataType_Float,
                           &command->adcsreferenceircvectorcmd.Payload.ECIPointingVectorY);
        ImGui::InputScalar("ECI Z", ImGuiDataType_Float,
                           &command->adcsreferenceircvectorcmd.Payload.ECIPointingVectorZ);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsreferenceircvectorcmd, sizeof(ADCS_ReferenceIRCVectorCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsreferenceircvectorcmd, sizeof(ADCS_ReferenceIRCVectorCmd_t));
        }
        break;
    }

    /* 64 : ADCS Set Reference LLH Target (CC 18) */
    case 64: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_LLH_TARGET_CC;   // 18

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Lat [deg]", ImGuiDataType_Float,
                           &command->adcsreferencellhtargetcmd.Payload.TargetLatitude);
        ImGui::InputScalar("Lon [deg]", ImGuiDataType_Float,
                           &command->adcsreferencellhtargetcmd.Payload.TargetLongiTude);
        ImGui::InputScalar("Alt [m]",   ImGuiDataType_Float,
                           &command->adcsreferencellhtargetcmd.Payload.TargetAltitude);

        if (ImGui::Button("Generate CMD")) {

            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsreferencellhtargetcmd, sizeof(ADCS_ReferenceLLHTargetCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsreferencellhtargetcmd, sizeof(ADCS_ReferenceLLHTargetCmd_t));
        }
        break;
    }

    /* 65 : ADCS Set Orbit Mode (CC 19) */
    case 65: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_ORBIT_MODE_CC;   // 19

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("OrbitMode",ImGuiDataType_U8,
                           &command->adcsorbitmodecmd.Payload.OrbitMode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsorbitmodecmd, sizeof(ADCS_OrbitModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsorbitmodecmd, sizeof(ADCS_OrbitModeCmd_t));
        }
        break;
    }
/* 66 : ADCS MAG Deploy Command (CC 20) */
case 66: {
    static uint16_t msgid   = ADCS_CMD_ID;
    static uint8_t  fnccode = ADCS_SET_MAG_DEPLOY_CMD_CC;   // 20

    static uint8_t deploy0 = 0;
    static uint8_t deploy1 = 0;


    deploy0 = command->adcsmagdeploycmd.Payload.DeployMAG0;
    deploy1 = command->adcsmagdeploycmd.Payload.DeployMAG1;

    ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
    ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

    ImGui::InputScalar("DeployMAG0", ImGuiDataType_U8, &deploy0);
    ImGui::InputScalar("DeployMAG1", ImGuiDataType_U8, &deploy1);

    if (ImGui::Button("Generate CMD")) {

        command->adcsmagdeploycmd.Payload.DeployMAG0 = deploy0 & 0x1;
        command->adcsmagdeploycmd.Payload.DeployMAG1 = deploy1 & 0x1;

        WriteSystemName(msgid);

        mim::tc::EncodeCfeCommand(&command->adcsmagdeploycmd, sizeof(ADCS_MagDeployCmd_t), msgid, fnccode);
        mim::tc::SendTelecommand(&command->adcsmagdeploycmd, sizeof(ADCS_MagDeployCmd_t));
    }
    break;
}

    /* 67 : ADCS Set Reference RPY Values (CC 21) */
    case 67: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_REFERENCE_RPY_VALUES_CC;   // 21

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("Roll [deg]",  ImGuiDataType_Float,
                           &command->adcsreferencerpyvaluescmd.Payload.Roll);
        ImGui::InputScalar("Pitch [deg]", ImGuiDataType_Float,
                           &command->adcsreferencerpyvaluescmd.Payload.Pitch);
        ImGui::InputScalar("Yaw [deg]",   ImGuiDataType_Float,
                           &command->adcsreferencerpyvaluescmd.Payload.Yaw);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsreferencerpyvaluescmd, sizeof(ADCS_ReferenceRPYvaluesCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsreferencerpyvaluescmd, sizeof(ADCS_ReferenceRPYvaluesCmd_t));
        }
        break;
    }

    /* 68 : ADCS Set Open Loop MTQ Command (CC 22) */
    case 68: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_OPENLOOPCMD_MTQ_CC;   // 22

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("MTQ0 Cmd", ImGuiDataType_S16,
                           &command->adcsopenloopcmdmtqcmd.Payload.MTQ0_OpenLoopCmd);
        ImGui::InputScalar("MTQ1 Cmd", ImGuiDataType_S16,
                           &command->adcsopenloopcmdmtqcmd.Payload.MTQ1_OpenLoopCmd);
        ImGui::InputScalar("MTQ2 Cmd", ImGuiDataType_S16,
                           &command->adcsopenloopcmdmtqcmd.Payload.MTQ2_OpenLoopCmd);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsopenloopcmdmtqcmd, sizeof(ADCS_OpenLoopCmdMTQCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsopenloopcmdmtqcmd, sizeof(ADCS_OpenLoopCmdMTQCmd_t));
        }
        break;
    }


    case 69: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_POWER_STATE_CC;

        ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

        ImGui::InputScalar("RWL0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.RWL0);
        ImGui::InputScalar("RWL1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.RWL1);
        ImGui::InputScalar("RWL2", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.RWL2);
        ImGui::InputScalar("RWL3", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.RWL3);

        ImGui::InputScalar("MAG0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.MAG0);
        ImGui::InputScalar("MAG1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.MAG1);

        ImGui::InputScalar("GYR0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.GYR0);
        ImGui::InputScalar("GYR1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.GYR1);

        ImGui::InputScalar("FSS0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.FSS0);
        ImGui::InputScalar("FSS1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.FSS1);
        ImGui::InputScalar("FSS2", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.FSS2);
        ImGui::InputScalar("FSS3", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.FSS3);

        ImGui::InputScalar("HSS0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.HSS0);
        ImGui::InputScalar("HSS1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.HSS1);

        ImGui::InputScalar("STR0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.STR0);
        ImGui::InputScalar("STR1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.STR1);

        ImGui::InputScalar("ExtSensor0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.ExtSensor0);
        ImGui::InputScalar("ExtSensor1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.ExtSensor1);

        ImGui::InputScalar("ExtGYR0", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.ExtGYR0);
        ImGui::InputScalar("ExtGYR1", ImGuiDataType_U8,
                           &command->adcspowerstatecmd.Payload.ExtGYR1);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcspowerstatecmd, sizeof(ADCS_PowerStateCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcspowerstatecmd, sizeof(ADCS_PowerStateCmd_t));
        }
        break;
    }







    /* 70 : ADCS Set Run Mode (CC 24) */
    case 70: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_RUN_MODE_CC;   // 24

        ImGui::InputScalar("msgid",    ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",  ImGuiDataType_U8,  &fnccode);
        ImGui::InputScalar("RunMode",  ImGuiDataType_U8,
                           &command->adcsrunmodecmd.Payload.RunMode);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcsrunmodecmd, sizeof(ADCS_RunModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcsrunmodecmd, sizeof(ADCS_RunModeCmd_t));
        }
        break;
    }

    /* 71 : ADCS Set Control Mode (CC 25) */
    case 71: {
        static uint16_t msgid   = ADCS_CMD_ID;
        static uint8_t  fnccode = ADCS_SET_CONTROL_MODE_CC;   // 25

        ImGui::InputScalar("msgid",        ImGuiDataType_U16, &msgid);
        ImGui::InputScalar("fnccode",      ImGuiDataType_U8,  &fnccode);
        ImGui::InputScalar("ControlMode",  ImGuiDataType_U8,
                           &command->adcscontrolmodecmd.Payload.ControlMode);
        ImGui::InputScalar("ControlTimeout", ImGuiDataType_U16,
                           &command->adcscontrolmodecmd.Payload.Controltimeout);

        if (ImGui::Button("Generate CMD")) {
            WriteSystemName(msgid);

            mim::tc::EncodeCfeCommand(&command->adcscontrolmodecmd, sizeof(ADCS_ControlModeCmd_t), msgid, fnccode);
            mim::tc::SendTelecommand(&command->adcscontrolmodecmd, sizeof(ADCS_ControlModeCmd_t));
        }
        break;
    }



    // case 72: {
    //     static uint16_t msgid   = ADCS_CMD_ID;
    //     static uint8_t  fnccode = ADCS_SET_WHL_CONFIG_CC;

    //     ImGui::InputScalar("msgid",   ImGuiDataType_U16,  &msgid);
    //     ImGui::InputScalar("fnccode", ImGuiDataType_U8,   &fnccode);

    //     ImGui::InputScalar("Rwl0Inertia",      ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl0Inertia);
    //     ImGui::InputScalar("Rwl0MaxMomentum",  ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl0MaxMomentum);
    //     ImGui::InputScalar("Rwl0MaxToque",     ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl0MaxToque);

    //     ImGui::InputScalar("Rwl1Inertia",      ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl1Inertia);
    //     ImGui::InputScalar("Rwl1MaxMomentum",  ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl1MaxMomentum);
    //     ImGui::InputScalar("Rwl1MaxToque",     ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl1MaxToque);

    //     ImGui::InputScalar("Rwl2Inertia",      ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl2Inertia);
    //     ImGui::InputScalar("Rwl2MaxMomentum",  ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl2MaxMomentum);
    //     ImGui::InputScalar("Rwl2MaxToque",     ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl2MaxToque);

    //     ImGui::InputScalar("Rwl3Inertia",      ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl3Inertia);
    //     ImGui::InputScalar("Rwl3MaxMomentum",  ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl3MaxMomentum);
    //     ImGui::InputScalar("Rwl3MaxToque",     ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.Rwl3MaxToque);

    //     ImGui::InputScalar("WheelRampTorque",  ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.WheelRampTorque);

    //     ImGui::InputScalar("WheelScheme",      ImGuiDataType_U8,
    //                        &command->adcswhlconfigcmd.Payload.WheelScheme);
    //     ImGui::InputScalar("FailedWheelID",    ImGuiDataType_U8,
    //                        &command->adcswhlconfigcmd.Payload.FailedWheelID);

    //     ImGui::InputScalar("PyramidNominalMomentum",
    //                        ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.PyramidNominalMomentum);
    //     ImGui::InputScalar("PyramidTiltAngle",
    //                        ImGuiDataType_Float,
    //                        &command->adcswhlconfigcmd.Payload.PyramidTiltAngle);

    //     if (ImGui::Button("Generate CMD")) {
    //         WriteSystemName(msgid);
    //         uint16_t mid  = htons(msgid);
    //         uint8_t  seq[2] = {0xC0, 0x00};
    //         uint8_t  len[2] = {
    //             0x00, (uint8_t)(sizeof(ADCS_WhlConfigCmd_t) - 7)
    //         };

    //         memcpy(command->adcswhlconfigcmd.CmdHeader,     &mid, 2);
    //         memcpy(command->adcswhlconfigcmd.CmdHeader + 2, seq,  2);
    //         memcpy(command->adcswhlconfigcmd.CmdHeader + 4, len,  2);
    //         memcpy(command->adcswhlconfigcmd.CmdHeader + 6, &fnccode, 1);

    //         uint16_t total = sizeof(ADCS_WhlConfigCmd_t);
    //         const uint8_t *p =
    //             (const uint8_t *)&command->adcswhlconfigcmd;
    //         uint8_t crc = 0xFF;
    //         while (total--) crc ^= *(p++);

    //         memcpy(command->adcswhlconfigcmd.CmdHeader + 7, &crc, 1);

    //         packetsign *pkt = (packetsign *)malloc(
    //             2 + 2 + 4 + sizeof(ADCS_WhlConfigCmd_t));
    //         pkt->Identifier = HVD_TEST;
    //         pkt->PacketType = MIM_PT_TMTC_TEST;
    //         pkt->Length     = sizeof(ADCS_WhlConfigCmd_t);
    //         memcpy(pkt->Data, &command->adcswhlconfigcmd,
    //                sizeof(ADCS_WhlConfigCmd_t));

    //         pthread_join(p_thread[4], NULL);
    //         pthread_create(&p_thread[4], NULL,
    //                        task_uplink_onorbit, (void *)pkt);
    //     }
    //     break;
    // }
        case 72: {

            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_WHL_CONFIG_CC;


            static bool initialized = false;
            if (!initialized) {
                command->adcswhlconfigcmd.Payload.Rwl0Inertia        = 9.51015E-06 ;
                command->adcswhlconfigcmd.Payload.Rwl0MaxMomentum    = 0.00996 ;
                command->adcswhlconfigcmd.Payload.Rwl0MaxToque       = 0.002 ;

                command->adcswhlconfigcmd.Payload.Rwl1Inertia        = 9.51015E-06;
                command->adcswhlconfigcmd.Payload.Rwl1MaxMomentum    = 0.00996;
                command->adcswhlconfigcmd.Payload.Rwl1MaxToque       = 0.002;

                command->adcswhlconfigcmd.Payload.Rwl2Inertia        = 9.51015E-06;
                command->adcswhlconfigcmd.Payload.Rwl2MaxMomentum    = 0.00996;
                command->adcswhlconfigcmd.Payload.Rwl2MaxToque       = 0.002;

                command->adcswhlconfigcmd.Payload.Rwl3Inertia        = 9.51015E-06;
                command->adcswhlconfigcmd.Payload.Rwl3MaxMomentum    = 0.00996;
                command->adcswhlconfigcmd.Payload.Rwl3MaxToque       = 0.002;

                command->adcswhlconfigcmd.Payload.WheelRampTorque    = 0.00002;

                command->adcswhlconfigcmd.Payload.WheelScheme        = 3;
                command->adcswhlconfigcmd.Payload.FailedWheelID      = 0;

                command->adcswhlconfigcmd.Payload.PyramidNominalMomentum = 0.001;
                command->adcswhlconfigcmd.Payload.PyramidTiltAngle       = 26.565;

                initialized = true;
            }


            ImGui::Text("ADCS Wheel Config (fixed parameters)");
            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u", fnccode);
            ImGui::Separator();

            ImGui::Text("=== ADCS Wheel Config Parameters (Fixed) ===");
            ImGui::Separator();

            ImGui::Text("Rwl0Inertia          : %.6f", command->adcswhlconfigcmd.Payload.Rwl0Inertia);
            ImGui::Text("Rwl0MaxMomentum      : %.6f", command->adcswhlconfigcmd.Payload.Rwl0MaxMomentum);
            ImGui::Text("Rwl0MaxTorque        : %.6f", command->adcswhlconfigcmd.Payload.Rwl0MaxToque);

            ImGui::Text("Rwl1Inertia          : %.6f", command->adcswhlconfigcmd.Payload.Rwl1Inertia);
            ImGui::Text("Rwl1MaxMomentum      : %.6f", command->adcswhlconfigcmd.Payload.Rwl1MaxMomentum);
            ImGui::Text("Rwl1MaxTorque        : %.6f", command->adcswhlconfigcmd.Payload.Rwl1MaxToque);

            ImGui::Text("Rwl2Inertia          : %.6f", command->adcswhlconfigcmd.Payload.Rwl2Inertia);
            ImGui::Text("Rwl2MaxMomentum      : %.6f", command->adcswhlconfigcmd.Payload.Rwl2MaxMomentum);
            ImGui::Text("Rwl2MaxTorque        : %.6f", command->adcswhlconfigcmd.Payload.Rwl2MaxToque);

            ImGui::Text("Rwl3Inertia          : %.6f", command->adcswhlconfigcmd.Payload.Rwl3Inertia);
            ImGui::Text("Rwl3MaxMomentum      : %.6f", command->adcswhlconfigcmd.Payload.Rwl3MaxMomentum);
            ImGui::Text("Rwl3MaxTorque        : %.6f", command->adcswhlconfigcmd.Payload.Rwl3MaxToque);

            ImGui::Text("WheelRampTorque      : %.6f", command->adcswhlconfigcmd.Payload.WheelRampTorque);

            ImGui::Text("WheelScheme          : %u", command->adcswhlconfigcmd.Payload.WheelScheme);
            ImGui::Text("FailedWheelID        : %u", command->adcswhlconfigcmd.Payload.FailedWheelID);

            ImGui::Text("PyramidNominalMomentum : %.6f", command->adcswhlconfigcmd.Payload.PyramidNominalMomentum);
            ImGui::Text("PyramidTiltAngle       : %.6f", command->adcswhlconfigcmd.Payload.PyramidTiltAngle);

            ImGui::Separator();


            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                mim::tc::EncodeCfeCommand(&command->adcswhlconfigcmd, sizeof(ADCS_WhlConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcswhlconfigcmd, sizeof(ADCS_WhlConfigCmd_t));
            }
            break;
        }


        case 73: {
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_SATELLITE_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_SatConfigCmd_Payload_t &p = command->adcssatconfigcmd.Payload;

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

            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u",     fnccode);
            ImGui::Separator();

            ADCS_SatConfigCmd_Payload_t &p = command->adcssatconfigcmd.Payload;

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

                mim::tc::EncodeCfeCommand(&command->adcssatconfigcmd, sizeof(ADCS_SatConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcssatconfigcmd, sizeof(ADCS_SatConfigCmd_t));
            }

            break;
        }

        case 74: {
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_CONTROLLER_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_ControllerConfig_Payload_t &p = command->adcscontrollerconfigcmd.Payload;

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

            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u",     fnccode);
            ImGui::Separator();

            ADCS_ControllerConfig_Payload_t &p = command->adcscontrollerconfigcmd.Payload;

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

                mim::tc::EncodeCfeCommand(&command->adcscontrollerconfigcmd, sizeof(ADCS_ControllerConfig_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcscontrollerconfigcmd, sizeof(ADCS_ControllerConfig_t));
            }

            break;
        }

        case 75: { // ADCS Set Mag0 MMT Calib Config (CC 29)
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
                &command->adcsmag0mmtcalibconfigcmd.Payload;

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

                mim::tc::EncodeCfeCommand(&command->adcsmag0mmtcalibconfigcmd, sizeof(ADCS_Mag0MMTCalibConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsmag0mmtcalibconfigcmd, sizeof(ADCS_Mag0MMTCalibConfigCmd_t));
            }

            ImGui::Text("Header: %02X %02X %02X %02X %02X %02X %02X %02X",
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[0],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[1],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[2],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[3],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[4],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[5],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[6],
                        command->adcsmag0mmtcalibconfigcmd.CmdHeader[7]);

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






        case 76: {  
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_DEFAULT_MODE_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_DefaultModeConfigCmd_Payload_t &p =
                    command->adcsdefaultmodeconfigcmd.Payload;

                p.DefaultRunMode                   = 1; 
                p.DefaultOperationalState          = 0;  
                p.DefaultControlModeInOpStateSafe  = 1;  
                p.DefaultControlModeInOpStateAuto  = 13; 


                initialized = true;
            }

            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u", fnccode);
            ImGui::Separator();

            ADCS_DefaultModeConfigCmd_Payload_t &p =
                command->adcsdefaultmodeconfigcmd.Payload;

            ImGui::Text("=== Default Mode Settings ===");
            ImGui::Text("DefaultRunMode                  : %u", p.DefaultRunMode);
            ImGui::Text("DefaultOperationalState         : %u", p.DefaultOperationalState);
            ImGui::Text("DefaultControlModeInOpStateSafe : %u", p.DefaultControlModeInOpStateSafe);
            ImGui::Text("DefaultControlModeInOpStateAuto : %u", p.DefaultControlModeInOpStateAuto);

            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                mim::tc::EncodeCfeCommand(&command->adcsdefaultmodeconfigcmd, sizeof(ADCS_DefaultModeConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsdefaultmodeconfigcmd, sizeof(ADCS_DefaultModeConfigCmd_t));
            }

            break;
        }

        case 77: {  
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_MOUNTING_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_MountingConfigCmd_Payload_t &p =
                    command->adcsmountingconfigcmd.Payload;


                // ==== Stack & Actuators ====
                p.StackX_mounting = 3;
                p.StackY_mounting = 1;
                p.StackZ_mounting = 6;

                p.MTQ0_mounting = 2;
                p.MTQ1_mounting = 3;
                p.MTQ2_mounting = 5;

                p.Wheel0_mounting = 11;
                p.Wheel1_mounting = 12;
                p.Wheel2_mounting = 13;
                p.Wheel3_mounting = 14;

                // ==== Pyramid RWL angles (raw = deg * 100) ====
                p.PyramidRWL_alpha = 9000;
                p.PyramidRWL_beta  = 0;
                p.PyramidRWL_gamma = 18000;

                // ==== CSS Mountings ====
                p.CSS0_mounting = 5;
                p.CSS1_mounting = 0;
                p.CSS2_mounting = 0;
                p.CSS3_mounting = 0;
                p.CSS4_mounting = 0;
                p.CSS5_mounting = 1;
                p.CSS6_mounting = 3;
                p.CSS7_mounting = 2;
                p.CSS8_mounting = 4;
                p.CSS9_mounting = 6;

                // ==== FSS0..3 ====
                p.FSS0_alpha = 9000; p.FSS0_beta = 18000; p.FSS0_gamma = 0;
                p.FSS1_alpha = p.FSS1_beta = p.FSS1_gamma = 0;
                p.FSS2_alpha = p.FSS2_beta = p.FSS2_gamma = 0;
                p.FSS3_alpha = p.FSS3_beta = p.FSS3_gamma = 0;

                // ==== HSS0..1 ====
                p.HSS0_alpha = 0; p.HSS0_beta = -9000; p.HSS0_gamma = 0;
                p.HSS1_alpha = p.HSS1_beta = p.HSS1_gamma = 0;

                // ==== MAG0..1 ====
                p.MAG0_alpha = 0; p.MAG0_beta = 0; p.MAG0_gamma = 0;
                p.MAG1_alpha = p.MAG1_beta = p.MAG1_gamma = 0;

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

            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u", fnccode);
            ImGui::Separator();

            ADCS_MountingConfigCmd_Payload_t &p =
                command->adcsmountingconfigcmd.Payload;

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

                mim::tc::EncodeCfeCommand(&command->adcsmountingconfigcmd, sizeof(ADCS_MountingConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsmountingconfigcmd, sizeof(ADCS_MountingConfigCmd_t));
            }

            break;
        }


        case 79: {  
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_ESTIMATOR_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_EstimatorConfigCmd_Payload_t &p =
                    command->adcsestimatorconfigcmd.Payload;


                p.DefaultMainEstimatorMode   = 0;  
                p.DefaultBackupEstimatorMode = 0; 

                p.MAGMeasurementNoise  =0.001;
                p.CSSMeasurementNoise  =10.0;
                p.FSSMeasurementNoise = 0.0001;
                p.HSSMeasurementNoise = 0.0001;
                p.STRMeasurementNoise  = 1E-8;

                p.MMTRKFSystemNoise    = 0.0002;
                p.EKFSystemNoise       = 1E-9;
                p.NutationEpsilonCorrection = 4.243392E-05;
                p.NutationPsiCorrection     = 1.607126E-05;

                // // ---- Bit-fields ----
                // p.UseFSSinEKF = 1;
                // p.UseCSSinEKF = 0;
                // p.UseHSSinEKF = 1;
                // p.UseSTRinEKF = 0;

                // p.TriadVector1 = 1;   // 4-bit
                // p.TriadVector2 = 0;   // 4-bit
                // p.Spare        = 0;   // unused padding
//revise
                // ---- Bit-fields ----
                p.UseFSSinEKF = 1;
                p.UseCSSinEKF = 1;
                p.UseHSSinEKF = 0;
                p.UseSTRinEKF = 0;

                p.TriadVector1 = 1;   // 4-bit
                p.TriadVector2 = 0;   // 4-bit


                // ============================================================

                initialized = true;
            }

            // GUI 출력 (read-only)
            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u", fnccode);
            ImGui::Separator();

            ADCS_EstimatorConfigCmd_Payload_t &p =
                command->adcsestimatorconfigcmd.Payload;

            ImGui::Text("=== Default Estimator Modes ===");
            ImGui::Text("Main   : %u", p.DefaultMainEstimatorMode);
            ImGui::Text("Backup : %u", p.DefaultBackupEstimatorMode);

            ImGui::Separator();
            ImGui::Text("=== Measurement Noise ===");
            ImGui::Text("MAG : %.6f", p.MAGMeasurementNoise);
            ImGui::Text("CSS : %.6f", p.CSSMeasurementNoise);
            ImGui::Text("FSS : %.6f", p.FSSMeasurementNoise);
            ImGui::Text("HSS : %.6f", p.HSSMeasurementNoise);
            ImGui::Text("STR : %.6f", p.STRMeasurementNoise);

            ImGui::Separator();
            ImGui::Text("=== System Noise ===");
            ImGui::Text("MMT-RKF : %.6f", p.MMTRKFSystemNoise);
            ImGui::Text("EKF     : %.6f", p.EKFSystemNoise);

            ImGui::Separator();
            ImGui::Text("=== Nutation Correction ===");
            ImGui::Text("Epsilon : %.6f", p.NutationEpsilonCorrection);
            ImGui::Text("Psi     : %.6f", p.NutationPsiCorrection);

            ImGui::Separator();
            ImGui::Text("=== Flags ===");
            ImGui::Text("UseFSSinEKF : %u", p.UseFSSinEKF);
            ImGui::Text("UseCSSinEKF : %u", p.UseCSSinEKF);
            ImGui::Text("UseHSSinEKF : %u", p.UseHSSinEKF);
            ImGui::Text("UseSTRinEKF : %u", p.UseSTRinEKF);

            ImGui::Separator();
            ImGui::Text("TriadVector1 : %u", p.TriadVector1);
            ImGui::Text("TriadVector2 : %u", p.TriadVector2);
            // ImGui::Text("Spare        : %u", p.Spare);

            ImGui::Separator();


            // ============================ Generate CMD ============================
            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                mim::tc::EncodeCfeCommand(&command->adcsestimatorconfigcmd, sizeof(ADCS_EstimatorConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsestimatorconfigcmd, sizeof(ADCS_EstimatorConfigCmd_t));
            }

            break;
        }

case 80: {
    static const uint16_t msgid   = ADCS_CMD_ID;
    static const uint8_t  fnccode = ADCS_SET_SAT_ORBIT_PARAMS_CONFIG_CC;


    ADCS_SatOrbitParamConfigCmd_Payload_t &p =
        command->adcssatorbitparamconfigcmd.Payload;

    ImGui::Text("MsgID   : 0x%04X", msgid);
    ImGui::Text("FncCode : %u", fnccode);
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

        memcpy(command->adcssatorbitparamconfigcmd.CmdHeader + 0, &mid_be, 2);
        command->adcssatorbitparamconfigcmd.CmdHeader[2] = 0xC0;
        command->adcssatorbitparamconfigcmd.CmdHeader[3] = 0x00;
        memcpy(command->adcssatorbitparamconfigcmd.CmdHeader + 4, &ccsds_len_be, 2);
        memcpy(command->adcssatorbitparamconfigcmd.CmdHeader + 6, &fnccode, 1);
        command->adcssatorbitparamconfigcmd.CmdHeader[7] = 0x00;

        uint16_t total = (uint16_t)sizeof(ADCS_SatOrbitParamConfigCmd_t);
        const uint8_t *pp = (const uint8_t *)&command->adcssatorbitparamconfigcmd;
        uint8_t crc = 0xFF;
        while (total--) crc ^= *(pp++);
        command->adcssatorbitparamconfigcmd.CmdHeader[7] = crc;

        packetsign *pkt = (packetsign *)malloc(2 + 2 + 4 + sizeof(ADCS_SatOrbitParamConfigCmd_t));
        pkt->Identifier = HVD_TEST;
        pkt->PacketType = MIM_PT_TMTC_TEST;
        pkt->Length     = sizeof(ADCS_SatOrbitParamConfigCmd_t);

        memcpy(pkt->Data, &command->adcssatorbitparamconfigcmd, sizeof(ADCS_SatOrbitParamConfigCmd_t));

        mim::svc::WaitTx();
        mim::svc::SendUplink((void *)pkt);
    }

    break;
}




        case 81: { 
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_NODE_SELECTION_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {
                ADCS_NodeSelectionConfigCmd_Payload_t &p =
                    command->adcsnodeselectionconfigcmd.Payload;

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
                command->adcsnodeselectionconfigcmd.Payload;


            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u",     fnccode);
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

                mim::tc::EncodeCfeCommand(&command->adcsnodeselectionconfigcmd, sizeof(ADCS_NodeSelectionConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsnodeselectionconfigcmd, sizeof(ADCS_NodeSelectionConfigCmd_t));
            }

            break;
        }



        case 82: { 
            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_MTQ_CONFIG_CC;

            static bool initialized = false;
            if (!initialized) {

                ADCS_MTQConfigCmd_Payload_t &p =
                    command->adcsmtqconfigcmd.Payload;


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
                command->adcsmtqconfigcmd.Payload;

     
            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u",     fnccode);
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

                mim::tc::EncodeCfeCommand(&command->adcsmtqconfigcmd, sizeof(ADCS_MTQConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsmtqconfigcmd, sizeof(ADCS_MTQConfigCmd_t));
            }

            break;
        }

        case 85: { 

            static const uint16_t msgid   = ADCS_CMD_ID;
            static const uint8_t  fnccode = ADCS_SET_MAG_SENSING_ELM_CONFIG_CC;


            static bool initialized = false;
            if (!initialized) {

                ADCS_MagSensingElmConfigCmd_Payload_t &p =
                    command->adcsmagsensingelmconfigcmd.Payload;

                p.Mag0SensingElement = 0; 
                p.Mag1SensingElement = 0;  
                p.Spare = 0;              


                initialized = true;
            }

            // Payload alias
            ADCS_MagSensingElmConfigCmd_Payload_t &p =
                command->adcsmagsensingelmconfigcmd.Payload;


            ImGui::Text("MsgID   : 0x%04X", msgid);
            ImGui::Text("FncCode : %u", fnccode);
            ImGui::Separator();

            ImGui::Text("=== Magnetic Sensing Element Enable Flags ===");
            ImGui::Text("MAG0 Sensing Element : %u", p.Mag0SensingElement);
            ImGui::Text("MAG1 Sensing Element : %u", p.Mag1SensingElement);
            ImGui::Text("Spare Bits           : %u", p.Spare);
            ImGui::Separator();

            if (ImGui::Button("Generate CMD")) {

                WriteSystemName(msgid);

                // CCSDS Header (7바이트 + CRC)

                // uplink thread 실행
                mim::tc::EncodeCfeCommand(&command->adcsmagsensingelmconfigcmd, sizeof(ADCS_MagSensingElmConfigCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsmagsensingelmconfigcmd, sizeof(ADCS_MagSensingElmConfigCmd_t));
            }

            break;
        }


        case 86: {
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
                command->adcsunsolicittlmmsgsetupcmd.Payload;

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

                mim::tc::EncodeCfeCommand(&command->adcsunsolicittlmmsgsetupcmd, sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsunsolicittlmmsgsetupcmd, sizeof(ADCS_UnsolicitTlmMsgSetupCmd_t));
            }
            break;
        }




        case 88: // ADCS2_COMM_01_CC
        {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = 101;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("Flag_TLM Type", ImGuiDataType_U8,
                            &command->adcs101cmd.Payload.flag_tlmtype);

            ImGui::InputScalar("Flag_EST Mode", ImGuiDataType_U8,
                            &command->adcs101cmd.Payload.flag_estmode);

            ImGui::InputScalar("Flag_Cont Mode", ImGuiDataType_U8,
                            &command->adcs101cmd.Payload.flag_contmode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                mim::tc::EncodeCfeCommand(&command->adcs101cmd, sizeof(ADCS2_Comm01Cmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcs101cmd, sizeof(ADCS2_Comm01Cmd_t));
            }
            break;
        }


        case 89: // cc 43 ADCS_OPEN_LOOP_RW_CMD
        {
            static uint16_t msgid   = ADCS_CMD_ID;
            static uint8_t  fnccode = 43;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("Float1", ImGuiDataType_Float,
                            &command->adcsopenlooprwcmd.Payload.float1);

            ImGui::InputScalar("Float2", ImGuiDataType_Float,
                            &command->adcsopenlooprwcmd.Payload.float2);

            ImGui::InputScalar("Float3", ImGuiDataType_Float,
                            &command->adcsopenlooprwcmd.Payload.float3);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                mim::tc::EncodeCfeCommand(&command->adcsopenlooprwcmd, sizeof(ADCS_OpenLoopRWCmd_t), msgid, fnccode);
                mim::tc::SendTelecommand(&command->adcsopenlooprwcmd, sizeof(ADCS_OpenLoopRWCmd_t));
            }
            break;
        }



        // SC_NOOP_CC (0)
    default:
        return false;
    }
    return true;
}
