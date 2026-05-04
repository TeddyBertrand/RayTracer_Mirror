#include "Cone.hpp"
#include "components/Entity.hpp"
#include "factory/PrimitiveFactory.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {
extern "C" {
const char* getName() { return "cone"; }

IPrimitive* createPlugin(const ISetting& settings) {
    const std::string type = settings.getString("type");

    const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

    std::shared_ptr<IMaterial> mat = nullptr;
    if (pSettings) {
        mat = pSettings->getMaterial();
    }

    auto conePrimitive = std::make_shared<Cone>();
    auto* entity = new Entity(type, conePrimitive, mat);

    // Rotation : Oriente l'axe du cône
    Vector3D rot = settings.getVector("rotation", Vector3D(0, 0, 0));
    entity->rotateX(Math::degreesToRadians(rot.x));
    entity->rotateY(Math::degreesToRadians(rot.y));
    entity->rotateZ(Math::degreesToRadians(rot.z));

    // Position : Place le sommet (Apex)
    Vector3D pos = settings.getVector("position", Vector3D(0, 0, 0));
    entity->translate(pos.x, pos.y, pos.z);

    return entity;
}
}

bool Cone::solve(const Ray& r, float& t0, float& t1) const {
    const Vector3D& O = r.origin();
    const Vector3D& D = r.direction();
    float a = D.x * D.x + D.z * D.z - D.y * D.y;
    float b = 2.0f * (O.x * D.x + O.z * D.z - O.y * D.y);
    float c = O.x * O.x + O.z * O.z - O.y * O.y;
    return Math::solveQuadratic(a, b, c, t0, t1); // Appelle ta fonction globale
}

bool Cone::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    float t0, t1;
    if (!solve(r, t0, t1))
        return false;

    float t_final = -1.0f;
    if (ray_t.surrounds(t0) && r.at(t0).y > 0)
        t_final = t0;
    else if (ray_t.surrounds(t1) && r.at(t1).y > 0)
        t_final = t1;

    if (t_final < 0)
        return false;

    rec.t = t_final;
    rec.point = r.at(t_final);
    rec.setFaceNormal(r, Vector3D(rec.point.x, -rec.point.y, rec.point.z).normalized());
    rec.material = _material;
    return true;
}
}; // namespace Raytracer