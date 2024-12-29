cmake_minimum_required(VERSION 3.2)

project(PathTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(lodepng REQUIRED)
find_package(OpenMP)

file(GLOB library_files
     "lib/*.hpp"
     "lib/*.cpp"
)

add_library(tracer_lib ${library_files})
target_include_directories(tracer_lib INTERFACE lib)
target_link_libraries(tracer_lib PUBLIC glm::glm PUBLIC lodepng::lodepng)

add_executable(tracer src/main.cpp)
target_link_libraries(tracer tracer_lib SDL2::SDL2 SDL2::SDL2main)
if(OpenMP_CXX_FOUND)
    target_link_libraries(tracer OpenMP::OpenMP_CXX)
endif()

add_custom_command(TARGET tracer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:tracer> ${CMAKE_SOURCE_DIR}/meshes/mesh.obj
  COMMAND_EXPAND_LISTS
)

add_subdirectory(perf)

install(TARGETS tracer DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/meshes/mesh.obj DESTINATION .)