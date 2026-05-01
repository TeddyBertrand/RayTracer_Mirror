#include "BVHNode.hpp"

namespace Raytracer {

namespace {

AABB mergedBoundingBox(const std::shared_ptr<IPrimitive>& left,
                       const std::shared_ptr<IPrimitive>& right) {
    if (!left || !right) {
        return AABB();
    }

    const AABB leftBox = left->getBoundingBox();
    const AABB rightBox = right->getBoundingBox();

    if (!leftBox.isFinite() || !rightBox.isFinite()) {
        return AABB::infinite();
    }

    return AABB::combine(leftBox, rightBox);
}

int longestAxis(const AABB& box) {
    const Vector3D extent = box.max - box.min;
    if (extent.x >= extent.y && extent.x >= extent.z)
        return 0;
    if (extent.y >= extent.z)
        return 1;
    return 2;
}

template <typename Comparator>
void partitionObjects(std::vector<std::shared_ptr<IPrimitive>>& objects,
                      size_t start,
                      size_t mid,
                      size_t end,
                      Comparator comparator) {
    std::nth_element(
        objects.begin() + start, objects.begin() + mid, objects.begin() + end, comparator);
}

} // namespace

BVHNode::BVHNode(std::vector<std::shared_ptr<IPrimitive>>& objects, size_t start, size_t end) {
    size_t object_span = end - start;

    if (object_span == 1) {
        _left = objects[start];
        _right = nullptr;
        _bbox = _left->getBoundingBox();
        return;
    } else if (object_span == 2) {
        auto comparator = compare_x;
        const AABB box0 = objects[start]->getBoundingBox();
        const AABB box1 = objects[start + 1]->getBoundingBox();
        const int axis = longestAxis(AABB::combine(box0, box1));
        comparator = (axis == 0) ? compare_x : (axis == 1) ? compare_y : compare_z;

        if (comparator(objects[start], objects[start + 1])) {
            _left = objects[start];
            _right = objects[start + 1];
        } else {
            _left = objects[start + 1];
            _right = objects[start];
        }
    } else {
        AABB spanBox = objects[start]->getBoundingBox();
        for (size_t i = start + 1; i < end; ++i) {
            spanBox = AABB::combine(spanBox, objects[i]->getBoundingBox());
        }

        const int axis = longestAxis(spanBox);
        auto comparator = (axis == 0) ? compare_x : (axis == 1) ? compare_y : compare_z;

        auto mid = start + object_span / 2;
        partitionObjects(objects, start, mid, end, comparator);
        _left = std::make_shared<BVHNode>(objects, start, mid);
        _right = std::make_shared<BVHNode>(objects, mid, end);
    }

    _bbox = mergedBoundingBox(_left, _right);
}

bool BVHNode::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    if (!_bbox.hit(r, ray_t))
        return false;

    bool hit_left = _left->hit(r, ray_t, rec);

    if (!_right)
        return hit_left;

    Interval right_interval(ray_t.min, hit_left ? rec.t : ray_t.max);
    bool hit_right = _right->hit(r, right_interval, rec);

    return hit_left || hit_right;
}

} // namespace Raytracer
