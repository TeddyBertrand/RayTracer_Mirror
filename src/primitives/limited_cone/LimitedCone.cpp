#include "LimitedCone.hpp"
#include "builder/EntityBuilder.hpp"
#include "math/MathUtils.hpp"

namespace Raytracer {

extern "C" {
const char* getName() { return "limited_cone"; }

IPrimitive* createPlugin(const ISetting& settings) {
    float base_r = settings.getFloat("base_radius", 1.0f);
    float h = settings.getFloat("height", 1.0f);

    auto limitedCone = std::make_shared<LimitedCone>(h, base_r);

    EntityBuilder builder(settings);

    std::unique_ptr<Entity> entity =
        builder.setPrimitive(limitedCone).parseTransform(settings).parseMaterial(settings).build();

    return entity.release();
}
}

void getLimitedConeUV(
    const Vector3D& p, float height, float radius, bool is_base, double& u, double& v) {
    if (is_base) {
        u = (p.x / (2.0 * radius)) + 0.5;
        v = (p.z / (2.0 * radius)) + 0.5;
        return;
    }

    float phi = std::atan2(p.z, p.x);
    u = 1.0 - (phi + M_PI) / (2.0 * M_PI);
    v = p.y / height;
}

bool LimitedCone::intersectBody(const Ray& r, Interval ray_t, float& t_hit) const {
    const Vector3D& O = r.origin();
    const Vector3D& D = r.direction();

    float a = D.x * D.x + D.z * D.z - _k * D.y * D.y;
    float b = 2.0f * (O.x * D.x + O.z * D.z + _k * (_height - O.y) * D.y);
    float c = O.x * O.x + O.z * O.z - _k * (_height - O.y) * (_height - O.y);

    float t0, t1;
    if (!Math::solveQuadratic(a, b, c, t0, t1))
        return false;

    for (float t : {t0, t1}) {
        if (!ray_t.surrounds(t))
            continue;
        float y = O.y + t * D.y;
        if (y >= 0 && y <= _height) {
            t_hit = t;
            return true;
        }
    }
    return false;
}

bool LimitedCone::intersectBase(const Ray& r, Interval ray_t, float& t_hit) const {
    if (std::abs(r.direction().y) < 1e-8)
        return false;

    float t = -r.origin().y / r.direction().y;
    if (!ray_t.surrounds(t))
        return false;

    Vector3D p = r.at(t);
    if ((p.x * p.x + p.z * p.z) <= (_radius * _radius)) {
        t_hit = t;
        return true;
    }
    return false;
}

bool LimitedCone::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    float t_body = -1.0f;
    float t_base = -1.0f;

    bool hit_body = intersectBody(r, ray_t, t_body);
    bool hit_cap = intersectBase(r, ray_t, t_base);

    if (!hit_body && !hit_cap)
        return false;

    bool use_base = false;
    if (hit_body && hit_cap) {
        use_base = (t_base < t_body);
    } else {
        use_base = hit_cap;
    }

    rec.t = use_base ? t_base : t_body;
    rec.point = r.at(rec.t);

    Vector3D outward_normal =
        use_base ? Vector3D(0, -1, 0)
                 : Vector3D(rec.point.x, _k * (_height - rec.point.y), rec.point.z).normalized();

    getLimitedConeUV(rec.point, _height, _radius, use_base, rec.u, rec.v);
    rec.setFaceNormal(r, outward_normal);

    return true;
}

} // namespace Raytracer