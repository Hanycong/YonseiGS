// View - 명령 생성기 입력 폼: BEE1012 ltrx (명령 번호 3400, 3401, 3402, 3403, 3410, 3411, 3412, 3440, 3441)
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
    {  3400, LTRX_CMD_ID, LTRX_NOOP_CC,                      "##direct_3400" },   // LTRX NOOP
    {  3401, LTRX_CMD_ID, LTRX_RESET_COUNTERS_CC,            "##direct_3401" },   // LTRX RESET_COUNTERS
    {  3402, LTRX_CMD_ID, LTRX_RESET_APP_CMD_COUNTERS_CC,    "##direct_3402" },   // LTRX RESET_APP_CMD_COUNTERS
    {  3403, LTRX_CMD_ID, LTRX_RESET_DEVICE_CMD_COUNTERS_CC, "##direct_3403" },   // LTRX RESET_DEVICE_CMD_COUNTERS
    {  3410, LTRX_CMD_ID, LTRX_SESSION_START_DOWNLINK_CC,    "##direct_3410" },   // LTRX SESSION_START_DOWNLINK
    {  3411, LTRX_CMD_ID, LTRX_SESSION_ABORT_CC,             "##direct_3411" },   // LTRX SESSION_ABORT
    {  3412, LTRX_CMD_ID, LTRX_SESSION_RESET_STATE_CC,       "##direct_3412" },   // LTRX SESSION_RESET_STATE
    {  3440, LTRX_CMD_ID, LTRX_DOWNSTREAM_ENABLE_CC,         "##direct_3440" },   // LTRX DOWNSTREAM_ENABLE
    {  3441, LTRX_CMD_ID, LTRX_DOWNSTREAM_DISABLE_CC,        "##direct_3441" },   // LTRX DOWNSTREAM_DISABLE
};

bool DrawBee1012LtrxForms(uint16_t fnccode)
{
    return mim::ui::DrawNoArgForm(kNoArgForms, fnccode);
}
