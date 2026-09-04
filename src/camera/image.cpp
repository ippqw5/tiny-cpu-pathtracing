#include "camera/image.h"
#include "thread/thread_pool.h"
#include "util/profile.h"
#include "util/rgb.h"

namespace tcpr
{
Image::Image(size_t width, size_t height) : m_width(width), m_height(height)
{
    m_pixels.resize(width * height);
}

void Image::save(const std::filesystem::path& filename)
{
    PROFILE_SCOPE("Image::save(" + filename.string() + ")");

    // each pixel writes its own 3 bytes, so no data race
    std::vector<uint8_t> buffer(m_width * m_height * 3);

    ThreadPool::getInstance().parallelFor(m_width, m_height, [&](size_t x, size_t y) {
        const Pixel pixel = getPixel(x, y);
        const RGB   color(pixel.color / static_cast<float>(pixel.sample_count));

        const size_t offset = (y * m_width + x) * 3;
        buffer[offset] = static_cast<uint8_t>(color.r);
        buffer[offset + 1] = static_cast<uint8_t>(color.g);
        buffer[offset + 2] = static_cast<uint8_t>(color.b);
    });
    ThreadPool::getInstance().wait();

    // PPM
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n"
         << m_width << ' ' << m_height << "\n255\n";
    file.write(reinterpret_cast<const char*>(buffer.data()),
               static_cast<std::streamsize>(buffer.size()));
}

} // namespace tcpr