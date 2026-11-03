#include "svc_radio.h"
#include "svc_thread.h"

#include <unistd.h>
#include <gs/param/rparam.h>

#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_radial.h"
#include "sat_mode.h"
#include "tc_log.h"

#include <atomic>
#include <stdint.h>

extern StateCheckUnit State;
extern Console console;
extern Setup * setup;

namespace mim {
namespace svc {

// ---- 위성 전환 (GS100 HMAC 키 + TC 로그 폴더). 누리호 5차 GS 의 SwitchSatelliteTask ----
// BEE1000 은 TX HMAC 을 끄고, 나머지는 위성 키(16바이트, 남는 자리는 0)를 TX 테이블(5)에 쓴다.
// RX 테이블(1)은 건드리지 않는다.
static std::atomic<bool> switching(false);

static void* SwitchSatelliteTask(void* arg)
{
    const int mode = (int)(intptr_t)arg;
    const mim::sat::SatProfile & profile = mim::sat::Profile(mode);
    const uint8_t node = setup->gs100_node;

    console.AddLog("Changing GS HMAC Key for %s...", profile.name);

    gs_error_t err;
    if (!profile.hmac)
    {
        err = gs_rparam_set_int8(node, 5, 0x0010, 0xB00B, 100, 0);
    }
    else
    {
        uint8_t key[0x10] = { 0 };
        size_t len = strlen(profile.key);
        memcpy(key, profile.key, len > sizeof(key) ? sizeof(key) : len);
        err = gs_rparam_set_int8(node, 5, 0x0010, 0xB00B, 100, 1);
        if (err == GS_OK)
            err = gs_rparam_set(node, 5, 0x0020, GS_PARAM_DATA, 0xB00B, 100, key, sizeof(key));
    }

    if (!SwitchTcLogFile(mode))
        console.AddLog("[ERROR]##Failed to switch TC log file for %s", profile.name);

    if (err == GS_OK)
        console.AddLog("[OK]## Satellite Switched to: %s (TX HMAC %s)", profile.name, profile.hmac ? "enabled" : "disabled");
    else
        console.AddLog("[ERROR]##GS100 HMAC setting failed for %s (%s). Command list switched, but check the GS100 HMAC key.",
                       profile.name, gs_error_string(err));

    switching = false;
    return NULL;
}

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
    LinkFlags.Signaltest = false;
    JoinThread(SLOT_SIGNALTEST);
}

static const char * AMP_DEV = "/dev/ttyACM0"; // ampcontrol.cpp / ampoff.cpp 의 DEVNAME 과 같아야 한다

bool AmpReady(const char * program)
{
    if (access(AMP_DEV, F_OK) != 0)
    {
        console.AddLog("[ERROR]##AMP not connected: %s not found. Check the AMP (Arduino) USB cable.", AMP_DEV);
        return false;
    }
    if (access(AMP_DEV, R_OK | W_OK) != 0)
    {
        console.AddLog("[ERROR]##AMP device %s: permission denied. Run with sudo or add user to dialout.", AMP_DEV);
        return false;
    }
    char path[64];
    snprintf(path, sizeof(path), "../amp/%s", program);
    if (access(path, X_OK) != 0)
    {
        console.AddLog("[ERROR]##AMP program %s is missing or not executable (chmod +x).", path);
        return false;
    }
    return true;
}

void AmpOn()
{
    if (!AmpReady("ampcontrol"))
        return;
    JoinThread(SLOT_AMP_TIMER);
    pid_t pid_on = fork();

    if (pid_on == -1) {
        console.AddLog("[ERROR]##Failed to fork process");
    }
    else if (pid_on == 0) {
        // Child process
        execl("../amp/ampcontrol", "ampcontrol", NULL);
        // exec 실패 시 자식이 GS 복제본으로 계속 돌면 X 연결이 깨져 본체까지 죽는다
        perror("execl ../amp/ampcontrol");
        _exit(127);

    }
    else
    {
        console.AddLog("[OK]##Forked Safe Amplifier Process.(ON, Timer)");
        StartThread(SLOT_AMP_TIMER, AmpTimer, NULL);
    }
}

void AmpOff()
{
    if (!AmpReady("ampoff"))
    {
        LinkFlags.AMPON = false;
        return;
    }
    pid_t pid_off = fork();

    if (pid_off == -1) {
        console.AddLog("[ERROR]##Failed to fork process");
    }
    else if (pid_off == 0) {
        // Child process
        execl("../amp/ampoff", "ampoff", NULL);
        // exec 실패 시 자식이 GS 복제본으로 계속 돌면 X 연결이 깨져 본체까지 죽는다
        perror("execl ../amp/ampoff");
        _exit(127);
    }
    else
    {
        console.AddLog("[OK]##Forked Safe Amplifier Process. (OFF)");
    }
    LinkFlags.AMPON = false;
}

const char * SwitchSatelliteName(int mode)
{
    return mim::sat::Profile(mode).name;
}

bool SwitchSatellite(int mode)
{
    if (mode < 0 || mode >= mim::sat::SAT_MODE_COUNT)
        return false;
    // 앞 전환이 GS100 설정 중이면 기다리지 않고 거절한다 (GUI 스레드에서 join 하지 않기 위해)
    if (switching.exchange(true))
    {
        console.AddLog("[ERROR]##Satellite switch is still in progress. Try again.");
        return false;
    }
    mim::sat::SetCurrent(mode);
    JoinThread(SLOT_SWITCH_SAT);        // 앞 작업은 끝났다 (switching 이 false 였음)
    if (StartThread(SLOT_SWITCH_SAT, SwitchSatelliteTask, (void*)(intptr_t)mode) != 0)
    {
        switching = false;
        console.AddLog("[ERROR]##Failed to start satellite switch task.");
        return false;
    }
    return true;
}

} // namespace svc
} // namespace mim
