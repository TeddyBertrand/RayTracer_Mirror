#pragma once

#include "components/ITexture.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <random>
#include <vector>

namespace Raytracer {

class PerlinTexture : public ITexture {
public:
    PerlinTexture(double scale, Color a, Color b) : _scale(scale), _colorA(a), _colorB(b) {
        // Initialize permutation table with values 0..255 and duplicate them
        std::array<int, 256> p{};
        std::iota(p.begin(), p.end(), 0);
        // deterministic shuffle for reproducible renders
        std::mt19937 gen(20260506);
        std::shuffle(p.begin(), p.end(), gen);

        for (int i = 0; i < 256; ++i) {
            _permutation[i] = p[i];
            _permutation[i + 256] = p[i];
        }
    }

    Color value([[maybe_unused]] double u,
                [[maybe_unused]] double v,
                const Vector3D& p) const noexcept override {
        Vector3D point = p * _scale;

        double noiseValue = turbulence(point, 7);

        return _colorA * (1.0 - noiseValue) + _colorB * noiseValue;
    }

private:
    double _scale;
    Color _colorA;
    Color _colorB;
    std::array<int, 512> _permutation;

private:
    double noise(const Vector3D& v) const {
        Vector3D i(std::floor(v.x), std::floor(v.y), std::floor(v.z));
        Vector3D f(v.x - i.x, v.y - i.y, v.z - i.z);

        f.x = f.x * f.x * (3.0 - 2.0 * f.x);
        f.y = f.y * f.y * (3.0 - 2.0 * f.y);
        f.z = f.z * f.z * (3.0 - 2.0 * f.z);

        return lerp3D(i, f);
    }

    double lerp3D(const Vector3D& i, const Vector3D& f) const {
        double x0 = 1.0 - f.x, x1 = f.x;
        double y0 = 1.0 - f.y, y1 = f.y;
        double z0 = 1.0 - f.z, z1 = f.z;

        return hash(i + Vector3D(0, 0, 0)) * x0 * y0 * z0 +
               hash(i + Vector3D(1, 0, 0)) * x1 * y0 * z0 +
               hash(i + Vector3D(0, 1, 0)) * x0 * y1 * z0 +
               hash(i + Vector3D(1, 1, 0)) * x1 * y1 * z0 +
               hash(i + Vector3D(0, 0, 1)) * x0 * y0 * z1 +
               hash(i + Vector3D(1, 0, 1)) * x1 * y0 * z1 +
               hash(i + Vector3D(0, 1, 1)) * x0 * y1 * z1 +
               hash(i + Vector3D(1, 1, 1)) * x1 * y1 * z1;
    }

    double hash(const Vector3D& v) const {
        int X = (int)std::floor(v.x) & 255;
        int Y = (int)std::floor(v.y) & 255;
        int Z = (int)std::floor(v.z) & 255;

        int h = _permutation[_permutation[_permutation[X] + Y] + Z];

        return static_cast<double>(h) / 255.0;
    }

    double turbulence(const Vector3D& p, int octaves) const {
        double accum = 0.0;
        Vector3D temp_p = p;
        double weight = 0.5;

        for (int i = 0; i < octaves; i++) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p = temp_p * 2.0;
        }
        return std::abs(accum);
    }
};

} // namespace Raytracer
