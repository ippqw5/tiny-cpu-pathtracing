#ifndef __RAY_H__
#define __RAY_H__

#include "common.h"

namespace tcpr
{

struct Ray
{
    Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction)
    {
    }

    glm::vec3 at(float t) const
    {
        return origin + t * direction;
    }

    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitInfo
{
    float     t;
    glm::vec3 p;
    glm::vec3 n;
};

} // namespace tcpr

#endif // __RAY_H__