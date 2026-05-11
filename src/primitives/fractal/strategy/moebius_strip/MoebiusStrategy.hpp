#pragma once

#include "../IFractalStrategy.hpp"

namespace Raytracer {

class MoebiusStrategy : public IFractalStrategy {
public:
    FractalResult getInfo(const Vector3D& p, int maxIter) const override;

    AABB getLocalBounds() const override { return AABB({-1.5, -0.5, -1.5}, {1.5, 0.5, 1.5}); }
};

} // namespace Raytracer
