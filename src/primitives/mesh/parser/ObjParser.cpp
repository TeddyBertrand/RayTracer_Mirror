#include "ObjParser.hpp"

#include <algorithm>
#include <functional>
#include <unordered_map>

namespace Raytracer {

MeshData ObjParser::parse(const std::string& filename) {
    static const std::unordered_map<std::string,
                                    std::function<void(std::istringstream&, MeshData&)>>
        handlers = {
            {"v", [this](std::istringstream& iss, MeshData& data) { processVertex(iss, data); }},
            {"vn", [this](std::istringstream& iss, MeshData& data) { processNormal(iss, data); }},
            {"vt", [this](std::istringstream& iss, MeshData& data) { processTexture(iss, data); }},
            {"f", [this](std::istringstream& iss, MeshData& data) { processFace(iss, data); }}};

    MeshData data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("Failed to open OBJ file: " + filename);

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        auto it = handlers.find(prefix);
        if (it != handlers.end())
            it->second(iss, data);
    }
    return data;
}

int ObjParser::fixIndex(int idx, size_t currentArraySize) {
    if (idx > 0)
        return idx - 1;
    if (idx < 0)
        return currentArraySize + idx;
    return -1;
}

void ObjParser::processFace(std::istringstream& iss, MeshData& data) {
    std::vector<FaceElement> vbt;
    std::string token;

    while (iss >> token) {
        vbt.push_back(parseFaceElement(token));
    }

    for (size_t i = 1; i < vbt.size() - 1; ++i) {
        MeshData::TriangleIndices tri;

        int indices[3] = {0, static_cast<int>(i), static_cast<int>(i) + 1};

        for (int j = 0; j < 3; ++j) {
            int idx = indices[j];
            tri.v[j] = fixIndex(vbt[idx].v, data.vertices.size());
            tri.n[j] = fixIndex(vbt[idx].n, data.normals.size());
            tri.t[j] = fixIndex(vbt[idx].t, data.texCoords.size());
        }
        data.triangles.push_back(tri);
    }
}

void ObjParser::processVertex(std::istringstream& line, MeshData& data) {
    double x, y, z;
    if (line >> x >> y >> z) {
        data.vertices.emplace_back(x, y, z);
    }
}

void ObjParser::processNormal(std::istringstream& line, MeshData& data) {
    double x, y, z;
    if (line >> x >> y >> z) {
        data.normals.emplace_back(x, y, z);
    }
}

void ObjParser::processTexture(std::istringstream& line, MeshData& data) {
    double u, v;
    if (line >> u >> v) {
        data.texCoords.push_back({u, v});
    }
}

ObjParser::FaceElement ObjParser::parseFaceElement(const std::string& token) {
    FaceElement fe = {-1, -1, -1};
    
    size_t firstSlash = token.find('/');
    
    if (firstSlash == std::string::npos) {
        fe.v = std::stoi(token);
        return fe;
    }
    
    fe.v = std::stoi(token.substr(0, firstSlash));
    
    size_t secondSlash = token.find('/', firstSlash + 1);
    
    if (secondSlash == std::string::npos) {
        std::string tPart = token.substr(firstSlash + 1);
        if (!tPart.empty()) fe.t = std::stoi(tPart);
        return fe;
    }
    
    std::string tPart = token.substr(firstSlash + 1, secondSlash - (firstSlash + 1));
    if (!tPart.empty()) fe.t = std::stoi(tPart);
    
    std::string nPart = token.substr(secondSlash + 1);
    if (!nPart.empty()) fe.n = std::stoi(nPart);
    
    return fe;
}

} // namespace Raytracer
