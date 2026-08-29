#ifndef __SCENE_H__
#define __SCENE_H__

#include "common.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "shape.h"

namespace tcpr
{

struct ShapeInstance
{
    Shape* shape;

    glm::mat4 world_from_object;
    glm::mat4 object_from_world;
};

class Scene : public Shape
{
public:
    void addShape(
        Shape*           shape,
        const glm::vec3& translate = {0.f, 0.f, 0.f},
        const glm::vec3& scale = {1.f, 1.f, 1.f},
        const glm::vec3& rotate = {0.f, 0.f, 0.f}
    );

    std::optional<HitInfo> intersect(
        const Ray& ray,
        float      tMin = 1e-4,
        float      tMax = std::numeric_limits<float>::infinity()
    ) const override;

    Scene() = default;

private:
    std::vector<ShapeInstance> m_shape_instances;
};

} // namespace tcpr

#endif // __SCENE_H__