// Model - 수신 데이터 저장 파일 이름
#include "telemetry_files.h"

#include <time.h>
#include <filesystem>
#include <system_error>

FILE * OpenTelemetryFile(const char * dir, const char * prefix, const char * suffix,
                         const struct timeval * rx_time, const char * mode, std::string * path)
{
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);   // 실패해도 fopen 결과로 판단

    struct timeval tv;
    if (rx_time)
        tv = *rx_time;
    else
        gettimeofday(&tv, NULL);

    time_t sec = tv.tv_sec;
    struct tm local;
    localtime_r(&sec, &local);

    char stamp[64];
    snprintf(stamp, sizeof(stamp), "%04d-%02d-%02d-%02d-%02d-%02d-%03d",
             local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
             local.tm_hour, local.tm_min, local.tm_sec, (int)(tv.tv_usec / 1000));

    const std::string base = std::string(dir) + "/" + prefix + stamp;
    std::string name = base + suffix;
    // 같은 밀리초에 저장한 파일이 있으면 번호를 붙여 덮어쓰지 않는다
    for (int n = 1; std::filesystem::exists(name, ec) && n < 1000; n++)
        name = base + "_" + std::to_string(n) + suffix;

    if (path) *path = name;
    return fopen(name.c_str(), mode);
}
