#pragma once
#include <string>

namespace Raytracer
{
    class IDLLoader
    {
        public:
            /**
             * @brief Virtual destructor
             */
            virtual ~IDLLoader() = default;

            /**
             * @brief Open and load a library
             * 
             * @param libraryPath 
             */
            virtual void openLibrary(const std::string &libraryPath) = 0;

            /**
             * @brief Close a loaded library
             */
            virtual void closeLibrary() = 0;
    };
}
