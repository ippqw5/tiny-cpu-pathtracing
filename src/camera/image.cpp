#include "camera/image.h"
#include "util/rgb.h"

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
            Pixel pixel(getPixel(x, y));
            RGB   color(pixel.color / static_cast<float>(pixel.sample_count));

            file << static_cast<uint8_t>(color.r) << static_cast<uint8_t>(color.g)
                 << static_cast<uint8_t>(color.b);
        }
    }
}

} // namespace tcpr