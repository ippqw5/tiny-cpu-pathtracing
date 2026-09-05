#include "accel/aabb_accel.h"

namespace tcpr
{

void AABB::expand(const glm::vec3& point)
{
    min = glm::min(min, point);
    max = glm::max(max, point);
}

bool AABB::intersect(const Ray& ray, float t_min, float t_max) const
{
    for (size_t axis = 0; axis < 3; axis++)
    {
        const float inv_dir = 1.f / ray.dir[axis];
        float       t_near = (min[axis] - ray.ori[axis]) * inv_dir;
        float       t_far = (max[axis] - ray.ori[axis]) * inv_dir;

        // parallel rays get +/-inf here, which fails the test below naturally
        if (inv_dir < 0.f)
        {
            std::swap(t_near, t_far);
        }

        t_min = std::max(t_min, t_near);
        t_max = std::min(t_max, t_far);

        if (t_max < t_min)
        {
            return false;
        }
    }
    return true;
}

void AABBAccel::build(std::span<const Triangle> triangles)
{
    m_triangles.assign(triangles.begin(), triangles.end());

    m_aabb = AABB{};
    for (const auto& tri : m_triangles)
    {
        m_aabb.expand(tri.p0);
        m_aabb.expand(tri.p1);
        m_aabb.expand(tri.p2);
    }
}

std::optional<HitInfo> AABBAccel::intersect(const Ray& ray, float t_min, float t_max) const
{
    if (!m_aabb.intersect(ray, t_min, t_max))
    {
        return std::nullopt;
    }

    std::optional<HitInfo> closest_hit;
    for (const auto& tri : m_triangles)
    {
        auto hit = tri.intersect(ray, t_min, t_max);
        if (hit.has_value())
        {
            t_max = hit->t;
            closest_hit = hit;
        }
    }
    return closest_hit;
}

} // namespace tcpr
