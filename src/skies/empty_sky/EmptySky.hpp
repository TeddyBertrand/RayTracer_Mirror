#pragma once

#include "skies/commun/ASky.hpp"

namespace Raytracer {

class EmptySky : public ASky {
public:
    EmptySky() = default;

    Color getBackgroundColor([[maybe_unused]] const Ray& r) const override {
        return Color(0, 0, 0);
    }

    std::string getName() const override { return "empty_sky"; }
};

} // namespace Raytracer
