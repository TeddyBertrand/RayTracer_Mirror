#include "Cone.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

namespace {

void getConeUV(const Vector3D& p, double& u, double& v) {
    double phi = std::atan2(p.z, p.x);
    u = 1.0 - (phi + M_PI) / (2.0 * M_PI);

    v = std::fmod(std::abs(p.y), 1.0);
}

} // namespace

extern "C" {
const char* getName() { return "cone"; }

IPrimitive* createPlugin(const ISetting& settings) {
    auto conePrimitive = std::make_shared<Cone>();

    EntityBuilder builder(settings);

    std::unique_ptr<Entity> entity = builder.setPrimitive(conePrimitive)
                                         .parseTransform(settings)
                                         .parseMaterial(settings)
                                         .build();

    return entity.release();
}
}

bool Cone::solve(const Ray& r, float& t0, float& t1) const {
    const Vector3D& O = r.origin();
    const Vector3D& D = r.direction();
    float a = D.x * D.x + D.z * D.z - D.y * D.y;
    float b = 2.0f * (O.x * D.x + O.z * D.z - O.y * D.y);
    float c = O.x * O.x + O.z * O.z - O.y * O.y;
    return Math::solveQuadratic(a, b, c, t0, t1); // Appelle ta fonction globale
}

bool Cone::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    float t0, t1;
    if (!solve(r, t0, t1))
        return false;

    float t_final = -1.0f;

    if (ray_t.surrounds(t0) && r.at(t0).y > 0)
        t_final = t0;
    else if (ray_t.surrounds(t1) && r.at(t1).y > 0)
        t_final = t1;

    if (t_final < 0)
        return false;

    rec.t = t_final;
    rec.point = r.at(t_final);

    Vector3D outward_normal = Vector3D(rec.point.x, -rec.point.y, rec.point.z).normalized();
    rec.setFaceNormal(r, outward_normal);

    getConeUV(rec.point, rec.u, rec.v);

    return true;
}
}; // namespace Raytracer
