#include "PointLight.hpp"
#include "builder/LightBuilder.hpp"
#include "factory/LightFactory.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {

extern "C" const char* getName() { return "point"; }

LightSample PointLight::getSample(const Point3D& local_hit_point) const {
    Vector3D direction = -local_hit_point;
    double distance_squared = direction.lengthSquared();
    direction.normalize();

    LightSample sample;
    double attenuation = 1.0 / std::max(1.0, distance_squared);

    sample.color = Color(1.0, 1.0, 1.0) * attenuation;
    sample.direction = direction;
    sample.distance = std::sqrt(distance_squared);
    sample.isActive = true;

    return sample;
}

extern "C" ILight* createPlugin(const ISetting& settings) {
    auto source = std::make_shared<PointLight>();

    return LightBuilder(source).parseTransform(settings).parseCommon(settings).build();
}

} // namespace Raytracer
