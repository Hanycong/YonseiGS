// View - 명령 생성기 입력 폼: BEE1000 SC 앱 (명령 번호 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152)
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
#include <mutex>


#include "cmdgen_forms.h"

using namespace mim;

extern StateCheckUnit State;
extern Command * command;
extern uint16_t gen_fnccode;
extern bool raw_data;

bool DrawBee1000ScForms(uint16_t fnccode)
{
    switch (fnccode)
    {
        case 136: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_NOOP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_NoopCmd_t) - 7)
                };

                memcpy(command->scnoopcmd.CmdHeader,     &mid, 2);
                memcpy(command->scnoopcmd.CmdHeader + 2, seq,  2);
                memcpy(command->scnoopcmd.CmdHeader + 4, len,  2);
                memcpy(command->scnoopcmd.CmdHeader + 6, &fnccode, 1);

                command->scnoopcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_NoopCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scnoopcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scnoopcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_NoopCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_NoopCmd_t);
                memcpy(pkt->Data, &command->scnoopcmd, sizeof(SC_NoopCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_RESET_COUNTERS_CC (1)
        case 137: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_RESET_COUNTERS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_ResetCountersCmd_t) - 7)
                };

                memcpy(command->scresetcounterscmd.CmdHeader,     &mid, 2);
                memcpy(command->scresetcounterscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scresetcounterscmd.CmdHeader + 4, len,  2);
                memcpy(command->scresetcounterscmd.CmdHeader + 6, &fnccode, 1);

                command->scresetcounterscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_ResetCountersCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scresetcounterscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scresetcounterscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_ResetCountersCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_ResetCountersCmd_t);
                memcpy(pkt->Data, &command->scresetcounterscmd,
                    sizeof(SC_ResetCountersCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_START_ATS_CC (2)
        case 138: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_START_ATS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("AtsNum", ImGuiDataType_U16,
                            &command->scstartatscmd.Payload.AtsNum);

            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scstartatscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_StartAtsCmd_t) - 7)
                };

                memcpy(command->scstartatscmd.CmdHeader,     &mid, 2);
                memcpy(command->scstartatscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstartatscmd.CmdHeader + 4, len,  2);
                memcpy(command->scstartatscmd.CmdHeader + 6, &fnccode, 1);

                command->scstartatscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_StartAtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstartatscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstartatscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StartAtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StartAtsCmd_t);
                memcpy(pkt->Data, &command->scstartatscmd,
                    sizeof(SC_StartAtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_STOP_ATS_CC (3)
        case 139: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_STOP_ATS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_StopAtsCmd_t) - 7)
                };

                memcpy(command->scstopatscmd.CmdHeader,     &mid, 2);
                memcpy(command->scstopatscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstopatscmd.CmdHeader + 4, len,  2);
                memcpy(command->scstopatscmd.CmdHeader + 6, &fnccode, 1);

                command->scstopatscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_StopAtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstopatscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstopatscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StopAtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StopAtsCmd_t);
                memcpy(pkt->Data, &command->scstopatscmd,
                    sizeof(SC_StopAtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 140: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_START_RTS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("RtsNum",  ImGuiDataType_U16,
                            &command->scstartrtscmd.Payload.RtsNum);
            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scstartrtscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SC_StartRtsCmd_t) - 7)};

                memcpy(command->scstartrtscmd.CmdHeader,     &mid, 2);
                memcpy(command->scstartrtscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstartrtscmd.CmdHeader + 4, len,  2);
                memcpy(command->scstartrtscmd.CmdHeader + 6, &fnccode, 1);

                command->scstartrtscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_StartRtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstartrtscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstartrtscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StartRtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StartRtsCmd_t);
                memcpy(pkt->Data, &command->scstartrtscmd,
                    sizeof(SC_StartRtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 141: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_STOP_RTS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("RtsNum",  ImGuiDataType_U16,
                            &command->scstoprtscmd.Payload.RtsNum);
            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scstoprtscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SC_StopRtsCmd_t) - 7)};

                memcpy(command->scstoprtscmd.CmdHeader,     &mid, 2);
                memcpy(command->scstoprtscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstoprtscmd.CmdHeader + 4, len,  2);
                memcpy(command->scstoprtscmd.CmdHeader + 6, &fnccode, 1);
        command->scstoprtscmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(SC_StopRtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstoprtscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstoprtscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StopRtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StopRtsCmd_t);
                memcpy(pkt->Data, &command->scstoprtscmd,
                    sizeof(SC_StopRtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        case 142: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_DISABLE_RTS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("RtsNum",  ImGuiDataType_U16,
                            &command->scdisablertscmd.Payload.RtsNum);
            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scdisablertscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SC_DisableRtsCmd_t) - 7)};

                memcpy(command->scdisablertscmd.CmdHeader,     &mid, 2);
                memcpy(command->scdisablertscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scdisablertscmd.CmdHeader + 4, len,  2);
                memcpy(command->scdisablertscmd.CmdHeader + 6, &fnccode, 1);
        command->scdisablertscmd.CmdHeader[7] = 0x00;

                uint16_t total = sizeof(SC_DisableRtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scdisablertscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scdisablertscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_DisableRtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_DisableRtsCmd_t);
                memcpy(pkt->Data, &command->scdisablertscmd,
                    sizeof(SC_DisableRtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        case 143: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_ENABLE_RTS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("RtsNum",  ImGuiDataType_U16,
                            &command->scenablertscmd.Payload.RtsNum);
            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scenablertscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SC_EnableRtsCmd_t) - 7)};

                memcpy(command->scenablertscmd.CmdHeader,     &mid, 2);
                memcpy(command->scenablertscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scenablertscmd.CmdHeader + 4, len,  2);
                memcpy(command->scenablertscmd.CmdHeader + 6, &fnccode, 1);

                command->scenablertscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_EnableRtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scenablertscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scenablertscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_EnableRtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_EnableRtsCmd_t);
                memcpy(pkt->Data, &command->scenablertscmd,
                    sizeof(SC_EnableRtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        // SC_SWITCH_ATS_CC (8)
        case 144: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_SWITCH_ATS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_SwitchAtsCmd_t) - 7)
                };

                memcpy(command->scswitchatscmd.CmdHeader,     &mid, 2);
                memcpy(command->scswitchatscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scswitchatscmd.CmdHeader + 4, len,  2);
                memcpy(command->scswitchatscmd.CmdHeader + 6, &fnccode, 1);

                command->scswitchatscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_SwitchAtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scswitchatscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scswitchatscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_SwitchAtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_SwitchAtsCmd_t);
                memcpy(pkt->Data, &command->scswitchatscmd,
                    sizeof(SC_SwitchAtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_JUMP_ATS_CC (9)
        case 145: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_JUMP_ATS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("NewTime", ImGuiDataType_U32,
                            &command->scjumpatscmd.Payload.NewTime);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_JumpAtsCmd_t) - 7)
                };

                memcpy(command->scjumpatscmd.CmdHeader,     &mid, 2);
                memcpy(command->scjumpatscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scjumpatscmd.CmdHeader + 4, len,  2);
                memcpy(command->scjumpatscmd.CmdHeader + 6, &fnccode, 1);

                command->scjumpatscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_JumpAtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scjumpatscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scjumpatscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_JumpAtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_JumpAtsCmd_t);
                memcpy(pkt->Data, &command->scjumpatscmd,
                    sizeof(SC_JumpAtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_CONTINUE_ATS_ON_FAILURE_CC (10)
        case 146: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_CONTINUE_ATS_ON_FAILURE_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("ContinueState", ImGuiDataType_U8,
                            &command->sccontinueatsonfailurecmd.Payload.ContinueState);
            ImGui::InputScalar("Padding",       ImGuiDataType_U16,
                            &command->sccontinueatsonfailurecmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00,
                    (uint8_t)(sizeof(SC_ContinueAtsOnFailureCmd_t) - 7)};

                memcpy(command->sccontinueatsonfailurecmd.CmdHeader,     &mid, 2);
                memcpy(command->sccontinueatsonfailurecmd.CmdHeader + 2, seq,  2);
                memcpy(command->sccontinueatsonfailurecmd.CmdHeader + 4, len,  2);
                memcpy(command->sccontinueatsonfailurecmd.CmdHeader + 6, &fnccode, 1);

                command->sccontinueatsonfailurecmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_ContinueAtsOnFailureCmd_t);
                const uint8_t *p =
                    (const uint8_t *)&command->sccontinueatsonfailurecmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->sccontinueatsonfailurecmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 +
                                        sizeof(SC_ContinueAtsOnFailureCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_ContinueAtsOnFailureCmd_t);
                memcpy(pkt->Data, &command->sccontinueatsonfailurecmd,
                    sizeof(SC_ContinueAtsOnFailureCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_APPEND_ATS_CC (11)
       
        case 147: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_APPEND_ATS_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("AtsNum",  ImGuiDataType_U16,
                            &command->scappendatscmd.Payload.AtsNum);
            ImGui::InputScalar("Padding", ImGuiDataType_U16,
                            &command->scappendatscmd.Payload.Padding);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid   = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {0x00, (uint8_t)(sizeof(SC_AppendAtsCmd_t) - 7)};

                memcpy(command->scappendatscmd.CmdHeader,     &mid, 2);
                memcpy(command->scappendatscmd.CmdHeader + 2, seq,  2);
                memcpy(command->scappendatscmd.CmdHeader + 4, len,  2);
                memcpy(command->scappendatscmd.CmdHeader + 6, &fnccode, 1);

                command->scappendatscmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_AppendAtsCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scappendatscmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scappendatscmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_AppendAtsCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_AppendAtsCmd_t);
                memcpy(pkt->Data, &command->scappendatscmd,
                    sizeof(SC_AppendAtsCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }


        // SC_MANAGE_TABLE_CC (12)
        case 148: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_MANAGE_TABLE_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("Parameter", ImGuiDataType_U32,
                            &command->scmanagetablecmd.Payload.Parameter);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_ManageTableCmd_t) - 7)
                };

                memcpy(command->scmanagetablecmd.CmdHeader,     &mid, 2);
                memcpy(command->scmanagetablecmd.CmdHeader + 2, seq,  2);
                memcpy(command->scmanagetablecmd.CmdHeader + 4, len,  2);
                memcpy(command->scmanagetablecmd.CmdHeader + 6, &fnccode, 1);

                command->scmanagetablecmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_ManageTableCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scmanagetablecmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scmanagetablecmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_ManageTableCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_ManageTableCmd_t);
                memcpy(pkt->Data, &command->scmanagetablecmd,
                    sizeof(SC_ManageTableCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_START_RTS_GRP_CC (13)
        case 149: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_START_RTS_GRP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("FirstRtsNum", ImGuiDataType_U16,
                            &command->scstartrtsgrpcmd.Payload.FirstRtsNum);
            ImGui::InputScalar("LastRtsNum", ImGuiDataType_U16,
                            &command->scstartrtsgrpcmd.Payload.LastRtsNum);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_StartRtsGrpCmd_t) - 7)
                };

                memcpy(command->scstartrtsgrpcmd.CmdHeader,     &mid, 2);
                memcpy(command->scstartrtsgrpcmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstartrtsgrpcmd.CmdHeader + 4, len,  2);
                memcpy(command->scstartrtsgrpcmd.CmdHeader + 6, &fnccode, 1);

                command->scstartrtsgrpcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_StartRtsGrpCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstartrtsgrpcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstartrtsgrpcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StartRtsGrpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StartRtsGrpCmd_t);
                memcpy(pkt->Data, &command->scstartrtsgrpcmd,
                    sizeof(SC_StartRtsGrpCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_STOP_RTS_GRP_CC (14)
        case 150: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_STOP_RTS_GRP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("FirstRtsNum", ImGuiDataType_U16,
                            &command->scstoprtsgrpcmd.Payload.FirstRtsNum);
            ImGui::InputScalar("LastRtsNum", ImGuiDataType_U16,
                            &command->scstoprtsgrpcmd.Payload.LastRtsNum);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_StopRtsGrpCmd_t) - 7)
                };

                memcpy(command->scstoprtsgrpcmd.CmdHeader,     &mid, 2);
                memcpy(command->scstoprtsgrpcmd.CmdHeader + 2, seq,  2);
                memcpy(command->scstoprtsgrpcmd.CmdHeader + 4, len,  2);
                memcpy(command->scstoprtsgrpcmd.CmdHeader + 6, &fnccode, 1);

                command->scstoprtsgrpcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_StopRtsGrpCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scstoprtsgrpcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scstoprtsgrpcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_StopRtsGrpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_StopRtsGrpCmd_t);
                memcpy(pkt->Data, &command->scstoprtsgrpcmd,
                    sizeof(SC_StopRtsGrpCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_DISABLE_RTS_GRP_CC (15)
        case 151: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_DISABLE_RTS_GRP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("FirstRtsNum", ImGuiDataType_U16,
                            &command->scdisablertsgrpcmd.Payload.FirstRtsNum);
            ImGui::InputScalar("LastRtsNum", ImGuiDataType_U16,
                            &command->scdisablertsgrpcmd.Payload.LastRtsNum);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_DisableRtsGrpCmd_t) - 7)
                };

                memcpy(command->scdisablertsgrpcmd.CmdHeader,     &mid, 2);
                memcpy(command->scdisablertsgrpcmd.CmdHeader + 2, seq,  2);
                memcpy(command->scdisablertsgrpcmd.CmdHeader + 4, len,  2);
                memcpy(command->scdisablertsgrpcmd.CmdHeader + 6, &fnccode, 1);

                command->scdisablertsgrpcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_DisableRtsGrpCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scdisablertsgrpcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scdisablertsgrpcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_DisableRtsGrpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_DisableRtsGrpCmd_t);
                memcpy(pkt->Data, &command->scdisablertsgrpcmd,
                    sizeof(SC_DisableRtsGrpCmd_t));

                mim::svc::WaitTx();
                mim::svc::SendUplink((void *)pkt);
            }
            break;
        }

        // SC_ENABLE_RTS_GRP_CC (16)
        case 152: {
            static uint16_t msgid   = SC_CMD_MID;
            static uint8_t  fnccode = SC_ENABLE_RTS_GRP_CC;

            ImGui::InputScalar("msgid",   ImGuiDataType_U16, &msgid);
            ImGui::InputScalar("fnccode", ImGuiDataType_U8,  &fnccode);

            ImGui::InputScalar("FirstRtsNum", ImGuiDataType_U16,
                            &command->scenablertsgrpcmd.Payload.FirstRtsNum);
            ImGui::InputScalar("LastRtsNum", ImGuiDataType_U16,
                            &command->scenablertsgrpcmd.Payload.LastRtsNum);

            if (ImGui::Button("Generate CMD")) {
                WriteSystemName(msgid);

                uint16_t mid = htons(msgid);
                uint8_t  seq[2] = {0xC0, 0x00};
                uint8_t  len[2] = {
                    0x00,
                    (uint8_t)(sizeof(SC_EnableRtsGrpCmd_t) - 7)
                };

                memcpy(command->scenablertsgrpcmd.CmdHeader,     &mid, 2);
                memcpy(command->scenablertsgrpcmd.CmdHeader + 2, seq,  2);
                memcpy(command->scenablertsgrpcmd.CmdHeader + 4, len,  2);
                memcpy(command->scenablertsgrpcmd.CmdHeader + 6, &fnccode, 1);

                command->scenablertsgrpcmd.CmdHeader[7] = 0;
                uint16_t total = sizeof(SC_EnableRtsGrpCmd_t);
                const uint8_t *p = (const uint8_t *)&command->scenablertsgrpcmd;
                uint8_t crc = 0xFF;
                while (total--) crc ^= *(p++);

                memcpy(command->scenablertsgrpcmd.CmdHeader + 7, &crc, 1);

                packetsign *pkt =
                    (packetsign *)malloc(2 + 2 + 4 + sizeof(SC_EnableRtsGrpCmd_t));
                pkt->Identifier = HVD_TEST;
                pkt->PacketType = MIM_PT_TMTC_TEST;
                pkt->Length     = sizeof(SC_EnableRtsGrpCmd_t);
                memcpy(pkt->Data, &command->scenablertsgrpcmd,
                    sizeof(SC_EnableRtsGrpCmd_t));

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
