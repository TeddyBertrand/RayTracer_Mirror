#include "PointLight.hpp"
#include "factory/LightFactory.hpp"
#include "parser/ISettings.hpp"
#include <algorithm> // Pour std::max si besoin, bien que plus nécessaire ici

namespace Raytracer {

extern "C" const char* getName() { return "point"; }

LightSample PointLight::computeLight(const Point3D& world_hit_point) const {
    Vector3D direction = (_position - world_hit_point);
    double distance = direction.length();

    if (distance > 0) {
        direction /= distance;
    }

    LightSample sample;

    sample.color = _color * _intensity;

    sample.direction = direction;
    sample.distance = distance;
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    return new PointLight(
        settings.getVector("position"), settings.getColor("color"), settings.getFloat("intensity"));
}

} // namespace Raytracer
