#pragma once

#include "components/Entity.hpp"
#include "math/MathUtils.hpp"
#include "parser/ISettings.hpp"
#include "parser/PrimitiveSettings.hpp"
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
     * @brief Construct a new Entity Builder object
     *
     * @param name
     */
    EntityBuilder(const ISetting& settings) {
        const std::string type = settings.getString("type");
        _name = std::move(type);
    }

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
        _scale = settings.getVector("scale", Vector3D(1.0, 1.0, 1.0));
        return *this;
    }

    /**
     * @brief Parse scale for shapes with radius
     *
     * Store scale from radius in settings
     * @param settings
     * @return EntityBuilder &
     */
    EntityBuilder& parseRadius(const ISetting& settings) {
        float radius = settings.getFloat("radius");
        _scale = Vector3D(radius, radius, radius);
        return *this;
    }

    /**
     * @brief Parse and assign material to the entity.
     *
     * Attempts to cast the settings to a PrimitiveSetting to extract
     * a shared pointer to an IMaterial. If the cast fails or no material
     * is found, the material remains unchanged (or null).
     *
     * @param settings The settings object (expected to be a PrimitiveSetting).
     * @return EntityBuilder& A reference to the current builder instance.
     */
    EntityBuilder& parseMaterial(const ISetting& settings) {
        const auto* pSettings = dynamic_cast<const PrimitiveSetting*>(&settings);

        if (pSettings) {
            _material = pSettings->getMaterial();
        }
        return *this;
    }

    /**
     * @brief Parse all transformations (position, rotation, scale/radius)
     *
     * @param settings The settings block for the entity
     * @return EntityBuilder&
     */
    EntityBuilder& parseTransform(const ISetting& settings) {
        if (settings.exists("position")) {
            parseTranslation(settings);
        }

        if (settings.exists("rotation")) {
            _rotation = settings.getVector("rotation");
        }

        if (settings.exists("scale")) {
            parseScale(settings);
        }
        return *this;
    }

    /**
     * @brief Parse rotation
     *
     * Store rotation from settings (converted from degrees to radians)
     * @param settings
     * @return EntityBuilder &
     */
    EntityBuilder& parseRotation(const ISetting& settings) {
        const Vector3D rot = settings.getVector("rotation", Vector3D(0.0, 0.0, 0.0));
        _rotation = Vector3D(Math::degreesToRadians(rot.x),
                             Math::degreesToRadians(rot.y),
                             Math::degreesToRadians(rot.z));
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

        Matrix transform = Matrix::rotateX(_rotation.x);
        transform = Matrix::rotateY(_rotation.y) * transform;
        transform = Matrix::rotateZ(_rotation.z) * transform;
        transform = Matrix::scale(_scale.x, _scale.y, _scale.z) * transform;
        transform = Matrix::translate(_position.x, _position.y, _position.z) * transform;

        std::unique_ptr<Entity> entity = std::make_unique<Entity>(_name, _primitive, _material);
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
