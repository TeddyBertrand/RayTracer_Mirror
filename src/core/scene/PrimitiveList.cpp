#include "PrimitiveList.hpp"

namespace Raytracer {

void PrimitiveList::add(std::shared_ptr<IPrimitive> object) {
    if (!object)
        return;

    _bvh_root = nullptr;

    const AABB bbox = object->getBoundingBox();
    if (bbox.isFinite()) {
        _bounded_objects.push_back(object);
    } else if (bbox.isInfinite()) {
        _unbounded_objects.push_back(object);
    }
}

void PrimitiveList::clear() {
    _bounded_objects.clear();
    _unbounded_objects.clear();
    _bvh_root = nullptr;
}

bool PrimitiveList::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    if (_bvh_root) {
        if (_bvh_root->hit(r, ray_t, rec)) {
            hit_anything = true;
            closest_so_far = rec.t;
        }
    } else {
        for (const auto& object : _bounded_objects) {
            if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
    }

    for (const auto& object : _unbounded_objects) {
        if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

void PrimitiveList::buildBVH() {
    if (_bounded_objects.empty()) {
        _bvh_root = nullptr;
        return;
    }

    _bvh_root = std::make_shared<BVHNode>(_bounded_objects, 0, _bounded_objects.size());
}

AABB PrimitiveList::getBoundingBox() const {
    if (_bounded_objects.empty()) {
        if (_unbounded_objects.empty())
            return AABB();
        return AABB::infinite();
    }

    if (!_unbounded_objects.empty())
        return AABB::infinite();

    AABB bbox = _bounded_objects[0]->getBoundingBox();
    for (size_t i = 1; i < _bounded_objects.size(); ++i) {
        bbox = AABB::combine(bbox, _bounded_objects[i]->getBoundingBox());
    }

    return bbox;
}

} // namespace Raytracer
