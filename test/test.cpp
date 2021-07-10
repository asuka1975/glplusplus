//
// Created by hungr on 2021/07/11.
//
#include <gtest/gtest.h>

#include <list>

#include "vertex_buffer.h"

#include <GLFW/glfw3.h>

TEST(BUFFER_READ, BUFFER_TEST) {
    std::vector<float> data { 1, 2, 3 };
    gl::vertex_buffer<gl::buffer_trait<float, GL_ARRAY_BUFFER, GL_STATIC_DRAW>> vbo(data.begin(), data.end());

    // read all buffer
    std::vector<float> buffer1(data.size());
    vbo.get(buffer1.begin(), buffer1.end());
    for(std::size_t i = 0; i < buffer1.size(); i++) {
        EXPECT_EQ(buffer1[i], data[i]);
    }

    // read a part of buffer
    std::vector<float> buffer2(data.size());
    vbo.get(1, buffer2.begin() + 1, buffer2.end());
    EXPECT_EQ(buffer2[0], 0);
    for(std::size_t i = 1; i < buffer2.size(); i++) {
        EXPECT_EQ(buffer1[i], data[i]);
    }

    // read over size buffer
    std::vector<float> buffer3(data.size() + 3);
    vbo.get(buffer3.begin(), buffer3.end());
    for(std::size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(buffer3[i], data[i]);
    }
    for(std::size_t i = 3; i < buffer3.size(); i++) {
        EXPECT_EQ(buffer3[i], 0);
    }

    // read mismatch area
    std::vector<float> buffer4(data.size());
    vbo.get(3, buffer4.begin(), buffer4.end());
    for(std::size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(buffer4[i], 0);
    }

    // read slided area
    std::vector<float> buffer5(data.size());
    vbo.get(2, buffer5.begin(), buffer5.end());
    EXPECT_EQ(buffer5[0], data[2]);
    EXPECT_EQ(buffer5[1], 0);
    EXPECT_EQ(buffer5[2], 0);

    // read to not random access iterator(NRA iterator)
    std::list<float> buffer6(data.size());
    vbo.get(buffer6.begin(), buffer6.end());
    for(std::size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(*std::next(buffer6.begin(), i), data[i]);
    }

    // read a part of buffer (NRA version)
    std::list<float> buffer7(data.size());
    vbo.get(1, std::next(buffer7.begin(), 1), buffer7.end());
    EXPECT_EQ(*buffer7.begin(), 0);
    for(std::size_t i = 1; i < buffer7.size(); i++) {
        EXPECT_EQ(*std::next(buffer7.begin(), i), data[i]);
    }
}

TEST(BUFFER_MODIFY, BUFFER_TEST) {
    std::vector<float> data { 1, 2, 3 };
    std::vector<float> buffer(3);

    gl::vertex_buffer<gl::buffer_trait<float, GL_ARRAY_BUFFER, GL_STATIC_DRAW>> vbo(data.begin(), data.end());
    // update whole buffer
    std::vector<float> modify1 { 2, 3, 4 };
    vbo.modify(modify1.begin(), modify1.end());
    vbo.get(buffer.begin(), buffer.end());
    for(std::size_t i = 0; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], modify1[i]);
    }

    vbo.modify(data.begin(), data.end());

    // update a part of buffer
    std::vector<float> modify2 { 3, 4 };
    vbo.modify(1, modify2.begin(), modify2.end());
    vbo.get(buffer.begin(), buffer.end());
    EXPECT_EQ(buffer[0], data[0]);
    for(std::size_t i = 1; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], modify2[i - 1]);
    }

    vbo.modify(data.begin(), data.end());

    // update slided buffer
    std::vector<float> modify3 { 3, 4, 5 };
    vbo.modify(1, modify3.begin(), modify3.end());
    vbo.get(buffer.begin(), buffer.end());
    EXPECT_EQ(buffer[0], data[0]);
    for(std::size_t i = 1; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], modify3[i - 1]);
    }

    vbo.modify(data.begin(), data.end());

    // update mismatch offset
    std::vector<float> modify4 { 3, 4, 5 };
    vbo.modify(3, modify4.begin(), modify4.end());
    vbo.get(buffer.begin(), buffer.end());
    for(std::size_t i = 0; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], data[i]);
    }
}

TEST(BUFFER_EXTEND, BUFFER_TEST) {
    std::vector<float> data { 1, 2, 3 };
    std::vector<float> buffer(6);
    gl::vertex_buffer<gl::buffer_trait<float, GL_ARRAY_BUFFER, GL_STATIC_DRAW>> vbo(data.begin(), data.end());

    std::vector<float> extend1 { 4, 5, 6 };
    vbo.extend(extend1.begin(), extend1.end());
    vbo.get(buffer.begin(), buffer.end());
    for(std::size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(buffer[i], data[i]);
        EXPECT_EQ(buffer[i + 3], extend1[i]);
    }
    EXPECT_EQ(vbo.size(), 8);

    std::vector<float> extend2 { 7, 8, 9 };
    std::vector<float> answer2 { 1, 2, 3, 4, 5, 7, 8, 9 };
    buffer.resize(answer2.size());
    vbo.extend(5, extend2.begin(), extend2.end());
    vbo.get(buffer.begin(), buffer.end());
    for(std::size_t i = 0; i < buffer.size(); i++) {
        EXPECT_EQ(buffer[i], answer2[i]);
    }
    EXPECT_EQ(vbo.size(), 8);
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