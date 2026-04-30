#pragma once

#include "components/ITexture.hpp"
#include "materials/commun/texture/ImageTexture.hpp"
#include "materials/commun/texture/SolidColor.hpp"
#include "parser/ISettings.hpp"
#include <exception>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<ITexture> fromSetting(const ISetting& settings, const std::string& key) {
        if (!settings.exists(key)) {
            throw std::runtime_error("Missing texture setting '" + key + "'.");
        }

        try {
            return std::make_shared<Texture>(settings.getColor(key));
        } catch (const std::exception&) {
        }

        try {
            const std::string filename = settings.getString(key);

            if (filename.empty()) {
                throw std::runtime_error("Texture path for '" + key + "' cannot be empty.");
            }
            return std::make_shared<Texture>(filename);
        } catch (const std::exception&) {
        }

        throw std::runtime_error("Texture setting '" + key +
                                 "' must be either a color ({r,g,b}) or a string path.");
    }

    Color value(double u, double v) const noexcept override { return _impl->value(u, v); }

private:
    std::unique_ptr<ITexture> _impl;
};

} // namespace Raytracer
