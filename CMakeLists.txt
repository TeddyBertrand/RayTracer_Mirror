cmake_minimum_required(VERSION 3.20)

project(
    raytracer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options(-fno-gnu-unique -Wall -Wextra)

option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O0 -g --coverage)
        add_link_options(--coverage)
    endif()
endif()
set(PLUGIN_DIR ${CMAKE_BINARY_DIR}/plugins)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/primitives)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/materials)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/lights)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/skies)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/cameras)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIGPP REQUIRED libconfig++)

add_subdirectory(src)

if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
    enable_testing()
    add_subdirectory(tests)
endif()