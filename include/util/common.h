#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <ostream>
#include <queue>
#include <random>
#include <span>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <rapidobj/rapidobj.hpp>

#include <bvh/v2/bvh.h>
#include <bvh/v2/default_builder.h>
#include <bvh/v2/executor.h>
#include <bvh/v2/node.h>
#include <bvh/v2/ray.h>
#include <bvh/v2/stack.h>
#include <bvh/v2/thread_pool.h>
#include <bvh/v2/tri.h>
#include <bvh/v2/vec.h>

#endif // COMMON_H