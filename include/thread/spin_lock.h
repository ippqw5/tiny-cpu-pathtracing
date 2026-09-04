#ifndef __SPIN_LOCK_H__
#define __SPIN_LOCK_H__

#include "../util/common.h"

namespace tcpr
{

class SpinLock
{
public:
    void acquire();

    void release();

private:
    std::atomic_flag m_flag;
};

class Guard
{
public:
    Guard(SpinLock& spin_lock);
    ~Guard();

private:
    SpinLock& m_spin_lock;
};

} // namespace tcpr

#endif // __SPIN_LOCK_H__