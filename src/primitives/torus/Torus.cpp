#include "Torus.hpp"
#include "components/Entity.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "torus"; }

IPrimitive* createPlugin(const ISetting& settings) {
    std::cout << "\n[TorusPlugin] --- Start Creation ---" << std::endl;

    const std::string type = settings.getString("type");
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
        std::cout << "[TorusPlugin] Material found: " << (mat ? "Yes" : "No (nullptr!)")
                  << std::endl;
    }

    // Récupération des rayons avec feedback
    float majorR = settings.getFloat("major_radius", 1.0f);
    float minorR = settings.getFloat("minor_radius", 0.25f);

    auto torusPrimitive = std::make_shared<Torus>(majorR, minorR);
    torusPrimitive->setMaterial(mat);
    auto* entity = new Entity(type, torusPrimitive, mat);

    // Debug Transformations
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    Vector3D sca = settings.getVector("scale", Vector3D(1, 1, 1));

    std::cout << "[TorusPlugin] Position:  (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
              << std::endl;
    std::cout << "[TorusPlugin] Rotation:  (" << rot.x << ", " << rot.y << ", " << rot.z << ")"
              << std::endl;
    std::cout << "[TorusPlugin] Scale:     (" << sca.x << ", " << sca.y << ", " << sca.z << ")"
              << std::endl;

    entity->scale(sca.x, sca.y, sca.z);
    entity->rotateX(Math::degreesToRadians(rot.x));
    entity->rotateY(Math::degreesToRadians(rot.y));
    entity->rotateZ(Math::degreesToRadians(rot.z));
    entity->translate(pos.x, pos.y, pos.z);

    // Vérification de la Bounding Box après transformation
    AABB bbox = entity->getBoundingBox();
    std::cout << "[TorusPlugin] BBox Min: (" << bbox.min.x << ", " << bbox.min.y << ", "
              << bbox.min.z << ")" << std::endl;
    std::cout << "[TorusPlugin] BBox Max: (" << bbox.max.x << ", " << bbox.max.y << ", "
              << bbox.max.z << ")" << std::endl;

    std::cout << "[TorusPlugin] --- Creation Success ---" << std::endl;
    return entity;
}
} // extern "C"

Torus::Torus() : _majorRadius(1.0), _minorRadius(0.25) {}

Torus::Torus(double majorR, double minorR) : _majorRadius(majorR), _minorRadius(minorR) {}

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
    rec.material = _material;
}

AABB Torus::getBoundingBox() const {
    double r_total = _majorRadius + _minorRadius;
    return AABB(Vector3D(-r_total, -_minorRadius, -r_total),
                Vector3D(r_total, _minorRadius, r_total));
}

}; // namespace Raytracer
