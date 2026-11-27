// View - 명령 생성기 입력 폼: UELYSYS payuel_yonsei (명령 번호 7600, 7601, 7607)
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
    {  7600, 0x18CC, 0, "##direct_7600" },   // payuel_yonsei NOOP
    {  7601, 0x18CC, 1, "##direct_7601" },   // payuel_yonsei RESET_COUNTERS
};

bool DrawUelysysPayuelYonseiForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 7607: { // payuel_yonsei FORWARD_SB
            static uint16_t msgid=0x18CC; static uint8_t fnccode=7; struct Payload { uint16_t TargetMid; uint16_t PayloadLength; uint8_t TargetFcnCode; uint8_t TargetNode; uint8_t TargetPort; uint8_t Spare[1]; uint8_t PayloadBytes[192]; }; static_assert(sizeof(Payload)==200,"payuel_yonsei FORWARD_SB payload size"); static Payload payload{};
            ImGui::InputScalar("msgid##direct_7607",ImGuiDataType_U16,&msgid); ImGui::InputScalar("fnccode##direct_7607",ImGuiDataType_U8,&fnccode); ImGui::InputScalar("TargetMid##direct_7607",ImGuiDataType_U16,&payload.TargetMid); ImGui::InputScalar("PayloadLength##direct_7607",ImGuiDataType_U16,&payload.PayloadLength); ImGui::InputScalar("TargetFcnCode##direct_7607",ImGuiDataType_U8,&payload.TargetFcnCode); ImGui::InputScalar("TargetNode##direct_7607",ImGuiDataType_U8,&payload.TargetNode); ImGui::InputScalar("TargetPort##direct_7607",ImGuiDataType_U8,&payload.TargetPort); ImGui::InputText("Payload (ASCII, max 191)##direct_7607",reinterpret_cast<char*>(payload.PayloadBytes),sizeof(payload.PayloadBytes));
            if(ImGui::Button("Generate CMD##direct_7607")){ WriteSystemName(msgid); uint8_t cmd[208]={0}; memcpy(cmd+8,&payload,sizeof(payload)); mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode); mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST); } break;
        }

        // ==================== UELYSYS payuel_roma ====================
    default:
        return false;
    }
    return true;
}
