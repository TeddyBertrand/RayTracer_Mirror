#pragma once

#include "math/Interval.hpp"
#include <iostream>
#include <cmath>

namespace Raytracer
{

struct Color
{
    double r, g, b;

    constexpr Color() noexcept : r(0), g(0), b(0) {}
    constexpr Color(double r, double g, double b) noexcept : r(r), g(g), b(b) {}

    Color& operator+=(const Color& c) noexcept {
        r += c.r; g += c.g; b += c.b;
        return *this;
    }

    Color& operator*=(double t) noexcept {
        r *= t; g *= t; b *= t;
        return *this;
    }

    Color& operator*=(const Color& c) noexcept {
        r *= c.r; g *= c.g; b *= c.b;
        return *this;
    }

    [[nodiscard]] bool isNearZero() const noexcept {
        const auto s = 1e-8;
        return (std::fabs(r) < s) && (std::fabs(g) < s) && (std::fabs(b) < s);
    }

    static int toByte(double value) noexcept {
        static const Interval intensity(0.0, 0.999);
        return static_cast<int>(256 * intensity.clamp(std::sqrt(value)));
    }
};

inline Color operator+(const Color& u, const Color& v) noexcept { return {u.r + v.r, u.g + v.g, u.b + v.b}; }
inline Color operator*(const Color& u, const Color& v) noexcept { return {u.r * v.r, u.g * v.g, u.b * v.b}; }
inline Color operator*(double t, const Color& v) noexcept { return {t * v.r, t * v.g, t * v.b}; }
inline Color operator*(const Color& v, double t) noexcept { return t * v; }

inline Color operator/(const Color& v, double t) noexcept { return (1.0 / t) * v; }

} // namespace Raytracer
