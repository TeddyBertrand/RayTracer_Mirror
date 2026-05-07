#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <memory>

namespace Raytracer {

class MetalBSDF : public ABSDF {
public:
    explicit MetalBSDF(std::shared_ptr<ITexture> tex, double fuzz = 0.0)
        : _albedo_texture(tex),
          _fuzz(fuzz < 0.0 ? 0.0 : (fuzz > 1.0 ? 1.0 : fuzz)) {}

    /**
     * @brief Sample bounce 
     * 
     * @param r_in 
     * @param hit 
     * @param attenuation 
     * @param scattered 
     * @return true 
     * @return false 
     */
    bool scatter(const Ray& r_in,
                 const HitRecord& hit,
                 Color& attenuation,
                 Ray& scattered) const override;

    /**
     * @brief Evaluate color from hit
     * 
     * @param light_dir 
     * @param view_dir 
     * @param hit 
     * @return Color 
     */
    Color evaluate([[maybe_unused]]const Vector3D& light_dir,
                   [[maybe_unused]] const Vector3D& view_dir,
                   [[maybe_unused]] const HitRecord& hit) const override {
        return Color(0, 0, 0); 
    }

private:
    std::shared_ptr<ITexture> _albedo_texture;
    double _fuzz;
};

};