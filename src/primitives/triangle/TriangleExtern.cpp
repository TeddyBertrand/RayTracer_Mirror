#include "Triangle.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "triangle"; }

IPrimitive* createPlugin(const ISetting& settings) {
    const std::string type = settings.getString("type");
    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    Vector3D v0 = pSettings->getVector("v0");
    Vector3D v1 = pSettings->getVector("v1");
    Vector3D v2 = pSettings->getVector("v2");

    auto triangle = std::make_shared<Triangle>(v0, v1, v2, mat);
    auto* entity = new Entity(type, triangle, mat);

    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    entity->rotateX(Math::degreesToRadians(rot.x));
    entity->rotateY(Math::degreesToRadians(rot.y));
    entity->rotateZ(Math::degreesToRadians(rot.z));
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    entity->translate(pos.x, pos.y, pos.z);

    return entity;
}
}

} // namespace Raytracer
