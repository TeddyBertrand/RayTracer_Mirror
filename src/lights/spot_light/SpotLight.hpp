#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/MathUtils.hpp"
#include "math/Vector3D.hpp"
#include <cmath>

namespace Raytracer {

class SpotLight : public ILight {
public:
    SpotLight(const Vector3D& position,
              const Vector3D& direction,
              const Color& color,
              double intensity,
              double cutoffDeg)
        : _position(position), _direction(direction.normalized()), _color(color),
          _intensity(intensity) {

        double rad = Math::degreesToRadians(cutoffDeg * 0.5);
        _cutoff = std::cos(rad);
    }

    void applyTransform(const Matrix& m) override {
        _position = m * _position;
        _direction = (m * _direction).normalized();
    }

    LightSample computeLight(const HitRecord& hit) const override;

private:
    Vector3D _position;
    Vector3D _direction;
    Color _color;
    double _intensity;
    double _cutoff;
};

} // namespace Raytracer
