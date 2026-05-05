#pragma once
#include "ILight.hpp"
#include "ILightSource.hpp"
#include "math/Matrix.hpp"

namespace Raytracer {
class Light : public ILight {
public:
    Light(std::shared_ptr<ILightSource> source,
          Color color = Color(255, 255, 255),
          double intensity = 1.0)
        : _source(std::move(source)), _color(color), _intensity(intensity) {}

    void setTransform(const Matrix& m) {
        _transform = m;
        _transform_inv = m.inverse();
    }

    LightSample computeLight(const Point3D& world_hit_point) const override {
        Point3D local_hit = _transform_inv * world_hit_point;

        LightSample sample = _source->getSample(local_hit);

        sample.direction = _transform.transformDirection(sample.direction).normalized();

        sample.color = sample.color * _color * _intensity;

        return sample;
    }

    void setColor(const Color& c) override { _color = c; }
    void setIntensity(double i) override { _intensity = i; }

private:
    std::shared_ptr<ILightSource> _source;
    Color _color;
    double _intensity;
    Matrix _transform;
    Matrix _transform_inv;
};
} // namespace Raytracer
