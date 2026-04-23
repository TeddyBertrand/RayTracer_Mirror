#pragma once

#include "materials/commun/AMaterial.hpp"

namespace Raytracer
{

class Transparent : public AMaterial
{
public:
    Transparent(Color albedo, double ref_idx): 
        _albedo(albedo), _ref_idx(ref_idx) {}

    bool scatter(
        const Ray& r_in,
        const HitRecord& rec,
        Color& attenuation,
        Ray& scattered
    ) const override;
    
    double getSpecularWeight() const override { return 1.0; }
    
    Color getTransmittance() const override { return _albedo; }

    virtual std::string getName() const override { return "transparent"; }

private:
    Color _albedo;
    double _ref_idx;
};

} // namespace Raytracer
