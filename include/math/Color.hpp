#pragma once

#include "math/Interval.hpp"
#include <cmath>
#include <iostream>

namespace Raytracer {

/**
 * @brief RGB color container with arithmetic helpers used in shading.
 */
struct Color {
    /** Red channel (linear space). */
    double r, g, b;

    /**
     * @brief Build black color.
     */
    constexpr Color() noexcept : r(0), g(0), b(0) {}

    /**
     * @brief Build color from explicit channel values.
     */
    constexpr Color(double r, double g, double b) noexcept : r(r), g(g), b(b) {}

    /**
     * @brief Add another color in place.
     */
    Color& operator+=(const Color& c) noexcept {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    /**
     * @brief Multiply all channels by a scalar.
     */
    Color& operator*=(double t) noexcept {
        r *= t;
        g *= t;
        b *= t;
        return *this;
    }

    /**
     * @brief Multiply channels component-wise by another color.
     */
    Color& operator*=(const Color& c) noexcept {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        return *this;
    }

    /**
     * @brief Check whether the color is close to black.
     */
    [[nodiscard]] bool isNearZero() const noexcept {
        const auto s = 1e-8;
        return (std::fabs(r) < s) && (std::fabs(g) < s) && (std::fabs(b) < s);
    }

    /**
     * @brief Convert a linear channel value to an 8-bit output value.
     *
     * Applies gamma correction (`sqrt`) and clamps to `[0, 255]`.
     */
    static int toByte(double value) noexcept {
        static const Interval intensity(0.0, 0.999);
        return static_cast<int>(256 * intensity.clamp(std::sqrt(value)));
    }
};

inline Color operator+(const Color& u, const Color& v) noexcept {
    return {u.r + v.r, u.g + v.g, u.b + v.b};
}
inline Color operator*(const Color& u, const Color& v) noexcept {
    return {u.r * v.r, u.g * v.g, u.b * v.b};
}
inline Color operator*(double t, const Color& v) noexcept { return {t * v.r, t * v.g, t * v.b}; }
inline Color operator*(const Color& v, double t) noexcept { return t * v; }

inline Color operator/(const Color& v, double t) noexcept { return (1.0 / t) * v; }

} // namespace Raytracer
