#include "shape.h"
#include "ray.h"

namespace tcpr
{

std::optional<HitInfo> Sphere::intersect(const Ray& ray, float tMin, float tMax) const
{
    glm::vec3 oc = ray.ori - c;
    float     b = 2.0f * glm::dot(ray.dir, oc);
    float     c = glm::dot(oc, oc) - r * r;
    float     delta = b * b - 4 * c;

    if (delta < 0)
        return {};
    float t = (-b - glm::sqrt(delta)) * 0.5f;
    if (t < 0.f)
        t = (-b + glm::sqrt(delta)) * 0.5f;
    if (t > tMin && t < tMax)
    {
        glm::vec3 p = ray.at(t);
        glm::vec3 n = glm::normalize(p - c);
        return HitInfo{.t = t, .p = p, .n = n};
    }
    return {};
}

std::optional<HitInfo> Triangle::intersect(const Ray& ray, float tMin, float tMax) const
{
    glm::vec3 e1 = p1 - p0;
    glm::vec3 e2 = p2 - p0;

    glm::vec3 s = ray.ori - p0;

    glm::vec3 s1 = glm::cross(ray.dir, e2);
    glm::vec3 s2 = glm::cross(s, e1);
    float     inv_det = 1.f / glm::dot(s1, e1);

    float u = glm::dot(s1, s) * inv_det;
    if (u < 0 || u > 1)
        return {};
    float v = glm::dot(s2, ray.dir) * inv_det;
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

std::optional<HitInfo> Plane::intersect(const Ray& ray, float tMin, float tMax) const
{
    float t = glm::dot(p - ray.ori, n) / glm::dot(ray.dir, n);
    if (t > tMin && t < tMax)
    {
        glm::vec3 p = ray.at(t);
        return HitInfo{.t = t, .p = p, .n = n};
    }
    return {};
}

} // namespace tcpr