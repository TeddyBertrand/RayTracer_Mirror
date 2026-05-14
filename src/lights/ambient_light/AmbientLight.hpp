#pragma once

#include "components/ILight.hpp"
#include "math/Color.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

class AmbientLight : public ILight {
public:
    // L'ambiance n'a besoin que d'une couleur et d'une intensité
    AmbientLight(const Color& color, double intensity) : _color(color), _intensity(intensity) {}

    // La lumière ambiante est partout, les transformations ne l'affectent pas
    void applyTransform(const Matrix& m) override { (void)m; }

    LightSample computeLight(const HitRecord& hit) const override;

private:
    Color _color;
    double _intensity;
};

} // namespace Raytracer
