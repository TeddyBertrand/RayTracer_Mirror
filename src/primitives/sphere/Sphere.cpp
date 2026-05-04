#include "Sphere.hpp"
#include "components/Entity.hpp"
#include "builder/EntityBuilder.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <cmath>

namespace Raytracer {

namespace {

void getSphereUV(const Vector3D& p, double& u, double& v) {
    constexpr double pi = M_PI;
    const double theta = std::acos(-p.y);
    const double phi = std::atan2(-p.z, p.x) + pi;

    u = phi / (2.0 * pi);
    v = theta / pi;
}

} // namespace

extern "C" {

const char* getName() { return "sphere"; }

IPrimitive* createPlugin(const ISetting& settings) {
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    auto sphere = std::make_shared<Sphere>();

    auto entity = EntityBuilder("sphere")
                .setPrimitive(sphere)
                .parseTranslation(settings)
                .parseScaleRadius(settings)
                .build();

    if (!entity)
        return nullptr;

    entity->setMaterial(mat);

    return entity.release();
}
}

bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1) {
    // get the discriminant
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    // make sure that x0 is still the smallest value
    if (x0 > x1)
        std::swap(x0, x1);

    return true;
}

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    Vector3D L = r.origin();                    // vector from center to ray origin
    float a = r.direction().dot(r.direction()); // direction lenght square
    float b = 2 * r.direction().dot(L);         // ray/ sphere alignement
    float c = L.dot(L) - 1;                     // start position

    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1))
        return false;

    // root should be the nearest point
    float root = t0;
    if (root < ray_t.min || root > ray_t.max) {
        // if t0 is too far, try t1
        root = t1;
        if (root < ray_t.min || root > ray_t.max)
            return false; // neither worked
    }

    rec.t = root;
    rec.point = r.origin() + r.direction() * rec.t; // impact point

    // impact distance
    Vector3D normal = rec.point;

    // Compute UVs from the geometric normal for texture lookup.
    getSphereUV(normal.normalized(), rec.u, rec.v);

    // check if ray hit inside or outside and orient in function
    rec.setFaceNormal(r, normal);

    return true;
}

}; // namespace Raytracer
