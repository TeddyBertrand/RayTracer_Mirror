cmake_minimum_required(VERSION 3.20)

project(
    raytracer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(TEST "Build the testing suite" OFF)
option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)
option(ENABLE_ULTRA_OPTIMIZATION "Enable aggressive performance flags" ON)

add_compile_options(-fno-gnu-unique -Wall -Wextra)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O0 -g --coverage)
        add_link_options(--coverage)
    endif()
endif()

if(ENABLE_ULTRA_OPTIMIZATION AND NOT ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

        add_compile_options(
            -O3
            -DNDEBUG
            -march=native
            -mtune=native
            -flto
            -ffast-math
            -funroll-loops
            -fomit-frame-pointer
            -fno-math-errno
            -fno-trapping-math
            -fno-signed-zeros
        )
        add_link_options(-flto)

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            add_compile_options(-fno-signaling-nans -fno-plt)
        endif()

        if(ipo_supported)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
        else()
            message(WARNING "[Config] IPO/LTO not supported: ${ipo_error}")
        endif()
    endif()
elseif(ENABLE_COVERAGE)
    message(STATUS "[Config] Ultra optimization disabled automatically because coverage is enabled.")
else()
    message(STATUS "[Config] Ultra optimization disabled (-DENABLE_ULTRA_OPTIMIZATION=OFF).")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins)

set(FETCHCONTENT_BASE_DIR ${PROJECT_BINARY_DIR}/_deps)

# Dossiers de sortie pour les plugins
set(PLUGIN_DIR ${CMAKE_BINARY_DIR}/plugins)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/primitives)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/materials)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/lights)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/skies)
file(MAKE_DIRECTORY ${PLUGIN_DIR}/cameras)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIGPP REQUIRED libconfig++)

add_subdirectory(src)

set(LIBCONFIGPP_LIBRARIES ${LIBCONFIGPP_LIBRARIES} CACHE INTERNAL "")
set(LIBCONFIGPP_INCLUDE_DIRS ${LIBCONFIGPP_INCLUDE_DIRS} CACHE INTERNAL "")

if(TEST)
    if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
        enable_testing()
        add_subdirectory(tests)
        message(STATUS "[Config] Testing enabled (-DTEST=ON).")
    else()
        message(WARNING "[Config] Testing enabled but 'tests/CMakeLists.txt' not found.")
    endif()
else()
    message(STATUS "[Config] Testing disabled (Default). Use -DTEST=ON to enable.")
endif()