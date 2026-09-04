#ifndef FRAME_H
#define FRAME_H

#include "./common.h"

namespace tcpr
{

class Frame
{
public:
    Frame(const glm::vec3& normal);

    glm::vec3 toLocal(const glm::vec3& v_in_world) const;

    glm::vec3 toWorld(const glm::vec3& v_in_local) const;

private:
    glm::vec3 x_axis{}, y_axis{}, z_axis{};
};

} // namespace tcpr

#endif // FRAME_H