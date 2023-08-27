cmake_minimum_required(VERSION 3.2)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY_FILE conanfile.txt ${CMAKE_BINARY_DIR}/conanfile.txt)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR};${CMAKE_PREFIX_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(lodepng REQUIRED)

file(GLOB files
     "*.h"
     "*.cpp"
)

add_executable(raytracing ${files})
target_link_libraries(raytracing glm::glm SDL2::SDL2 SDL2::SDL2main lodepng::lodepng)
