#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/texture/ImageTexture.hpp"
#include "materials/commun/texture/SolidColor.hpp"
#include <memory>
#include <string>

namespace Raytracer {

/**
 * @brief Wrapper texture that can be either a solid color or an image.
 * The constructor overload determines which implementation is used.
 */
class Texture : public ITexture {
public:
    explicit Texture(const Color& c) : _impl(std::make_unique<SolidColor>(c)) {}

    explicit Texture(const std::string& filename)
        : _impl(std::make_unique<ImageTexture>(filename)) {}

    Color value(double u, double v) const noexcept override { return _impl->value(u, v); }

private:
    std::unique_ptr<ITexture> _impl;
};

} // namespace Raytracer
