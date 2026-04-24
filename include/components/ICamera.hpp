#pragma once

#include "math/Ray.hpp"

namespace Raytracer {

/**
 * @brief Camera interface used by the renderer to generate primary rays.
 *
 * Implementations map normalized image-plane coordinates to rays in world space.
 * The renderer calls this interface for each pixel sample to determine the
 * direction from which scene radiance is queried.
 */
class ICamera {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~ICamera() = default;

    /**
     * @brief Build a ray passing through normalized image coordinates.
     *
     * @param u Horizontal coordinate in normalized image space.
     * @param v Vertical coordinate in normalized image space.
     * @return Ray starting from the camera origin toward the sampled direction.
     */
    virtual Ray getRay(double u, double v) const = 0;

    /**
     * @brief Return the output image width in pixels.
     */
    virtual int getWidth() const = 0;

    /**
     * @brief Return the output image height in pixels.
     */
    virtual int getHeight() const = 0;
};

} // namespace Raytracer
