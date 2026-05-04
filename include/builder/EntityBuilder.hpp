#pragma once

#include "components/Entity.hpp"
#include "parser/ISettings.hpp"
#include <utility>

namespace Raytracer {

class EntityBuilder {
public:
    /**
     * @brief Construct a new Entity Builder object
     *
     * @param name
     */
    EntityBuilder(std::string name) : _name(std::move(name)) {}

    /**
     * @brief Set the Primitive object
     *
     * @param primitive
     * @return EntityBuilder &
     */
    EntityBuilder& setPrimitive(std::shared_ptr<IPrimitive> primitive) {
        _primitive = primitive;
        return *this;
    }

    /**
     * @brief Parse translation
     *
     * Store translation from settings
     * @param settings
     * @return EntityBuilder&
     */
    EntityBuilder& parseTranslation(const ISetting& settings) {
        _position = settings.getVector("position");
        return *this;
    }

    /**
     * @brief Parse scale
     *
     * Store scale from settings
     * @param settings
     * @return EntityBuilder&
     */
    EntityBuilder& parseScale(const ISetting& settings) {
        _scale = settings.getVector("scale");
        return *this;
    }

    /**
     * @brief Parse scale for shapes with radius
     *
     * Store scale from radius in settings
     * @param settings
     * @return EntityBuilder &
     */
    EntityBuilder& parseScaleRadius(const ISetting& settings) {
        float radius = settings.getFloat("radius");
        _scale = Vector3D(radius, radius, radius);
        return *this;
    }

    /**
     * @brief Parse rotation
     *
     * Store rotation from settings (in degrees)
     * @param settings
     * @return EntityBuilder &
     */
    EntityBuilder& parseRotation(const ISetting& settings) {
        _rotation = settings.getVector("rotation");
        return *this;
    }

    /**
     * @brief Add scale
     *
     * Add scale with raw values
     * @param sx
     * @param sy
     * @param sz
     * @return EntityBuilder &
     */
    EntityBuilder& addScale(double sx, double sy, double sz) {
        _scale = Vector3D(sx, sy, sz);
        return *this;
    }

    /**
     * @brief Add translation
     *
     * Add translation with raw values
     * @param x
     * @param y
     * @param z
     * @return EntityBuilder &
     */
    EntityBuilder& addTranslation(double x, double y, double z) {
        _position = Vector3D(x, y, z);
        return *this;
    }

    /**
     * @brief Add rotation
     *
     * Add rotation with raw values (in radians)
     * @param x
     * @param y
     * @param z
     * @return EntityBuilder &
     */
    EntityBuilder& addRotation(double x, double y, double z) {
        _rotation = Vector3D(x, y, z);
        return *this;
    }

    /**
     * @brief Build method
     *
     * Create and return a unique ptr of entity with current shape and
     * transformations. Order: Rotate -> Scale -> Translate
     * @return std::unique_ptr<Entity>
     */
    std::unique_ptr<Entity> build() {
        if (!_primitive)
            return NULL;

        Matrix transform = Matrix::translate(_position.x, _position.y, _position.z);
        transform = transform * Matrix::scale(_scale.x, _scale.y, _scale.z);
        transform = transform * Matrix::rotateX(_rotation.x);
        transform = transform * Matrix::rotateY(_rotation.y);
        transform = transform * Matrix::rotateZ(_rotation.z);

        std::unique_ptr<Entity> entity = std::make_unique<Entity>(_name, _primitive);
        entity->setTransform(transform);
        return entity;
    }

private:
    std::string _name;
    std::shared_ptr<IPrimitive> _primitive;
    std::shared_ptr<IMaterial> _material;

    Vector3D _position{0.0, 0.0, 0.0};
    Vector3D _rotation{0.0, 0.0, 0.0};
    Vector3D _scale{1.0, 1.0, 1.0};
};

}; // namespace Raytracer
