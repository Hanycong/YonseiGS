// View - 명령 생성기 입력 폼: BEE1012 gpio (명령 번호 2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2210, 2211, 2212, 2213, 2214, 2215, 2216, 2217)
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
    {  2200, 0x182B,      0,                     "##direct_2200" },   // gpio NOOP
    {  2201, 0x182B,      1,                     "##direct_2201" },   // gpio RESET_COUNTERS
    {  2202, 0x182B,      2,                     "##direct_2202" },   // gpio PROCESS
    {  2204, 0x182B,      4,                     "##direct_2204" },   // gpio LTRX_EN_ON
    {  2205, 0x182B,      5,                     "##direct_2205" },   // gpio LTRX_EN_OFF
    {  2206, GPIO_CMD_ID, GPIO_DEP1_EN_ON_CC,    ""              },
    {  2207, GPIO_CMD_ID, GPIO_DEP1_EN_OFF_CC,   ""              },
    {  2208, GPIO_CMD_ID, GPIO_DEP2_EN_ON_CC,    ""              },
    {  2209, GPIO_CMD_ID, GPIO_DEP2_EN_OFF_CC,   ""              },
    {  2210, GPIO_CMD_ID, GPIO_SP_IN_READ_5S_CC, ""              },
    {  2211, 0x182B,      11,                    "##direct_2211" },   // gpio STX_EN_ON
    {  2212, 0x182B,      12,                    "##direct_2212" },   // gpio STX_EN_OFF
    {  2213, 0x182B,      13,                    "##direct_2213" },   // gpio ADCS_EN_ON
    {  2214, 0x182B,      14,                    "##direct_2214" },   // gpio ADCS_EN_OFF
    {  2215, 0x182B,      15,                    "##direct_2215" },   // gpio ADCS_BOOT_ON
    {  2216, 0x182B,      16,                    "##direct_2216" },   // gpio ADCS_BOOT_OFF
};

bool DrawBee1012GpioForms(uint16_t fnccode)
{
    if (mim::ui::DrawNoArgForm(kNoArgForms, fnccode))
        return true;

    switch (fnccode)
    {
        case 2203: { // gpio DIGPIOLAY_PARAM
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=3;
            struct Payload { uint32_t ValU32; int16_t ValI16; char ValStr[10]; };
            static_assert(sizeof(Payload)==16,"gpio DIGPIOLAY_PARAM payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2203",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2203",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("ValU32##direct_2203",ImGuiDataType_U32,&payload.ValU32);
            ImGui::InputScalar("ValI16##direct_2203",ImGuiDataType_S16,&payload.ValI16);
            ImGui::InputText("ValStr##direct_2203",payload.ValStr,sizeof(payload.ValStr));
            if(ImGui::Button("Generate CMD##direct_2203")){
                WriteSystemName(msgid); uint8_t cmd[24]={0};
                memcpy(cmd+8,&payload,sizeof(payload));
                   mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                 mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }


        case 2217: { // gpio DEP_BURN
            static uint16_t msgid=0x182B;
            static uint8_t fnccode=17;
            struct Payload { uint8_t Channel; uint8_t Reserved[3]; uint32_t BurnTimeSeconds; };
            static_assert(sizeof(Payload)==8,"gpio DEP_BURN payload size");
            static Payload payload{};
            ImGui::InputScalar("msgid##direct_2217",ImGuiDataType_U16,&msgid);
            ImGui::InputScalar("fnccode##direct_2217",ImGuiDataType_U8,&fnccode);
            ImGui::InputScalar("Channel##direct_2217",ImGuiDataType_U8,&payload.Channel);
            ImGui::InputScalar("BurnTimeSeconds##direct_2217",ImGuiDataType_U32,&payload.BurnTimeSeconds);
            if(ImGui::Button("Generate CMD##direct_2217")){
                WriteSystemName(msgid); uint8_t cmd[16]={0};
                memcpy(cmd+8,&payload,sizeof(payload));
                   mim::tc::EncodeCfeCommand16(cmd, sizeof(cmd), msgid, fnccode);
                 mim::tc::SendTelecommand(cmd, sizeof(cmd), MIM_PT_TMTC_TEST);
            }
            break;
        }

        // ==================== pay_slt ====================
    default:
        return false;
    }
    return true;
}
