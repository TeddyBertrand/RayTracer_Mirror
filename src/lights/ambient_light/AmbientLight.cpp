#include "AmbientLight.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" const char* getName() { return "ambient"; }
#include "AmbientLight.hpp"

LightSample AmbientLight::computeLight(const HitRecord& hit) const {
    LightSample sample;

    sample.direction = hit.normal;

    sample.color = _color * _intensity;
    sample.distance = 0.0;
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    Color rawColor = settings.getColor("color");
    double intensity = settings.getFloat("intensity", 1.0);

    return new AmbientLight(rawColor, intensity);
}

} // namespace Raytracer
