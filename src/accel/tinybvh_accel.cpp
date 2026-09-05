#include "accel/tinybvh_accel.h"

#define TINYBVH_IMPLEMENTATION
#include "tiny_bvh.h"

namespace tcpr
{

void TinybvhAccel::build(std::span<const Triangle> triangles)
{
    m_vertices.clear();
    m_triangles.clear();
    m_vertices.reserve(triangles.size() * 3);
    m_triangles.reserve(triangles.size());

    for (const auto& t : triangles)
    {
        m_vertices.emplace_back(t.p0.x, t.p0.y, t.p0.z, 0.f);
        m_vertices.emplace_back(t.p1.x, t.p1.y, t.p1.z, 0.f);
        m_vertices.emplace_back(t.p2.x, t.p2.y, t.p2.z, 0.f);
        m_triangles.push_back(t);
    }

    m_bvh.Build(m_vertices.data(), static_cast<uint32_t>(triangles.size()));
}

std::optional<HitInfo> TinybvhAccel::intersect(const Ray& ray, float t_min, float t_max) const
{
    // TinyBVH's Ray has no tmin; offset the origin along the direction so the
    // effective hit range becomes [0, t_max - t_min] in the offset frame.
    const glm::vec3 origin = ray.ori + ray.dir * t_min;
    const float     tmax = std::min(t_max - t_min, 1e30f);

    tinybvh::Ray bvh_ray(
        tinybvh::bvhvec3(origin.x, origin.y, origin.z),
        tinybvh::bvhvec3(ray.dir.x, ray.dir.y, ray.dir.z),
        tmax
    );

    m_bvh.Intersect(bvh_ray);

    if (bvh_ray.hit.t >= tmax)
    {
        return std::nullopt;
    }

    const auto& tri = m_triangles[bvh_ray.hit.prim];
    const float t = bvh_ray.hit.t + t_min;
    const float u = bvh_ray.hit.u;
    const float v = bvh_ray.hit.v;
    const auto  p = ray.at(t);
    const auto  n = glm::normalize((1.f - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
    return HitInfo{.t = t, .p = p, .n = n};
}

} // namespace tcpr
