#pragma once

#include "parser/dlloader/IDLLoader.hpp"
#include <dlfcn.h>
#include <unordered_map>

namespace Raytracer {

/**
 * @brief POSIX dynamic library loader based on `dlopen`/`dlsym`.
 *
 * This class owns loaded shared library handles and provides symbol lookup
 * helpers used by plugin registration systems.
 */
class DLLoader : public virtual IDLLoader {
public:
    /**
     * @brief Exception raised when dynamic loading operations fail.
     */
    class DLLoaderException : public std::exception {
    private:
        std::string _message;

    public:
        /**
         * @brief Build an exception with a descriptive error message.
         */
        DLLoaderException(std::string message) : _message(message) {};

        /**
         * @brief Return the stored error message.
         */
        const char* what() const noexcept override { return this->_message.c_str(); };
    };

    /**
     * @brief Construct a loader and immediately open one library.
     * @param libraryPath Path to the first shared object to load.
     */
    DLLoader(const std::string& libraryPath) {
        _libMap.clear();
        openLibrary(libraryPath);
    };

    DLLoader() = default;

    /**
     * @brief Open and register a shared library handle.
     * @param libraryPath Path to the target shared object.
     * @throw DLLoaderException If `dlopen` fails.
     */
    void openLibrary(const std::string& libraryPath) override {
        dlerror();
        void* newHandle = dlopen(libraryPath.c_str(), RTLD_LAZY);
        const char* error = dlerror();
        if (error)
            throw DLLoaderException(std::string("On dlopen of ") + libraryPath + ": " + error);
        _libMap.insert({libraryPath, newHandle});
    }

    /**
     * @brief Resolve and cast a symbol from a previously loaded library.
     * @tparam T Callable/function pointer type expected for the symbol.
     * @param registerFunction Symbol name to query with `dlsym`.
     * @param libraryName Key used when the library was opened.
     * @return Symbol cast to `T`.
     * @throw DLLoaderException If the library or symbol is not found.
     */
    template <typename T>
    T getFunction(const std::string& registerFunction, const std::string& libraryName) const {
        auto handle = _libMap.find(libraryName);
        if (handle == _libMap.end())
            throw DLLoaderException("Library not found in the map: " + libraryName);

        dlerror();
        void* entry = dlsym(handle->second, registerFunction.c_str());
        const char* error = dlerror();
        if (error)
            throw DLLoaderException("On dlsym of " + libraryName + ": " + error);

        T fptr = reinterpret_cast<T>(entry);
        if (!fptr)
            throw DLLoaderException("On function recuperation from " + libraryName);
        return fptr;
    };

    /**
     * @brief Close all loaded libraries and clear internal handle storage.
     */
    void closeLibrary() override {
        for (auto lib : _libMap) {
            if (lib.second) {
                dlclose(lib.second);
                lib.second = nullptr;
            }
        }
        _libMap.clear();
    }

    /**
     * @brief Destroy loader and release every loaded library.
     */
    ~DLLoader() { closeLibrary(); }

private:
    std::unordered_map<std::string, void*> _libMap;
};
}; // namespace Raytracer
