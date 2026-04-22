#pragma once

#include "components/ISky.hpp"

namespace Raytracer
{

class ASky : public ISky
{
public:
    ASky() = default;

    Color getEnvironmentColor(const Ray& r) const override { return getBackgroundColor(r); }
};

} // namespace Raytracer
