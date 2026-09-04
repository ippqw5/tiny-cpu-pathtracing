#include "renderer/renderer.h"
#include "thread/thread_pool.h"
#include "util/frame.h"
#include "util/profile.h"
#include "util/progress.h"

namespace tcpr
{

glm::vec3 Renderer::renderPixel(glm::ivec2 pixel_coord, RNG& rng)
{
    // 1. 生成当前采样的相机射线（像素内偏移在 [-3, 1] 区间，与原始 main.cpp 行为一致）
    Ray ray = m_camera.generateRay(
        pixel_coord,
        {(rng.uniform() * 2.f) - 1.f, (rng.uniform() * 2.f) - 1.f}
    );

    glm::vec3 beta = {1.f, 1.f, 1.f};  // 累积的路径权重
    glm::vec3 color = {0.f, 0.f, 0.f}; // 最终累积颜色

    // 2. 路径追踪主循环（弹射直到没有命中）
    while (true)
    {
        auto hit_info = m_scene.intersect(ray);
        if (hit_info.has_value())
        {
            // 2.1 累加光源贡献（自发光 * 当前路径权重）
            color += beta * hit_info->material->emissive;
            // 2.2 更新路径权重（乘以物体反照率）
            beta *= hit_info->material->albedo;

            // 2.3 移动射线起点到击中点
            ray.ori = hit_info->p;

            // 2.4 构造局部坐标系，生成出射方向
            Frame     frame{hit_info->n};
            glm::vec3 v_out;
            if (hit_info->material->is_specular)
            {
                // 镜面反射：关于 y 轴（法线）镜像反射
                glm::vec3 v_in = frame.toLocal(-ray.dir);
                v_out = glm::vec3{-v_in.x, v_in.y, -v_in.z};
            }
            else
            {
                // 漫反射：在上半球做均匀拒绝采样
                do
                {
                    v_out = {rng.uniform(), rng.uniform(), rng.uniform()};
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
            // 没有击中任何物体 —— 结束路径
            break;
        }
    }

    return color;
}

void Renderer::render(size_t spp, const std::filesystem::path& filename)
{
    PROFILE_SCOPE("Renderer::render(" + std::to_string(spp) + ", " + filename.string() + ")");

    Image& image = m_camera.getImage();

    const size_t width = image.getWidth();
    const size_t height = image.getHeight();
    const size_t progress_total = width * height * spp;

    Progress progress(progress_total, "Render Progress", 10);

    size_t spp_count = 0;
    size_t spp_in_one_pass = 1;
    while (spp_count < spp)
    {
        {
            PROFILE_SCOPE("Renderer::render() - spp_in_one_pass = " + std::to_string(spp_in_one_pass));
            ThreadPool::getInstance().parallelFor(width, height, [&](size_t x, size_t y) {
                thread_local RNG rng{12345, -1.f, 1.f};

                for (size_t i = 0; i < spp_in_one_pass; i++)
                {
                    glm::vec3 c = renderPixel(
                        {static_cast<int>(x), static_cast<int>(y)},
                        rng
                    );
                    image.addSample(x, y, c);

                    progress.advance(1);
                }
            });

            ThreadPool::getInstance().wait();
        }

        spp_count += spp_in_one_pass;
        spp_in_one_pass = std::min<size_t>({2 * spp_in_one_pass, spp - spp_count, static_cast<unsigned long long>(32)});
        image.save(filename);
    }
}

} // namespace tcpr
