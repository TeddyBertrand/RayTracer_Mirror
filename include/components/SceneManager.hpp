#pragma once

#include <memory>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "components/ILight.hpp"
#include "components/IMaterial.hpp"
#include "components/PrimitiveGroup.hpp"
#include "math/Matrix.hpp"

namespace Raytracer {

class SceneManager {
public:
    SceneManager() { _transformStack.push(Matrix()); }

    [[nodiscard]] bool isCached(const std::string& path) const {
        return _cache.find(path) != _cache.end();
    }
    [[nodiscard]] std::shared_ptr<PrimitiveGroup> getFromCache(const std::string& path) const {
        return _cache.at(path);
    }

    void addToCache(const std::string& path, std::shared_ptr<PrimitiveGroup> group) {
        if (group)
            _cache[path] = std::move(group);
    }

    /**
     * @brief Pousse une nouvelle transformation sur la pile en la multipliant par la précédente.
     */
    void pushTransformation(const Matrix& m) { _transformStack.push(_transformStack.top() * m); }

    /**
     * @brief Retire la dernière transformation.
     */
    void popTransformation() {
        if (_transformStack.size() > 1) {
            _transformStack.pop();
        }
    }

    /**
     * @brief Retourne la transformation cumulative actuelle.
     */
    [[nodiscard]] Matrix getCurrentTransformation() const { return _transformStack.top(); }

    void pushNamespace(const std::string& name) { _namespaceStack.push_back(name); }
    void popNamespace() {
        if (!_namespaceStack.empty())
            _namespaceStack.pop_back();
    }

    [[nodiscard]] std::string getFullNamespace() const {
        std::string prefix;
        for (const auto& ns : _namespaceStack)
            prefix += ns + "::";
        return prefix;
    }

    void registerMaterial(const std::string& localName, std::shared_ptr<IMaterial> mat) {
        if (mat)
            _materials[getFullNamespace() + localName] = mat;
    }

    /**
     * @brief Helper pour résoudre les matériaux dans le contexte actuel.
     */
    [[nodiscard]] std::unordered_map<std::string, std::shared_ptr<IMaterial>>
    getContextualMaterials(
        const std::unordered_map<std::string, std::shared_ptr<IMaterial>>& sceneMaterials) const {
        std::unordered_map<std::string, std::shared_ptr<IMaterial>> context;

        context = sceneMaterials;

        for (const auto& [name, mat] : _materials) {
            context[name] = mat;

            std::string prefix = getFullNamespace();
            if (!prefix.empty() && name.find(prefix) == 0) {
                std::string localName = name.substr(prefix.length());
                context[localName] = mat;
            }
        }
        return context;
    }

    [[nodiscard]] std::shared_ptr<IMaterial> getMaterial(const std::string& name) const {
        auto it = _materials.find(name);
        if (it != _materials.end())
            return it->second;
        it = _materials.find(getFullNamespace() + name);
        if (it != _materials.end())
            return it->second;
        return nullptr;
    }

    void trackFile(const std::string& path) {
        if (_activeImports.find(path) != _activeImports.end())
            throw std::runtime_error("SceneManager: Circular import detected: " + path);
        _activeImports.insert(path);
    }
    void untrackFile(const std::string& path) { _activeImports.erase(path); }

private:
    std::unordered_map<std::string, std::shared_ptr<PrimitiveGroup>> _cache;
    std::unordered_map<std::string, std::shared_ptr<IMaterial>> _materials;
    std::vector<std::string> _namespaceStack;
    std::set<std::string> _activeImports;

    std::stack<Matrix> _transformStack;
};

} // namespace Raytracer
