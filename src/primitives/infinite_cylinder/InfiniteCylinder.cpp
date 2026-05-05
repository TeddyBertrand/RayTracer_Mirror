#include "InfiniteCylinder.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <cmath>

namespace Raytracer {

extern "C" {

const char* getName() { return "infinite_cylinder"; }

IPrimitive* createPlugin(const ISetting& settings) {
    auto prim = std::make_shared<InfiniteCylinder>();

    EntityBuilder builder(settings);

    // Allow scaling radius via config; keep Y scale = 1 to preserve infinity
    if (settings.exists("radius")) {
        const double radius = settings.getFloat("radius", 1.0);
        builder.addScale(radius, 1.0, radius);
    }

    std::unique_ptr<Entity> entity =
        builder.setPrimitive(prim).parseTransform(settings).parseMaterial(settings).build();

    return entity.release();
}

} // extern "C"

namespace {

void getInfiniteCylinderUV(const Raytracer::Vector3D& p,
                           const Raytracer::Vector3D& center,
                           double& u,
                           double& v) {
    double theta = std::atan2(-(p.z - center.z), p.x - center.x) + M_PI;
    u = theta / (2.0 * M_PI);

    // Tile V along Y by taking fractional part of Y coordinate
    double raw_v = (p.y - center.y);
    raw_v = raw_v - std::floor(raw_v);
    v = raw_v;
}

} // namespace

bool InfiniteCylinder::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    const double kEpsilon = 1e-12;

    const double dx = r.direction().x;
    const double dz = r.direction().z;
    const double ox = r.origin().x - _center.x;
    const double oz = r.origin().z - _center.z;

    const double a = dx * dx + dz * dz;
    if (std::abs(a) < kEpsilon)
        return false; // Ray parallel to cylinder axis in XZ plane

    const double b = 2.0 * (dx * ox + dz * oz);
    const double c = (ox * ox + oz * oz) - (_radius * _radius);

    // Quadratic solve
    double discr = b * b - 4.0 * a * c;
    if (discr < 0.0)
        return false;

    double sqrt_discr = std::sqrt(discr);
    double t0 = (-b - sqrt_discr) / (2.0 * a);
    double t1 = (-b + sqrt_discr) / (2.0 * a);
    if (t0 > t1)
        std::swap(t0, t1);

    // Choose smallest valid t
    double t =
        (t0 >= ray_t.min) ? t0 : ((t1 >= ray_t.min) ? t1 : std::numeric_limits<double>::infinity());
    if (t > ray_t.max)
        return false;

    rec.t = t;
    rec.point = r.at(t);

    Vector3D outward_normal(rec.point.x - _center.x, 0.0, rec.point.z - _center.z);
    outward_normal.normalize();
    rec.setFaceNormal(r, outward_normal);

    getInfiniteCylinderUV(rec.point, _center, rec.u, rec.v);

    return true;
}

} // namespace Raytracer
