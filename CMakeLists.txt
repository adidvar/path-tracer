cmake_minimum_required(VERSION 3.2)
project(Opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(raytracing main.cpp render.cpp render.h figures.h figures.cpp materials.h materials.cpp)
target_link_libraries(raytracing ${CONAN_LIBS})
