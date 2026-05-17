add_library(raytracer_core STATIC
    Raytracer.cpp
    plugin_loader/PluginLoader.cpp
    scene/Scene.cpp
    scene/PrimitiveList.cpp
    scene/bvh/BVHNode.cpp
    image/Image.cpp
    parser/SceneParser.cpp
    display/loading_bar/LoadingBar.cpp
    logging/Logger.cpp
    file_watcher/FileWatcher.cpp
    network/Cluster.cpp
)

target_include_directories(raytracer_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${LIBCONFIGPP_INCLUDE_DIRS}
)

target_link_libraries(raytracer_core PUBLIC ${LIBCONFIGPP_LIBRARIES} sfml-network sfml-system)

target_compile_options(raytracer_core PRIVATE
    -W
    -Wall
    -Wextra
)
