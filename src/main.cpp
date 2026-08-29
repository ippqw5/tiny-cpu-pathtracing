#include "camera.h"
#include "image.h"
#include "model.h"
#include "ray.h"
#include "scene.h"
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
    tcpr::ThreadPool thread_pool{16};

    tcpr::Image image{800, 600};

    tcpr::Camera camera{image, {-1.6, 0, 0}, {0, 0, 0}, 90};

    tcpr::Sphere sphere{{0, 0, 0}, 0.3};
    tcpr::Model  model{"assets/simple_dragon.obj"};
    tcpr::Plane  plane{
        {0, 0, 0},
        {0, 1, 0},
    };

    tcpr::Scene scene{};
    scene.addShape(&model);
    scene.addShape(&sphere, {0, 0, 1.0});
    scene.addShape(&plane, {0, -0.5, 0});

    glm::vec3 light_pos{-1, 2, 1};

    std::atomic<size_t> count{0};

    thread_pool.parallelFor(image.getWidth(), image.getHeight(), [&](size_t x, size_t y) {
        tcpr::Ray ray = camera.generateRay({x, y});
        auto      hit_info = scene.intersect(ray);
        if (hit_info.has_value())
        {

            auto  L = glm::normalize(light_pos - hit_info->p);
            float cos = glm::max(0.f, glm::dot(hit_info->n, L));

            image.setPixel(x, y, {cos, cos, cos});
        }

        int n = ++count;
        if (n % (image.getWidth() * image.getHeight() / 10) == 0)
        {
            std::cout << "Progress: " << (n * 100) / (image.getWidth() * image.getHeight()) << "%" << std::endl;
        }
    });

    thread_pool.wait();

    image.save("test.ppm");
    std::cout << "Image saved to test.ppm" << std::endl;
    return 0;
}