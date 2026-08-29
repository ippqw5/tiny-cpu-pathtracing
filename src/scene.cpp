#include "scene.h"

namespace tcpr
{

void Scene::addShape(
    Shape*           shape,
    const glm::vec3& translate,
    const glm::vec3& scale,
    const glm::vec3& rotate
)
{
    glm::mat4 world_form_object =
        glm::translate(glm::mat4(1.0f), translate) *
        glm::rotate(glm::mat4(1.0f), rotate.x, {1.f, 0.f, 0.f}) *
        glm::rotate(glm::mat4(1.0f), rotate.y, {0.f, 1.f, 0.f}) *
        glm::rotate(glm::mat4(1.0f), rotate.z, {0.f, 0.f, 1.f}) *
        glm::scale(glm::mat4(1.0f), scale);

    glm::mat4 object_from_world = glm::inverse(world_form_object);

    m_shape_instances.push_back({shape, world_form_object, object_from_world});
}

std::optional<HitInfo> Scene::intersect(
    const Ray& ray,
    float      tMin,
    float      tMax
) const
{
    std::optional<HitInfo> closest_hit = {};
    const ShapeInstance*   closest_instance = nullptr;

    for (const auto& instance : m_shape_instances)
    {
        Ray  ray_object = ray.transform(instance.object_from_world);
        auto hit_info = instance.shape->intersect(ray_object, tMin, tMax);
        if (hit_info.has_value())
        {
            closest_hit = hit_info;
            closest_instance = &instance;
            tMax = hit_info->t;
        }
    }

    if (closest_instance)
    {
        closest_hit->p = closest_instance->world_from_object * glm::vec4(closest_hit->p, 1.f);
        closest_hit->n = glm::transpose(closest_instance->object_from_world) * glm::vec4(closest_hit->n, 0.f);
    }

    return closest_hit;
}

} // namespace tcpr