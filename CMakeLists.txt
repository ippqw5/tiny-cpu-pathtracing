cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    TinyCPUPathTracing
    LANGUAGES CXX
)

add_subdirectory(external/glm)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS src/*.cpp)
add_executable(TinyCPUPathTracing ${src_files})

target_compile_definitions(TinyCPUPathTracing PRIVATE
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
)

target_include_directories(TinyCPUPathTracing PUBLIC include)
target_precompile_headers(TinyCPUPathTracing PRIVATE "include/common.h")
target_link_libraries(TinyCPUPathTracing glm)