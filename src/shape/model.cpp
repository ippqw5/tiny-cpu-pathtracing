#include "shape/model.h"
#include "shape/shape.h"
#include "util/profile.h"

namespace tcpr
{

Model::Model(const std::filesystem::path& path)
{
    PROFILE_SCOPE("Model::Model(" + path.string() + ")");
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;

    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::string line;
    char        trash = 0;
    while (!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line);

        if (line.starts_with("v "))
        {
            glm::vec3 position;
            iss >> trash >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (line.starts_with("vn "))
        {
            glm::vec3 normal;
            iss >> trash >> trash >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (line.starts_with("f "))
        {
            glm::ivec3 idx_v;
            glm::ivec3 idx_vn;
            iss >> trash;
            iss >> idx_v.x >> trash >> trash >> idx_vn.x;
            iss >> idx_v.y >> trash >> trash >> idx_vn.y;
            iss >> idx_v.z >> trash >> trash >> idx_vn.z;
            m_triangles.emplace_back(positions[idx_v.x - 1], positions[idx_v.y - 1], positions[idx_v.z - 1],
                                           normals[idx_vn.x - 1], normals[idx_vn.y - 1], normals[idx_vn.z - 1]);
        }
    }
}

std::optional<HitInfo> Model::intersect(const Ray& ray, float t_min, float t_max) const
{
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