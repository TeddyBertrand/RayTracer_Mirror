#include "SFMLDisplay.hpp"
#include <algorithm>
#include <iostream>

namespace Raytracer {

SFMLDisplay::SFMLDisplay(const ISetting& settings)
    : _width(settings.getInt("width", 800)), _height(settings.getInt("height", 600)),
      _title(settings.getString("title", "RayTracer")), _scale(settings.getInt("scale", 1)),
      _imageWidth(0), _imageHeight(0) {}

SFMLDisplay::~SFMLDisplay() {
    if (_window) {
        _window->close();
    }
}

bool SFMLDisplay::init() {
    try {
        _window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(sf::Vector2u(_width * _scale, _height * _scale)),
            _title,
            sf::Style::Close | sf::Style::Resize);

        if (!_window) {
            std::cerr << "Failed to create SFML window" << std::endl;
            return false;
        }

        if (_imageWidth <= 0 || _imageHeight <= 0) {
            _imageWidth = _width;
            _imageHeight = _height;
        }

        _pixelBuffer.assign(static_cast<size_t>(_imageWidth) * _imageHeight * 4, 0);
        _texture = std::make_unique<sf::Texture>(sf::Vector2u(_imageWidth, _imageHeight));
        _sprite = std::make_unique<sf::Sprite>(*_texture);
        _sprite->setPosition(sf::Vector2f(0.f, 0.f));
        _sprite->setOrigin(sf::Vector2f(0.f, 0.f));

        _view = sf::View(
            sf::Vector2f(static_cast<float>(_imageWidth) * 0.5f,
                         static_cast<float>(_imageHeight) * 0.5f),
            sf::Vector2f(static_cast<float>(_imageWidth), static_cast<float>(_imageHeight)));

        _window->setFramerateLimit(60);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during SFMLDisplay init: " << e.what() << std::endl;
        return false;
    }
}

void SFMLDisplay::setImageSize(int width, int height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    _imageWidth = width;
    _imageHeight = height;
}

void SFMLDisplay::updateView() {
    if (!_window || _imageWidth <= 0 || _imageHeight <= 0) {
        return;
    }

    const auto windowSize = _window->getSize();
    const float windowAspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float imageAspect = static_cast<float>(_imageWidth) / static_cast<float>(_imageHeight);

    sf::FloatRect viewport(sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 1.f));
    if (windowAspect > imageAspect) {
        const float width = imageAspect / windowAspect;
        viewport.position.x = (1.f - width) * 0.5f;
        viewport.size.x = width;
    } else {
        const float height = windowAspect / imageAspect;
        viewport.position.y = (1.f - height) * 0.5f;
        viewport.size.y = height;
    }

    _view =
        sf::View(sf::Vector2f(static_cast<float>(_imageWidth) * 0.5f,
                              static_cast<float>(_imageHeight) * 0.5f),
                 sf::Vector2f(static_cast<float>(_imageWidth), static_cast<float>(_imageHeight)));
    _view.setViewport(viewport);
}

void SFMLDisplay::updatePixel(int x, int y, unsigned int color) {
    if (x < 0 || y < 0) {
        return;
    }

    if (x >= _imageWidth || y >= _imageHeight || !_pixelBuffer.size()) {
        return;
    }

    const int idx = (y * _imageWidth + x) * 4;

    _pixelBuffer[idx + 0] = (color >> 24) & 0xFF;
    _pixelBuffer[idx + 1] = (color >> 16) & 0xFF;
    _pixelBuffer[idx + 2] = (color >> 8) & 0xFF;
    _pixelBuffer[idx + 3] = (color >> 0) & 0xFF;
}

void SFMLDisplay::refresh() {
    if (!_window || !_texture || !_sprite) {
        return;
    }

    _texture->update(_pixelBuffer.data());

    _window->clear(sf::Color::Black);

    updateView();
    _window->setView(_view);
    _sprite->setScale(sf::Vector2f(1.f, 1.f));
    _sprite->setPosition(sf::Vector2f(0.f, 0.f));
    _sprite->setOrigin(sf::Vector2f(0.f, 0.f));

    _window->draw(*_sprite);
    _window->display();

    auto event = _window->pollEvent();
    while (event) {
        if (event->is<sf::Event::Resized>()) {
            updateView();
            _window->setView(_view);
        }
        if (event->is<sf::Event::Closed>()) {
            _window->close();
        }
        event = _window->pollEvent();
    }
}

bool SFMLDisplay::isOpen() const { return _window && _window->isOpen(); }

void SFMLDisplay::close() {
    if (_window) {
        _window->close();
    }
}

extern "C" {

const char* getName() { return "sfml"; }

IGraphic* createPlugin(const ISetting& settings) { return new SFMLDisplay(settings); }
}

} // namespace Raytracer
