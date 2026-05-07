#pragma once

#include "components/IPrimitive.hpp"
#include "core/scene/bvh/BVHNode.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

/**
 * @brief Composite pattern: treats a group of primitives as a single primitive.
 * Uses a BVH (Bounding Volume Hierarchy) to accelerate intersections.
 */
class PrimitiveGroup : public IPrimitive {
public:
    PrimitiveGroup() = default;
    ~PrimitiveGroup() override = default;

    /**
     * @brief Adds a child primitive to the group.
     * Marks the group as dirty to trigger a BVH rebuild.
     */
    void add(std::shared_ptr<IPrimitive> primitive) {
        if (primitive) {
            _children.push_back(std::move(primitive));
            _bvh_root = nullptr;
        }
    }

    /**
     * @brief Tests intersection against children using the BVH.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        if (_children.empty()) {
            return false;
        }

        if (!_bvh_root) {
            const_cast<PrimitiveGroup*>(this)->buildBVH();
        }

        return _bvh_root->hit(r, ray_t, rec);
    }

    /**
     * @brief Returns the bounding box of the entire group.
     */
    [[nodiscard]] AABB getBoundingBox() const override {
        if (_children.empty()) {
            return AABB();
        }
        if (!_bvh_root) {
            const_cast<PrimitiveGroup*>(this)->buildBVH();
        }
        return _bvh_root->getBoundingBox();
    }

    /**
     * @brief Manually builds the BVH.
     */
    void buildBVH() {
        if (_children.empty()) {
            return;
        }
        if (_children.size() == 1) {
            _bvh_root = _children[0];
        } else {
            auto children_copy = _children;
            _bvh_root = std::make_shared<BVHNode>(children_copy, 0, children_copy.size());
        }
    }

    /**
     * @brief Returns the list of children.
     */
    [[nodiscard]] const std::vector<std::shared_ptr<IPrimitive>>& getChildren() const {
        return _children;
    }

private:
    std::vector<std::shared_ptr<IPrimitive>> _children;
    std::shared_ptr<IPrimitive> _bvh_root;
};

} // namespace Raytracer
