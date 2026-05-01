#pragma once

#include <cmath>
#include <random>

#include "math/Vector3D.hpp"

namespace Raytracer {

/**
 * @brief Collection of math helpers used by the ray tracer.
 */
class Math {
public:
    /**
     * @brief Convert degrees to radians.
     * @param degrees Angle in degrees.
     * @return Equivalent angle in radians.
     */
    static inline double degreesToRadians(double degrees) { return degrees * M_PI / 180.0; }

    /**
     * @brief Convert radians to degrees.
     * @param radians Angle in radians.
     * @return Equivalent angle in degrees.
     */
    static inline double radiansToDegrees(double radians) { return radians * 180.0 / M_PI; }

    /**
     * @brief Generate a random floating-point value in `[min, max)`.
     * @param min Inclusive lower bound.
     * @param max Exclusive upper bound.
     */
    static inline double randomDouble(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    /**
     * @brief Generate a random integer in `[min, max]`.
     * @param min Inclusive lower bound.
     * @param max Inclusive upper bound.
     */
    static inline int randomInt(int min, int max) {
        static std::uniform_int_distribution<int> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    /**
     * @brief Reflect vector `v` around surface normal `n`.
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

        double r_out_parallel_len = -std::sqrt(std::abs(1.0 - r_out_perp.lengthSquared()));
        Vector3D r_out_parallel = n * r_out_parallel_len;

        return r_out_perp + r_out_parallel;
    }

    /**
     * @brief Approximate Fresnel reflectance using Schlick's formula.
     * @param cosine Cosine of the incident angle.
     * @param ref_idx Relative refractive index.
     * @return Approximated reflectance ratio in `[0, 1]`.
     */
    static inline double reflectance(double cosine, double ref_idx) {
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
    }
};

} // namespace Raytracer
