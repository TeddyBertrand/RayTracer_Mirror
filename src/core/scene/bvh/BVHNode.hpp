#pragma once

#include "components/IPrimitive.hpp"
#include "math/AABB.hpp"
#include "math/MathUtils.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace Raytracer {

class BVHNode : public IPrimitive {
public:
    BVHNode(std::vector<std::shared_ptr<IPrimitive>>& objects, size_t start, size_t end);

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    AABB getBoundingBox() const override { return _bbox; }

private:
    std::shared_ptr<IPrimitive> _left;
    std::shared_ptr<IPrimitive> _right;
    AABB _bbox;

    static bool compare_x(const std::shared_ptr<IPrimitive>& a,
                          const std::shared_ptr<IPrimitive>& b) {
        return a->getBoundingBox().min.x < b->getBoundingBox().min.x;
    }
    static bool compare_y(const std::shared_ptr<IPrimitive>& a,
                          const std::shared_ptr<IPrimitive>& b) {
        return a->getBoundingBox().min.y < b->getBoundingBox().min.y;
    }
    static bool compare_z(const std::shared_ptr<IPrimitive>& a,
                          const std::shared_ptr<IPrimitive>& b) {
        return a->getBoundingBox().min.z < b->getBoundingBox().min.z;
    }
};

} // namespace Raytracer
