#pragma once

#include <memory>

#include "components/IPrimitive.hpp"
#include "math/Matrix.hpp"

namespace Raytracer {

/**
 * @brief Abstract base decorator that applies transformations to primitives.
 *
 * The decorator pattern allows composition of transformations without modifying
 * the underlying primitive. It wraps an IPrimitive and modifies rays before testing
 * hits, then transforms the hit records back to world space.
 *
 * Usage: TransformDecorator assumes the wrapped primitive is UNITAIRE and in local space.
 * The decorator transforms rays from world space to local space before hitting, then
 * transforms the hit back to world space.
 */
class TransformDecorator : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new TransformDecorator wrapping a primitive.
     *
     * @param wrapped The wrapped IPrimitive (typically unitaire)
     * @param transform The transformation matrix to apply
     */
    TransformDecorator(std::shared_ptr<IPrimitive> wrapped, const Matrix& transform) noexcept
        : _wrapped(wrapped), _transform(transform), _inverse(transform.inverse()) {}

    /**
     * @brief Test intersection by transforming the ray to local space.
     *
     * @param r Ray in world space
     * @param ray_t Valid interval for hit distance
     * @param rec Hit record output
     * @return true if intersection found
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    std::string getName() const override { return "Transform[" + _wrapped->getName() + "]"; }

protected:
    std::shared_ptr<IPrimitive> _wrapped;
    Matrix _transform; // World-space transform
    Matrix _inverse;   // Inverse for local-space ray
};

/**
 * @brief Decorator applying translation to a primitive.
 */
class TranslateDecorator : public TransformDecorator {
public:
    /**
     * @brief Create a translated primitive.
     *
     * @param wrapped The base primitive
     * @param x Translation along X
     * @param y Translation along Y
     * @param z Translation along Z
     */
    TranslateDecorator(std::shared_ptr<IPrimitive> wrapped, double x, double y, double z) noexcept
        : TransformDecorator(wrapped, Matrix::translate(x, y, z)) {}

    std::string getName() const override { return "Translate[" + _wrapped->getName() + "]"; }
};

/**
 * @brief Decorator applying rotation around X axis to a primitive.
 */
class RotateXDecorator : public TransformDecorator {
public:
    /**
     * @brief Create a rotated primitive around X axis.
     *
     * @param wrapped The base primitive
     * @param angle_radians Rotation angle in radians
     */
    RotateXDecorator(std::shared_ptr<IPrimitive> wrapped, double angle_radians) noexcept
        : TransformDecorator(wrapped, Matrix::rotateX(angle_radians)) {}

    std::string getName() const override { return "RotateX[" + _wrapped->getName() + "]"; }
};

/**
 * @brief Decorator applying rotation around Y axis to a primitive.
 */
class RotateYDecorator : public TransformDecorator {
public:
    /**
     * @brief Create a rotated primitive around Y axis.
     *
     * @param wrapped The base primitive
     * @param angle_radians Rotation angle in radians
     */
    RotateYDecorator(std::shared_ptr<IPrimitive> wrapped, double angle_radians) noexcept
        : TransformDecorator(wrapped, Matrix::rotateY(angle_radians)) {}

    std::string getName() const override { return "RotateY[" + _wrapped->getName() + "]"; }
};

/**
 * @brief Decorator applying rotation around Z axis to a primitive.
 */
class RotateZDecorator : public TransformDecorator {
public:
    /**
     * @brief Create a rotated primitive around Z axis.
     *
     * @param wrapped The base primitive
     * @param angle_radians Rotation angle in radians
     */
    RotateZDecorator(std::shared_ptr<IPrimitive> wrapped, double angle_radians) noexcept
        : TransformDecorator(wrapped, Matrix::rotateZ(angle_radians)) {}

    std::string getName() const override { return "RotateZ[" + _wrapped->getName() + "]"; }
};

/**
 * @brief Decorator applying non-uniform scale to a primitive.
 */
class ScaleDecorator : public TransformDecorator {
public:
    /**
     * @brief Create a scaled primitive.
     *
     * @param wrapped The base primitive
     * @param sx Scale factor along X
     * @param sy Scale factor along Y
     * @param sz Scale factor along Z
     */
    ScaleDecorator(std::shared_ptr<IPrimitive> wrapped, double sx, double sy, double sz) noexcept
        : TransformDecorator(wrapped, Matrix::scale(sx, sy, sz)) {}

    std::string getName() const override { return "Scale[" + _wrapped->getName() + "]"; }
};

} // namespace Raytracer
