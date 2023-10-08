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
    src/Ui/Button.hpp
    src/Ui/Button.cpp
)

find_package(Threads REQUIRED)

# Add ImGui source files to your project
# Add ImGui source files to your project with IMGUI_DISABLE_TABLES defined
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
add_library(
    imgui
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.h
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
)

# Add ImGui include directory
target_include_directories(imgui PRIVATE ${IMGUI_DIR})


add_executable(main src/main.cpp)

target_link_libraries(imgui PRIVATE SDL2)
target_link_libraries(polygon PRIVATE point)
target_link_libraries(main PRIVATE polygon)
target_link_libraries(main PRIVATE SDL2)
target_link_libraries(main PRIVATE window)
target_link_libraries(main PRIVATE Threads::Threads)

target_link_libraries(window PRIVATE SDL2)
target_link_libraries(window PRIVATE SDL2_gfx)
target_link_libraries(window PRIVATE SDL2_ttf)
target_link_libraries(window PRIVATE polygon)
target_link_libraries(window PRIVATE Threads::Threads)
target_link_libraries(window PRIVATE imgui) # Link your executable with ImGui
target_link_libraries(window PRIVATE GL)

