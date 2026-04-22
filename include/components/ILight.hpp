#pragma once

#include "math/Vector3D.hpp"
#include "math/Color.hpp"
#include "math/HitRecord.hpp"

namespace Raytracer
{

struct LightSample {
    Color color;
    Vector3D direction;
    double distance;
    bool isActive;
};

class ILight {
public:
    virtual ~ILight() = default;
    
    virtual LightSample computeLight(const Point3D& hit_point) const = 0;
};

} // namespace Raytracer
