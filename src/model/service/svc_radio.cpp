#include "svc_radio.h"
#include "svc_thread.h"

#include <unistd.h>
#include <gs/param/rparam.h>

#include "miman_config.h"
#include "miman_csp.h"
#include "miman_coms.h"
#include "miman_radial.h"

extern StateCheckUnit State;
extern Console console;
extern Setup * setup;

namespace mim {
namespace svc {

/********************************************5차 추가***************************************************/
// (miman_imgui.cpp 에서 이동)

static const char* sat_names[3] = { "BEE1000", "BEE1012", "Uelysys" };
static const char* hmac_keys[3] = { "BEE1000", "BEE1012", "UELYSYS" };
static bool is_switching = false; // 상태 체크를 위함

static void* SwitchSatelliteTask(void* arg)
{
    int mode = *(int*)arg;
    uint8_t target_node = setup->gs100_node; // 지상국의 gs100 노드

    char target_key[32] = { 0 };

    console.AddLog("Changing GS HMAC Key for %s...", sat_names[mode]); // 테스트 텍스트, 후에 수정가능

    // RX 업데이트
    gs_rparam_set_int8(target_node, 1, 0x0010, 0xB00B, 100, 1); //100은 수정 가능, 1은 테이블 번호 (rx table)
    gs_rparam_set_string(target_node, 1, 0x0020, 0xB00B, 100, target_key, strlen(target_key) + 1);

    // TX 업데이트
    gs_rparam_set_int8(target_node, 5, 0x0010, 0xB00B, 100, 1);
    gs_rparam_set_string(target_node, 5, 0x0020, 0xB00B, 100, target_key, strlen(target_key) + 1);

    console.AddLog("[OK]## Satellite Switched to: %s", sat_names[mode]);

    is_switching = false; // 통신이 끝나면 플래그 해제 (다시 버튼 클릭 가능)
    return NULL;
}

/***********************************************************************************************************/

int LoadParamTable(int table)
{
    switch (table)
    {
    case 0: return StartThread(SLOT_RADIO_CONTROL, &Load_Paramtable0, NULL);
    case 1: return StartThread(SLOT_RADIO_CONTROL, &Load_Paramtable1, NULL);
    case 5: return StartThread(SLOT_RADIO_CONTROL, &Load_Paramtable5, NULL);
    }
    return -1;
}

int SaveParamTable(int table)
{
    switch (table)
    {
    case 0: return StartThread(SLOT_RADIO_CONTROL, &Save_Paramtable0, NULL);
    case 1: return StartThread(SLOT_RADIO_CONTROL, &Save_Paramtable1, NULL);
    case 5: return StartThread(SLOT_RADIO_CONTROL, &Save_Paramtable5, NULL);
    }
    return -1;
}

int SaveParamToFRAM(void * param_id)
{
    return StartThread(SLOT_RADIO_CONTROL, &Save2FRAM, param_id);
}

void StartBaudCalibration()
{
    JoinThread(SLOT_CALIBRATION);
    StartThread(SLOT_CALIBRATION, csp_baud_calibration, NULL);
}

void StartFreqCalibration()
{
    JoinThread(SLOT_CALIBRATION);
    StartThread(SLOT_CALIBRATION, csp_freq_calibration, NULL);
}

int StartSignalTest()
{
    return StartThread(SLOT_SIGNALTEST, &SignalTest, NULL);
}

void StopSignalTest()
{
    State.Signaltest = false;
    JoinThread(SLOT_SIGNALTEST);
}

void AmpOn()
{
    JoinThread(SLOT_AMP_TIMER);
    pid_t pid_on = fork();

    if (pid_on == -1) {
        console.AddLog("[ERROR]##Failed to fork process");
    }
    else if (pid_on == 0) {
        // Child process
        execl("../amp/ampcontrol", "ampcontrol", NULL);

    }
    else
    {
        console.AddLog("[OK]##Forked Safe Amplifier Process.(ON, Timer)");
        StartThread(SLOT_AMP_TIMER, AmpTimer, NULL);
    }
}

void AmpOff()
{
    pid_t pid_off = fork();

    if (pid_off == -1) {
        console.AddLog("[ERROR]##Failed to fork process");
    }
    else if (pid_off == 0) {
        // Child process
        execl("../amp/ampoff", "ampoff", NULL);
    }
    else
    {
        console.AddLog("[OK]##Forked Safe Amplifier Process. (OFF)");
    }
    State.AMPON = false;
}

const char * SwitchSatelliteName(int mode)
{
    return sat_names[mode];
}

void SwitchSatellite(int * mode_arg)
{
    JoinThread(SLOT_SWITCH_SAT);
    StartThread(SLOT_SWITCH_SAT, SwitchSatelliteTask, (void*)mode_arg);
}

} // namespace svc
} // namespace mim
