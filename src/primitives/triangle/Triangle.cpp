#include "Triangle.hpp"
#include "builder/EntityBuilder.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "triangle"; }

IPrimitive* createPlugin(const ISetting& settings) {
    const std::string type = settings.getString("type", "triangle");
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    Vector3D A = settings.getVector("A", Vector3D(0, 0, 0));
    Vector3D B = settings.getVector("B", Vector3D(1, 0, 0));
    Vector3D C = settings.getVector("C", Vector3D(0, 1, 0));

    auto trianglePrimitive = std::make_shared<Triangle>(A, B, C);

    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    Vector3D scale = settings.getVector("scale", Vector3D(1, 1, 1));

    auto entity = EntityBuilder(type)
                      .setPrimitive(trianglePrimitive)
                      .addScale(scale.x, scale.y, scale.z)
                      .addRotation(Math::degreesToRadians(rot.x),
                                   Math::degreesToRadians(rot.y),
                                   Math::degreesToRadians(rot.z))
                      .addTranslation(pos.x, pos.y, pos.z)
                      .build();

    if (!entity)
        return nullptr;

    entity->setMaterial(mat);

    return entity.release();
}

} // extern "C"

bool Triangle::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    const double EPSILON = 1e-8;

    Vector3D edge1 = _v1 - _v0;
    Vector3D edge2 = _v2 - _v0;
    Vector3D h = r.direction().cross(edge2);
    double a = edge1.dot(h);

    if (std::abs(a) < EPSILON) {
        return false;
    }

    double f = 1.0 / a;
    Vector3D s = r.origin() - _v0;
    double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    Vector3D q = s.cross(edge1);
    double v = f * r.direction().dot(q);

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = f * edge2.dot(q);

    if (ray_t.excludes(t)) {
        return false;
    }

    rec.t = t;
    rec.point = r.at(t);

    Vector3D normal = edge1.cross(edge2).normalized();
    rec.setFaceNormal(r, normal);
    rec.material = _material;

    // Optional: compute u, v texture coordinates if hit based on barycentric coordinates u, v
    // Triangle barycentric coordinates: w = 1 - u - v
    // We could assign u and v to rec.u and rec.v
    rec.u = u;
    rec.v = v;

    return true;
}

}; // namespace Raytracer
