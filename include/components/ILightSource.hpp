#pragma once

#include "ILight.hpp"
#include <memory>

namespace Raytracer {

/**
 * @brief Interface for the geometric/radiance part of a light source.
 * This is meant to be decorated by the Light class which handles transformations,
 * global color and intensity.
 */
class ILightSource {
public:
    virtual ~ILightSource() = default;

    /**
     * @brief Sample the light source from a point in local space.
     * 
     * @param local_hit_point The hit point in the light's local space.
     * @return LightSample The sample containing direction, distance and radiance.
     */
    virtual LightSample getSample(const Point3D& local_hit_point) const = 0;
};

} // namespace Raytracer
