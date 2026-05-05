#pragma once

#include "../IFractalStrategy.hpp"

namespace Raytracer {

class MengerSpongeStrategy : public IFractalStrategy {
public:
    FractalResult getInfo(const Vector3D& p, int maxIter) const override;

    AABB getLocalBounds() const override {
        return AABB({-1.1, -1.1, -1.1}, {1.1, 1.1, 1.1});
    }

private:
    double sdBox(Vector3D p, Vector3D b) const;
};

} // namespace Raytracer
