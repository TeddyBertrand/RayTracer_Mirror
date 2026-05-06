#pragma once

#include "components/IPrimitive.hpp"
#include "strategy/IFractalStrategy.hpp"

namespace Raytracer {

class Fractal : public IPrimitive {
public:
    Fractal(std::unique_ptr<IFractalStrategy> strategy,
            std::shared_ptr<IMaterial> material,
            int maxIterations)
        : _strategy(std::move(strategy)), _material(material), _max_iterations(maxIterations) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    AABB getBoundingBox() const override { return _strategy->getLocalBounds(); }

private:
    Vector3D computeNormal(const Vector3D& p) const;

private:
    std::unique_ptr<IFractalStrategy> _strategy;
    std::shared_ptr<IMaterial> _material;
    int _max_iterations;
};

} // namespace Raytracer
