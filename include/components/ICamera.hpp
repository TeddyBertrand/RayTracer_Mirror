#pragma once

#pragma once
#include "math/Ray.hpp"

namespace Raytracer {

class ICamera {
public:
    virtual ~ICamera() = default;

    virtual Ray getRay(double u, double v) const = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

} // namespace Raytracer
