cmake_minimum_required(VERSION 3.16)
project(MultiGame)

set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source
file(GLOB_RECURSE SOURCES "Source/*.cpp")

add_executable(MultiGame ${SOURCES})

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Lier SFML
target_link_libraries(MultiGame PRIVATE sfml-graphics sfml-window sfml-system)