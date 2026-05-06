#pragma once

#include "math/Vector3D.hpp"
#include "math/AABB.hpp"

namespace Raytracer {

/**
 * Return structure to couple the distance and iteration index.
 */
struct FractalResult {
    double distance;
    double iterationIndex;
};

class IFractalStrategy {
public:
    virtual ~IFractalStrategy() = default;

    /**
     * getInfo : The core method of the Strategy Pattern.
     * @param p The current point where the ray is located.
     * @param maxIter The iteration limit (passed by the config).
     * @return A FractalResult containing the safety distance.
     */
    virtual FractalResult getInfo(const Vector3D& p, int maxIter) const = 0;

    virtual AABB getLocalBounds() const = 0;
};

} // namespace Raytracer
