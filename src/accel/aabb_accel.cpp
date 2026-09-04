#include "accel/aabb_accel.h"

namespace tcpr
{

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
