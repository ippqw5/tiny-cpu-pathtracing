#ifndef __RGB_H__
#define __RGB_H__

#include "common.h"

namespace tcpr
{

class RGB
{
public:
    RGB(int r, int g, int b)
        : r(r), g(g), b(b)
    {
    }

    RGB(const glm::vec3& color)
    {
        r = glm::clamp<int>(glm::pow(color.r, 1.f / 2.2f) * 255.f, 0, 255);
        g = glm::clamp<int>(glm::pow(color.g, 1.f / 2.2f) * 255.f, 0, 255);
        b = glm::clamp<int>(glm::pow(color.b, 1.f / 2.2f) * 255.f, 0, 255);
    }

    operator glm::vec3() const
    {
        return glm::vec3(
            glm::pow(r / 255.f, 2.2f),
            glm::pow(g / 255.f, 2.2f),
            glm::pow(b / 255.f, 2.2f)
        );
    }

public:
    int r, g, b;
};

} // namespace tcpr

#endif // __RGB_H__