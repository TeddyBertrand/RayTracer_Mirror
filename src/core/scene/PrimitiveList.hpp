#pragma once

#include "components/IPrimitive.hpp"
#include <memory>
#include <vector>

namespace Raytracer {

class PrimitiveList : public IPrimitive {
public:
    PrimitiveList() = default;

    void add(std::shared_ptr<IPrimitive> object) { _objects.push_back(object); }
    void clear() { _objects.clear(); }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

private:
    std::vector<std::shared_ptr<IPrimitive>> _objects;
};

} // namespace Raytracer
