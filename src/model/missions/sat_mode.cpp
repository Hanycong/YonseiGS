#include "sat_mode.h"

#include <atomic>

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

} // namespace sat
} // namespace mim
