cmake_minimum_required(VERSION 3.16)
project(MultiGame)

set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source
file(GLOB_RECURSE SOURCES "Source/*.cpp")

add_executable(MultiGame ${SOURCES})

# Trouver SFML
target_link_libraries(MultiGame
    PRIVATE
        /opt/homebrew/lib/libsfml-graphics.dylib
        /opt/homebrew/lib/libsfml-window.dylib
        /opt/homebrew/lib/libsfml-system.dylib
)
include_directories(/opt/homebrew/include)