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
    tcpr::ThreadPool threadPool{};

    tcpr::Image image{1920, 1080};

    threadPool.parallelFor(80, 60, [&](size_t x, size_t y) {
        glm::vec3 color;
        if (x % 10 == 0 || y % 10 == 0)
            color = {1.0, 1.0, 1.0};
        else
            color = {0.5, 0.7, 0.2};
        image.setPixel(x, y, color);
    });

    threadPool.wait();

    image.save("test.ppm");

    return 0;
}