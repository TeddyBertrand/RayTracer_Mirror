include(FetchContent)

set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "" FORCE)
set(RAYTRACER_BUILD_SHARED_LIBS_BACKUP ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

find_package(SFML 3 QUIET COMPONENTS Graphics System)
if(NOT SFML_FOUND)
    message(STATUS "SFML not found, fetching it now")
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
    )
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_deps/lib)
    FetchContent_MakeAvailable(sfml)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP})
endif()

set(BUILD_SHARED_LIBS ${RAYTRACER_BUILD_SHARED_LIBS_BACKUP})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/materials)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugins/materials)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins/materials)

add_subdirectory(commun)
add_subdirectory(flat_material)
add_subdirectory(light_material)
add_subdirectory(metal_material)
