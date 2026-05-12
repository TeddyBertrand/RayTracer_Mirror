#include "SpotLight.hpp"
#include "factory/LightFactory.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" const char* getName() { return "spot"; }

LightSample SpotLight::computeLight(const Point3D& world_hit_point) const {
    Vector3D lightToPoint = (world_hit_point - _position);
    double distance = lightToPoint.length();

    if (distance > 0) {
        lightToPoint /= distance;
    }

    double cosTheta = lightToPoint.dot(_direction);

    LightSample sample;
    sample.distance = distance;
    sample.direction = -lightToPoint;

    if (cosTheta > _cutoff) {
        double attenuation = 1.0 / (1.0 + 0.1 * distance);

        sample.color = _color * (_intensity * attenuation);
        sample.isActive = true;
    } else {
        sample.color = Color(0, 0, 0);
        sample.isActive = false;
    }

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    return new SpotLight(settings.getVector("position"),
                         settings.getVector("direction", Vector3D(0, -1, 0)),
                         settings.getColor("color", Color(255, 255, 255)),
                         settings.getFloat("intensity", 1.0),
                         settings.getFloat("cutoff", 45.0));
}

} // namespace Raytracer
