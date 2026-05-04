#pragma once

#include "components/ITexture.hpp"
#include <SFML/Graphics/Image.hpp>
#include <algorithm>
#include <string>

namespace Raytracer {

/**
 * @brief Texture mapping using an SFML Image.
 */
class ImageTexture : public ITexture {
public:
    /**
     * @brief Load an image from a file using SFML.
     * If loading fails, creates a 1x1 magenta fallback texture.
     */
    explicit ImageTexture(const std::string& filename) {
        if (!_image.loadFromFile(filename)) {
            _image.create(1, 1, sf::Color::Magenta);
        }
    }

    /**
     * @brief Get color at UV coordinates.
     */
    [[nodiscard]] Color value(double u, double v) const noexcept override {
        auto size = _image.getSize();
        if (size.x == 0 || size.y == 0)
            return Color(1, 0, 1);

        u = std::clamp(u, 0.0, 1.0);
        v = 1.0 - std::clamp(v, 0.0, 1.0);

        auto i = static_cast<unsigned int>(u * (size.x - 1));
        auto j = static_cast<unsigned int>(v * (size.y - 1));

        sf::Color pixel = _image.getPixel(i, j);

        return Color(static_cast<double>(pixel.r) / 255.0,
                     static_cast<double>(pixel.g) / 255.0,
                     static_cast<double>(pixel.b) / 255.0);
    }

private:
    sf::Image _image;
};

} // namespace Raytracer
