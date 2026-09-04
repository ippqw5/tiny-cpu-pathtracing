#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../util/common.h"

namespace tcpr
{

struct Material
{
    glm::vec3 albedo = {1.f, 1.f, 1.f};
    glm::vec3 emissive = {0.f, 0.f, 0.f};

    bool is_specular = false;
};

} // namespace tcpr

#endif // __MATERIAL_H__