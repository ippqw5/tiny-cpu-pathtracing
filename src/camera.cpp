#include "camera.h"

namespace tcpr
{

Camera::Camera(Image& image, const glm::vec3& pos, const glm::vec3& viewpoint, float fovy)
    : mImage(image), mPos(pos), mViewpoint(viewpoint), mFovy(fovy)
{
    mCameraFromClip = glm::inverse(
        glm::perspective(glm::radians(mFovy),
                         static_cast<float>(mImage.getWidth()) / static_cast<float>(mImage.getHeight()), 1.0f, 2.0f));

    mWorldFromCamera = glm::inverse(glm::lookAt(mPos, mViewpoint, {0.f, 1.f, 0.f}));
}

Ray Camera::generateRay(const glm::ivec2& pixelCoord, const glm::vec2& offset) const
{
    glm::vec2 ndcCoord = (glm::vec2(pixelCoord) + offset) / glm::vec2(mImage.getWidth(), mImage.getHeight());
    ndcCoord.y = 1.0f - ndcCoord.y;    // Flip Y coordinate for NDC space
    ndcCoord = ndcCoord * 2.0f - 1.0f; // Convert to NDC space [-1, 1]

    glm::vec4 clipCoord = {ndcCoord.x, ndcCoord.y, 0.f, 1.f};

    glm::vec3 worldCoord = mWorldFromCamera * mCameraFromClip * clipCoord;

    return Ray(mPos, glm::normalize(worldCoord - mPos));
}

} // namespace tcpr