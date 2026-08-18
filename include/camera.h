#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"
#include "image.h"
#include "ray.h"

namespace tcpr
{

class Camera
{
public:
    Camera(Image& image, const glm::vec3& pos, const glm::vec3& viewpoint, float fovy);

    Ray generateRay(const glm::ivec2& pixelCoord, const glm::vec2& offset = {0.5f, 0.5f}) const;

private:
    Image&    mImage;
    glm::vec3 mPos;
    glm::vec3 mViewpoint;
    float     mFovy;

    glm::mat4 mCameraFromClip;
    glm::mat4 mWorldFromCamera;
};

} // namespace tcpr

#endif // __CAMERA_H__