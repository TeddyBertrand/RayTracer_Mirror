#include "Plane.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

namespace {

void getPlaneUV(
    const Vector3D& point, const Vector3D& normal, const Vector3D& pos, double& u, double& v) {
    Vector3D up = (std::abs(normal.y) < 0.9) ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);

    Vector3D u_axis = normal.cross(up).normalized();
    Vector3D v_axis = u_axis.cross(normal);

    Vector3D relative_point = point - pos;
    u = relative_point.dot(u_axis);
    v = relative_point.dot(v_axis);
}

} // namespace

const char* getName() { return "plane"; }

IPrimitive* createPlugin(const ISetting& settings) {
    auto planePrimitive = std::make_shared<Plane>();

    EntityBuilder builder(settings);

    std::unique_ptr<Entity> entity = builder.setPrimitive(planePrimitive)
                                         .parseTransform(settings)
                                         .parseMaterial(settings)
                                         .build();

    return entity.release();
}

} // extern "C"

bool Plane::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    float denom = _normal.dot(r.direction());

    if (std::abs(denom) < 1e-6) {
        return false;
    }

    float t = (_position - r.origin()).dot(_normal) / denom;

    if (ray_t.excludes(t)) {
        return false;
    }

    rec.t = t;
    rec.point = r.at(t);
    rec.setFaceNormal(r, _normal);

    getPlaneUV(rec.point, _normal, _position, rec.u, rec.v);
    return true;
}

}; // namespace Raytracer
