#include "Triangle.hpp"
#include "parser/PrimitiveSettings.hpp"
#include "components/Entity.hpp"
#include "math/MathUtils.hpp"

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

Triangle::Triangle(const Point3D& v0,
                   const Point3D& v1,
                   const Point3D& v2,
                   std::shared_ptr<IMaterial> material)
    : _v0(v0), _v1(v1), _v2(v2), _material(material) {
    _edge1 = _v1 - _v0;
    _edge2 = _v2 - _v0;
}

bool Triangle::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    Vector3D pvec = r.direction().cross(_edge2);
    double det = _edge1.dot(pvec);

    if (std::abs(det) < 1e-8)
        return false;
    double inv_det = 1.0 / det;

    Vector3D tvec = r.origin() - _v0;
    double u = tvec.dot(pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return false;

    Vector3D qvec = tvec.cross(_edge1);
    double v = r.direction().dot(qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = _edge2.dot(qvec) * inv_det;

    if (!ray_t.contains(t))
        return false;

    rec.t = t;
    rec.point = r.at(t);

    Vector3D normal = _edge1.cross(_edge2).normalized();
    rec.setFaceNormal(r, normal);

    rec.material = _material;

    rec.u = u;
    rec.v = v;

    return true;
}

AABB Triangle::getBoundingBox() const {
    double min_x = std::min({_v0.x, _v1.x, _v2.x});
    double min_y = std::min({_v0.y, _v1.y, _v2.y});
    double min_z = std::min({_v0.z, _v1.z, _v2.z});

    double max_x = std::max({_v0.x, _v1.x, _v2.x});
    double max_y = std::max({_v0.y, _v1.y, _v2.y});
    double max_z = std::max({_v0.z, _v1.z, _v2.z});

    return AABB{Point3D(min_x - bounding_box_epsilon,
                        min_y - bounding_box_epsilon,
                        min_z - bounding_box_epsilon),
                Point3D(max_x + bounding_box_epsilon,
                        max_y + bounding_box_epsilon,
                        max_z + bounding_box_epsilon)};
}

} // namespace Raytracer
