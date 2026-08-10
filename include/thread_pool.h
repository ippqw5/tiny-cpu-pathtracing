#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

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
    ThreadPool(size_t threadCount = 0);
    ~ThreadPool();

    void parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda);
    void wait() const;

    void  addTask(Task* task);
    Task* getTask();

private:
    std::vector<std::thread> mThreads;
    std::queue<Task*>        mTasks;
    std::mutex               mLock;
    std::atomic<bool>        mAlive;
    std::atomic<size_t>      mPendingTaskCount;
};

} // namespace tcpr

#endif // __THREAD_POOL_H__