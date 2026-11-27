// View - 명령 생성기 입력 폼: UELYSYS sp (명령 번호 8200, 8201, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209)
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
    {  8200, 0x1858, 0, "##direct_8200" },   // sp NOOP
    {  8201, 0x1858, 1, "##direct_8201" },   // sp RESET_COUNTERS
    {  8208, 0x1858, 8, "##direct_8208" },   // sp ALL_STOP_BURN
};

bool DrawUelysysSpForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 8202: { // sp GET_AR6_STATUS
            static uint16_t msgid=0x1858; static uint8_t fnccode=2; static uint8_t Ar6Addr=0; ImGui::InputScalar("msgid##direct_8202",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8202",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Ar6Addr##direct_8202",ImGuiDataType_U8,&Ar6Addr); if(ImGui::Button("Generate CMD##direct_8202")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; cmd[8]=Ar6Addr; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(2)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8203: { // sp DEPLOY
            static uint16_t msgid=0x1858; static uint8_t fnccode=3; struct __attribute__((packed)) Payload{uint8_t Ar6Addr;uint8_t BurnDuration;}; static_assert(sizeof(Payload)==2,"sp DEPLOY payload size"); static Payload payload{}; ImGui::InputScalar("msgid##direct_8203",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8203",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Ar6Addr##direct_8203",ImGuiDataType_U8,&payload.Ar6Addr); ImGui::InputScalar("BurnDuration##direct_8203",ImGuiDataType_U8,&payload.BurnDuration); if(ImGui::Button("Generate CMD##direct_8203")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; memcpy(cmd+8,&payload,2); mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(3)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8204: { // sp STOP_BURN
            static uint16_t msgid=0x1858; static uint8_t fnccode=4; static uint8_t Ar6Addr=0; ImGui::InputScalar("msgid##direct_8204",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8204",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Ar6Addr##direct_8204",ImGuiDataType_U8,&Ar6Addr); if(ImGui::Button("Generate CMD##direct_8204")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; cmd[8]=Ar6Addr; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(2)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8205: { // sp AUTO_DEPLOY
            static uint16_t msgid=0x1858; static uint8_t fnccode=5; struct __attribute__((packed)) Payload{uint8_t StartBurnTime;uint8_t Increment;uint8_t MaxBurnTime;}; static_assert(sizeof(Payload)==3,"sp AUTO_DEPLOY payload size"); static Payload payload{}; ImGui::InputScalar("msgid##direct_8205",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8205",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("StartBurnTime##direct_8205",ImGuiDataType_U8,&payload.StartBurnTime); ImGui::InputScalar("Increment##direct_8205",ImGuiDataType_U8,&payload.Increment); ImGui::InputScalar("MaxBurnTime##direct_8205",ImGuiDataType_U8,&payload.MaxBurnTime); if(ImGui::Button("Generate CMD##direct_8205")){ WriteSystemName(msgid); uint8_t cmd[11]={0}; memcpy(cmd+8,&payload,3); mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(4)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8206: { // sp SET_AR6_ADDR
            static uint16_t msgid=0x1858; static uint8_t fnccode=6; struct __attribute__((packed)) Payload{uint8_t CurrentAddr;uint8_t NewAddr;}; static_assert(sizeof(Payload)==2,"sp SET_AR6_ADDR payload size"); static Payload payload{}; ImGui::InputScalar("msgid##direct_8206",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8206",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("CurrentAddr##direct_8206",ImGuiDataType_U8,&payload.CurrentAddr); ImGui::InputScalar("NewAddr##direct_8206",ImGuiDataType_U8,&payload.NewAddr); if(ImGui::Button("Generate CMD##direct_8206")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; memcpy(cmd+8,&payload,2); mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(3)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8207: { // sp SCAN_AR6
            static uint16_t msgid=0x1858; static uint8_t fnccode=7; struct __attribute__((packed)) Payload{uint8_t StartAddr;uint8_t StopAddr;}; static_assert(sizeof(Payload)==2,"sp SCAN_AR6 payload size"); static Payload payload{}; ImGui::InputScalar("msgid##direct_8207",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8207",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("StartAddr##direct_8207",ImGuiDataType_U8,&payload.StartAddr); ImGui::InputScalar("StopAddr##direct_8207",ImGuiDataType_U8,&payload.StopAddr); if(ImGui::Button("Generate CMD##direct_8207")){ WriteSystemName(msgid); uint8_t cmd[10]={0}; memcpy(cmd+8,&payload,2); mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(3)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }
        case 8209: { // sp GET_AR6_STATUS
            static uint16_t msgid=0x1858; static uint8_t fnccode=9; static uint8_t Ar6Addr=0; ImGui::InputScalar("msgid##direct_8209",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_8202",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("Ar6Addr##direct_8202",ImGuiDataType_U8,&Ar6Addr); if(ImGui::Button("Generate CMD##direct_8202")){ WriteSystemName(msgid); uint8_t cmd[9]={0}; cmd[8]=Ar6Addr; mim::tc::EncodeCfeCommand(cmd, sizeof(cmd), msgid, fnccode, (uint8_t)(2)); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);} break;
        }

        // ==================== UELYSYS stx ====================
    default:
        return false;
    }
    return true;
}
