#ifndef SCENE_H
#define SCENE_H

#include "../util/common.h"
#include "./shape.h"

namespace tcpr
{

class Scene : public Shape
{
public:
    struct ShapeInstance
    {
        // Non-owning: the pointed-to Shape must outlive this Scene.
        const Shape* shape;
        Material     material;

        glm::mat4 world_from_object;
        glm::mat4 object_from_world;
    };

    void addShape(
        const Shape&     shape,
        const Material&  material = {},
        const glm::vec3& translate = {0.f, 0.f, 0.f},
        const glm::vec3& scale = {1.f, 1.f, 1.f},
        const glm::vec3& rotate = {0.f, 0.f, 0.f}
    );

    [[nodiscard]] std::optional<HitInfo> intersect(
        const Ray& ray,
        float      t_min = 1e-4,
        float      t_max = std::numeric_limits<float>::infinity()
    ) const override;

    Scene() = default;

private:
    std::vector<ShapeInstance> m_shape_instances;
};

} // namespace tcpr

#endif // SCENE_H