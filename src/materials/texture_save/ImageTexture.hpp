#pragma once

#include "math/Color.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace Raytracer {

class ImageTexture {
public:
    ImageTexture(const std::string& filename) {
        _image.loadFromFile(filename);
        _size = _image.getSize();
    }

    Color value(double u, double v) const {
        if (_size.x <= 0 || _size.y <= 0)
            return Color(0, 0, 0);

        u = std::clamp(u, 0.0, 1.0);
        v = 1.0 - std::clamp(v, 0.0, 1.0);

        auto x = static_cast<unsigned int>(u * (_size.x - 1));
        auto y = static_cast<unsigned int>(v * (_size.y - 1));

        sf::Color pixel = _image.getPixel(sf::Vector2u(x, y));
        return Color(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0);
    }

private:
    sf::Image _image;
    sf::Vector2u _size;
};

} // namespace Raytracer
