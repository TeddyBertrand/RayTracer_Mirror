#pragma once

#include "primitives/mesh/MeshData.hpp"
#include <string>

namespace Raytracer {

class IMeshParser {
public:
    virtual ~IMeshParser() = default;

    virtual MeshData parse(const std::string& filename) = 0;

    virtual bool canParse(const std::string& filename) const = 0;
};

} // namespace Raytracer
