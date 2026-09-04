#include "camera/camera.h"
#include "camera/image.h"
#include "renderer/renderer.h"
#include "shape/model.h"
#include "shape/scene.h"
#include "shape/shape.h"
#include "thread/thread_pool.h"
#include "util/rgb.h"

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
    tcpr::ThreadPool::getInstance(32);

    tcpr::Image image{192 * 5, 108 * 5};

    tcpr::Camera camera{image, {-3.6, 0, 0}, {0, 0, 0}, 45};

    tcpr::Sphere sphere{{0, 0, 0}, 1};
    tcpr::Model  model{"assets/simple_dragon.obj"};
    tcpr::Plane  plane{
        {0, 0, 0},
        {0, 1, 0},
    };

    tcpr::Scene scene{};
    scene.addShape(
        model,
        {tcpr::RGB(202, 159, 117)},
        {0, 0, 0},
        {1, 3, 2}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, tcpr::RGB(255, 128, 128), false},
        {0, 0, 2.5}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, tcpr::RGB(128, 128, 255), false},
        {0, 0, -2.5}
    );
    scene.addShape(
        sphere,
        {{1, 1, 1}, {}, true},
        {3, 0.5, -2}
    );
    scene.addShape(
        plane,
        {tcpr::RGB(120, 204, 157)},
        {0, -0.5, 0}
    );

    tcpr::Renderer renderer{camera, scene};

    const std::string filename = "test.ppm";
    renderer.render(32, filename);

    std::cout << "Image saved to " << filename << std::endl;
    return 0;
}