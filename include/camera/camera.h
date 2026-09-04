#ifndef CAMERA_H
#define CAMERA_H

#include "../util/common.h"
#include "./image.h"
#include "./ray.h"

namespace tcpr
{

class Camera
{
public:
    Camera(Image& image, const glm::vec3& pos, const glm::vec3& viewpoint, float fovy);

    Ray generateRay(const glm::ivec2& pixel_coord, const glm::vec2& offset = {0.5f, 0.5f}) const;

    Image& getImage()
    {
        return m_image;
    }

private:
    Image&    m_image;
    glm::vec3 m_pos;
    glm::vec3 m_viewpoint;
    float     m_fovy;

    glm::mat4 m_camera_from_clip{};
    glm::mat4 m_world_from_camera{};
};

} // namespace tcpr

#endif // CAMERA_H