#include "MandelbulbStrategy.hpp"

namespace Raytracer {

FractalResult MandelbulbStrategy::getInfo(const Vector3D& p, int maxIter) const {
    Vector3D z = p;
    double dr = 1.0;
    double r = 0.0;
    double power = 8.0;
    
    double orbitTrap = 1e10; 

    for (int i = 0; i < maxIter; i++) {
        r = z.length();
        if (r > 2.0) break;

        orbitTrap = std::min(orbitTrap, std::abs(z.z)); 

        double theta = std::acos(z.y / std::max(r, 1e-12));
        double phi = std::atan2(z.z, z.x);
        dr = std::pow(r, power - 1.0) * power * dr + 1.0;

        double zr = std::pow(r, power);
        z = Vector3D(std::sin(theta * power) * std::cos(phi * power), 
                     std::cos(theta * power), 
                     std::sin(theta * power) * std::sin(phi * power)) * zr + p;
    }

    double distance = 0.5 * std::log(r) * r / dr;
    
    return {distance, orbitTrap}; 
}

} // namespace Raytracer
