#ifndef MODEL_H
#define MODEL_H

#include "../accel/accel.h"
#include "../camera/ray.h"
#include "../util/common.h"
#include "./shape.h"

namespace tcpr
{

/**
 * Triangle mesh loaded from an OBJ file, accelerated by an `Accel` structure.
 * The acceleration implementation is selectable at construction time via `accel_type`.
 */
class Model : public Shape
{
public:
    Model(const std::vector<Triangle>& triangles, AccelType accel_type = AccelType::Madmann);

    Model(const std::filesystem::path& path, AccelType accel_type = AccelType::Madmann);

    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;

private:
    std::unique_ptr<Accel> m_accel;
};

} // namespace tcpr

#endif // MODEL_H
