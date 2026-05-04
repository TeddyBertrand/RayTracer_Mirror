#pragma once

#include "math/Vector3D.hpp"
#include <vector>

namespace Raytracer {

struct MeshData {
    struct UV {
        double u, v;
    };

    std::vector<Point3D> vertices;
    std::vector<Vector3D> normals;
    std::vector<UV> texCoords;

    struct TriangleIndices {
        int v[3];
        int n[3];
        int t[3];
    };

    std::vector<TriangleIndices> triangles;
};

} // namespace Raytracer
