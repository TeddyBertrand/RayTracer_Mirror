#pragma once

#include "components/Light.hpp"
#include "parser/ISettings.hpp"
#include "math/Matrix.hpp"
#include "math/MathUtils.hpp"
#include <memory>

namespace Raytracer {

/**
 * @brief Builder and decorator for Light components.
 * Simplified version focusing on properties relevant for light sources (position, rotation, color).
 */
class LightBuilder {
public:
    /**
     * @brief Construct a new Light Builder with a source.
     * @param source The geometric light source to decorate.
     */
    LightBuilder(std::shared_ptr<ILightSource> source) : _source(std::move(source)) {}

    /**
     * @brief Parse common light properties (color, intensity).
     * @param settings The settings block.
     * @return LightBuilder& 
     */
    LightBuilder& parseCommon(const ISetting& settings) {
        _color = settings.getColor("color", Color(1.0, 1.0, 1.0));
        _intensity = settings.getFloat("intensity", 1.0f);
        return *this;
    }

    /**
     * @brief Parse transformations (position, rotation).
     * @param settings The settings block.
     * @return LightBuilder& 
     */
    LightBuilder& parseTransform(const ISetting& settings) {
        if (settings.exists("position")) {
            _position = settings.getVector("position");
        }
        if (settings.exists("rotation")) {
            _rotation = settings.getVector("rotation");
        }
        return *this;
    }

    /**
     * @brief Build and return the decorated Light object.
     * @return ILight* The constructed light.
     */
    ILight* build() {
        Matrix transform = Matrix::translate(_position.x, _position.y, _position.z);
        transform = transform * Matrix::rotateX(Math::degreesToRadians(_rotation.x));
        transform = transform * Matrix::rotateY(Math::degreesToRadians(_rotation.y));
        transform = transform * Matrix::rotateZ(Math::degreesToRadians(_rotation.z));

        auto light = new Light(_source, _color, _intensity);
        light->setTransform(transform);
        return light;
    }

private:
    std::shared_ptr<ILightSource> _source;
    Color _color{1.0, 1.0, 1.0};
    double _intensity{1.0};
    
    Vector3D _position{0.0, 0.0, 0.0};
    Vector3D _rotation{0.0, 0.0, 0.0};
};

} // namespace Raytracer
