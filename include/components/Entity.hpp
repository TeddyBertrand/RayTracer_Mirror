#pragma once

#include <memory>
#include <string>

#include "components/IMaterial.hpp"
#include "components/IPrimitive.hpp"
#include "math/HitRecord.hpp"
#include "math/Interval.hpp"
#include "math/Matrix.hpp"
#include "math/Ray.hpp"

namespace Raytracer {

/**
 * @brief Entity class - a decorator that combines transformation and material.
 *
 * An Entity is a decorator that:
 * - Wraps a primitive (IPrimitive)
 * - Applies transformations (translation, rotation, scale)
 * - Includes the material for rendering
 * - Inherits from IPrimitive for uniform composition
 *
 * This allows for flexible composition: Entity wraps any IPrimitive (Sphere, Plane, etc.)
 * and can apply transformations without needing separate decorator layers.
 *
 * Usage:
 *   auto sphere = std::make_shared<Sphere>(...);
 *   auto entity = std::make_shared<Entity>("My Sphere", sphere, material);
 *   entity->translate(10, 0, 0);
 *   entity->rotateX(M_PI / 4);
 *   scene.addPrimitive(entity);  // Entity IS an IPrimitive
 */
class Entity : public IPrimitive {
public:
    /**
     * @brief Construct a new Entity.
     *
     * @param name Descriptive name for debugging
     * @param primitive The wrapped primitive
     * @param material The material (optional)
     */
    Entity(const std::string& name,
           std::shared_ptr<IPrimitive> primitive,
           std::shared_ptr<IMaterial> material = nullptr) noexcept
        : _name(name), _primitive(primitive), _material(material), _transform(), _transform_inv() {}

    /**
     * @brief Apply translation transformation.
     * @param x Translation along X axis.
     * @param y Translation along Y axis.
     * @param z Translation along Z axis.
     */
    void translate(double x, double y, double z) noexcept {
        _transform = Matrix::translate(x, y, z) * _transform;
        _transform_inv = _transform.inverse();
    }

    /**
     * @brief Apply rotation transformation around X axis.
     * @param angle Angle in radians.
     */
    void rotateX(double angle) noexcept {
        _transform = Matrix::rotateX(angle) * _transform;
        _transform_inv = _transform.inverse();
    }

    /**
     * @brief Apply rotation transformation around Y axis.
     * @param angle Angle in radians.
     */
    void rotateY(double angle) noexcept {
        _transform = Matrix::rotateY(angle) * _transform;
        _transform_inv = _transform.inverse();
    }

    /**
     * @brief Apply rotation transformation around Z axis.
     * @param angle Angle in radians.
     */
    void rotateZ(double angle) noexcept {
        _transform = Matrix::rotateZ(angle) * _transform;
        _transform_inv = _transform.inverse();
    }

    /**
     * @brief Apply scale transformation.
     * @param sx Scale factor along X axis.
     * @param sy Scale factor along Y axis.
     * @param sz Scale factor along Z axis.
     */
    void scale(double sx, double sy, double sz) noexcept {
        _transform = Matrix::scale(sx, sy, sz) * _transform;
        _transform_inv = _transform.inverse();
    }

    /**
     * @brief Test ray/object intersection with transformation and material.
     *
     * @param r Candidate ray.
     * @param ray_t Valid interval for hit distance along the ray.
     * @param rec Output hit information when an intersection is found.
     * @return true if a valid hit exists in the interval.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        Ray local_ray(_transform_inv * r.origin(),
                      _transform_inv.transformDirection(r.direction()));

        if (!_primitive->hit(local_ray, ray_t, rec)) {
            return false;
        }

        rec.point = _transform * rec.point;

        rec.normal = _transform_inv.transpose().transformDirection(rec.normal).normalized();

        rec.front_face = r.direction().dot(rec.normal) < 0;
        if (!rec.front_face) {
            rec.normal = -rec.normal;
        }

        rec.t = (rec.point - r.origin()).length();

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

    void setTransform(const Matrix& m) {
        _transform = m;
        _transform_inv = m.inverse();
    }

    AABB getBoundingBox() const override {
        if (!_primitive) {
            return AABB();
        }

        AABB local_box = _primitive->getBoundingBox();

        if (local_box.isEmpty()) {
            return AABB();
        }

        if (local_box.isInfinite()) {
            return AABB::infinite();
        }

        Point3D min(std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity());
        Point3D max(-std::numeric_limits<double>::infinity(),
                    -std::numeric_limits<double>::infinity(),
                    -std::numeric_limits<double>::infinity());

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    double x = (i == 0) ? local_box.min.x : local_box.max.x;
                    double y = (j == 0) ? local_box.min.y : local_box.max.y;
                    double z = (k == 0) ? local_box.min.z : local_box.max.z;

                    Point3D transformed_point = _transform * Point3D(x, y, z);

                    min.x = std::min(min.x, transformed_point.x);
                    min.y = std::min(min.y, transformed_point.y);
                    min.z = std::min(min.z, transformed_point.z);

                    max.x = std::max(max.x, transformed_point.x);
                    max.y = std::max(max.y, transformed_point.y);
                    max.z = std::max(max.z, transformed_point.z);
                }
            }
        }
        return AABB{min, max};
    }

private:
    std::string _name;
    std::shared_ptr<IPrimitive> _primitive;
    std::shared_ptr<IMaterial> _material;
    Matrix _transform;
    Matrix _transform_inv;
};

} // namespace Raytracer
