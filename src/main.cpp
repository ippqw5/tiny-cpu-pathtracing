#include <iostream>

#include <glm/glm.hpp>

#include "image.h"
#include "thread_pool.h"


class FooTask : public tcpr::Task
{
  public:
    void run() override
    {
        std::cout << "Hello World" << std::endl;
    }
};

int main()
{
    tcpr::Image image{1920, 1080};
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            image.setPixel(i, j, {0.5, 0.7, 0.2});
        }
    }
    image.save("test.ppm");

    tcpr::ThreadPool threadPool{};

    threadPool.addTask(new FooTask());
    threadPool.addTask(new FooTask());
    threadPool.addTask(new FooTask());
    threadPool.addTask(new FooTask());

    return 0;
}