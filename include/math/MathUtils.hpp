#pragma once

#include <cmath>

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
};

} // namespace Raytracer