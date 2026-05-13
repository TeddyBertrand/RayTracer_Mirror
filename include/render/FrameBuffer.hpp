#pragma once

#include "math/Color.hpp"
#include <mutex>
#include <vector>

namespace Raytracer {

/**
 * @brief Linear storage for rendered pixel colors.
 *
 * The framebuffer keeps one `Color` per pixel in row-major order.
 */
using FrameBuffer = std::vector<Color>;

inline std::mutex& getFrameBufferWriteMutex() {
    static std::mutex mutex;
    return mutex;
}

} // namespace Raytracer
