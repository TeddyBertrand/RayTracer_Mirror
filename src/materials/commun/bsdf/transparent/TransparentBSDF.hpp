#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/bsdf/ABSDF.hpp"
#include <memory>

namespace Raytracer {

class TransparentBSDF : public ABSDF {
public:
    explicit TransparentBSDF(std::shared_ptr<ITexture> tex, double ref = 0.0)
        : _albedo_texture(tex),
          _ref(ref < 0.0 ? 0.0 : ref) {}

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
    Color evaluate(const Vector3D& light_dir,
                   const Vector3D& view_dir,
                   const HitRecord& hit) const override;
private:
    std::shared_ptr<ITexture> _albedo_texture;
    double _ref;
};

};