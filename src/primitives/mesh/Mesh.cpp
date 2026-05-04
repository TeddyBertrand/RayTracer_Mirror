#include "Mesh.hpp"
#include "builder/EntityBuilder.hpp"
#include "math/MathUtils.hpp"
#include "parser/PrimitiveSettings.hpp"

namespace Raytracer {

extern "C" {

const char* getName() { return "mesh"; }

IPrimitive* createPlugin(const ISetting& settings) {
    std::string path = settings.getString("file");

    auto mesh = std::make_shared<Mesh>(path);

    auto entity = EntityBuilder(settings)
                      .setPrimitive(mesh)
                      .parseMaterial(settings)
                      .parseTransform(settings)
                      .build();

    return entity.release();
}
}

Mesh::Mesh(const std::string& filepath) {
    ObjParser parser;
    _data = parser.parse(filepath);

    if (_data.triangles.empty()) {
        throw std::runtime_error("Mesh contains no triangles: " + filepath);
    }

    std::vector<std::shared_ptr<IPrimitive>> triangles;
    triangles.reserve(_data.triangles.size());

    for (const auto& triIdx : _data.triangles) {
        triangles.push_back(createTriangle(triIdx));
    }

    _bvh_root = std::make_shared<BVHNode>(triangles, 0, triangles.size());
}

std::shared_ptr<IPrimitive> Mesh::createTriangle(const MeshData::TriangleIndices& triIdx) const {
    const Point3D& v0 = _data.vertices[triIdx.v[0]];
    const Point3D& v1 = _data.vertices[triIdx.v[1]];
    const Point3D& v2 = _data.vertices[triIdx.v[2]];

    auto triangle = std::make_shared<Triangle>(v0, v1, v2, nullptr);
    if (triIdx.n[0] != -1 || triIdx.t[0] != -1) {
        Vector3D ns[3];
        double us[3], vs[3];

        for (int i = 0; i < 3; i++) {
            ns[i] = (triIdx.n[i] != -1) ? _data.normals[triIdx.n[i]] : Vector3D(0, 0, 1);
            us[i] = (triIdx.t[i] != -1) ? _data.texCoords[triIdx.t[i]].u : 0.0;
            vs[i] = (triIdx.t[i] != -1) ? _data.texCoords[triIdx.t[i]].v : 0.0;
        }
        triangle->setExtraData(ns, us, vs);
    }
    return triangle;
}

bool Mesh::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
    return _bvh_root->hit(r, ray_t, rec);
}

AABB Mesh::getBoundingBox() const { return _bvh_root->getBoundingBox(); }

} // namespace Raytracer
