#include "shape/model.h"
#include "shape/shape.h"
#include "util/profile.h"

namespace tcpr
{

Model::Model(const std::filesystem::path& path)
{
    PROFILE_SCOPE("Model::Model(" + path.string() + ")");

    rapidobj::Result result = rapidobj::ParseFile(path);
    if (result.error)
    {
        throw std::runtime_error("Failed to load file: " + path.string() + " (" + result.error.code.message() + ")");
    }
    if (!rapidobj::Triangulate(result))
    {
        throw std::runtime_error("Failed to triangulate: " + path.string());
    }

    const auto& positions = result.attributes.positions;
    const auto& normals = result.attributes.normals;

    auto to_vec3 = [](const rapidobj::Array<float>& arr, int index) {
        return glm::vec3{arr[3 * index], arr[3 * index + 1], arr[3 * index + 2]};
    };

    for (const auto& shape : result.shapes)
    {
        const auto& mesh = shape.mesh;
        // after Triangulate, indices come in groups of three
        for (size_t i = 0; i < mesh.indices.size(); i += 3)
        {
            const auto& i0 = mesh.indices[i];
            const auto& i1 = mesh.indices[i + 1];
            const auto& i2 = mesh.indices[i + 2];

            const glm::vec3 p0 = to_vec3(positions, i0.position_index);
            const glm::vec3 p1 = to_vec3(positions, i1.position_index);
            const glm::vec3 p2 = to_vec3(positions, i2.position_index);

            if (i0.normal_index >= 0)
            {
                m_triangles.emplace_back(p0, p1, p2,
                                         to_vec3(normals, i0.normal_index),
                                         to_vec3(normals, i1.normal_index),
                                         to_vec3(normals, i2.normal_index));
            }
            else
            {
                // no vertex normals: the 3-point ctor computes a flat normal
                m_triangles.emplace_back(p0, p1, p2);
            }
        }
    }

    build();
}

void Model::build()
{
    m_aabb = AABB{};
    for (const auto& tri : m_triangles)
    {
        m_aabb.expand(tri.p0);
        m_aabb.expand(tri.p1);
        m_aabb.expand(tri.p2);
    }
}

std::optional<HitInfo> Model::intersect(const Ray& ray, float t_min, float t_max) const
{
    if (!m_aabb.intersect(ray, t_min, t_max))
    {
        return std::nullopt;
    }

    std::optional<HitInfo> closest_hit_info{};
    for (const auto& tri : m_triangles)
    {
        auto hit_info = tri.intersect(ray, t_min, t_max);
        if (hit_info.has_value())
        {
            t_max = hit_info->t;
            closest_hit_info = hit_info;
        }
    }
    return closest_hit_info;
}

} // namespace tcpr