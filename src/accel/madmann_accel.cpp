#include "accel/madmann_accel.h"

namespace tcpr
{

using Scalar = float;
using BvhVec3 = bvh::v2::Vec<Scalar, 3>;
using BvhBBox = bvh::v2::BBox<Scalar, 3>;
using BvhTri = bvh::v2::Tri<Scalar, 3>;
using BvhNode = bvh::v2::Node<Scalar, 3>;
using BvhT = bvh::v2::Bvh<BvhNode>;
using PrecomputedTri = bvh::v2::PrecomputedTri<Scalar>;

static BvhVec3 to_bvh(const glm::vec3& v)
{
    return BvhVec3{v.x, v.y, v.z};
}

void MadmannAccel::build(std::span<const Triangle> triangles)
{
    std::vector<BvhTri> bvh_tris;
    bvh_tris.reserve(triangles.size());
    for (const auto& t : triangles)
    {
        bvh_tris.emplace_back(to_bvh(t.p0), to_bvh(t.p1), to_bvh(t.p2));
    }

    std::vector<BvhBBox> bboxes(triangles.size());
    std::vector<BvhVec3> centers(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++)
    {
        bboxes[i] = bvh_tris[i].get_bbox();
        centers[i] = bvh_tris[i].get_center();
    }

    typename bvh::v2::DefaultBuilder<BvhNode>::Config config;
    config.quality = bvh::v2::DefaultBuilder<BvhNode>::Quality::High;
    m_bvh = bvh::v2::DefaultBuilder<BvhNode>::build(bboxes, centers, config);

    // Permuting primitive data removes an indirection during traversal.
    static constexpr bool should_permute = true;

    m_precomputed.clear();
    m_triangles.clear();
    m_precomputed.reserve(triangles.size());
    m_triangles.reserve(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++)
    {
        const size_t j = should_permute ? m_bvh.prim_ids[i] : i;
        m_precomputed.push_back(bvh_tris[j]);
        m_triangles.push_back(triangles[j]);
    }
}

std::optional<HitInfo> MadmannAccel::intersect(const Ray& ray, float t_min, float t_max) const
{
    bvh::v2::Ray<Scalar, 3> bvh_ray{to_bvh(ray.ori), to_bvh(ray.dir), t_min, t_max};

    static constexpr size_t invalid_id = std::numeric_limits<size_t>::max();
    static constexpr size_t stack_size = 64;

    size_t prim_id = invalid_id;
    Scalar u = 0.f;
    Scalar v = 0.f;

    bvh::v2::SmallStack<BvhT::Index, stack_size> stack;
    m_bvh.intersect<false, false>(bvh_ray, m_bvh.get_root().index, stack,
                                  [&](size_t begin, size_t end) {
                                      for (size_t i = begin; i < end; i++)
                                      {
                                          if (auto hit = m_precomputed[i].intersect(bvh_ray))
                                          {
                                              prim_id = i;
                                              std::tie(bvh_ray.tmax, u, v) = *hit;
                                          }
                                      }
                                      return prim_id != invalid_id;
                                  });

    if (prim_id == invalid_id)
    {
        return std::nullopt;
    }

    const auto& tri = m_triangles[prim_id];
    const float t = bvh_ray.tmax;
    const auto  p = ray.at(t);
    const auto  n = glm::normalize((1.f - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
    return HitInfo{.t = t, .p = p, .n = n};
}

} // namespace tcpr
