#ifndef MADMANN_ACCEL_H
#define MADMANN_ACCEL_H

#include "accel.h"

namespace tcpr
{

/**
 * High-performance acceleration structure backed by madmann91/bvh v2.
 * Uses the library's high-quality (sweeping SAH + reinsertion) builder.
 */
class MadmannAccel : public Accel
{
public:
    void build(std::span<const Triangle> triangles) override;

    [[nodiscard]] std::optional<HitInfo> intersect(
        const Ray& ray,
        float      t_min,
        float      t_max
    ) const override;

private:
    bvh::v2::Bvh<bvh::v2::Node<float, 3>>       m_bvh;
    std::vector<bvh::v2::PrecomputedTri<float>> m_precomputed;
    std::vector<Triangle>                       m_triangles; // permuted, for hit reconstruction
};

} // namespace tcpr

#endif // MADMANN_ACCEL_H
