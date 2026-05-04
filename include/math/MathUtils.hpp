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
    struct QuadraticCoeffs {
        double a, b, c;
    };
    struct CubicCoeffs {
        double a, b, c, d;
    };
    struct QuarticCoeffs {
        double a, b, c, d, e;
    };
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
        thread_local static std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    /**
     * @brief Generate a random integer in `[min, max]`.
     * @param min Inclusive lower bound.
     * @param max Inclusive upper bound.
     */
    static inline int randomInt(int min, int max) {
        thread_local static std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    static bool
    solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1) {
        float discr = b * b - 4 * a * c;
        if (discr < 0)
            return false;
        else if (discr == 0)
            x0 = x1 = -0.5 * b / a;
        else {
            float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1)
            std::swap(x0, x1);

        return true;
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

    /**
     * @brief Résout une équation quadratique : ax^2 + bx + c = 0.
     * @param coeffs Structure contenant les coefficients a, b et c.
     * @param roots Tableau pour stocker les racines réelles trouvées.
     * @return Nombre de racines réelles trouvées (0, 1 ou 2).
     */
    static inline int solveQuadratic(const QuadraticCoeffs& coeffs, double roots[2]) {
        double a = coeffs.a;
        double b = coeffs.b;
        double c = coeffs.c;
        if (std::abs(a) < 1e-9) {
            if (std::abs(b) < 1e-9)
                return 0;
            roots[0] = -c / b;
            return 1;
        }
        double disc = b * b - 4 * a * c;
        if (disc < 0)
            return 0;
        if (std::abs(disc) < 1e-12) {
            roots[0] = -b / (2 * a);
            return 1;
        }
        double sqrt_disc = std::sqrt(disc);
        roots[0] = (-b - sqrt_disc) / (2 * a);
        roots[1] = (-b + sqrt_disc) / (2 * a);
        return 2;
    }

    /**
     * @brief Résout une équation cubique : ax^3 + bx^2 + cx + d = 0.
     * @param coeffs Structure contenant les coefficients a, b, c et d.
     * @param roots Tableau pour stocker les racines réelles trouvées.
     * @return Nombre de racines réelles trouvées (1, 2 ou 3).
     */
    static inline int solveCubic(const CubicCoeffs& coeffs, double roots[3]) {
        double a = coeffs.a;
        double b = coeffs.b;
        double c = coeffs.c;
        double d = coeffs.d;
        if (std::abs(a) < 1e-9)
            return solveQuadratic({b, c, d}, roots);

        double A = b / a;
        double B = c / a;
        double C = d / a;
        double Q = (3.0 * B - A * A) / 9.0;
        double R = (9.0 * A * B - 27.0 * C - 2.0 * A * A * A) / 54.0;
        double D = Q * Q * Q + R * R;

        if (D > 0) {
            double S = std::cbrt(R + std::sqrt(D));
            double T = std::cbrt(R - std::sqrt(D));
            roots[0] = -A / 3.0 + (S + T);
            return 1;
        } else if (std::abs(D) < 1e-12) {
            double S = std::cbrt(R);
            roots[0] = -A / 3.0 + 2.0 * S;
            roots[1] = -A / 3.0 - S;
            return 2;
        } else {
            double arg = R / std::sqrt(-Q * Q * Q);
            double theta = std::acos(std::max(-1.0, std::min(1.0, arg)));
            double sqrtQ = 2.0 * std::sqrt(-Q);
            roots[0] = sqrtQ * std::cos(theta / 3.0) - A / 3.0;
            roots[1] = sqrtQ * std::cos((theta + 2.0 * M_PI) / 3.0) - A / 3.0;
            roots[2] = sqrtQ * std::cos((theta + 4.0 * M_PI) / 3.0) - A / 3.0;
            return 3;
        }
    }

    /**
     * @brief Résout une équation quartique : ax^4 + bx^3 + cx^2 + dx + e = 0.
     * @param coeffs Structure contenant les coefficients a, b, c, d et e.
     * @param roots Tableau pour stocker les racines réelles trouvées.
     * @return Nombre de racines réelles trouvées (0 à 4).
     */
    static inline int solveQuartic(const QuarticCoeffs& coeffs, double roots[4]) {
        if (std::abs(coeffs.a) < 1e-9)
            return solveCubic({coeffs.b, coeffs.c, coeffs.d, coeffs.e}, roots);

        double A = coeffs.b / coeffs.a;
        double B = coeffs.c / coeffs.a;
        double C = coeffs.d / coeffs.a;
        double D = coeffs.e / coeffs.a;

        double cub_roots[3];
        solveCubic({1.0, -B, (A * C - 4.0 * D), (4.0 * B * D - A * A * D - C * C)}, cub_roots);
        double y = cub_roots[0];

        double p_sq = A * A / 4.0 - B + y;
        if (p_sq < 0) {
            if (p_sq > -1e-7)
                p_sq = 0;
            else
                return 0;
        }

        double p = std::sqrt(p_sq);
        double q;
        if (p > 1e-9) {
            q = (A * y / 2.0 - C) / (2.0 * p);
        } else {
            double q_sq = y * y / 4.0 - D;
            if (q_sq < 0) {
                if (q_sq > -1e-7)
                    q_sq = 0;
                else
                    return 0;
            }
            q = std::sqrt(q_sq);
        }

        int count = solveQuadratic({1.0, A / 2.0 + p, y / 2.0 + q}, roots);
        count += solveQuadratic({1.0, A / 2.0 - p, y / 2.0 - q}, roots + count);

        return count;
    }
};

} // namespace Raytracer
