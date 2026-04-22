#pragma once
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
             * try to load a .so library and stock it in the _handle variable
             * @param libraryPath 
             */
            DLLoader(const std::string &libraryPath) :_libPath(""), _handle(nullptr)
            {
                openLibrary(libraryPath);
            };

            /**
             * @brief Open and load a library
             * 
             * close the current library if there is one, dlopen the new library with 
             * the given path and throw an exception if there is one
             * @param libraryPath 
             */
            void openLibrary(const std::string &libraryPath) override
            {
                if (_handle) {
                    dlclose(_handle);
                    _handle = nullptr;
                }
                
                _libPath = libraryPath;
                dlerror();
                _handle = dlopen(_libPath.c_str(), RTLD_LAZY);
                const char *error = dlerror();
                if (error)
                throw DLLoaderException(std::string("On dlopen of ") + _libPath + ": " + error);
            }
            
            /**
             * @brief Get the instance function
             * 
             * get the function pointer of the library from the function entryPoint with dlsym
             * 
             * @param entryPoint 
             * @return registerPlugin 
             */
            template<typename T>
            T getFunction(const std::string &entryPoint) const
            {
                T fptr;
                dlerror();
                void *entry = dlsym(_handle, entryPoint.c_str());
                const char *error = dlerror();
                if (error)
                    throw DLLoaderException(std::string("On dlsym of ") + _libPath + ": " + error);
                fptr = reinterpret_cast<T>(entry);
                if (!fptr)
                    throw DLLoaderException("On function recuperation from " + _libPath);
                return fptr;
            };
            
            /**
             * @brief Close library
             * 
             * check if _handle is initialized and dclose the current library
             */
            void closeLibrary() override
            {
                if (_handle) {
                    dlclose(_handle);
                    _handle = nullptr;
                }
            }

            /**
             * @brief Destroy the DLLoader object
             * 
             * close the current library
             */
            ~DLLoader() {
                closeLibrary();
            }

        private:
            std::string _libPath;
            void *_handle;
    };
};
