#ifndef PROFILE_H
#define PROFILE_H

#include "./common.h"

#define PROFILE_SCOPE(name) tcpr::Profile __profile(name)

namespace tcpr
{

class Profile
{
public:
    Profile() : Profile("dummy") {};

    Profile(std::string name)
        : m_name(std::move(name)), m_start_time(std::chrono::high_resolution_clock::now())
    {
    }

    ~Profile()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_start_time).count();
        logger().info("Profile [{}] took {:.3f} seconds.", m_name, static_cast<float>(duration) / 1000.0f);
    }

private:
    static spdlog::logger& logger()
    {
        static const std::shared_ptr<spdlog::logger> instance = [] {
            try
            {
                auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("profile.log", true);
                auto file_logger = std::make_shared<spdlog::logger>("profile", std::move(sink));
                file_logger->flush_on(spdlog::level::info);
                return file_logger;
            }
            catch (const spdlog::spdlog_ex&)
            {
                return spdlog::default_logger();
            }
        }();
        return *instance;
    }

    std::string                                    m_name;
    std::chrono::high_resolution_clock::time_point m_start_time;
};

} // namespace tcpr

#endif // PROFILE_H