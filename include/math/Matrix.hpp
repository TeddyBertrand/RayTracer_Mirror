#pragma once

#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief 4x4 Matrix class used for 3D transformations.
 */
class Matrix {
public:
    double m[4][4];

    /**
     * @brief Construct a new Matrix object natively initialized to Identity Matrix.
     */
    Matrix() noexcept;

    /**
     * @brief Matrix-Matrix multiplication.
     */
    Matrix operator*(const Matrix& other) const noexcept;

    /**
     * @brief Matrix-Vector multiplication (Transformation).
     */
    Vector3D operator*(const Vector3D& vec) const noexcept;

    /**
     * @brief Generate a rotation matrix around the X-axis.
     * @param angle_radians Angle in radians.
     */
    static Matrix rotateX(double angle_radians) noexcept;

    /**
     * @brief Generate a rotation matrix around the Y-axis.
     * @param angle_radians Angle in radians.
     */
    static Matrix rotateY(double angle_radians) noexcept;

    /**
     * @brief Generate a rotation matrix around the Z-axis.
     * @param angle_radians Angle in radians.
     */
    static Matrix rotateZ(double angle_radians) noexcept;

    /**
     * @brief Generates a transformation utility (LookAt).
     * Transforms from world space to camera view space.
     */
    static Matrix
    lookAt(const Point3D& position, const Point3D& target, const Vector3D& up) noexcept;
};

} // namespace Raytracer
