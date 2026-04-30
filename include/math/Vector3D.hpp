#pragma once

#include <cmath>
#include <iostream>
#include <random>

namespace Raytracer {

/**
 * @brief 3D vector type used for positions, directions and normals.
 */
struct Vector3D {
    /** Cartesian components. */
    double x, y, z;

    constexpr Vector3D() noexcept : x(0), y(0), z(0) {}
    constexpr Vector3D(double x, double y, double z) noexcept : x(x), y(y), z(z) {}

    /**
     * @brief Add another vector in place.
     */
    Vector3D& operator+=(const Vector3D& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    /**
     * @brief Scale vector by a scalar in place.
     */
    Vector3D& operator*=(double t) noexcept {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    /**
     * @brief Normalize this vector in place when possible.
     */
    inline void normalize() noexcept {
        double l = length();
        if (l > 0) {
            double inv = 1.0 / l;
            x *= inv;
            y *= inv;
            z *= inv;
        }
    }

    /**
     * @brief Divide this vector by a scalar in place.
     */
    Vector3D& operator/=(double t) noexcept { return *this *= (1.0 / t); }

    [[nodiscard]] constexpr Vector3D operator-() const noexcept { return {-x, -y, -z}; }

    [[nodiscard]] constexpr Vector3D operator*(double t) const noexcept {
        return {x * t, y * t, z * t};
    }

    [[nodiscard]] constexpr Vector3D operator/(double t) const noexcept {
        return {x / t, y / t, z / t};
    }

    [[nodiscard]] constexpr bool operator==(const Vector3D& v) const noexcept {
        return (x == v.x && y == v.y && z == v.z);
    }

    /**
     * @brief Return the squared Euclidean length.
     */
    [[nodiscard]] double lengthSquared() const noexcept { return x * x + y * y + z * z; }

    /**
     * @brief Return the Euclidean length.
     */
    [[nodiscard]] double length() const noexcept { return std::sqrt(lengthSquared()); }

    /**
     * @brief Compute dot product with another vector.
     */
    [[nodiscard]] double dot(const Vector3D& v) const noexcept {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * @brief Compute cross product with another vector.
     */
    [[nodiscard]] constexpr Vector3D cross(const Vector3D& v) const noexcept {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    /**
     * @brief Return a normalized copy of this vector.
     */
    [[nodiscard]] Vector3D normalized() const noexcept {
        double l = length();
        if (l <= 0) {
            return Vector3D(0, 0, 0);
        }
        double inv = 1.0 / l;
        return Vector3D(x * inv, y * inv, z * inv);
    }

    /**
     * @brief Check whether all components are near zero.
     */
    [[nodiscard]] bool isNearZero() const noexcept {
        const auto s = 1e-8;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }

    static constexpr Vector3D zero() noexcept { return {0, 0, 0}; }
    static constexpr Vector3D unitX() noexcept { return {1, 0, 0}; }
    static constexpr Vector3D unitY() noexcept { return {0, 1, 0}; }
    static constexpr Vector3D unitZ() noexcept { return {0, 0, 1}; }

    static constexpr Vector3D up() noexcept { return {0, 1, 0}; }
    static constexpr Vector3D down() noexcept { return {0, -1, 0}; }
    static constexpr Vector3D right() noexcept { return {1, 0, 0}; }
    static constexpr Vector3D left() noexcept { return {-1, 0, 0}; }
    static constexpr Vector3D forward() noexcept { return {0, 0, -1}; }
    static constexpr Vector3D back() noexcept { return {0, 0, 1}; }

    /**
     * @brief Return a random scalar in `[min, max)`.
     */
    static double getRandomDouble(double min, double max) {
        thread_local static std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    /**
     * @brief Return a random vector with components in `[-1, 1)`.
     */
    static Vector3D random() {
        return Vector3D(getRandomDouble(-1, 1), getRandomDouble(-1, 1), getRandomDouble(-1, 1));
    }
    /**
     * @brief Return a random uniformly distributed unit vector.
     */
    static Vector3D getRandomUnitVector() {
        while (true) {
            auto p = Vector3D::random();
            auto lensq = p.lengthSquared();

            if (lensq > 1e-160 && lensq <= 1.0) {
                return p / std::sqrt(lensq);
            }
        }
    }
};

[[nodiscard]] constexpr inline Vector3D operator+(const Vector3D& u, const Vector3D& v) noexcept {
    return {u.x + v.x, u.y + v.y, u.z + v.z};
}

[[nodiscard]] constexpr inline Vector3D operator-(const Vector3D& u, const Vector3D& v) noexcept {
    return {u.x - v.x, u.y - v.y, u.z - v.z};
}

[[nodiscard]] constexpr inline Vector3D operator*(double t, const Vector3D& v) noexcept {
    return {t * v.x, t * v.y, t * v.z};
}

using Point3D = Vector3D;

} // namespace Raytracer
