#pragma once

#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Ray.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer {

/**
 * @brief Interface for geometric objects that can be intersected by rays.
 */
class IPrimitive : public IPlugin {
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~IPrimitive() = default;

    /**
     * @brief Test ray/object intersection and fill a hit record on success.
     *
     * @param r Candidate ray.
     * @param ray_t Valid interval for hit distance along the ray.
     * @param rec Output hit information when an intersection is found.
     * @return true if a valid hit exists in the interval.
     */
    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

} // namespace Raytracer
