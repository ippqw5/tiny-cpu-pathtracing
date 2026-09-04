#ifndef AABB_H
#define AABB_H

#include "../camera/ray.h"
#include "../util/common.h"

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

} // namespace tcpr

#endif // AABB_H
