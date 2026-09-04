#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "../camera/ray.h"
#include "../util/common.h"


namespace tcpr
{

struct Shape
{
    virtual std::optional<HitInfo> intersect(
        const Ray& ray,
        float      tMin,
        float      tMax
    ) const = 0;
};

struct Sphere : public Shape
{
    Sphere(const glm::vec3& center, float radius) : center(center), radius(radius)
    {
    }

    std::optional<HitInfo> intersect(const Ray& ray, float tMin, float tMax) const override;

    glm::vec3 center;
    float     radius;
};

struct Triangle : public Shape
{
    Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) : p0(p0), p1(p1), p2(p2)
    {
        glm::vec3 e0 = p1 - p0;
        glm::vec3 e1 = p2 - p0;
        glm::vec3 n = glm::normalize(glm::cross(e0, e1));
        n0 = n;
        n1 = n;
        n2 = n;
    }

    Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& n0, const glm::vec3& n1, const glm::vec3& n2)
        : p0(p0), p1(p1), p2(p2), n0(n0), n1(n1), n2(n2)
    {
    }

    std::optional<HitInfo> intersect(const Ray& ray, float tMin, float tMax) const override;

    glm::vec3 p0, p1, p2;
    glm::vec3 n0, n1, n2;
};

struct Plane : public Shape
{
    Plane(const glm::vec3& point, const glm::vec3& normal) : p(point), n(glm::normalize(normal))
    {
    }

    std::optional<HitInfo> intersect(const Ray& ray, float tMin, float tMax) const override;

    glm::vec3 p;
    glm::vec3 n;
};

} // namespace tcpr

#endif // __SHAPE_H__