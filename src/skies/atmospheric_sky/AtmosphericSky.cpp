#include "AtmosphericSky.hpp"
#include "math/Color.hpp"
#include "parser/ISettings.hpp"

namespace Raytracer {
extern "C" {
const char* getName() { return "atmospheric"; }

ISky* createPlugin(const ISetting& settings) {
    Color groundColor = settings.getColor("groundColor", Color(0.5, 0.7, 1.0));
    Color zenithColor = settings.getColor("zenithColor", Color(1.0, 1.0, 1.0));
    return new AtmosphericSky(groundColor, zenithColor);
}

Color AtmosphericSky::getBackgroundColor(const Ray& r) const {
    Vector3D unit_direction = r.direction().normalized();

    double t = 0.5 * (unit_direction.y + 1.0);

    return _groundColor * (1.0 - t) + _zenithColor * t;
}
}

} // namespace Raytracer
