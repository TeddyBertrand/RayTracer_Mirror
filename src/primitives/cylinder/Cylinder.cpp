#include "Cylinder.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <algorithm>
#include <cmath>

namespace Raytracer {

extern "C" {

const char* getName() { return "cylinder"; }

IPrimitive* createPlugin(const ISetting& settings) {
    auto cylinderPrimitive = std::make_shared<Cylinder>();

    EntityBuilder builder(settings);

    builder.setPrimitive(cylinderPrimitive).parseTransform(settings);

    if (settings.exists("radius") || settings.exists("height")) {
        const double radius = settings.getFloat("radius", 1.0);
        const double height = settings.getFloat("height", 1.0);
        builder.addScale(radius, height, radius);
    }

    auto entity = builder.parseMaterial(settings).build();

    return entity.release();
}

} // extern "C"

namespace {

/**
 * @brief Calcule les UV pour la surface laterale d'un cylindre oriente sur l'axe Y
 * @param p Le point d'impact local
 * @param center Le centre du cylindre
 * @param height La hauteur du cylindre
 */
void getCylinderSideUV(const Raytracer::Vector3D& p,
                       const Raytracer::Vector3D& center,
                       double height,
                       double& u,
                       double& v) {
    double theta = std::atan2(-(p.z - center.z), p.x - center.x) + M_PI;
    u = theta / (2.0 * M_PI);

    v = (p.y - center.y) / height;
}

/**
 * @brief Calcule les UV pour un disque (cap) sur l'axe Y
 * @param p Le point d'impact local
 * @param center Le centre du cylindre
 * @param radius Le rayon du cylindre
 */
void getCylinderCapUV(const Raytracer::Vector3D& p,
                      const Raytracer::Vector3D& center,
                      double radius,
                      double& u,
                      double& v) {
    const double local_x = (p.x - center.x) / radius;
    const double local_z = (p.z - center.z) / radius;
    u = 0.5 + (local_x * 0.5);
    v = 0.5 + (local_z * 0.5);
}

} // namespace

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

bool Cylinder::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    const double kEpsilon = 1e-12;
    bool hit_any = false;
    double closest = ray_t.max;

    // Intersections avec la surface laterale
    const double dx = r.direction().x;
    const double dz = r.direction().z;
    const double ox = r.origin().x - _center.x;
    const double oz = r.origin().z - _center.z;

    const double a = dx * dx + dz * dz;
    if (std::abs(a) > kEpsilon) {
        const double b = 2.0 * (dx * ox + dz * oz);
        const double c = (ox * ox + oz * oz) - (_radius * _radius);

        float t0, t1;
        if (solveQuadratic(
                static_cast<float>(a), static_cast<float>(b), static_cast<float>(c), t0, t1)) {
            const double roots[2] = {t0, t1};
            for (double t : roots) {
                if (t < ray_t.min || t > ray_t.max || t >= closest)
                    continue;

                const double y_hit = r.origin().y + t * r.direction().y;
                if (y_hit < _center.y || y_hit > _center.y + _height)
                    continue;

                rec.t = t;
                rec.point = r.at(t);

                Vector3D outward_normal(rec.point.x - _center.x, 0.0, rec.point.z - _center.z);
                outward_normal.normalize();
                rec.setFaceNormal(r, outward_normal);
                getCylinderSideUV(rec.point, _center, _height, rec.u, rec.v);

                closest = t;
                hit_any = true;
            }
        }
    }

    // Intersections avec les caps (top/bottom)
    const double dy = r.direction().y;
    if (std::abs(dy) > kEpsilon) {
        const double y_caps[2] = {_center.y, _center.y + _height};
        const Vector3D cap_normals[2] = {Vector3D(0.0, -1.0, 0.0), Vector3D(0.0, 1.0, 0.0)};

        for (int i = 0; i < 2; ++i) {
            const double t = (y_caps[i] - r.origin().y) / dy;
            if (t < ray_t.min || t > ray_t.max || t >= closest)
                continue;

            const Point3D p = r.at(t);
            const double dxp = p.x - _center.x;
            const double dzp = p.z - _center.z;
            if ((dxp * dxp + dzp * dzp) > (_radius * _radius))
                continue;

            rec.t = t;
            rec.point = p;
            rec.setFaceNormal(r, cap_normals[i]);
            getCylinderCapUV(rec.point, _center, _radius, rec.u, rec.v);

            closest = t;
            hit_any = true;
        }
    }

    return hit_any;
}

}; // namespace Raytracer
