#ifndef ACCEL_H
#define ACCEL_H

#include "../camera/ray.h"
#include "../shape/shape.h"
#include "../util/common.h"

namespace tcpr
{

// Selects which concrete acceleration structure to use.
enum class AccelType
{
    // Single AABB over all triangles with a linear scan (reference).
    AABB,
    // madmann91/bvh v2 SAH-built binary BVH (high performance).
    Madmann,
};

/**
 * Abstract acceleration structure over a triangle soup.
 *
 * A BVH is one kind of acceleration structure; other kinds (e.g. a plain
 * AABB-culled list, a grid, etc.) can implement this interface.
 * Concrete implementations own their primitive data after `build()` returns,
 * so the caller is free to discard the input triangles.
 */
class Accel
{
public:
    virtual ~Accel() = default;

    // Build the structure from the given triangles.
    virtual void build(std::span<const Triangle> triangles) = 0;

    // Find the closest intersection of `ray` within `[t_min, t_max]`.
    [[nodiscard]] virtual std::optional<HitInfo> intersect(
        const Ray& ray,
        float      t_min,
        float      t_max
    ) const = 0;
};

// Create an acceleration structure of the requested type.
[[nodiscard]] std::unique_ptr<Accel> createAccel(AccelType type);

} // namespace tcpr

#endif // ACCEL_H
