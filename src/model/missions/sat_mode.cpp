#include "sat_mode.h"

#include <atomic>
#include <ctype.h>
#include <string.h>

namespace mim {
namespace sat {

static const SatProfile kProfiles[SAT_MODE_COUNT] = {
    { "BEE1000", "BEE1000", false },
    { "BEE1012", "BEE1012", true  },
    { "UELYSYS", "UELYSYS", true  },
    { "SD1-SAT", "SD1SAT",  true  },
};

// GUI 스레드가 바꾸고 TX/RX 스레드가 읽는다
static std::atomic<int> current_mode(SAT_BEE1000);

const SatProfile & Profile(int mode)
{
    if (mode < 0 || mode >= SAT_MODE_COUNT)
        return kProfiles[SAT_BEE1000];
    return kProfiles[mode];
}

int Current()
{
    return current_mode.load();
}

void SetCurrent(int mode)
{
    if (mode >= 0 && mode < SAT_MODE_COUNT)
        current_mode.store(mode);
}

const char * CurrentKey()
{
    return Profile(Current()).key;
}

int ModeFromSatName(const char * name)
{
    if (name == NULL)
        return -1;

    // 영문자/숫자만 남기고 대문자로 ("BEE-1000 " -> "BEE1000", "SD-1" -> "SD1")
    char n[32];
    size_t len = 0;
    for (const char * c = name; *c != '\0' && len < sizeof(n) - 1; c++)
    {
        if (isalnum((unsigned char)*c))
            n[len++] = (char)toupper((unsigned char)*c);
    }
    n[len] = '\0';

    if (strcmp(n, "BEE1000") == 0) return SAT_BEE1000;
    if (strcmp(n, "BEE1012") == 0) return SAT_BEE1012;
    if (strcmp(n, "UELYSYS") == 0) return SAT_UELYSYS;
    if (strcmp(n, "SD1") == 0 || strcmp(n, "SD1SAT") == 0) return SAT_SD1SAT;
    return -1;
}

} // namespace sat
} // namespace mim
