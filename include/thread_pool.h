#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "common.h"

namespace tcpr
{

class Task
{
public:
    virtual void run() = 0;
};

class ThreadPool
{
public:
    static void Entry(ThreadPool* master);
    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();

    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda);
    void wait() const;

    void  addTask(Task* task);
    Task* getTask();

private:
    std::vector<std::thread> m_threads;
    std::queue<Task*>        m_tasks;
    std::mutex               m_lock;
    std::atomic<bool>        m_alive;
    std::atomic<size_t>      m_pending_task_count;
};

} // namespace tcpr

#endif // __THREAD_POOL_H__