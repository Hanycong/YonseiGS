// View - 명령 생성기 입력 폼: BEE1012 mission (명령 번호 3200, 3201, 3202)
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
    {  3200, BEE1012_MISSION_CMD_ID, BEE1012_MISSION_NOOP_CC,          "##direct_3200" },
    {  3201, BEE1012_MISSION_CMD_ID, BEE1012_MISSION_RESET_COUNTER_CC, "##direct_3201" },   // MISSION RESET_COUNTER
    {  3202, BEE1012_MISSION_CMD_ID, BEE1012_MISSION_APPS_PERM_OFF_CC, "##direct_3202" },   // MISSION APPS_PERM_OFF
};

bool DrawBee1012MissionForms(uint16_t fnccode)
{
    return mim::ui::DrawNoArgForm(kNoArgForms, fnccode);
}
