#pragma once

#include "../IFractalStrategy.hpp"

namespace Raytracer
{

class MandelbulbStrategy : public IFractalStrategy
{
public:
    FractalResult getInfo(const Vector3D& p, int maxIter) const override;

    AABB getLocalBounds() const override {
        return AABB({-1.2, -1.2, -1.2}, {1.2, 1.2, 1.2});
    }
};

} // namespace Raytracer
