#ifndef AABB_ACCEL_H
#define AABB_ACCEL_H

#include "accel.h"

namespace tcpr
{

/**
 * Axis-aligned bounding box for ray-shape intersection acceleration.
 * Default-constructed box is empty (min = +inf, max = -inf).
 */
struct AABB
{
    void expand(const glm::vec3& point);

    // Slab-method ray-box test within `[t_min, t_max]`.
    [[nodiscard]] bool intersect(const Ray& ray, float t_min, float t_max) const;

    glm::vec3 min{std::numeric_limits<float>::infinity()};
    glm::vec3 max{-std::numeric_limits<float>::infinity()};
};

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
