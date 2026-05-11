#pragma once

#include "components/ITexture.hpp"
#include <cmath>

namespace Raytracer {

class CheckerTexture : public ITexture {
public:
    CheckerTexture(double scale, Color a, Color b) : _scale(scale), _colorA(a), _colorB(b) {}

    Color value(double u, double v, [[maybe_unused]] const Vector3D& p) const noexcept override {
        int u_integer = static_cast<int>(std::floor(u * _scale));
        int v_integer = static_cast<int>(std::floor(v * _scale));

        if ((u_integer + v_integer) % 2 == 0) {
            return _colorA;
        }
        return _colorB;
    }

private:
    double _scale;
    Color _colorA;
    Color _colorB;
};

} // namespace Raytracer
