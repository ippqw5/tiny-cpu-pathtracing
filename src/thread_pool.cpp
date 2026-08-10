#include "thread_pool.h"
#include <cstddef>

namespace tcpr
{
void ThreadPool::Entry(ThreadPool* master)
{
    while (master->mAlive)
    {
        Task* task = master->getTask();
        if (task)
        {
            task->run();
            master->mPendingTaskCount--;
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool(size_t threadCount)
{
    mAlive = true;
    mPendingTaskCount = 0;
    if (threadCount == 0)
    {
        // set to the maximum of hardware cores
        threadCount = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < threadCount; i++)
    {
        mThreads.push_back(std::thread(ThreadPool::Entry, this));
    }
}

ThreadPool::~ThreadPool()
{
    wait();

    mAlive = false;

    for (auto& thread : mThreads)
    {
        thread.join();
    }
    mThreads.clear();
}

class ParallelForTask : public Task
{
public:
    ParallelForTask(size_t x, size_t y, const std::function<void(size_t, size_t)>& lambda) : x(x), y(y), lambda(lambda)
    {
    }

    void run() override
    {
        lambda(x, y);
    }

private:
    size_t                              x, y;
    std::function<void(size_t, size_t)> lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            addTask(new ParallelForTask(x, y, lambda));
        }
    }
}

void ThreadPool::wait() const
{
    while (mPendingTaskCount > 0)
    {
        std::this_thread::yield();
    }
}

void ThreadPool::addTask(Task* task)
{
    std::lock_guard<std::mutex> guard(mLock);
    mPendingTaskCount++;
    mTasks.push(task);
}

Task* ThreadPool::getTask()
{
    std::lock_guard<std::mutex> guard(mLock);
    if (mTasks.empty())
        return nullptr;

    Task* task = mTasks.front();
    mTasks.pop();
    return task;
}

} // namespace tcpr
