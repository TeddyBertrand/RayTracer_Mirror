#pragma once

#include <limits>

namespace Raytracer
{

struct Interval
{
    double min, max;

    constexpr Interval() noexcept 
        : min(+std::numeric_limits<double>::infinity()), 
            max(-std::numeric_limits<double>::infinity()) {}

    constexpr Interval(double _min, double _max) noexcept : min(_min), max(_max) {}

    [[nodiscard]] constexpr bool contains(double x) const noexcept {
        return min <= x && x <= max;
    }

    [[nodiscard]] constexpr bool surrounds(double x) const noexcept {
        return min < x && x < max;
    }
    
    [[nodiscard]] double clamp(double x) const noexcept {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty    (+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
inline const Interval Interval::universe (-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());

} // namespace Raytracer
