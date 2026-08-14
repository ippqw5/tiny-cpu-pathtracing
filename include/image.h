#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

namespace tcpr
{

class Image
{
public:
    Image(size_t width, size_t height);

    void save(const std::filesystem::path& filename);

    size_t getWidth() const
    {
        return mWidth;
    }
    size_t getHeight() const
    {
        return mHeight;
    }

    glm::vec3 getPixel(size_t x, size_t y)
    {
        return mPixels[y * mWidth + x];
    }
    void setPixel(size_t x, size_t y, glm::vec3 color)
    {
        mPixels[y * mWidth + x] = color;
    }

private:
    size_t                 mWidth, mHeight;
    std::vector<glm::vec3> mPixels;
};

} // namespace tcpr

#endif // __IMAGE_H__