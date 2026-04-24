#pragma once

#include <limits>

namespace Raytracer {

/**
 * @brief Numeric interval utility used for range checks and clamping.
 */
struct Interval {
        /** Lower bound. */
    double min, max;

        /**
         * @brief Build an empty interval by default.
         */
    constexpr Interval() noexcept
        : min(+std::numeric_limits<double>::infinity()),
          max(-std::numeric_limits<double>::infinity()) {}

        /**
         * @brief Build an interval from explicit bounds.
         */
    constexpr Interval(double _min, double _max) noexcept : min(_min), max(_max) {}

        /**
         * @brief Check whether value belongs to the closed interval `[min, max]`.
         */
    [[nodiscard]] constexpr bool contains(double x) const noexcept { return min <= x && x <= max; }

        /**
         * @brief Check whether value belongs to the open interval `(min, max)`.
         */
    [[nodiscard]] constexpr bool surrounds(double x) const noexcept { return min < x && x < max; }

        /**
         * @brief Clamp value inside interval bounds.
         */
    [[nodiscard]] double clamp(double x) const noexcept {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    /** Predefined empty interval. */
    static const Interval empty;

    /** Predefined interval spanning all finite values. */
    static const Interval universe;
};

inline const Interval Interval::empty(+std::numeric_limits<double>::infinity(),
                                      -std::numeric_limits<double>::infinity());
inline const Interval Interval::universe(-std::numeric_limits<double>::infinity(),
                                         +std::numeric_limits<double>::infinity());

} // namespace Raytracer
