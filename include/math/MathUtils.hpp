#pragma once

#include <cmath>
#include <random>

namespace Raytracer
{

class Math
{
public:
    /**
     * @brief Convert degrees to radians
     */
    static inline double degreesToRadians(double degrees) {
        return degrees * M_PI / 180.0;
    }

    /**
     * @brief Convert radians to degrees
     */
    static inline double radiansToDegrees(double radians) {
        return radians * 180.0 / M_PI;
    }

    /**
     * @brief Generate a random double precision floating point number in the range [min, max)
     */
    static inline double random_double(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }
};

} // namespace Raytracer