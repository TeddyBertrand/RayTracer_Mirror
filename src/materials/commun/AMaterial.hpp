#pragma once

#include "components/IMaterial.hpp"

namespace Raytracer
{

class AMaterial : public IMaterial
{
public:
    double getSpecularWeight() const override { return 0.0; }

    Color getTransmittance() const override { return Color(0, 0, 0); }

    Color emit([[maybe_unused]] const HitRecord& rec) const override { return Color(0, 0, 0); }
};

} // namespace Raytracer
