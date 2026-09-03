#include "thread_pool.h"

namespace tcpr
{
void ThreadPool::Entry(ThreadPool* master)
{
    while (master->m_alive)
    {
        Task* task = master->getTask();
        if (task)
        {
            task->run();
            master->m_pending_task_count--;
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count)
{
    m_alive = true;
    m_pending_task_count = 0;
    if (thread_count == 0)
    {
        // set to the maximum of hardware cores
        thread_count = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        m_threads.push_back(std::thread(ThreadPool::Entry, this));
    }
}

ThreadPool::~ThreadPool()
{
    wait();

    m_alive = false;

    for (auto& thread : m_threads)
    {
        thread.join();
    }
    m_threads.clear();
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
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            addTask(new ParallelForTask(x, y, lambda));
        }
    }
}

void ThreadPool::wait() const
{
    while (m_pending_task_count > 0)
    {
        std::this_thread::yield();
    }
}

void ThreadPool::addTask(Task* task)
{
    std::lock_guard<std::mutex> guard(m_lock);
    m_pending_task_count++;
    m_tasks.push(task);
}

Task* ThreadPool::getTask()
{
    std::lock_guard<std::mutex> guard(m_lock);
    if (m_tasks.empty())
        return nullptr;

    Task* task = m_tasks.front();
    m_tasks.pop();
    return task;
}

} // namespace tcpr
