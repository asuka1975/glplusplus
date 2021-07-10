//
// Created by hungr on 2021/07/11.
//
#include <gtest/gtest.h>

#include "vertex_buffer.h"

#include <GLFW/glfw3.h>

TEST(BUFFER_READ, BUFFER_TEST) {
    std::vector<float> data { 1, 2, 3 };
    gl::vertex_buffer<gl::buffer_trait<float, GL_ARRAY_BUFFER, GL_STATIC_DRAW>> vbo(data.begin(), data.end());
    std::vector<float> buffer(data.size());
    vbo.get(buffer.begin(), buffer.end());
    for(std::size_t i = 0; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], data[i]);
    }


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if(glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("failed to initialize GLFW");
    }
    std::atexit(glfwTerminate);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto window = glfwCreateWindow(1, 1, "sample", nullptr, nullptr);
    if(window == nullptr) {
        throw std::runtime_error("failed to create GLFWwindow");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        throw std::runtime_error("failed to initialize GLEW");
    }

    auto ret = RUN_ALL_TESTS();
    glfwDestroyWindow(window);

    return ret;
}