#ifndef __PROFILE_H__
#define __PROFILE_H__

#include "./common.h"

#define PROFILE_SCOPE(name) tcpr::Profile __profile(name)

namespace tcpr
{

class Profile
{
public:
    Profile() : Profile("dummy") {};

    Profile(const std::string& name)
        : m_name(name), m_start_time(std::chrono::high_resolution_clock::now())
    {
    }

    ~Profile()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_start_time).count();
        std::cout << "Profile [" << m_name << "] took " << float(duration) / 1000.0f << " seconds. \n"
                  << std::endl;
    }

private:
    std::string                                    m_name;
    std::chrono::high_resolution_clock::time_point m_start_time;
};

} // namespace tcpr

#endif // __PROFILE_H__