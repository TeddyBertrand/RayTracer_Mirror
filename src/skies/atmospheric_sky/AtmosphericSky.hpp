#pragma once

#include "skies/commun/ASky.hpp"

namespace Raytracer
{

class AtmosphericSky : public ASky
{
public:
    AtmosphericSky(const Color& groundColor, const Color& zenithColor)
        : _groundColor(groundColor), _zenithColor(zenithColor) {}

    Color getBackgroundColor(const Ray& r) const override;

    std::string getName() const override { return "atmospheric_sky"; }

private:
    Color _groundColor;
    Color _zenithColor;
};

} // namespace Raytracer
