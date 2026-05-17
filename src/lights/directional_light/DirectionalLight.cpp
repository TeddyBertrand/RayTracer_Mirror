#include "DirectionalLight.hpp"
#include "math/MathUtils.hpp"
#include "parser/ISettings.hpp"
#include <cmath>
#include <limits>

namespace Raytracer {

extern "C" const char* getName() { return "directional"; }

LightSample DirectionalLight::computeLight([[maybe_unused]] const HitRecord& hit) const {
    LightSample sample;

    sample.direction = -_direction;
    sample.color = _color * _intensity;
    sample.distance = 1e6; // Distance "infinie"
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    Color rawColor = settings.getColor("color");
    double intensity = settings.getFloat("intensity", 1.0);

    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));

    double pitch = Math::degreesToRadians(rot.x);
    double yaw = Math::degreesToRadians(rot.y);

    double dx = std::sin(yaw) * std::cos(pitch);
    double dy = -std::cos(yaw) * std::cos(pitch);
    double dz = std::sin(pitch);

    Vector3D dir(dx, dy, dz);

    return new DirectionalLight(rawColor, intensity, dir.normalized());
}

} // namespace Raytracer
