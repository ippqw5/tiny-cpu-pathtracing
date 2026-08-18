#include "model.h"
#include "shape.h"
#include <fstream>
#include <iostream>
#include <sstream>


namespace tcpr
{

Model::Model(const std::filesystem::path& path)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;

    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::string line;
    char        trash;
    while (!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line);

        if (line.compare(0, 2, "v ") == 0)
        {
            glm::vec3 position;
            iss >> trash >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            glm::vec3 normal;
            iss >> trash >> trash >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            glm::ivec3 idx_v, idx_vn;
            iss >> trash;
            iss >> idx_v.x >> trash >> trash >> idx_vn.x;
            iss >> idx_v.y >> trash >> trash >> idx_vn.y;
            iss >> idx_v.z >> trash >> trash >> idx_vn.z;
            mTriangles.push_back(Triangle(positions[idx_v.x - 1], positions[idx_v.y - 1], positions[idx_v.z - 1],
                                          normals[idx_vn.x - 1], normals[idx_vn.y - 1], normals[idx_vn.z - 1]));
        }
    }
}

std::optional<HitInfo> Model::intersect(const Ray& ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closestHitInfo{};
    for (const auto& tri : mTriangles)
    {
        auto hitInfo = tri.intersect(ray, tMin, tMax);
        if (hitInfo.has_value())
        {
            tMax = hitInfo->t;
            closestHitInfo = hitInfo;
        }
    }
    return closestHitInfo;
}

} // namespace tcpr