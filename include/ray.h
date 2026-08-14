#ifndef __RAY_H__
#define __RAY_H__

#include "common.h"

namespace tcpr
{

struct Ray
{
    Ray(glm::vec3 pos, glm::vec3 dir) : position(pos), direction(dir)
    {
    }

    glm::vec3 position;
    glm::vec3 direction;
};

} // namespace tcpr

#endif // __RAY_H__