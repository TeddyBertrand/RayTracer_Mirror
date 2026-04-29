#include "Plane.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "plane"; }

IPrimitive* createPlugin(const ISetting& settings) {
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    Vector3D normal = settings.getVector("normal", Vector3D(0, 1, 0));
    normal.normalize();
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    return new Plane(pos, normal, mat);
}
}

bool Plane::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    float denom = _normal.dot(r.direction());

    if (std::abs(denom) < 1e-6) {
        return false;
    }

    float t = (_position - r.origin()).dot(_normal) / denom;

    if (t < ray_t.min || t > ray_t.max) {
        return false;
    }

    rec.t = t;
    rec.point = r.origin() + t * r.direction();

    rec.setFaceNormal(r, _normal);

    rec.material = _material;

    return true;
}

}; // namespace Raytracer
