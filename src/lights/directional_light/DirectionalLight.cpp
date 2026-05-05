#include "DirectionalLight.hpp"
#include "builder/LightBuilder.hpp"
#include "parser/ISettings.hpp"
#include <limits>

namespace Raytracer {

extern "C" const char* getName() { return "directional"; }

LightSample DirectionalLight::getSample(const Point3D& /*local_hit_point*/) const {
    LightSample sample;

    sample.direction = Vector3D(0, 1, 0);
    sample.color = Color(1.0, 1.0, 1.0);
    sample.distance = std::numeric_limits<double>::max();
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    auto source = std::make_shared<DirectionalLight>();

    return LightBuilder(source)
        .parseCommon(settings)
        .parseTransform(settings)
        .build();
}

} // namespace Raytracer
