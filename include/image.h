#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

namespace tcpr
{

struct Pixel
{
    glm::vec3 color = {0.f, 0.f, 0.f};

    size_t sample_count = 0;
};

class Image
{
public:
    Image(size_t width, size_t height);

    void save(const std::filesystem::path& filename);

    size_t getWidth() const
    {
        return m_width;
    }
    size_t getHeight() const
    {
        return m_height;
    }

    Pixel getPixel(size_t x, size_t y)
    {
        return m_pixels[y * m_width + x];
    }
    void addSample(size_t x, size_t y, glm::vec3 color)
    {
        m_pixels[y * m_width + x].color += color;
        m_pixels[y * m_width + x].sample_count++;
    }

private:
    size_t             m_width, m_height;
    std::vector<Pixel> m_pixels;
};

} // namespace tcpr

#endif // __IMAGE_H__