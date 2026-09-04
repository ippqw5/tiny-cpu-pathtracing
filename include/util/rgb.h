#ifndef __RGB_H__
#define __RGB_H__

#include "./common.h"

namespace tcpr
{

class RGB
{
public:
    RGB(int r, int g, int b)
        : r(r), g(g), b(b)
    {
    }

    RGB(const glm::vec3& color);

    operator glm::vec3() const;

public:
    int r, g, b;
};

} // namespace tcpr

#endif // __RGB_H__