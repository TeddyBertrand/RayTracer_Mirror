#pragma once

#include "components/ITexture.hpp"

namespace Raytracer {

class SolidColor : public ITexture {
public:
    explicit SolidColor(const Color& c) : _color(c) {}

    Color value([[maybe_unused]] double u,
                [[maybe_unused]] double v,
                [[maybe_unused]] const Vector3D& p) const noexcept override {
        return _color;
    }

private:
    Color _color;
};

} // namespace Raytracer
