#include "accel/accel.h"
#include "accel/aabb_accel.h"
#include "accel/madmann_accel.h"
#include "accel/tinybvh_accel.h"

namespace tcpr
{

std::unique_ptr<Accel> createAccel(AccelType type)
{
    switch (type)
    {
    case AccelType::AABB:
        return std::make_unique<AABBAccel>();
    case AccelType::Madmann:
        return std::make_unique<MadmannAccel>();
    case AccelType::Tinybvh:
        return std::make_unique<TinybvhAccel>();
    }
    return nullptr;
}

} // namespace tcpr
