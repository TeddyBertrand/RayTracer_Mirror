#pragma once

#include "components/IPrimitive.hpp"
#include "core/scene/bvh/BVHNode.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class PrimitiveList : public IPrimitive {
public:
    PrimitiveList() = default;

    void add(std::shared_ptr<IPrimitive> object);
    void clear();

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    void buildBVH();

    AABB getBoundingBox() const override;

    std::vector<std::shared_ptr<IPrimitive>> getObjects() const {
        std::vector<std::shared_ptr<IPrimitive>> all = _bounded_objects;
        all.insert(all.end(), _unbounded_objects.begin(), _unbounded_objects.end());
        return all;
    }

    void dump(int indent = 0) const override;

private:
    std::vector<std::shared_ptr<IPrimitive>> _bounded_objects;
    std::vector<std::shared_ptr<IPrimitive>> _unbounded_objects;
    std::shared_ptr<BVHNode> _bvh_root = nullptr;
};

} // namespace Raytracer
