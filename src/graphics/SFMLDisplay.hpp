#pragma once

#include "components/IGraphic.hpp"
#include "parser/ISettings.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Raytracer {

/**
 * @brief SFML-based graphics implementation for real-time pixel display
 */
class SFMLDisplay : public IGraphic {
public:
    /**
     * @brief Construct display with configuration settings
     * @param settings Configuration: width, height, title, scale
     */
    explicit SFMLDisplay(const ISetting& settings);

    ~SFMLDisplay() override;

    bool init() override;
    void setImageSize(int width, int height) override;
    void updatePixel(int x, int y, unsigned int color) override;
    void refresh() override;
    bool isOpen() const override;
    void close() override;

private:
    void updateView();

    int _width;
    int _height;
    std::string _title;
    int _scale;

    int _imageWidth;
    int _imageHeight;

    std::unique_ptr<sf::RenderWindow> _window;
    std::unique_ptr<sf::Texture> _texture;
    std::unique_ptr<sf::Sprite> _sprite;
    sf::View _view;
    std::vector<std::uint8_t> _pixelBuffer;
};

} // namespace Raytracer
