#include "DirectionalLight.hpp"
#include "math/MathUtils.hpp"
#include "parser/ISettings.hpp"
#include <limits>

namespace Raytracer {

extern "C" const char* getName() { return "directional"; }

LightSample DirectionalLight::computeLight(const Point3D& world_hit_point) const {
    LightSample sample;

    sample.direction = -_direction;
    sample.color = _color * _intensity;
    sample.distance = 1e6;
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    Color rawColor = settings.getColor("color");
    Color normalizedColor(rawColor.r / 255.0, rawColor.g / 255.0, rawColor.b / 255.0);

    return new DirectionalLight(normalizedColor, settings.getFloat("intensity"));
}

} // namespace Raytracer
