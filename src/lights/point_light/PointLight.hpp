#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

extern "C" std::string getName() { return "sphere"; }

class PointLight : public ILight {
public:
    PointLight(const Vector3D& position, const Color& color, double intensity)
        : _position(position), _color(color), _intensity(intensity) {}

    LightSample computeLight(const Point3D& hit_point) const override;

private:
    Vector3D _position;
    Color _color;
    double _intensity;
};

} // namespace Raytracer
