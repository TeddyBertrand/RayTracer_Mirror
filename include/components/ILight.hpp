#pragma once

#include "math/Vector3D.hpp"
#include "math/Color.hpp"
#include "math/HitRecord.hpp"

namespace Raytracer
{

class ILight
{
public:
    virtual ~ILight() = default;

    virtual bool illuminate(
        const HitRecord& rec,
        Vector3D& light_dir,
        Color& light_color
    ) const = 0;
};

} // namespace Raytracer
