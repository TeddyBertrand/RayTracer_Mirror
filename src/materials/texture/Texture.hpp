#pragma once

#include "ImageTexture.hpp"
#include "materials/commun/AMaterial.hpp"

namespace Raytracer {

class TextureMaterial : public AMaterial {
public:
    TextureMaterial(const ImageTexture& texture, const Color& albedo = Color(1, 1, 1))
        : _texture(texture), _albedo(albedo) {}

    bool scatter(const Ray& r_in,
                 const HitRecord& rec,
                 Color& attenuation,
                 Ray& scattered) const override;

    std::string getName() const override { return "texture"; }

private:
    ImageTexture _texture;
    Color _albedo;
};

} // namespace Raytracer
