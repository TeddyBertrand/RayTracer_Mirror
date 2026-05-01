#include "Plane.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "plane"; }

IPrimitive* createPlugin(const ISetting& settings) {
    const std::string type = settings.getString("type");
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    auto planePrimitive = std::make_shared<Plane>();
    auto* entity = new Entity(type, planePrimitive, mat);

    // Unit plane: local origin (0,0,0), local normal (0,1,0).
    // Transform order must be rotate -> translate so position is not rotated.
    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    entity->rotateX(Math::degreesToRadians(rot.x));
    entity->rotateY(Math::degreesToRadians(rot.y));
    entity->rotateZ(Math::degreesToRadians(rot.z));
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    entity->translate(pos.x, pos.y, pos.z);

    return entity;
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

    rec.material = _material;

    return true;
}

}; // namespace Raytracer
