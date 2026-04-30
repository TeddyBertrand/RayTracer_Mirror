#pragma once

#include <algorithm>

#include "math/Interval.hpp"
#include "math/Ray.hpp"
#include "math/Vector3D.hpp"

namespace Raytracer {

struct AABB {
    enum class Kind {
        Empty,
        Finite,
        Infinite,
    };

    Point3D min, max;
    Kind kind = Kind::Empty;

    constexpr AABB() noexcept = default;

    constexpr AABB(Point3D minPoint, Point3D maxPoint) noexcept
        : min(minPoint), max(maxPoint), kind(Kind::Finite) {}

    static constexpr AABB infinite() noexcept {
        AABB box;
        box.kind = Kind::Infinite;
        return box;
    }

    [[nodiscard]] constexpr bool isEmpty() const noexcept { return kind == Kind::Empty; }
    [[nodiscard]] constexpr bool isFinite() const noexcept { return kind == Kind::Finite; }
    [[nodiscard]] constexpr bool isInfinite() const noexcept { return kind == Kind::Infinite; }

    bool hit(const Ray& r, Interval ray_t) const {
        if (!isFinite()) {
            return false;
        }

        const auto& origin = r.origin();
        const auto& direction = r.direction();

        // X axis
        {
            const double invD = 1.0 / direction.x;
            double t0 = (min.x - origin.x) * invD;
            double t1 = (max.x - origin.x) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            ray_t.min = std::max(ray_t.min, t0);
            ray_t.max = std::min(ray_t.max, t1);
            if (ray_t.max <= ray_t.min)
                return false;
        }

        // Y axis
        {
            const double invD = 1.0 / direction.y;
            double t0 = (min.y - origin.y) * invD;
            double t1 = (max.y - origin.y) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            ray_t.min = std::max(ray_t.min, t0);
            ray_t.max = std::min(ray_t.max, t1);
            if (ray_t.max <= ray_t.min)
                return false;
        }

        // Z axis
        {
            const double invD = 1.0 / direction.z;
            double t0 = (min.z - origin.z) * invD;
            double t1 = (max.z - origin.z) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            ray_t.min = std::max(ray_t.min, t0);
            ray_t.max = std::min(ray_t.max, t1);
            if (ray_t.max <= ray_t.min)
                return false;
        }

        return true;
    }

    static AABB combine(const AABB& box0, const AABB& box1) {
        if (box0.isInfinite() || box1.isInfinite()) {
            return AABB::infinite();
        }

        if (box0.isEmpty())
            return box1;
        if (box1.isEmpty())
            return box0;

        Point3D small(std::min(box0.min.x, box1.min.x),
                      std::min(box0.min.y, box1.min.y),
                      std::min(box0.min.z, box1.min.z));

        Point3D big(std::max(box0.max.x, box1.max.x),
                    std::max(box0.max.y, box1.max.y),
                    std::max(box0.max.z, box1.max.z));

        return AABB{small, big};
    }
};

} // namespace Raytracer
