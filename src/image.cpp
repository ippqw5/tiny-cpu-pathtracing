#include "image.h"

namespace tcpr
{
Image::Image(size_t width, size_t height) : m_width(width), m_height(height)
{
    m_pixels.resize(width * height);
}

void Image::save(const std::filesystem::path& filename)
{
    std::ofstream file(filename, std::ios::binary);
    // PPM
    file << "P6\n"
         << m_width << ' ' << m_height << "\n255\n";

    for (size_t y = 0; y < m_height; y++)
    {
        for (size_t x = 0; x < m_width; x++)
        {
            const glm::vec3& color = getPixel(x, y);
            glm::uvec3       color_u = glm::clamp(color * 255.f, 0.f, 255.f);

            file << static_cast<uint8_t>(color_u.x) << static_cast<uint8_t>(color_u.y)
                 << static_cast<uint8_t>(color_u.z);
        }
    }
}

} // namespace tcpr