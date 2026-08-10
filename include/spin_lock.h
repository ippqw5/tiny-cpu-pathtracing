#ifndef __SPIN_LOCK_H__
#define __SPIN_LOCK_H__

#include <atomic>
#include <thread>

namespace tcpr
{

class SpinLock
{
public:
    void acquire()
    {
        while (mFlag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    void release()
    {
        mFlag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag mFlag;
};

class Guard
{
public:
    Guard(SpinLock& spinLock) : mSpinLock(spinLock)
    {
        mSpinLock.acquire();
    }

    ~Guard()
    {
        mSpinLock.release();
    }

private:
    SpinLock& mSpinLock;
};

} // namespace tcpr

#endif // __SPIN_LOCK_H__