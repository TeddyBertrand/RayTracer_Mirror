cmake_minimum_required(VERSION 3.20)

project(
    raytracer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(BUILD_TESTING "Build the testing suite" ON)
option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

add_compile_options(-fno-gnu-unique -Wall -Wextra)

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

if(BUILD_TESTING)
    if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
        enable_testing()
        add_subdirectory(tests)
        message(STATUS "[Config] Testing enabled and directory found.")
    else()
        message(WARNING "[Config] Testing enabled but 'tests/CMakeLists.txt' not found. Skipping.")
    endif()
else()
    message(STATUS "[Config] Testing disabled by user (-DBUILD_TESTING=OFF).")
endif()