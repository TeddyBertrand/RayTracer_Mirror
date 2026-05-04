#pragma once

#include "components/IPrimitive.hpp"

namespace Raytracer {

class Triangle : public IPrimitive {
public:
    Triangle(const Point3D& v0,
             const Point3D& v1,
             const Point3D& v2,
             std::shared_ptr<IMaterial> material);

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;

    AABB getBoundingBox() const override;

    void setMaterial(std::shared_ptr<IMaterial> m) override { _material = m; }

    void setExtraData(const Vector3D n[3], const double u_coords[3], const double v_coords[3]);

private:
    static constexpr float bounding_box_epsilon = 0.001f;

private:
    Point3D _v0, _v1, _v2;
    std::shared_ptr<IMaterial> _material;

    Vector3D _edge1;
    Vector3D _edge2;

    bool _has_extra;
    Vector3D _normals[3];
    double _u[3], _v[3];
};

} // namespace Raytracer
