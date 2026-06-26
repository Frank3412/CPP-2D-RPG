cmake_minimum_required(VERSION 4.2)

project(PracticeCode)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(PracticeCode main.cpp)

target_link_libraries(
        PracticeCode
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)
