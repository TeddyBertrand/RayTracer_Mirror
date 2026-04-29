#include "components/TransformDecorator.hpp"

namespace Raytracer {

bool TransformDecorator::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    Point3D origin_local = _inverse * r.origin();
    Vector3D direction_local = _inverse * r.direction();

    Ray ray_local(origin_local, direction_local);

    if (!_wrapped->hit(ray_local, ray_t, rec)) {
        return false;
    }

    rec.point = _transform * rec.point;
    rec.normal = _transform * rec.normal;
    rec.normal.normalize();

    return true;
}

} // namespace Raytracer
