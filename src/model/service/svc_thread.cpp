#include "svc_thread.h"

#include <atomic>

// 실체는 main.cpp 에 있다. (전역 정리 단계 전까지 위치 유지)
extern pthread_t p_thread[16];
static_assert(sizeof(p_thread) / sizeof(p_thread[0]) == mim::svc::SLOT_COUNT, "p_thread 크기와 Slot 정의가 다름");

namespace mim {
namespace svc {

// StartThread 로 만들고 아직 join 하지 않은 슬롯
static std::atomic<bool> joinable[SLOT_COUNT];
// ThreadHandle 로 밖에 넘긴 슬롯 (만들기/join 을 밖에서 하므로 추적하지 않는다)
static std::atomic<bool> external[SLOT_COUNT];

int StartThread(Slot slot, ThreadFn fn, void * arg)
{
    int rc = pthread_create(&p_thread[slot], NULL, fn, arg);
    if (rc == 0)
        joinable[slot] = true;
    return rc;
}

int JoinThread(Slot slot)
{
    if (external[slot])
        return pthread_join(p_thread[slot], NULL);
    if (!joinable[slot].exchange(false))
        return 0;
    return pthread_join(p_thread[slot], NULL);
}

pthread_t * ThreadHandle(Slot slot)
{
    external[slot] = true;
    return &p_thread[slot];
}

} // namespace svc
} // namespace mim
