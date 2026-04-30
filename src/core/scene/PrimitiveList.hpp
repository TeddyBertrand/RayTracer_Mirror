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

    void setMaterial([[maybe_unused]] std::shared_ptr<IMaterial> m) override {}

    void buildBVH();

    AABB getBoundingBox() const override;

private:
    std::vector<std::shared_ptr<IPrimitive>> _bounded_objects;
    std::vector<std::shared_ptr<IPrimitive>> _unbounded_objects;
    std::shared_ptr<BVHNode> _bvh_root = nullptr;
};

} // namespace Raytracer
