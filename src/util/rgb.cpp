#include "util/rgb.h"

namespace tcpr
{

RGB::RGB(const glm::vec3& color)
{
    r = glm::clamp<int>(glm::pow(color.r, 1.f / 2.2f) * 255.f, 0, 255);
    g = glm::clamp<int>(glm::pow(color.g, 1.f / 2.2f) * 255.f, 0, 255);
    b = glm::clamp<int>(glm::pow(color.b, 1.f / 2.2f) * 255.f, 0, 255);
}

RGB::operator glm::vec3() const
{
    return glm::vec3(
        glm::pow(r / 255.f, 2.2f),
        glm::pow(g / 255.f, 2.2f),
        glm::pow(b / 255.f, 2.2f)
    );
}

} // namespace tcpr
