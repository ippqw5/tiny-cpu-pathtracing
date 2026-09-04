#ifndef RAY_H
#define RAY_H

#include "../shape/material.h"
#include "../util/common.h"

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

    Ray transform(const glm::mat4& mat) const;

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

#endif // RAY_H