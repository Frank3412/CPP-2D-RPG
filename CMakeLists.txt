cmake_minimum_required(VERSION 4.2)

project(PracticeCode)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

add_executable(PracticeCode
        src/main.cpp
        src/Player.cpp
        src/TileMap.cpp
        src/Camera.cpp
        src/AssetManager.cpp
        src/Game.cpp
        src/InteractableObject.cpp
        src/Sign.cpp
        src/DialogueManager.cpp
        src/NPC.cpp
        src/TiledLevel.cpp
        src/Enemy.cpp
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
        SDL3_ttf::SDL3_ttf
        nlohmann_json::nlohmann_json
)
