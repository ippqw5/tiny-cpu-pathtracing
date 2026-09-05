cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    target_compile_options(TinyCPUPathTracing PRIVATE /W4 /arch:AVX2)
else()
    target_compile_options(TinyCPUPathTracing PRIVATE -Wall -Wextra -Wpedantic -mavx2 -mfma)
endif()

target_compile_definitions(TinyCPUPathTracing PRIVATE
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_FORCE_LEFT_HANDED

    # windows.h defines RGB(), which collides with our RGB class
    NOGDI
)

# Cache var set in preset -> preprocessor macro
if(PROFILE_ON)
    target_compile_definitions(TinyCPUPathTracing PRIVATE PROFILE_ON)
endif()

target_include_directories(TinyCPUPathTracing PUBLIC include)
target_include_directories(TinyCPUPathTracing PRIVATE external/tinybvh)
target_precompile_headers(TinyCPUPathTracing PRIVATE "include/util/common.h")

# tinybvh_impl.cpp compiles the single-header library's implementation; it must
# define TINYBVH_IMPLEMENTATION before including tiny_bvh.h, so it skips the PCH.
set_source_files_properties(src/accel/tinybvh_impl.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_link_libraries(TinyCPUPathTracing glm spdlog::spdlog rapidobj::rapidobj bvh)