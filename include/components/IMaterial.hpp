#pragma once

#include "math/Color.hpp"
#include "math/Ray.hpp"
#include "math/HitRecord.hpp"

#include "components/IPlugin.hpp"

namespace Raytracer
{

class IMaterial : public IPlugin
{
public:
    virtual ~IMaterial() = default;

    virtual bool scatter(
        const Ray& r_in,
        const HitRecord& rec,
        Color& attenuation,
        Ray& scattered
    ) const = 0;
};

} // namespace Raytracer
