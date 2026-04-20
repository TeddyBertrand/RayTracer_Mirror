cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# On force le standard C++17 ou 20 (indispensable pour un RayTracer moderne)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On cherche tous tes fichiers .cpp dans le dossier src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# On crée l'exécutable
add_executable(RayTracer ${SOURCES})