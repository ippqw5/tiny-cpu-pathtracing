#include "camera.h"
#include "image.h"
#include "model.h"
#include "ray.h"
#include "shape.h"
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

    tcpr::Image  image{400, 300};
    tcpr::Camera camera{image, {-0.6f, 0.1f, 0.f}, {0.0f, 0.0f, 0.0f}, 90.0f};
    tcpr::Sphere sphere{{0.0f, 0.0f, 0.0f}, 0.5f};
    tcpr::Model  model{"assets/simple_dragon.obj"};
    tcpr::Plane  plane{
         {0.0f, 0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
    };
    tcpr::Shape& shape = plane;

    glm::vec3 lightPos{-1.0f, 2.0f, 1.0f};

    std::atomic<size_t> count{0};

    threadPool.parallelFor(image.getWidth(), image.getHeight(), [&](size_t x, size_t y) {
        tcpr::Ray ray = camera.generateRay({x, y});
        auto      hitInfo = shape.intersect(ray);
        if (hitInfo.has_value())
        {
            glm::vec3 L = glm::normalize(lightPos - hitInfo->p);
            float     cos = glm::max(0.f, glm::dot(hitInfo->n, L));
            image.setPixel(x, y, {cos, cos, cos});
        }

        int n = ++count;
        if (n % (image.getWidth() * image.getHeight() / 10) == 0)
        {
            std::cout << "Progress: " << (n * 100) / (image.getWidth() * image.getHeight()) << "%" << std::endl;
        }
    });

    threadPool.wait();

    image.save("test.ppm");
    std::cout << "Image saved to test.ppm" << std::endl;
    return 0;
}