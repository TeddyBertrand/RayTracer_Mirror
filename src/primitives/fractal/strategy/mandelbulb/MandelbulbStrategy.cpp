#include "MandelbulbStrategy.hpp"

namespace Raytracer {

FractalResult MandelbulbStrategy::getInfo(const Vector3D& p, int maxIter) const {
    Vector3D z = p;
    double dr = 1.0;
    double r = 0.0;
    double power = 8.0;

    int iteration = 0;
    for (; iteration < maxIter; ++iteration) {
        r = z.length();
        if (r > 2.0)
            break;

        double theta = std::acos(z.y / std::max(r, 1e-12));
        double phi = std::atan2(z.z, z.x);
        dr = std::pow(r, power - 1.0) * power * dr + 1.0;

        double zr = std::pow(r, power);
        z = Vector3D(std::sin(theta * power) * std::cos(phi * power),
                     std::cos(theta * power),
                     std::sin(theta * power) * std::sin(phi * power)) *
                zr +
            p;
    }

    double distance = 0.5 * std::log(r) * r / dr;

    const double iteration_ratio =
        (maxIter > 1) ? (static_cast<double>(iteration) / static_cast<double>(maxIter - 1)) : 0.0;
    return {distance, iteration_ratio};
}

} // namespace Raytracer
