cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(polygon-visualiser)

add_library(
    point
    src/Logic/Point.hpp
    src/Logic/Point.cpp
)
add_library(
    polygon
    src/Logic/Polygon.hpp
    src/Logic/Polygon.cpp
)
add_library(
    window
    src/Ui/Window.hpp
    src/Ui/Window.cpp
)


find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE SDL2)

target_link_libraries(window PRIVATE SDL2)

target_link_libraries(polygon PRIVATE point)

target_link_libraries(main PRIVATE polygon)
target_link_libraries(main PRIVATE window)

target_link_libraries(main PRIVATE Threads::Threads)
target_link_libraries(window PRIVATE Threads::Threads)
