#include "camera.h"

namespace tcpr
{

Camera::Camera(Image& image, const glm::vec3& pos, const glm::vec3& viewpoint, float fovy)
    : m_image(image), m_pos(pos), m_viewpoint(viewpoint), m_fovy(fovy)
{
    m_camera_from_clip = glm::inverse(
        glm::perspective(glm::radians(m_fovy), static_cast<float>(m_image.getWidth()) / static_cast<float>(m_image.getHeight()), 1.0f, 2.0f)
    );

    m_world_from_camera = glm::inverse(glm::lookAt(m_pos, m_viewpoint, {0.f, 1.f, 0.f}));
}

Ray Camera::generateRay(const glm::ivec2& pixel_coord, const glm::vec2& offset) const
{
    glm::vec2 ndc_coord = (glm::vec2(pixel_coord) + offset) / glm::vec2(m_image.getWidth(), m_image.getHeight());
    ndc_coord.y = 1.0f - ndc_coord.y;    // Flip Y coordinate for NDC space
    ndc_coord = ndc_coord * 2.0f - 1.0f; // Convert to NDC space [-1, 1]

    glm::vec4 clip_coord = {ndc_coord.x, ndc_coord.y, 0.f, 1.f};

    glm::vec3 world_coord = m_world_from_camera * m_camera_from_clip * clip_coord;

    return Ray(m_pos, glm::normalize(world_coord - m_pos));
}

} // namespace tcpr