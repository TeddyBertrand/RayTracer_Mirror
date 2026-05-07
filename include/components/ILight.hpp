#pragma once

#include "math/Color.hpp"
#include "math/HitRecord.hpp"
#include "math/Matrix.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Result of evaluating a light source at a shading point.
 */
struct LightSample {
    /** Light radiance/intensity contribution. */
    Color color;
    /** Direction from the hit point toward the light. */
    Vector3D direction;
    /** Distance from the hit point to the sampled light position. */
    double distance;
    /** Indicates whether this light sample contributes to shading. */
    bool isActive;
};

/**
 * @brief Interface for light emitters used during direct lighting.
 */
class ILight {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~ILight() = default;

    /**
     * @brief Apply a transformation matrix to the light's position and direction.
     *
     * @param m The transformation matrix (Translation, Rotation, Scale) to apply.
     */
    virtual void applyTransform(const Matrix& m) = 0;

    /**
     * @brief Evaluate this light from a given point in space.
     *
     * @param hit_point Shading position in world space.
     * @return LightSample containing direction, distance and emitted color data.
     */
    virtual LightSample computeLight(const Point3D& hit_point) const = 0;
};

} // namespace Raytracer
