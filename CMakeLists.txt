cmake_minimum_required(VERSION 3.2)

project(PathTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

add_custom_command(TARGET tracer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:tracer> ${CMAKE_SOURCE_DIR}/meshes/mesh.obj
  COMMAND_EXPAND_LISTS
)

install(TARGETS tracer DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/meshes/mesh.obj DESTINATION .)