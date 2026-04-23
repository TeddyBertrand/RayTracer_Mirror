#pragma once

#include <cmath>
#include <random>
#include "math/Vector3D.hpp"

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

    /**
     * @brief Calculate the reflection of a vector on a normal
     */
    static inline Vector3D reflect(const Vector3D& v, const Vector3D& n) {
        return v - n * (2.0 * v.dot(n));
    }

    /**
     * @brief Calculate refraction according to Snell-Descartes law
     * @param uv The incident vector (must be normalized)
     * @param n The surface normal
     * @param etai_over_etat The ratio of refractive indices
     */
    static inline Vector3D refract(const Vector3D& uv, const Vector3D& n, double etai_over_etat) {
        double cos_theta = std::min((-uv).dot(n), 1.0);
        
        Vector3D r_out_perp = (uv + n * cos_theta) * etai_over_etat;
        
        double r_out_parallel_len = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared()));
        Vector3D r_out_parallel = n * r_out_parallel_len;
        
        return r_out_perp + r_out_parallel;
    }

    /**
     * @brief Schlick approximation for reflectance (Fresnel effect)
     */
    static inline double reflectance(double cosine, double ref_idx) {
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
    }
};

} // namespace Raytracer