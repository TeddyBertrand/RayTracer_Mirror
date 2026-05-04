#pragma once

#include "IMeshParser.hpp"
#include <string>
#include <fstream>
#include <sstream>

namespace Raytracer {

class ObjParser : public IMeshParser {
public:
    ObjParser() = default;
    ~ObjParser() override = default;

    MeshData parse(const std::string& filename) override;

    bool canParse(const std::string& filename) const override {
        return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".obj";
    }

private:
    void processVertex(std::istringstream& line, MeshData& data);
    
    void processNormal(std::istringstream& line, MeshData& data);
    
    void processTexture(std::istringstream& line, MeshData& data);
    
    void processFace(std::istringstream& line, MeshData& data);

    struct FaceElement { int v = -1, t = -1, n = -1; };
    FaceElement parseFaceElement(const std::string& token);
    
    int fixIndex(int idx, size_t currentArraySize);
};

} // namespace Raytracer
