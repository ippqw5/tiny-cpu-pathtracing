#include "accel/accel.h"
#include "camera/camera.h"
#include "camera/image.h"
#include "renderer/renderer.h"
#include "shape/model.h"
#include "shape/scene.h"
#include "shape/shape.h"
#include "thread/thread_pool.h"
#include "util/rgb.h"

int main(int argc, char** argv)
{
    tcpr::AccelType accel_type = tcpr::AccelType::Madmann;
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "aabb")
        {
            accel_type = tcpr::AccelType::AABB;
        }
        else if (arg == "tinybvh")
        {
            accel_type = tcpr::AccelType::Tinybvh;
        }
    }

    tcpr::ThreadPool::getInstance(32);

    tcpr::Image image{static_cast<size_t>(192 * 5), static_cast<size_t>(108 * 5)};

    tcpr::Camera camera{image, {-3.6, 0, 0}, {0, 0, 0}, 45};

    tcpr::Sphere sphere{{0, 0, 0}, 1};
    tcpr::Model  model{"assets/simple_dragon.obj", accel_type};
    tcpr::Plane  plane{
        {0, 0, 0},
        {0, 1, 0},
    };

    tcpr::Scene scene{};
    scene.addShape(
        model,
        {.albedo = tcpr::RGB(202, 159, 117)},
        {0, 0, 0},
        {1, 3, 2}
    );
    scene.addShape(
        sphere,
        {.albedo = {1, 1, 1}, .emissive = tcpr::RGB(255, 128, 128), .is_specular = false},
        {0, 0, 2.5}
    );
    scene.addShape(
        sphere,
        {.albedo = {1, 1, 1}, .emissive = tcpr::RGB(128, 128, 255), .is_specular = false},
        {0, 0, -2.5}
    );
    scene.addShape(
        sphere,
        {.albedo = {1, 1, 1}, .emissive = {}, .is_specular = true},
        {3, 0.5, -2}
    );
    scene.addShape(
        plane,
        {.albedo = tcpr::RGB(120, 204, 157)},
        {0, -0.5, 0}
    );

    tcpr::Renderer renderer{camera, scene};

    const std::string accel_name = (accel_type == tcpr::AccelType::AABB)
                                       ? "aabb"
                                       : (accel_type == tcpr::AccelType::Tinybvh ? "tinybvh" : "madmann");
    const std::string filename = "test_" + accel_name + ".ppm";
    renderer.render(32, filename);

    return 0;
}
