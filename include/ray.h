#ifndef __RAY_H__
#define __RAY_H__

#include "common.h"
#include "material.h"

namespace tcpr
{

struct Ray
{
    Ray(glm::vec3 origin, glm::vec3 direction) : ori(origin), dir(direction)
    {
    }

    glm::vec3 at(float t) const
    {
        return ori + t * dir;
    }

    Ray transform(const glm::mat4& mat) const
    {
        glm::vec4 new_ori = mat * glm::vec4(ori, 1.f);
        glm::vec4 new_dir = mat * glm::vec4(dir, 0.f);
        return Ray{glm::vec3(new_ori), glm::normalize(glm::vec3(new_dir))};
    }

    glm::vec3 ori;
    glm::vec3 dir;
};

struct HitInfo
{
    float     t;
    glm::vec3 p;
    glm::vec3 n;

    const Material* material = nullptr;
};

} // namespace tcpr

#endif // __RAY_H__