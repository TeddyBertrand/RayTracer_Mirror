#pragma once

#include "components/IPrimitive.hpp"
#include "core/scene/bvh/BVHNode.hpp"
#include "parser/ObjParser.hpp"
#include "primitives/triangle/Triangle.hpp"

namespace Raytracer {

class Mesh : public IPrimitive {
public:
    Mesh(const std::string& filepath);

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
    virtual AABB getBoundingBox() const override;

private:
    std::shared_ptr<IPrimitive> createTriangle(const MeshData::TriangleIndices& triIdx) const;

private:
    std::shared_ptr<IPrimitive> _bvh_root;
    MeshData _data;
};

} // namespace Raytracer
