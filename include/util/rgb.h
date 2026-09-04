#ifndef RGB_H
#define RGB_H

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

    int r, g, b;
};

} // namespace tcpr

#endif // RGB_H