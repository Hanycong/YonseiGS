// View - 명령 생성기 입력 폼: UELYSYS gpio (명령 번호 5804, 5805, 5806, 5807, 5808, 5809, 5810, 5811, 5812, 5813)
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
    {  5804, 0x182B, 4,  "##direct_5804" },   // gpio STX_PAY_HIGH
    {  5805, 0x182B, 5,  "##direct_5805" },   // gpio STX_PAY_LOW
    {  5806, 0x182B, 6,  "##direct_5806" },   // gpio LGE_MCU_RESET_HIGH
    {  5807, 0x182B, 7,  "##direct_5807" },   // gpio LGE_MCU_RESET_LOW
    {  5808, 0x182B, 8,  "##direct_5808" },   // gpio STX_BUS_HIGH
    {  5809, 0x182B, 9,  "##direct_5809" },   // gpio STX_BUS_LOW
    {  5810, 0x182B, 10, "##direct_5810" },   // gpio ADCS_EN_HIGH
    {  5811, 0x182B, 11, "##direct_5811" },   // gpio ADCS_EN_LOW
    {  5812, 0x182B, 12, "##direct_5812" },   // gpio ADCS_BOOT_HIGH
    {  5813, 0x182B, 13, "##direct_5813" },   // gpio ADCS_BOOT_LOW
};

bool DrawUelysysGpioForms(uint16_t fnccode)
{
    return mim::ui::DrawNoArgForm(kNoArgForms, fnccode);
}
