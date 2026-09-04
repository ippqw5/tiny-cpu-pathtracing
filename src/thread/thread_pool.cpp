#include "thread/thread_pool.h"

namespace tcpr
{

void ThreadPool::Entry(ThreadPool* master)
{
    while (master->m_alive)
    {
        Task* task = master->getTask();
        if (task != nullptr)
        {
            task->run();
            task->destroy();
            master->m_pending_task_count--;
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count) : m_alive(true), m_pending_task_count(0)
{

    if (thread_count == 0)
    {
        // set to the maximum of hardware cores
        thread_count = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        m_threads.emplace_back(ThreadPool::Entry, this);
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

/**
 * A task covering a 2D rectangular chunk of pixels: `[x_begin, x_end) x [y_begin, y_end)`.
 *
 * On `run()`, the lambda is invoked once for every pixel in the chunk,
 * so one task replaces `chunk_size * chunk_size` per-pixel tasks.
 */
class ParallelForTask : public Task
{
public:
    ParallelForTask(size_t x_begin, size_t x_end, size_t y_begin, size_t y_end, const std::function<void(size_t, size_t)>& lambda)
        : m_x_begin(x_begin), m_x_end(x_end), m_y_begin(y_begin), m_y_end(y_end), m_lambda(lambda)
    {
    }

    void run() override
    {
        for (size_t x = m_x_begin; x < m_x_end; x++)
        {
            for (size_t y = m_y_begin; y < m_y_end; y++)
            {
                m_lambda(x, y);
            }
        }
    }

private:
    size_t                              m_x_begin, m_x_end;
    size_t                              m_y_begin, m_y_end;
    std::function<void(size_t, size_t)> m_lambda;
};

void ThreadPool::parallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda, size_t chunk_size)
{
    // ceiling division: chunks per axis (edge chunks may be smaller)
    const size_t chunk_x_count = (width + chunk_size - 1) / chunk_size;
    const size_t chunk_y_count = (height + chunk_size - 1) / chunk_size;

    for (size_t chunk_y = 0; chunk_y < chunk_y_count; chunk_y++)
    {
        for (size_t chunk_x = 0; chunk_x < chunk_x_count; chunk_x++)
        {
            const size_t x_begin = chunk_x * chunk_size;
            const size_t y_begin = chunk_y * chunk_size;

            addTask(new ParallelForTask(
                x_begin, std::min(x_begin + chunk_size, width),
                y_begin, std::min(y_begin + chunk_size, height),
                lambda
            ));
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
    {
        return nullptr;
    }

    Task* task = m_tasks.front();
    m_tasks.pop();
    return task;
}

} // namespace tcpr
