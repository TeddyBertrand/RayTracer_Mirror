#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class DirectionalLight : public ILight {
public:
    DirectionalLight(const Color& color, double intensity) : _intensity(intensity) {
        _color.r = color.r / 255.0;
        _color.g = color.g / 255.0;
        _color.b = color.b / 255.0;

        _direction = Vector3D(0, -1, 0);
    }

    void applyTransform(const Matrix& m) {
        _direction = m * _direction;
        _direction.normalize();
    }

    LightSample computeLight(const Point3D& world_hit_point) const override;

private:
    Vector3D _direction;
    Color _color;
    double _intensity;
};

} // namespace Raytracer
