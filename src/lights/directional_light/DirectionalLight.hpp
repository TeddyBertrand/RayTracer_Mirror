#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class DirectionalLight : public ILight {
public:
    DirectionalLight(const Color& color, double intensity, const Vector3D& direction)
        : _color(color), _intensity(intensity), _direction(direction.normalized()) {}

    void applyTransform(const Matrix& m) {
        _direction = m * _direction;
        _direction.normalize();
    }

    LightSample computeLight(const HitRecord& hit) const override;

private:
    Color _color;
    double _intensity;
    Vector3D _direction;
};

} // namespace Raytracer