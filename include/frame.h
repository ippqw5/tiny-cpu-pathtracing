#ifndef __FRAME_H__
#define __FRAME_H__

#include "common.h"
#include "glm/geometric.hpp"

namespace tcpr
{

class Frame
{
public:
    Frame(const glm::vec3& normal)
    {
        y_axis = normal;

        glm::vec3 up = glm::abs(y_axis.y) < 0.999f ? glm::vec3{0.f, 1.f, 0.f} : glm::vec3{0.f, 0.f, 1.f};

        x_axis = glm::normalize(glm::cross(up, y_axis));
        z_axis = glm::normalize(glm::cross(x_axis, y_axis));
    }

    glm::vec3 toLocal(const glm::vec3& v_in_world) const
    {
        return glm::normalize(glm::vec3{
            glm::dot(v_in_world, x_axis),
            glm::dot(v_in_world, y_axis),
            glm::dot(v_in_world, z_axis),
        });
    }

    glm::vec3 toWorld(const glm::vec3& v_in_local) const
    {
        return glm::normalize(v_in_local.x * x_axis + v_in_local.y * y_axis + v_in_local.z * z_axis);
    }

private:
    glm::vec3 x_axis, y_axis, z_axis;
};

} // namespace tcpr

#endif // __FRAME_H__