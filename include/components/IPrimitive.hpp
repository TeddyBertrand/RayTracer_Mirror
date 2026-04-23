#pragma once

#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Ray.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer {

class IPrimitive : public IPlugin {
public:
    virtual ~IPrimitive() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

} // namespace Raytracer
