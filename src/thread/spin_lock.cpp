#include "thread/spin_lock.h"

namespace tcpr
{

void SpinLock::acquire()
{
    while (m_flag.test_and_set(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }
}

void SpinLock::release()
{
    m_flag.clear(std::memory_order_release);
}

Guard::Guard(SpinLock& spin_lock) : m_spin_lock(spin_lock)
{
    m_spin_lock.acquire();
}

Guard::~Guard()
{
    m_spin_lock.release();
}

} // namespace tcpr