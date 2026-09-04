cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#Default build                          type for bare `cmake..` runs; does not override the
#CMAKE_BUILD_TYPE passed on the command line(e.g.by CMake Tools variants).
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

project(
    TinyCPUPathTracing
    LANGUAGES CXX
)

add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/rapidobj)
add_subdirectory(external/bvh)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS src/*.cpp)
add_executable(TinyCPUPathTracing ${src_files})

# enable compile warnings
if(MSVC)
    target_compile_options(TinyCPUPathTracing PRIVATE /W4)
else()
    target_compile_options(TinyCPUPathTracing PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(TinyCPUPathTracing PRIVATE
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_FORCE_LEFT_HANDED
    # windows.h defines RGB(), which collides with our RGB class
    NOGDI
)

target_include_directories(TinyCPUPathTracing PUBLIC include)
target_precompile_headers(TinyCPUPathTracing PRIVATE "include/util/common.h")
target_link_libraries(TinyCPUPathTracing glm spdlog::spdlog rapidobj::rapidobj bvh)