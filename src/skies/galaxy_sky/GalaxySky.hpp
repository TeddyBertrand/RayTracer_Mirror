#pragma once

#include "skies/commun/ASky.hpp"

namespace Raytracer {

class GalaxySky : public ASky {
public:
    GalaxySky(double starDensity = 0.001,
              const Color& nebulaColor = Color(0.025, 0.01, 0.05),
              double nebulaDensity = 0.7,
              double nebulaContrast = 3.0)
        : _starDensity(starDensity), _nebulaColor(nebulaColor), _nebulaDensity(nebulaDensity),
          _nebulaContrast(nebulaContrast) {}

    Color getBackgroundColor(const Ray& r) const override;
    Color getEnvironmentColor(const Ray& r) const override;

    std::string getName() const override { return "galaxy_sky"; }

private:
    double _starDensity;
    Color _nebulaColor;
    double _nebulaDensity;
    double _nebulaContrast;

    double hash(Vector3D v) const;
    double noise(Vector3D v) const;
};

} // namespace Raytracer
