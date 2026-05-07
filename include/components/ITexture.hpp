#pragma once

#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Interface for texture mapping.
 * Allows materials to sample colors based on UV coordinates.
 */
class ITexture {
public:
    virtual ~ITexture() = default;

    /**
     * @brief Get the color at given UV coordinates.
     * @param u Horizontal coordinate [0, 1].
     * @param v Vertical coordinate [0, 1].
     * @return The color at the specified mapping.
     */
    [[nodiscard]] virtual Color value(double u, double v, const Vector3D& p) const noexcept = 0;
};

} // namespace Raytracer
