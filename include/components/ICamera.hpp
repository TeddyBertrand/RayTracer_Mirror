#pragma once

#pragma once
#include "math/Ray.hpp"

namespace Raytracer
{

class ICamera
{
public:
    virtual ~ICamera() = default;

    virtual Ray get_ray(double u, double v) const = 0;
};

} // namespace Raytracer
