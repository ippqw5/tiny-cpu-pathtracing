#include "shape.h"
#include "glm/geometric.hpp"
#include "ray.h"

namespace tcpr
{

std::optional<HitInfo> Sphere::intersect(const Ray& ray, float tMin, float tMax) const
{
    glm::vec3 oc = ray.origin - center;
    float     b = 2.0f * glm::dot(ray.direction, oc);
    float     c = glm::dot(oc, oc) - radius * radius;
    float     delta = b * b - 4 * c;

    if (delta < 0)
        return {};
    float t = (-b - glm::sqrt(delta)) * 0.5f;
    if (t < 0.f)
        t = (-b + glm::sqrt(delta)) * 0.5f;
    if (t > tMin && t < tMax)
    {
        glm::vec3 p = ray.at(t);
        glm::vec3 n = glm::normalize(p - center);
        return HitInfo{.t = t, .p = p, .n = n};
    }
    return {};
}

std::optional<HitInfo> Triangle::intersect(const Ray& ray, float tMin, float tMax) const
{
    glm::vec3 e1 = p1 - p0;
    glm::vec3 e2 = p2 - p0;

    glm::vec3 s = ray.origin - p0;

    glm::vec3 s1 = glm::cross(ray.direction, e2);
    glm::vec3 s2 = glm::cross(s, e1);
    float     inv_det = 1.f / glm::dot(s1, e1);

    float u = glm::dot(s1, s) * inv_det;
    if (u < 0 || u > 1)
        return {};
    float v = glm::dot(s2, ray.direction) * inv_det;
    if (v < 0 || u + v > 1)
        return {};

    float t = glm::dot(s2, e2) * inv_det;
    if (t > tMin && t < tMax)
    {
        glm::vec3 p = ray.at(t);
        glm::vec3 n = glm::normalize((1.f - u - v) * n0 + u * n1 + v * n2);
        return HitInfo{.t = t, .p = p, .n = n};
    }

    return {};
}

} // namespace tcpr