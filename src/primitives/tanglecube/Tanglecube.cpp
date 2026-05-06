#include "Tanglecube.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"
#include <algorithm>
#include <cmath>

namespace Raytracer {

extern "C" {

const char* getName() { return "tanglecube"; }

IPrimitive* createPlugin(const ISetting& settings) {
    try {
        std::cerr << "Tanglecube::createPlugin called" << std::endl;
        auto prim = std::make_shared<Tanglecube>();

        EntityBuilder builder(settings);

        std::unique_ptr<Entity> entity =
            builder.setPrimitive(prim).parseTransform(settings).parseMaterial(settings).build();

        return entity.release();
    } catch (const std::exception& e) {
        std::cerr << "Exception in tanglecube createPlugin: " << e.what() << std::endl;
        throw;
    }
}

} // extern "C"

double Tanglecube::implicitFunction(const Vector3D& p) const {
    double dx = p.x - _center.x;
    double dy = p.y - _center.y;
    double dz = p.z - _center.z;

    double dx2 = dx * dx;
    double dy2 = dy * dy;
    double dz2 = dz * dz;

    return (dx2 * dx2) + (dy2 * dy2) + (dz2 * dz2) - 5.0 * (dx2 + dy2 + dz2) + 11.8;
}

void Tanglecube::computeGradient(const Vector3D& p, Vector3D& grad) const {
    double dx = p.x - _center.x;
    double dy = p.y - _center.y;
    double dz = p.z - _center.z;

    grad.x = 4.0 * dx * dx * dx - 10.0 * dx;
    grad.y = 4.0 * dy * dy * dy - 10.0 * dy;
    grad.z = 4.0 * dz * dz * dz - 10.0 * dz;
}

bool Tanglecube::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    Vector3D D = r.direction();
    Vector3D O = r.origin() - _center;

    double dx = D.x, dy = D.y, dz = D.z;
    double ox = O.x, oy = O.y, oz = O.z;

    double dx2 = dx * dx, dy2 = dy * dy, dz2 = dz * dz;
    double ox2 = ox * ox, oy2 = oy * oy, oz2 = oz * oz;

    double dx3 = dx2 * dx, dy3 = dy2 * dy, dz3 = dz2 * dz;
    double ox3 = ox2 * ox, oy3 = oy2 * oy, oz3 = oz2 * oz;

    double dx4 = dx2 * dx2, dy4 = dy2 * dy2, dz4 = dz2 * dz2;
    double ox4 = ox2 * ox2, oy4 = oy2 * oy2, oz4 = oz2 * oz2;

    Math::QuarticCoeffs coeffs;
    coeffs.a = dx4 + dy4 + dz4;
    coeffs.b = 4.0 * (dx3 * ox + dy3 * oy + dz3 * oz);
    coeffs.c = 6.0 * (dx2 * ox2 + dy2 * oy2 + dz2 * oz2) - 5.0 * (dx2 + dy2 + dz2);
    coeffs.d = 4.0 * (dx * ox3 + dy * oy3 + dz * oz3) - 10.0 * (dx * ox + dy * oy + dz * oz);
    coeffs.e = (ox4 + oy4 + oz4) - 5.0 * (ox2 + oy2 + oz2) + 11.8;

    double roots[4];
    int num_roots = Math::solveQuartic(coeffs, roots);

    double best_t = ray_t.max;
    bool found = false;

    for (int i = 0; i < num_roots; ++i) {
        if (roots[i] > ray_t.min && roots[i] < best_t) {
            best_t = roots[i];
            found = true;
        }
    }

    if (!found)
        return false;

    rec.t = best_t;
    rec.point = r.at(best_t);

    Vector3D grad;
    computeGradient(rec.point, grad);
    if (grad.lengthSquared() > 1e-12) {
        grad.normalize();
        rec.setFaceNormal(r, grad);
    } else {
        rec.setFaceNormal(r, Vector3D(0, 1, 0));
    }

    // UV mapping
    rec.u = (rec.point.x - _center.x + 2.0) / 4.0;
    rec.v = (rec.point.y - _center.y + 2.0) / 4.0;

    return true;
}

} // namespace Raytracer
