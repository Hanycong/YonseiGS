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



            mim::tc::EncodeCfeCommand(&command5->lgpmnoopcmd, sizeof(PAYUEL_LGPM_NoopCmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmnoopcmd, sizeof(PAYUEL_LGPM_NoopCmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmresetcountercmd, sizeof(PAYUEL_LGPM_ResetCountersCmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmresetcountercmd, sizeof(PAYUEL_LGPM_ResetCountersCmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmmcualivecheckcmd, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmmcualivecheckcmd, sizeof(PAYUEL_LGPM_MCU_ALIVE_CHECK_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpm3v3pwroncmd, sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpm3v3pwroncmd, sizeof(PAYUEL_LGPM_3V3_PWR_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpm3v3pwroffcmd, sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpm3v3pwroffcmd, sizeof(PAYUEL_LGPM_3V3_PWR_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmmainboostswoncmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmmainboostswoncmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmmainboostswoffcmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmmainboostswoffcmd, sizeof(PAYUEL_LGPM_MAIN_BOOST_SW_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmsubboostswoncmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmsubboostswoncmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmsubboostswoffcmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmsubboostswoffcmd, sizeof(PAYUEL_LGPM_SUB_BOOST_SW_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv28mainoncmd, sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv28mainoncmd, sizeof(PAYUEL_LGPM_V28_MAIN_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv28mainoffcmd, sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv28mainoffcmd, sizeof(PAYUEL_LGPM_V28_MAIN_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv28suboncmd, sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv28suboncmd, sizeof(PAYUEL_LGPM_V28_SUB_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv28suboffcmd, sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv28suboffcmd, sizeof(PAYUEL_LGPM_V28_SUB_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv12mainoncmd, sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv12mainoncmd, sizeof(PAYUEL_LGPM_V12_MAIN_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmv12mainoffcmd, sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmv12mainoffcmd, sizeof(PAYUEL_LGPM_V12_MAIN_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmpwrsenseinfocmd, sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmpwrsenseinfocmd, sizeof(PAYUEL_LGPM_PWR_SENSE_INFO_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmpwrseqoncmd, sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmpwrseqoncmd, sizeof(PAYUEL_LGPM_PWR_SEQ_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmpwrseqoffcmd, sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmpwrseqoffcmd, sizeof(PAYUEL_LGPM_PWR_SEQ_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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

    case 7021: { /* 251 : LGPM RWA_PWR_ON (CC 21, No-Arg) */
        static uint16_t msgid = PAYUEL_LGPM_CMD_ID;
        static uint8_t fnccode = PAYUEL_LGPM_RWA_PWR_ON_CC;

        ImGui::InputScalar("Msg ID", ImGuiDataType_U16, &msgid, NULL, NULL ,"0x%04X");
        ImGui::InputScalar("Command Code(CC)", ImGuiDataType_U8, &fnccode);

        if (ImGui::Button("Generate CMD")){
            WriteSystemName(msgid);



            mim::tc::EncodeCfeCommand(&command5->lgpmrwapwroncmd, sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmrwapwroncmd, sizeof(PAYUEL_LGPM_RWA_PWR_ON_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmrwapwroffcmd, sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmrwapwroffcmd, sizeof(PAYUEL_LGPM_RWA_PWR_OFF_Cmd_t), MIM_PT_TMTC_TEST);
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



            mim::tc::EncodeCfeCommand(&command5->lgpmrwasenseinfocmd, sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t), msgid, fnccode);



            mim::tc::SendTelecommand(&command5->lgpmrwasenseinfocmd, sizeof(PAYUEL_LGPM_RWA_SENSE_INFO_Cmd_t), MIM_PT_TMTC_TEST);
        }
        break;
    }

    default:
        return false;
    }
    return true;
}
