#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "../util/common.h"

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
    // Meyers Singleton：首次调用时按 thread_count 创建线程，后续调用返回同一实例
    static ThreadPool& getInstance(size_t thread_count = 0)
    {
        static ThreadPool instance(thread_count);
        return instance;
    }

    ThreadPool(const ThreadPool&)            = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&)                 = delete;
    ThreadPool& operator=(ThreadPool&&)      = delete;

    ~ThreadPool();

    static void Entry(ThreadPool* master);

    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda);
    void wait() const;

    void  addTask(Task* task);
    Task* getTask();

private:
    explicit ThreadPool(size_t thread_count);

    std::vector<std::thread> m_threads;
    std::queue<Task*>        m_tasks;
    std::mutex               m_lock;
    std::atomic<bool>        m_alive;
    std::atomic<size_t>      m_pending_task_count;
};
} // namespace tcpr

#endif // __THREAD_POOL_H__