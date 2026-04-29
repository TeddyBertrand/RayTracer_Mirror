#pragma once

#include <memory>
#include <string>

#include "components/IMaterial.hpp"
#include "components/IPrimitive.hpp"
#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Ray.hpp"

namespace Raytracer {

/**
 * @brief Entity class - a decorator that adds material to a primitive.
 *
 * An Entity is a decorator that:
 * - Wraps a transformed primitive (IPrimitive with decorators applied)
 * - Includes the material for rendering
 * - Inherits from IPrimitive for uniform composition
 *
 * This allows for flexible composition: Entity can wrap any IPrimitive (Sphere, Plane,
 * TransformDecorator, or even another Entity).
 *
 * Usage:
 *   auto sphere = std::make_shared<Sphere>(...);
 *   auto translated = std::make_shared<TranslateDecorator>(sphere, 10, 0, 0);
 *   auto entity = std::make_shared<Entity>("My Sphere", translated, material);
 *   scene.addPrimitive(entity);  // Entity IS an IPrimitive
 */
class Entity : public IPrimitive {
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
     * @brief Test ray/object intersection and attach material to hit record.
     *
     * @param r Candidate ray.
     * @param ray_t Valid interval for hit distance along the ray.
     * @param rec Output hit information when an intersection is found.
     * @return true if a valid hit exists in the interval.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        if (!_primitive->hit(r, ray_t, rec)) {
            return false;
        }
        rec.material = _material;
        return true;
    }

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
