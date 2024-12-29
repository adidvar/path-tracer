cmake_minimum_required(VERSION 3.2)
project(PathTracer)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tracer_perf main.cpp)    
target_link_libraries(tracer_perf tracer_lib Catch2::Catch2WithMain)