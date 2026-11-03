#include "tc_log.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <filesystem>
#include <string>

#include "YonseiGS_config.h"
#include "YonseiGS_coms.h"      // packetsign
#include "sat_mode.h"

extern FILE * log_ptr;
extern Console console;

namespace mim {
namespace svc {

static pthread_mutex_t tc_log_lock = PTHREAD_MUTEX_INITIALIZER;

void LockTcLog()   { pthread_mutex_lock(&tc_log_lock); }
void UnlockTcLog() { pthread_mutex_unlock(&tc_log_lock); }

bool SwitchTcLogFile(int mode)
{
    std::string dir = (mode == mim::sat::SAT_BEE1000)
        ? std::string("../log/TC_Log")
        : std::string("../data/") + mim::sat::Profile(mode).key + "/tc_log";

    char name[64];
    time_t now = time(0);
    struct tm local;
    localtime_r(&now, &local);
    snprintf(name, sizeof(name), "/log--%04d-%02d-%02d-%02d-%02d-%02d--",
             local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec)
        return false;
    FILE * fp = fopen((dir + name).c_str(), "wb");
    if (fp == NULL)
        return false;

    LockTcLog();
    FILE * old = log_ptr;
    log_ptr = fp;
    UnlockTcLog();
    if (old != NULL)
        fclose(old);
    return true;
}

void CloseTcLogFile()
{
    LockTcLog();
    FILE * old = log_ptr;
    log_ptr = NULL;
    UnlockTcLog();
    if (old != NULL)
        fclose(old);
}

void RecordTcPacket(const void * packet_, const char * sat_key)
{
    const packetsign * packet = (const packetsign *)packet_;
    if (packet == NULL || sat_key == NULL)
        return;

    std::string dir = std::string("../data/") + sat_key + "/tc_log/packet";
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm local;
    localtime_r(&tv.tv_sec, &local);
    char stamp[48];
    snprintf(stamp, sizeof(stamp), "%04d-%02d-%02d-%02d-%02d-%02d",
             local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
    // 같은 초에 여러 명령을 보내도 덮어쓰지 않게 밀리초까지 붙인다
    char name[96];
    snprintf(name, sizeof(name), "/TCpacketLog--%s-%03ld.txt", stamp, (long)(tv.tv_usec / 1000));

    FILE * fp = fopen((dir + name).c_str(), "w");
    if (fp == NULL)
    {
        console.AddLog("[ERROR]##Failed to create TC packet log in %s", dir.c_str());
        return;
    }

    const uint8_t * d = packet->Data;
    const int len = packet->Length;
    fprintf(fp, "======================TC Packet======================");
    fprintf(fp, "\n[TimeStamp] : %s\n", stamp);
    fprintf(fp, "\n[Identifier] : %u\n", packet->Identifier);
    fprintf(fp, "\n[PacketType] : %u\n", packet->PacketType);
    fprintf(fp, "\n[Length] : %u\n", packet->Length);
    fprintf(fp, "\n[Data]\n");
    if (len >= 8)
    {
        fprintf(fp, "mid : 0x%02x%02x\n", d[0], d[1]);
        fprintf(fp, "seq : 0x%02x%02x\n", d[2], d[3]);
        fprintf(fp, "len : 0x%02x%02x\n", d[4], d[5]);
        fprintf(fp, "cc : %u\n", d[6]);
        fprintf(fp, "crc : 0x%02x\n", d[7]);
        fprintf(fp, "data :\n");
        for (int i = 0; i < len - 8; i++)
        {
            fprintf(fp, "0x%02x ", d[i + 8]);
            if (i % 10 == 0 && i != 0) fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n======================CCSDS RAW bytes======================\n");
    for (int i = 0; i < len; i++)
    {
        fprintf(fp, "%02x ", d[i]);
        if (i % 10 == 0 && i != 0) fprintf(fp, "\n");
    }
    fclose(fp);
}

} // namespace svc
} // namespace mim
