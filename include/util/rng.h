#ifndef RNG_H
#define RNG_H

namespace tcpr
{
class RNG
{
public:
    RNG() : RNG(0, 0.f, 1.f)
    {
    }

    RNG(size_t seed, float min, float max) : m_uniform_dist(min, max)
    {
        setSeed(seed);
    }

    void setSeed(size_t seed)
    {
        m_rng.seed(seed);
    }

    float uniform()
    {
        return m_uniform_dist(m_rng);
    }

private:
    std::mt19937                          m_rng;
    std::uniform_real_distribution<float> m_uniform_dist;
};
} // namespace tcpr

#endif // RNG_H