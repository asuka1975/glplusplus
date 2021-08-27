cmake_minimum_required(VERSION 3.16)
project(gl++)

set(CMAKE_CXX_STANDARD 17)

add_library(gl++ src/vertex_buffer.cpp src/vertex_array.cpp src/shader.cpp include/gl++/gl++.h)

target_include_directories(gl++ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)