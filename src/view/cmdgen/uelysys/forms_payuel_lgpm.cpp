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
    {  7000, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_NOOP_CC,              "", mim::ui::NoArgCommandForm::kTitled },   // 230 : LGPM NOOP (CC 0, No-Arg)
    {  7001, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_RESET_COUNTERS_CC,    "", mim::ui::NoArgCommandForm::kTitled },   // 231 : LGPM Reset Counter (CC 1, No-Arg)
    {  7003, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_3V3_PWR_ON_CC,        "", mim::ui::NoArgCommandForm::kTitled },   // 233 : LGPM 3V3_PWR_ON (CC 3, No-Arg)
    {  7004, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_3V3_PWR_OFF_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 234 : LGPM 3V3_PWR_OFF (CC 4, No-Arg)
    {  7005, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_MAIN_BOOST_SW_ON_CC,  "", mim::ui::NoArgCommandForm::kTitled },   // 235 : LGPM MAIN_BOOST_SW_ON (CC 5, No-Arg)
    {  7006, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_MAIN_BOOST_SW_OFF_CC, "", mim::ui::NoArgCommandForm::kTitled },   // 236 : LGPM MAIN_BOOST_SW_OFF (CC 6, No-Arg)
    {  7007, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_SUB_BOOST_SW_ON_CC,   "", mim::ui::NoArgCommandForm::kTitled },   // 237 : LGPM SUB_BOOST_SW_ON (CC 7, No-Arg)
    {  7008, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_SUB_BOOST_SW_OFF_CC,  "", mim::ui::NoArgCommandForm::kTitled },   // 238 : LGPM SUB_BOOST_SW_OFF (CC 8, No-Arg)
    {  7009, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V28_MAIN_ON_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 239 : LGPM V28_MAIN_ON (CC 9, No-Arg)
    {  7010, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V28_MAIN_OFF_CC,      "", mim::ui::NoArgCommandForm::kTitled },   // 240 : LGPM V28_MAIN_OFF (CC 10, No-Arg)
    {  7011, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V28_SUB_ON_CC,        "", mim::ui::NoArgCommandForm::kTitled },   // 241 : LGPM V28_SUB_ON (CC 11, No-Arg)
    {  7012, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V28_SUB_OFF_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 242 : LGPM V28_SUB_OFF (CC 12, No-Arg)
    {  7013, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V12_MAIN_ON_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 243 : LGPM V12_MAIN_ON (CC 13, No-Arg)
    {  7014, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_V12_MAIN_OFF_CC,      "", mim::ui::NoArgCommandForm::kTitled },   // 244 : LGPM V12_MAIN_OFF (CC 14, No-Arg)
    {  7015, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_PWR_SENSE_INFO_CC,    "", mim::ui::NoArgCommandForm::kTitled },   // 245 : LGPM PWR_SENSE_INFO (CC 15, No-Arg)
    {  7016, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_PWR_SEQ_ON_CC,        "", mim::ui::NoArgCommandForm::kTitled },   // 246 : LGPM PWR_SEQ_ON (CC 16, No-Arg)
    {  7017, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_PWR_SEQ_OFF_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 247 : LGPM PWR_SEQ_OFF (CC 17, No-Arg)
    {  7021, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_RWA_PWR_ON_CC,        "", mim::ui::NoArgCommandForm::kTitled },   // 251 : LGPM RWA_PWR_ON (CC 21, No-Arg)
    {  7022, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_RWA_PWR_OFF_CC,       "", mim::ui::NoArgCommandForm::kTitled },   // 252 : LGPM RWA_PWR_OFF (CC 22, No-Arg)
    {  7023, PAYUEL_LGPM_CMD_ID, PAYUEL_LGPM_RWA_SENSE_INFO_CC,    "", mim::ui::NoArgCommandForm::kTitled },   // 253 : LGPM RWA_SENSE_INFO (CC 23, No-Arg)
};

bool DrawUelysysPayuelLgpmForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
    case 7002: { /* 232 : LGPM MCU_ALIVE_CHECK (CC 2, With Payload) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_MCU_ALIVE_CHECK_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);
        ImGui::InputScalar("Ground UTC Time", ImGuiDataType_U32,
            &command5->lgpmmcualivecheckcmd.Ground_UTC_Time);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);


            mim::tc::EncodeCfeCommand(&command5->lgpmmcualivecheckcmd, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t), msgid, fnccode);


            mim::tc::SendTelecommand(&command5->lgpmmcualivecheckcmd, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t), MIM_PT_TMTC_TEST);
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


            mim::tc::EncodeCfeCommand(&command5->lgpmrwacontrolidx1cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t), msgid, fnccode);


            mim::tc::SendTelecommand(&command5->lgpmrwacontrolidx1cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx1_Cmd_t), MIM_PT_TMTC_TEST);
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


            mim::tc::EncodeCfeCommand(&command5->lgpmrwacontrolidx2cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t), msgid, fnccode);


            mim::tc::SendTelecommand(&command5->lgpmrwacontrolidx2cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx2_Cmd_t), MIM_PT_TMTC_TEST);
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


            mim::tc::EncodeCfeCommand(&command5->lgpmrwacontrolidx3cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t), msgid, fnccode);


            mim::tc::SendTelecommand(&command5->lgpmrwacontrolidx3cmd, sizeof(PAYUEL_LGPM_RWA_CONTROL_idx3_Cmd_t), MIM_PT_TMTC_TEST);
        }
        break;
    }

    default:
        return false;
    }
    return true;
}
