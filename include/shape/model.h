#ifndef MODEL_H
#define MODEL_H

#include "../camera/ray.h"
#include "../util/common.h"
#include "./shape.h"


namespace tcpr
{

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> triangles) : m_triangles(triangles)
    {
    }

    Model(const std::filesystem::path& path);

    std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const override;

private:
    std::vector<Triangle> m_triangles;
};

} // namespace tcpr

#endif // MODEL_H