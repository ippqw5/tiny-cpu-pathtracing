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

    tcpr::Image image{400, 300};

    tcpr::Camera camera{image, glm::vec3{1.f, 0.0f, 0.f}, glm::vec3{0.0f, 0.0f, 0.0f}, 90.0f};

    tcpr::Sphere sphere{glm::vec3{0.0f, 0.0f, 0.0f}, 0.5f};

    tcpr::Model model{"assets/simple_dragon.obj"};

    tcpr::Shape& shape = model;

    glm::vec3 lightPos{1.0f, 1.0f, 1.0f};

    threadPool.parallelFor(image.getWidth(), image.getHeight(), [&](size_t x, size_t y) {
        tcpr::Ray ray = camera.generateRay({x, y});
        auto      hitInfo = shape.intersect(ray);
        if (hitInfo.has_value())
        {
            glm::vec3 L = glm::normalize(lightPos - hitInfo->p);
            float     cos = glm::max(0.f, glm::dot(hitInfo->n, L));
            image.setPixel(x, y, {cos, cos, cos});
        }
    });

    threadPool.wait();

    image.save("test.ppm");
    std::cout << "Image saved to test.ppm" << std::endl;
    return 0;
}