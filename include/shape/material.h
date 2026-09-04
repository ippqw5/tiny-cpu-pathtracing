#ifndef MATERIAL_H
#define MATERIAL_H

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

#endif // MATERIAL_H