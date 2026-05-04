#include "Box.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <cmath>

namespace Raytracer {

namespace {

/**
 * @brief Compute UV coordinates for a unit box (-0.5 to 0.5)
 *
 * @param p The hit point in local space
 * @param normal The face normal
 * @param u Output U coordinate [0, 1]
 * @param v Output V coordinate [0, 1]
 */
void getBoxUV(const Vector3D& p, const Vector3D& normal, double& u, double& v) {
    if (std::abs(normal.x) > 0.5) {
        u = p.z + 0.5;
        v = p.y + 0.5;
    } else if (std::abs(normal.y) > 0.5) {
        u = p.x + 0.5;
        v = p.z + 0.5;
    } else {
        u = p.x + 0.5;
        v = p.y + 0.5;
    }
}

} // namespace

extern "C" {

const char* getName() { return "box"; }

IPrimitive* createPlugin(const ISetting& settings) {
    auto boxPrimitive = std::make_shared<Box>();

    EntityBuilder builder(settings);

    std::unique_ptr<Entity> entity =
        builder.setPrimitive(boxPrimitive).parseTransform(settings).parseMaterial(settings).build();

    return entity.release();
}
}

Vector3D Box::getNormalAt(const Vector3D& p) const {
    double epsilon = 0.0001;

    if (std::abs(p.x - 0.5) < epsilon)
        return Vector3D(1, 0, 0);
    if (std::abs(p.x + 0.5) < epsilon)
        return Vector3D(-1, 0, 0);

    if (std::abs(p.y - 0.5) < epsilon)
        return Vector3D(0, 1, 0);
    if (std::abs(p.y + 0.5) < epsilon)
        return Vector3D(0, -1, 0);

    if (std::abs(p.z - 0.5) < epsilon)
        return Vector3D(0, 0, 1);
    return Vector3D(0, 0, -1);
}

bool Box::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    for (int i = 0; i < 3; ++i) {
        double invD = 1.0 / r.direction()[i];
        double t0 = (-0.5 - r.origin()[i]) * invD;
        double t1 = (0.5 - r.origin()[i]) * invD;

        if (invD < 0.0)
            std::swap(t0, t1);

        ray_t.min = std::max(ray_t.min, t0);
        ray_t.max = std::min(ray_t.max, t1);

        if (ray_t.max <= ray_t.min)
            return false;
    }

    rec.t = ray_t.min;
    rec.point = r.at(rec.t);

    Vector3D outward_normal = getNormalAt(rec.point);
    getBoxUV(rec.point, outward_normal, rec.u, rec.v);
    rec.setFaceNormal(r, outward_normal);

    return true;
}

}; // namespace Raytracer
