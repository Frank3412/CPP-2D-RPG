cmake_minimum_required(VERSION 4.2)

project(PracticeCode)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(PracticeCode
        src/main.cpp
        src/Player.cpp
        src/TileMap.cpp
        src/Camera.cpp
        src/AssetManager.cpp
)

target_include_directories(
        PracticeCode
        PRIVATE
        include
)

target_link_libraries(
        PracticeCode
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)
