#pragma once

#include <unordered_map>
#include <dlfcn.h>
#include "parser/dlloader/IDLLoader.hpp"

namespace Raytracer {

    class DLLoader : public virtual IDLLoader
    {
        public:
            /**
             * @brief Exception class for DLLoader
             * 
             * if there is an error during the loading/ deletion of the library
             * it will be thrown with a DLLoaderException
             */
            class DLLoaderException : public std::exception
            {
                private:
                    std::string _message;
                public:
                    DLLoaderException(std::string message) : _message(message) {};
                    const char* what() const noexcept override { return this->_message.c_str(); };
            };

            /**
             * @brief Construct a new DLLoader<T> object
             * 
             * try to load a .so library and store it in the library map
             * @param libraryPath 
             */
            DLLoader(const std::string &libraryPath)
            {
                _libMap.clear();
                openLibrary(libraryPath);
            };

            /**
             * @brief Open and load a library
             * 
             * dlopen a new library with the given path, add it to the library map
             * and throw an exception if there is one
             * @param libraryPath 
             */
            void openLibrary(const std::string &libraryPath) override
            {
                dlerror();
                void *newHandle = dlopen(libraryPath.c_str(), RTLD_LAZY);
                const char *error = dlerror();
                if (error)
                    throw DLLoaderException(std::string("On dlopen of ") + libraryPath + ": " + error);
                _libMap.insert({libraryPath, newHandle});
            }
            
            /**
             * @brief Get the instance function
             * 
             * get the library function pointer from the function register with dlsym
             * 
             * @param entryPoint 
             * @return registerPlugin 
             */
            template<typename T>
            T getFunction(const std::string &registerFunction, const std::string &libraryName) const
            {
                auto handle = _libMap.find(libraryName);
                if (handle == _libMap.end())
                    throw DLLoaderException("Library not found in the map: " + libraryName);
                
                dlerror();
                void *entry = dlsym(handle->second, registerFunction.c_str());
                const char *error = dlerror();
                if (error)
                    throw DLLoaderException("On dlsym of " + _libPath + ": " + error);

                T fptr = reinterpret_cast<T>(entry);
                if (!fptr)
                    throw DLLoaderException("On function recuperation from " + _libPath);
                return fptr;
            };
            
            /**
             * @brief Close library
             * 
             * check if lib is initialized, dclose all library and clear map
             */
            void closeLibrary() override
            {
                for (auto lib : _libMap) {
                    if (lib.second) {
                        dlclose(lib.second);
                        lib.second = nullptr;
                    }
                }
                _libMap.clear();
            }

            /**
             * @brief Destroy the DLLoader object
             * 
             * close every library
             */
            ~DLLoader() {
                closeLibrary();
            }

        private:
            std::unordered_map<std::string, void *> _libMap;
    };
};
