#pragma once

#include <memory>
#include <string>

#include "components/IMaterial.hpp"
#include "components/IPrimitive.hpp"

namespace Raytracer {

/**
 * @brief Entity class - represents a transformed primitive with a material.
 *
 * An Entity is the highest-level composition:
 * - Wraps a transformed primitive (IPrimitive with decorators applied)
 * - Includes the material for rendering
 * - Can be positioned, rotated, scaled in the scene
 *
 * This is the object you actually add to a Scene.
 *
 * Usage:
 *   auto sphere = std::make_shared<Sphere>(...);
 *   auto translated = std::make_shared<TranslateDecorator>(sphere, 10, 0, 0);
 *   Entity entity("My Sphere", translated, material);
 *   scene.addEntity(entity);
 */
class Entity {
public:
    /**
     * @brief Construct a new Entity.
     *
     * @param name Descriptive name for debugging
     * @param primitive The transformed primitive (with decorators)
     * @param material The material (optional)
     */
    Entity(const std::string& name,
           std::shared_ptr<IPrimitive> primitive,
           std::shared_ptr<IMaterial> material = nullptr) noexcept
        : _name(name), _primitive(primitive), _material(material) {}

    /**
     * @brief Get the entity name.
     */
    [[nodiscard]] const std::string& getName() const noexcept { return _name; }

    /**
     * @brief Get the wrapped primitive.
     */
    [[nodiscard]] std::shared_ptr<IPrimitive> getPrimitive() const noexcept { return _primitive; }

    /**
     * @brief Get the material.
     */
    [[nodiscard]] std::shared_ptr<IMaterial> getMaterial() const noexcept { return _material; }

    /**
     * @brief Set the material.
     */
    void setMaterial(std::shared_ptr<IMaterial> material) noexcept { _material = material; }

private:
    std::string _name;
    std::shared_ptr<IPrimitive> _primitive;
    std::shared_ptr<IMaterial> _material;
};

} // namespace Raytracer
