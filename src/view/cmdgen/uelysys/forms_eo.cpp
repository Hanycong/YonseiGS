// View - 명령 생성기 입력 폼: UELYSYS eo (명령 번호 4800, 4801, 4802, 4803, 4804, 4806)
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
    {  4800, 0x1890, 0, "##direct_4800" },   // eo NOOP (CC0)
    {  4801, 0x1890, 1, "##direct_4801" },   // eo RESET_COUNTER (CC1)
    {  4802, 0x1890, 2, "##direct_4802" },   // eo RESET_PHASE (CC2)
    {  4803, 0x1890, 3, "##direct_4803" },   // eo SEND_STATE (CC3)
    {  4804, 0x1890, 4, "##direct_4804" },   // eo FINISH_PHASE (CC4)
};

bool DrawUelysysEoForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 4806: { // eo START_CHILD_TASK (CC6)
            static uint16_t msgid=0x1890; static uint8_t fnccode=6;
            struct __attribute__((packed)) Payload { uint32_t ArmMagic; uint16_t SequenceId; uint16_t Reserved; };
            static_assert(sizeof(Payload)==8,"eo START_CHILD_TASK payload size");
            static Payload payload{0x454F4152u,1u,0u};
            ImGui::InputScalar("msgid##direct_4806",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_4806",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("ArmMagic##direct_4806",ImGuiDataType_U32,&payload.ArmMagic); ImGui::InputScalar("SequenceId##direct_4806",ImGuiDataType_U16,&payload.SequenceId);
            if(ImGui::Button("Generate CMD##direct_4806")){ WriteSystemName(msgid); uint8_t cmd[16]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); }
            break;
        }

        // ==================== UELYSYS payuel_aos ====================
    default:
        return false;
    }
    return true;
}
