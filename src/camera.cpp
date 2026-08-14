#include "camera.h"

namespace tcpr
{

Camera::Camera(Image& image, const glm::vec3& pos, const glm::vec3& viewpoint, float fovy)
    : mImage(image), mPos(pos), mViewpoint(viewpoint), mFovy(fovy)
{
    mCameraFromClip =
        glm::inverse(glm::perspective(mFovy, (float)mImage.getWidth() / (float)mImage.getHeight(), 1.0f, 2.0f));

    mWorldFromCamera = glm::lookAt(mPos, mViewpoint, {0.f, 1.f, 0.f});
}

Ray Camera::generateRay(const glm::vec2& pixelCoord, const glm::vec2& offset) const
{
    auto ndcCoord = (pixelCoord + offset) / glm::vec2(mImage.getWidth(), mImage.getHeight());
    ndcCoord.y = 1.0f - ndcCoord.y;    // Flip Y coordinate for NDC space
    ndcCoord = ndcCoord * 2.0f - 1.0f; // Convert to NDC space [-1, 1]

    glm::vec4 clipCoord = {ndcCoord.x, ndcCoord.y, 0.f, 1.f};

    auto cameraSpacePos = mCameraFromClip * clipCoord;
    auto worldSpacePos = mWorldFromCamera * cameraSpacePos;

    return Ray(mPos, glm::normalize(glm::vec3(worldSpacePos) - mPos));
}

} // namespace tcpr