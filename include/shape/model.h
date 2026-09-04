#ifndef MODEL_H
#define MODEL_H

#include "../accel/aabb.h"
#include "../camera/ray.h"
#include "../util/common.h"
#include "./shape.h"

namespace tcpr
{

class Model : public Shape
{
public:
    Model(const std::vector<Triangle>& triangles) : m_triangles(triangles)
    {
        build();
    }

    Model(const std::filesystem::path& path);

    /** Rebuild `m_aabb` from all triangle vertices. */
    void build();

    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;

private:
    std::vector<Triangle> m_triangles;
    AABB                  m_aabb;
};

} // namespace tcpr

#endif // MODEL_H