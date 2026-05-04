#include "Torus.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

namespace {

void getTorusUV(const Vector3D& p, double majorR, double& u, double& v) {
    double phi = std::atan2(p.z, p.x);
    u = 1.0 - (phi + M_PI) / (2.0 * M_PI);

    Vector3D centerOnTube(std::cos(phi) * majorR, 0, std::sin(phi) * majorR);
    Vector3D relP = p - centerOnTube;

    double localX = std::sqrt(relP.x * relP.x + relP.z * relP.z);
    double theta = std::atan2(relP.y, std::sqrt(p.x * p.x + p.z * p.z) - majorR);

    v = (theta + M_PI) / (2.0 * M_PI);
}

} // namespace

const char* getName() { return "torus"; }

IPrimitive* createPlugin(const ISetting& settings) {
    float majorR = settings.getFloat("major_radius", 1.0f);
    float minorR = settings.getFloat("minor_radius", 0.25f);

    auto torusPrimitive = std::make_shared<Torus>(majorR, minorR);

    EntityBuilder builder(settings);

    std::unique_ptr<Entity> entity = builder.setPrimitive(torusPrimitive)
                                         .parseTransform(settings)
                                         .parseMaterial(settings)
                                         .build();

    return entity.release();
}

} // extern "C"

bool Torus::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    Math::QuarticCoeffs coeffs = computeCoefficients(r);

    double roots[4];
    int numRoots = Math::solveQuartic(coeffs, roots);

    if (numRoots == 0)
        return false;

    double t_final;
    if (!findClosestValidT(roots, numRoots, ray_t, t_final))
        return false;

    fillHitRecord(r, t_final, rec);

    return true;
}

Math::QuarticCoeffs Torus::computeCoefficients(const Ray& r) const {
    const Vector3D& O = r.origin();
    const Vector3D& D = r.direction();

    double R2 = _majorRadius * _majorRadius;
    double r2 = _minorRadius * _minorRadius;

    double sum_O2 = O.lengthSquared();
    double dot_OD = O.dot(D);
    double sum_D2 = D.lengthSquared();

    double K = sum_O2 - R2 - r2;
    double a = sum_D2 * sum_D2;
    double b = 4.0 * sum_D2 * dot_OD;
    double c = 2.0 * sum_D2 * K + 4.0 * dot_OD * dot_OD + 4.0 * R2 * D.y * D.y;
    double d = 4.0 * K * dot_OD + 8.0 * R2 * O.y * D.y;
    double e = K * K + 4.0 * R2 * O.y * O.y - 4.0 * R2 * r2;

    return {a, b, c, d, e};
}

bool Torus::findClosestValidT(const double roots[4],
                              int numRoots,
                              Interval ray_t,
                              double& t_out) const {
    double closest = ray_t.max;
    bool found = false;

    for (int i = 0; i < numRoots; ++i) {
        // On utilise ray_t.min + epsilon pour éviter l'acné de surface
        if (roots[i] > ray_t.min + 0.0001 && roots[i] < closest) {
            closest = roots[i];
            found = true;
        }
    }

    if (found)
        t_out = closest;
    return found;
}

void Torus::fillHitRecord(const Ray& r, double t, HitRecord& rec) const {
    rec.t = t;
    rec.point = r.at(t);

    double param = sqrt(rec.point.x * rec.point.x + rec.point.z * rec.point.z);

    Vector3D local_normal;
    if (param < 0.0001) { // Sécurité pour le centre exact
        local_normal = Vector3D(0, rec.point.y > 0 ? 1 : -1, 0);
    } else {
        Vector3D pointOnMajorCircle(
            _majorRadius * rec.point.x / param, 0, _majorRadius * rec.point.z / param);
        local_normal = rec.point - pointOnMajorCircle;
    }

    rec.setFaceNormal(r, local_normal.normalized());

    getTorusUV(rec.point, _majorRadius, rec.u, rec.v);
}

}; // namespace Raytracer
