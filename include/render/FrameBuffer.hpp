#pragma once

#include "math/Color.hpp"
#include <vector>

namespace Raytracer {

/**
 * @brief Linear storage for rendered pixel colors.
 *
 * The framebuffer keeps one `Color` per pixel in row-major order.
 */
using FrameBuffer = std::vector<Color>;

} // namespace Raytracer
