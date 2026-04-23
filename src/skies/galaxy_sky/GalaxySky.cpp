#include "GalaxySky.hpp"
#include <algorithm>
#include <cmath>

namespace Raytracer {

double GalaxySky::hash(Vector3D v) const {
    double dot = v.x * 12.9898 + v.y * 78.233 + v.z * 37.719;
    return std::abs(std::fmod(std::sin(dot) * 43758.5453123, 1.0));
}

double GalaxySky::noise(Vector3D v) const {
    Vector3D i(std::floor(v.x), std::floor(v.y), std::floor(v.z));
    Vector3D f(v.x - i.x, v.y - i.y, v.z - i.z);

    f.x = f.x * f.x * (3.0 - 2.0 * f.x);
    f.y = f.y * f.y * (3.0 - 2.0 * f.y);
    f.z = f.z * f.z * (3.0 - 2.0 * f.z);

    double n = hash(i + Vector3D(0, 0, 0)) * (1 - f.x) * (1 - f.y) * (1 - f.z) +
               hash(i + Vector3D(1, 0, 0)) * f.x * (1 - f.y) * (1 - f.z) +
               hash(i + Vector3D(0, 1, 0)) * (1 - f.x) * f.y * (1 - f.z) +
               hash(i + Vector3D(1, 1, 0)) * f.x * f.y * (1 - f.z) +
               hash(i + Vector3D(0, 0, 1)) * (1 - f.x) * (1 - f.y) * f.z +
               hash(i + Vector3D(1, 0, 1)) * f.x * (1 - f.y) * f.z +
               hash(i + Vector3D(0, 1, 1)) * (1 - f.x) * f.y * f.z +
               hash(i + Vector3D(1, 1, 1)) * f.x * f.y * f.z;
    return n;
}

Color GalaxySky::getBackgroundColor(const Ray& r) const {
    Vector3D unit_dir = r.direction().normalized();

    double fbm = 0.0;
    double amplitude = 0.5;
    Vector3D p = unit_dir * 3.0;

    for (int i = 0; i < 4; ++i) {
        fbm += amplitude * noise(p);
        p = p * 2.0;
        amplitude *= 0.5;
    }

    double threshold = 1.0 - _nebulaDensity;
    double nebulaValue = std::max(0.0, fbm - threshold);
    double nebulaStrength = std::pow(nebulaValue, _nebulaContrast);

    Color finalColor = _nebulaColor * nebulaStrength;

    double gridSize = 1000.0;
    Vector3D cell(std::floor(unit_dir.x * gridSize),
                  std::floor(unit_dir.y * gridSize),
                  std::floor(unit_dir.z * gridSize));

    double starRand = hash(cell);

    if (starRand > 1.0 - _starDensity) {
        double intensity = (starRand - (1.0 - _starDensity)) / _starDensity;
        return finalColor + Color(1.0, 1.0, 1.0) * std::pow(intensity, 2.0);
    }

    return finalColor;
}

Color GalaxySky::getEnvironmentColor([[maybe_unused]] const Ray& r) const {
    return _nebulaColor * 0.8;
}

} // namespace Raytracer
