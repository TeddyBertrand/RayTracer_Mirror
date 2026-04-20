cmake_minimum_required(VERSION 3.20)

project(
    raytracer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins)

set(FETCHCONTENT_BASE_DIR ${PROJECT_BINARY_DIR}/_deps)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/primitives)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/materials)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/lights)

add_compile_options(-fno-gnu-unique)

add_subdirectory(src)

if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
    enable_testing()
    add_subdirectory(tests)
endif()
