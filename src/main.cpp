#include "camera.h"
#include "frame.h"
#include "glm/geometric.hpp"
#include "image.h"
#include "model.h"
#include "ray.h"
#include "rgb.h"
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
    tcpr::ThreadPool thread_pool{32};

    tcpr::Image image{192 * 4, 108 * 4};

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

    std::atomic<size_t> count{0};

    size_t spp = 2;

    thread_pool.parallelFor(image.getWidth(), image.getHeight(), [&](size_t x, size_t y) {
        // Per-thread RNG to avoid data race on shared mt19937 internal state
        thread_local std::mt19937 rng{
            static_cast<std::mt19937::result_type>(
                12345u ^ std::hash<std::thread::id>{}(std::this_thread::get_id())
            )
        };
        thread_local std::uniform_real_distribution<float> dist{-1.f, 1.f};

        for (size_t i = 0; i < spp; i++)
        {
            tcpr::Ray ray = camera.generateRay(
                {x, y},
                {dist(rng) * 2.f - 1.f, dist(rng) * 2.f - 1.f}
            );

            struct Payload
            {
                glm::vec3 beta = {1.f, 1.f, 1.f};
                glm::vec3 color = {0.f, 0.f, 0.f};
            } playload;

            while (true)
            {
                auto hit_info = scene.intersect(ray);
                if (hit_info.has_value())
                {
                    playload.color += playload.beta * hit_info->material->emissive;
                    playload.beta *= hit_info->material->albedo;

                    // reflection
                    ray.ori = hit_info->p;

                    tcpr::Frame frame{hit_info->n};
                    glm::vec3   v_out = {};
                    if (hit_info->material->is_specular)
                    {
                        glm::vec3 v_in = frame.toLocal(-ray.dir);
                        v_out = glm::vec3{-v_in.x, v_in.y, -v_in.z};
                    }
                    else
                    {
                        do
                        {
                            v_out = {dist(rng), dist(rng), dist(rng)};
                        } while (glm::length(v_out) > 1.f);
                        if (v_out.y < 0.f)
                        {
                            v_out.y = -v_out.y;
                        }
                    }
                    ray.dir = frame.toWorld(v_out);
                }
                else
                {
                    break;
                }
            }

            image.addSample(x, y, playload.color);
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