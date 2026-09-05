#ifndef TINYBVH_ACCEL_H
#define TINYBVH_ACCEL_H

#include "accel.h"

namespace tcpr
{

/**
 * Acceleration structure backed by jbikker/tinybvh (binned SAH BVH).
 * Single-header, dependency-free, with SSE/AVX-optimized traversal.
 */
class TinybvhAccel : public Accel
{
public:
    void build(std::span<const Triangle> triangles) override;

    [[nodiscard]] std::optional<HitInfo> intersect(
        const Ray& ray,
        float      t_min,
        float      t_max
    ) const override;

private:
    tinybvh::BVH                  m_bvh;
    std::vector<tinybvh::bvhvec4> m_vertices;  // 3 verts per triangle
    std::vector<Triangle>         m_triangles; // for hit reconstruction (normals)
};

} // namespace tcpr

#endif // TINYBVH_ACCEL_H
