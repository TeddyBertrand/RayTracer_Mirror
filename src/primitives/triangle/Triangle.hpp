#pragma once

#include <memory>

#include "components/IMaterial.hpp"
#include <components/IPrimitive.hpp>
#include <math/Vector3D.hpp>

namespace Raytracer {

class Triangle : public virtual IPrimitive {
public:
    /**
     * @brief Construct a new Triangle object
     *
     * @param v0 First vertex
     * @param v1 Second vertex
     * @param v2 Third vertex
     */
    Triangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2)
        : _v0(v0), _v1(v1), _v2(v2), _material(nullptr) {}

    /**
     * @brief Hit function
     *
     * check if a ray between an interval t0 and t1 has hit the triangle or not
     * @param r
     * @param ray_t
     * @param rec
     * @return true
     * @return false
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    void setMaterial(std::shared_ptr<IMaterial> m) override { _material = m; }

    AABB getBoundingBox() const override {
        double minX = std::min({_v0.x, _v1.x, _v2.x});
        double minY = std::min({_v0.y, _v1.y, _v2.y});
        double minZ = std::min({_v0.z, _v1.z, _v2.z});

        double maxX = std::max({_v0.x, _v1.x, _v2.x});
        double maxY = std::max({_v0.y, _v1.y, _v2.y});
        double maxZ = std::max({_v0.z, _v1.z, _v2.z});

        Vector3D minPoint(minX - 1e-4, minY - 1e-4, minZ - 1e-4);
        Vector3D maxPoint(maxX + 1e-4, maxY + 1e-4, maxZ + 1e-4);

        return AABB{minPoint, maxPoint};
    }

private:
    Vector3D _v0;
    Vector3D _v1;
    Vector3D _v2;
    std::shared_ptr<IMaterial> _material;
};

} // namespace Raytracer
