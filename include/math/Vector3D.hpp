#pragma once
#include <cmath>
#include <iostream>

namespace Raytracer
{

struct Vector3D
{
    double x, y, z;

    constexpr Vector3D() noexcept : x(0), y(0), z(0) {}
    constexpr Vector3D(double x, double y, double z) noexcept : x(x), y(y), z(z) {}

    Vector3D& operator+=(const Vector3D& v) noexcept {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector3D& operator*=(double t) noexcept {
        x *= t; y *= t; z *= t;
        return *this;
    }

    [[nodiscard]] constexpr Vector3D operator-() const noexcept {
        return {-x, -y, -z};
    }


    [[nodiscard]] double length_squared() const noexcept { return x*x + y*y + z*z; }
    [[nodiscard]] double length() const noexcept { return std::sqrt(length_squared()); }

    [[nodiscard]] double dot(const Vector3D& v) const noexcept {
        return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] Vector3D normalized() const noexcept {
        double l = length();
        return (l > 0) ? (*this * (1.0 / l)) : Vector3D(0,0,0);
    }


    static constexpr Vector3D zero()     noexcept { return {0, 0, 0}; }
    static constexpr Vector3D unit_x()   noexcept { return {1, 0, 0}; }
    static constexpr Vector3D unit_y()   noexcept { return {0, 1, 0}; }
    static constexpr Vector3D unit_z()   noexcept { return {0, 0, 1}; }

    static constexpr Vector3D up()       noexcept { return {0, 1, 0}; }
    static constexpr Vector3D down()     noexcept { return {0, -1, 0}; }
    static constexpr Vector3D right()    noexcept { return {1, 0, 0}; }
    static constexpr Vector3D left()     noexcept { return {-1, 0, 0}; }
    static constexpr Vector3D forward()  noexcept { return {0, 0, -1}; }
    static constexpr Vector3D back()     noexcept { return {0, 0, 1}; }
};


[[nodiscard]] inline Vector3D operator+(const Vector3D& u, const Vector3D& v) noexcept {
    return {u.x + v.x, u.y + v.y, u.z + v.z};
}

[[nodiscard]] inline Vector3D operator-(const Vector3D& u, const Vector3D& v) noexcept {
    return {u.x - v.x, u.y - v.y, u.z - v.z};
}

[[nodiscard]] inline Vector3D operator*(double t, const Vector3D& v) noexcept {
    return {t * v.x, t * v.y, t * v.z};
}

[[nodiscard]] inline Vector3D operator*(const Vector3D& v, double t) noexcept {
    return t * v;
}

using Point3D = Vector3D;

} // namespace Raytracer
