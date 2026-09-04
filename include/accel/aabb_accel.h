#ifndef AABB_ACCEL_H
#define AABB_ACCEL_H

#include "aabb.h"
#include "accel.h"

namespace tcpr
{

/**
 * Reference acceleration structure: a single AABB over all triangles,
 * followed by a linear scan when the ray hits the box.
 */
class AABBAccel : public Accel
{
public:
    void build(std::span<const Triangle> triangles) override;

    [[nodiscard]] std::optional<HitInfo> intersect(
        const Ray& ray,
        float      t_min,
        float      t_max
    ) const override;

private:
    std::vector<Triangle> m_triangles;
    AABB                  m_aabb;
};

} // namespace tcpr

#endif // AABB_ACCEL_H
