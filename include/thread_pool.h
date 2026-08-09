#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <atomic>
#include <list>
#include <mutex>
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
    static void Entry(ThreadPool *master);
    ThreadPool(size_t threadCount = 0);
    ~ThreadPool();

    void wait() const
    {
        while (mTasks.empty() == false)
        {
            std::this_thread::yield();
        }
    }

    void addTask(Task *task);
    Task *getTask();

  private:
    std::vector<std::thread> mThreads;
    std::list<Task *> mTasks;
    std::mutex mLock;
    std::atomic<bool> mAlive;
};

} // namespace tcpr

#endif // __THREAD_POOL_H__