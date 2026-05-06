#include "Fractal.hpp"
#include "FractalStrategyFactory.hpp"
#include "builder/EntityBuilder.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "fractal"; }

IPrimitive* createPlugin(const ISetting& settings) {
    std::string type = settings.getString("fractal_type");
    std::unique_ptr<IFractalStrategy> strategy = FractalStrategyFactory::create(type, settings);

    auto fractal = std::make_shared<Fractal>(
        std::move(strategy), nullptr, settings.getInt("max_iterations", 1000));

    auto entity = EntityBuilder(settings)
                      .setPrimitive(fractal)
                      .parseTransform(settings)
                      .parseMaterial(settings)
                      .build();

    return entity.release();
}
}

bool Fractal::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    double t = ray_t.min;
    const double precision = 0.001;
    const int max_steps = 256;

    for (int i = 0; i < max_steps; ++i) {
        Vector3D current_p = r.at(t);

        FractalResult res = _strategy->getInfo(current_p, _max_iterations);
        if (res.distance < precision) {
            rec.t = t;
            rec.point = current_p;
            rec.normal = computeNormal(current_p);
            rec.material = _material;
            rec.fractal_info = res.iterationIndex;
            return true;
        }

        t += res.distance;

        if (t > ray_t.max)
            break;
    }

    return false;
}

Vector3D Fractal::computeNormal(const Vector3D& p) const {
    const double e = 0.001;

    double dx = _strategy->getInfo({p.x + e, p.y, p.z}, _max_iterations).distance -
                _strategy->getInfo({p.x - e, p.y, p.z}, _max_iterations).distance;

    double dy = _strategy->getInfo({p.x, p.y + e, p.z}, _max_iterations).distance -
                _strategy->getInfo({p.x, p.y - e, p.z}, _max_iterations).distance;

    double dz = _strategy->getInfo({p.x, p.y, p.z + e}, _max_iterations).distance -
                _strategy->getInfo({p.x, p.y, p.z - e}, _max_iterations).distance;

    return Vector3D(dx, dy, dz).normalized();
}

} // namespace Raytracer
