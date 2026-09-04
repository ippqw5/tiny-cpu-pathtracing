#include "camera/ray.h"

namespace tcpr
{

Ray Ray::transform(const glm::mat4& mat) const
{
    glm::vec4 new_ori = mat * glm::vec4(ori, 1.f);
    glm::vec4 new_dir = mat * glm::vec4(dir, 0.f);
    return Ray{glm::vec3(new_ori), glm::normalize(glm::vec3(new_dir))};
}

} // namespace tcpr
