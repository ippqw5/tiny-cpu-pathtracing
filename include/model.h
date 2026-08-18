#ifndef __MODEL_H__
#define __MODEL_H__

#include "ray.h"
#include "shape.h"

#include <filesystem>

namespace tcpr
{

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> triangles) : mTriangles(triangles)
    {
    }

    Model(const std::filesystem::path& filename);

    std::optional<HitInfo> intersect(const Ray& ray, float tMin, float tMax) const override;

private:
    std::vector<Triangle> mTriangles;
};

} // namespace tcpr

#endif // __MODEL_H__