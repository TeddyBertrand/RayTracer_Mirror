#pragma once

#include "libraries/dlfcn.h"
#include "parser/dlloader/IDLLoader.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Raytracer {

class DLLoader : public virtual IDLLoader {
public:
    DLLoader() = default;
    ~DLLoader() { closeLibrary(); }

    void openLibrary(const std::string& libraryPath) override {
        if (_libMap.find(libraryPath) != _libMap.end())
            return;

        void* handle = sys_open(libraryPath);
        if (!handle)
            throw std::runtime_error("Failed to load: " + libraryPath);

        _libMap[libraryPath] = handle;
    }

    template <typename T>
    T getFunction(const std::string& funcName, const std::string& libName) const {
        auto it = _libMap.find(libName);
        if (it == _libMap.end())
            return nullptr;

        return reinterpret_cast<T>(sys_getSym(it->second, funcName));
    }

    void closeLibrary() override {
        for (auto& [path, handle] : _libMap) {
            if (handle)
                sys_close(handle);
        }
        _libMap.clear();
    }

private:
    // --- COUCHE D'ABSTRACTION PRIVÉE (L'isolation est ici) ---

    void* sys_open(const std::string& path) {
#ifdef _WIN32
        return static_cast<void*>(LoadLibraryA(path.c_str()));
#else
        return dlopen(path.c_str(), RTLD_LAZY);
#endif
    }

    void* sys_getSym(void* handle, const std::string& name) const {
#ifdef _WIN32
        return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), name.c_str()));
#else
        return dlsym(handle, name.c_str());
#endif
    }

    void sys_close(void* handle) {
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(handle));
#else
        dlclose(handle);
#endif
    }

private:
    std::unordered_map<std::string, void*> _libMap;
};

} // namespace Raytracer
