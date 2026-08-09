#include "thread_pool.h"

namespace tcpr
{
void ThreadPool::Entry(ThreadPool *master)
{
    while (master->mAlive)
    {
        Task *task = master->getTask();
        if (task)
            task->run();
        else
            std::this_thread::yield();
    }
}

ThreadPool::ThreadPool(size_t threadCount)
{
    mAlive = true;
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

    for (auto &thread : mThreads)
    {
        thread.join();
    }
    mThreads.clear();
}

void ThreadPool::addTask(Task *task)
{
    std::lock_guard<std::mutex> guard(mLock);
    mTasks.push_back(task);
}

Task *ThreadPool::getTask()
{
    std::lock_guard<std::mutex> guard(mLock);
    if (mTasks.empty())
        return nullptr;

    Task *task = mTasks.front();
    mTasks.pop_front();
    return task;
}

} // namespace tcpr
