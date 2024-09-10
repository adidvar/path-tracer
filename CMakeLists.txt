cmake_minimum_required(VERSION 3.2)

project(PathTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(COPY_FILE conanfile.txt ${CMAKE_BINARY_DIR}/conanfile.txt)
file(COPY_FILE D:/User/Desktop/raytracing/meshes/mesh.obj ${CMAKE_BINARY_DIR}/mesh.obj)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR};${CMAKE_PREFIX_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(lodepng REQUIRED)

file(GLOB files
     "include/PathTracer/*.hpp"
     "src/*.cpp"
)

add_executable(tracer ${files})
target_link_libraries(tracer glm::glm SDL2::SDL2 SDL2::SDL2main lodepng::lodepng)
target_include_directories(tracer PUBLIC include)
target_precompile_headers(tracer
   PUBLIC include/PathTracer/pch.hpp
   PUBLIC include/PathTracer/PathTracer.hpp)
