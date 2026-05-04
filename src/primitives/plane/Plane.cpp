#include "Plane.hpp"
#include "builder/EntityBuilder.hpp"
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

    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));

    auto entity = EntityBuilder(type)
                      .setPrimitive(planePrimitive)
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
