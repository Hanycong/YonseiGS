#include "svc_thread.h"

// 실체는 main.cpp 에 있다. (전역 정리 단계 전까지 위치 유지)
extern pthread_t p_thread[16];
static_assert(sizeof(p_thread) / sizeof(p_thread[0]) == mim::svc::SLOT_COUNT, "p_thread 크기와 Slot 정의가 다름");

namespace mim {
namespace svc {

int StartThread(Slot slot, ThreadFn fn, void * arg)
{
    return pthread_create(&p_thread[slot], NULL, fn, arg);
}

int JoinThread(Slot slot)
{
    return pthread_join(p_thread[slot], NULL);
}

pthread_t * ThreadHandle(Slot slot)
{
    return &p_thread[slot];
}

} // namespace svc
} // namespace mim
