#ifndef RENDERER_H
#define RENDERER_H

#include "../camera/camera.h"
#include "../shape/scene.h"
#include "../util/rng.h"

namespace tcpr
{

class Renderer
{
public:
    Renderer(Camera& camera, Scene& scene) : m_camera(camera), m_scene(scene)
    {
    }

    void render(size_t spp, const std::filesystem::path& filename);

    virtual glm::vec3 renderPixel(glm::ivec2 pixel_coord, RNG& rng);

    virtual ~Renderer()
    {
    }

private:
    Camera& m_camera;
    Scene&  m_scene;
};

} // namespace tcpr

#endif // RENDERER_H