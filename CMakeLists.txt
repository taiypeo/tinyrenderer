cmake_minimum_required(VERSION 3.10)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(
    SOURCES
    tinyrenderer/main.cpp
    tinyrenderer/rendering/renderer.cpp
    tinyrenderer/rendering/model.cpp
    tinyrenderer/rendering/camera.cpp
    tinyrenderer/math/triangle.cpp
    tinyrenderer/math/linalg.cpp
)

include_directories(tinyrenderer)
add_executable(tinyrenderer.out ${SOURCES})
target_link_libraries(tinyrenderer.out -lsfml-graphics -lsfml-window -lsfml-system)
