cmake_minimum_required(VERSION 3.16)
project(gl++_test)

find_package(GTest REQUIRED)

add_executable(gl++_test test.cpp)
target_include_directories(gl++_test PRIVATE ${GTest_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(gl++_test GTest::GTest glfw GL GLEW)

enable_testing()