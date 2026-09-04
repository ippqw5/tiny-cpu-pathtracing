#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "../util/common.h"

namespace tcpr
{

class Task
{
public:
    virtual ~Task() = default;

    virtual void run() = 0;

    /**
     * Destroy the task object once it has finished running.
     *
     * Default implementation: `delete this`.
     * Override to customize lifetime management (e.g. return the task to a pool).
     */
    virtual void destroy()
    {
        delete this;
    }
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

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool();

    static void Entry(ThreadPool* master);

    /** Run `lambda(x, y)` for the whole area, one task per `chunk_size x chunk_size` block. */
    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda,
                     size_t chunk_size = 32);
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

#endif // THREAD_POOL_H